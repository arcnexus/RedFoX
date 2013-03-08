#include "frmasociarproveedor.h"
#include "ui_frmasociarproveedor.h"


FrmAsociarProveedor::FrmAsociarProveedor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAsociarProveedor)
{
    ui->setupUi(this);
    modelProveedor = new QSqlQueryModel(this);
    modelProveedor->setQuery("select cProveedor from proveedores",QSqlDatabase::database("terra"));
    ui->tablaproveedores->setModel(modelProveedor);
}

FrmAsociarProveedor::~FrmAsociarProveedor()
{
    delete ui;
}
