#include "frmabrircaja.h"
#include "ui_frmabrircaja.h"

FrmAbrirCaja::FrmAbrirCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAbrirCaja)
{
    ui->setupUi(this);
}

FrmAbrirCaja::~FrmAbrirCaja()
{
    delete ui;
}

void FrmAbrirCaja::on_pushButton_clicked()
{

}
