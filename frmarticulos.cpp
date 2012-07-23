#include "frmarticulos.h"
#include "ui_frmarticulos.h"

FrmArticulos::FrmArticulos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmArticulos)
{
    ui->setupUi(this);
}

FrmArticulos::~FrmArticulos()
{
    delete ui;
}
