#include "frmagenda.h"
#include "ui_frmagenda.h"

frmAgenda::frmAgenda(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmAgenda)
{
    ui->setupUi(this);
}

frmAgenda::~frmAgenda()
{
    delete ui;
}
