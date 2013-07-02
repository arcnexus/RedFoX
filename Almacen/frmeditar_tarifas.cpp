#include "frmeditar_tarifas.h"
#include "ui_frmeditar_tarifas.h"

frmEditar_tarifas::frmEditar_tarifas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmEditar_tarifas)
{
    ui->setupUi(this);
}

frmEditar_tarifas::~frmEditar_tarifas()
{
    delete ui;
}

void frmEditar_tarifas::setFiltro(int id)
{
    QSqlQuery tarifa(QSqlDatabase::database("Maya"));
    if(tarifa.exec("select id,pvp,pais,moneda,simbolo,codigo_tarifa,descripcion,margen,margen_minimo "
                          "from viewtarifa where id_articulo = "+QString::number(id)))
    {
        ui->tabla_tarifa->setColumnCount(3);
        QStringList cabecera;
        cabecera <<tr("id") << tr("Cod") << tr("Tarifa");
        ui->tabla_tarifa->setHorizontalHeaderLabels(cabecera);

        int pos = 0;
        while (tarifa.next()) {
            ui->tabla_tarifa->setRowCount(pos+1);

            QTableWidgetItem *citem_columna0 = new QTableWidgetItem(tarifa.record().value("id").toString());
            citem_columna0->setFlags(citem_columna0->flags() & (~Qt::ItemIsEditable));
            citem_columna0->setTextColor(Qt::blue); // color de los items
            ui->tabla_tarifa->setItem(pos,0,citem_columna0);
            ui->tabla_tarifa->setColumnHidden(0,true);

            QTableWidgetItem *citem_columna1 = new QTableWidgetItem(tarifa.record().value("codigo_tarifa").toString());
            citem_columna1->setFlags(citem_columna1->flags() & (~Qt::ItemIsEditable));
            citem_columna1->setTextColor(Qt::blue); // color de los items
            ui->tabla_tarifa->setItem(pos,1,citem_columna1);


            QTableWidgetItem *citem_columna2 = new QTableWidgetItem(tarifa.record().value("descripcion").toString());
            citem_columna2->setFlags(citem_columna2->flags() & (~Qt::ItemIsEditable));
            citem_columna2->setTextColor(Qt::blue); // color de los items
            ui->tabla_tarifa->setItem(pos,2,citem_columna2);

            pos++;
        }
    }

}
