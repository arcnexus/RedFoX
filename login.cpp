#include "login.h"
#include "ui_login.h"

#include "mainwindow.h"



#include "../Zona_Administrador/frmempresas.h"
#include "../Zona_Administrador/arearestringida_form.h"
#include "../Zona_Administrador/frmconfigmaya.h"
#include "mainwindow.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    //this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint/*|Qt::WindowStaysOnTopHint*/);
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

    if (Configuracion_global->cDriverBDMaya == "QSQLITE")
    {
        dbMaya.setDatabaseName(Configuracion_global->cRutaBdMaya);
        dbMaya.open();
    }
    else
    {
        dbMaya.setDatabaseName("MayaGeneral");
        dbMaya.setHostName(Configuracion_global->cHostBDMaya);
        dbMaya.open(Configuracion_global->cUsuarioBDMaya,Configuracion_global->cPasswordBDMaya);
        dbMaya.open(Configuracion_global->cUsuarioBDMaya,Configuracion_global->cPasswordBDMaya);
    }
    if (dbMaya.lastError().isValid())
    {
        QMessageBox::critical(0, "error:", dbMaya.lastError().text());
    }
    if(dbMaya.isOpen())
        Configuracion_global->CargarDatos();
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

int Login::getIdUser()
{
    return m_id;
}


void Login::on_btnAcceder_clicked()
{
    QSqlQuery qEmpresa(QSqlDatabase::database("Maya"));
    qEmpresa.prepare("select id from empresas where nombre = :nombreemp");
    qEmpresa.bindValue(":nombreemp",ui->cboEmpresa->currentText());
    if(qEmpresa.exec())
        Configuracion_global->idEmpresa = qEmpresa.record().field("id").value().toInt();
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
            qDebug() << Configuracion::SHA256HashString(ui->linePassword->text());
            qDebug() << qryUsers.value(2).toString();
            if (Configuracion::SHA256HashString(ui->linePassword->text()) == qryUsers.value(2).toString().trimmed())
			{
                m_id = qryUsers.value(0).toInt();
                QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
                settings.setValue("cUsuarioActivo",rUsuario.field("nombre").value().toString());
                settings.setValue("nNivelAcceso",rUsuario.field("nivelacceso").value().toInt());
                settings.setValue("cCategoria",rUsuario.field("categoria").value().toString());
                Configuracion_global->id_usuario_activo = this->getIdUser();
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
 frmConfigmaya frmConf;
 if(frmConf.exec()==QDialog::Accepted)
    TimedMessageBox * t = new TimedMessageBox(this,tr("Configuración inicial realizada con éxito"));
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
	// TODO - Rellenar en base a fichero de empresas BD Maya.
	// Relleno combo empresas

    //NOTE - Fixed pointer
    ui->cboEmpresa->clear();
    QScopedPointer<QSqlQuery>QryEmpresas(new QSqlQuery(QSqlDatabase::database("Maya")));
    //QSqlQuery *QryEmpresas = new QSqlQuery(QSqlDatabase::database("Maya"));

	QryEmpresas->prepare("Select * from empresas");
	if(QryEmpresas->exec()) 
	{
		while (QryEmpresas->next()) 
		{
			QSqlRecord rEmpresa = QryEmpresas->record();
			ui->cboEmpresa->addItem(rEmpresa.field("nombre").value().toString());
		}
	}
	this->ui->lineUsuario->setFocus();
    //Esto es por pereza mas que nada xD

	this->ui->lineUsuario->setText("marc");
	this->ui->linePassword->setText("patata");
   //this->ui->btnAcceder->click();

}
