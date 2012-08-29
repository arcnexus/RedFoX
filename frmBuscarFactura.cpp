#include "frmBuscarFactura.h"
#include "ui_frmBuscarFactura.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QErrorMessage>
#include<QSqlError>
#include<QDate>
#include <QSqlQueryModel>
#include <QDebug>

FrmBuscarFactura::FrmBuscarFactura(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmBuscarFactura)
{
    ui->setupUi(this);
   // this::connect(this,SIGNAL(enviar_id_Factura(int)),frmFacturas,SLOT(set_id_Factura(int)));
    ui->FechaFin->setDate(QDate::currentDate());

}

FrmBuscarFactura::~FrmBuscarFactura()
{
    delete ui;
}

void FrmBuscarFactura::on_pushButton_clicked()
{
    // 1º - Preparamos la Select y la asociamos a un QSqlQueryModel

    //QString QDate::toString ( Qt::DateFormat format = Qt::TextDate ) const
    QDate dFecha1,dFecha2;
    QString cFecha1,cFecha2,cImporteMaximo;
    dFecha1 = ui->FechaInicio->date();
    dFecha2 = ui->FechaFin->date();
    cFecha1 = dFecha1.toString("yyyy.MM.dd");
    cFecha2 = dFecha2.toString("yyyy.MM.dd");
    cImporteMaximo = ui->ImporteMaximo->text();
    cImporteMaximo.replace(".","");
    QString  cSQL;
    cSQL ="Select Id,cFactura,cCliente,dFecha,dFechaCobro,rSubtotal,rImporteDescuento,rImporteDescuentoPP,"
            "rBase,rImporteIva,rTotal from cab_fac where cCliente like '%"+ui->txtBuscar->text().trimmed()+"%' and "
            "dFecha Between '"+cFecha1+"' and '"+cFecha2+"' and rTotal Between "+QString::number(ui->Importeminimo->text().toDouble(),'f',2)+
            " and "+QString::number(cImporteMaximo.toDouble(),'f',2);

    ModelFacturas = new QSqlQueryModel();
    ModelFacturas->setQuery(cSQL,QSqlDatabase::database("empresa"));
    ui->Grid->setModel(ModelFacturas);

    // 2º - Creamos Objeto de la clase Cabecera
    Cabecera = new QHeaderView(Qt::Horizontal,this);
    // 3º - La vinculamos a QTableView
    ui->Grid->setHorizontalHeader(Cabecera);
    Cabecera->setResizeMode(0,QHeaderView::Fixed);
    Cabecera->resizeSection(0,0);
    Cabecera->setResizeMode(1,QHeaderView::Fixed);
    Cabecera->resizeSection(1,200);
    Cabecera->setResizeMode(2,QHeaderView::Fixed);
    Cabecera->resizeSection(2,300);
    Cabecera->setResizeMode(3,QHeaderView::Fixed);
    Cabecera->resizeSection(3,100);
    Cabecera->setResizeMode(4,QHeaderView::Fixed);
    Cabecera->resizeSection(4,100);
    Cabecera->setResizeMode(5,QHeaderView::Fixed);
    Cabecera->resizeSection(5,100);
    Cabecera->setResizeMode(6,QHeaderView::Fixed);
    Cabecera->resizeSection(6,100);
    Cabecera->setResizeMode(7,QHeaderView::Fixed);
    Cabecera->resizeSection(7,100);
    Cabecera->setResizeMode(8,QHeaderView::Fixed);
    Cabecera->resizeSection(8,100);
    Cabecera->setResizeMode(9,QHeaderView::Fixed);
    Cabecera->resizeSection(9,100);
    Cabecera->setResizeMode(10,QHeaderView::Fixed);
    Cabecera->resizeSection(10,100);
    Cabecera->setVisible(true);
    // 4º - Defino cabecera Modelo
    ModelFacturas->setHeaderData(1,Qt::Horizontal,tr("Factura"));
    ModelFacturas->setHeaderData(2,Qt::Horizontal,tr("Cliente"));
    ModelFacturas->setHeaderData(3,Qt::Horizontal,tr("Fecha"));
    ModelFacturas->setHeaderData(4,Qt::Horizontal,tr("Fecha Cobro"));
    ModelFacturas->setHeaderData(5,Qt::Horizontal,tr("Sub-Total"));
    ModelFacturas->setHeaderData(6,Qt::Horizontal,tr("DTO"));
    ModelFacturas->setHeaderData(7,Qt::Horizontal,tr("DTO PP"));
    ModelFacturas->setHeaderData(8,Qt::Horizontal,tr("BASE"));
    ModelFacturas->setHeaderData(9,Qt::Horizontal,tr("IVA"));
    ModelFacturas->setHeaderData(10,Qt::Horizontal,tr("TOTAL"));
    // 5º - Parametrizo valores QTableView
    ui->Grid->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Grid->setAlternatingRowColors(true);
    ui->Grid->setFocus();
    //emit enviar_id_Factura( ui->txtBuscar->text().toInt());
}

int FrmBuscarFactura::DevolverID() {
    QModelIndex celda=ui->Grid->currentIndex();
    QModelIndex index=ModelFacturas->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=ModelFacturas->data(index,Qt::EditRole);
   return pKey.toInt();
}


