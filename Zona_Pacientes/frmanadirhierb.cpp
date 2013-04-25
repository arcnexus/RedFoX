#include "frmanadirhierb.h"
#include "ui_frmanadirhierb.h"

frmAnadirHierb::frmAnadirHierb(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmAnadirHierb)
{
    ui->setupUi(this);
    connect(ui->btn_buscar_producto,SIGNAL(clicked()),this,SLOT(llenar_tabla_producto_nombre()));
}

frmAnadirHierb::~frmAnadirHierb()
{
    delete ui;
}

void frmAnadirHierb::llenar_tabla_producto_nombre()
{
    Configuracion_global->AbridBDMediTec();
    QSqlQueryModel *modelProducto = new QSqlQueryModel(this);
    modelProducto->setQuery(
   "select id,nombre,laboratorio from productos where nombre like '%"+ui->txtTexto_producto->text().trimmed()+"%' ",
                QSqlDatabase::database("db_meditec"));
    ui->tabla_producto->setModel(modelProducto);
    ui->tabla_producto->setColumnHidden(0,true);
    ui->tabla_producto->setColumnWidth(1,400);
    modelProducto->setHeaderData(1,Qt::Horizontal,tr("DENOMINACIÓN PRODUCTO"),Qt::EditRole);
    modelProducto->setHeaderData(2,Qt::Horizontal,tr("LABORATORIO"),Qt::EditRole);
    Configuracion_global->CerrarBDMediTec();
}

void frmAnadirHierb::ver_vademecum()
{

}

void frmAnadirHierb::anadir_producto()
{
    switch (ui->pestanas->currentIndex()) {
    case 0:
        if (ui->tabla_producto->currentIndex().row()>=0){
            QModelIndex index = ui->tabla_producto->currentIndex();
            this->id =ui->tabla_producto->model()->data(ui->tabla_producto->model()->index(index.row(),0),Qt::EditRole).toInt();
        }
        break;
//    case 1:
//        break;
//    default:
//        break;
    }
    accept();
}
