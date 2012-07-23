#include "frmproveedores.h"
#include "ui_frmproveedores.h"

frmProveedores::frmProveedores(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmProveedores)
{
    ui->setupUi(this);
}

frmProveedores::~frmProveedores()
{
    delete ui;
}
