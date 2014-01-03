#include "frmeditline.h"
#include "ui_frmeditline.h"

frmEditLine::frmEditLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmEditLine)
{
    ui->setupUi(this);
}

frmEditLine::~frmEditLine()
{
    delete ui;
}
