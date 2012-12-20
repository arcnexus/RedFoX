#include "login.h"
#include "ui_login.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QErrorMessage>
#include <QMessageBox>
#include<QSqlError>
#include<mainwindow.h>
#include <QDir>
#include <QDebug>
#include <configuracion.h>
#include <QSqlRecord>
#include <QSqlField>
#include "frmempresas.h"


Login::Login(Configuracion *m_config,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    ui->setupUi(this);
    // TODO - Rellenar en base a fichero de empresas BD terra.
    // Relleno combo empresas

    QSqlQuery *QryEmpresas = new QSqlQuery(QSqlDatabase::database("terra"));
    QryEmpresas->prepare("Select * from empresas");
    if(QryEmpresas->exec()) {

        while (QryEmpresas->next()) {
              QSqlRecord rEmpresa = QryEmpresas->record();
              ui->cboEmpresa->addItem(rEmpresa.field("nombre").value().toString());
        }
    }

    this->ui->lineUsuario->setFocus();

}

Login::~Login()
{

    delete ui;

    //dbTerra.close();
}

const QString &Login::getUsuario() const
{
    return ui->lineUsuario->text();
}

const QString &Login::getEmpresa() const
{
    return ui->cboEmpresa->currentText();
}


void Login::on_btnAcceder_clicked()
{

    QSqlQuery qryUsers(QSqlDatabase::database("terra"));
    qryUsers.prepare( "SELECT * FROM usuarios where nombre =:Nombre" );
    qryUsers.bindValue(":Nombre",ui->lineUsuario->text());
    //qryUsers.prepare("Select * from usuarios where nombre = '"+ui->lineUsuario->text.trimmed()+"'");
    if( !qryUsers.exec() ) {
        QMessageBox::critical(NULL, "error:", qryUsers.lastError().text());
    } else {
        if (qryUsers.next()) {
            QSqlRecord rUsuario = qryUsers.record();
            if (ui->linePassword->text()==qryUsers.value(2).toString()) {
                QSettings settings("infint", "terra");
                settings.setValue("cUsuarioActivo",rUsuario.field("nombre").value().toString());
                settings.setValue("nNivelAcceso",rUsuario.field("nivelacceso").value().toInt());
                settings.setValue("cCategoria",rUsuario.field("categoria").value().toString());
               // QStyle *style = QStyleFactory::create("oxygen");
               //QStyle *style = QStyleFactory::create("plastique");
               //QApplication::setStyle(style);
                Login::done( QDialog::Accepted);

            } else {
                QMessageBox::critical(0,"ACCESO DENEGADO","El usuario y la contraseña no se corresponden\n\n Verifique los datos");
                ui->linePassword->setText("");
                ui->linePassword->setFocus();
            }

        } else {

            QMessageBox::critical(0,"Error","No existe ningún usuario con este nombre");
            ui->lineUsuario->setText("");
            ui->lineUsuario->setFocus();
        }
    }
}

void Login::on_Crearconfiguracin_clicked()
{
    QSettings settings("infint", "terra");
    settings.setValue("cDriverBDTerra","QSQLITE");
    settings.setValue("cRutaDBTerra","/home/arcnexus/TerraSoftware/Proyectos/terra/DB/terra.sqlite");
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


}

void Login::on_btnEmpresa_clicked()
{
    FrmEmpresas *formEmpresa = new FrmEmpresas();
    formEmpresa->setWindowState(Qt::WindowMaximized);
    formEmpresa->exec();

    delete formEmpresa;
}

void Login::on_pushButton_clicked()
{
    exit(0);
}
