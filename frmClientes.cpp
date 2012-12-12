#include "frmClientes.h"
#include "ui_frmClientes.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QErrorMessage>
#include<QSqlError>
#include "cliente.h"
#include<QDate>
#include <QSqlQueryModel>
#include <QDebug>
#include "frmbuscarcliente.h"
#include "frmbuscarpoblacion.h"
#include <QString>
#include "columnamoneda.h"
#include "columnafecha.h"
#include <QSettings>
#include "frmfichapaciente.h"
#include <sqlcalls.h>
#include "paciente.h"


Cliente *oCliente = new Cliente();
SqlCalls *llamadasSQL = new SqlCalls();
frmClientes::frmClientes(Configuracion *oConfiguracion,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmClientes)
{
    ui->setupUi(this);

    // Rellenar formas de pago
    modelFP = new QSqlQueryModel();
    modelFP->setQuery("Select cFormaPago,id from FormPago",QSqlDatabase::database("empresa"));

    // Ocultar campos según configuración
    QSettings settings("infint", "terra");
    int nNivel = settings.value("nNivelAcceso").toInt();
    //QString cCategoria = settings.value("cCategoria").toString();
    if (nNivel >7)
        ui->btnFichaPaciente->setVisible(true);
    else
        ui->btnFichaPaciente->setVisible(false);
    oConfiguracion->CargarDatos();
    if(oConfiguracion->nIRPF)
        ui->chkClienteEmpresa->setVisible(true);
    else
        ui->chkClienteEmpresa->setVisible(false);

    //FP
    ui->cbocFormaPago->setModel(modelFP);
    //ui->tableView_5->setModel(model);

    // Rellenar Tarifas:
    ui->cbonTarifaCliente->addItem("A");
    ui->cbonTarifaCliente->addItem("B");
    ui->cbonTarifaCliente->addItem("C");

    //oCliente->Recuperar("Select * from clientes");
    //LLenarCampos();
    bloquearCampos();
    this->Altas = false;


}

frmClientes::~frmClientes()
{
    delete ui;
    delete oCliente;
    dbCliente.close();
}
void frmClientes::LLenarCampos() {
    Configuracion *o_Configuracion = new Configuracion();
    ui->txtcCodigoCliente->setText(oCliente->getcCodigoCliente());
    ui->txtPrimerApellido->setText(oCliente->getcApellido1());
    ui->txtSegundoApellido->setText(oCliente->getcApellido2());
    ui->txtcNombre->setText(oCliente->getcNombre());
    ui->txtcNombreFiscal->setText(oCliente->getcNombreFiscal());
    ui->txtNombreFiscal->setText(oCliente->getcNombreFiscal());
    ui->txtcNombreComercial->setText(oCliente->getcNombreComercial());
    ui->txtcPersonaContacto->setText(oCliente->getcPersonaContacto());
    ui->txtcCifNif->setText(oCliente->getcCifNif());
    ui->txtcDireccion1->setText(oCliente->getcDireccion1());
    ui->txtcDireccion2->setText(oCliente->getcDireccion2());
    ui->txtcCp->setText(oCliente->getcCP());
    ui->txtcPoblacion->setText(oCliente->getcPoblacion());
    ui->txtcProvincia->setText(oCliente->getcProvincia());
    ui->txtcPais->setText(oCliente->getcPais());
    ui->txtcTelefono1->setText(oCliente->getcTelefono1());
    ui->txtcTelefono2->setText(oCliente->getcTelefono2());
    ui->txtcFax->setText(oCliente->getcFax());
    ui->txtcMovil->setText(oCliente->getcMovil());
    ui->txtcEMail->setText(oCliente->getcEmail());
    ui->txtcWeb->setText(oCliente->getcWeb());
    ui->txtcDireccionFactura1->setText(oCliente->getcDireccionFactura1());
    ui->txtcDireccionFactura2->setText(oCliente->getcDireccionFactura2());
    ui->txtcCPFactura->setText(oCliente->getcCPFactura());
    ui->txtcPoblacionFactura->setText(oCliente->getcPoblacionFactura());
    ui->txtcProvinciaFactura->setText(oCliente->getcProvinciaFactura());
    ui->txtcPaisFactura->setText(oCliente->getcPaisFactura());
    ui->txtcDireccionAlmacen1->setText(oCliente->getcDireccionAlmacen());
    ui->txtcDireccionAlmacen2->setText(oCliente->getcDireccionAlmacen2());
    ui->txtcCpPoblacionAlmacen->setText(oCliente->getcCPAlmacen());
    ui->txtcPoblacionAlmacen->setText(oCliente->getcPoblacionAlmacen());
    ui->txtcProvinciaAlmacen->setText(oCliente->getcProvinciaAlmacen());
    ui->txtcPaisAlmacen->setText(oCliente->getcPaisAlmacen());
    ui->txtdFechaAlta->setDate(oCliente->getdFechaAlta());
    ui->txtdFechaUltimaCompra->setDate(oCliente->getdFechaUltimaCompra());
    ui->txtrImporteAcumulado->setText(o_Configuracion->FormatoNumerico(QString::number( oCliente->getrAcumuladoVentas(),'f',2)));
    ui->txtrVentasEjercicio->setText(o_Configuracion->FormatoNumerico(QString::number(oCliente->getrVentasEjercicio(),'f',2)));
    ui->txtrRiesgoPermitido->setText(o_Configuracion->FormatoNumerico(QString::number(oCliente->getrRiesgoMaximo(),'f',2)));
    ui->txtrDeudaActual->setText(o_Configuracion->FormatoNumerico(QString::number(oCliente->getrDeudaActual(),'f',2)));
    ui->txttComentarios->setText(oCliente->gettComentarios());
    if(oCliente->islBloqueado()) {
        ui->chklBloqueoCliente->setChecked(true);
    } else {
        ui->chklBloqueoCliente->setChecked(false);
    }
    ui->txttComentarioBloqueo->setText(oCliente->gettComentarioBloqueo());
    ui->txtnPorcDtoCliente->setText(QString::number(oCliente->getnPorcDtoCliente()));
    if(oCliente->islRecargoEquivalencia()) {
        ui->chklRecargoEquivalencia->setChecked(true);
    } else {
        ui->chklRecargoEquivalencia->setChecked(false);
    }
    ui->txtcCuentaContable->setText(oCliente->getcCuentaContable());
    ui->txtcCuentaIvaRepercutido->setText(oCliente->getcCuentaIvaRepercutido());
    ui->txtcCuentaDeudas->setText(oCliente->getcCuentaDeudas());
    ui->txtcCuentaCobros->setText(oCliente->getcCuentaCobros());
    int indice=ui->cbocFormaPago->findText(oCliente->getcFormaPago());
    if(indice!=-1)
     ui->cbocFormaPago->setCurrentIndex(indice);

    //ui->cbocFormaPago->setItemText();
    ui->txtnDiaPago1->setValue(oCliente->getnDiaPago1());
    ui->txtnDiaPago2->setValue(oCliente->getnDiaPago2());
   // ui->cbonTarifaCliente->lineEdit()->setText(oCliente->getnTarifaCliente());
    ui->txtrImporteACuenta->setText( o_Configuracion->FormatoNumerico(QString::number(oCliente->getrImporteACuenta(),'f',2)));
    ui->txtrVales->setText(o_Configuracion->FormatoNumerico(QString::number(oCliente->getrVales(),'f',2)));
    ui->txtcEntidadBancaria->setText(oCliente->getcEntidadBancaria());
    ui->txtcOficinaBancaria->setText(oCliente->getcOficinaBancaria());
    ui->txtcDc->setText(oCliente->getcDC());
    ui->txtcCuentaCorriente->setText(oCliente->getcCuentaCorriente());
    ui->txtdFechaNacimiento->setDate(oCliente->getdFechaNacimiento());
    ui->txtcAccesoWeb->setText(oCliente->getcAccesoWeb());
    ui->txtcPasswordWeb->setText(oCliente->getcPasswordweb());
    if (oCliente->getlIRPF())
        ui->chkClienteEmpresa->setChecked(true);
    else
        ui->chkClienteEmpresa->setChecked(false);

    // Tablas
    /********************************************************************
     * DEUDAS
     *******************************************************************/

    QSqlQueryModel *deudas = new QSqlQueryModel();
    QString cSQL;
    cSQL= "Select id,cDocumento,rPendienteCobro,dFecha,dVencimiento from clientes_deuda where Id_cliente =" + QString::number(oCliente->getId());
    deudas->setQuery(cSQL,QSqlDatabase::database("empresa"));
    ColumnaMoneda *columnaMoneda = new ColumnaMoneda();
    ColumnaFecha *columnaFecha = new ColumnaFecha();
    ui->TablaDeudas->setModel(deudas);
        ui->TablaDeudas->setItemDelegateForColumn(2,columnaMoneda);
        ui->TablaDeudas->setItemDelegateForColumn(3,columnaFecha);
        ui->TablaDeudas->setItemDelegateForColumn(4,columnaFecha);
    //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
   QHeaderView *CabeceraDeu = new QHeaderView(Qt::Horizontal,this);
   // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
   ui->TablaDeudas->setHorizontalHeader(CabeceraDeu);
   /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0".  */
   //CabeceraDeu->setResizeMode(0,QHeaderView::Fixed);
   CabeceraDeu->resizeSection(0,0);
   //CabeceraDeu->setResizeMode(1,QHeaderView::Fixed);
   CabeceraDeu->resizeSection(1,85);
   //CabeceraDeu->setResizeMode(2,QHeaderView::Fixed);
   CabeceraDeu->resizeSection(2,85);
   //CabeceraDeu->setResizeMode(3,QHeaderView::Fixed);
   CabeceraDeu->resizeSection(3,85);
   //CabeceraDeu->setResizeMode(4,QHeaderView::Fixed);
   CabeceraDeu->resizeSection(4,85);
   deudas->setHeaderData(1, Qt::Horizontal, QObject::tr("DOCUMENTO"));
   deudas->setHeaderData(2, Qt::Horizontal, QObject::tr("I.PENDIENTE"));
   deudas->setHeaderData(3, Qt::Horizontal, QObject::tr("FECHA"));
   deudas->setHeaderData(4, Qt::Horizontal, QObject::tr("VTO"));
   CabeceraDeu->setVisible(true);

   /********************************************************************
    * ALBARANES
    *******************************************************************/

   QSqlQueryModel *Albaranes = new QSqlQueryModel();
   cSQL= "Select id,nAlbaran,dFecha,nFactura,rTotalAlbaran from cab_alb where id_Cliente =" + QString::number(oCliente->getId());
   Albaranes->setQuery(cSQL,QSqlDatabase::database("empresa"));
   ui->TablaAlbaranes->setModel(Albaranes);
       ui->TablaAlbaranes->setItemDelegateForColumn(2,columnaFecha);
       ui->TablaAlbaranes->setItemDelegateForColumn(4,columnaMoneda);
   //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
  QHeaderView *CabeceraAlb = new QHeaderView(Qt::Horizontal,this);
  // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
  ui->TablaAlbaranes->setHorizontalHeader(CabeceraAlb);
  /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0".  */
 // CabeceraAlb->setResizeMode(0,QHeaderView::Fixed);
  CabeceraAlb->resizeSection(0,0);
 //  CabeceraAlb->setResizeMode(1,QHeaderView::Fixed);
  CabeceraAlb->resizeSection(1,85);
 // CabeceraAlb->setResizeMode(2,QHeaderView::Fixed);
  CabeceraAlb->resizeSection(2,85);
 // CabeceraAlb->setResizeMode(3,QHeaderView::Fixed);
  CabeceraAlb->resizeSection(3,85);
 // CabeceraAlb->setResizeMode(4,QHeaderView::Fixed);
  CabeceraAlb->resizeSection(4,85);
  Albaranes->setHeaderData(1, Qt::Horizontal, QObject::tr("ALBARAN"));
  Albaranes->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
  Albaranes->setHeaderData(3, Qt::Horizontal, QObject::tr("FACTURA"));
  Albaranes->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));
  CabeceraAlb->setVisible(true);

  /********************************************************************
   * FACTURAS
   *******************************************************************/

  QSqlQueryModel *Facturas = new QSqlQueryModel(this);

  cSQL= "Select id,cFactura,dFecha,dFechaCobro,rTotal from cab_fac where iId_Cliente =" + QString::number(oCliente->getId());
  Facturas->setQuery(cSQL,QSqlDatabase::database("empresa"));
  ui->tablaFacturas->setModel(Facturas);
      ui->tablaFacturas->setItemDelegateForColumn(2,columnaFecha);
      ui->tablaFacturas->setItemDelegateForColumn(3,columnaFecha);
      ui->tablaFacturas->setItemDelegateForColumn(4,columnaMoneda);
  //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
 QHeaderView *CabeceraFac = new QHeaderView(Qt::Horizontal,this);
 // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
 ui->tablaFacturas->setHorizontalHeader(CabeceraFac);
 /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0".  */
// CabeceraFac->setResizeMode(0,QHeaderView::Fixed);
 CabeceraFac->resizeSection(0,0);
// CabeceraFac->setResizeMode(1,QHeaderView::Fixed);
 CabeceraFac->resizeSection(1,85);
// CabeceraFac->setResizeMode(2,QHeaderView::Fixed);
 CabeceraFac->resizeSection(2,85);
// CabeceraFac->setResizeMode(3,QHeaderView::Fixed);
 CabeceraFac->resizeSection(3,85);
// CabeceraFac->setResizeMode(4,QHeaderView::Fixed);
 CabeceraFac->resizeSection(4,85);
 Facturas->setHeaderData(1, Qt::Horizontal, QObject::tr("FACTURA"));
 Facturas->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
 Facturas->setHeaderData(3, Qt::Horizontal, QObject::tr("FECHA COBRO"));
 Facturas->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));
 CabeceraFac->setVisible(true);

 /********************************************************************
  * PRESUPUESTOS
  *******************************************************************/

 QSqlQueryModel *Presupuestos = new QSqlQueryModel(this);

 cSQL= "Select id,nPresupuesto,dFecha,dValidoHasta,rTotal from cab_pre where id_Cliente =" + QString::number(oCliente->getId());
 Presupuestos->setQuery(cSQL,QSqlDatabase::database("empresa"));
 ui->tablaPresupuestos->setModel(Presupuestos);
 ui->tablaPresupuestos->setItemDelegateForColumn(2,columnaFecha);
 ui->tablaPresupuestos->setItemDelegateForColumn(3,columnaFecha);
 ui->tablaPresupuestos->setItemDelegateForColumn(4,columnaMoneda);
 //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
QHeaderView *CabeceraPre = new QHeaderView(Qt::Horizontal,this);
// Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
ui->tablaPresupuestos->setHorizontalHeader(CabeceraPre);
/*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0".  */
//CabeceraPre->setResizeMode(0,QHeaderView::Fixed);
CabeceraPre->resizeSection(0,0);
//CabeceraPre->setResizeMode(1,QHeaderView::Fixed);
CabeceraPre->resizeSection(1,85);
//CabeceraPre->setResizeMode(2,QHeaderView::Fixed);
CabeceraPre->resizeSection(2,85);
//CabeceraPre->setResizeMode(3,QHeaderView::Fixed);
CabeceraPre->resizeSection(3,85);
//CabeceraPre->setResizeMode(4,QHeaderView::Fixed);
CabeceraPre->resizeSection(4,85);
//CabeceraPre->set
Presupuestos->setHeaderData(1, Qt::Horizontal, QObject::tr("PRES"));
Presupuestos->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
Presupuestos->setHeaderData(3, Qt::Horizontal, QObject::tr("FECHA APRO."));
Presupuestos->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));
CabeceraPre->setVisible(true);


/********************************************************************
 * VALES
 *******************************************************************/

QSqlQueryModel *Vales = new QSqlQueryModel(this);

cSQL= "Select id,nNumero,dFecha,dVto,rImporte from vales where id_Cliente =" + QString::number(oCliente->getId());
Vales->setQuery(cSQL,QSqlDatabase::database("empresa"));
ui->tablaVales->setModel(Vales);
ui->tablaVales->setItemDelegateForColumn(2,columnaFecha);
ui->tablaVales->setItemDelegateForColumn(3,columnaFecha);
ui->tablaVales->setItemDelegateForColumn(4,columnaMoneda);
//Creamos Objeto de la clase Cabecera para las cabeceras horizontales
QHeaderView *CabeceraVal = new QHeaderView(Qt::Horizontal,this);
// Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
ui->tablaVales->setHorizontalHeader(CabeceraVal);
/*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0".  */
//CabeceraVal->setResizeMode(0,QHeaderView::Fixed);
CabeceraVal->resizeSection(0,0);
//CabeceraVal->setResizeMode(1,QHeaderView::Fixed);
CabeceraVal->resizeSection(1,85);
//CabeceraVal->setResizeMode(2,QHeaderView::Fixed);
CabeceraVal->resizeSection(2,85);
//CabeceraVal->setResizeMode(3,QHeaderView::Fixed);
CabeceraVal->resizeSection(3,85);
//CabeceraVal->setResizeMode(4,QHeaderView::Fixed);
CabeceraVal->resizeSection(4,85);
Vales->setHeaderData(1, Qt::Horizontal, QObject::tr("VALE"));
Vales->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
Vales->setHeaderData(3, Qt::Horizontal, QObject::tr("VTO."));
Vales->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));
CabeceraVal->setVisible(true);

/********************************************************************
 * TICKETS
 *******************************************************************/

QSqlQueryModel *Tickets = new QSqlQueryModel(this);

cSQL= "Select id,nTicket,dFecha,cHora,rImporte from cab_tpv where id_Cliente =" + QString::number(oCliente->getId());
Tickets->setQuery(cSQL,QSqlDatabase::database("empresa"));
ui->tablaTickets->setModel(Tickets);
ui->tablaTickets->setItemDelegateForColumn(2,columnaFecha);
ui->tablaTickets->setItemDelegateForColumn(3,columnaFecha);
ui->tablaTickets->setItemDelegateForColumn(4,columnaMoneda);
//Creamos Objeto de la clase Cabecera para las cabeceras horizontales
QHeaderView *CabeceraTic = new QHeaderView(Qt::Horizontal,this);
// Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
ui->tablaTickets->setHorizontalHeader(CabeceraTic);
/*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0".  */
//CabeceraTic->setResizeMode(0,QHeaderView::Fixed);
CabeceraTic->resizeSection(0,0);
//CabeceraTic->setResizeMode(1,QHeaderView::Fixed);
CabeceraTic->resizeSection(1,85);
//CabeceraTic->setResizeMode(2,QHeaderView::Fixed);
CabeceraTic->resizeSection(2,85);
//CabeceraTic->setResizeMode(3,QHeaderView::Fixed);
CabeceraTic->resizeSection(3,85);
//CabeceraTic->setResizeMode(4,QHeaderView::Fixed);
CabeceraTic->resizeSection(4,85);
Tickets->setHeaderData(1, Qt::Horizontal, QObject::tr("TICKET"));
Tickets->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
Tickets->setHeaderData(3, Qt::Horizontal, QObject::tr("HORA"));
Tickets->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));
CabeceraTic->setVisible(true);


}
void frmClientes::VaciarCampos() {
    QDate dFecha;

    ui->txtcCodigoCliente->setText("");
    ui->txtPrimerApellido->setText("");
    ui->txtSegundoApellido->setText("");
    ui->txtcNombre->setText("");
    ui->txtcNombreFiscal->setText("");
    ui->txtcNombreComercial->setText("");
    ui->txtcPersonaContacto->setText("");
    ui->txtcCifNif->setText("");
    ui->txtcDireccion1->setText("");
    ui->txtcDireccion2->setText("");
    ui->txtcPoblacion->setText("");
    ui->txtcProvincia->setText("");
    ui->txtcPais->setText("");
    ui->txtcTelefono1->setText("");
    ui->txtcTelefono2->setText("");
    ui->txtcFax->setText("");
    ui->txtcMovil->setText("");
    ui->txtcEMail->setText("");
    ui->txtcWeb->setText("");
    ui->txtcDireccionFactura1->setText("");
    ui->txtcDireccionFactura2->setText("");
    ui->txtcCPFactura->setText("");
    ui->txtcPoblacionFactura->setText("");
    ui->txtcProvinciaFactura->setText("");
    ui->txtcPaisFactura->setText("");
    ui->txtcDireccionAlmacen1->setText("");
    ui->txtcDireccionAlmacen2->setText("");
    ui->txtcCpPoblacionAlmacen->setText("");
    ui->txtcPoblacionAlmacen->setText("");
    ui->txtcProvinciaAlmacen->setText("");
    ui->txtcPaisAlmacen->setText("");
    ui->txtdFechaAlta->setDate(dFecha.currentDate());
    ui->txtdFechaUltimaCompra->setDate(dFecha.currentDate());
    ui->txtrImporteAcumulado->setText("");
    ui->txtrVentasEjercicio->setText("");
    ui->txtrRiesgoPermitido->setText("");
    ui->txtrDeudaActual->setText("");
    ui->txttComentarios->setText("");
    ui->chklBloqueoCliente->setChecked(false);
    ui->txttComentarioBloqueo->setText( "");
    ui->txtnPorcDtoCliente->setText("0");
    ui->chklRecargoEquivalencia->setChecked(false);
    ui->txtcCuentaContable->setText("");
    ui->txtcCuentaIvaRepercutido->setText("");
    ui->txtcCuentaDeudas->setText("");
    ui->txtcCuentaCobros->setText("");


    //ui->cbocFormaPago->setItemText();
    ui->txtnDiaPago1->setValue(0);
    ui->txtnDiaPago2->setValue(0);
   // ui->cbonTarifaCliente->lineEdit()->setText(oCliente->getnTarifaCliente());
    ui->txtrImporteACuenta->setText("");
    ui->txtrVales->setText("");
    ui->txtcEntidadBancaria->setText("");
    ui->txtcOficinaBancaria->setText("");
    ui->txtcDc->setText("");
    ui->txtcCuentaCorriente->setText("");
    ui->txtdFechaNacimiento->setDate(dFecha.currentDate());
    ui->txtcAccesoWeb->setText("");
    ui->txtcPasswordWeb->setText("");
    ui->txtPrimerApellido->setFocus();
    ui->chkClienteEmpresa->setChecked(false);

}

void frmClientes::LLenarCliente() {
    oCliente->setcCodigoCliente(ui->txtcCodigoCliente->text());
    oCliente->setcApellido1(ui->txtPrimerApellido->text());
    oCliente->setcApellido2(ui->txtSegundoApellido->text());
    oCliente->setcNombre(ui->txtcNombre->text());
    oCliente->setcNombreFiscal(ui->txtcNombreFiscal->text());
    oCliente->setcNombreComercial(ui->txtcNombreComercial->text());
    oCliente->setcPersonaContacto(ui->txtcPersonaContacto->text());
    oCliente->setcCifNif(ui->txtcCifNif->text());
    oCliente->setcDireccion1(ui->txtcDireccion1->text());
    oCliente->setcDireccion2(ui->txtcDireccion2->text());
    oCliente->setcCp(ui->txtcCp->text());
    oCliente->setcPoblacion(ui->txtcPoblacion->text());
    oCliente->setcProvincia(ui->txtcProvincia->text());
    oCliente->setcPais(ui->txtcPais->text());
    oCliente->setcTelefono1(ui->txtcTelefono1->text());
    oCliente->setcTelefono2(ui->txtcTelefono2->text());
    oCliente->setcFax(ui->txtcFax->text());
    oCliente->setcMovil(ui->txtcMovil->text());
    oCliente->setcEmail(ui->txtcEMail->text());
    oCliente->setcWeb(ui->txtcWeb->text());
    oCliente->setcDireccionFactura1(ui->txtcDireccionFactura1->text());
    oCliente->setcDireccionFactura2(ui->txtcDireccionFactura2->text());
    oCliente->setcCPFactura(ui->txtcCPFactura->text());
    oCliente->setcPoblacionFactura(ui->txtcPoblacionFactura->text());
    oCliente->setcProvinciaFactura(ui->txtcProvinciaFactura->text());
    oCliente->setcPaisFactura(ui->txtcPaisFactura->text());
    oCliente->setcDireccionAlmacen1(ui->txtcDireccionAlmacen1->text());
    oCliente->setcDireccionAlmacen2(ui->txtcDireccionAlmacen2->text());
    oCliente->setcCPAlmacen(ui->txtcCpPoblacionAlmacen->text());
    oCliente->setcPoblacionAlmacen(ui->txtcPoblacionAlmacen->text());
    oCliente->setcProvinciaAlmacen(ui->txtcProvinciaAlmacen->text());
    oCliente->setcPaisAlmacen(ui->txtcPaisAlmacen->text());
    oCliente->setdFechaAlta(ui->txtdFechaAlta->date());
    oCliente->setdFechaUltimaCompra(ui->txtdFechaUltimaCompra->date());
    oCliente->setrAcumuladoVentas(ui->txtrImporteAcumulado->text().replace(".","").toDouble());
    oCliente->setrVentasEjercicio(ui->txtrVentasEjercicio->text().replace(".","").toDouble());
    oCliente->setrRiesgoMaximo(ui->txtrRiesgoPermitido->text().replace(".","").toDouble());
    oCliente->setrDeudaActual(ui->txtrDeudaActual->text().replace(".","").toDouble());
    oCliente->settComentarios(ui->txttComentarios->toPlainText());
    oCliente->setisBloqueado(ui->chklBloqueoCliente->isChecked());
    oCliente->settComentarioBloqueo(ui->txttComentarioBloqueo->toPlainText());
    oCliente->setnPorcDtoCliente(ui->txtnPorcDtoCliente->text().toInt());
    oCliente->setislRecargoEquivalencia(ui->chklRecargoEquivalencia->isChecked());
    oCliente->setcCuentaContable(ui->txtcCuentaContable->text());
    oCliente->setcCuentaIvaRepercutido(ui->txtcCuentaIvaRepercutido->text());
    oCliente->setcCuentaDeudas(ui->txtcCuentaDeudas->text());
    oCliente->setcCuentaCobros(ui->txtcCuentaCobros->text());
    oCliente->setcFormaPago(ui->cbocFormaPago->currentText());
    oCliente->setnDiaPago1(ui->txtnDiaPago1->value());
    oCliente->setnDiaPago2(ui->txtnDiaPago2->value());
    oCliente->setnTarifaCliente(ui->cbonTarifaCliente->currentText().toDouble());
    oCliente->setrImporteaCuenta(ui->txtrImporteACuenta->text().toDouble());
    oCliente->setrVales( ui->txtrVales->text().replace(".","").toDouble());
    oCliente->setcEntidadBancaria(ui->txtcEntidadBancaria->text());
    oCliente->setcOficinaBancaria(ui->txtcOficinaBancaria->text());
    oCliente->setcDC(ui->txtcDc->text());
    oCliente->setcCuentaCorriente(ui->txtcCuentaCorriente->text());
    oCliente->setdFechaNacimiento(ui->txtdFechaNacimiento->date());
    oCliente->setrImportePendiente(ui->txtrImporteAcumulado->text().toDouble());
    oCliente->setcAccesoWeb(ui->txtcAccesoWeb->text());
    oCliente->setcPasswordWeb(ui->txtcPasswordWeb->text());
    if (ui->chkClienteEmpresa)
        oCliente->setlIRPF(true);
    else
        oCliente->setlIRPF(false);
}


void frmClientes::on_btnSiguiente_clicked()
{

    QString cId = QString::number(oCliente->getId());
    oCliente->Recuperar("Select * from clientes where id >"+cId+" order by id limit 1 ");
    LLenarCampos();

}

void frmClientes::on_btnAnterior_clicked()
{
    QString cId = QString::number(oCliente->getId());
    oCliente->Recuperar("Select * from clientes where id <"+cId+" order by id desc limit 1 ");
    LLenarCampos();
}

void frmClientes::on_btnGuardar_clicked()
{

    LLenarCliente();
    if(this->Altas) {
       this->Altas = false;
       oCliente->Anadir();
    } else {

        oCliente->Guardar();
    }
    bloquearCampos();
}

void frmClientes::on_btnAnadir_clicked()
{
    desbloquearCampos();

   VaciarCampos();
   this->Altas = true;
    ui->txtcCodigoCliente->setText(oCliente->NuevoCodigoCliente());
    ui->txtcCodigoCliente->setFocus();
}

void frmClientes::on_txtPrimerApellido_lostFocus()
{
    ui->txtPrimerApellido->setText(ui->txtPrimerApellido->text().toUpper());
    if(!ui->txtPrimerApellido->text().isEmpty() and !ui->txtSegundoApellido->text().isEmpty() and !ui->txtcNombre->text().isEmpty()) {
        ui->txtcNombreFiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtcNombre->text());
        ui->txtNombreFiscal->setText(ui->txtcNombreFiscal->text());
    }
}

void frmClientes::on_txtSegundoApellido_lostFocus()
{
    ui->txtSegundoApellido->setText(ui->txtSegundoApellido->text().toUpper());
    if(!ui->txtPrimerApellido->text().isEmpty() and !ui->txtSegundoApellido->text().isEmpty() and !ui->txtcNombre->text().isEmpty()) {
        ui->txtcNombreFiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtcNombre->text());
        ui->txtNombreFiscal->setText(ui->txtcNombreFiscal->text());
    }

}

void frmClientes::on_txtcNombre_lostFocus()
{
    ui->txtcNombre->setText(ui->txtcNombre->text().toUpper());
    if(!ui->txtPrimerApellido->text().isEmpty() and !ui->txtSegundoApellido->text().isEmpty() and !ui->txtcNombre->text().isEmpty()) {
        ui->txtcNombreFiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtcNombre->text());
        ui->txtNombreFiscal->setText(ui->txtcNombreFiscal->text());
    }

}

void frmClientes::on_txtcPoblacion_lostFocus()
{
    ui->txtcPoblacion->setText(ui->txtcPoblacion->text().toUpper());
    if (ui->txtcCp->text().isEmpty() and !ui->txtcPoblacion->text().isEmpty()) {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcPoblacion->text(),1);
       if(BuscarPoblacion.exec()) {
        //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
           int nId = BuscarPoblacion.DevolverID();
           qDebug() <<nId;
         if(nId > 0) {
               QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
               QString cId;
               cId = QString::number(nId);
               qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
               qPoblacion.bindValue(":id",cId);
               if(!qPoblacion.exec()) {
                  /* qDebug() << qPoblacion.lastQuery();
                   qDebug() << qPoblacion.value(0).toString(); */
                   QMessageBox::critical(NULL,tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
               } else {
                  // qDebug() << qPoblacion.lastQuery();
                   if (qPoblacion.next()) {
                        ui->txtcPoblacion->setText(qPoblacion.value(0).toString());
                        ui->txtcCp->setText(qPoblacion.value(1).toString());
                        ui->txtcProvincia->setText(qPoblacion.value(2).toString());
                        ui->txtcPais->setText("ESPAÑA");
                   }
               }

           }
       }
      // BuscarPoblacion.close();
    }

}

void frmClientes::on_txtcProvincia_lostFocus()
{
    ui->txtcProvincia->setText(ui->txtcProvincia->text().toUpper());
}

void frmClientes::on_txtcCifNif_lostFocus()
{
    ui->txtcCifNif->setText(ui->txtcCifNif->text().toUpper());
}

void frmClientes::on_btnEditar_clicked()
{
        desbloquearCampos();
        ui->txtcCodigoCliente->setEnabled(false);
        ui->txtcCifNif->setFocus();


}
void frmClientes::bloquearCampos() {

    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    QSpinBox *SpinBox;
    foreach (SpinBox, SpinBoxList) {
        SpinBox->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }

    ui->btnAnadir->setEnabled(true);
    ui->btnAnterior->setEnabled(true);
    ui->btnBorrar->setEnabled(true);
    ui->btnBuscar->setEnabled(true);
    ui->btnCerrar->setEnabled(true);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);


}
void frmClientes::desbloquearCampos() {
    // LineEdit
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    QSpinBox *SpinBox;
    foreach (SpinBox, SpinBoxList) {
        SpinBox->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    ui->btnAnadir->setEnabled(false);
    ui->btnAnterior->setEnabled(false);
    ui->btnBorrar->setEnabled(false);
    ui->btnBuscar->setEnabled(false);
    ui->btnCerrar->setEnabled(false);
    ui->btnDeshacer->setEnabled(true);
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnSiguiente->setEnabled(false);
    ui->txtrDeudaActual->setEnabled(false);
    ui->txtrImporteAcumulado->setEnabled(false);
    ui->txtrVentasEjercicio->setEnabled(false);
}

void frmClientes::on_btnDeshacer_clicked()
{
    QString cId = QString::number(oCliente->getId());
    oCliente->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
    LLenarCampos();
    bloquearCampos();
}

void frmClientes::on_btnBorrar_clicked()
{
    int id_Cliente = oCliente->getId();
    id_Cliente --;

    oCliente->Borrar(oCliente->getId());
    VaciarCampos();

}

void frmClientes::on_btnBuscar_clicked()
{
    FrmBuscarCliente BuscarClientes;
    BuscarClientes.exec();
    int nId = BuscarClientes.DevolverID();
    qDebug() << nId;
    QString cId = QString::number(nId);

    oCliente->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
    LLenarCampos();
}

void frmClientes::on_txtcCp_lostFocus()
    {
    if (!ui->txtcCp->text().isEmpty() and ui->txtcPoblacion->text().isEmpty()) {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
       if(BuscarPoblacion.exec()) {
        //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
           int nId = BuscarPoblacion.DevolverID();
           //qDebug() <<nId;
         if(nId > 0) {
               QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
               QString cId;
               cId = QString::number(nId);
               qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
               qPoblacion.bindValue(":id",cId);
               if(!qPoblacion.exec()) {
                  /* qDebug() << qPoblacion.lastQuery();
                   qDebug() << qPoblacion.value(0).toString(); */
                   QMessageBox::critical(NULL,tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
               } else {
                   //qDebug() << qPoblacion.lastQuery();
                   if (qPoblacion.next()) {
                        ui->txtcPoblacion->setText(qPoblacion.value(0).toString());
                        ui->txtcProvincia->setText(qPoblacion.value(2).toString());
                        ui->txtcPais->setText("ESPAÑA");
                   }
               }

           }
       }
      // BuscarPoblacion.close();
    }

    }


void frmClientes::on_txtcCPFactura_lostFocus()
{
    if (!ui->txtcCPFactura->text().isEmpty() and ui->txtcPoblacionFactura->text().isEmpty()) {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcCPFactura->text(),0);
       if(BuscarPoblacion.exec()) {
        //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
           int nId = BuscarPoblacion.DevolverID();
           //qDebug() <<nId;
         if(nId > 0) {
               QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
               QString cId;
               cId = QString::number(nId);

               qPoblacion.prepare("select col_3 as poblacion, col_4 as CP, col_6 as provincia from poblaciones where col_1 = :cId");
               qPoblacion.bindValue(":id",cId);
               if(!qPoblacion.exec()) {
                  // qDebug() << qPoblacion.lastQuery();
                  // qDebug() << qPoblacion.value(0).toString();
                   QMessageBox::critical(NULL,tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
               } else {
                   //qDebug() << qPoblacion.lastQuery();
                   if (qPoblacion.next()) {
                                        qDebug() << qPoblacion.value(0).toString();
                        ui->txtcPoblacionFactura->setText(qPoblacion.value(0).toString());
                        ui->txtcProvinciaFactura->setText(qPoblacion.value(2).toString());
                        ui->txtcPaisFactura->setText("ESPAÑA");
                   }
               }

           }
       }
      // BuscarPoblacion.close();
    }

}

void frmClientes::on_txtcPoblacionFactura_lostFocus()
{
    ui->txtcPoblacionFactura->setText(ui->txtcPoblacionFactura->text().toUpper());
    if (ui->txtcCPFactura->text().isEmpty() and !ui->txtcPoblacionFactura->text().isEmpty()) {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcPoblacionFactura->text(),1);
       if(BuscarPoblacion.exec()) {
        //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
           int nId = BuscarPoblacion.DevolverID();
           //qDebug() <<nId;
         if(nId > 0) {
               QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
               QString cId;
               cId = QString::number(nId);
               qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
               qPoblacion.bindValue(":id",cId);
               if(!qPoblacion.exec()) {
                  // qDebug() << qPoblacion.lastQuery();
                   //qDebug() << qPoblacion.value(0).toString();
                   QMessageBox::critical(NULL,tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
               } else {
                  // qDebug() << qPoblacion.lastQuery();
                   if (qPoblacion.next()) {
                        ui->txtcPoblacionFactura->setText(qPoblacion.value(0).toString());
                        ui->txtcCPFactura->setText(qPoblacion.value(1).toString());
                        ui->txtcProvinciaFactura->setText(qPoblacion.value(2).toString());
                        ui->txtcPaisFactura->setText("ESPAÑA");
                   }
               }

           }
       }
      // BuscarPoblacion.close();
    }
}

void frmClientes::on_txtcCpPoblacionAlmacen_lostFocus()
{
    if (!ui->txtcCpPoblacionAlmacen->text().isEmpty() and ui->txtcPoblacionAlmacen->text().isEmpty()) {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcCpPoblacionAlmacen->text(),0);
       if(BuscarPoblacion.exec()) {
        //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
           int nId = BuscarPoblacion.DevolverID();
          // qDebug() <<nId;
         if(nId > 0) {
               QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
               QString cId;
               cId = QString::number(nId);
               qPoblacion.prepare("select col_3 as poblacion, col_4 as CP, col_6 as provincia from poblaciones where col_1 = :cId");
               qPoblacion.bindValue(":id",cId);
               if(!qPoblacion.exec()) {
                  /* qDebug() << qPoblacion.lastQuery();
                   qDebug() << qPoblacion.value(0).toString(); */
                   QMessageBox::critical(NULL,tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
               } else {
                  // qDebug() << qPoblacion.lastQuery();
                   if (qPoblacion.next()) {
                        ui->txtcPoblacionAlmacen->setText(qPoblacion.value(0).toString());
                        ui->txtcProvinciaAlmacen->setText(qPoblacion.value(2).toString());
                        ui->txtcPaisAlmacen->setText("ESPAÑA");
                   }
               }

           }
       }
      // BuscarPoblacion.close();
    }
}

void frmClientes::on_txtcPoblacionAlmacen_lostFocus()
{
    ui->txtcPoblacionAlmacen->setText(ui->txtcPoblacionAlmacen->text().toUpper());
    if (ui->txtcCpPoblacionAlmacen->text().isEmpty() and !ui->txtcPoblacionAlmacen->text().isEmpty()) {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcPoblacionAlmacen->text(),1);
       if(BuscarPoblacion.exec()) {
        //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
           int nId = BuscarPoblacion.DevolverID();
           qDebug() <<nId;
         if(nId > 0) {
               QSqlQuery qPoblacion(QSqlDatabase::database("terra"));
               QString cId;
               cId = QString::number(nId);
               qPoblacion.prepare("select col_3 as poblacion, col_4 as CP,col_6 as provincia from poblaciones where col_1 = :cId");
               qPoblacion.bindValue(":id",cId);
               if(!qPoblacion.exec()) {
                  /* qDebug() << qPoblacion.lastQuery();
                   qDebug() << qPoblacion.value(0).toString(); */
                   QMessageBox::critical(NULL,tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
               } else {
                  // qDebug() << qPoblacion.lastQuery();
                   if (qPoblacion.next()) {
                        ui->txtcPoblacionAlmacen->setText(qPoblacion.value(0).toString());
                        ui->txtcCpPoblacionAlmacen->setText(qPoblacion.value(1).toString());
                        ui->txtcProvinciaAlmacen->setText(qPoblacion.value(2).toString());
                        ui->txtcPaisAlmacen->setText("ESPAÑA");
                   }
               }

           }
       }
      // BuscarPoblacion.close();
    }
}

void frmClientes::on_TablaDeudas_clicked(const QModelIndex &index)
{

}

void frmClientes::on_txtrRiesgoPermitido_lostFocus()
{
    Configuracion *o_conf = new Configuracion;
    ui->txtrRiesgoPermitido->setText(o_conf->FormatoNumerico(ui->txtrRiesgoPermitido->text()));
    delete o_conf;
}

void frmClientes::on_btnFichaPaciente_clicked()
{
    int idPaciente;
    //if not patient record in database, create it now.
    Paciente oPaciente;
    oPaciente.RecuperarPaciente(oCliente->getId());
    if(oPaciente.getid()==0){
        int idCliente = oCliente->getId();
        idPaciente = llamadasSQL->CrearPaciente(idCliente);
        if (idPaciente!=0)
            oPaciente.RecuperarPaciente(idCliente);
    }
    // open patient form
    FrmFichaPaciente *frmPaciente = new FrmFichaPaciente(this);
    frmPaciente->setWindowModality(Qt::WindowModal);
    frmPaciente->setWindowState(Qt::WindowMaximized);
    // conexiones
    connect(this,SIGNAL(enviahistoriaynombre(int,QString)),frmPaciente,SLOT(recibedatospaciente(int ,QString)));
    emit enviahistoriaynombre(oCliente->getId(),oCliente->getcNombreFiscal());
    frmPaciente->cargarDatos(oCliente->getId());

    frmPaciente->show();
}
