#include "gestion_seccionalmacen.h"
#include "ui_gestion_seccionalmacen.h"

gestion_SeccionAlmacen::gestion_SeccionAlmacen(QWidget *parent,  QString tabla, int tipo ) :
    QDialog(parent),
    ui(new Ui::gestion_SeccionAlmacen)
{
    this->tabla = tabla;
    ui->setupUi(this);
    if(tipo == solo_ver)
    {
        ui->edit_container->hide();
        ui->btn_aceptar->setText("Seleccionar");
    }
    else
    {
        ui->btn_cancel->hide();
    }
    model = new QSqlRelationalTableModel(this,QSqlDatabase::database("empresa"));
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    ui->resultado_list->setItemDelegate(new QSqlRelationalDelegate(ui->resultado_list));
    model->setTable(tabla);
    ui->resultado_list->setModel(model);
    ui->resultado_list->hideColumn(0);


    if(tabla == "secciones")
    {
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Secciones"));
    }
    else if(tabla == "familias")
    {
        model->setRelation(3,QSqlRelation("secciones","id","cSeccion"));

        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Codigo"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Familia"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Pertenece a"));
    }
    else if(tabla == "subfamilias")
    {
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("SubFamilias"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Pertenece a"));
        model->setRelation(2,QSqlRelation("familias","id","cFamilia"));
    }

    model->select();
}

gestion_SeccionAlmacen::~gestion_SeccionAlmacen()
{
    delete ui;
}

void gestion_SeccionAlmacen::filter(QString filter)
{
    model->setFilter(filter);
    model->select();
}

void gestion_SeccionAlmacen::on_btn_add_clicked()
{
    model->insertRow(model->rowCount());
}

void gestion_SeccionAlmacen::on_btn_remove_clicked()
{
    model->removeRow(ui->resultado_list->currentIndex().row());
}

void gestion_SeccionAlmacen::on_btn_cancel_clicked()
{
    gestion_SeccionAlmacen::done(QDialog::Rejected);
}

void gestion_SeccionAlmacen::on_btn_aceptar_clicked()
{
    gestion_SeccionAlmacen::done(QDialog::Accepted);
}

void gestion_SeccionAlmacen::on_btn_save_clicked()
{
    model->submitAll();
}



void gestion_SeccionAlmacen::on_resultado_list_clicked(const QModelIndex &index)
{
    int row = index.row();
    QSqlRecord r = model->record(row);
    value = (tabla == "familias") ? r.value(2).toString() : r.value(1).toString();
}
