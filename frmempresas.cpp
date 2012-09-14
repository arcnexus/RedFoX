#include "frmempresas.h"
#include "ui_frmempresas.h"
#include <QtSql>
#include <QSqlDatabase>
#include<QSqlQuery>
#include <QMessageBox>

FrmEmpresas::FrmEmpresas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmEmpresas)
{
    ui->setupUi(this);
    ui->txtMensaje->setText("ATENCION: Modifique con cuidado. Cambiar estos\n valores puede hacer que el programa deje\n de funcionar");
}

FrmEmpresas::~FrmEmpresas()
{
    delete ui;
}

void FrmEmpresas::LLenarCampos()
{
    ui->txtEmpresa->setText(oEmpresa.getcNombre());
    ui->txtRutaBd->setText(oEmpresa.getcRutaBDSqLite());
    ui->txtcHost->setText(oEmpresa.getcHost());
    ui->txtcNombreBd->setText(oEmpresa.getcNombreBD());
    ui->txtcUser->setText(oEmpresa.getcUser());
    ui->txtcPassword->setText(oEmpresa.getcContrasena());
    ui->txtcPuerto->setText(oEmpresa.getcPuerto());
    ui->txtcDriver->setEditText(oEmpresa.getcDriverBD());
    ui->txtcDireccion->setText(oEmpresa.getcDireccion());
    ui->txtcCP->setText(oEmpresa.getcCp());



}

void FrmEmpresas::CargarCamposEnEmpresa()
{
    oEmpresa.setcNombre(ui->txtEmpresa->text());
    oEmpresa.setcRutaBDSqLite(ui->txtRutaBd->text());
    oEmpresa.setcHost(ui->txtcHost->text());
    oEmpresa.setcNombreBD(ui->txtcNombreBd->text());
    oEmpresa.setcUser(ui->txtcUser->text());
    oEmpresa.setcContrasena(ui->txtcPassword->text());
    oEmpresa.setcPuerto(ui->txtcPuerto->text());
    oEmpresa.setcDriverBD(ui->txtcDriver->currentText());
}

void FrmEmpresas::on_pushButton_clicked()
{
    QSqlQuery *Empresa = new QSqlQuery(QSqlDatabase::database("terra"));
    Empresa->prepare("insert into empresas (codigo,nombre,digitosfactura,serie,RutaBDSqLite,driverBD) "
                     "Values('04','Empresa Julian',5,'2012','/home/dismet/Dismet/Programming/terra/DB/emp0999.sqlite','QSQLITE')");
    if(Empresa->exec())
        QMessageBox::information(NULL,"Empresas","Enhorabuena, su empresa ha sido creada\n "
                                 "Debe salir y volver a entrar del programa para poder acceder a ella","Ok");
    else
        QMessageBox::warning(NULL,"Empresas","Ha ocurrido un error :" +Empresa->lastError().text());

}

void FrmEmpresas::on_botSiguiente_clicked()
{
    oEmpresa.Recuperar("select * from empresas where id > "+QString::number(oEmpresa.getid()),1);
    LLenarCampos();
}

void FrmEmpresas::on_botAnterior_clicked()
{
    oEmpresa.Recuperar("select * from empresas where id < "+QString::number(oEmpresa.getid())+" order by id desc",2);
    LLenarCampos();
}


void FrmEmpresas::on_botGuardar_clicked()
{
    CargarCamposEnEmpresa();
    oEmpresa.Guardar();
}
