#include "frmpersonascontactocliente.h"
#include "ui_frmpersonascontactocliente.h"

frmPersonasContactoCliente::frmPersonasContactoCliente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmPersonasContactoCliente)
{
    ui->setupUi(this);
    RefrescarTabla();
}

frmPersonasContactoCliente::~frmPersonasContactoCliente()
{
    delete ui;
}

void frmPersonasContactoCliente::RefrescarTabla()
{

}


void frmPersonasContactoCliente::Anadir()
{

}

void frmPersonasContactoCliente::Borrar()
{
}
