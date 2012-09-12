#include "frmempresas.h"
#include "ui_frmempresas.h"

FrmEmpresas::FrmEmpresas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmEmpresas)
{
    ui->setupUi(this);
    ui->txtMensaje->setText("ATENCION: Modifique con cuidado. Cambiar estos\n valores puede hacer que el programa deje\n de funcionar");
}

FrmEmpresas::~FrmEmpresas()
{
    delete ui;
}
