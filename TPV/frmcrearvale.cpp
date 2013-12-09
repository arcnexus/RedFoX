#include "frmcrearvale.h"
#include "ui_frmcrearvale.h"
#include <Auxiliares/Globlal_Include.h>

frmCrearVale::frmCrearVale(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmCrearVale)
{
    ui->setupUi(this);
}

frmCrearVale::~frmCrearVale()
{
    delete ui;
}

void frmCrearVale::setImporte(double importe)
{
    if(importe < 0)
        importe = - importe;

    ui->txtImporte->setText(Configuracion_global->toFormatoMoneda(QString::number(importe,'f',
                                                                                  Configuracion_global->decimales_campos_totales)));
}

void frmCrearVale::on_txtImporte_editingFinished()
{
    blockSignals(true);
    ui->txtImporte->setText(Configuracion_global->toFormatoMoneda(ui->txtImporte->text()));
    blockSignals(false);
}

void frmCrearVale::on_txtcodigo_editingFinished()
{
    QMap <int,QSqlRecord> cli;
    QString error;
    cli = SqlCalls::SelectRecord("clientes",QString("codigo=%1").arg(ui->txtcodigo->text()),Configuracion_global->groupDB,error);
    QMapIterator <int,QSqlRecord> i(cli);
    if(cli.count() == 1){
        while(i.hasNext())
        {
            i.next();
            ui->txtnombre_fiscal->setText(i.value().value("nombre_fiscal").toString());
        }
    }else
    {
        QMessageBox::warning(this,tr("Vales"),tr("Atención se ha localizado más de un cliente con el mismo código,"
                                                 "repase la base de datos"),tr("Aceptar"));
    }

}
