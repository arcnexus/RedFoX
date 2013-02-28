#include "frmdependient.h"
#include "ui_frmdependient.h"

FrmDependient::FrmDependient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmDependient)
{
    ui->setupUi(this);
}

FrmDependient::~FrmDependient()
{
    delete ui;
}
