#include "frmclientesdeuda.h"
#include "ui_frmclientesdeuda.h"

FrmClientesDeuda::FrmClientesDeuda(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmClientesDeuda)
{
    ui->setupUi(this);
}

FrmClientesDeuda::~FrmClientesDeuda()
{
    delete ui;
}
