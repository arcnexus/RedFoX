#include "login.h"
#include "ui_login.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QErrorMessage>
#include <QMessageBox>
#include<QSqlError>
#include "mainwindow.h"
#include <QDir>
#include <QDebug>
#include "configuracion.h"
#include <QSqlRecord>
#include <QSqlField>
#include "frmempresas.h"

//NOTE - TheFox :: Archivo Revisado
#include <QtCore/QTimer>

Login::Login(Configuracion *m_config,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    ui->setupUi(this);
    //--------------------------------------------
    // Conexiones
    //--------------------------------------------
    connect(ui->btnEmpresa,SIGNAL(clicked()),this,SLOT(btnEmpresa_clicked()));
    connect(ui->Crearconfiguracin,SIGNAL(clicked()),this,SLOT(Crearconfiguracion_clicked()));

	QTimer::singleShot(0,this,SLOT(init()));
}

Login::~Login()
{
    delete ui;
    //dbTerra.close();
}

const QString Login::getUsuario() const
{
    return ui->lineUsuario->text();
}

const QString Login::getEmpresa() const
{
    return ui->cboEmpresa->currentText();
}


void Login::on_btnAcceder_clicked()
{
    QSqlQuery qryUsers(QSqlDatabase::database("terra"));

    qryUsers.prepare( "SELECT * FROM usuarios where nombre =:Nombre" );
    qryUsers.bindValue(":Nombre",ui->lineUsuario->text());
    //qryUsers.prepare("Select * from usuarios where nombre = '"+ui->lineUsuario->text.trimmed()+"'");

    if( !qryUsers.exec() ) 
	{
        QMessageBox::critical(NULL, "error:", qryUsers.lastError().text());
    } 
	else 
	{
       if (qryUsers.next()) 
	   {
            QSqlRecord rUsuario = qryUsers.record();
            if (ui->linePassword->text() == qryUsers.value(2).toString()) 
			{
                QSettings settings("infint", "terra");
                settings.setValue("cUsuarioActivo",rUsuario.field("nombre").value().toString());
                settings.setValue("nNivelAcceso",rUsuario.field("nivelacceso").value().toInt());
                settings.setValue("cCategoria",rUsuario.field("categoria").value().toString());
                Login::done(QDialog::Accepted);
            }
			else 
			{
                QMessageBox::critical(0,"ACCESO DENEGADO","El usuario y la contraseña no se corresponden\n\n Verifique los datos");
                ui->linePassword->setText("");
                ui->linePassword->setFocus();
            }
       } 
	   else
	   {
           QMessageBox::critical(0,"Error","No existe ningún usuario con este nombre");
           ui->lineUsuario->setText("");
           ui->lineUsuario->setFocus();
       }
    }
}

void Login::Crearconfiguracion_clicked()
{
    //NOTE - Hacer esto por defecto la primera vez que se ejecute Terra?

    QSettings settings("infint", "terra");
    settings.setValue("cDriverBDTerra","QSQLITE");
    settings.setValue("cRutaDBTerra",qApp->applicationDirPath()+"/DB/terra.sqlite");

    QDir directorioBd(qApp->applicationDirPath()+"/DB");
    if(!directorioBd.exists())
    {
        QDir path(qApp->applicationDirPath());
        path.mkdir(qApp->applicationDirPath()+"/DB");
    }

    QFile bd(qApp->applicationDirPath()+"/DB/terra.sqlite");
    if(!bd.exists())
    {
       //TODO - Descargar terra.sqlite del servidor??
    }

    settings.setValue("cHostBDTerra","localhost");
    settings.setValue("cUserBDTerra","root");
    settings.setValue("cPasswordBDTerra","PatataBullida_99");
    settings.setValue("cPais","España");
    settings.setValue("cEjercicioActivo","2012");
    settings.setValue("nDigitosFactura",5);
    settings.setValue("nIVA1",21);
    settings.setValue("nIVA2",10);
    settings.setValue("nIVA3",4);
    settings.setValue("nIVA4",0);
    settings.setValue("nRE1",5.2);
    settings.setValue("nRE2",1.4);
    settings.setValue("nRE3",0.5);
    settings.setValue("nRE4",0);
    settings.setValue("lProfesional",1);
    settings.setValue("nIRPF",21);
    settings.setValue("cSerie","2012");
    settings.setValue("nDigitosCuentas",0);
    settings.setValue("cCuentaClientes","430");
    settings.setValue("cCuentaProveedores","400");
    settings.setValue("cCuentaAcreedores","410");
    settings.setValue("cUsuarioActivo","");
    settings.setValue("nNivelAcceso",0);
    settings.setValue("cCategoria","");
    settings.setValue("Clave1","");
    settings.setValue("Clave2","");

    //Note - notificar al usuario que el boton hace algo
    QMessageBox::information(this,
                             tr("Configurado"),
                             tr("Configuración inicial realizada con éxito"),
                             tr("Aceptar"));
}

void Login::btnEmpresa_clicked()
{
    //NOTE - Fixed pointer
    QScopedPointer<FrmEmpresas>formEmpresa(new FrmEmpresas(this));
    //FrmEmpresas *formEmpresa = new FrmEmpresas(this);
    formEmpresa->setWindowState(Qt::WindowMaximized);
    formEmpresa->exec();	
}

void Login::on_pushButton_clicked()
{
	this->hide();
}

void Login::init()
{
	// TODO - Rellenar en base a fichero de empresas BD terra.
	// Relleno combo empresas

    //NOTE - Fixed pointer
    QScopedPointer<QSqlQuery>QryEmpresas(new QSqlQuery(QSqlDatabase::database("terra")));
    //QSqlQuery *QryEmpresas = new QSqlQuery(QSqlDatabase::database("terra"));

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
   this->ui->btnAcceder->click();

}
