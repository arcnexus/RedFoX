#include "frmreservas.h"
#include "ui_frmreservas.h"

FrmReservas::FrmReservas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmReservas)
{
    ui->setupUi(this);
}

FrmReservas::~FrmReservas()
{
    delete ui;
}
