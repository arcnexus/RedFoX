#include "frmcajaminuta.h"
#include "ui_frmcajaminuta.h"

FrmCajaMinuta::FrmCajaMinuta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmCajaMinuta)
{
    ui->setupUi(this);
}

FrmCajaMinuta::~FrmCajaMinuta()
{
    delete ui;
}
