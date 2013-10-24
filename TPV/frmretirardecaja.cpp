#include "frmretirardecaja.h"
#include "ui_frmretirardecaja.h"

FrmRetirardeCaja::FrmRetirardeCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmRetirardeCaja)
{
    ui->setupUi(this);
}

FrmRetirardeCaja::~FrmRetirardeCaja()
{
    delete ui;
}
