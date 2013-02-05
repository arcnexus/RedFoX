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

    QSqlTableModel* modelo = new QSqlTableModel(ui->combo_Pais,QSqlDatabase::database("empresa"));
    modelo->setTable("paises");
    modelo->select();
    ui->combo_Pais->setModel(modelo);
    ui->combo_Pais->setModelColumn(modelo->fieldIndex("pais"));

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

    Db_table_View* searcher = new Db_table_View(this);
    searcher->set_db("empresa");
    searcher->set_table("articulos");
    searcher->set_selection("cCodigo");
    searcher->setWindowTitle(tr("Articulos"));
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

    connect(ui->chklRecargoEq,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

    if(oPres->siguiente())
    {
        LLenarCampos();
        QString filter = QString("Id_Cab = '%1'").arg(ui->txtnPresupuesto->text());
        helper.fillTable("empresa","lin_pre",filter);
    }

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
    BloquearCampos(true);
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
    //FIXME pais
    //ui->txtcPais->setText(QString::number(oPres->idPais));
    ui->txtcTelefono->setText(oPres->cTelefono);
    ui->txtcMovil->setText(oPres->cMovil);
    ui->txtcFax->setText(oPres->cFax);
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
    if (!(nIndex ==-1))
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
    //FIXME pais
    //ui->txtcPais->setText(oClientePres->getcPais());
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

    QSqlTableModel*  modelo = static_cast<QSqlTableModel*>(ui->combo_Pais->model());
    oPres->idPais = modelo->record(ui->combo_Pais->currentIndex()).value("id").toInt();

    oPres->cTelefono = (ui->txtcTelefono->text());
    oPres->cMovil = (ui->txtcMovil->text());
    oPres->cFax = (ui->txtcFax->text());
    oPres->tComentarios = (ui->txttComentario->toPlainText());
    oPres->rImporte = (ui->txtrBase->text().replace(".","").toDouble());
    oPres->rSubTotal = (ui->txtrSubtotal->text().replace(".","").toDouble());
    oPres->rDescuento = (ui->txtrDescuento->text().replace(".","").toDouble());
    oPres->rTotal = (ui->txtrTotal->text().replace(".","").toDouble());

    oPres->lAprobado = ui->chklAprovado->isChecked();

    oPres->dFechaAprobacion = (ui->txtdFechaAprovacion->date());
    oPres->rImporteFactura = (ui->txtrImporteFactura->text().replace(".","").toDouble());

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
    oPres->lRecargoEquivalencia = ui->chklRecargoEq->isChecked();
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
    ui->btnDeshacer->setEnabled(!state);
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
        QString filter = QString("Id_Cab = '%1'").arg(ui->txtnPresupuesto->text());
        helper.fillTable("empresa","lin_pre",filter);
    }
    else
        QMessageBox::information(this,tr("Final del archivo"),
                                 tr("Se ha llegado al final del archivo.\nNo hay mas presupuestos disponibles"),
                                 tr("&Aceptar"));
}

void FrmPresupuestosCli::on_btnAnterior_clicked()
{
    if(oPres->anterior())
    {
        LLenarCampos();
        QString filter = QString("Id_Cab = '%1'").arg(ui->txtnPresupuesto->text());
        helper.fillTable("empresa","lin_pre",filter);
    }
    else
        QMessageBox::information(this,tr("Inicio de archivo"),
                                 tr("Se ha llegado al principio del archivo.\nNo hay mas presupuestos disponibles"),
                                 tr("&Aceptar"));
}

void FrmPresupuestosCli::on_btnAnadir_clicked()
{
    VaciarCampos();
    QString filter = QString("Id_Cab = '%1'").arg(ui->txtnPresupuesto->text());
    helper.fillTable("empresa","lin_pre",filter);
    BloquearCampos(false);
    int next = oPres->NuevoNumeroPresupuesto();
    ui->txtnPresupuesto->setText(QString::number(next));
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
        QMessageBox::information(this,tr("Presupuesto vacio"),
                                 tr("Está intentando guardar un presupuesto vacio.\nPor favor, añada alguna linea al presupuesto."),
                                 tr("&Aceptar"));
        return;
    }
    LLenarPresupuesto();
    QSqlDatabase::database("empresa").transaction();
    bool succes = true;
    if(editando)
    {
        int iPre = ui->txtnPresupuesto->text().toInt();
        //actualizar cabezera
        succes &= oPres->GuardarPres(iPre);
        //Borrar lineas
        succes &= oPres->BorrarLineas(iPre);
        //guardar lineas
        succes &= helper.saveTable(iPre,"empresa","lin_pre");
        if(succes)
        {
            LLenarCampos();
            succes = QSqlDatabase::database("empresa").commit();
        }
    }
    else
    {
        succes &= oPres->AnadirPresupuesto();
        succes &= helper.saveTable(oPres->nPresupuesto,"empresa","lin_pre");
        if(succes)
        {
            LLenarCampos();
            succes = QSqlDatabase::database("empresa").commit();
        }
    }
    if(succes)
    {
        QMessageBox::information(this,tr("Guardado"),tr("Guardado con éxito"),tr("&Aceptar"));
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
    FrmBuscarCliente BuscarClientes(this);
    BuscarClientes.exec();
    int nId = BuscarClientes.DevolverID();
    QString cId = QString::number(nId);
    oPres->id_cliente = cId.toInt();
    oClientePres->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
    LLenarCamposCliente();
}

void FrmPresupuestosCli::totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda)
{
    ui->txtrBase->setText(QString::number(base)+moneda);
    ui->txtrDescuento->setText(QString::number(dto)+moneda);
    ui->txtrSubtotal->setText(QString::number(subTotal)+moneda);
    ui->txtrImporteIva->setText(QString::number(iva)+moneda);
    ui->txtrTotalRecargoEq->setText(QString::number(re)+moneda);
    ui->txtrTotal->setText(QString::number(total)+moneda);
    ui->lbl_total->setText(QString::number(total)+moneda);

    ui->txtrBaseTotal->setText(QString::number(base)+moneda);
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
        q.prepare("DELETE FROM cab_pre WHERE nPresupuesto = "+ui->txtnPresupuesto->text());
        succes &= q.exec();
        succes &= oPres->BorrarLineas(ui->txtnPresupuesto->text().toInt());

        if(succes)
            succes &= QSqlDatabase::database("empresa").commit();

        if(succes)
            QMessageBox::information(this,tr("Borrado"),tr("Borrado con exito"));
        else
            QSqlDatabase::database("empresa").rollback();
    }
}
