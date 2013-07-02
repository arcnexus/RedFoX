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
    //qDebug() << Configuracion::SHA256HashString("admin");
    qDebug() << Configuracion::SHA256HashString("patata");
    //this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint|Qt::WindowStaysOnTopHint);
    ui->setupUi(this);
    //--------------------------------------------
    // Conexiones
    //--------------------------------------------
    connect(ui->btnEmpresa,SIGNAL(clicked()),this,SLOT(btnEmpresa_clicked()));
    connect(ui->Crearconfiguracin,SIGNAL(clicked()),this,SLOT(Crearconfiguracion_clicked()));
    if (! QFile::exists(qApp->applicationDirPath()+"/MayaConfig.ini")){
        Crearconfiguracion_clicked();
    }

    if(!Configuracion_global)
        Configuracion_global = new Configuracion;
    Configuracion_global->CargarDatosBD();

    QSqlDatabase dbMaya  = QSqlDatabase::addDatabase(Configuracion_global->cDriverBDMaya,"Maya");
    QSqlDatabase dbReports = QSqlDatabase::addDatabase(Configuracion_global->cDriverBDMaya,"Reports");

    if (Configuracion_global->cDriverBDMaya == "QSQLITE")
    {
        dbReports.setDatabaseName(Configuracion_global->cRutaBdMaya);
        dbReports.open();
        dbMaya.setDatabaseName(Configuracion_global->cRutaBdMaya);
        dbMaya.open();
    }
    else
    {
        dbMaya.setDatabaseName("maya_global");
        dbMaya.setHostName(Configuracion_global->cHostBDMaya);
        dbReports.setHostName(Configuracion_global->cHostBDMaya);
        dbMaya.open(Configuracion_global->cUsuarioBDMaya,Configuracion_global->cPasswordBDMaya);
        dbReports.open(Configuracion_global->cUsuarioBDMaya,Configuracion_global->cPasswordBDMaya);
    }
    if (dbMaya.lastError().isValid())
    {
        QMessageBox::critical(0, "error:", dbMaya.lastError().text());
    }
    //if(dbMaya.isOpen())
    //    Configuracion_global->CargarDatos();
    init();
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
    QSqlDatabase db = QSqlDatabase::database("Maya");
    db.close();

    QSqlRecord rEmpresa = _empresas.value(ui->comboGroup->currentIndex()).second;

    QString bd_driver = rEmpresa.value("bd_driver").toString();
    QString bd_name = rEmpresa.value("bd_name").toString();
    QString bd_host = rEmpresa.value("bd_host").toString();
    QString bd_user = rEmpresa.value("bd_user").toString();
    QString bd_pass = rEmpresa.value("bd_pass").toString();
    int bd_port = rEmpresa.value("bd_port").toInt();

    db = QSqlDatabase::addDatabase(bd_driver , "Maya");
    db.setHostName(bd_host);
    db.setUserName(bd_user);
    db.setPassword(bd_pass);
    db.setPort(bd_port);
    db.setDatabaseName(bd_name);
    db.open();

    QSqlQuery qEmpresa(QSqlDatabase::database("Maya"));
    qEmpresa.prepare("select id, nombre_bd from empresas where nombre = :nombreemp");
    qEmpresa.bindValue(":nombreemp",ui->cboEmpresa->currentText());
    if(qEmpresa.exec())
    {
        Configuracion_global->idEmpresa = qEmpresa.record().field("id").value().toInt();
        Configuracion_global->nombre_bd_empresa = qEmpresa.record().field("nombre_bd").value().toString();
    }
    else
        QMessageBox::warning(this,tr("ABRIR EMPRESA"),tr("No se encuentra la empresa"),tr("Aceptar"));


    QSqlQuery qryUsers(QSqlDatabase::database("Maya"));

    qryUsers.prepare( "SELECT * FROM usuarios where nombre =:Nombre" );
    qryUsers.bindValue(":Nombre",ui->lineUsuario->text());
    //qryUsers.prepare("Select * from usuarios where nombre = '"+ui->lineUsuario->text.trimmed()+"'");

    if( !qryUsers.exec() ) 
	{
        QMessageBox::critical(qApp->activeWindow(), "error:", qryUsers.lastError().text());
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
    if(check.es_valido())
    {
        frmConfigmaya frmConf;
        frmConf.hideButton();
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
        formEmpresa.hideButton();
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
    ui->cboEmpresa->clear();
    QScopedPointer<QSqlQuery>QryEmpresas(new QSqlQuery(QSqlDatabase::database("Maya")));
    //QSqlQuery *QryEmpresas = new QSqlQuery(QSqlDatabase::database("Maya"));

    QryEmpresas->prepare("Select * from grupos");

    int row = 0;
	if(QryEmpresas->exec()) 
	{
		while (QryEmpresas->next()) 
		{
			QSqlRecord rEmpresa = QryEmpresas->record();            
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

            QString query = QString("SELECT * FROM %1.empresas;").arg(bd_name);
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
    //Esto es por pereza mas que nada xD

	this->ui->lineUsuario->setText("marc");
	this->ui->linePassword->setText("patata");
   //this->ui->btnAcceder->click();
}

void Login::on_pushButton_2_clicked()
{
    AreaRestringida_form check(this);
    check.exec();
    if(check.es_valido())
    {
        FrmUsuarios formUsers(this);
        formUsers.hideButton();
        formUsers.setWindowState(Qt::WindowMaximized);
        formUsers.exec();
    }
}

void Login::on_comboGroup_currentIndexChanged(int index)
{
    ui->cboEmpresa->clear();
    ui->cboEmpresa->addItems(_empresas.value(index).first);
}
