#include "frmasociarproveedor.h"
#include "ui_frmasociarproveedor.h"

FrmAsociarProveedor::FrmAsociarProveedor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAsociarProveedor)
{
    ui->setupUi(this);
}

FrmAsociarProveedor::~FrmAsociarProveedor()
{
    delete ui;
}
