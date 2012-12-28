#include "frmanalitica.h"
#include "ui_frmanalitica.h"

FrmAnalitica::FrmAnalitica(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAnalitica)
{
    ui->setupUi(this);
    connect(ui->btnCerrar,SIGNAL(clicked()),this,SLOT(close()));
}

FrmAnalitica::~FrmAnalitica()
{
    delete ui;
}
