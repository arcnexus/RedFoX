#include "frmpersonascontactocliente.h"
#include "ui_frmpersonascontactocliente.h"
#include <QtSql>
#include <QSqlDatabase>

frmPersonasContactoCliente::frmPersonasContactoCliente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmPersonasContactoCliente)
{
    ui->setupUi(this);
}

frmPersonasContactoCliente::~frmPersonasContactoCliente()
{
    delete ui;
}

void frmPersonasContactoCliente::RefrescarTabla()
{
    QSqlQueryModel *lista = new QSqlQueryModel(this);
    QString cSQL = "select id, nombre, cargo_empresa, desctelefono1,telefono1, desctelefono2, "
            "telefono2, desctelefono3, telefono3, descmovil1, movil, descmovil2, movil2"
            " from Personascontactocliente where  idcliente= "+ QString::number(nIdCliente);
    lista->setQuery(cSQL,QSqlDatabase::database("terra"));
;

    ui->tabla_PersonasContacto->setModel(lista);

}


void frmPersonasContactoCliente::Anadir()
{
   oCliente.AnadirPersonaContacto(nIdCliente,ui->txtNombre->text(),ui->txtDescripcionT1->text(),
                                  ui->txtTelefono1->text(),ui->txtDescripcionT2->text(),ui->txtTelefono2->text(),
                                  ui->txtDescripcionT3->text(),ui->txtTelefono3->text(),ui->txtDescripcionM1->text(),
                                  ui->txtMovil1->text(),ui->txtDescripcionM2->text(),ui->txtMovil2->text(),ui->txtCargo->text());
   ui->txtDescripcionM1->setText("");
   ui->txtDescripcionM2->setText("");
   ui->txtDescripcionT1->setText("");
   ui->txtDescripcionT2->setText("");
   ui->txtDescripcionT3->setText("");
   ui->txtMovil1->setText("");
   ui->txtMovil2->setText("");
   ui->txtNombre->setText("");
   ui->txtTelefono1->setText("");
   ui->txtTelefono2->setText("");
   ui->txtTelefono3->setText("");
   ui->txtCargo->setText("");
   ui->txtNombre->setFocus();

   RefrescarTabla();
}

void frmPersonasContactoCliente::Borrar()
{
}
