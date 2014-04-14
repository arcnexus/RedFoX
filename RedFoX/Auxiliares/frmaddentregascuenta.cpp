#include "frmaddentregascuenta.h"
#include "ui_frmaddentregascuenta.h"
#include "../Zona_Pacientes/cliente.h"

frmAddEntregasCuenta::frmAddEntregasCuenta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmAddEntregasCuenta)
{
    ui->setupUi(this);
    ui->txtFecha->setDate(QDate::currentDate());
    ui->txtImporte->setFocus();
}

frmAddEntregasCuenta::~frmAddEntregasCuenta()
{
    delete ui;
}

void frmAddEntregasCuenta::set_concepto(QString concepto)
{
    ui->txtConcepto->setText(concepto);
}

double frmAddEntregasCuenta::get_importe_entrega()
{
    return ui->txtImporte->text().replace(".","").replace(",",".").toDouble();
}

void frmAddEntregasCuenta::set_id_cliente(int id)
{
    this->id_cliente = id;
}

void frmAddEntregasCuenta::on_txtImporte_editingFinished()
{
    ui->txtImporte->setText(Configuracion_global->toFormatoMoneda(ui->txtImporte->text()));
    this->importe = ui->txtImporte->text().replace(".","").replace(",",".").toDouble();
}

void frmAddEntregasCuenta::on_txtFecha_editingFinished()
{
    this->fecha = ui->txtFecha->date();

}

void frmAddEntregasCuenta::on_txtConcepto_cursorPositionChanged(int arg1, int arg2)
{
    this->concepto = ui->txtConcepto->text();
}

void frmAddEntregasCuenta::validar()
{
    QString texto_error;
    texto_error = tr("Faltan campos a rellenar:");
    if(ui->txtImporte->text().isEmpty() || ui->txtImporte->text() == "0,00")
        texto_error.append(tr("\nFalta rellenar el importe"));
    if(ui->txtConcepto->text().isEmpty())
        texto_error.append(tr("\nFalta rellenar el concepto"));

    if((ui->txtImporte->text().isEmpty() || ui->txtImporte->text() == "0,00")  || ui->txtConcepto->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Añadir entregas"),texto_error,tr("Aceptar"));
        ui->txtImporte->setFocus();
    }
    else
        accept();
    Cliente oCliente(this);
    oCliente.anadir_entrega_cuenta(this->id_cliente,ui->txtFecha->date(),ui->txtImporte->text().replace(".","").replace(",",".").toDouble(),
                                   ui->txtConcepto->text());
}
