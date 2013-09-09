#include "frmgestioncobros.h"
#include "ui_frmgestioncobros.h"
#include "../Zona_Ventas/frmgestioncobros2.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/datedelegate.h"

frmGestionCobros::frmGestionCobros(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmGestionCobros),
    toolButton(tr("Gest. Cobros"),":/Icons/PNG/Cobros.png",this),
    menuButton(QIcon(":/Icons/PNG/Cobros.png"),tr("Gest. Cobros"),this)
{
    ui->setupUi(this);
    this->id_cliente = -1;
    this->id_factura = -1;
    this->id_ticket = -1;

    ui->txtfecha_ini->setDate(QDate::currentDate());
    ui->txtfecha_fin->setDate(QDate::currentDate().addDays(7));
    ui->lblNombre->setText(tr("DEUDAS ENTRE FECHAS"));
    // -----------------
    // deudas
    // -----------------

    deudas = new QSqlQueryModel(this);
    ui->tabla_deuda->setModel(deudas);
    QStringList headers;
    QVariantList sizes;
    headers <<"id" <<tr("fecha") <<tr("vencimiento") <<tr("documento") <<tr("importe") << tr("pagado") <<tr("pendiente");
    sizes << 0 << 100 << 100 << 120 << 120 << 120 << 120;

    for(int x=1;x<sizes.size();x++)
    {
         ui->tabla_deuda->setColumnWidth(x,sizes.at(x).toInt());
         deudas->setHeaderData(x,Qt::Horizontal,headers.at(x));
    }
    ui->tabla_deuda->setColumnHidden(0,true);
    seleccionar_varios();

    //--------------------
    // Clientes
    //--------------------
    ui->txtbuscar_cliente->setText("a");
    QSqlQueryModel *modelo_clientes = new QSqlQueryModel(this);
    modelo_clientes->setQuery("select id,codigo_cliente, nombre_fiscal, cif_nif from clientes where nombre_fiscal like '%a' limit 0,150",
                              Configuracion_global->groupDB);

    ui->tabla_clientes->setModel(modelo_clientes);
    ui->tabla_clientes->setColumnHidden(0,true);
    ui->tabla_clientes->setColumnWidth(1,70);
    ui->tabla_clientes->setColumnWidth(2,170);
    ui->tabla_clientes->setColumnWidth(3,90);
    modelo_clientes->setHeaderData(1,Qt::Horizontal,tr("Código"));
    modelo_clientes->setHeaderData(2,Qt::Horizontal,tr("Denominación"));
    modelo_clientes->setHeaderData(3,Qt::Horizontal,tr("Cif/Nif"));
    ui->txtbuscar_cliente->setFocus();

    //-----------------------
    // Conexiones
    //-----------------------
    connect(ui->radPagados,SIGNAL(clicked()),this,SLOT(seleccionar()));
    connect(ui->radPendientes,SIGNAL(clicked()),this,SLOT(seleccionar()));
    connect(ui->radTodos,SIGNAL(clicked()),this,SLOT(seleccionar()));
}

frmGestionCobros::~frmGestionCobros()
{
    delete ui;
}

void frmGestionCobros::buscar_deuda(int id_cli)
{
    this->id_cliente = id_cli;
    deudas = new QSqlQueryModel(this);
    seleccionar_factura();
    ui->tabla_deuda->setModel(deudas);
    QStringList headers;
    QVariantList sizes;
    headers << tr("id") << tr("Fecha") << tr("Vto") <<tr("Documento") << tr("Importe") <<tr("Pagado") <<tr("Pendiente");
    sizes << 0 << 100 << 100 << 120 << 120 << 120 <<120;

    for(int i = 0; i <headers.length();i++){
        ui->tabla_deuda->setColumnWidth(i,sizes.at(i).toInt());
        deudas->setHeaderData(i,Qt::Horizontal,headers.at(i));

    }
    ui->tabla_deuda->setColumnHidden(0,true);
    ui->tabla_deuda->setItemDelegateForColumn(1, new DateDelegate);
    ui->tabla_deuda->setItemDelegateForColumn(2, new DateDelegate);
    ui->tabla_deuda->setItemDelegateForColumn(4, new MonetaryDelegate);
    ui->tabla_deuda->setItemDelegateForColumn(5, new MonetaryDelegate);
    ui->tabla_deuda->setItemDelegateForColumn(6, new MonetaryDelegate);
}

void frmGestionCobros::buscar_deuda(int id_cli, int id_doc)
{
    this->id_cliente = id_cli;
    deudas = new QSqlQueryModel(this);
    if(this->id_factura > -1)
        seleccionar_factura();
    else
        seleccionar_ticket();
    ui->tabla_deuda->setModel(deudas);
    QStringList headers;
    QVariantList sizes;
    headers << tr("id") << tr("Fecha") << tr("Vto") <<tr("Documento") << tr("Importe") <<tr("Pagado") <<tr("Pendiente");
    sizes << 0 << 100 << 100 << 120 << 120 << 120 <<120;

    for(int i = 0; i >headers.length();i++){
        ui->tabla_deuda->setColumnWidth(i,sizes.at(i).toInt());
        deudas->setHeaderData(i,Qt::Horizontal,headers.at(i));

    }
    ui->tabla_deuda->setColumnHidden(0,true);
    ui->tabla_deuda->setItemDelegateForColumn(1, new DateDelegate);
    ui->tabla_deuda->setItemDelegateForColumn(2, new DateDelegate);
    ui->tabla_deuda->setItemDelegateForColumn(4, new MonetaryDelegate);
    ui->tabla_deuda->setItemDelegateForColumn(5, new MonetaryDelegate);
    ui->tabla_deuda->setItemDelegateForColumn(6, new MonetaryDelegate);
}

void frmGestionCobros::titulo(QString titulo)
{
    ui->lblNombre->setText(titulo);
}
int frmGestionCobros::getId_factura() const
{
    return id_factura;
}

void frmGestionCobros::setId_factura(int value)
{
    id_factura = value;
}
int frmGestionCobros::getId_ticket() const
{
    return id_ticket;
}

void frmGestionCobros::setId_ticket(int value)
{
    id_ticket = value;
}



void frmGestionCobros::seleccionar_factura()
{
    QString cSQL;
    if(ui->radTodos->isChecked())
    {
        if(this->id_factura = -1)
            cSQL = QString("select id,fecha, vencimiento,documento,importe,pagado,"
                               "pendiente_cobro from clientes_deuda where id_cliente = %1").arg(this->id_cliente);
        else
            cSQL = QString("select id,fecha, vencimiento,documento,importe,pagado,"
                               "pendiente_cobro from clientes_deuda "
                           "where id_cliente = %1 and documento = %2").arg(this->id_cliente,this->id_factura);
    } else if(ui->radPendientes->isChecked())
    {
        if(this->id_factura = -1)
            cSQL = QString("select id,fecha, vencimiento,documento,importe,pagado,"
                               "pendiente_cobro from clientes_deuda where id_cliente = %1 and pendiente_cobro >0" ).arg(this->id_cliente);
        else
            cSQL = QString("select id,fecha, vencimiento,documento,importe,pagado,"
                               "pendiente_cobro from clientes_deuda "
                           "where id_cliente = %1 and documento = %2 and pendiente_cobro >0").arg(this->id_cliente,this->id_factura);
    } else if(ui->radPagados->isChecked())
    {
        if(this->id_factura = -1)
            cSQL = QString("select id,fecha, vencimiento,documento,importe,pagado,"
                               "pendiente_cobro from clientes_deuda where id_cliente = %1 and pendiente_cobro =0" ).arg(this->id_cliente);
        else
            cSQL = QString("select id,fecha, vencimiento,documento,importe,pagado,"
                               "pendiente_cobro from clientes_deuda "
                           "where id_cliente = %1 and documento = %2 and pendiente_cobro =0").arg(this->id_cliente,this->id_factura);
    }
    deudas->setQuery(cSQL,Configuracion_global->groupDB);

}

void frmGestionCobros::seleccionar_ticket()
{

}

void frmGestionCobros::seleccionar_varios()
{
    QString cSQL;
    if(ui->chkCliente->isChecked())
    {
        cSQL = QString("select id,fecha, vencimiento,documento,importe,pagado,"
                           "pendiente_cobro from clientes_deuda "
                       "where id_cliente = %1  and vencimiento between %2 and %3").arg(QString::number(this->id_cliente),
                                                                                 ui->txtfecha_ini->date().toString("yyyyMMdd"),
                                                                                 ui->txtfecha_fin->date().toString("yyyyMMdd"));
    } else
    {
        cSQL = QString("select id,fecha, vencimiento,documento,importe,pagado,"
                       "pendiente_cobro from clientes_deuda "
                       "where vencimiento between %1 and %3").arg(ui->txtfecha_ini->date().toString("yyyyMMdd"),
                                                            ui->txtfecha_fin->date().toString("yyyyMMdd"));
    }
    deudas->setQuery(cSQL,Configuracion_global->groupDB);
    ui->tabla_deuda->setModel(deudas);
}


void frmGestionCobros::on_txtbuscar_cliente_textChanged(const QString &arg1)
{
    QSqlQueryModel *modelo_clientes = new QSqlQueryModel(this);
    QString indice,cSQL;
    if(ui->radNombre->isChecked())
        indice = "nombre_fiscal";
    if(ui->radCif->isChecked())
        indice = "cif_nif";
    if(ui->radCodigo->isChecked())
        indice = "cif_nif";
    if (ui->radEmpieza->isChecked())
    {
        cSQL = "select id,codigo_cliente, nombre_fiscal, cif_nif from clientes where "+indice+
                " like '"+arg1.trimmed()+"%'";
        modelo_clientes->setQuery(cSQL,Configuracion_global->groupDB);
    } else {
        cSQL= "select id,codigo_cliente, nombre_fiscal, cif_nif from clientes where "+indice+
                                          " like '%"+arg1.trimmed()+"%'";
        modelo_clientes->setQuery(cSQL,Configuracion_global->groupDB);
    }

    ui->tabla_clientes->setModel(modelo_clientes);
    ui->tabla_clientes->setColumnHidden(0,true);
    ui->tabla_clientes->setColumnWidth(1,70);
    ui->tabla_clientes->setColumnWidth(2,170);
    ui->tabla_clientes->setColumnWidth(3,90);
    modelo_clientes->setHeaderData(1,Qt::Horizontal,tr("Código"));
    modelo_clientes->setHeaderData(2,Qt::Horizontal,tr("Denominación"));
    modelo_clientes->setHeaderData(3,Qt::Horizontal,tr("Cif/Nif"));


}

void frmGestionCobros::on_btncobro_total_clicked()
{
    FrmGestionCobros2 g2(this);
    QModelIndex index = ui->tabla_deuda->currentIndex();

    double imp = ui->tabla_deuda->model()->data(ui->tabla_deuda->model()->index(index.row(),4)).toDouble();
    int id = ui->tabla_deuda->model()->data(ui->tabla_deuda->model()->index(index.row(),0)).toDouble();
    g2.setImporte(imp,id);
    g2.setId_factura(this->id_factura);
    if(g2.exec() == QDialog::Accepted)
    {
        buscar_deuda(this->id_cliente);
    }


}

void frmGestionCobros::on_btnCerrar_clicked()
{
    accept();
}

void frmGestionCobros::on_txtfecha_fin_editingFinished()
{
    seleccionar_varios();
}

void frmGestionCobros::on_chkCliente_clicked(bool checked)
{
    ui->frame_pagados->setVisible(!checked);
}

void frmGestionCobros::on_txtfecha_ini_editingFinished()
{
    seleccionar_varios();
}
