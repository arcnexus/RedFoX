#include "frmpresupuestoscli.h"
#include "ui_frmpresupuestoscli.h"
#include <QSqlQueryModel>
#include <presupuesto.h>
#include "configuracion.h"
#include "cliente.h"
#include "frmbuscarcliente.h"
#include "articulo.h"
#include <QMessageBox>
#include "columnamoneda.h"
#include "frmmodificarlin_precli.h"

Presupuesto *oPres = new Presupuesto();
Cliente *oClientePres = new Cliente();
ColumnaMoneda *ColumnaPres = new ColumnaMoneda();
FrmPresupuestosCli::FrmPresupuestosCli(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmPresupuestosCli)
{
    ui->setupUi(this);
    // cargar datos FormaPago
    ui->cboFormaPago->setInsertPolicy(QComboBox::NoInsert);
    ui->cboFormaPago->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    model=new QSqlQueryModel(this);
    model->setQuery("Select cFormapago from FormPago",QSqlDatabase::database("empresa"));
    ui->cboFormaPago->setModel(model);
    BloquearCampos();

}

FrmPresupuestosCli::~FrmPresupuestosCli()
{
    delete ui;
}

void FrmPresupuestosCli::LLenarCampos()
{
    Configuracion *o_conf = new Configuracion();
    ui->txtnPresupuesto->setText(QString::number(oPres->getnPresupuesto()));
    ui->txtdFecha->setDate(oPres->getdFecha());
    ui->txtdValidoHasta->setDate(oPres->getdValidoHasta());
    ui->txtcCodigoCliente->setText(oPres->getcCodigoCliente());
    ui->txtcCliente->setText(oPres->getcCliente());
    ui->txtcCif->setText(oPres->getcCif());
    ui->txtcDireccion->setText(oPres->getcDireccion());
    ui->txtcDireccion2->setText(oPres->getcDireccion2());
    ui->txtcCp->setText(oPres->getcCP());
    ui->txtcPoblacion->setText(oPres->getcPoblacion());
    ui->txtcProvincia->setText(oPres->getcProvincia());
    ui->txtcPais->setText(oPres->getcPais());
    ui->txtcTelefono->setText(oPres->getcTelefono());
    ui->txtcMovil->setText(oPres->getcMovil());
    ui->txtcFax->setText(oPres->getcFax());
    ui->txtnDto->setText(QString::number(oPres->getnDto()));
    ui->txttComentario->setPlainText(oPres->gettComentarios());
    ui->txtrBase->setText(o_conf->FormatoNumerico(QString::number(oPres->getrImporte(),'f',2)));
    ui->txtrSubtotal->setText(o_conf->FormatoNumerico(QString::number(oPres->getrSubTotal(),'f',2)));
    ui->txtrDescuento->setText(o_conf->FormatoNumerico(QString::number(oPres->getrDescuento(),'f',2)));
    ui->txtrTotal->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal(),'f',2)));
    ui->lblImpreso->setVisible(oPres->getlImpreso());
    ui->chklAprovado->setChecked(oPres->getlAprobado());
    ui->txtdFechaAprovacion->setDate(oPres->getdFechaAprobacion());
    ui->txtrImporteFactura->setText(o_conf->FormatoNumerico(QString::number(oPres->getrImporteFactura(),'f',2)));
    ui->txtcFactura->setText(oPres->getcFactura());
    ui->txtnAlbaran->setText(QString::number(oPres->getnAlbaran()));
    ui->txtnPedido->setText(QString::number(oPres->getnPedido()));
    int nIndex =  ui->cboFormaPago->findText(oPres->getcDescripcionFormaPago());
    if (!nIndex ==-1)
        ui->cboFormaPago->setCurrentIndex(nIndex);
    ui->txttLugarEntrega->setPlainText(oPres->gettLugarEntrega());
    ui->txtcAtencionde->setText(oPres->getcAtencionde());
    ui->txtrBase1->setText(o_conf->FormatoNumerico(QString::number(oPres->getrBase1(),'f',2)));
    ui->txtrBase2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrBase2(),'f',2)));
    ui->txtrBase3->setText(o_conf->FormatoNumerico(QString::number(oPres->getrBase3(),'f',2)));
    ui->txtrBase4->setText(o_conf->FormatoNumerico(QString::number(oPres->getrBase4(),'f',2)));
    ui->txtrBaseTotal->setText(o_conf->FormatoNumerico(QString::number(oPres->getrImporte(),'f',2)));

    ui->txtnPorcentajeIva1->setText(QString::number(oPres->getnIva1()));
    ui->txtnPorcentajeIva2->setText(QString::number(oPres->getnIva2()));
    ui->txtnPorcentajeIva3->setText(QString::number(oPres->getnIva3()));
    ui->txtnPorcentajeIva4->setText(QString::number(oPres->getnIva4()));
    ui->txtrIVA1->setText(o_conf->FormatoNumerico(QString::number(oPres->getrIva1(),'f',2)));
    ui->txtrIVA2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrIva2(),'f',2)));
    ui->txtrIVA3->setText(o_conf->FormatoNumerico(QString::number(oPres->getrIva3(),'f',2)));
    ui->txtrIVA4->setText(o_conf->FormatoNumerico(QString::number(oPres->getrIva4(),'f',2)));
    ui->txtrTotalIVA->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotalIva(),'f',2)));
    ui->txtrImporteIva->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotalIva(),'f',2)));
    ui->txtnRec1->setText(QString::number(oPres->getnRecargoEquivalencia1()));
    ui->txtnRec2->setText(QString::number(oPres->getnRecargoEquivalencia2()));
    ui->txtnRec3->setText(QString::number(oPres->getnRecargoEquivalencia3()));
    ui->txtnRec4->setText(QString::number(oPres->getnRecargoEquivalencia4()));
    ui->txtrTotalRecargoEq->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotalRec(),'f',2)));
    ui->txtrTotalRecargoEq_2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotalRec(),'f',2)));
    ui->txtrRecargoEq1->setText(o_conf->FormatoNumerico(QString::number(oPres->getrRec1(),'f',2)));
    ui->txtrRecargoEq2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrRec2(),'f',2)));
    ui->txtrRecargoEq3->setText(o_conf->FormatoNumerico(QString::number(oPres->getrRec3(),'f',2)));
    ui->txtrRecargoEq4->setText(o_conf->FormatoNumerico(QString::number(oPres->getrRec4(),'f',2)));
    ui->txtrTotal1->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal1(),'f',2)));
    ui->txtrTotal2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal2(),'f',2)));
    ui->txtrTotal3->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal3(),'f',2)));
    ui->txtrTotal4->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal4(),'f',2)));
    ui->txtrTotal_2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal(),'f',2)));
    ui->txtcEmail->setText(oPres->getcEmail());
    ui->chklRecargoEq->setChecked(oPres->getlRecargoEquivalencia());
    LineasVentas();
}

void FrmPresupuestosCli::LLenarCamposCliente()
{
    ui->txtcCodigoCliente->setText(oClientePres->getcCodigoCliente());
    ui->txtcCliente->setText(oClientePres->getcNombreFiscal());
    ui->txtcDireccion->setText(oClientePres->getcDireccion1());
    ui->txtcDireccion2->setText(oClientePres->getcDireccion2());
    ui->txtcCp->setText(oClientePres->getcCP());
    ui->txtcPoblacion->setText(oClientePres->getcPoblacion());
    ui->txtcProvincia->setText(oClientePres->getcProvincia());
    ui->txtcPais->setText(oClientePres->getcPais());
    ui->txtcCif->setText(oClientePres->getcCifNif());
    ui->txtcTelefono->setText(oClientePres->getcTelefono1());
    ui->txtcFax->setText(oClientePres->getcFax());
    ui->txtcMovil->setText(oClientePres->getcMovil());
    ui->txtcEmail->setText(oClientePres->getcEmail());
    if (oClientePres->getlIRPF()==1) {
        ui->chklRecargoEq->setChecked(true);
        oPres->setlRecargoEquivalencia(1);
    } else {
        ui->chklRecargoEq->setChecked(false);
        oPres->setlRecargoEquivalencia(0);
    }
}

void FrmPresupuestosCli::LLenarPresupuesto()
{
    oPres->setnPresupuesto(ui->txtnPresupuesto->text().toInt());
    oPres->setdFecha(ui->txtdFecha->date());
    oPres->setdValidoHasta(ui->txtdValidoHasta->date());
    oPres->setcCodigoCliente(ui->txtcCodigoCliente->text());
    oPres->setcCliente(ui->txtcCliente->text());
    oPres->setcCif(ui->txtcCif->text());
    oPres->setcDireccion(ui->txtcDireccion->text());
    oPres->setcDireccion2(ui->txtcDireccion2->text());
    oPres->setcCP(ui->txtcCp->text());
    oPres->setcPoblacion(ui->txtcPoblacion->text());
    oPres->setcProvincia(ui->txtcProvincia->text());
    oPres->setcPais(ui->txtcPais->text());
    oPres->setcTelefono(ui->txtcTelefono->text());
    oPres->setcMovil(ui->txtcMovil->text());
    oPres->setcFax(ui->txtcFax->text());
    oPres->setnDto(ui->txtnDto->text().toInt());
    oPres->settComentarios(ui->txttComentario->toPlainText());
    oPres->setrImporte(ui->txtrBase->text().replace(".","").toDouble());
    oPres->setrSubtotal(ui->txtrSubtotal->text().replace(".","").toDouble());
    oPres->setrDescuento(ui->txtrDescuento->text().replace(".","").toDouble());
    oPres->setrTotal(ui->txtrTotal->text().replace(".","").toDouble());
    if (ui->chklAprovado->isChecked())
        oPres->setlAprobado(1);
    else
        oPres->setlAprobado(0);
    oPres->setdFechaAprobacion(ui->txtdFechaAprovacion->date());
    oPres->setrImporteFactura(ui->txtrImporteFactura->text().replace(".","").toDouble());
    oPres->setcFactura(ui->txtcFactura->text());
    oPres->setnAlbaran(ui->txtnAlbaran->text().toInt());
    oPres->setnPedido(ui->txtnPedido->text().toInt());
    oPres->setcDescripcionFormaPago(ui->cboFormaPago->currentText());
    QSqlQuery *qFormPago = new QSqlQuery(QSqlDatabase::database("empresa"));
    qFormPago->exec("Select id,cCodigo from FormPago where cFormapago ='"+oPres->getcDescripcionFormaPago()+"'");
    qFormPago->next();
    QSqlRecord rFormPago = qFormPago->record();
    oPres->setcCodigoFormaPago(rFormPago.field("cCodigo").value().toString());
    oPres->setid_FormaPago(rFormPago.field("id").value().toInt());
    delete qFormPago;
    oPres->settLugarEntrega(ui->txttLugarEntrega->toPlainText());
    oPres->setcAtencionde(ui->txtcAtencionde->text());
    oPres->setrbase1(ui->txtrBase1->text().replace(".","").toDouble());
    oPres->setrbase2(ui->txtrBase2->text().replace(".","").toDouble());
    oPres->setrbase3(ui->txtrBase3->text().replace(".","").toDouble());
    oPres->setrbase4(ui->txtrBase4->text().replace(".","").toDouble());
    oPres->setnIva1(ui->txtnPorcentajeIva1->text().toDouble());
    oPres->setnIva2(ui->txtnPorcentajeIva2->text().toDouble());
    oPres->setnIva3(ui->txtnPorcentajeIva3->text().toDouble());
    oPres->setnIva4(ui->txtnPorcentajeIva4->text().toDouble());
    oPres->setrIva1(ui->txtrIVA1->text().replace(".","").toDouble());
    oPres->setrIva2(ui->txtrIVA2->text().replace(".","").toDouble());
    oPres->setrIva3(ui->txtrIVA3->text().replace(".","").toDouble());
    oPres->setrIva4(ui->txtrIVA4->text().replace(".","").toDouble());
    oPres->setnRecargoEquivalencia1(ui->txtnRec1->text().toDouble());
    oPres->setnRecargoEquivalencia2(ui->txtnRec2->text().toDouble());
    oPres->setnRecargoEquivalencia3(ui->txtnRec3->text().toDouble());
    oPres->setnRecargoEquivalencia4(ui->txtnRec4->text().toDouble());
    oPres->setrRec1(ui->txtrRecargoEq1->text().replace(".","").toDouble());
    oPres->setrRec2(ui->txtrRecargoEq2->text().replace(".","").toDouble());
    oPres->setrRec3(ui->txtrRecargoEq3->text().replace(".","").toDouble());
    oPres->setrRec4(ui->txtrRecargoEq4->text().replace(".","").toDouble());
    oPres->setrTotal1(ui->txtrTotal1->text().replace(".","").toDouble());
    oPres->setrTotal2(ui->txtrTotal2->text().replace(".","").toDouble());
    oPres->setrTotal3(ui->txtrTotal3->text().replace(".","").toDouble());
    oPres->setrTotal4(ui->txtrTotal4->text().replace(".","").toDouble());
    oPres->setcEmail(ui->txtcEmail->text());
}

void FrmPresupuestosCli::VaciarCampos()
{
    ui->txtnPresupuesto->setText("0");
    ui->txtdFecha->setDate(QDate::currentDate());
    ui->txtdValidoHasta->setDate(QDate::currentDate());
    ui->txtcCodigoCliente->setText("");
    ui->txtcCif->setText("");
    ui->txtcDireccion->setText("");
    ui->txtcDireccion2->setText("");
    ui->txtcCp->setText("");
    ui->txtcPoblacion->setText("");
    ui->txtcProvincia->setText("");
    ui->txtcPais->setText("");
    ui->txtcTelefono->setText("");
    ui->txtcMovil->setText("");
    ui->txtcFax->setText("");
    ui->txtnDto->setText("0");
    ui->txttComentario->setPlainText("");
    ui->txtrBase->setText("0,00");
    ui->txtrSubtotal->setText("0,00");
    ui->txtrDescuento->setText("0,00");
    ui->txtrTotal->setText("0,00");
    ui->lblImpreso->setVisible(false);
    ui->chklAprovado->setChecked(false);
    ui->txtdFechaAprovacion->setDate(QDate::currentDate());
    ui->txtrImporteFactura->setText("0,00");
    ui->txtcFactura->setText("");
    ui->txtnAlbaran->setText("");
    ui->txtnPedido->setText("");
    ui->cboFormaPago->setCurrentIndex(-1);
    ui->txttLugarEntrega->setPlainText("");
    ui->txtcAtencionde->setText("");
    ui->txtrBase1->setText("0,00");
    ui->txtrBase2->setText("0,00");
    ui->txtrBase3->setText("0,00");
    ui->txtrBase4->setText("0,00");
    ui->txtnPorcentajeIva1->setText("0");
    ui->txtnPorcentajeIva2->setText("0");
    ui->txtnPorcentajeIva3->setText("0");
    ui->txtnPorcentajeIva4->setText("0");
    ui->txtrIVA1->setText("0,00");
    ui->txtrIVA2->setText("0,00");
    ui->txtrIVA3->setText("0,00");
    ui->txtrIVA4->setText("0,00");
    ui->txtnRec1->setText("0");
    ui->txtnRec2->setText("0");
    ui->txtnRec3->setText("0");
    ui->txtnRec4->setText("0");
    ui->txtrRecargoEq1->setText("0,00");
    ui->txtrRecargoEq2->setText("0,00");
    ui->txtrRecargoEq3->setText("0,00");
    ui->txtrRecargoEq4->setText("0,00");
    ui->txtrTotal1->setText("0,00");
    ui->txtrTotal2->setText("0,00");
    ui->txtrTotal3->setText("0,00");
    ui->txtrTotal4->setText("0,00");
    ui->txtcEmail->setText("");
}

void FrmPresupuestosCli::BloquearCampos()
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
    ui->btnBorrar->setEnabled(true);
    ui->btnBuscar->setEnabled(true);
    ui->btnCerrar->setEnabled(true);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);
    ui->botBorrarLinea->setEnabled(false);
    ui->botEditarLinea->setEnabled(false);
    ui->botBuscarCliente->setEnabled(false);
    ui->btnFacturar->setEnabled(false);
    ui->btnAlbaran->setEnabled(false);
    ui->btnPedido->setEnabled(false);
}

void FrmPresupuestosCli::DesbloquearCampos()
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
    ui->btnBorrar->setEnabled(false);
    ui->btnBuscar->setEnabled(false);
    ui->btnCerrar->setEnabled(false);
    ui->btnDeshacer->setEnabled(true);
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnSiguiente->setEnabled(false);
    ui->botBorrarLinea->setEnabled(true);
    ui->botEditarLinea->setEnabled(true);
    ui->botBuscarCliente->setEnabled(true);
    ui->btnFacturar->setEnabled(true);
    ui->btnAlbaran->setEnabled(true);
    ui->btnPedido->setEnabled(true);
}

void FrmPresupuestosCli::CalcularTotalLinea()
{
    // Calculo totales línea
    Configuracion *o_configuracion = new Configuracion();
    double impDto,impTot,impSubtotal;
    impSubtotal = (ui->txtcCantidadArticulo->text().replace(".","").toDouble() * ui->txtPVPArticulo->text().replace(".","").toDouble());
    ui->txtSubtotalArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impSubtotal,'f',2)));
    impDto = (ui->txtSubtotalArticulo->text().replace(".","").toDouble() * ui->txtPorcDtoArticulo->text().replace(".","").toDouble())/100 ;
    ui->txtDtoArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impDto,'f',2)));
    impTot = ui->txtSubtotalArticulo->text().replace(".","").toDouble() - ui->txtDtoArticulo->text().replace(".","").toDouble();
    ui->txtTotalArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impTot,'f',2)));
    delete o_configuracion;
}

void FrmPresupuestosCli::RellenarDespuesCalculo()
{
    Configuracion *o_conf = new Configuracion();
    ui->txtnDto->setText(QString::number(oPres->getnDto()));
    ui->txtrBase->setText(o_conf->FormatoNumerico(QString::number(oPres->getrImporte(),'f',2)));
    ui->txtrSubtotal->setText(o_conf->FormatoNumerico(QString::number(oPres->getrSubTotal(),'f',2)));
    ui->txtrDescuento->setText(o_conf->FormatoNumerico(QString::number(oPres->getrDescuento(),'f',2)));
    ui->txtrTotal->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal(),'f',2)));
    ui->txtrImporteFactura->setText(o_conf->FormatoNumerico(QString::number(oPres->getrImporteFactura(),'f',2)));
    ui->txtrBase1->setText(o_conf->FormatoNumerico(QString::number(oPres->getrBase1(),'f',2)));
    ui->txtrBase2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrBase2(),'f',2)));
    ui->txtrBase3->setText(o_conf->FormatoNumerico(QString::number(oPres->getrBase3(),'f',2)));
    ui->txtrBase4->setText(o_conf->FormatoNumerico(QString::number(oPres->getrBase4(),'f',2)));
    ui->txtrBaseTotal->setText(o_conf->FormatoNumerico(QString::number(oPres->getrImporte(),'f',2)));
    ui->txtnPorcentajeIva1->setText(QString::number(oPres->getnIva1()));
    ui->txtnPorcentajeIva2->setText(QString::number(oPres->getnIva2()));
    ui->txtnPorcentajeIva3->setText(QString::number(oPres->getnIva3()));
    ui->txtnPorcentajeIva4->setText(QString::number(oPres->getnIva4()));
    ui->txtrIVA1->setText(o_conf->FormatoNumerico(QString::number(oPres->getrIva1(),'f',2)));
    ui->txtrIVA2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrIva2(),'f',2)));
    ui->txtrIVA3->setText(o_conf->FormatoNumerico(QString::number(oPres->getrIva3(),'f',2)));
    ui->txtrIVA4->setText(o_conf->FormatoNumerico(QString::number(oPres->getrIva4(),'f',2)));
    ui->txtrTotalIVA->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotalIva(),'f',2)));
    ui->txtrImporteIva->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotalIva(),'f',2)));
    ui->txtnRec1->setText(QString::number(oPres->getnRecargoEquivalencia1()));
    ui->txtnRec2->setText(QString::number(oPres->getnRecargoEquivalencia2()));
    ui->txtnRec3->setText(QString::number(oPres->getnRecargoEquivalencia3()));
    ui->txtnRec4->setText(QString::number(oPres->getnRecargoEquivalencia4()));
    ui->txtrTotalRecargoEq->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotalRec(),'f',2)));
    ui->txtrTotalRecargoEq_2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotalRec(),'f',2)));
    ui->txtrRecargoEq1->setText(o_conf->FormatoNumerico(QString::number(oPres->getrRec1(),'f',2)));
    ui->txtrRecargoEq2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrRec2(),'f',2)));
    ui->txtrRecargoEq3->setText(o_conf->FormatoNumerico(QString::number(oPres->getrRec3(),'f',2)));
    ui->txtrRecargoEq4->setText(o_conf->FormatoNumerico(QString::number(oPres->getrRec4(),'f',2)));
    ui->txtrTotal1->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal1(),'f',2)));
    ui->txtrTotal2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal2(),'f',2)));
    ui->txtrTotal3->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal3(),'f',2)));
    ui->txtrTotal4->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal4(),'f',2)));
    ui->txtrTotal_2->setText(o_conf->FormatoNumerico(QString::number(oPres->getrTotal(),'f',2)));
    delete o_conf;
}

void FrmPresupuestosCli::LineasVentas()
{
    // lineas de ventas
    QString cSQL;
    QString cId;

    cId = cId.number(oPres->getId());
    cSQL ="select id,cCodigo,nCantidad,cDescripcion,rPvp,rSubtotal,nPorcDto,rDto,rTotal, nPorcIva "
            " from lin_pre where Id_Cab = "+cId;
     ModelLin_pre = new QSqlQueryModel();
     ModelLin_pre->setQuery(cSQL,QSqlDatabase::database("empresa"));
     ui->Lineas->setModel(ModelLin_pre);
      //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
     QHeaderView *Cabecera = new QHeaderView(Qt::Horizontal,this);
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
     Cabecera->resizeSection(3,200);
     Cabecera->setResizeMode(4,QHeaderView::Fixed);
     Cabecera->resizeSection(4,90);
     Cabecera->setResizeMode(9,QHeaderView::Fixed);
     Cabecera->resizeSection(9,35);
     ModelLin_pre->setHeaderData(1, Qt::Horizontal, QObject::tr("CÓDIGO"));
     ModelLin_pre->setHeaderData(2, Qt::Horizontal, QObject::tr("CANTIDAD"));
     ModelLin_pre->setHeaderData(3, Qt::Horizontal, QObject::tr("DESCRIPCIÓN"));
     ModelLin_pre->setHeaderData(4, Qt::Horizontal, QObject::tr("PVP"));
     ModelLin_pre->setHeaderData(5, Qt::Horizontal, QObject::tr("SUBTOTAL"));
     ModelLin_pre->setHeaderData(6, Qt::Horizontal, QObject::tr("%DTO"));
     ModelLin_pre->setHeaderData(7, Qt::Horizontal, QObject::tr("DTO"));
     ModelLin_pre->setHeaderData(8, Qt::Horizontal, QObject::tr("BASE"));
     ModelLin_pre->setHeaderData(9, Qt::Horizontal, QObject::tr("%IVA"));

    // Hacemos visible la cabecera
     Cabecera->setVisible(true);
     // Delegamos el control contenido en las columnas al nuevo objeto ColumnaGrid
     ui->Lineas->setItemDelegateForColumn(4, ColumnaPres);
     ui->Lineas->setItemDelegateForColumn(5, ColumnaPres);
     ui->Lineas->setItemDelegateForColumn(6, ColumnaPres);
     ui->Lineas->setItemDelegateForColumn(7, ColumnaPres);
     ui->Lineas->setItemDelegateForColumn(8, ColumnaPres);
}

void FrmPresupuestosCli::on_chklEnviado_stateChanged(int arg1)
{
    if(arg1 == 0) {
        ui->lblFechaAprovacin->setVisible(false);
        ui->txtdFechaAprovacion->setVisible(false);
    } else {
        ui->lblFechaAprovacin->setVisible(true);
        ui->txtdFechaAprovacion->setVisible(true);
    }
}

void FrmPresupuestosCli::on_btnSiguiente_clicked()
{
    int nPresupuesto = oPres->getnPresupuesto();
    oPres->RecuperarPresupuesto("Select * from cab_pre where nPresupuesto >'"+
                                QString::number(nPresupuesto)+"' order by nPresupuesto  limit 1 ",0);
    LLenarCampos();
}

void FrmPresupuestosCli::on_btnAnterior_clicked()
{
    int nPresupuesto = oPres->getnPresupuesto();
    oPres->RecuperarPresupuesto("Select * from cab_pre where nPresupuesto <'"+QString::number(nPresupuesto)+
                                "' order by nPresupuesto desc limit 1 ",1);
    LLenarCampos();
}

void FrmPresupuestosCli::on_btnAnadir_clicked()
{
    VaciarCampos();
    LLenarPresupuesto();
    oPres->AnadirPresupuesto();
    LLenarCampos();
    DesbloquearCampos();
    ui->txtcCodigoCliente->setFocus();
}

void FrmPresupuestosCli::on_btnEditar_clicked()
{
    if (oPres->getcFactura()=="0") {
        DesbloquearCampos();
    } else {
        QMessageBox::warning(NULL,tr("Editar Presupuesto"),tr("No se puede editar un Presupuesto que ha sido facturado, solo los NO facturados se pueden editar")+
                             tr("<p><b> Si necesita modificar algo genere una factura nueva y realice el abono correspondiente</b>")+
                                tr("y luego si es preciso realice un nuevo presupuesto y facture de nuevo "),tr("OK"));
    }
}

void FrmPresupuestosCli::on_btnGuardar_clicked()
{
    int nId = oPres->getId();
    if (ui->txtnPresupuesto->text().trimmed() =="0")
        ui->txtnPresupuesto->setText(QString::number(oPres->NuevoNumeroPresupuesto()));
    LLenarPresupuesto();
    BloquearCampos();
    oPres->GuardarPres(nId);
    oPres->RecuperarPresupuesto("Select * from cab_pre where id="+QString::number(nId)+"  limit 1 ");
    LLenarCampos();
}

void FrmPresupuestosCli::on_btnBuscar_clicked()
{

}

void FrmPresupuestosCli::on_botBuscarCliente_clicked()
{
    FrmBuscarCliente BuscarClientes;
    BuscarClientes.exec();
    int nId = BuscarClientes.DevolverID();
    QString cId = QString::number(nId);
    oPres->setidCliente(cId.toInt());
    oClientePres->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
    LLenarCamposCliente();
}

void FrmPresupuestosCli::on_txtcCodigoArticulo_editingFinished()
{
    Configuracion *o_configuracion = new Configuracion();
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
            oClientePres->Recuperar("select * from clientes where id="+ QString::number(oPres->getid_cliente()) );
            ui->txtPorcDtoArticulo->setText(QString::number(oClientePres->getnPorcDtoCliente(),'f',0));
            // Asigno el descuento mayor seleccionando entre dto ficha artículo y descuento ficha cliente
            if (oArt->getrDto() > oClientePres->getnPorcDtoCliente()) {
                ui->txtPorcDtoArticulo->setText(o_configuracion->FormatoNumerico(QString::number(oArt->getrDto(),'f',0)));
            }
            ui->txtPorcIVAArticulo->setText(QString::number(oArt->getnTipoIva(),'f',0));

        }
        CalcularTotalLinea();
    }
    delete o_configuracion;
}

void FrmPresupuestosCli::on_btnAnadirLinea_clicked()
{
    if (!ui->txtDescripcionArticulo->text().isEmpty()) {
        //double pvp =ui->txtPVPArticulo->text().replace(".","").toDouble();
        oPres->AnadirLineaPresupuesto(oPres->getId(),ui->txtcCodigoArticulo->text(),ui->txtcCantidadArticulo->text().replace(".","").toDouble(),
                                     ui->txtDescripcionArticulo->text(),ui->txtPVPArticulo->text().replace(".","").toDouble(),
                                     ui->txtSubtotalArticulo->text().replace(".","").toDouble(),ui->txtPorcDtoArticulo->text().replace(".","").toDouble(),
                                     ui->txtDtoArticulo->text().replace(".","").toDouble(),ui->txtTotalArticulo->text().replace(".","").toDouble(),
                                     ui->txtPorcIVAArticulo->text().replace(".","").toDouble());
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
    LineasVentas();
    // Calculo totales Presupuesto
    oPres->calcularPresupuesto();
    RellenarDespuesCalculo();
}

void FrmPresupuestosCli::on_botEditarLinea_clicked()
{
    QModelIndex celda=ui->Lineas->currentIndex();
    QModelIndex index= ModelLin_pre->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=ModelLin_pre->data(index,Qt::EditRole);
    int Id_lin =  pKey.toInt();
    FrmModificarLin_precli *Modificar = new FrmModificarLin_precli();
    Modificar->PonerCampos(Id_lin);
    if (Modificar->exec() == QDialog::Accepted)

    {
        LineasVentas();
        oPres->calcularPresupuesto();
        RellenarDespuesCalculo();
    }
    delete Modificar;
}

void FrmPresupuestosCli::on_botBorrarLinea_clicked()
{
    QModelIndex celda=ui->Lineas->currentIndex();
    QModelIndex index=ModelLin_pre->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=ModelLin_pre->data(index,Qt::EditRole);
    int Id_lin =  pKey.toInt();
    oPres->BorrarLineaPresupuesto(Id_lin);
    LineasVentas();
    RellenarDespuesCalculo();
}
