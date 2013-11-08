#include "frmcierrecaja.h"
#include "ui_frmcierrecaja.h"


FrmCierreCaja::FrmCierreCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmCierreCaja)
{
    ui->setupUi(this);
    QSqlQueryModel *users = new QSqlQueryModel(this);
    users->setQuery("select nombre from usuarios",Configuracion_global->groupDB);
    ui->cboUsuarioCierre->setModel(users);

    //------------------------------------------
    // Recupero parametros empresa para cierre
    //------------------------------------------
    QMap <int, QSqlRecord> e;
    QString error;
    e = SqlCalls::SelectRecord("empresas",QString("id =%1").arg(Configuracion_global->idEmpresa),Configuracion_global->groupDB,error);
    if(error.isEmpty()){
        this->facturas_en_cierre = e.value(Configuracion_global->idEmpresa).value("facturas_en_cierre").toBool();
        this->importe_fijo_cierre = e.value(Configuracion_global->idEmpresa).value("importe_cierre").toDouble();
    } else
    {
        QMessageBox::warning(this,tr("Cierre de caja"),tr("Error al recuperar los datos de la empresa: %1 ").arg(error),
                             tr("Aceptar"));
    }
    //cargar_datos_caja();


}

FrmCierreCaja::~FrmCierreCaja()
{
    delete ui;
}



void FrmCierreCaja::cargar_datos_caja(int id)
{
    QMap <int,QSqlRecord> caja;
    QString error;
    caja = SqlCalls::SelectRecord("cierrecaja",QString("id=%1").arg(id),Configuracion_global->empresaDB,error);
    if(caja.size()<1)
        QMessageBox::warning(this,tr("Gestión de tickets"),tr("Ocurrió un error al recuperar la caja : %1").arg(error),
                             tr("Aceptar"));
    else
    {
        ui->lblimporte_abertura->setText(Configuracion_global->toFormatoMoneda(
                                             QString::number(caja.value(id).value("importe_abertura_dia").toDouble(),
                                                     'f',Configuracion_global->decimales_campos_totales)));
        QString user = SqlCalls::SelectOneField("usuarios","nombre",QString("id = %1").arg(
                                                    caja.value(id).value("id_usuario").toInt()),Configuracion_global->groupDB,
                                                error).toString();
        if(error.isEmpty())
            ui->lblUsuario_abertura->setText(user);
        else
            QMessageBox::warning(this,tr("Gestión de TPV / Cierre caja"),
                                 tr("Ocurrió un error al mostrar datos abertura caja: %1").arg(error));
    }
}



void FrmCierreCaja::on_btnIniciar_cierre_clicked()
{
    ui->txtimporte_abertura->setText(ui->lblimporte_abertura->text());
    ui->txthora_cierre->setTime(QTime::currentTime());
    QDate fecha_cierre =ui->calendarWidget->selectedDate();

    //----------------------------
    // Calculo ingresos
    // ---------------------------
    QString cSQL;
    cSQL = QString("select  sum(importe_efectivo) as efectivo, sum(importe_tarjeta) as tarjeta, sum(importe_cheque) as cheque,"
            "sum(importe_domiciliacion) as domiciliacion, sum(importe_transferencia) as transferencia, sum(importe_vale) as vale "
                   "from cab_tpv where fecha = '%1'").arg(fecha_cierre.toString("yyyyMMdd"));

    QSqlQuery result(Configuracion_global->empresaDB);
    result.exec(cSQL);
    result.next();
    ui->txtEfectivo->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                       result.record().value("efectivo").toDouble(),'f',
                                                                       Configuracion_global->decimales_campos_totales)));
    double efect = Configuracion_global->MonedatoDouble(ui->lblimporte_abertura->text()) +
                                                         Configuracion_global->MonedatoDouble(ui->txtEfectivo->text());
    ui->txtCaja_moneda->setText(Configuracion_global->toFormatoMoneda(QString::number(efect,'f',
                                                                                      Configuracion_global->decimales_campos_totales)));

    ui->txtTarjeta->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                      result.record().value("tarjeta").toDouble(),'f',
                                                                      Configuracion_global->decimales_campos_totales)));
    ui->txtCheques->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                       result.record().value("cheque").toDouble(),'f',
                                                                       Configuracion_global->decimales_campos_totales)));
    ui->txtDomiciliacion->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                       result.record().value("domiciliacion").toDouble(),'f',
                                                                       Configuracion_global->decimales_campos_totales)));
    ui->txtTransferencia->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                       result.record().value("transferencia").toDouble(),'f',
                                                                       Configuracion_global->decimales_campos_totales)));
    ui->txtVales_recibidos->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                       result.record().value("vale").toDouble(),'f',
                                                                       Configuracion_global->decimales_campos_totales)));

    //-----------------
    // Vales emitidos
    //-----------------
    cSQL = QString("select  sum(importe) as importe from vales where fecha = '%1'").arg(fecha_cierre.toString("yyyyMMdd"));
    result.exec(cSQL);
    result.next();
    QString importe = QString::number(result.record().value("importe").toDouble(),'f',
                                      Configuracion_global->decimales_campos_totales);
    ui->txtValesEmitidos->setText(Configuracion_global->toFormatoMoneda(importe));

    TimedMessageBox * t = new TimedMessageBox(this,tr("Se han obtenido los datos de las cabeceras de tickets"));
    ui->btnCierreCaja->setEnabled(true);
    ui->btnCierreParcial->setEnabled(true);


}
