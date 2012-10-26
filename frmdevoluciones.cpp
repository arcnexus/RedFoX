#include "frmdevoluciones.h"
#include "ui_frmdevoluciones.h"

FrmDevoluciones::FrmDevoluciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmDevoluciones)
{
    ui->setupUi(this);
}

FrmDevoluciones::~FrmDevoluciones()
{
    delete ui;
}
