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
    e = SqlCalls::SelectRecord("empresas",QString("id =%1").arg(Configuracion_global->idEmpresa),Configuracion_global->globalDB,error);
    if(error.isEmpty()){
        this->facturas_en_cierre = e.value(Configuracion_global->idEmpresa).value("facturas_en_cierre").toBool();
        this->importe_fijo_cierre = e.value(Configuracion_global->idEmpresa).value("importe_cierre").toDouble();
    } else
    {
        QMessageBox::warning(this,tr("Cierre de caja"),tr("Error al recuperar los datos de la empresa: %1 ").arg(error),
                             tr("Aceptar"));
    }
}

FrmCierreCaja::~FrmCierreCaja()
{
    delete ui;
}

void FrmCierreCaja::on_btnAceptar_clicked()
{
    close();
}

void FrmCierreCaja::on_btnIniciar_cierre_clicked()
{
    //----------------------------
    // Calculo ingresos
    // ---------------------------
    QString cSQL;
    cSQL = "select  sum(importe_efectivo) as efectivo, sum(importe_tarjeta) as tarjeta, sum(importe_cheque) as cheque,"
            "sum(importe_domiciliacion) as domiciliacion, sum(importe_transferencia) as transferencia, sum(importe_vale) as vale "
            "from cab_tpv";
    QSqlQuery result(Configuracion_global->empresaDB);
    result.exec(cSQL);
    ui->txtEfectivo->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                       result.record().value("efectivo").toDouble(),'f',
                                                                       Configuracion_global->decimales_campos_totales)));

}
