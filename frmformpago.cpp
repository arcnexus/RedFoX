#include "frmformpago.h"
#include "ui_frmformpago.h"

FrmFormPago::FrmFormPago(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmFormPago)
{
    ui->setupUi(this);
}

FrmFormPago::~FrmFormPago()
{
    delete ui;
}
