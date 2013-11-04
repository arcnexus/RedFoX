#include "login.h"
#include "ui_login.h"

#include "mainwindow.h"



#include "../Zona_Administrador/frmempresas.h"
#include "../Zona_Administrador/arearestringida_form.h"
#include "../Zona_Administrador/frmconfigmaya.h"
#include "../Zona_Administrador/frmusuarios.h"
#include "mainwindow.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{  
    ui->setupUi(this);

    connect(ui->btnEmpresa,SIGNAL(clicked()),this,SLOT(btnEmpresa_clicked()));
    connect(ui->Crearconfiguracin,SIGNAL(clicked()),this,SLOT(Crearconfiguracion_clicked()));

    if (! QFile::exists(qApp->applicationDirPath()+"/MayaConfig.ini")){
        frmConfigmaya frmConf;
        if(frmConf.exec()==QDialog::Accepted)
            TimedMessageBox * t = new TimedMessageBox(this,tr("Configuración inicial realizada con éxito"));
    } else
    {
        QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
        QString nombreusuario = settings.value("cUsuarioActivo").toString();
        QString contrasena = Configuracion_global->DeCrypt(settings.value("contrasenaactiva").toString());
        Configuracion_global->caja = settings.value("cajaactiva").toString();
        ui->lineUsuario->setText(nombreusuario);
        ui->linePassword->setText(contrasena);
    }

    if(Configuracion_global->CargarDatosBD())
        init();
//    QString a = "RedFoX";
//    QString b = Configuracion::Crypt(a);
//    QString c = Configuracion::DeCrypt(b);

//    qDebug() << b;
//    qDebug() << c;
}

Login::~Login()
{
    delete ui;

    //dbMaya.close();
}

const QString Login::getUsuario() const
{
    return ui->lineUsuario->text();
}

const QString Login::getPass() const
{
    return ui->linePassword->text();
}

const QString Login::getEmpresa() const
{
    return ui->cboEmpresa->currentText();
}

int Login::getid_user()
{
    return m_id;
}


void Login::on_btnAcceder_clicked()
{
    QSqlRecord rEmpresa = _empresas.value(ui->comboGroup->currentIndex()).second;
    Configuracion_global->idEmpresa = rEmpresa.value("id").toInt();
    QString bd_driver = rEmpresa.value("bd_driver").toString();    
    QString bd_name = rEmpresa.value("bd_name").toString();    
    QString bd_host = rEmpresa.value("bd_host").toString();
    QString bd_user = rEmpresa.value("bd_user").toString();
    QString bd_pass = rEmpresa.value("bd_pass").toString();
    int bd_port = rEmpresa.value("bd_port").toInt();


    Configuracion_global->group_host = bd_host;
    Configuracion_global->group_user = bd_user;
    Configuracion_global->group_pass = bd_pass;
    Configuracion_global->group_port = bd_port;
    Configuracion_global->group_Driver = bd_driver;

    Configuracion_global->group_DBName = bd_name;
    Configuracion_global->groupDB = QSqlDatabase::addDatabase(bd_driver , "Maya");
    Configuracion_global->reportsDB = QSqlDatabase::addDatabase(bd_driver,"Reports");

    Configuracion_global->groupDB.setHostName(bd_host);
    Configuracion_global->groupDB.setUserName(bd_user);
    Configuracion_global->groupDB.setPassword(bd_pass);
    Configuracion_global->groupDB.setPort(bd_port);
    Configuracion_global->groupDB.setDatabaseName(bd_name);
    if(!Configuracion_global->groupDB.open())
    {
        QMessageBox::critical(this,tr("Error"),Configuracion_global->groupDB.lastError().text());
        return;
    }

    Configuracion_global->reportsDB.setHostName(bd_host);
    Configuracion_global->reportsDB.setUserName(bd_user);
    Configuracion_global->reportsDB.setPassword(bd_pass);
    Configuracion_global->reportsDB.setPort(bd_port);
    Configuracion_global->reportsDB.open();
    if(!Configuracion_global->reportsDB.open())
    {
        QMessageBox::critical(this,tr("Error"),Configuracion_global->reportsDB.lastError().text());
        return;
    }


    QSqlQuery qEmpresa(Configuracion_global->groupDB);
    qEmpresa.prepare("select * from empresas where nombre = :nombreemp");
    qEmpresa.bindValue(":nombreemp",ui->cboEmpresa->currentText());

    if(qEmpresa.exec())
    {
        if(qEmpresa.next())
        {
            Configuracion_global->nombre_bd_empresa = qEmpresa.record().value("nombre_bd").toString();
            Configuracion_global->nombre_bd_conta = qEmpresa.record().value("nombre_db_conta").toString();
            //qDebug() << Configuracion_global->nombre_bd_conta;
            Configuracion_global->nombre_bd_medica = qEmpresa.record().value("nombre_bd_medica").toString();
            Configuracion_global->decimales = qEmpresa.record().value("decimales").toInt();
            Configuracion_global->decimales_campos_totales = qEmpresa.record().value("decimales_campos_totales").toInt();
            Configuracion_global->importado_sp = qEmpresa.record().value("importada_sp").toBool();
            Configuracion_global->porc_irpf = qEmpresa.record().value("porc_irpf").toFloat();

        }
        else
            QMessageBox::warning(this,tr("ABRIR EMPRESA"),tr("No se encuentra la empresa"),tr("Aceptar"));
    }

    QSqlQuery qryUsers(Configuracion_global->groupDB);

    qryUsers.prepare( "SELECT * FROM usuarios where nombre =:Nombre" );
    qryUsers.bindValue(":Nombre",ui->lineUsuario->text());

    if( !qryUsers.exec() ) 
	{
        QMessageBox::critical(qApp->activeWindow(), "Error:", qryUsers.lastError().text());
    } 
	else 
	{
       if (qryUsers.next()) 
	   {
            QSqlRecord rUsuario = qryUsers.record();
            if (Configuracion::SHA256HashString(ui->linePassword->text()) == qryUsers.value(2).toString().trimmed())
			{
                m_id = qryUsers.value(0).toInt();
                QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
                settings.setValue("cUsuarioActivo",rUsuario.field("nombre").value().toString());
                settings.setValue("nNivelAcceso",rUsuario.field("nivelacceso").value().toInt());
                settings.setValue("cCategoria",rUsuario.field("categoria").value().toString());
                Configuracion_global->id_usuario_activo = this->getid_user();
                Login::done(QDialog::Accepted);
            }
			else 
			{
                QMessageBox::critical(this,"ACCESO DENEGADO","El usuario y la contraseña no se corresponden\n\n Verifique los datos");
                ui->linePassword->setText("");
                ui->linePassword->setFocus();
            }
       } 
	   else
	   {
           QMessageBox::critical(this,"Error","No existe ningún usuario con este nombre");
           ui->lineUsuario->setText("");
           ui->lineUsuario->setFocus();
       }
    }
}

void Login::Crearconfiguracion_clicked()
{
    AreaRestringida_form check(this);
    check.exec();
    if(check.es_valido())    {
        frmConfigmaya frmConf;
        if(frmConf.exec()==QDialog::Accepted)
            TimedMessageBox * t = new TimedMessageBox(this,tr("Configuración inicial realizada con éxito"));
    }
}

void Login::btnEmpresa_clicked()
{
    AreaRestringida_form check(this);
    check.exec();
    if(check.es_valido())
    {
        FrmEmpresas formEmpresa(this);
        formEmpresa.setWindowState(Qt::WindowMaximized);        
        formEmpresa.exec();
        init();
    }
}

void Login::on_pushButton_clicked()
{
	this->hide();
}

void Login::init()
{
    ui->comboGroup->clear();
    _empresas.clear();

    QSqlQuery QryEmpresas(QSqlDatabase::database("Global"));

    QryEmpresas.prepare("Select * from grupos");

    int row = 0;
    if(QryEmpresas.exec())
	{
        while (QryEmpresas.next())
		{
            QSqlRecord rEmpresa = QryEmpresas.record();
            ui->comboGroup->addItem(rEmpresa.field("nombre").value().toString());

            QString bd_driver = rEmpresa.value("bd_driver").toString();
            QString bd_name = rEmpresa.value("bd_name").toString();
            QString bd_host = rEmpresa.value("bd_host").toString();
            QString bd_user = rEmpresa.value("bd_user").toString();
            QString bd_pass = rEmpresa.value("bd_pass").toString();
            int bd_port = rEmpresa.value("bd_port").toInt();

            QSqlDatabase db = QSqlDatabase::addDatabase(bd_driver);
            db.setHostName(bd_host);
            db.setUserName(bd_user);
            db.setPassword(bd_pass);
            db.setPort(bd_port);
            db.setDatabaseName(bd_name);

            if(!db.open())
                continue;

            QString query = QString("SELECT * FROM `%1`.empresas;").arg(bd_name);
            QStringList empresas;

            QSqlQuery q(db);
            q.exec(query);

            while(q.next())
            {
                empresas.append(q.record().value("nombre").toString());
            }
            QPair<QStringList , QSqlRecord> par;
            par.first = empresas;
            par.second = rEmpresa;
            _empresas.insert(row,par);
            row++;
		}
	}
    ui->cboEmpresa->clear();
    ui->cboEmpresa->addItems(_empresas.value(0).first);
	this->ui->lineUsuario->setFocus();

   // this->ui->lineUsuario->setText("marc");
    //this->ui->linePassword->setText("patata");
}

void Login::on_pushButton_2_clicked()
{
    AreaRestringida_form check(this);
    check.exec();
    if(check.es_valido())
    {
        FrmUsuarios formUsers(this);
        formUsers.setWindowState(Qt::WindowMaximized);
        formUsers.exec();
    }
}

void Login::on_comboGroup_currentIndexChanged(int index)
{
    ui->cboEmpresa->clear();
    ui->cboEmpresa->addItems(_empresas.value(index).first);
}
