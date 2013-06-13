#include "efectivocaja.h"
#include "ui_efectivocaja.h"

EfectivoCaja::EfectivoCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EfectivoCaja)
{
    ui->setupUi(this);
}

EfectivoCaja::~EfectivoCaja()
{
    delete ui;
}
