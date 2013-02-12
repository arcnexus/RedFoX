#include "frmfacturaralabaranes.h"
#include "ui_frmfacturaralabaranes.h"

FrmFacturarAlabaranes::FrmFacturarAlabaranes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmFacturarAlabaranes)
{
    ui->setupUi(this);
}

FrmFacturarAlabaranes::~FrmFacturarAlabaranes()
{
    delete ui;
}
