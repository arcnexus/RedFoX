#include "frmcierrecaja.h"
#include "ui_frmcierrecaja.h"

FrmCierreCaja::FrmCierreCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmCierreCaja)
{
    ui->setupUi(this);
}

FrmCierreCaja::~FrmCierreCaja()
{
    delete ui;
}
