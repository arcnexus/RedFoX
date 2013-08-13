#include "frmcobrardeuda.h"
#include "ui_frmcobrardeuda.h"

frmCobrarDeuda::frmCobrarDeuda(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmCobrarDeuda)
{
    ui->setupUi(this);
}

frmCobrarDeuda::~frmCobrarDeuda()
{
    delete ui;
}

void frmCobrarDeuda::calcular()
{
    double importe = 0;

    importe = importe + ui->txtimporte_cheque->text().toDouble();
    importe = importe + ui->txtimporte_efectivo->text().toDouble();
    importe = importe + ui->txtImporteInternet->text().toDouble();
    importe = importe + ui->txtimporte_tarjeta->text().toDouble();
    importe = importe + ui->txtImporteTransferencia->text().toDouble();

    ui->txtimporte_cheque->setText(Configuracion_global->toFormatoMoneda(ui->txtimporte_cheque->text()));
    ui->txtimporte_efectivo->setText(Configuracion_global->toFormatoMoneda(ui->txtimporte_efectivo->text()));
    ui->txtImporteInternet->setText(Configuracion_global->toFormatoMoneda(ui->txtImporteInternet->text()));
    ui->txtimporte_tarjeta->setText(Configuracion_global->toFormatoMoneda(ui->txtimporte_tarjeta->text()));
    ui->txtImporteTransferencia->setText(Configuracion_global->toFormatoMoneda(ui->txtImporteTransferencia->text()));

    ui->txtImporteTotal->setText(Configuracion_global->toFormatoMoneda(QString::number(importe,'f',Configuracion_global->decimales)));

}

void frmCobrarDeuda::cobrar()
{
    oCliente.CobrarDeudaTotal(this->id);
    QDialog::accept();
}
