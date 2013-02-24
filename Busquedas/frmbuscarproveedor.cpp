#include "frmbuscarproveedor.h"
#include "ui_frmbuscarproveedor.h"
#include <QtSql>
#include <QSqlDatabase>
#include<QSqlQueryModel>

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
    modelo->setQuery("select cProveedor, cCodigo,cCif,id from proveedores where cProveedor like'"+ui->txtcBuscar->text()+"%'",
                     QSqlDatabase::database("terra"));
    ui->tableProv->setModel(modelo);

}

void FrmBuscarProveedor::Aceptar()
{
    QModelIndex celda=ui->tableProv->currentIndex();
    QModelIndex index=modelo->index(celda.row(),3);     ///< '3' id

    QVariant pKey=modelo->data(index,Qt::EditRole);
    this->nIdProv = pKey.toInt();
    accept();
}

