#include "frmseccionesalmacen.h"
#include "ui_frmseccionesalmacen.h"

frmSeccionesAlmacen::frmSeccionesAlmacen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSeccionesAlmacen)
{
    ui->setupUi(this);
    model_secciones = new QSqlQueryModel(this);
    model_secciones->setQuery("select id, codigo, seccion from secciones order by seccion",
                              Configuracion_global->groupDB);
    ui->tabla_secciones->setModel(model_secciones);
    QStringList headers;
    headers << "id" << tr("código") << tr("Sección");
    QVariantList sizes;
    sizes << 0 << 120 <<200;
    for(int i;i < headers.size();i++)
    {
        ui->tabla_secciones->setColumnWidth(i,sizes.at(i).toInt());
        model_secciones->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
}

frmSeccionesAlmacen::~frmSeccionesAlmacen()
{
    delete ui;
}

void frmSeccionesAlmacen::on_btnAnadir_clicked()
{

}
