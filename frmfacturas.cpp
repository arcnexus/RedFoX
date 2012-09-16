#include "frmfacturas.h"
#include "ui_frmfacturas.h"
#include "factura.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QErrorMessage>
#include<QSqlError>
#include<QDate>
#include <QSqlQueryModel>
#include <QDebug>
#include "frmbuscarcliente.h"
#include "frmBuscarFactura.h"
#include <QTableView>
#include <QHeaderView>
#include "articulo.h"
#include "cliente.h"
#include "frmmodificarlin_fac.h"
#include "columnamoneda.h"


Factura *oFactura = new Factura();
Cliente *oCliente1 = new Cliente();
ColumnaMoneda *Columna = new ColumnaMoneda();
frmFacturas::frmFacturas(Configuracion *o_config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmFacturas)
{
    o_configuracion =o_config;
    o_configuracion->CargarDatos();
    ui->setupUi(this);
    // Escondo/muestro campos según configuración
    if(o_configuracion->lProfesional) {
        ui->txtnIRPF->setVisible(true);
        ui->txtrImporteIRPF->setVisible(true);
        ui->txtrImporteIRPF_2->setVisible(true);
        ui->lblIRPF->setVisible(true);
        ui->lblIRPF_2->setVisible(true);
    } else {
        ui->txtnIRPF->setVisible(false);
        ui->txtrImporteIRPF->setVisible(false);
        ui->txtrImporteIRPF_2->setVisible(false);
        ui->lblIRPF->setVisible(false);
        ui->lblIRPF_2->setVisible(false);
    }


    // Pongo valores por defecto
    ui->lblContabilizada->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lblFacturaImpresa->setVisible(false);
    // Rellenar formas de pago
    modelFP = new QSqlQueryModel();
    modelFP->setQuery("Select cFormaPago,id from FormPago",QSqlDatabase::database("empresa"));
    ui->txtcFormaPago->setModel(modelFP);
    // valores edicion
    this->Altas = false;
    ui->txtcCodigoArticulo->setFocus();
    BloquearCampos();
}

frmFacturas::~frmFacturas()
{
    delete ui;
    delete oCliente1;
    delete oFactura;
    delete Columna;
}

void frmFacturas::lineasVentas()
{
    // lineas de ventas
    QString cSQL;
    QString cId;

    cId = cId.number(oFactura->Getid());
     cSQL ="select id,cCodigo,nCantidad,cDescripcion,rPvp,rSubtotal,nDto,rDto,rTotal, nPorcIva from lin_fac "
             " where id_Cab = "+cId;
     ModelLin_fac = new QSqlQueryModel();
     ModelLin_fac->setQuery(cSQL,QSqlDatabase::database("empresa"));
     ui->Lineas->setModel(ModelLin_fac);
      //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
     Cabecera = new QHeaderView(Qt::Horizontal,this);
     // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
     ui->Lineas->setHorizontalHeader(Cabecera);
     /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0". (en nuestro caso está oculta ya que muestra el id de la tabla y esto no nos interesa que lo vea el usuario */
     Cabecera->setResizeMode(0,QHeaderView::Fixed);
     Cabecera->resizeSection(0,0);
     Cabecera->setResizeMode(1,QHeaderView::Fixed);
     Cabecera->resizeSection(1,85);
     Cabecera->setResizeMode(2,QHeaderView::Fixed);
     Cabecera->resizeSection(2,65);
     Cabecera->setResizeMode(3,QHeaderView::Fixed);
     Cabecera->resizeSection(3,213);
     Cabecera->setResizeMode(4,QHeaderView::Fixed);
     Cabecera->resizeSection(4,120);
     Cabecera->setResizeMode(9,QHeaderView::Fixed);
     Cabecera->resizeSection(9,35);
     ModelLin_fac->setHeaderData(1, Qt::Horizontal, QObject::tr("CÓDIGO"));
     ModelLin_fac->setHeaderData(2, Qt::Horizontal, QObject::tr("CANTIDAD"));
     ModelLin_fac->setHeaderData(3, Qt::Horizontal, QObject::tr("DESCRIPCIÓN"));
     ModelLin_fac->setHeaderData(4, Qt::Horizontal, QObject::tr("PVP"));
     ModelLin_fac->setHeaderData(5, Qt::Horizontal, QObject::tr("SUBTOTAL"));
     ModelLin_fac->setHeaderData(6, Qt::Horizontal, QObject::tr("%DTO"));
     ModelLin_fac->setHeaderData(7, Qt::Horizontal, QObject::tr("DTO"));
     ModelLin_fac->setHeaderData(8, Qt::Horizontal, QObject::tr("BASE"));
     ModelLin_fac->setHeaderData(9, Qt::Horizontal, QObject::tr("%IVA"));

    // Hacemos visible la cabecera
     Cabecera->setVisible(true);
     // Delegamos el control contenido en las columnas al nuevo objeto ColumnaGrid
     ui->Lineas->setItemDelegateForColumn(4, Columna);
     ui->Lineas->setItemDelegateForColumn(5, Columna);
     ui->Lineas->setItemDelegateForColumn(6, Columna);
     ui->Lineas->setItemDelegateForColumn(7, Columna);
     ui->Lineas->setItemDelegateForColumn(8, Columna);
}


void frmFacturas::LLenarCampos() {
    int lEstado;
    ui->txtcCodigoCliente->setText(oFactura->getcCodigoCliente());
    ui->txtcFactura->setText(oFactura->getcFactura());
    ui->txtdFecha->setDate(oFactura->getdFecha());
    ui->txtdFechaCobro->setDate(oFactura->getdFechaCobro());
    ui->txtcCliente->setText(oFactura->getcCliente());
    ui->txtcDireccion->setText(oFactura->getcDireccion());
    ui->txtcDireccion2->setText(oFactura->getcDireccion2());
    ui->txtcCp->setText(oFactura->getcCp());
    ui->txtcPoblacion->setText(oFactura->getcPoblacion());
    ui->txtcProvincia->setText(oFactura->getcProvincia());
    ui->txtcPais->setText(oFactura->getcPais());
    ui->txtcCif->setText(oFactura->getcCif());
     lEstado = oFactura->getlRecargoEquivalencia();
    if ((lEstado= 1)) {
        ui->chklRecargoEquivalencia->setChecked(true);
    } else {
        ui->chklRecargoEquivalencia->setChecked(false);
    }
    ui->txtrSubtotal->setText(o_configuracion->FormatoNumerico( QString::number(oFactura->getrSubtotal(),'f',2)));
    ui->txtnDto->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnDto(),'f',2)));
    ui->txtnDtoPP->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnDtoPP(),'f',2)));
    ui->txtrImporteDescuento->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteDescuento(),'f',2)));
    ui->txtrImporteDescuentoPP->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteDescuentoPP(),'f',2)));
    ui->txtrBase->setText(o_configuracion->FormatoNumerico(QString::number( oFactura->getrBase(),'f',2)));
    ui->txtnIva_2->setText(o_configuracion->FormatoNumerico(QString::number( oFactura->getnIva(),'f',2)));
    ui->txtrImporteIva->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteIva(),'f',2)));
    ui->txtrTotal->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal(),'f',2)));
    lEstado = oFactura->getlImpresa();
    if((lEstado == 1)) {
        ui->lblFacturaImpresa->setVisible(true);
    } else {
        ui->lblFacturaImpresa->setVisible(false);
    }
    lEstado = oFactura->getlCobrada();
    if ((lEstado == 1)) {
        ui->lblFacturaCobrada->setVisible(true);
        ui->txtdFechaCobro->setVisible(true);
    } else {
        ui->lblFacturaCobrada->setVisible(false);
        ui->txtdFechaCobro->setVisible(false);
    }
    lEstado = oFactura->getlContablilizada();

    if((lEstado == 1)) {
        ui->lblContabilizada->setVisible(true);
    } else {
        ui->lblContabilizada->setVisible(false);
    }
    int indice=ui->txtcFormaPago->findText(oFactura->getcFormaPago());
    if(indice!=-1)
     ui->txtcFormaPago->setCurrentIndex(indice);
    ui->txttComentario->setText(oFactura->gettComentario());
    ui->txtrBase1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase1(),'f',2)));
    ui->txtrBase2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase2(),'f',2)));
    ui->txtrBase3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase3(),'f',2)));
    ui->txtrBase4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase4(),'f',2)));
    ui->txtnPorcentajeIva1->setText(QString::number(oFactura->getnPorcentajeIVA1()));
    ui->txtnPorcentajeIva2->setText(QString::number(oFactura->getnPorcentajeIVA2()));
    ui->txtnPorcentajeIva3->setText(QString::number(oFactura->getnPorcentajeIVA3()));
    ui->txtnPorcentajeIva4->setText(QString::number(oFactura->getnPorcentajeIVA4()));
    ui->txtrIVA1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA1(),'f',2)));
    ui->txtrIVA2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA2(),'f',2)));
    ui->txtrIVA3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA3(),'f',2)));
    ui->txtrIVA4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA4(),'f',2)));
    ui->txtrTotal1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal1(),'f',2)));
    ui->txtrTotal2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal2(),'f',2)));
    ui->txtrTotal3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal3(),'f',2)));
    ui->txtrTotal4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal4(),'f',2)));
    ui->txtnRec1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnRec1(),'f',2)));
    ui->txtnRec2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnRec2(),'f',2)));
    ui->txtnRec3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnRec3(),'f',2)));
    ui->txtnRec4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnRec4(),'f',2)));
    ui->txtrRecargoEq1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq1(),'f',2)));
    ui->txtrRecargoEq2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq2(),'f',2)));
    ui->txtrRecargoEq3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq3(),'f',2)));
    ui->txtrRecargoEq4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq4(),'f',2)));
    ui->txtrTotalRecargoEq->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotalRecargoEq(),'f',2)));
    ui->txtrEntregadoaCuenta->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrEntregadoaCuenta(),'f',2)));
    ui->txtrImportePendiente->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImportePendiente(),'f',2)));
    ui->txtcCodigoEntidad->setText(oFactura->getcCodigoEntidad());
    ui->txtcOficinaEntidad->setText(oFactura->getcOficinaEntidad());
    ui->txtcDCCuenta->setText(oFactura->getcDCCuenta());
    ui->txtcNumeroCuenta->setText(oFactura->getcNumeroCuenta());
    ui->txtcPedidoCliente->setText(oFactura->getcPedidoCliente());
    // cargamos líneas de ventas
   lineasVentas();


}

void frmFacturas::LLenarCamposCliente()
{
    ui->txtcCodigoCliente->setText(oCliente1->getcCodigoCliente());
    ui->txtcCliente->setText(oCliente1->getcNombreFiscal());
    ui->txtcDireccion->setText(oCliente1->getcDireccion1());
    ui->txtcDireccion2->setText(oCliente1->getcDireccion2());
    ui->txtcCp->setText(oCliente1->getcCP());
    ui->txtcPoblacion->setText(oCliente1->getcPoblacion());
    ui->txtcProvincia->setText(oCliente1->getcProvincia());
    ui->txtcPais->setText(oCliente1->getcPais());
    ui->txtcCif->setText(oCliente1->getcCifNif());
    int lEstado = 0;
     lEstado = oCliente1->islRecargoEquivalencia();
    if ((lEstado= 1)) {
        ui->chklRecargoEquivalencia->setChecked(true);
    } else {
        ui->chklRecargoEquivalencia->setChecked(false);
    }
}

void frmFacturas::VaciarCampos() {
    QDate dFecha;
    ui->txtcCodigoCliente->setText("");
    ui->txtcFactura->setText("");
    ui->txtdFecha->setDate(dFecha.currentDate());
    ui->txtdFechaCobro->setDate(dFecha.currentDate());
    ui->txtcCliente->setText("");
    ui->txtcDireccion->setText("");
    ui->txtcDireccion2->setText("");
    ui->txtcCp->setText("");
    ui->txtcPoblacion->setText("");
    ui->txtcProvincia->setText("");
    ui->txtcPais->setText("");
    ui->txtcCif->setText("");
    ui->txtrSubtotal->setText(0);
    ui->txtnDto->setText(0);
    ui->txtnDtoPP->setText(0);
    ui->txtrImporteDescuento->setText("0,00");
    ui->txtrImporteDescuentoPP->setText("0,00");
    ui->txtrBase->setText("0,00");
    ui->txtnIva_2->setText(0);
    ui->txtrImporteIva->setText("0,00");
    ui->txtrTotal->setText("0,00");
    ui->lblFacturaImpresa->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lblContabilizada->setVisible(false);
    int indice=ui->txtcFormaPago->findText("Contado");
    if(indice!=-1) ui->txtcFormaPago->setCurrentIndex(indice);
    ui->txttComentario->setText("");
    ui->txtrBase1->setText(0);
    ui->txtrBase2->setText(0);
    ui->txtrBase3->setText(0);
    ui->txtrBase4->setText(0);
    ui->txtnPorcentajeIva1->setText(QString::number(o_configuracion->nIVA1));
    ui->txtnPorcentajeIva2->setText(QString::number(o_configuracion->nIVA2));
    ui->txtnPorcentajeIva3->setText(QString::number(o_configuracion->nIVA3));
    ui->txtnPorcentajeIva4->setText(QString::number(o_configuracion->nIVA4));
    ui->txtnRec1->clear();
    ui->txtnRec2->clear();
    ui->txtnRec3->clear();
    ui->txtnRec4->clear();
    ui->txtrIVA1->setText(0);
    ui->txtrIVA2->setText(0);
    ui->txtrIVA3->setText(0);
    ui->txtrIVA4->setText(0);
    ui->txtrTotal1->setText(0);
    ui->txtrTotal2->setText(0);
    ui->txtrTotal3->setText(0);
    ui->txtrTotal4->setText(0);
    ui->txtrRecargoEq1->setText(0);
    ui->txtrRecargoEq2->setText(0);
    ui->txtrRecargoEq3->setText(0);
    ui->txtrRecargoEq4->setText(0);
    ui->txtrTotalRecargoEq->setText(0);
    ui->txtrEntregadoaCuenta->setText("0,00");
    ui->txtrImportePendiente->setText("0,00");
    ui->txtcCodigoEntidad->setText("");
    ui->txtcOficinaEntidad->setText("");
    ui->txtcDCCuenta->setText("");
    ui->txtcNumeroCuenta->setText("");
    ui->txtcPedidoCliente->setText("");
    ui->chklRecargoEquivalencia->setCheckState(Qt::Unchecked);
}

void frmFacturas::BloquearCampos()
{
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
    /*
//    // SpinBox
//    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
//    QSpinBox *SpinBox;
//    foreach (SpinBox, SpinBoxList) {
//        SpinBox->setReadOnly(true);
//        //qDebug() << lineEdit->objectName();
//    }
//    // DoubleSpinBox
//    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
//    QDoubleSpinBox *DSpinBox;
//    foreach (DSpinBox, DSpinBoxList) {
//        DSpinBox->setReadOnly(true);
//        //qDebug() << lineEdit->objectName();
//    } */
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> textEditList = this->findChildren<QTextEdit *>();
    QTextEdit *textEdit;
    foreach (textEdit,textEditList) {
        textEdit->setReadOnly(true);
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
   // ui->btnBorrar->setEnabled(true);
    ui->btnBuscar->setEnabled(true);
    ui->btnCerrar->setEnabled(true);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);
    ui->botBorrarLinea->setEnabled(false);
    ui->botEditarLinea->setEnabled(false);
    ui->botBuscarCliente->setEnabled(false);

}

void frmFacturas::DesbloquearCampos()
{
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
//    // SpinBox
//    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
//    QSpinBox *SpinBox;
//    foreach (SpinBox, SpinBoxList) {
//        SpinBox->setReadOnly(false);
//        //qDebug() << lineEdit->objectName();
//    }
//    // DoubleSpinBox
//    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
//    QDoubleSpinBox *DSpinBox;
//    foreach (DSpinBox, DSpinBoxList) {
//        DSpinBox->setReadOnly(false);
//        //qDebug() << lineEdit->objectName();
//    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> textEditList = this->findChildren<QTextEdit *>();
    QTextEdit *textEdit;
    foreach (textEdit,textEditList) {
        textEdit->setReadOnly(false);
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
   // ui->btnBorrar->setEnabled(false);
    ui->btnBuscar->setEnabled(false);
    ui->btnCerrar->setEnabled(false);
    ui->btnDeshacer->setEnabled(true);
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnSiguiente->setEnabled(false);
    ui->botBorrarLinea->setEnabled(true);
    ui->botEditarLinea->setEnabled(true);
    ui->botBuscarCliente->setEnabled(true);
}

void frmFacturas::LLenarFactura() {
    oFactura->setcCodigoCliente(ui->txtcCodigoCliente->text());
    oFactura->setcFactura(ui->txtcFactura->text());
    oFactura->setdFecha(ui->txtdFecha->date());
    oFactura->setdFechaCobro(ui->txtdFechaCobro->date());
    oFactura->setcCliente(ui->txtcCliente->text());
    oFactura->setcDireccion(ui->txtcDireccion->text());
    oFactura->setcDireccion2(ui->txtcDireccion2->text());
    oFactura->setcCp(ui->txtcCp->text());
    oFactura->setcPoblacion(ui->txtcPoblacion->text());
    oFactura->setcProvincia(ui->txtcProvincia->text());
    oFactura->setcPais(ui->txtcPais->text());
    oFactura->setcCif(ui->txtcCif->text());
    if (ui->chklRecargoEquivalencia->isChecked()) {
        oFactura->setlRecargoEquivalencia(1);
    } else {
        oFactura->setlRecargoEquivalencia(0);
    }
    oFactura->setrSubtotal(ui->txtrSubtotal->text().toDouble());
    oFactura->setnDto(ui->txtnDto->text().toDouble());
    oFactura->setnDtoPP(ui->txtnDtoPP->text().toDouble());
    oFactura->setrImporteDescuento(ui->txtrImporteDescuento->text().toDouble());
    oFactura->setrImporteDescuentoPP(ui->txtrImporteDescuentoPP->text().toDouble());
    oFactura->setrBase(ui->txtrBase->text().toDouble());
    oFactura->setrImporteIva(ui->txtrImporteIva->text().toDouble());
    oFactura->setrTotal(ui->txtrTotal->text().toDouble());
    oFactura->setcFormaPago(ui->txtcFormaPago->currentText());
    oFactura->settComentario(ui->txttComentario->toPlainText());
    oFactura->setrBase1(ui->txtrBase1->text().toDouble());
    oFactura->setrBase2(ui->txtrBase2->text().toDouble());
    oFactura->setrBase3(ui->txtrBase3->text().toDouble());
    oFactura->setrBase4(ui->txtrBase4->text().toDouble());
    oFactura->setnPorcentajeIVA1(ui->txtnPorcentajeIva1->text().toDouble());
    oFactura->setnPorcentajeIVA2(ui->txtnPorcentajeIva2->text().toDouble());
    oFactura->setnPorcentajeIVA3(ui->txtnPorcentajeIva3->text().toDouble());
    oFactura->setnPorcentajeIVA4(ui->txtnPorcentajeIva4->text().toDouble());
    oFactura->setrIVA1(ui->txtrIVA1->text().toDouble());
    oFactura->setrIVA2(ui->txtrIVA2->text().toDouble());
    oFactura->setrIVA3(ui->txtrIVA3->text().toDouble());
    oFactura->setrIVA4(ui->txtrIVA4->text().toDouble());
    oFactura->setrTotal1(ui->txtrTotal1->text().toDouble());
    oFactura->setrTotal2(ui->txtrTotal2->text().toDouble());
    oFactura->setrTotal3(ui->txtrTotal3->text().toDouble());
    oFactura->setrTotal4(ui->txtrTotal4->text().toDouble());
    oFactura->setnRec1(ui->txtnRec1->text().toDouble());
    oFactura->setnRec2(ui->txtnRec2->text().toDouble());
    oFactura->setnRec3(ui->txtnRec3->text().toDouble());
    oFactura->setnRec4(ui->txtnRec4->text().toDouble());
    oFactura->setrRecargoEq1(ui->txtrRecargoEq1->text().toDouble());
    oFactura->setrRecargoEq2(ui->txtrRecargoEq2->text().toDouble());
    oFactura->setrRecargoEq3(ui->txtrRecargoEq3->text().toDouble());
    oFactura->setrRecargoEq4(ui->txtrRecargoEq4->text().toDouble());
    oFactura->setrTotalRecargoEq(ui->txtrTotalRecargoEq->text().toDouble());
    oFactura->setrEntregadoaCuenta(ui->txtrEntregadoaCuenta->text().toDouble());
    oFactura->setrImportePendiente(ui->txtrImportePendiente->text().toDouble());
    oFactura->setcCodigoEntidad(ui->txtcCodigoEntidad->text());
    oFactura->setcOficinaEntidad(ui->txtcOficinaEntidad->text());
    oFactura->setcDCCuenta(ui->txtcDCCuenta->text());
    oFactura->setcNumeroCuenta(ui->txtcNumeroCuenta->text());
    oFactura->setcPedidoCliente(ui->txtcPedidoCliente->text());
}

void frmFacturas::on_btnSiguiente_clicked()
{
    QString cFactura = oFactura->getcFactura();
    oFactura->RecuperarFactura("Select * from cab_fac where cFactura >'"+cFactura+"' order by cFactura  limit 0,1 ");
    LLenarCampos();
    //delete cFactura;
}
void frmFacturas::on_btnAnterior_clicked()
{
    QString cFactura = oFactura->getcFactura();
    oFactura->RecuperarFactura("Select * from cab_fac where cFactura <'"+cFactura+"' order by cFactura desc limit 0,1 ");
    LLenarCampos();
    //delete cFactura;
}


void frmFacturas::on_btnGuardar_clicked()
{
    int nId = oFactura->Getid();
    if (ui->txtcFactura->text() == tr("BORRADOR"))
        ui->txtcFactura->setText(oFactura->NuevoNumeroFactura());
    LLenarFactura();
    BloquearCampos();
    oFactura->GuardarFactura(nId,true);
    oFactura->RecuperarFactura("Select * from cab_fac where Id="+QString::number(nId)+"  limit 0,1 ");
    LLenarCampos();
}

void frmFacturas::on_btnAnadir_clicked()
{
    VaciarCampos();
    LLenarFactura();
    oFactura->setcFactura("BORRADOR");
    oFactura->AnadirFactura();

    LLenarCampos();
    DesbloquearCampos();
    ui->chklRecargoEquivalencia->setCheckState(Qt::Unchecked);

    ui->txtcCodigoCliente->setFocus();

}


void frmFacturas::on_btnEditar_clicked()
{
    if (oFactura->getcFactura() ==tr("BORRADOR")) {
        DesbloquearCampos();
    } else {
        QMessageBox::warning(NULL,tr("Editar Factura"),tr("No se puede editar una factura que ha sido numerada, solo los BORRADORES se pueden editar")+
                             tr("<p><b> Si necesita modificar algo genere una factura nueva y realice el abono correspondiente</b>"),tr("OK"));
    }
}



void frmFacturas::on_txtPVPArticulo_lostFocus()
{
    if (!ui->txtPVPArticulo->text().isEmpty()) {
        bool ok;
        ok = o_configuracion->EsNumero(ui->txtPVPArticulo->text());
        if(!ok) {
           QMessageBox::critical(NULL,tr("Entrada de Importe"),
                                 tr("No puede entrar letras en un campo monetario. \n"
                                    "Sólo se aceptan los valores  (-)  (0123456789) (,)  (.)  "),tr("&Aceptar"));
           ui->txtPVPArticulo->setText("0,00");
           ui->txtPVPArticulo->setSelection(0,4);
           ui->txtPVPArticulo->setFocus();
         } else {
            ui->txtPVPArticulo->setText(o_configuracion->FormatoNumerico(ui->txtPVPArticulo->text()) );
        }
        calcularTotalLinea();
    }
}



void frmFacturas::on_txtcCodigoArticulo_lostFocus()
{
    if (!ui->txtcCodigoArticulo->text().isEmpty()) {
        if (ui->txtDescripcionArticulo->text().isEmpty()) {
            Articulo *oArt =  new Articulo();
            oArt->Recuperar("Select * from articulos where cCodigo = '"+ui->txtcCodigoArticulo->text()+"'");
            ui->btnAnadirLinea->setToolTip("Código: "+ oArt->getcCodigo()+"<br>Descripción: "+oArt->getcDescripcion()+
                                           "<br><b>Stock:</b><font color = 'red'>"+QString::number(oArt->getnStockReal())+"</color>");
            ui->txtcCodigoArticulo->setText(oArt->getcCodigo());
            ui->txtDescripcionArticulo->setText(oArt->getcDescripcion());
            ui->txtPVPArticulo->setText(o_configuracion->FormatoNumerico(QString::number(oArt->getrTarifa1(),'f',2)));
            ui->txtcCantidadArticulo->setText("1");
            ui->txtSubtotalArticulo->setText(o_configuracion->FormatoNumerico(QString::number(oArt->getrTarifa1(),'f',2)));
            // Recupero datos cliente para determinar descuento en factura
            oCliente1->Recuperar("select * from clientes where id="+ QString::number(oFactura->getiId_Cliente()) );
            ui->txtPorcDtoArticulo->setText(QString::number(oCliente1->getnPorcDtoCliente(),'f',0));
            // Asigno el descuento mayor seleccionando entre dto ficha artículo y descuento ficha cliente
            if (oArt->getrDto() > oCliente1->getnPorcDtoCliente()) {
                ui->txtPorcDtoArticulo->setText(o_configuracion->FormatoNumerico(QString::number(oArt->getrDto(),'f',0)));
            }
            ui->txtPorcIVAArticulo->setText(QString::number(oArt->getnTipoIva(),'f',0));

        }
        calcularTotalLinea();
    }

}

void frmFacturas::on_txtcCantidadArticulo_lostFocus()
{
    double nSubtotal;
    nSubtotal = ui->txtcCantidadArticulo->text().toDouble() * ui->txtPVPArticulo->text().toDouble();
    ui->txtSubtotalArticulo->setText(o_configuracion->FormatoNumerico( QString::number(nSubtotal,'f',2)));
    calcularTotalLinea();
}

void frmFacturas::calcularTotalLinea()
{
    // Calculo totales línea
    double impDto,impTot,impSubtotal;
    impSubtotal = (ui->txtcCantidadArticulo->text().replace(".","").toDouble() * ui->txtPVPArticulo->text().replace(".","").toDouble());
    ui->txtSubtotalArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impSubtotal,'f',2)));
    impDto = (ui->txtSubtotalArticulo->text().replace(".","").toDouble() * ui->txtPorcDtoArticulo->text().toDouble())/100 ;
    ui->txtDtoArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impDto,'f',2)));
    impTot = ui->txtSubtotalArticulo->text().replace(".","").toDouble() - ui->txtDtoArticulo->text().replace(".","").toDouble();
    ui->txtTotalArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impTot,'f',2)));

}


void frmFacturas::on_txtPorcDtoArticulo_lostFocus()
{
    calcularTotalLinea();
    ui->txtcCodigoArticulo->setFocus();
}


void frmFacturas::on_btnAnadirLinea_clicked()
{
    if (!ui->txtDescripcionArticulo->text().isEmpty()) {
        double pvp =ui->txtPVPArticulo->text().replace(".","").toDouble();
        oFactura->AnadirLineaFactura(oFactura->Getid(),ui->txtcCodigoArticulo->text(),ui->txtcCantidadArticulo->text().toDouble(),
                                     ui->txtDescripcionArticulo->text(),ui->txtPVPArticulo->text().replace(".","").toDouble(),
                                     ui->txtSubtotalArticulo->text().replace(".","").toDouble(),ui->txtPorcDtoArticulo->text().toDouble(),
                                     ui->txtDtoArticulo->text().replace(".","").toDouble(),ui->txtTotalArticulo->text().replace(".","").toDouble(),
                                     ui->txtPorcIVAArticulo->text().toDouble());
        ui->txtcCodigoArticulo->setText("");
        ui->txtcCantidadArticulo->setText(0);
        ui->txtDescripcionArticulo->setText("");
        ui->txtPVPArticulo->setText(0);
        ui->txtSubtotalArticulo->setText(0);
        ui->txtPorcDtoArticulo->setText(0);
        ui->txtDtoArticulo->setText(0);
        ui->txtTotalArticulo->setText(0);
        ui->txtPorcIVAArticulo->setText(0);
        ui->txtcCodigoArticulo->setFocus();
    } else {
        QMessageBox::critical(NULL,tr("Insertar Línea"),tr("Debe especificar un artículo y una cantidad"),tr("&Aceptar"));
        ui->txtcCodigoArticulo->setFocus();
    }
    lineasVentas();
    // Calculo totales factura
    oFactura->calcularFactura();
    RellenarDespuesCalculo();

}

void frmFacturas::on_btnDeshacer_clicked()
{
    BloquearCampos();
    QString cId = QString::number(oFactura->Getid());
    oFactura->RecuperarFactura("Select * from cab_fac where id ="+cId+" order by id limit 0,1 ");
    LLenarCampos();
}


void frmFacturas::on_botRecalcular_clicked()
{
    oFactura->calcularFactura();
    RellenarDespuesCalculo();


}

void frmFacturas::on_botBuscarCliente_clicked()
{
    FrmBuscarCliente BuscarClientes;
    BuscarClientes.exec();
    int nId = BuscarClientes.DevolverID();
    //qDebug() << nId;
    QString cId = QString::number(nId);
    oCliente1->Recuperar("Select * from clientes where id ="+cId+" order by id limit 0,1 ");
    LLenarCamposCliente();
}

void frmFacturas::on_btnBuscarArt_clicked()
{
    if(!ui->txtcTextoBuscar->text().isEmpty()) {
        modArt = new QSqlQueryModel();
        QString cSQL = "select cCodigo,cDescripcion,rTarifa1,rTarifa2,rTarifa3 from articulos where cDescripcion like '%" +
                ui->txtcTextoBuscar->text().trimmed()+"%'";
        modArt->setQuery(cSQL,QSqlDatabase::database("empresa"));
        ui->tablaBuscaArt->setModel(modArt);
        ui->tablaBuscaArt->setItemDelegateForColumn(2, Columna);
        ui->tablaBuscaArt->setItemDelegateForColumn(3, Columna);
        ui->tablaBuscaArt->setItemDelegateForColumn(4, Columna);
    } else {
        QMessageBox::critical(this,tr("Buscar Artículos"),tr("Debe especificar algún criterio de búsqueda"),tr("OK"));
    }
    //delete modArt;

}


void frmFacturas::on_tablaBuscaArt_doubleClicked(const QModelIndex &index)
{
    QString texto = index.model()->data(index, Qt::DisplayRole).toString();
    ui->btnEditar->click();
    ui->txtcCodigoArticulo->setText(texto);
    ui->txtcCodigoArticulo->setFocus();
}

void frmFacturas::on_chklRecargoEquivalencia_stateChanged(int arg1)
{
    if(arg1==2){
        ui->txtnRec1->setText(QString::number(o_configuracion->nRE1,'f',1));
        ui->txtnRec2->setText(QString::number(o_configuracion->nRE2,'f',1));
        ui->txtnRec3->setText(QString::number(o_configuracion->nRE3,'f',1));
        ui->txtnRec4->setText(QString::number(o_configuracion->nRE4,'f',1));
    } else {
        ui->txtnRec1->setText("0");
        ui->txtnRec2->setText("0");
        ui->txtnRec3->setText("0");
        ui->txtnRec4->setText("0");
    }
}

void frmFacturas::on_btnBuscar_clicked()
{
    FrmBuscarFactura BuscarFactura;
    BuscarFactura.exec();
    int nId = BuscarFactura.DevolverID();
    //qDebug() << nId;
    QString cId = QString::number(nId);
    oFactura->RecuperarFactura("Select * from cab_fac where Id ="+cId+" limit 0,1 ");
    LLenarCampos();
}


void frmFacturas::on_botEditarLinea_clicked()
{
    QModelIndex celda=ui->Lineas->currentIndex();
    QModelIndex index=ModelLin_fac->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=ModelLin_fac->data(index,Qt::EditRole);
    int Id_lin =  pKey.toInt();
    frmModificarLin_fac *Modificar = new frmModificarLin_fac();
    Modificar->ponerCampos(Id_lin);
    if (Modificar->exec() == QDialog::Accepted)

    {
        lineasVentas();
        oFactura->calcularFactura();
        RellenarDespuesCalculo();
    }
    delete Modificar;
}

void frmFacturas::on_txtcCodigoCliente_lostFocus()
{
    if (!ui->txtcCodigoCliente->text().isEmpty() && ui->txtcCliente->text().isEmpty()) {
        QString cSQL = "Select * from clientes where cCodigoCliente = '"+ui->txtcCodigoCliente->text().trimmed() +"' limit 0,1";
        oCliente1->Recuperar(cSQL);
        LLenarCamposCliente();
        ui->txtcCodigoArticulo->setFocus();
    }
}

void frmFacturas::on_btnImprimir_clicked()
{
   // KDReports::Report report;

}

void frmFacturas::RellenarDespuesCalculo()
{
    ui->txtrSubtotal->setText(o_configuracion->FormatoNumerico( QString::number(oFactura->getrSubtotal(),'f',2)));
    ui->txtrImporteDescuento->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteDescuento(),'f',2)));
    ui->txtrImporteDescuentoPP->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteDescuentoPP(),'f',2)));
    ui->txtrBase->setText(o_configuracion->FormatoNumerico(QString::number( oFactura->getrBase(),'f',2)));
    ui->txtnIva_2->setText(o_configuracion->FormatoNumerico(QString::number( oFactura->getnIva(),'f',2)));
    ui->txtrImporteIva->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteIva(),'f',2)));
    ui->txtrTotal->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal(),'f',2)));
    ui->txtrBase1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase1(),'f',2)));
    ui->txtrBase2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase2(),'f',2)));
    ui->txtrBase3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase3(),'f',2)));
    ui->txtrBase4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase4(),'f',2)));
    ui->txtrIVA1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA1(),'f',2)));
    ui->txtrIVA2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA2(),'f',2)));
    ui->txtrIVA3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA3(),'f',2)));
    ui->txtrIVA4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA4(),'f',2)));
    ui->txtrTotal1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal1(),'f',2)));
    ui->txtrTotal2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal2(),'f',2)));
    ui->txtrTotal3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal3(),'f',2)));
    ui->txtrTotal4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal4(),'f',2)));
    ui->txtrRecargoEq1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq1(),'f',2)));
    ui->txtrRecargoEq2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq2(),'f',2)));
    ui->txtrRecargoEq3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq3(),'f',2)));
    ui->txtrRecargoEq4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq4(),'f',2)));
    ui->txtrTotalRecargoEq->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotalRecargoEq(),'f',2)));
}

void frmFacturas::on_botBorrarLinea_clicked()
{
    QModelIndex celda=ui->Lineas->currentIndex();
    QModelIndex index=ModelLin_fac->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=ModelLin_fac->data(index,Qt::EditRole);
    int Id_lin =  pKey.toInt();
    oFactura->BorrarLineaFactura(Id_lin);
    lineasVentas();
    RellenarDespuesCalculo();

}



void frmFacturas::on_botBorrador_clicked()
{
    int nId = oFactura->Getid();
    LLenarFactura();
    BloquearCampos();
    oFactura->setcFactura("BORRADOR");
    oFactura->GuardarFactura(nId,false);
    oFactura->RecuperarFactura("Select * from cab_fac where Id="+QString::number(nId)+"  limit 0,1 ");
    LLenarCampos();
}
