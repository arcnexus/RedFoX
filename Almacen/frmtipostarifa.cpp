#include "frmtipostarifa.h"
#include "ui_frmtipostarifa.h"
//#include <qsql.h>
//#include<QSqlDatabase>
//#include <QSqlQuery>
//#include <QSqlQueryModel>


FrmTiposTarifa::FrmTiposTarifa(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmTiposTarifa)
{
    ui->setupUi(this);
    QSqlQueryModel *mTarifas = new QSqlQueryModel(this);
    mTarifas->setQuery("select descripcion from codigotarifa",QSqlDatabase::database("Maya"));
    ui->listatarifas->setModel(mTarifas);
    ui->cboPais->setModel(Configuracion_global->paises_model);
    ui->cboPais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    QSqlQueryModel *monedas = new QSqlQueryModel(this);
    monedas->setQuery("Select moneda from monedas",QSqlDatabase::database("Maya"));
    ui->cboMoneda->setModel(monedas);
}

FrmTiposTarifa::~FrmTiposTarifa()
{
    delete ui;
}

void FrmTiposTarifa::on_btnAgregarTarifa_clicked()
{

}
