#include "frminventario.h"
#include "ui_frminventario.h"

frmInventario::frmInventario(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmInventario),
  menuButton(QIcon(":/Icons/PNG/inventario.png"),tr("Inventario"),this),
  shortCut(new QPushButton(QIcon(":/Icons/PNG/inventario.png"),"",this))

{
    ui->setupUi(this);
    // ------------------------
    // Tabla
    // ------------------------
    m = new QSqlTableModel(this,Configuracion_global->groupDB);
    m->setTable("inventario");
    m->setSort(1,Qt::AscendingOrder);
    m->select();
    ui->tabla->setModel(m);
    formato_tabla(m);
    //---------------------
    // combo indices
    //---------------------
    QStringList orders;
    orders << tr("código") <<tr("código Barras") <<tr("referencia proveedor") <<tr("descripción");
    ui->cboOrder->addItems(orders);
    shortCut->setStyleSheet("background-color: rgb(133, 170, 142)");
}

frmInventario::~frmInventario()
{
    delete ui;
}

void frmInventario::formato_tabla(QSqlTableModel *modelo)
{
    QStringList headers;
    QVariantList sizes;

    headers << "id" << tr("código") <<tr("C.Barras") <<tr("R. Fabricante") << tr("Descripción") << tr("Stock Real") << tr("Stock Almacen");
    sizes << 0 << 120 << 120 <<120 <<500 << 120 << 120;
    for(int i = 0; i < headers.length();i++)
    {
        ui->tabla->setColumnWidth(i,sizes.at(i).toInt());
        modelo->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
}

void frmInventario::on_txtBuscar_textEdited(const QString &arg1)
{
    QHash <QString,QString> h;
    h["código"] = "codigo";
    h["código Barras"] = "codigo_barras";
    h["referencia proveedor"] = "referencia_proveedor";
    h["descripción"] = "descripcion";

    QString indice = h.value(ui->cboOrder->currentText());
    m->setFilter(indice +" like '%"+arg1.trimmed()+"%'");
    m->select();
    formato_tabla(m);
}
