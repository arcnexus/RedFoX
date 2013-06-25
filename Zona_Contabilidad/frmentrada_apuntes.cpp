#include "frmentrada_apuntes.h"
#include "ui_frmentrada_apuntes.h"
#include "../Auxiliares/datedelegate.h"
#include "../Auxiliares/monetarydelegate_contabilidad.h"
#include "../Zona_Contabilidad/cuentas_contables.h"

FrmEntrada_apuntes::FrmEntrada_apuntes(QWidget *parent) :
    MayaModule(ModuleZone(),ModuleName(),parent),
    ui(new Ui::FrmEntrada_apuntes),
    toolButton(tr("Diario"),":/Icons/PNG/asientos.png",this),
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
    oApunte->cuentaD = ui->txtcuenta_debe->text();
    oApunte->cuentaH = ui->txtcuenta_haber->text();
    oApunte->descripcionD = ui->txtdesc_cuenta_debe->text();
    oApunte->descripcionH = ui->txtdesc_cuenta_haber->text();
    oApunte->fechaAsiento = ui->txtfecha_asiento->date();
    oApunte->importeD = ui->txtimporte_debe->text().replace(",",".").toDouble();
    oApunte->importeH = ui->txtimporte_haber->text().replace(",",".").toDouble();
    // TODO oApunte->posenasiento =

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
    int index = ui->cbo_moneda->findText(Configuracion_global->codDivisaLocal);
    ui->cbo_moneda->setCurrentIndex(index);
    ui->btnbuscar_cuenta_debe->setText("");
    ui->btnbuscar_cuenta_haber->setText("");
    ui->btncerrar_asiento->setText("");
    ui->btnnuevo_asiento->setText("");
}

void FrmEntrada_apuntes::llenar_tabla_apunte(int asiento)
{
    QSqlQueryModel *modelApunte = new QSqlQueryModel(this);
    QString cSQL ="select id,posenasiento,fechaAsiento,cuentaD,descripcionD,importeD,cuentaH,descripcionH,importeH "
            "from diario where asiento = "+QString::number(asiento)+"  order by posenasiento";
 //   qDebug() << cSQL;
    modelApunte->setQuery(cSQL,QSqlDatabase::database("dbconta"));
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
    QSqlQuery queryAsiento(QSqlDatabase::database("dbconta"));
    queryAsiento.exec("select * from diario where asiento = "+QString::number(asiento)+"  order by posenasiento");
    int lineas=0;
    double importedebe,importehaber,descuadre;
    while (queryAsiento.next())
    {
        lineas++;
        importedebe += queryAsiento.record().value("importeD").toDouble();
        importehaber += queryAsiento.record().value("importeH").toDouble();

    }
    descuadre = importedebe - importehaber;
    ui->txttotal_debe->setText(Configuracion_global->FormatoNumerico(QString::number(importedebe,'f',2)));
    ui->txttotal_haber->setText(Configuracion_global->FormatoNumerico(QString::number(importehaber,'f',2)));
    if(descuadre <0)
    {
        ui->txtdescuadre_asiento->setStyleSheet("color:red; background-color:white");
        ui->txtdescuadre_asiento->setText(Configuracion_global->FormatoNumerico(QString::number(descuadre,'f',2)));
        ui->txtdescuadre_asiento->setText(ui->txtdescuadre_asiento->text().append(" D"));
    } else
    {
        ui->txtdescuadre_asiento->setStyleSheet("color:black; background-color:white");
        if(descuadre >0)
        {
            ui->txtdescuadre_asiento->setText(Configuracion_global->FormatoNumerico(QString::number(descuadre,'f',2)));
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
