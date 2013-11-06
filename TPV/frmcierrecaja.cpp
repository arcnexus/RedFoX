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
    // ----------------
    // Cajas abiertas
    // ----------------
    QMap <int,QSqlRecord> cajas;
    QStringList condiciones;
    condiciones << QString("fecha_abertura = '%1'").arg(QDate::currentDate().toString("yyyyMMdd"));
    condiciones << QString("id_caja = %1").arg(Configuracion_global->caja);
    condiciones << QString("ejercicio = '%1'").arg(Configuracion_global->cEjercicio);
    cajas = SqlCalls::SelectRecord("cierrecaja",condiciones,Configuracion_global->empresaDB,error);
    if(cajas.size() ==1)
    {
        QMapIterator <int, QSqlRecord> it(cajas);
        while (it.hasNext())
        {
            it.next();
            cargar_datos_caja(it.value().value("id").toInt());
        }
    }
    else if(cajas.size() <1)
        QMessageBox::warning(this,tr("Gestión de TPV"),tr("No hay cajas abiertas en este terminal"),tr("Aceptar"));
    else if(cajas.size()>1)
    {
        frmCajasAbiertas cajas;
        cajas.cargar_datos(Configuracion_global->caja,Configuracion_global->cEjercicio);
        if(cajas.exec() == QDialog::Accepted)
        {
            int id = cajas.id;
            cargar_datos_caja(id);
        }
    }


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


/*void FrmCierreCaja::on_btnAceptar_clicked()
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

}*/
