#include "frmrecibos.h"
#include "ui_frmrecibos.h"

FrmRecibos::FrmRecibos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmRecibos)
{
    ui->setupUi(this);
}

FrmRecibos::~FrmRecibos()
{
    delete ui;
}
