#include "frmextrascaja.h"
#include "ui_frmextrascaja.h"

frmExtrasCaja::frmExtrasCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmExtrasCaja)
{
    ui->setupUi(this);
}

frmExtrasCaja::~frmExtrasCaja()
{
    delete ui;
}
