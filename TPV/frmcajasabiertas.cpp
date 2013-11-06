#include "frmcajasabiertas.h"
#include "ui_frmcajasabiertas.h"

frmCajasAbiertas::frmCajasAbiertas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmCajasAbiertas)
{
    ui->setupUi(this);

}

frmCajasAbiertas::~frmCajasAbiertas()
{
    delete ui;
}

void frmCajasAbiertas::cargar_datos(QString caja, QString ejercicio)
{
    QString cSQL = QString("select id, fecha_abertura, hora_abertura where id_caja = %1 and ejercicio ='%2' ").arg(
                Configuracion_global->caja,Configuracion_global->cEjercicio);
    QSqlQueryModel *modelcajas = new QSqlQueryModel(this);
    modelcajas->setQuery(cSQL,Configuracion_global->empresaDB);
    ui->tableView->setModel(modelcajas);
}


void frmCajasAbiertas::on_tableView_doubleClicked(const QModelIndex &index)
{
    int id = ui->tableView->model()->data(index.model()->index(index.row(),0)).toInt();

    this->id  = id;
}
