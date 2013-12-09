#include "frmdevolucion2.h"
#include "ui_frmdevolucion2.h"

frmdevolucion2::frmdevolucion2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmdevolucion2)
{
    ui->setupUi(this);

}

frmdevolucion2::~frmdevolucion2()
{
    delete ui;
}


void frmdevolucion2::set_values(int id, QString codigo, QString descripcion, float cantidad,float dev_act)
{
    this->id = id;
    ui->txtcodigo->setText(codigo);
    ui->txtdescripcion->setText(descripcion);
    ui->txtcantidad->setText(QString::number(cantidad,'f',2));
    ui->txtdev_act->setText(QString::number(cantidad,'f',2));
    ui->txtdev_act->setFocus();
    ui->txtdev_act->selectAll();
}


void frmdevolucion2::on_btnAceptar_clicked()
{
    QHash<QString, QVariant> h;
    h["dev_act"] = ui->txtdev_act->text().toFloat();
    QString error;
    bool success = SqlCalls::SqlUpdate(h,"lin_tpv",Configuracion_global->empresaDB,QString("id=%1").arg(this->id),error);
    if(!success)
        QMessageBox::warning(this,tr("Devoluciones"),tr("Ocurrió un error al añadir devolución: %1").arg(error),
                             tr("Aceptar"));
    accept();

}
