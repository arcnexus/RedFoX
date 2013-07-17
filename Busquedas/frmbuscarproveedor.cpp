#include "frmbuscarproveedor.h"
#include "ui_frmbuscarproveedor.h"


FrmBuscarProveedor::FrmBuscarProveedor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmBuscarProveedor)
{
    ui->setupUi(this);
  //  connect(ui->btnBuscar,SIGNAL(clicked()), this,SLOT(on_)

}

FrmBuscarProveedor::~FrmBuscarProveedor()
{
    delete ui;
}

void FrmBuscarProveedor::on_btnBuscar_clicked()
{
    modelo = new QSqlQueryModel(this);
    modelo->setQuery("select id, proveedor, codigo,cif from proveedores where proveedor like'"+ui->txtcBuscar->text()+"%'",
                     Configuracion_global->groupDB);
    ui->tableProv->setModel(modelo);
    ui->tableProv->setColumnHidden(0,true);
    modelo->setHeaderData(1,Qt::Horizontal,tr("Proveedor"));
    ui->tableProv->setColumnWidth(1,300);
    modelo->setHeaderData(2,Qt::Horizontal,tr("Código"));
    modelo->setHeaderData(3,Qt::Horizontal,tr("CIF"));


}

void FrmBuscarProveedor::Aceptar()
{
    QModelIndex celda=ui->tableProv->currentIndex();
    QModelIndex index=modelo->index(celda.row(),0);     ///< '0' id

    QVariant pKey=modelo->data(index,Qt::EditRole);
    this->nidProv = pKey.toInt();
    accept();
}

