#include "dlgpedidoalbfact.h"
#include "ui_dlgpedidoalbfact.h"
#include "../Auxiliares/monetarydelegate.h"

DlgPedidoAlbFact::DlgPedidoAlbFact(QString label, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPedidoAlbFact)
{
    ui->setupUi(this);
    ui->label->setText(label);
    this->setWindowTitle(label);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

DlgPedidoAlbFact::~DlgPedidoAlbFact()
{
    delete ui;
}

void DlgPedidoAlbFact::on_btnAccept_clicked()
{

}

void DlgPedidoAlbFact::calcular(double)
{
    double base , iva , re , total = 0.0;
    for(auto i=0 ; i< ui->tablaLineas->rowCount(); ++i)
    {
        int id = ui->tablaLineas->item(i,0)->data(Qt::DisplayRole).toInt();
        double cantidad = qobject_cast<QDoubleSpinBox*>(ui->tablaLineas->cellWidget(i,5))->value();
        double art_base = cantidad * lineas.value(id).value("precio").toDouble();
        base += art_base;
        iva += art_base * lineas.value(id).value("porc_iva").toDouble();

    }
}

QMap<int, QSqlRecord> DlgPedidoAlbFact::getLineas() const
{
    return lineas;
}

void DlgPedidoAlbFact::setLineas(const QMap<int, QSqlRecord> &value)
{
    lineas = value;
    ui->tablaLineas->clear();
    ui->tablaLineas->setRowCount(0);
    ui->tablaLineas->setColumnCount(8);

    QStringList cabecera;
    cabecera <<tr("id") << tr("C.prov") << tr("Descripción")
            <<tr("Recibidos") <<tr("En Albarán anterior") << tr("Este Albarán") <<tr("Precio Ud.");
    ui->tablaLineas->setHorizontalHeaderLabels(cabecera);

    int pos = 0;
    for(auto l = lineas.cbegin() ; l != lineas.cend() ; ++l)
    {
        ui->tablaLineas->setRowCount(pos+1);
        const QSqlRecord r = l.value();
        QTableWidgetItem *item_columna0 = new QTableWidgetItem(r.value("id").toString());
        item_columna0->setFlags(item_columna0->flags() & (~Qt::ItemIsEditable));
        item_columna0->setTextColor(Qt::blue);
        ui->tablaLineas->setItem(pos,0,item_columna0);
        ui->tablaLineas->setColumnHidden(0,true);

        QTableWidgetItem *item_columna1 = new QTableWidgetItem(r.value("codigo").toString());
        item_columna1->setFlags(item_columna1->flags() & (~Qt::ItemIsEditable));
        item_columna1->setTextColor(Qt::blue);
        ui->tablaLineas->setItem(pos,1,item_columna1);

        QTableWidgetItem *item_columna3 = new QTableWidgetItem(r.value("descripcion").toString());
        item_columna3->setFlags(item_columna3->flags() & (~Qt::ItemIsEditable));
        item_columna3->setTextColor(Qt::blue);
        ui->tablaLineas->setItem(pos,2,item_columna3);
        ui->tablaLineas->setColumnWidth(2,140);


        QTableWidgetItem *item_columna6 = new QTableWidgetItem(r.value("cantidad_recibida").toString());
        if(r.value("cantidad_recibida").toInt()>0)
            item_columna6->setBackgroundColor(Qt::yellow);
        if(r.value("cantidad_pendiente").toInt() <=0)
            item_columna6->setBackgroundColor(Qt::green);
        if(r.value("cantidad_recibida").toInt()==0 )
            item_columna6->setBackgroundColor(Qt::yellow);

        item_columna6->setTextColor(Qt::blue); // color de los items
        item_columna6->setFlags(item_columna6->flags() & (~Qt::ItemIsEditable));
        item_columna6->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tablaLineas->setItem(pos,3,item_columna6);

        QTableWidgetItem *item_columna4 = new QTableWidgetItem(r.value("en_albaran").toString());
        item_columna4->setFlags(item_columna1->flags() & (~Qt::ItemIsEditable));
        item_columna4->setTextColor(Qt::blue);
        ui->tablaLineas->setItem(pos,4,item_columna4);

        QDoubleSpinBox* sp = new QDoubleSpinBox(this);
        sp->setMaximum(r.value("cantidad_recibida").toDouble() - r.value("en_albaran").toDouble());
        sp->setMinimum(0);
        sp->setValue(sp->maximum());
        connect(sp,SIGNAL(valueChanged(double)),this,SLOT(calcular(double)));
        ui->tablaLineas->setCellWidget(pos,5,sp);

        QTableWidgetItem *item_columna8 = new QTableWidgetItem(QString::number(r.value("precio").toDouble(),'f',Configuracion_global->decimales));
        item_columna8->setTextColor(Qt::black); // color de los items
        ui->tablaLineas->setItem(pos,6,item_columna8);


        QTableWidgetItem *item_columna10 = new QTableWidgetItem(r.value("id_articulo").toString());
        item_columna10->setTextColor(Qt::black); // color de los items
        item_columna10->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tablaLineas->setItem(pos,7,item_columna10);
        ui->tablaLineas->setColumnHidden(7,true);

        pos++;
    }
    ui->tablaLineas->setItemDelegateForColumn(6,new MonetaryDelegate(this));
    calcular(0.0);
}

