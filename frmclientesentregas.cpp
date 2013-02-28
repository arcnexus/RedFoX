#include "frmclientesentregas.h"
#include "ui_frmclientesentregas.h"

FrmClientesEntregas::FrmClientesEntregas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmClientesEntregas)
{
    ui->setupUi(this);
}

FrmClientesEntregas::~FrmClientesEntregas()
{
    delete ui;
}
