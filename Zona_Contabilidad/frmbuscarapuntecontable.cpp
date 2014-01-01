#include "frmbuscarapuntecontable.h"
#include "ui_frmbuscarapuntecontable.h"

frmBuscarApunteContable::frmBuscarApunteContable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmBuscarApunteContable)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);
    model->setQuery("select id, asiento, cuenta_d, descripcion_d,importe_d,fecha_asiento from diario where id= 0",Configuracion_global->contaDB);
    ui->tablaapuntes->setModel(model);
    //-------------------------------
    // Estructura presentación tabla
    //-------------------------------
    QStringList headers;
    headers << tr("id") << tr("ASIENTO") << tr("Sub-Cta") << tr("Descripción") << tr("Importe D") << tr("Fecha");
    QVariantList sizes;
    sizes << 0 << 120 << 120 << 300 << 120 << 120;
    for (int i = 0; i<headers.size(); i++)
    {
       ui->tablaapuntes->setColumnWidth(i,sizes.at(i).toInt());
       model->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
}

frmBuscarApunteContable::~frmBuscarApunteContable()
{
    delete ui;
}

void frmBuscarApunteContable::on_btnAceptar_clicked()
{
    QModelIndex index = ui->tablaapuntes->currentIndex();
    this->id = ui->tablaapuntes->model()->index(index.row(),0).data().toInt();

}

int frmBuscarApunteContable::get_id()
{
    return this->id;
}

void frmBuscarApunteContable::on_btnBuscar_clicked()
{
    QString fecha_ini,fecha_fin,subcuenta,importe_ini,importe_fin;
    fecha_ini = ui->dateFecha_inicio->date().toString("yyyyMMdd");
    fecha_fin = ui->dateFecha_fin->date().toString("yyyyMMdd");
    importe_ini = Configuracion_global->MonedatoDouble(ui->txtImporte_ini->text());
    importe_fin = Configuracion_global->MonedatoDouble(ui->txtImporte_fin->text());
    subcuenta = ui->txtcod_subcta->text();
    QString cSQL = QString("select id, asiento, cuenta_d, descripcion_d,importe_d,fecha_asiento from diario where fecha between('%1' and '%2')"
                           " and importe_d between('%3' and '%4') and cuenta_d ='%5'").arg(fecha_ini,fecha_fin,importe_ini,importe_fin,subcuenta);

    model->setQuery(cSQL,Configuracion_global->contaDB);
}

void frmBuscarApunteContable::on_txtImporte_ini_editingFinished()
{
    ui->txtImporte_ini->blockSignals(true);
    ui->txtImporte_ini->setText(Configuracion_global->toFormatoMoneda(ui->txtImporte_ini->text()));
    ui->txtImporte_ini->blockSignals(false);
}

void frmBuscarApunteContable::on_txtImporte_fin_editingFinished()
{
    ui->txtImporte_fin->blockSignals(true);
    ui->txtImporte_fin->setText(Configuracion_global->toFormatoMoneda(ui->txtImporte_fin->text()));
    ui->txtImporte_fin->blockSignals(false);
}
