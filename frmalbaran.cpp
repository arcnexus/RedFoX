#include "frmalbaran.h"
#include "ui_frmalbaran.h"

FrmAlbaran::FrmAlbaran(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAlbaran)
{
    ui->setupUi(this);
}

FrmAlbaran::~FrmAlbaran()
{
    delete ui;
}
