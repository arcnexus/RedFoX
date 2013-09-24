#include "frmentrada_apuntes.h"
#include "ui_frmentrada_apuntes.h"
#include "../Auxiliares/datedelegate.h"
#include "../Auxiliares/monetarydelegate_contabilidad.h"
#include "../Zona_Contabilidad/cuentas_contables.h"

FrmEntrada_apuntes::FrmEntrada_apuntes(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmEntrada_apuntes),
    menuButton(QIcon(":/Icons/PNG/asientos.png"),tr("Diario"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/asientos.png"),"",this))
{
    ui->setupUi(this);
    oApunte = new apuntes(this);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de apuntes del Diario general contable"));
}

FrmEntrada_apuntes::~FrmEntrada_apuntes()
{
    delete ui;
}

void FrmEntrada_apuntes::llenar_objeto()
{
    oApunte->asiento = ui->txtnum_asiento->text().toInt();
    oApunte->cuenta_d = ui->txtcuenta_debe->text();
    oApunte->cuenta_h = ui->txtcuenta_haber->text();
    oApunte->descripcion_d = ui->txtdesc_cuenta_debe->text();
    oApunte->descripcion_h = ui->txtdesc_cuenta_haber->text();
    oApunte->fecha_asiento = ui->txtfecha_asiento->date();
    oApunte->importe_d = ui->txtimporte_debe->text().replace(".","").replace(",",".").toDouble();
    oApunte->importe_h = ui->txtimporte_haber->text().replace(".","").replace(",",".").toDouble();
    // TODO oApunte->pos_en_asiento =

}

void FrmEntrada_apuntes::clear()
{
    ui->txtcuenta_debe->setText("");
    ui->txtcuenta_haber->setText("");
    ui->txtdesc_cuenta_debe->setText("");
    ui->txtdesc_cuenta_haber->setText("");
    ui->txtdocumento_debe->setText("");
    ui->txtdoc_haber->setText("");
    ui->txtfecha_asiento->setDate(QDate::currentDate());
    ui->txtimporte_debe->setText("0,00");
    ui->txtimporte_haber->setText("0,00");
    int index = ui->cbo_moneda->findText(Configuracion_global->cod_divisa_local);
    ui->cbo_moneda->setCurrentIndex(index);
    ui->btnbuscar_cuenta_debe->setText("");
    ui->btnbuscar_cuenta_haber->setText("");
    ui->btncerrar_asiento->setText("");
    ui->btnnuevo_asiento->setText("");
}

void FrmEntrada_apuntes::llenar_tabla_apunte(int asiento)
{
    QSqlQueryModel *modelApunte = new QSqlQueryModel(this);
    QString cSQL ="select id,pos_en_asiento,fecha_asiento,cuenta_d,descripcion_d,importe_d,cuenta_h,descripcion_h,importe_h "
            "from diario where asiento = "+QString::number(asiento)+"  order by pos_en_asiento";
 //   qDebug() << cSQL;
    modelApunte->setQuery(cSQL,Configuracion_global->contaDB);
    ui->tabla_asientos->setModel(modelApunte);
    ui->tabla_asientos->setColumnHidden(0,true);
    ui->tabla_asientos->setColumnWidth(1,80);
    ui->tabla_asientos->setColumnWidth(2,90);
    ui->tabla_asientos->setItemDelegateForColumn(2, new DateDelegate);
    ui->tabla_asientos->setColumnWidth(3,100);
    ui->tabla_asientos->setColumnWidth(4,280);
    ui->tabla_asientos->setColumnWidth(5,100);
    ui->tabla_asientos->setItemDelegateForColumn(5, new MonetaryDelegate_contabilidad);
    ui->tabla_asientos->setColumnWidth(6,100);
    ui->tabla_asientos->setColumnWidth(7,280);
    ui->tabla_asientos->setColumnWidth(8,100);
    ui->tabla_asientos->setItemDelegateForColumn(8, new MonetaryDelegate_contabilidad);
    QStringList cabecera;
    cabecera <<tr("id") << tr("Reg") << tr("Fecha") << tr("Cuenta Debe") << tr("Descripción Debe") <<tr("Importe D")
             <<tr("Cuenta Haber") << tr("Descripción Haber") << tr("Importe H") ;
    for(int pos = 0; pos <cabecera.size();pos++)
    {
        modelApunte->setHeaderData(pos,Qt::Horizontal,cabecera.at(pos));
    }
    totales_asiento(asiento);



}

void FrmEntrada_apuntes::totales_asiento(int asiento)
{
    QSqlQuery queryAsiento(Configuracion_global->contaDB);
    queryAsiento.exec("select * from diario where asiento = "+QString::number(asiento)+"  order by pos_en_asiento");
    int lineas=0;
    double importe_debe,importe_haber,descuadre;
    while (queryAsiento.next())
    {
        lineas++;
        importe_debe += queryAsiento.record().value("importe_d").toDouble();
        importe_haber += queryAsiento.record().value("importe_h").toDouble();

    }
    descuadre = importe_debe - importe_haber;
    ui->txttotal_debe->setText(Configuracion_global->toFormatoMoneda(QString::number(importe_debe,'f',Configuracion_global->decimales)));
    ui->txttotal_haber->setText(Configuracion_global->toFormatoMoneda(QString::number(importe_haber,'f',Configuracion_global->decimales)));
    if(descuadre <0)
    {
        ui->txtdescuadre_asiento->setStyleSheet("color:red; background-color:white");
        ui->txtdescuadre_asiento->setText(Configuracion_global->toFormatoMoneda(QString::number(descuadre,'f',Configuracion_global->decimales)));
        ui->txtdescuadre_asiento->setText(ui->txtdescuadre_asiento->text().append(" D"));
    } else
    {
        ui->txtdescuadre_asiento->setStyleSheet("color:black; background-color:white");
        if(descuadre >0)
        {
            ui->txtdescuadre_asiento->setText(Configuracion_global->toFormatoMoneda(QString::number(descuadre,'f',Configuracion_global->decimales)));
            ui->txtdescuadre_asiento->setText(ui->txtdescuadre_asiento->text().append(" H"));
            ui->txtdescuadre_asiento->setStyleSheet("color:red; background-color:white");
        }
    }
    ui->txtlineas_asiento->setText(QString::number(lineas));

}

void FrmEntrada_apuntes::on_btnnuevo_asiento_clicked()
{
    controles(true);
    int num_apunte = oApunte->nuevo_numero_apunte();
    ui->txtnum_asiento->setValue(num_apunte);
    ui->txtfecha_asiento->setDate(QDate::currentDate());
    ui->txtfecha_asiento->setFocus();
}

void FrmEntrada_apuntes::controles(bool stat)
{
    ui->txtcuenta_debe->setEnabled(stat);
    ui->txtcuenta_haber->setEnabled(stat);
    ui->txtdesc_cuenta_debe->setEnabled(stat);
    ui->txtdesc_cuenta_haber->setEnabled(stat);
    ui->txtdocumento_debe->setEnabled(stat);
    ui->txtdoc_haber->setEnabled(stat);
    ui->txtfecha_asiento->setEnabled(stat);
    ui->txtimporte_debe->setEnabled(stat);
    ui->txtimporte_haber->setEnabled(stat);
    ui->txtnum_asiento->setEnabled(!stat);
    if(stat)
        ui->txtnum_asiento->setStyleSheet("color:grey;background-color:#f3e0e0");
    else
        ui->txtnum_asiento->setStyleSheet("color:red;background-color:white");
    ui->cbo_moneda->setEnabled(stat);
    ui->btnbuscar_cuenta_debe->setEnabled(stat);
    ui->btnbuscar_cuenta_haber->setEnabled(stat);
    ui->btncerrar_asiento->setEnabled(stat);
    ui->btnnuevo_asiento->setEnabled(!stat);
}


void FrmEntrada_apuntes::on_btncerrar_asiento_clicked()
{
    controles(false);
}

void FrmEntrada_apuntes::on_btnanadir_linea_clicked()
{
    llenar_objeto();
    bool ok = oApunte->nuevalinea();
}

void FrmEntrada_apuntes::on_txtnum_asiento_valueChanged(int arg1)
{
    blockSignals(true);
    llenar_tabla_apunte(arg1);
    blockSignals(false);
}

void FrmEntrada_apuntes::on_txtcuenta_debe_editingFinished()
{
    Cuentas_contables cuenta(this);
    blockSignals(true);
    ui->txtcuenta_debe->setText(cuenta.completar_cuenta(ui->txtcuenta_debe->text()));
    blockSignals(false);
}
