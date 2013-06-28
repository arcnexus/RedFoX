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

    importe = importe + ui->txtImporteCheque->text().toDouble();
    importe = importe + ui->txtImporteEfectivo->text().toDouble();
    importe = importe + ui->txtImporteInternet->text().toDouble();
    importe = importe + ui->txtImporteTarjeta->text().toDouble();
    importe = importe + ui->txtImporteTransferencia->text().toDouble();

    ui->txtImporteCheque->setText(Configuracion_global->toFormatoMoneda(ui->txtImporteCheque->text()));
    ui->txtImporteEfectivo->setText(Configuracion_global->toFormatoMoneda(ui->txtImporteEfectivo->text()));
    ui->txtImporteInternet->setText(Configuracion_global->toFormatoMoneda(ui->txtImporteInternet->text()));
    ui->txtImporteTarjeta->setText(Configuracion_global->toFormatoMoneda(ui->txtImporteTarjeta->text()));
    ui->txtImporteTransferencia->setText(Configuracion_global->toFormatoMoneda(ui->txtImporteTransferencia->text()));

    ui->txtImporteTotal->setText(Configuracion_global->toFormatoMoneda(QString::number(importe,'f',2)));

}

void frmCobrarDeuda::cobrar()
{
    oCliente.CobrarDeudaTotal(this->id);
    QDialog::accept();
}
