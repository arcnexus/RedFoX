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
    mTarifas->setQuery("select descripcion from codigotarifa",QSqlDatabase::database("terra"));
    ui->listatarifas->setModel(mTarifas);
}

FrmTiposTarifa::~FrmTiposTarifa()
{
    delete ui;
}
