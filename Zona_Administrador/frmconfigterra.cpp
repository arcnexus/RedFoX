#include "frmconfigterra.h"
#include "ui_frmconfigterra.h"

frmConfigTerra::frmConfigTerra(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmConfigTerra)
{
    ui->setupUi(this);
}

frmConfigTerra::~frmConfigTerra()
{
    delete ui;
}
