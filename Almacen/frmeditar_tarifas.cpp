#include "frmeditar_tarifas.h"
#include "ui_frmeditar_tarifas.h"

frmEditar_tarifas::frmEditar_tarifas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmEditar_tarifas)
{
    ui->setupUi(this);
}

frmEditar_tarifas::~frmEditar_tarifas()
{
    delete ui;
}

void frmEditar_tarifas::setFiltro(int id)
{
    modeltarifa = new QSqlQueryModel(this);
    modeltarifa->setQuery("select id,pvp,pais,moneda,simbolo,codigo_tarifa,margen,margenminimo "
                          "from viewTarifa where id_Articulo = "+QString::number(id),QSqlDatabase::database("Maya"));
    ui->tabla_tarifa->setModel(modeltarifa);
}
