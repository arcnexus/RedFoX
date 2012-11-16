#include "frmbuscarcie.h"
#include "ui_frmbuscarcie.h"

FrmBuscarCIE::FrmBuscarCIE(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmBuscarCIE)
{
    ui->setupUi(this);
}

FrmBuscarCIE::~FrmBuscarCIE()
{
    delete ui;
}
