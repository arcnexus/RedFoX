#include "frmconfigmaya.h"
#include "ui_frmconfigmaya.h"
#include <QtSql>
#include <QSqlDatabase>
#include "../configuracion.h"


//Configuracion * Configuracion_global = 0;

frmConfigmaya::frmConfigmaya(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmConfigmaya),
    menuButton(QIcon(":/Icons/PNG/Config.png"),tr("Configuracion General"),this)
{
    ui->setupUi(this);

    QFile f(qApp->applicationDirPath()+"/MayaConfig.ini");
    if(!f.exists())
        return;
    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
    ui->txtHost->setText(settings.value("cHostBDMaya").toString());
    ui->txtUsuario->setText(Configuracion::DeCrypt(settings.value("cUserBDMaya").toString()));
    ui->txtPassword->setText(Configuracion::DeCrypt(settings.value("cPasswordBDMaya").toString()));
    ui->txtPuerto->setText(settings.value("cPuertoDBMaya").toString());
    ui->txtdireccionBD->setText(settings.value("cRutaDBMaya").toString());

    ui->txtclaveV1_1->setText(settings.value("Clave1").toString());
    ui->txtclaveV1_2->setText(settings.value("Clave2").toString());
    ui->txtclaveV2_1->setText(settings.value("Clave3").toString());
    ui->txtclaveV2_2->setText(settings.value("Clave4").toString());
    ui->txtclaveV3_1->setText(settings.value("Clave5").toString());
    ui->txtclaveV3_2->setText(settings.value("Clave6").toString());
    ui->txtclaveV4_1->setText(settings.value("Clave7").toString());
    ui->txtclaveV4_2->setText(settings.value("Clave8").toString());

    ui->txtHostBD_MediTec->setText(settings.value("HostDB_MediTec").toString());
    ui->txtnombre_bd_MediTec->setText(settings.value("NombreDB_MediTec").toString());
    ui->txtUsuarioDB_MediTec->setText(settings.value("UsuarioDB_MediTec").toString());
    ui->txtPasswordDB_MediTec->setText(settings.value("PasswordDB_MediTec").toString());
    ui->txtPuerto_DB_MediTec->setText(settings.value("PuertoDB_MediTec").toString());
    if(settings.value("cDriverBDMaya").toString() =="QMYSQL")
        ui->radMysql->setChecked(true);
    else
        ui->radMysql->setChecked(false);


    ui->chkmedicum->setChecked(settings.value("medicum").toBool());
    ui->chkconta->setChecked(settings.value("conta").toBool());
    ui->chkVademecum_alopatico->setChecked(settings.value("vad_alop").toBool());
    ui->chkVademecum_homeopatia->setChecked(settings.value("vad_home").toBool());
    ui->chkVademecum_MTC->setChecked(settings.value("vad_MTC").toBool());
    ui->chk_vademecum_fitoterapia->setChecked(settings.value("vad_fito").toBool());
}

frmConfigmaya::~frmConfigmaya()
{
    delete ui;
}

void frmConfigmaya::configurar()
{
    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
    if(ui->radMysql->isChecked())
        settings.setValue("cDriverBDMaya","QMYSQL");
    else
        settings.setValue("cRutaDBMaya",ui->txtdireccionBD->text());

    settings.setValue("cHostBDMaya",ui->txtHost->text());
    settings.setValue("cUserBDMaya",Configuracion::Crypt(ui->txtUsuario->text()));
    settings.setValue("cPasswordBDMaya",Configuracion::Crypt(ui->txtPassword->text()));
    settings.setValue("cPuertoDBMaya",ui->txtPuerto->text());

    settings.setValue("Clave1",ui->txtclaveV1_1->text());
    settings.setValue("Clave2",ui->txtclaveV1_2->text());
    settings.setValue("Clave3",ui->txtclaveV2_1->text());
    settings.setValue("Clave4",ui->txtclaveV2_2->text());
    settings.setValue("Clave5",ui->txtclaveV3_1->text());
    settings.setValue("Clave6",ui->txtclaveV3_2->text());
    settings.setValue("Clave7",ui->txtclaveV4_1->text());
    settings.setValue("Clave8",ui->txtclaveV4_2->text());
    if(ui->chkmedicum->isChecked())
        settings.setValue("medicum",1);
    else
        settings.setValue("medicum",0);
    if(ui->chkconta->isChecked())
        settings.setValue("conta",1);
    else
        settings.setValue("conta",0);

    if(ui->chkVademecum_alopatico->isChecked())
        settings.setValue("vad_alop",1);
    else
        settings.setValue("vad_alop",0);
   if (ui->chkVademecum_homeopatia->isChecked())
       settings.setValue("vad_home",1);
   else
       settings.setValue("vad_home",0);
   if (ui->chkVademecum_MTC->isChecked())
       settings.setValue("vad_MTC",1);
   else
       settings.setValue("vad_MTC",0);
   if (ui->chk_vademecum_fitoterapia->isChecked())
       settings.setValue("vad_fito",1);
   else
       settings.setValue("vad_fito",0);

   settings.setValue("HostDB_MediTec",ui->txtHostBD_MediTec->text());
   settings.setValue("NombreDB_MediTec",ui->txtnombre_bd_MediTec->text());
   settings.setValue("UsuarioDB_MediTec",ui->txtUsuarioDB_MediTec->text());
   settings.setValue("PasswordDB_MediTec",ui->txtPasswordDB_MediTec->text());
   settings.setValue("PuertoDB_MediTec",ui->txtPuerto_DB_MediTec->text());
}

void frmConfigmaya::on_btnSiguiente_clicked()
{
    int index = ui->stackedWidget->currentIndex();
    switch (index) {
    case 0:
        doMenuSelection();
        break;
    case 1:
        configurar();
        if(!checkForMayaGlobal())
        {
            crear_MayaGlobal();
            ui->stackedWidget->setCurrentIndex(4);
            ui->btnCancel->setEnabled(false);
        }
        break;
    case 4:
        if(createSuperUser())
        {
            ui->stackedWidget->setCurrentIndex(0);
            ui->btnCancel->setEnabled(true);
        }
        break;
    }
}

bool frmConfigmaya::checkForMayaGlobal()
{
    if(!Configuracion_global->CargarDatosBD())
        if(Configuracion_global->globalDB.isOpen())
            return false;
    return true;
}

void frmConfigmaya::crear_MayaGlobal()
{
    QFile f(":/DB/DB/Global.sql");
    bool error = !f.open(QFile::ReadOnly);
    QSqlQuery q(Configuracion_global->globalDB);
    if(!error)
    {
        QString s = f.readAll();
        s.replace("\r\n"," ");

        QStringList querys = s.split(";",QString::SkipEmptyParts);

        for(int i = 0; i< querys.size();i++)
        {
            if(!(querys.at(i)== "\n") && !querys.at(i).isEmpty())
            {
                if(!q.exec(querys.at(i)))
                {
                    qDebug() << q.lastError().text();
                    qDebug() << querys.at(i);
                    error = true;
                    break;
                }
            }
        }
    }
    f.close();
}

void frmConfigmaya::doMenuSelection()
{
    if(!ui->listWidget->currentIndex().isValid())
        return;
    int index = ui->listWidget->currentIndex().row();
    switch (index) {
    case 0:
        ui->stackedWidget->setCurrentWidget(ui->bd_driver_page);
        break;
    case 1:
        ui->stackedWidget->setCurrentWidget(ui->create_user_page);
        break;
    default:        
        break;
    }
}

bool frmConfigmaya::createSuperUser()
{
    if(ui->txtname->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Nombre de usuario"),tr("Especifique un nombre de usuario"));
        return false;
    }
    if(ui->txtpass1->text() != ui->txtpass2->text())
    {
        QMessageBox::warning(this,tr("Revise sus contraseñas"),tr("Las contraseñas de usuario no coinciden"));
        return false;
    }
    if(ui->txtmailpass1->text() != ui->txtmailpass2->text())
    {
        QMessageBox::warning(this,tr("Revise sus contraseñas"),tr("Las contraseñas de e-mail no coinciden"));
        return false;
    }
    QHash<QString,QVariant> _data;
    _data["nombre"] = ui->txtname->text();
    _data["contrasena"] = Configuracion::SHA256HashString(ui->txtpass1->text());
    _data["cuenta_smtp"] = ui->txtsmtp->text();
    _data["usuario_mail"] = ui->txtmail->text();
    _data["password_mail"] = Configuracion::SHA256HashString(ui->txtmailpass1->text());

    QString error;
    if(SqlCalls::SqlInsert(_data,"mayaglobal`.`usuarios",Configuracion_global->globalDB,error)<0)
    {
        QMessageBox::critical(this,tr("Error al insertar Super Usuario"),error);
        return false;
    }
    return true;
}

void frmConfigmaya::on_btnInicio_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->welcome_page);
}
