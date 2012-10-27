#include "frmtareas.h"
#include "ui_frmtareas.h"

FrmTareas::FrmTareas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmTareas)
{
    ui->setupUi(this);
}

FrmTareas::~FrmTareas()
{
    delete ui;
}
