#include "frminsertardinerocaja.h"
#include "ui_frminsertardinerocaja.h"

FrmInsertarDineroCaja::FrmInsertarDineroCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmInsertarDineroCaja)
{
    ui->setupUi(this);
}

FrmInsertarDineroCaja::~FrmInsertarDineroCaja()
{
    delete ui;
}
