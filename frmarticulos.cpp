#include "frmarticulos.h"
#include "ui_frmarticulos.h"
#include "configuracion.h"
#include "articulo.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlRecord>
#include <QString>


Articulo *oArticulo = new Articulo();
FrmArticulos::FrmArticulos(Configuracion *o_config,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmArticulos)
{
    ui->setupUi(this);
    o_config->CargarDatos();
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA1));
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA2));
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA3));
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA4));
}

FrmArticulos::~FrmArticulos()
{
    delete ui;
}

void FrmArticulos::on_botSiguiente_clicked()
{
    QString cId = QString::number(oArticulo->getId());
    oArticulo->Recuperar("Select * from articulos where id >"+cId+" order by id limit 0,1 ");
    LLenarCampos();

}

void FrmArticulos::LLenarCampos()
{
    Configuracion *oConfig = new Configuracion();
    oConfig->CargarDatos();
    ui->txtcCodigo->setText(oArticulo->getcCodigo());
   ui->txtcCodigoBarras->setText(oArticulo->getcCodigoBarras());
   ui->txtcCodigoFabricante->setText(oArticulo->getcCodigoFabricante());
   ui->txtcDescripcion->setText(oArticulo->getcDescripcion());
   ui->txtcDescripcionResumida->setText(oArticulo->getcDescripcionReducida());
   // Recupero proveedor
   QSqlQuery *qryProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
   qryProveedor->prepare("select id,cProveedor from Proveedores where id = :id");
   qryProveedor->bindValue(":id",oArticulo->getid_Proveedor());
   if (!qryProveedor->exec()) {
       QMessageBox::warning(NULL,tr("Error Datos"),tr("No se encuentra el proveedor asociado \n Deberá comprovar ficha producto"),tr("OK"));

   } else {
       qryProveedor->next();
       QSqlRecord record = qryProveedor->record();
       ui->txtcProveedor->setText(record.field("cProveedor").value().toString());
   }
   delete qryProveedor;
   ui->cboFamilia->setEditText(oArticulo->getcFamilia());
   ui->cboSeccion->setEditText(oArticulo->getcSeccion());
   ui->cboSubfamilia->setEditText(oArticulo->getcSubfamilia());
   ui->cboTipoIVA->setEditText(QString::number(oArticulo->getnTipoIva()));
   ui->txtrCoste->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrCoste(),'f',2)));
   ui->txtrTarifa1->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrTarifa1(),'f',2)));
   ui->txtrTarifa1_2->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrTarifa1(),'f',2)));
   ui->txtrDto->setText(QString::number(oArticulo->getrDto(),'f',2));
   ui->txtnDtoProveedor->setText(QString::number(oArticulo->getnDtoProveedor(),'f',2));
   ui->txtnDtoProveedor2->setText(QString::number(oArticulo->getnDtoProveedor2(),'f',2));
   ui->txtnDtoProveedor3->setText(QString::number(oArticulo->getnDtoProveedor3(),'f',2));
   ui->txtdFechaUltimaCompra->setDate(oArticulo->getdUltimaCompra());
   ui->txtdFechaUltimaVenta->setDate(oArticulo->getdUltimaVenta());
   ui->txtnMargen1->setText(QString::number(oArticulo->getnMargen1(),'f',2));
   ui->txtnMargen2->setText(QString::number(oArticulo->getnMargen2(),'f',2));
   ui->txtnMargen3->setText(QString::number(oArticulo->getnMargen3(),'f',2));
   ui->txtrPrecioMedio1->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrPrecioMedio(),'f',2)));
   ui->txtrPrecioMedio2->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrPrecioMedio2(),'f',2)));
   ui->txtrPrecioMedio3->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrPrecioMedio3(),'f',2)));
   ui->txtnUnidadesCompradas->setText(QString::number(oArticulo->getnUnidadesCompradas()));
   ui->txtnUnidadesVendidas->setText(QString::number(oArticulo->getnUnidadesVendidas()));
   ui->txtrAcumuladoCompras->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrAcumuladoCompras(),'f',2)));
   ui->txtrAcumuladoVentas->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrAcumuladoVentas(),'f',2)));
   ui->txttComentario->setText(oArticulo->gettComentario());
   ui->txtnStockMaximo->setText(QString::number(oArticulo->getnStockMaximo()));
   ui->txtnStockMinimo->setText(QString::number(oArticulo->getnStockMinimo()));
   if (oArticulo->getlControlarStock()==1)
        ui->chklControlarStock->setChecked(true);
   else
       ui->chklControlarStock->setChecked(false);
   ui->cboModelo->setEditText(oArticulo->getcModelo());
   ui->cboTalla->setEditText(oArticulo->getcTalla());
   ui->cboColor->setEditText(oArticulo->getcColor());


}
