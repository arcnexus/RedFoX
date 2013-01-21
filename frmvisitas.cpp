#include "frmvisitas.h"
#include "ui_frmvisitas.h"

frmVisitas::frmVisitas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmVisitas)
{
    ui->setupUi(this);
}

frmVisitas::~frmVisitas()
{
    delete ui;
}
