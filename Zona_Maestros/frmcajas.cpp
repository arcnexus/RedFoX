#include "frmcajas.h"
#include "ui_frmcajas.h"

FrmCajas::FrmCajas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmCajas)
{
    ui->setupUi(this);
}

FrmCajas::~FrmCajas()
{
    delete ui;
}
