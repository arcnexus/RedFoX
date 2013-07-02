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
    QString cSQL = "select id, nombre, cargo_empresa, desc_telefono1,telefono1, desc_telefono2, "
            "telefono2, desc_movil1, movil,email"
            " from Personascontactocliente where  id_cliente= "+ QString::number(nid_cliente);
    lista->setQuery(cSQL,QSqlDatabase::database("Maya"));
;

    ui->tabla_PersonasContacto->setModel(lista);
    ui->tabla_PersonasContacto->setColumnHidden(0,true);
    ui->tabla_PersonasContacto->setColumnWidth(1,120);
    ui->tabla_PersonasContacto->setColumnWidth(2,120);
    ui->tabla_PersonasContacto->setColumnWidth(3,100);
    ui->tabla_PersonasContacto->setColumnWidth(4,100);
    ui->tabla_PersonasContacto->setColumnWidth(5,100);
    ui->tabla_PersonasContacto->setColumnWidth(6,100);
    ui->tabla_PersonasContacto->setColumnWidth(7,100);
    ui->tabla_PersonasContacto->setColumnWidth(8,100);
    ui->tabla_PersonasContacto->setColumnWidth(9,150);

    QStringList cabecera;
    cabecera << tr("id") <<tr("Nombre") <<tr("Cargo") <<tr("Desc.Tel") << tr("Teléfono")
             << tr("Desc. Tel") << tr("Teléfono") << tr("Desc. Movil") << tr("Movil") << tr("Email");
    for (int cab =0; cab<cabecera.length(); cab++)
    {
        lista->setHeaderData(cab,Qt::Horizontal,cabecera.at(cab));
    }

}


void frmPersonasContactoCliente::Anadir()
{
   oCliente.AnadirPersonaContacto(nid_cliente,ui->txtNombre->text(),ui->txtDescripcionT1->text(),
                                  ui->txtTelefono1->text(),ui->txtDescripcionT2->text(),ui->txtTelefono2->text(),
                                  ui->txtDescripcionT3->text(),ui->txtTelefono3->text(),ui->txtDescripcionM1->text(),
                                  ui->txtMovil1->text(),ui->txtDescripcionM2->text(),ui->txtMovil2->text(),ui->txtCargo->text(),
                                  ui->txtMail->text());
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
   ui->txtMail->setText("");
   ui->txtNombre->setFocus();

   RefrescarTabla();
}
void frmPersonasContactoCliente::on_btnEliminar_clicked()
{
    int row = ui->tabla_PersonasContacto->currentIndex().row();
    int id = ui->tabla_PersonasContacto->model()->data(ui->tabla_PersonasContacto->model()->index(row,0)).toInt();
    if(oCliente.BorrarPersona_contacto(id))
        RefrescarTabla();

}

void frmPersonasContactoCliente::on_tabla_PersonasContacto_clicked(const QModelIndex &index)
{
    int id = ui->tabla_PersonasContacto->model()->data(ui->tabla_PersonasContacto->model()->index(index.row(),0),Qt::EditRole).toInt();
    QSqlQuery queryPersonas(QSqlDatabase::database("Maya"));
    if(queryPersonas.exec("select * from Personascontactocliente where id = "+QString::number(id)))
    {
        queryPersonas.next();
        this->id = id;
        ui->btnGuardar->setEnabled(true);
        ui->btnAnadir->setEnabled(false);
        ui->txtNombre->setText(queryPersonas.record().value("nombre").toString());
        ui->txtCargo->setText(queryPersonas.record().value("cargo_empresa").toString());
        ui->txtDescripcionM1->setText(queryPersonas.record().value("desc_movil1").toString());
        ui->txtDescripcionM2->setText(queryPersonas.record().value("desc_movil2").toString());
        ui->txtMovil1->setText(queryPersonas.record().value("movil").toString());
        ui->txtMovil2->setText(queryPersonas.record().value("movil2").toString());
        ui->txtDescripcionT1->setText(queryPersonas.record().value("desc_telefono1").toString());
        ui->txtDescripcionT2->setText(queryPersonas.record().value("desc_telefono2").toString());
        ui->txtDescripcionT3->setText(queryPersonas.record().value("desc_telefono3").toString());
        ui->txtTelefono1->setText(queryPersonas.record().value("telefono1").toString());
        ui->txtTelefono2->setText(queryPersonas.record().value("telefono2").toString());
        ui->txtTelefono3->setText(queryPersonas.record().value("telefono3").toString());
        ui->txtMail->setText(queryPersonas.record().value("email").toString());
        this->id_cliente = queryPersonas.record().value("id_cliente").toInt();
    }

}

void frmPersonasContactoCliente::on_btnGuardar_clicked()
{
    ui->btnAnadir->setEnabled(true);
    ui->btnGuardar->setEnabled(false);

    oCliente.GuardarPersonaContacto(this->id, ui->txtNombre->text(),ui->txtDescripcionT1->text(),
                                   ui->txtTelefono1->text(),ui->txtDescripcionT2->text(),ui->txtTelefono2->text(),
                                   ui->txtDescripcionT3->text(),ui->txtTelefono3->text(),ui->txtDescripcionM1->text(),
                                   ui->txtMovil1->text(),ui->txtDescripcionM2->text(),ui->txtMovil2->text(),ui->txtCargo->text(),
                                   ui->txtMail->text(),this->id_cliente);
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
    ui->txtMail->setText("");
    ui->txtNombre->setFocus();

    RefrescarTabla();

}
