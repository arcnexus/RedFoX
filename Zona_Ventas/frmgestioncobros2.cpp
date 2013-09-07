#include "frmgestioncobros2.h"
#include "ui_frmgestioncobros2.h"
#include "vencimientos.h"

FrmGestionCobros2::FrmGestionCobros2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmGestionCobros2)
{
    ui->setupUi(this);
    //-------------------
    // Conexiones
    //-------------------
}

FrmGestionCobros2::~FrmGestionCobros2()
{
    delete ui;
}

void FrmGestionCobros2::on_btnAceptar_clicked()
{
    valores * val = new valores;
    val->efectivo = Configuracion_global->MonedatoDouble(ui->txtEfectivo->text());
    val->cheque  = Configuracion_global->MonedatoDouble(ui->txtCheque->text());
    val->internet = Configuracion_global->MonedatoDouble(ui->txtInternet->text());
    val->tarjeta = Configuracion_global->MonedatoDouble(ui->txtTarjeta->text());
    val->transferencia = Configuracion_global->MonedatoDouble(ui->txtTransferencia->text());

    QHash <QString,QVariant> e;
    e["importe_efectivo"] = val->efectivo;
    e["importe_cheque"] = val->cheque;
    e["importe_internet"] = val->internet;
    e["importe_tarjeta"] = val->tarjeta;
    e["importe_transferencia"] = val->transferencia;
    e["pagado"] = Configuracion_global->MonedatoDouble(ui->txtEntrega->text());
    e["pendiente_cobro"] = Configuracion_global->MonedatoDouble(ui->txtPendiente->text());

    QString error;
    bool updated = SqlCalls::SqlUpdate(e,"clientes_deuda",Configuracion_global->groupDB,QString("id=%1").arg(this->id),error);
    if(!updated)
        QMessageBox::warning(this,tr("Gestión de cobros"),tr("Ocurrió un error al actualizar los datos: %1").arg(error));

    accept();
}
double FrmGestionCobros2::getImporte() const
{
    return importe;
}

void FrmGestionCobros2::setImporte(double value,int id_)
{
    importe = value;
    this->id = id_;
    ui->txtImporte_pendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(importe,
                                                                'f',Configuracion_global->decimales_campos_totales)));
}


void FrmGestionCobros2::on_txtEfectivo_editingFinished()
{
    blockSignals(true);
    ui->txtEfectivo->setText(Configuracion_global->toFormatoMoneda(ui->txtEfectivo->text()));
    calcular();
    blockSignals(false);
}

void FrmGestionCobros2::on_txtTransferencia_editingFinished()
{
    blockSignals(true);
    ui->txtTransferencia->setText(Configuracion_global->toFormatoMoneda(ui->txtTransferencia->text()));
    calcular();
    blockSignals(false);
}

void FrmGestionCobros2::on_txtTarjeta_editingFinished()
{
    blockSignals(true);
    ui->txtTarjeta->setText(Configuracion_global->toFormatoMoneda(ui->txtTarjeta->text()));
    calcular();
    blockSignals(false);
}

void FrmGestionCobros2::on_txtCheque_editingFinished()
{
    blockSignals(true);
    ui->txtCheque->setText(Configuracion_global->toFormatoMoneda(ui->txtCheque->text()));
    calcular();
    blockSignals(false);
}

void FrmGestionCobros2::on_txtInternet_editingFinished()
{
    blockSignals(true);
    ui->txtInternet->setText(Configuracion_global->toFormatoMoneda(ui->txtInternet->text()));
    calcular();
    blockSignals(false);
}

void FrmGestionCobros2::calcular()
{
    double pendiente,efectivo,transferencia,tarjeta,cheque,internet,nuevo_pendiente,cambio,pagado;
    pendiente = Configuracion_global->MonedatoDouble(ui->txtImporte_pendiente->text());
    efectivo = Configuracion_global->MonedatoDouble(ui->txtEfectivo->text());
    transferencia = Configuracion_global->MonedatoDouble(ui->txtTransferencia->text());
    tarjeta = Configuracion_global->MonedatoDouble(ui->txtTarjeta->text());
    cheque = Configuracion_global->MonedatoDouble(ui->txtCheque->text());
    internet = Configuracion_global->MonedatoDouble(ui->txtInternet->text());

    pagado = efectivo + transferencia + tarjeta + cheque + internet;
    ui->txtEntrega->setText(Configuracion_global->toFormatoMoneda(QString::number(pagado,'f',2)));
    nuevo_pendiente = pendiente - pagado;
    if(nuevo_pendiente >0)
    {
        ui->txtPendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(nuevo_pendiente,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
        ui->txtCambio->setText("0,00");
    }
    else{
        ui->txtCambio->setText(Configuracion_global->toFormatoMoneda(QString::number(nuevo_pendiente,'f',
                                                                                     Configuracion_global->decimales_campos_totales)));
        ui->txtPendiente->setText("0,00");
    }

}


