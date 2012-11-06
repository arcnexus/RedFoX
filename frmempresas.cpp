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
    ui->txtcCodigo->setText(oEmpresa.getcCodigo());
    ui->txtEmpresa->setText(oEmpresa.getcNombre());
    ui->txtRutaBd->setText(oEmpresa.getcRutaBDSqLite());
    ui->txtcHost->setText(oEmpresa.getcHost());
    ui->txtcNombreBd->setText(oEmpresa.getcNombreBD());
    ui->txtcUser->setText(oEmpresa.getcUser());
    ui->txtcPassword->setText(oEmpresa.getcContrasena());
    ui->txtcPuerto->setText(oEmpresa.getcPuerto());
    int indice=ui->txtcDriver->findText(oEmpresa.getcDriverBD());
    if(indice!=-1)
       ui->txtcDriver->setCurrentIndex(indice);

    ui->txtRutaBd->setText(oEmpresa.getcRutaBDSqLite());
    ui->txtcHostmedic->setText(oEmpresa.getcHostMed());
    ui->txtcNombreBdMedic->setText(oEmpresa.getcNombreBDMed());
    ui->txtcUserMedic->setText(oEmpresa.getcUserMed());
    ui->txtcPasswordBdMedic->setText(oEmpresa.getcContrasenaMed());
    ui->txtcPuertoMedic->setText(oEmpresa.getcPuertoMed());
    indice=ui->txtcDriverMedica->findText(oEmpresa.getcDriverBDMed());
    if(indice!=-1)
       ui->txtcDriverMedica->setCurrentIndex(indice);


    ui->txtcDireccion->setText(oEmpresa.getcDireccion());
    ui->txtcCP->setText(oEmpresa.getcCp());
    ui->txtcPoblacion->setText(oEmpresa.getcPoblacion());
    ui->txtcProvincia->setText(oEmpresa.getcProvincia());
    ui->txtcPais->setText(oEmpresa.getcPais());
    ui->txtcTelefono1->setText(oEmpresa.getcTelefono1());
    ui->txtcTelefono2->setText(oEmpresa.getcTelefono2());
    ui->txtcFax->setText(oEmpresa.getcFax());
    ui->txtcMail->setText(oEmpresa.getcMail());
    ui->txtcWeb->setText(oEmpresa.getcWeb());
    ui->txtcCif->setText(oEmpresa.getcCif());
    ui->txtcInscripcion->setText(oEmpresa.getcInscripcion());
    ui->txtcCometarioAlbaran->setText(oEmpresa.getcComentarioAlbaran());
    ui->txtcComentarioFactura->setText(oEmpresa.getcComentarioFactura());
    ui->txtcComentarioTicket->setText(oEmpresa.getcComentarioTicket());
    ui->txtnEjercicio->setValue(oEmpresa.getnEjercicio());
    ui->txtnDigitosFactura->setText(QString::number(oEmpresa.getnDigitosFactura()));
    ui->txtcSerieFactura->setText(oEmpresa.getcSerie());
    ui->txtnDigitosCuentas->setText(QString::number(oEmpresa.getnDigitosCuentas()));
    ui->txtcCuentaCliente->setText(oEmpresa.getcCuentaClientes());
    ui->txtcCuentaProveedores->setText(oEmpresa.getcCuentaProveedores());
    ui->txtcCuentaAcreedores->setText(oEmpresa.getcCuentaAcreedores());



}

void FrmEmpresas::CargarCamposEnEmpresa()
{
    oEmpresa.setcCodigo(ui->txtcCodigo->text());
    oEmpresa.setcNombre(ui->txtEmpresa->text());
    oEmpresa.setcRutaBDSqLite(ui->txtRutaBd->text());
    oEmpresa.setcHost(ui->txtcHost->text());
    oEmpresa.setcNombreBD(ui->txtcNombreBd->text());
    oEmpresa.setcUser(ui->txtcUser->text());
    oEmpresa.setcContrasena(ui->txtcPassword->text());
    oEmpresa.setcPuerto(ui->txtcPuerto->text());
    oEmpresa.setcDriverBD(ui->txtcDriver->currentText());

    oEmpresa.setcHostMed(ui->txtcHostmedic->text());
    oEmpresa.setcNombreBDMed(ui->txtcNombreBdMedic->text());
    oEmpresa.setcUserMed(ui->txtcUserMedic->text());
    oEmpresa.setcContrasenaMed(ui->txtcPasswordBdMedic ->text());
    oEmpresa.setcPuertoMed(ui->txtcPuertoMedic->text());
    oEmpresa.setcDriverBDMed(ui->txtcDriverMedica->currentText());



    oEmpresa.setcSerie(ui->txtcSerieFactura->text());
    oEmpresa.setcDireccion(ui->txtcDireccion->text());
    oEmpresa.setcCP(ui->txtcCP->text());
    oEmpresa.setcPoblacion(ui->txtcPoblacion->text());
    oEmpresa.setcProvincia(ui->txtcProvincia->text());
    oEmpresa.setcPais(ui->txtcPais->text());
    oEmpresa.setcTelefono1(ui->txtcTelefono1->text());
    oEmpresa.setcTelefono2(ui->txtcPais->text());
    oEmpresa.setcFax(ui->txtcFax->text());
    oEmpresa.setcMail(ui->txtcMail->text());
    oEmpresa.setcWeb(ui->txtcWeb->text());
    oEmpresa.setcCif(ui->txtcCif->text());
    oEmpresa.setcInscripcion(ui->txtcInscripcion->text());
    oEmpresa.setcComemtarioAlbaran(ui->txtcCometarioAlbaran->toPlainText());
    oEmpresa.setcComentarioFactura(ui->txtcComentarioFactura->toPlainText());
    oEmpresa.setcComentarioTicket(ui->txtcComentarioTicket->toPlainText());
    oEmpresa.setnEjercicio(ui->txtnEjercicio->text().toInt());
    oEmpresa.setnDigitosCuentas(ui->txtnDigitosCuentas->text().toInt());
    oEmpresa.setcCodigoCuentaClientes(ui->txtcCuentaCliente->text());
    oEmpresa.setcCodigoCuentaProveedor(ui->txtcCuentaProveedores->text());
    oEmpresa.setcCodigoCuentaAcreedores(ui->txtcCuentaAcreedores->text());
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
