#include "frmgastos_ped_pro.h"
#include "ui_frmgastos_ped_pro.h"

frmgastos_ped_pro::frmgastos_ped_pro(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmgastos_ped_pro)
{
    ui->setupUi(this);
}

frmgastos_ped_pro::~frmgastos_ped_pro()
{
    delete ui;
}

void frmgastos_ped_pro::on_btnAnadir_clicked()
{
    QHash <QString,QVariant> h;
    QString  error;
    h["id_cab"] = this->id_cab;
    h["descripcion"] = ui->txtDescripcion->text();
    h["importe"] = Configuracion_global->MonedatoDouble(ui->txtImporte->text());
    int new_id = SqlCalls::SqlInsert(h,"gastos_ped_pro",Configuracion_global->empresaDB,error);
    if(new_id <0)
        QMessageBox::warning(this,tr("Gestión de pedidos"),tr("Ocurrió un error al insertar: %1").arg(error));
    accept();
}

void frmgastos_ped_pro::setId_cabecera(int id_cab)
{
    this->id_cab = id_cab;
}
