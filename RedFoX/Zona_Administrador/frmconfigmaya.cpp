#include "frmconfigmaya.h"
#include "ui_frmconfigmaya.h"
#include <QtSql>
#include <QSqlDatabase>
#include "../configuracion.h"

#include "auxmodule.h"

frmConfigmaya::frmConfigmaya(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmConfigmaya),
    menuButton(QIcon(":/Icons/PNG/Config.png"),tr("Configuracion General"),this)
{
    ui->setupUi(this);
    model_grupos = new QSqlQueryModel(this);
    model_empresa= new QSqlQueryModel(this);

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


    model_grupos->setQuery("SELECT * from redfoxglobal.grupos",Configuracion_global->globalDB);



    ui->cbo_user_edit->setModel(Configuracion_global->usuarios_model);
    ui->cbo_user_edit->setModelColumn(1);
}

frmConfigmaya::~frmConfigmaya()
{
    delete ui;
    QSqlDatabase::database("temp_loadEmp").close();
    QSqlDatabase::removeDatabase("temp_loadEmp");
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
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(0);
        }
        break;
    case 4:
        if(createUser() && !ui->chk_createSuper->isChecked())
        {
            gotoEditPermisos();
            ui->cboUser_permiso->setCurrentIndex(ui->cboUser_permiso->findText(ui->txtname->text()));
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(0);
        }
        break;
    default:
        ui->stackedWidget->setCurrentIndex(0);
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
                    error = true;
                    break;
                }
            }
        }
    }
    f.close();

    if(error)
        QMessageBox::critical(this, tr("Error creando base de datos global"), q.lastError().text());
    else
        TimedMessageBox::Box(this,tr("Base de datos global creada con éxito"));
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
    case 2:
        ui->stackedWidget->setCurrentWidget(ui->edit_user_info);
        break;
    case 3:
        gotoEditPermisos();
        break;
    default:        
        break;
    }
}

bool frmConfigmaya::createUser()
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
    _data["password_mail"] = Configuracion::Crypt(ui->txtmailpass1->text());
    _data["nombre_completo"] = ui->txtNameComplete->text();
    _data["port_mail"] = ui->spinSmtpPort->value();
    _data["super_user"] = ui->chk_createSuper->isChecked();

    QString error;
    int id = SqlCalls::SqlInsert(_data,"redfoxglobal`.`usuarios",Configuracion_global->globalDB,error);
    if(id < 0)
    {
        QMessageBox::critical(this,tr("Error al insertar Super Usuario"),error);
        return false;
    }
    else
    {
        int lvl = ui->chk_createSuper->isChecked() ? 7 : 1;
        QString error;
        QHash<QString,QVariant> _dato_acceso;
        _dato_acceso["id_user"] = id;
        _dato_acceso["id_nivel_acceso"] = lvl;
        for(auto i=0; i < model_grupos->rowCount(); i++)
        {
            QSqlRecord r = model_grupos->record(i);

            QSqlDatabase db = QSqlDatabase::addDatabase(r.value("bd_driver").toString(),"temp_savePermiso");
            db.setUserName    (r.value("bd_user").toString());
            db.setPassword    (r.value("bd_pass").toString());
            db.setPort        (r.value("bd_port").toInt());
            if(db.open())
            {
                QString emps = QString("%1.empresas").arg(r.value("bd_name").toString());
                QStringList bds = SqlCalls::SelectList(emps,"nombre_bd","id>0",db,error);
                QStringListIterator _it(bds);
                while(_it.hasNext())
                {
                    QString empresa_bd = _it.next();
                    QString table_modulos = QString("%1.modulos").arg(empresa_bd);
                    QString table_acceso = QString("%1`.`accesousuarios").arg(empresa_bd);
                    QSqlQuery q(db);
                    q.exec(QString("SELECT id from %1").arg(table_modulos));
                    while(q.next())
                    {
                        _dato_acceso["id_modulo"] = q.record().value("id");
                        if(!SqlCalls::SqlInsert(_dato_acceso,table_acceso,db,error))
                        {
                            QMessageBox::critical(this,tr("Error al insertar acceso"),error);
                            return false;
                        }
                    }
                }
            }
            db.close();
            QSqlDatabase::removeDatabase("temp_savePermiso");
        }
    }
    Configuracion_global->usuarios_model->setQuery("SELECT * from redfoxglobal.usuarios",Configuracion_global->globalDB);
    ui->cbo_user_edit->setModel(Configuracion_global->usuarios_model);
    ui->cbo_user_edit->setModelColumn(1);

    ui->txtname->clear();
    ui->txtpass1->clear();
    ui->txtpass2->clear();
    ui->txtsmtp->clear();
    ui->txtmail->clear();
    ui->txtmailpass1->clear();
    ui->txtmailpass2->clear();
    ui->txtNameComplete->clear();
    ui->spinSmtpPort->setValue(445);

    TimedMessageBox::Box(this, tr("Usuario creado con éxito"));
    return true;
}

void frmConfigmaya::on_btnInicio_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->welcome_page);
}

void frmConfigmaya::gotoEditPermisos()
{
    ui->cboUser_permiso->setModel(Configuracion_global->usuarios_model);
    ui->cboUser_permiso->setModelColumn(1);

    ui->cboGrupo_permiso->setModel(model_grupos);
    ui->cboGrupo_permiso->setModelColumn(1);

    ui->cboEmpresa_permiso->setModel(model_empresa);
    ui->cboEmpresa_permiso->setModelColumn(0);

    ui->chkSuperUser_permiso->setChecked(Configuracion_global->usuarios_model->record(ui->cboUser_permiso->currentIndex()).value("super_user").toBool());
    ui->stackedWidget->setCurrentWidget(ui->edit_user_permiso);
}

void frmConfigmaya::on_cboGrupo_permiso_currentIndexChanged(int index)
{
    QSqlRecord r = model_grupos->record(index);
    QSqlDatabase db = QSqlDatabase::addDatabase(r.value("bd_driver").toString(),"temp_loadEmp");
    db.setUserName    (r.value("bd_user").toString());
    db.setPassword    (r.value("bd_pass").toString());
    db.setPort        (r.value("bd_port").toInt());
    if(db.open())
    {
        QString emps = QString("SELECT nombre,nombre_bd from %1.empresas").arg(r.value("bd_name").toString());
        model_empresa->setQuery(emps,db);
    }
    ui->cboEmpresa_permiso->setCurrentIndex(0);
}

void frmConfigmaya::on_cboEmpresa_permiso_currentIndexChanged(int index)
{
    QSqlRecord r = model_empresa->record(index);
    getModulesFromDB(r.value("nombre_bd").toString());
}

void frmConfigmaya::getModulesFromDB(QString bd_name)
{
    QVectorIterator<AuxModule*> _it(_modulos);
    while(_it.hasNext())
        delete _it.next();
    _modulos.clear();

    QSqlQuery q(QSqlDatabase::database("temp_loadEmp"));
    q.exec(QString("SELECT * FROM %1.modulos;").arg(bd_name));
    while(q.next())
    {
        AuxModule* m = new AuxModule(this,q.record().value(2).toString());
        m->id = q.record().value(0).toInt();
        MayaModule::module_zone z = static_cast<MayaModule::module_zone>(q.record().value(1).toInt());
        m->zone = z;
        switch (z) {
        case MayaModule::Mantenimiento:
            addMantenModule(m);
            break;
        case MayaModule::NoZone:
            break;
        case MayaModule::Contabilidad:
            addContaModule(m);
            break;
        case MayaModule::SecretariaMedica:
            addSecMedicModule(m);
            break;
        case MayaModule::InformacionMedica:
            addInfoMedicModule(m);
            break;
        case MayaModule::Compras:
            addComprasModule(m);
            break;
        case MayaModule::Ventas:
            addVentasModule(m);
            break;
        case MayaModule::Archivos:
            addArchivosModule(m);
            break;
        case MayaModule::Almacen:
            addAlmacenModule(m);
            break;
        case MayaModule::Utilidades:
            addUtilidadesModule(m);
            break;
        case MayaModule::AdminZone :
            addAdminModule(m);
            break;

        default:
            addNoZoneModule(m);
            break;
        }
        _modulos.append(m);
    }

    addMantenModule(0);
    addComprasModule(0);
    addVentasModule(0);
    addArchivosModule(0);
    addAlmacenModule(0);
    addUtilidadesModule(0);
    addNoZoneModule(0);
    addAdminModule(0);
    addContaModule(0);
    addSecMedicModule(0);
    addInfoMedicModule(0);

    int id_user = Configuracion_global->usuarios_model->record(ui->cboUser_permiso->currentIndex()).value("id").toInt();
    llenarModulos(bd_name,id_user);
}
void frmConfigmaya::addAdminModule(AuxModule * m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_admin->layout())
        QGridLayout *l= new QGridLayout(ui->tab_admin);

    if(m)
    {
        ((QGridLayout *)ui->tab_admin->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_admin->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void frmConfigmaya::addMantenModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_manten->layout())
        QGridLayout *l= new QGridLayout(ui->tab_manten);

    if(m)
    {
        ((QGridLayout *)ui->tab_manten->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_manten->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void frmConfigmaya::addComprasModule(AuxModule * m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_compras->layout())
        QGridLayout *l= new QGridLayout(ui->tab_compras);

    if(m)
    {
        ((QGridLayout *)ui->tab_compras->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_compras->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void frmConfigmaya::addVentasModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_ventas->layout())
        QGridLayout *l= new QGridLayout(ui->tab_ventas);

    if(m)
    {
        ((QGridLayout *)ui->tab_ventas->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_ventas->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void frmConfigmaya::addArchivosModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_archivos->layout())
        QGridLayout *l= new QGridLayout(ui->tab_archivos);

    if(m)
    {
        ((QGridLayout *)ui->tab_archivos->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_archivos->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void frmConfigmaya::addAlmacenModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_almacen->layout())
        QGridLayout *l= new QGridLayout(ui->tab_almacen);

    if(m)
    {
        ((QGridLayout *)ui->tab_almacen->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_almacen->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void frmConfigmaya::addUtilidadesModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_utils->layout())
        QGridLayout *l= new QGridLayout(ui->tab_utils);

    if(m)
    {
        ((QGridLayout *)ui->tab_utils->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_utils->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void frmConfigmaya::addNoZoneModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_noZone->layout())
        QGridLayout *l= new QGridLayout(ui->tab_noZone);

    if(m)
    {
        ((QGridLayout *)ui->tab_noZone->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_noZone->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void frmConfigmaya::addContaModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_conta->layout())
        QGridLayout *l= new QGridLayout(ui->tab_conta);

    if(m)
    {
        ((QGridLayout *)ui->tab_conta->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_conta->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void frmConfigmaya::addSecMedicModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_secMed->layout())
        QGridLayout *l= new QGridLayout(ui->tab_secMed);

    if(m)
    {
        ((QGridLayout *)ui->tab_secMed->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_secMed->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void frmConfigmaya::addInfoMedicModule(AuxModule *m)
{
    static int row = 0;
    static int column = 0;

    if(!ui->tab_med->layout())
        QGridLayout *l= new QGridLayout(ui->tab_med);

    if(m)
    {
        ((QGridLayout *)ui->tab_med->layout())->addWidget(m,row,column);

        if(column==0)
        {
            column++;
        }
        else
        {
            row++;
            column=0;
        }
    }
    else
    {
        ((QGridLayout *)ui->tab_med->layout())->addItem(new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Expanding),row+1,2);
        column = 0;
    }
}

void frmConfigmaya::llenarModulos(QString bd_name,int id_user)
{
    QSqlQuery q(QSqlDatabase::database("temp_loadEmp"));
    q.prepare(QString("SELECT * FROM %1.accesousuarios WHERE id_user=:id;").arg(bd_name));
    q.bindValue(":id",id_user);
    q.exec();
    while(q.next())
    {
        int id_modulo = q.record().value(2).toInt();
        int idAcceso = q.record().value(3).toInt();
        QVector<AuxModule*>::Iterator i;
        for(i=_modulos.begin();i!=_modulos.end();++i)
        {
            if((*i)->id == id_modulo)
            {                
                (*i)->id_user = id_user;
                (*i)->tabla = QString("%1.accesousuarios").arg(bd_name);
                (*i)->setNivel(idAcceso);
                break;
            }
        }
    }
}

void frmConfigmaya::on_cboUser_permiso_currentIndexChanged(int index)
{
    QSqlRecord r = model_empresa->record(ui->cboEmpresa_permiso->currentIndex());
    int id_user = Configuracion_global->usuarios_model->record(ui->cboUser_permiso->currentIndex()).value("id").toInt();
    llenarModulos(r.value("nombre_bd").toString(),id_user);
    ui->chkSuperUser_permiso->setChecked(r.value("super_user").toBool());
}

void frmConfigmaya::on_chkSuperUser_permiso_toggled(bool checked)
{
    ui->tabWidget_4->setEnabled(!checked);
    int id_user = Configuracion_global->usuarios_model->record(ui->cboUser_permiso->currentIndex()).value("id").toInt();
    QHash<QString,QVariant> _data;
    _data["super_user"] = checked;
    QString error;
    if(!SqlCalls::SqlUpdate(_data,"redfoxglobal`.`usuarios",Configuracion_global->globalDB,QString("id = %1").arg(id_user),error))
        QMessageBox::critical(this,tr("Error"),error);
}

void frmConfigmaya::on_cbo_user_edit_currentIndexChanged(int index)
{
    QSqlRecord r = Configuracion_global->usuarios_model->record(index);
    ui->txtname_edit->setText(r.value("nombre").toString());
    ui->txtNameComplete_edit->setText(r.value("nombre_completo").toString());
    ui->txtpass1_edit->setText(r.value("contrasena").toString());
    ui->txtsmtp_edit->setText(r.value("cuenta_smtp").toString());
    ui->spinSmtpPort_edit->setValue(r.value("port_mail").toInt());
    ui->txtmail_edit->setText(r.value("usuario_mail").toString());
    ui->txtmailpass1_edit->setText(r.value("password_mail").toString());
    ui->chk_Super_edit->setChecked(r.value("super_user").toBool());
}

void frmConfigmaya::on_btnGuardarEdit_clicked()
{
    QSqlRecord r = Configuracion_global->usuarios_model->record(ui->cbo_user_edit->currentIndex());
    int id = r.value("id").toInt();
    QString error;

    QHash<QString,QVariant> _data;
    if(r.value("nombre").toString() != ui->txtname_edit->text())
        _data["nombre"] = ui->txtname_edit->text();

    if(r.value("nombre_completo").toString() != ui->txtNameComplete_edit->text())
        _data["nombre_completo"] = ui->txtNameComplete_edit->text();

    if(r.value("contrasena").toString() != ui->txtpass1_edit->text())
        _data["contrasena"] = Configuracion::SHA256HashString(ui->txtpass1_edit->text());

    if(r.value("cuenta_smtp").toString() != ui->txtsmtp_edit->text())
        _data["cuenta_smtp"] = ui->txtsmtp_edit->text();

    if(r.value("usuario_mail").toString() != ui->txtmail_edit->text())
        _data["usuario_mail"] = ui->txtmail_edit->text();

    if(r.value("password_mail").toString() != ui->txtmailpass1_edit->text())
        _data["password_mail"] = Configuracion::Crypt(ui->txtmailpass1_edit->text());

    if(r.value("port_mail").toInt() != ui->spinSmtpPort_edit->value())
        _data["port_mail"] = ui->spinSmtpPort_edit->value();

    if(r.value("super_user").toInt() != ui->chk_Super_edit->isChecked())
        _data["super_user"] = ui->chk_Super_edit->isChecked();

    if(!SqlCalls::SqlUpdate(_data,"redfoxglobal`.`usuarios",Configuracion_global->globalDB,QString("id = %1").arg(id),error))
        QMessageBox::critical(this,tr("Error al actualizar datos de usuario"),error);
    else
        TimedMessageBox::Box(this,tr("Datos actualizados con éxito."));
}
