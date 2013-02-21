#include "frmpersonascontactocliente.h"
#include "ui_frmpersonascontactocliente.h"
#include <QtSql>
#include <QSqlDatabase>

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
    QSqlQueryModel *lista = new QSqlQueryModel(this);
    lista->setQuery("select id, nombre,  desctelefono1,telefono1, desctelefono2, "
                    "telefono2, desctelefono3, telefono3, descmovil1, movil, descmovil2, movil2 where  idcliente= "+nIdCliente);
    ui->tabla_PersonasContacto->setModel(lista);

}


void frmPersonasContactoCliente::Anadir()
{
   //oCliente.
}

void frmPersonasContactoCliente::Borrar()
{
}
