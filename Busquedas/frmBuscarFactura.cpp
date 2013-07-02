#include "frmBuscarFactura.h"
#include "ui_frmBuscarFactura.h"

FrmBuscarFactura::FrmBuscarFactura(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmBuscarFactura)
{
    ui->setupUi(this);
   // this::connect(this,SIGNAL(enviar_id_factura(int)),frmFacturas,SLOT(set_id_factura(int)));
    ui->FechaFin->setDate(QDate::currentDate());
    ui->FechaInicio->setDisplayFormat("dd/MM/yyyy");
    ui->FechaFin->setDisplayFormat("dd/MM/yyyy");

}

FrmBuscarFactura::~FrmBuscarFactura()
{
    delete ui;
}

void FrmBuscarFactura::on_pushButton_clicked()
{
    // 1º - Preparamos la Select y la asociamos a un QSqlQueryModel

    //QString QDate::toString ( Qt::DateFormat format = Qt::TextDate ) const
    QDate fecha1,fecha2;
    QString cFecha1,cFecha2,cImporteMaximo;
    fecha1 = ui->FechaInicio->date();
    fecha2 = ui->FechaFin->date();
    cFecha1 = fecha1.toString("yyyy-MM-dd");
    cFecha2 = fecha2.toString("yyyy-MM-dd");
    cImporteMaximo = ui->ImporteMaximo->text();
    cImporteMaximo.replace(".","");
    QString  cSQL;
    cSQL ="Select id,factura,cliente,fecha,fecha_cobro,subtotal,importe_descuento,importe_descuento_pp,"
          "base,iva,total from cab_fac where cliente like '%"+ui->txtBuscar->text().trimmed()+"%' and "
          "fecha >= '"+cFecha1+"' and fecha <= '"+cFecha2+"' and total >= "+QString::number(ui->Importeminimo->text().toDouble(),'f',2)+
          " and total <="+QString::number(cImporteMaximo.toDouble(),'f',2)+" order by factura";

    ModelFacturas = new QSqlQueryModel();
    ModelFacturas->setQuery(cSQL,QSqlDatabase::database("empresa"));
    ui->Grid->setModel(ModelFacturas);
    ui->Grid->setGridStyle(Qt::DotLine);

    // 2º - Creamos Objeto de la clase Cabecera
    Cabecera = new QHeaderView(Qt::Horizontal,this);
    // 3º - La vinculamos a QTableView
    ui->Grid->setHorizontalHeader(Cabecera);
  //  Cabecera->setSectionResizeMode(0,QHeaderView::Fixed);
    Cabecera->resizeSection(0,0);
  //  Cabecera->setSectionResizeMode(1,QHeaderView::Fixed);
    Cabecera->resizeSection(1,100);
  //  Cabecera->setSectionResizeMode(2,QHeaderView::Fixed);
    Cabecera->resizeSection(2,200);
  //  Cabecera->setSectionResizeMode(3,QHeaderView::Fixed);
    Cabecera->resizeSection(3,90);
  //  Cabecera->setSectionResizeMode(4,QHeaderView::Fixed);
    Cabecera->resizeSection(4,90);
  //  Cabecera->setSectionResizeMode(5,QHeaderView::Fixed);
    Cabecera->resizeSection(5,90);
   // Cabecera->setSectionResizeMode(6,QHeaderView::Fixed);
    Cabecera->resizeSection(6,90);
   // Cabecera->setSectionResizeMode(7,QHeaderView::Fixed);
    Cabecera->resizeSection(7,90);
  //  Cabecera->setSectionResizeMode(8,QHeaderView::Fixed);
    Cabecera->resizeSection(8,90);
  //  Cabecera->setSectionResizeMode(9,QHeaderView::Fixed);
    Cabecera->resizeSection(9,90);
   // Cabecera->setSectionResizeMode(10,QHeaderView::Fixed);
    Cabecera->resizeSection(10,90);
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
    //emit enviar_id_factura( ui->txtBuscar->text().toInt());
}

int FrmBuscarFactura::Devolverid() {
    QModelIndex celda=ui->Grid->currentIndex();
    QModelIndex index=ModelFacturas->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=ModelFacturas->data(index,Qt::EditRole);
   return pKey.toInt();
}


