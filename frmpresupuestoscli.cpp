#include "frmpresupuestoscli.h"
#include "ui_frmpresupuestoscli.h"
#include <QSqlQueryModel>

#include "Busquedas/frmbuscarcliente.h"
#include "articulo.h"
#include <QMessageBox>


FrmPresupuestosCli::FrmPresupuestosCli(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmPresupuestosCli),
    helper(this)
{
    ui->setupUi(this);

    // cargar datos FormaPago
    ui->cboFormaPago->setInsertPolicy(QComboBox::NoInsert);
    ui->cboFormaPago->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    model=new QSqlQueryModel(this);
    model->setQuery("Select cFormapago from FormPago",QSqlDatabase::database("empresa"));
    ui->cboFormaPago->setModel(model);
    BloquearCampos();

    //-----------------------
    // Conexiones
    //-----------------------
    oPres = new Presupuesto();
    oClientePres = new Cliente();

    Db_table_View* searcher = new Db_table_View(this);
    searcher->set_db("empresa");
    searcher->set_table("articulos");
    searcher->set_selection("cCodigo");
    searcher->setWindowTitle(tr("Articulos"));
    searcher->set_selection("cCodigo");
    QStringList headers;
    headers << tr("Codigo")<< "1" << "2" << tr("Descripción");
    searcher->set_table_headers(headers);

    searcher->set_columnHide(0);
    searcher->set_columnHide(2);
    searcher->set_columnHide(3);

    for(int i = 5; i<50;i++)
        searcher->set_columnHide(i);

    helper.set_Searcher(searcher);
    helper.set_Tipo(false);
    helper.help_table(ui->Lineas);

    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(&helper,SIGNAL(totalChanged(QString)),this,SLOT(totalChanged(QString)));
}

FrmPresupuestosCli::~FrmPresupuestosCli()
{
    delete ui;
    delete oPres;
    delete oClientePres;
}

void FrmPresupuestosCli::LLenarCampos()
{
    ui->txtnPresupuesto->setText(QString::number(oPres->nPresupuesto));
    ui->txtdFecha->setDate(oPres->dFecha);
    ui->txtdValidoHasta->setDate(oPres->dValidoHasta);
    ui->txtcCodigoCliente->setText(oPres->cCodigoCliente);
    ui->txtcCliente->setText(oPres->cCliente);
    ui->txtcCif->setText(oPres->cCif);
    ui->txtcDireccion->setText(oPres->cDireccion);
    ui->txtcDireccion2->setText(oPres->cDireccion2);
    ui->txtcCp->setText(oPres->cCP);
    ui->txtcPoblacion->setText(oPres->cPoblacion);
    ui->txtcProvincia->setText(oPres->cProvincia);
    ui->txtcPais->setText(QString::number(oPres->idPais));
    ui->txtcTelefono->setText(oPres->cTelefono);
    ui->txtcMovil->setText(oPres->cMovil);
    ui->txtcFax->setText(oPres->cFax);
    ui->txtnDto->setText(QString::number(oPres->nDto));
    ui->txttComentario->setPlainText(oPres->tComentarios);
    ui->txtrBase->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rImporte,'f',2)));
    ui->txtrSubtotal->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rSubTotal,'f',2)));
    ui->txtrDescuento->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rDescuento,'f',2)));
    ui->txtrTotal->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal,'f',2)));
    ui->lblImpreso->setVisible(oPres->lImpreso);
    ui->chklAprovado->setChecked(oPres->lAprobado);
    ui->txtdFechaAprovacion->setDate(oPres->dFechaAprobacion);
    ui->txtrImporteFactura->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rImporteFactura,'f',2)));
    ui->txtcFactura->setText(oPres->cFactura);
    ui->txtnAlbaran->setText(QString::number(oPres->nAlbaran));
    ui->txtnPedido->setText(QString::number(oPres->nPedido));
    int nIndex =  ui->cboFormaPago->findText(oPres->cDescripcionFormaPago);
    if (!nIndex ==-1)
        ui->cboFormaPago->setCurrentIndex(nIndex);
    ui->txttLugarEntrega->setPlainText(oPres->tLugarEntrega);
    ui->txtcAtencionde->setText(oPres->cAtencionde);
    ui->txtrBase1->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase1,'f',2)));
    ui->txtrBase2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase2,'f',2)));
    ui->txtrBase3->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase3,'f',2)));
    ui->txtrBase4->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase4,'f',2)));
    ui->txtrBaseTotal->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rImporte,'f',2)));

    ui->txtnPorcentajeIva1->setText(QString::number(oPres->nIva1));
    ui->txtnPorcentajeIva2->setText(QString::number(oPres->nIva2));
    ui->txtnPorcentajeIva3->setText(QString::number(oPres->nIva3));
    ui->txtnPorcentajeIva4->setText(QString::number(oPres->nIva4));
    ui->txtrIVA1->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva1,'f',2)));
    ui->txtrIVA2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva2,'f',2)));
    ui->txtrIVA3->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva3,'f',2)));
    ui->txtrIVA4->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva4,'f',2)));
    ui->txtrTotalIVA->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotalIva,'f',2)));
    ui->txtrImporteIva->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotalIva,'f',2)));
    ui->txtnRec1->setText(QString::number(oPres->nRecargoEquivalencia1));
    ui->txtnRec2->setText(QString::number(oPres->nRecargoEquivalencia2));
    ui->txtnRec3->setText(QString::number(oPres->nRecargoEquivalencia3));
    ui->txtnRec4->setText(QString::number(oPres->nRecargoEquivalencia4));
    ui->txtrTotalRecargoEq->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotalRec,'f',2)));
    ui->txtrTotalRecargoEq_2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotalRec,'f',2)));
    ui->txtrRecargoEq1->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rRec1,'f',2)));
    ui->txtrRecargoEq2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rRec2,'f',2)));
    ui->txtrRecargoEq3->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rRec3,'f',2)));
    ui->txtrRecargoEq4->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rRec4,'f',2)));
    ui->txtrTotal1->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal1,'f',2)));
    ui->txtrTotal2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal2,'f',2)));
    ui->txtrTotal3->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal3,'f',2)));
    ui->txtrTotal4->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal4,'f',2)));
    ui->txtrTotal_2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal,'f',2)));
    ui->txtcEmail->setText(oPres->cEmail);
    ui->chklRecargoEq->setChecked(oPres->lRecargoEquivalencia);
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
        oPres->lRecargoEquivalencia = 1;
    } else {
        ui->chklRecargoEq->setChecked(false);
        oPres->lRecargoEquivalencia = 0;
    }
}

void FrmPresupuestosCli::LLenarPresupuesto()
{
    oPres->nPresupuesto = (ui->txtnPresupuesto->text().toInt());
    oPres->dFecha = (ui->txtdFecha->date());
    oPres->dValidoHasta = (ui->txtdValidoHasta->date());
    oPres->cCodigoCliente = (ui->txtcCodigoCliente->text());
    oPres->cCliente = (ui->txtcCliente->text());
    oPres->cCif = (ui->txtcCif->text());
    oPres->cDireccion = (ui->txtcDireccion->text());
    oPres->cDireccion2 = (ui->txtcDireccion2->text());
    oPres->cCP = (ui->txtcCp->text());
    oPres->cPoblacion = (ui->txtcPoblacion->text());
    oPres->cProvincia = (ui->txtcProvincia->text());
    //TODO fix pais
    //oPres->cPais = (ui->txtcPais->text());
    oPres->cTelefono = (ui->txtcTelefono->text());
    oPres->cMovil = (ui->txtcMovil->text());
    oPres->cFax = (ui->txtcFax->text());
    oPres->nDto = (ui->txtnDto->text().toInt());
    oPres->tComentarios = (ui->txttComentario->toPlainText());
    oPres->rImporte = (ui->txtrBase->text().replace(".","").toDouble());
    oPres->rSubTotal = (ui->txtrSubtotal->text().replace(".","").toDouble());
    oPres->rDescuento = (ui->txtrDescuento->text().replace(".","").toDouble());
    oPres->rTotal = (ui->txtrTotal->text().replace(".","").toDouble());

    oPres->lAprobado = ui->chklAprovado->isChecked();

    oPres->dFechaAprobacion = (ui->txtdFechaAprovacion->date());
    oPres->rImporteFactura = (ui->txtrImporteFactura->text().replace(".","").toDouble());
    oPres->cFactura = (ui->txtcFactura->text());
    oPres->nAlbaran = (ui->txtnAlbaran->text().toInt());
    oPres->nPedido = (ui->txtnPedido->text().toInt());
    oPres->cDescripcionFormaPago = (ui->cboFormaPago->currentText());

    QSqlQuery qFormPago(QSqlDatabase::database("empresa"));
    qFormPago.exec("Select id,cCodigo from FormPago where cFormapago ='"+oPres->cDescripcionFormaPago+"'");
    qFormPago.next();
    QSqlRecord rFormPago = qFormPago.record();

    oPres->cCodigoFormaPago = (rFormPago.field("cCodigo").value().toString());
    oPres->id_FormaPago = (rFormPago.field("id").value().toInt());
    oPres->tLugarEntrega = (ui->txttLugarEntrega->toPlainText());
    oPres->cAtencionde = (ui->txtcAtencionde->text());
    oPres->rBase1 = (ui->txtrBase1->text().replace(".","").toDouble());
    oPres->rBase2 = (ui->txtrBase2->text().replace(".","").toDouble());
    oPres->rBase3 = (ui->txtrBase3->text().replace(".","").toDouble());
    oPres->rBase4 = (ui->txtrBase4->text().replace(".","").toDouble());
    oPres->nIva1 = (ui->txtnPorcentajeIva1->text().toDouble());
    oPres->nIva2 = (ui->txtnPorcentajeIva2->text().toDouble());
    oPres->nIva3 = (ui->txtnPorcentajeIva3->text().toDouble());
    oPres->nIva4 = (ui->txtnPorcentajeIva4->text().toDouble());
    oPres->rIva1 = (ui->txtrIVA1->text().replace(".","").toDouble());
    oPres->rIva2 = (ui->txtrIVA2->text().replace(".","").toDouble());
    oPres->rIva3 = (ui->txtrIVA3->text().replace(".","").toDouble());
    oPres->rIva4 = (ui->txtrIVA4->text().replace(".","").toDouble());
    oPres->nRecargoEquivalencia1 = (ui->txtnRec1->text().toDouble());
    oPres->nRecargoEquivalencia2 = (ui->txtnRec2->text().toDouble());
    oPres->nRecargoEquivalencia3 = (ui->txtnRec3->text().toDouble());
    oPres->nRecargoEquivalencia4 = (ui->txtnRec4->text().toDouble());
    oPres->rRec1 = (ui->txtrRecargoEq1->text().replace(".","").toDouble());
    oPres->rRec2 = (ui->txtrRecargoEq2->text().replace(".","").toDouble());
    oPres->rRec3 = (ui->txtrRecargoEq3->text().replace(".","").toDouble());
    oPres->rRec4 = (ui->txtrRecargoEq4->text().replace(".","").toDouble());
    oPres->rTotal1 = (ui->txtrTotal1->text().replace(".","").toDouble());
    oPres->rTotal2 = (ui->txtrTotal2->text().replace(".","").toDouble());
    oPres->rTotal3 = (ui->txtrTotal3->text().replace(".","").toDouble());
    oPres->rTotal4 = (ui->txtrTotal4->text().replace(".","").toDouble());
    oPres->cEmail = (ui->txtcEmail->text());
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
    }

    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(false);
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
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);
    //ui->botBorrarLinea->setEnabled(false);
    //ui->botEditarLinea->setEnabled(false);
    ui->botBuscarCliente->setEnabled(false);
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
    ui->btnDeshacer->setEnabled(true);
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnSiguiente->setEnabled(false);
    //ui->botBorrarLinea->setEnabled(true);
    //ui->botEditarLinea->setEnabled(true);
    ui->botBuscarCliente->setEnabled(true);

}

void FrmPresupuestosCli::RellenarDespuesCalculo()
{
    ui->txtnDto->setText(QString::number(oPres->nDto));
    ui->txtrBase->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rImporte,'f',2)));
    ui->txtrSubtotal->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rSubTotal,'f',2)));
    ui->txtrDescuento->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rDescuento,'f',2)));
    ui->txtrTotal->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal,'f',2)));
    ui->txtrImporteFactura->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rImporteFactura,'f',2)));
    ui->txtrBase1->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase1,'f',2)));
    ui->txtrBase2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase2,'f',2)));
    ui->txtrBase3->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase3,'f',2)));
    ui->txtrBase4->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase4,'f',2)));
    ui->txtrBaseTotal->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rImporte,'f',2)));
    ui->txtnPorcentajeIva1->setText(QString::number(oPres->nIva1));
    ui->txtnPorcentajeIva2->setText(QString::number(oPres->nIva2));
    ui->txtnPorcentajeIva3->setText(QString::number(oPres->nIva3));
    ui->txtnPorcentajeIva4->setText(QString::number(oPres->nIva4));
    ui->txtrIVA1->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva1,'f',2)));
    ui->txtrIVA2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva2,'f',2)));
    ui->txtrIVA3->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva3,'f',2)));
    ui->txtrIVA4->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva4,'f',2)));
    ui->txtrTotalIVA->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotalIva,'f',2)));
    ui->txtrImporteIva->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotalIva,'f',2)));
    ui->txtnRec1->setText(QString::number(oPres->nRecargoEquivalencia1));
    ui->txtnRec2->setText(QString::number(oPres->nRecargoEquivalencia2));
    ui->txtnRec3->setText(QString::number(oPres->nRecargoEquivalencia3));
    ui->txtnRec4->setText(QString::number(oPres->nRecargoEquivalencia4));
    ui->txtrTotalRecargoEq->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotalRec,'f',2)));
    ui->txtrTotalRecargoEq_2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotalRec,'f',2)));
    ui->txtrRecargoEq1->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rRec1,'f',2)));
    ui->txtrRecargoEq2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rRec2,'f',2)));
    ui->txtrRecargoEq3->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rRec3,'f',2)));
    ui->txtrRecargoEq4->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rRec4,'f',2)));
    ui->txtrTotal1->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal1,'f',2)));
    ui->txtrTotal2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal2,'f',2)));
    ui->txtrTotal3->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal3,'f',2)));
    ui->txtrTotal4->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal4,'f',2)));
    ui->txtrTotal_2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotal,'f',2)));
}

void FrmPresupuestosCli::on_chklAprovado_stateChanged(int arg1)
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
    int nPresupuesto = oPres->nPresupuesto;
    oPres->RecuperarPresupuesto("Select * from cab_pre where nPresupuesto >'"+
                                QString::number(nPresupuesto)+"' order by nPresupuesto  limit 1 ",0);
    LLenarCampos();
}

void FrmPresupuestosCli::on_btnAnterior_clicked()
{
    int nPresupuesto = oPres->nPresupuesto;
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
    if (oPres->cFactura=="0") {
        DesbloquearCampos();
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Editar Presupuesto"),tr("No se puede editar un Presupuesto que ha sido facturado, solo los NO facturados se pueden editar")+
                             tr("<p><b> Si necesita modificar algo genere una factura nueva y realice el abono correspondiente</b>")+
                                tr("y luego si es preciso realice un nuevo presupuesto y facture de nuevo "),tr("OK"));
    }
}

void FrmPresupuestosCli::on_btnGuardar_clicked()
{
    int nId = oPres->id;
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
    FrmBuscarCliente BuscarClientes(this);
    BuscarClientes.exec();
    int nId = BuscarClientes.DevolverID();
    QString cId = QString::number(nId);
    oPres->id_cliente = cId.toInt();
    oClientePres->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
    LLenarCamposCliente();
}

void FrmPresupuestosCli::totalChanged(QString total)
{
    ui->lbl_total->setText(total);
}
