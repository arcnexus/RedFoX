#include "frmpresupuestoscli.h"
#include "ui_frmpresupuestoscli.h"
#include <QSqlQueryModel>

#include "../Busquedas/frmbuscarcliente.h"
#include "../Almacen/articulo.h"
#include <QMessageBox>


FrmPresupuestosCli::FrmPresupuestosCli(QWidget *parent) :
    MayaModule(ModuleZone(),ModuleName(),parent),
    ui(new Ui::FrmPresupuestosCli),
    helper(this),
    toolButton(tr("Presupuestos"),":/Icons/PNG/presupuestos.png",this),
    menuButton(QIcon(":/Icons/PNG/presupuestos.png"),tr("Presupuestos"),this)
{
    ui->setupUi(this);

    ui->combo_Pais->setModel(Configuracion_global->paises_model);
    ui->combo_Pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    // cargar datos FormaPago
    ui->cboFormaPago->setInsertPolicy(QComboBox::NoInsert);
    ui->cboFormaPago->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    QSqlTableModel* model = new QSqlTableModel(ui->cboFormaPago,QSqlDatabase::database("empresa"));
    model->setTable("formpago");
    model->select();
    ui->cboFormaPago->setModel(model);
    ui->cboFormaPago->setModelColumn(model->fieldIndex("cFormapago"));

    oPres = new Presupuesto();
    oClientePres = new Cliente();

    helper.set_Tipo(false);
    helper.help_table(ui->tableWidget);

    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));

    connect(&helper,SIGNAL(totalChanged(double,double,double,double,double,double,QString)),
            this,SLOT(totalChanged(double,double,double,double,double,double,QString)));

    connect(&helper,SIGNAL(desglose1Changed(double,double,double,double)),
            this,SLOT(desglose1Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose2Changed(double,double,double,double)),
            this,SLOT(desglose2Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose3Changed(double,double,double,double)),
            this,SLOT(desglose3Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose4Changed(double,double,double,double)),
            this,SLOT(desglose4Changed(double,double,double,double)));

    connect(&helper,SIGNAL(lineaReady(lineaDetalle*)),this,SLOT(lineaReady(lineaDetalle*)));
    connect(&helper,SIGNAL(lineaDeleted(lineaDetalle*)),this,SLOT(lineaDeleted(lineaDetalle*)));

    connect(ui->chklRecargoEq,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

    helper.set_tarifa(Configuracion_global->id_tarifa_predeterminada);

    aPedido_action = new QAction(tr("En pedido"),this);
    aAlbaran_action = new QAction(tr("En albaran"),this);
    aFactura_action = new QAction(tr("En factura"),this);

    connect(aPedido_action,SIGNAL(triggered()),this,SLOT(convertir_enPedido()));
    connect(aAlbaran_action,SIGNAL(triggered()),this,SLOT(convertir_enAlbaran()));
    connect(aFactura_action,SIGNAL(triggered()),this,SLOT(convertir_enFactura()));

    convertir_menu = new QMenu(this);
    convertir_menu->addAction(aPedido_action);
    convertir_menu->addAction(aAlbaran_action);
    convertir_menu->addAction(aFactura_action);

    ui->btn_convertir->setMenu(convertir_menu);

    ui->txtnPorcentajeIva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtnPorcentajeIva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtnPorcentajeIva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtnPorcentajeIva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtnRec1->setText(Configuracion_global->reList.at(0));
    ui->txtnRec2->setText(Configuracion_global->reList.at(1));
    ui->txtnRec3->setText(Configuracion_global->reList.at(2));
    ui->txtnRec4->setText(Configuracion_global->reList.at(3));
    BloquearCampos(true);
    if(oPres->siguiente())
    {
        LLenarCampos();
        QString filter = QString("Id_Cab = '%1'").arg(oPres->id);
        helper.fillTable("empresa","lin_pre",filter);
        ui->btnAnterior->setEnabled(false);
    }
    else
    {
        ui->btnBorrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btn_convertir->setEnabled(false);
        ui->btnAnterior->setEnabled(false);
        ui->btnSiguiente->setEnabled(false);
        ui->btnBuscar->setEnabled(false);
        ui->btnImprimir->setEnabled(false);
    }
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
    ui->lblTopNpres->setText(QString::number(oPres->nPresupuesto));
    ui->lblTopCCliente->setText(oPres->cCliente);
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

    QList<QString> keys = Configuracion_global->paises.uniqueKeys();
    for (int i = 0;i<keys.size();i++)
    {
        if(Configuracion_global->paises[keys.at(i)].value("id").toInt() == oPres->idPais)
        {
            int index = ui->combo_Pais->findText(keys.at(i));
            ui->combo_Pais->setCurrentIndex(index);
        }
    }
    ui->txtcTelefono->setText(oPres->cTelefono);
    ui->txtcMovil->setText(oPres->cMovil);
    ui->txtcFax->setText(oPres->cFax);
    ui->txttComentario->setPlainText(oPres->tComentarios);
    ui->txtrBase->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rImporte,'f',2)));
    ui->txtrSubtotal->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase,'f',2)));
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
    if (!(nIndex ==-1))
        ui->cboFormaPago->setCurrentIndex(nIndex);

    ui->txttLugarEntrega->setPlainText(oPres->tLugarEntrega);
    ui->txtcAtencionde->setText(oPres->cAtencionde);
    ui->txtrBase1->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase1,'f',2)));
    ui->txtrBase2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase2,'f',2)));
    ui->txtrBase3->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase3,'f',2)));
    ui->txtrBase4->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rBase4,'f',2)));
    ui->txtrBaseTotal->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rImporte,'f',2)));

    //ui->txtnPorcentajeIva1->setText(QString::number(oPres->nIva1));
    //ui->txtnPorcentajeIva2->setText(QString::number(oPres->nIva2));
    //ui->txtnPorcentajeIva3->setText(QString::number(oPres->nIva3));
    //ui->txtnPorcentajeIva4->setText(QString::number(oPres->nIva4));
    ui->txtrIVA1->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva1,'f',2)));
    ui->txtrIVA2->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva2,'f',2)));
    ui->txtrIVA3->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva3,'f',2)));
    ui->txtrIVA4->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rIva4,'f',2)));
    ui->txtrTotalIVA->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotalIva,'f',2)));
    ui->txtrImporteIva->setText(Configuracion_global->FormatoNumerico(QString::number(oPres->rTotalIva,'f',2)));
    //ui->txtnRec1->setText(QString::number(oPres->nRecargoEquivalencia1));
    //ui->txtnRec2->setText(QString::number(oPres->nRecargoEquivalencia2));
    //ui->txtnRec3->setText(QString::number(oPres->nRecargoEquivalencia3));
    //ui->txtnRec4->setText(QString::number(oPres->nRecargoEquivalencia4));
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
    ui->txtcCodigoCliente->setText(oClientePres->cCodigoCliente);
    ui->txtcCliente->setText(oClientePres->cNombreFiscal);
    ui->lblTopCCliente->setText(oClientePres->cNombreFiscal);
    ui->txtcDireccion->setText(oClientePres->cDireccion1);
    ui->txtcDireccion2->setText(oClientePres->cDireccion2);
    ui->txtcCp->setText(oClientePres->cCp);
    ui->txtcPoblacion->setText(oClientePres->cPoblacion);
    ui->txtcProvincia->setText(oClientePres->cProvincia);

    QList<QString> keys = Configuracion_global->paises.uniqueKeys();
    for (int i = 0;i<keys.size();i++)
    {
        if(Configuracion_global->paises[keys.at(i)].value("id").toInt() == oPres->idPais)
        {
            int index = ui->combo_Pais->findText(keys.at(i));
            ui->combo_Pais->setCurrentIndex(index);
        }
    }
    ui->txtcCif->setText(oClientePres->cCifNif);
    ui->txtcTelefono->setText(oClientePres->cTelefono1);
    ui->txtcFax->setText(oClientePres->cFax);
    ui->txtcMovil->setText(oClientePres->cMovil);
    ui->txtcEmail->setText(oClientePres->cEmail);

    if (oClientePres->lIRPF==1) {
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

    oPres->idPais = Configuracion_global->paises[ui->combo_Pais->currentText()].value("id").toInt();

    oPres->cTelefono = (ui->txtcTelefono->text());
    oPres->cMovil = (ui->txtcMovil->text());
    oPres->cFax = (ui->txtcFax->text());
    oPres->tComentarios = (ui->txttComentario->toPlainText());

    oPres->rImporte = ui->txtrBase->text().replace(moneda,"") .toDouble();
    oPres->rBase = (ui->txtrSubtotal->text().replace(moneda,"") .toDouble());
    oPres->rDescuento = (ui->txtrDescuento->text().replace(moneda,"") .toDouble());
    oPres->rTotal = (ui->txtrTotal->text().replace(moneda,"") .toDouble());

    oPres->lAprobado = ui->chklAprovado->isChecked();

    oPres->dFechaAprobacion = (ui->txtdFechaAprovacion->date());
    oPres->rImporteFactura = (ui->txtrImporteFactura->text().replace(moneda,"") .toDouble());

    if(ui->txtcFactura->text()=="")
        oPres->cFactura = "0";
    else
        oPres->cFactura = (ui->txtcFactura->text());

    oPres->nAlbaran = (ui->txtnAlbaran->text().toInt());
    oPres->nPedido = (ui->txtnPedido->text().toInt());

    QSqlTableModel*  modeloPago = static_cast<QSqlTableModel*>(ui->cboFormaPago->model());
    oPres->cDescripcionFormaPago = ui->cboFormaPago->currentText();
    oPres->cCodigoFormaPago = modeloPago->record(ui->combo_Pais->currentIndex()).value("cCodigo").toString();
    oPres->id_FormaPago = modeloPago->record(ui->combo_Pais->currentIndex()).value("id").toInt();

    oPres->tLugarEntrega = (ui->txttLugarEntrega->toPlainText());
    oPres->cAtencionde = (ui->txtcAtencionde->text());
    oPres->rBase1 = (ui->txtrBase1->text().replace(moneda,"") .toDouble());
    oPres->rBase2 = (ui->txtrBase2->text().replace(moneda,"") .toDouble());
    oPres->rBase3 = (ui->txtrBase3->text().replace(moneda,"") .toDouble());
    oPres->rBase4 = (ui->txtrBase4->text().replace(moneda,"") .toDouble());
    //oPres->nIva1 = (ui->txtnPorcentajeIva1->text().toDouble());
    //oPres->nIva2 = (ui->txtnPorcentajeIva2->text().toDouble());
    //oPres->nIva3 = (ui->txtnPorcentajeIva3->text().toDouble());
    //oPres->nIva4 = (ui->txtnPorcentajeIva4->text().toDouble());
    oPres->rIva1 = (ui->txtrIVA1->text().replace(moneda,"") .toDouble());
    oPres->rIva2 = (ui->txtrIVA2->text().replace(moneda,"") .toDouble());
    oPres->rIva3 = (ui->txtrIVA3->text().replace(moneda,"") .toDouble());
    oPres->rIva4 = (ui->txtrIVA4->text().replace(moneda,"") .toDouble());
    //oPres->nRecargoEquivalencia1 = (ui->txtnRec1->text().toDouble());
    //oPres->nRecargoEquivalencia2 = (ui->txtnRec2->text().toDouble());
    //oPres->nRecargoEquivalencia3 = (ui->txtnRec3->text().toDouble());
    //oPres->nRecargoEquivalencia4 = (ui->txtnRec4->text().toDouble());
    oPres->lRecargoEquivalencia = ui->chklRecargoEq->isChecked();
    oPres->rRec1 = (ui->txtrRecargoEq1->text().replace(moneda,"") .toDouble());
    oPres->rRec2 = (ui->txtrRecargoEq2->text().replace(moneda,"") .toDouble());
    oPres->rRec3 = (ui->txtrRecargoEq3->text().replace(moneda,"") .toDouble());
    oPres->rRec4 = (ui->txtrRecargoEq4->text().replace(moneda,"") .toDouble());
    oPres->rTotal1 = (ui->txtrTotal1->text().replace(moneda,"") .toDouble());
    oPres->rTotal2 = (ui->txtrTotal2->text().replace(moneda,"") .toDouble());
    oPres->rTotal3 = (ui->txtrTotal3->text().replace(moneda,"") .toDouble());
    oPres->rTotal4 = (ui->txtrTotal4->text().replace(moneda,"") .toDouble());
    oPres->rTotalIva = ui->txtrTotalIVA->text().replace(moneda,"") .toDouble();
    oPres->rTotalRec = ui->txtrTotalRecargoEq->text().replace(moneda,"") .toDouble();
    oPres->cEmail = (ui->txtcEmail->text());
}
void FrmPresupuestosCli::VaciarCampos()
{
    ui->lblTopNpres->setText("");
    ui->lblTopCCliente->setText("");
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
    //ui->txtcPais->setText("");
    ui->txtcTelefono->setText("");
    ui->txtcMovil->setText("");
    ui->txtcFax->setText("");
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
   // ui->txtnPorcentajeIva1->setText("0");
   // ui->txtnPorcentajeIva2->setText("0");
   // ui->txtnPorcentajeIva3->setText("0");
   // ui->txtnPorcentajeIva4->setText("0");
    ui->txtrIVA1->setText("0,00");
    ui->txtrIVA2->setText("0,00");
    ui->txtrIVA3->setText("0,00");
    ui->txtrIVA4->setText("0,00");
    //ui->txtnRec1->setText("0");
    //ui->txtnRec2->setText("0");
    //ui->txtnRec3->setText("0");
    //ui->txtnRec4->setText("0");
    ui->txtrRecargoEq1->setText("0,00");
    ui->txtrRecargoEq2->setText("0,00");
    ui->txtrRecargoEq3->setText("0,00");
    ui->txtrRecargoEq4->setText("0,00");
    ui->txtrTotal1->setText("0,00");
    ui->txtrTotal2->setText("0,00");
    ui->txtrTotal3->setText("0,00");
    ui->txtrTotal4->setText("0,00");
    ui->txtcEmail->setText("");
    ui->txtcCliente->setText("");

    helper.fillTable("empresa","lin_pre","Id_Cab = -1");
}

void FrmPresupuestosCli::BloquearCampos(bool state)
{
    helper.blockTable(state);
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(state);
    }

    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(!state);
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!state);
    }
    // QTextEdit
    QList<QTextEdit *> textEditList = this->findChildren<QTextEdit *>();
    QTextEdit *textEdit;
    foreach (textEdit,textEditList) {
        textEdit->setReadOnly(state);
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!state);
    }

    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    ui->btnBorrar->setEnabled(state);
    ui->btnBuscar->setEnabled(state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->btnAnadirLinea->setEnabled(!state);
    ui->btn_borrarLinea->setEnabled(!state);
    ui->botBuscarCliente->setEnabled(!state);
    ui->txtnPresupuesto->setReadOnly(true);
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
    if(oPres->siguiente())
    {
        LLenarCampos();
        QString filter = QString("Id_Cab = '%1'").arg(oPres->id);
        helper.fillTable("empresa","lin_pre",filter);
        ui->btnBorrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
        ui->btn_convertir->setEnabled(true);
        ui->btnAnterior->setEnabled(true);
    }
    else
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Se ha llegado al final del archivo.\n"        
                                                          "No hay mas presupuestos disponibles"));
        VaciarCampos();
        ui->btnBorrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btn_convertir->setEnabled(false);
        oPres->id++;
        ui->btnSiguiente->setEnabled(false);
    }
}

void FrmPresupuestosCli::on_btnAnterior_clicked()
{
    if(oPres->anterior())
    {
        LLenarCampos();
        QString filter = QString("Id_Cab = '%1'").arg(oPres->id);
        helper.fillTable("empresa","lin_pre",filter);
        ui->btnBorrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
        ui->btn_convertir->setEnabled(true);
        ui->btnSiguiente->setEnabled(true);
    }
    else
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Se ha llegado al principio del archivo.\n"
                                                          "No hay mas presupuestos disponibles"));
        VaciarCampos();
        ui->btnBorrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btn_convertir->setEnabled(false);
        oPres->id = -1;
        ui->btnAnterior->setEnabled(false);
    }
}

void FrmPresupuestosCli::on_btnAnadir_clicked()
{
    VaciarCampos();
    QString filter = QString("Id_Cab = '%1'").arg(ui->txtnPresupuesto->text());
    helper.fillTable("empresa","lin_pre",filter);
    helper.set_tarifa(Configuracion_global->id_tarifa_predeterminada);
    BloquearCampos(false);
    int next = oPres->NuevoNumeroPresupuesto();
    ui->txtnPresupuesto->setText(QString::number(next));
    ui->lblTopNpres->setText(QString::number(next));
    LLenarPresupuesto();
    oPres->AnadirPresupuesto();
    ui->txtcCodigoCliente->setFocus();
    editando = false;
    emit block();
}

void FrmPresupuestosCli::on_btnEditar_clicked()
{
    if (oPres->cFactura=="0")
    {
        BloquearCampos(false);
        editando = true;
        emit block();
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Editar Presupuesto"),tr("No se puede editar un Presupuesto que ha sido facturado, solo los NO facturados se pueden editar")+
                             tr("<p><b> Si necesita modificar algo genere una factura nueva y realice el abono correspondiente</b>")+
                                tr("y luego si es preciso realice un nuevo presupuesto y facture de nuevo "),tr("OK"));
    }
}

void FrmPresupuestosCli::on_btnGuardar_clicked()
{
    if(ui->tableWidget->rowCount() == 0)
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Está intentando guardar un presupuesto vacio.\n"
                                         "Por favor, añada alguna linea al presupuesto."));
        return;
    }
    LLenarPresupuesto();
    QSqlDatabase::database("empresa").transaction();
    bool succes = true;

    succes &= oPres->GuardarPres(oPres->id);

    if(succes)
    {
        LLenarCampos();
        succes = QSqlDatabase::database("empresa").commit();
    }

    if(succes)
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Guardado con éxito"));
        BloquearCampos(true);
        emit unblock();
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),
                              tr("Error al guardar el presupuesto.\n")+QSqlDatabase::database("empresa").lastError().text(),
                              tr("&Aceptar"));
        QSqlDatabase::database("empresa").rollback();
    }
}

void FrmPresupuestosCli::on_btnBuscar_clicked()
{

}

void FrmPresupuestosCli::on_botBuscarCliente_clicked()
{    
    Db_table_View searcher(qApp->activeWindow());
    searcher.set_db("Maya");
    searcher.set_table("clientes");

    searcher.setWindowTitle(tr("Clientes"));

    QStringList headers;
    headers << tr("Codigo")<< tr("Nombre Fiscal") << tr("DNI/NIF") << tr("Poblacion");
    searcher.set_table_headers(headers);

    searcher.set_readOnly(true);
    searcher.set_selection("Id");

    searcher.set_columnHide(0);
    searcher.set_columnHide(2);
    searcher.set_columnHide(3);
    searcher.set_columnHide(4);
    searcher.set_columnHide(6);
    searcher.set_columnHide(7);
    searcher.set_columnHide(9);
    searcher.set_columnHide(10);
    searcher.set_columnHide(11);
    for(int i =13;i<55;i++)
        searcher.set_columnHide(i);
    if(searcher.exec() == QDialog::Accepted)
    {
        QString cId =searcher.selected_value;
        oPres->id_cliente = cId.toInt();
        oClientePres->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
        helper.set_tarifa(oClientePres->idTarifa);
        LLenarCamposCliente();
    }
}

void FrmPresupuestosCli::totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda)
{
    this->moneda = moneda;
    ui->txtrBase->setText(QString::number(base)+moneda);
    ui->txtrDescuento->setText(QString::number(dto)+moneda);
    ui->txtrSubtotal->setText(QString::number(subTotal)+moneda);
    ui->txtrImporteIva->setText(QString::number(iva)+moneda);
    ui->txtrTotalRecargoEq->setText(QString::number(re)+moneda);
    ui->txtrTotal->setText(QString::number(total)+moneda);

    ui->txtrBaseTotal->setText(QString::number(subTotal)+moneda);
    ui->txtrTotalIVA->setText(QString::number(iva)+moneda);
    ui->txtrTotalRecargoEq_2->setText(QString::number(re)+moneda);
    ui->txtrTotal_2->setText(QString::number(total)+moneda);
}

void FrmPresupuestosCli::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtrBase1->setText(QString::number(base));
    ui->txtrIVA1->setText(QString::number(iva));
    ui->txtrRecargoEq1->setText(QString::number(re));
    ui->txtrTotal1->setText(QString::number(total));
}

void FrmPresupuestosCli::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtrBase2->setText(QString::number(base));
    ui->txtrIVA2->setText(QString::number(iva));
    ui->txtrRecargoEq2->setText(QString::number(re));
    ui->txtrTotal2->setText(QString::number(total));
}

void FrmPresupuestosCli::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtrBase3->setText(QString::number(base));
    ui->txtrIVA3->setText(QString::number(iva));
    ui->txtrRecargoEq3->setText(QString::number(re));
    ui->txtrTotal3->setText(QString::number(total));
}

void FrmPresupuestosCli::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtrBase4->setText(QString::number(base));
    ui->txtrIVA4->setText(QString::number(iva));
    ui->txtrRecargoEq4->setText(QString::number(re));
    ui->txtrTotal4->setText(QString::number(total));
}

void FrmPresupuestosCli::on_btnDeshacer_clicked()
{
    VaciarCampos();
    LLenarCampos();
    BloquearCampos(true);
    QString filter = QString("Id_Cab = '%1'").arg(ui->txtnPresupuesto->text());
    helper.fillTable("empresa","lin_pre",filter);
    emit unblock();
}

void FrmPresupuestosCli::convertir_enPedido()
{
    //TODO FrmPresupuestosCli::convertir_enPresupuesto()
}

void FrmPresupuestosCli::convertir_enAlbaran()
{
    //TODO FrmPresupuestosCli::convertir_enAlbaran()
}

void FrmPresupuestosCli::convertir_enFactura()
{
    //TODO FrmPresupuestosCli::convertir_enFactura()
}

void FrmPresupuestosCli::on_btnBorrar_clicked()
{
    if (QMessageBox::question(this,tr("Borrar"),
                              tr("Esta acción no se puede deshacer.\n¿Desea continuar?"),
                              tr("Cancelar"),
                               tr("&Continuar"))== QMessageBox::Accepted)
    {
        bool succes = true;
        QSqlDatabase::database("empresa").transaction();

        QSqlQuery q(QSqlDatabase::database("empresa"));
        succes &= oPres->BorrarLineas(oPres->id);
        q.prepare("DELETE FROM cab_pre WHERE nPresupuesto = "+ui->txtnPresupuesto->text());
        succes &= q.exec();        

        if(succes)
            succes &= QSqlDatabase::database("empresa").commit();

        if(succes)
        {
            TimedMessageBox * t = new TimedMessageBox(this,tr("Borrado con exito"));
            VaciarCampos();
            oPres->id = -1;
            on_btnSiguiente_clicked();
        }
        else
            QSqlDatabase::database("empresa").rollback();
    }
}

void FrmPresupuestosCli::lineaReady(lineaDetalle * ld)
{
    //-----------------------------------------------------
    // Insertamos línea de pedido y controlamos acumulados
    //-----------------------------------------------------

    bool ok_empresa,ok_Maya;
    ok_empresa = true;
    ok_Maya = true;
    QSqlDatabase::database("empresa").transaction();
    QSqlDatabase::database("Maya").transaction();

    //Nueva linea
    if (ld->idLinea == -1)
    {
        //qDebug()<< ld->idLinea;
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("select id from articulos where cCodigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;
        QSqlQuery query_lin_ped_pro(QSqlDatabase::database("empresa"));
        query_lin_ped_pro.prepare("INSERT INTO lin_pre (Id_Cab, id_Articulo, cCodigo, nCantidad,"
                                  "cDescripcion, rPvp, rSubTotal, rDto,"
                                  " nPorcDto, nPorcIva, rTotal)"
                                  "VALUES (:id_cab,:id_articulo,:cCodigo,:cantidad,"
                                  ":descripcion,:coste_bruto,:subtotal_coste,:porc_dto,"
                                  ":dto,:porc_iva,:total);");
        query_lin_ped_pro.bindValue(":id_cab", oPres->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":cCodigo",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_ped_pro.bindValue(":subtotal_coste",ld->subTotal);
        query_lin_ped_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_ped_pro.bindValue(":dto",ld->dto);
        query_lin_ped_pro.bindValue(":iva",0); // importe iva hay que calcularlo
        query_lin_ped_pro.bindValue(":total",ld->total);
        if (!query_lin_ped_pro.exec()){
            ok_empresa = false;
            QMessageBox::warning(this,tr("Gestión de albaranes"),
                                 tr("Ocurrió un error al insertar la nueva línea: %1").arg(query_lin_ped_pro.lastError().text()),
                                 tr("Aceptar"));
        }
        //TODO control de stock pres clientes

        if(queryArticulos.exec() && ok_empresa){
            QSqlDatabase::database("empresa").commit();
            QSqlDatabase::database("Maya").commit();
        } else
        {
            QSqlDatabase::database("empresa").rollback();
            QSqlDatabase::database("Maya").rollback();
        }

        ld->idLinea = query_lin_ped_pro.lastInsertId().toInt();

    } else // Editando linea
    {
        //TODO control de stock editando en pres clientes
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("select id from articulos where cCodigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery query_lin_ped_pro(QSqlDatabase::database("empresa"));
        query_lin_ped_pro.prepare("UPDATE lin_pre SET "
                                  "id_Articulo =:id_articulo,"
                                  "cCodigo =:codigo_articulo_proveedor,"
                                  "cDescripcion =:descripcion,"
                                  "nCantidad =:cantidad,"
                                  "rPvp =:coste_bruto,"
                                  "rSubTotal =:subtotal_coste,"
                                  "rDto =:porc_dto,"
                                  "nPorcDto =:dto,"
                                  "nPorcIva =:porc_iva,"
                                  "rTotal =:total "
                                  "WHERE id = :id;");

        query_lin_ped_pro.bindValue(":id_cab", oPres->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_ped_pro.bindValue(":subtotal_coste",ld->subTotal);
        query_lin_ped_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_ped_pro.bindValue(":dto",ld->dto);
        query_lin_ped_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_ped_pro.bindValue(":total",ld->total);
        query_lin_ped_pro.bindValue(":id",ld->idLinea);

        if (!query_lin_ped_pro.exec()) {
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Ocurrió un error al guardar la línea: %1").arg(query_lin_ped_pro.lastError().text()),
                                 tr("Aceptar"));
            ok_empresa = false;
        }
        //TODO control stock editando linea alb
        if(queryArticulos.exec() && ok_empresa && ok_Maya){
            QSqlDatabase::database("empresa").commit();
            QSqlDatabase::database("Maya").commit();
        } else
        {
            QSqlDatabase::database("empresa").rollback();
            QSqlDatabase::database("Maya").rollback();
        }
    }
    ld->cantidad_old = ld->cantidad;
}

void FrmPresupuestosCli::lineaDeleted(lineaDetalle * ld)
{
    //todo borrar de la bd y stock y demas
    //if id = -1 pasando olimpicamente
    bool ok_empresa,ok_Maya;
    ok_empresa = true;
    ok_Maya = true;
    QSqlDatabase::database("empresa").transaction();
    QSqlDatabase::database("Maya").transaction();
    if(ld->idLinea >-1)
    {
        //TODO control de stock
        QSqlQuery q(QSqlDatabase::database("empresa"));
        q.prepare("delete from lin_pre where id =:id");
        q.bindValue(":id",ld->idLinea);
        if(q.exec() && ok_Maya)
        {
            QSqlDatabase::database("empresa").commit();
            QSqlDatabase::database("Maya").commit();
        } else
        {
            QSqlDatabase::database("empresa").rollback();
            QSqlDatabase::database("Maya").rollback();
        }
    }
    delete ld;
}

void FrmPresupuestosCli::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);
    helper.resizeTable();
}
