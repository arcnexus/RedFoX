#include "frmvales.h"
#include "ui_frmvales.h"

FrmVales::FrmVales(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmVales)
{
    ui->setupUi(this);
}

FrmVales::~FrmVales()
{
    delete ui;
}
