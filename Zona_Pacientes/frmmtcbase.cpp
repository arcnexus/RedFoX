#include "frmmtcbase.h"
#include "ui_frmmtcbase.h"

FrmMTCbase::FrmMTCbase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmMTCbase)
{
    ui->setupUi(this);
}

FrmMTCbase::~FrmMTCbase()
{
    delete ui;
}
