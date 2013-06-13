#include "configmonedatpv.h"
#include "ui_configmonedatpv.h"

configMonedaTpv::configMonedaTpv(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::configMonedaTpv)
{
    ui->setupUi(this);
}

configMonedaTpv::~configMonedaTpv()
{
    delete ui;
}
