#include "frmpresupuestoscli.h"
#include "ui_frmpresupuestoscli.h"
#include <QSqlQueryModel>

#include "../Busquedas/frmbuscarcliente.h"
#include "../Almacen/articulo.h"
#include <QMessageBox>


FrmPresupuestosCli::FrmPresupuestosCli(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmPresupuestosCli),
    helper(this),
    toolButton(tr("Presupuestos"),":/Icons/PNG/presupuestos.png",this),
    menuButton(QIcon(":/Icons/PNG/presupuestos.png"),tr("Presupuestos"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/presupuestos.png"),"",this))

{
    ui->setupUi(this);

    ui->combo_Pais->setModel(Configuracion_global->paises_model);
    ui->combo_Pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de presupuestos a clientes"));

    // cargar datos FormaPago
    ui->cboFormaPago->setInsertPolicy(QComboBox::NoInsert);
    ui->cboFormaPago->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    QSqlTableModel* model = new QSqlTableModel(ui->cboFormaPago,QSqlDatabase::database("empresa"));
    model->setTable("formpago");
    model->select();
    ui->cboFormaPago->setModel(model);
    ui->cboFormaPago->setModelColumn(model->fieldIndex("forma_pago"));

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

    connect(ui->chklporc_rec,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

    helper.set_tarifa(Configuracion_global->id_tarifa_predeterminada);

    aPedido_action = new QAction(tr("En pedido"),this);
    aAlbaran_action = new QAction(tr("En albaran"),this);
    aFactura_action = new QAction(tr("En factura"),this);

    connect(aPedido_action,SIGNAL(triggered()),this,SLOT(convertir_epedido()));
    connect(aAlbaran_action,SIGNAL(triggered()),this,SLOT(convertir_ealbaran()));
    connect(aFactura_action,SIGNAL(triggered()),this,SLOT(convertir_enFactura()));

    convertir_menu = new QMenu(this);
    convertir_menu->addAction(aPedido_action);
    convertir_menu->addAction(aAlbaran_action);
    convertir_menu->addAction(aFactura_action);

    ui->btn_convertir->setMenu(convertir_menu);

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));
    BloquearCampos(true);
    if(oPres->siguiente())
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oPres->id);
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
    ui->lblTopcliente->setText(oPres->cliente);
    ui->txtfecha->setDate(oPres->fecha);
    ui->txtdValidoHasta->setDate(oPres->dValidoHasta);
    ui->txtcodigo_cliente->setText(oPres->codigo_cliente);
    ui->txtcliente->setText(oPres->cliente);    
    ui->txtcif->setText(oPres->cif);
    ui->txtdireccion1->setText(oPres->direccion1);
    ui->txtdireccion2->setText(oPres->direccion2);
    ui->txtcp->setText(oPres->cp);
    ui->txtpoblacion->setText(oPres->poblacion);
    ui->txtprovincia->setText(oPres->provincia);

    QList<QString> keys = Configuracion_global->paises.uniqueKeys();
    for (int i = 0;i<keys.size();i++)
    {
        if(Configuracion_global->paises[keys.at(i)].value("id").toInt() == oPres->id_pais)
        {
            int index = ui->combo_Pais->findText(keys.at(i));
            ui->combo_Pais->setCurrentIndex(index);
        }
    }
    ui->txttelefono->setText(oPres->telefono);
    ui->txtmovil->setText(oPres->movil);
    ui->txtfax->setText(oPres->fax);
    ui->txtcomentario->setPlainText(oPres->comentarios);
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->importe,'f',2)));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base,'f',2)));
    ui->txtrDescuento->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rDescuento,'f',2)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total,'f',2)));
    ui->lbimpreso->setVisible(oPres->impreso);
    ui->chklAprovado->setChecked(oPres->lAprobado);
    ui->txtfechaAprovacion->setDate(oPres->fechaAprobacion);
    ui->txtimporteFactura->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->importeFactura,'f',2)));
    ui->txtfactura->setText(oPres->factura);
    ui->txtalbaran->setText(QString::number(oPres->albaran));
    ui->txtpedido->setText(QString::number(oPres->pedido));

    int nIndex =  ui->cboFormaPago->findText(oPres->descripcionFormaPago);
    if (!(nIndex ==-1))
        ui->cboFormaPago->setCurrentIndex(nIndex);

    ui->txttLugarEntrega->setPlainText(oPres->tLugarEntrega);
    ui->txtcAtencionde->setText(oPres->cAtencionde);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base1,'f',2)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base2,'f',2)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base3,'f',2)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base4,'f',2)));
    ui->txtbase_total->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->importe,'f',2)));

    //ui->txtporc_iva1->setText(QString::number(oPres->iva1));
    //ui->txtporc_iva2->setText(QString::number(oPres->iva2));
    //ui->txtporc_iva3->setText(QString::number(oPres->iva3));
    //ui->txtporc_iva4->setText(QString::number(oPres->iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva1,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva2,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva3,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva4,'f',2)));
    ui->txttotal_iva->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_iva,'f',2)));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_iva,'f',2)));
    //ui->txtporc_rec1->setText(QString::number(oPres->porc_porc_recuivalencia1));
    //ui->txtporc_rec2->setText(QString::number(oPres->porc_porc_recuivalencia2));
    //ui->txtporc_rec3->setText(QString::number(oPres->porc_porc_recuivalencia3));
    //ui->txtporc_rec4->setText(QString::number(oPres->porc_porc_recuivalencia4));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->totalRec,'f',2)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->totalRec,'f',2)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec1,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec2,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec3,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec4,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total1,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total2,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total3,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total4,'f',2)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total,'f',2)));
    ui->txtemail->setText(oPres->email);
    ui->chklporc_rec->setChecked(oPres->recargo_equivalencia);
}

void FrmPresupuestosCli::LLenarCamposCliente()
{
    ui->txtcodigo_cliente->setText(oClientePres->codigo_cliente);
    ui->txtcliente->setText(oClientePres->nombre_fiscal);
    ui->lblTopcliente->setText(oClientePres->nombre_fiscal);
    ui->txtdireccion1->setText(oClientePres->direccion1);
    ui->txtdireccion2->setText(oClientePres->direccion2);
    ui->txtcp->setText(oClientePres->cp);
    ui->txtpoblacion->setText(oClientePres->poblacion);
    ui->txtprovincia->setText(oClientePres->provincia);

    QList<QString> keys = Configuracion_global->paises.uniqueKeys();
    for (int i = 0;i<keys.size();i++)
    {
        if(Configuracion_global->paises[keys.at(i)].value("id").toInt() == oPres->id_pais)
        {
            int index = ui->combo_Pais->findText(keys.at(i));
            ui->combo_Pais->setCurrentIndex(index);
        }
    }
    ui->txtcif->setText(oClientePres->cif_nif);
    ui->txttelefono->setText(oClientePres->telefono1);
    ui->txtfax->setText(oClientePres->fax);
    ui->txtmovil->setText(oClientePres->movil);
    ui->txtemail->setText(oClientePres->email);

    if (oClientePres->lIRPF==1) {
        ui->chklporc_rec->setChecked(true);
        oPres->recargo_equivalencia = 1;
    } else {
        ui->chklporc_rec->setChecked(false);
        oPres->recargo_equivalencia = 0;
    }
}

void FrmPresupuestosCli::LLenarPresupuesto()
{
    oPres->nPresupuesto = (ui->txtnPresupuesto->text().toInt());
    oPres->fecha = (ui->txtfecha->date());
    oPres->dValidoHasta = (ui->txtdValidoHasta->date());
    oPres->codigo_cliente = (ui->txtcodigo_cliente->text());
    oPres->cliente = (ui->txtcliente->text());
    oPres->cif = (ui->txtcif->text());
    oPres->direccion1 = (ui->txtdireccion1->text());
    oPres->direccion2 = (ui->txtdireccion2->text());
    oPres->cp = (ui->txtcp->text());
    oPres->poblacion = (ui->txtpoblacion->text());
    oPres->provincia = (ui->txtprovincia->text());

    oPres->id_pais = Configuracion_global->paises[ui->combo_Pais->currentText()].value("id").toInt();

    oPres->telefono = (ui->txttelefono->text());
    oPres->movil = (ui->txtmovil->text());
    oPres->fax = (ui->txtfax->text());
    oPres->comentarios = (ui->txtcomentario->toPlainText());

    oPres->importe = ui->txtbase->text().replace(moneda,"") .toDouble();
    oPres->base = (ui->txtsubtotal->text().replace(moneda,"") .toDouble());
    oPres->rDescuento = (ui->txtrDescuento->text().replace(moneda,"") .toDouble());
    oPres->total = (ui->txttotal->text().replace(moneda,"") .toDouble());

    oPres->lAprobado = ui->chklAprovado->isChecked();

    oPres->fechaAprobacion = (ui->txtfechaAprovacion->date());
    oPres->importeFactura = (ui->txtimporteFactura->text().replace(moneda,"") .toDouble());

    if(ui->txtfactura->text()=="")
        oPres->factura = "0";
    else
        oPres->factura = (ui->txtfactura->text());

    oPres->albaran = (ui->txtalbaran->text().toInt());
    oPres->pedido = (ui->txtpedido->text().toInt());

    QSqlTableModel*  modeloPago = static_cast<QSqlTableModel*>(ui->cboFormaPago->model());
    oPres->descripcionFormaPago = ui->cboFormaPago->currentText();
    oPres->codigoFormaPago = modeloPago->record(ui->combo_Pais->currentIndex()).value("codigo").toString();
    oPres->id_forma_pago = modeloPago->record(ui->combo_Pais->currentIndex()).value("id").toInt();

    oPres->tLugarEntrega = (ui->txttLugarEntrega->toPlainText());
    oPres->cAtencionde = (ui->txtcAtencionde->text());
    oPres->base1 = (ui->txtbase1->text().replace(moneda,"") .toDouble());
    oPres->base2 = (ui->txtbase2->text().replace(moneda,"") .toDouble());
    oPres->base3 = (ui->txtbase3->text().replace(moneda,"") .toDouble());
    oPres->base4 = (ui->txtbase4->text().replace(moneda,"") .toDouble());
    //oPres->iva1 = (ui->txtporc_iva1->text().toDouble());
    //oPres->iva2 = (ui->txtporc_iva2->text().toDouble());
    //oPres->iva3 = (ui->txtporc_iva3->text().toDouble());
    //oPres->iva4 = (ui->txtporc_iva4->text().toDouble());
    oPres->iva1 = (ui->txtiva1->text().replace(moneda,"") .toDouble());
    oPres->iva2 = (ui->txtiva2->text().replace(moneda,"") .toDouble());
    oPres->iva3 = (ui->txtiva3->text().replace(moneda,"") .toDouble());
    oPres->iva4 = (ui->txtiva4->text().replace(moneda,"") .toDouble());
    //oPres->porc_porc_recuivalencia1 = (ui->txtporc_rec1->text().toDouble());
    //oPres->porc_porc_recuivalencia2 = (ui->txtporc_rec2->text().toDouble());
    //oPres->porc_porc_recuivalencia3 = (ui->txtporc_rec3->text().toDouble());
    //oPres->porc_porc_recuivalencia4 = (ui->txtporc_rec4->text().toDouble());
    oPres->recargo_equivalencia = ui->chklporc_rec->isChecked();
    oPres->rec1 = (ui->txtporc_rec1->text().replace(moneda,"") .toDouble());
    oPres->rec2 = (ui->txtporc_rec2->text().replace(moneda,"") .toDouble());
    oPres->rec3 = (ui->txtporc_rec3->text().replace(moneda,"") .toDouble());
    oPres->rec4 = (ui->txtporc_rec4->text().replace(moneda,"") .toDouble());
    oPres->total1 = (ui->txttotal1->text().replace(moneda,"") .toDouble());
    oPres->total2 = (ui->txttotal2->text().replace(moneda,"") .toDouble());
    oPres->total3 = (ui->txttotal3->text().replace(moneda,"") .toDouble());
    oPres->total4 = (ui->txttotal4->text().replace(moneda,"") .toDouble());
    oPres->total_iva = ui->txttotal_iva->text().replace(moneda,"") .toDouble();
    oPres->totalRec = ui->txttotal_recargo->text().replace(moneda,"") .toDouble();
    oPres->email = (ui->txtemail->text());
}
void FrmPresupuestosCli::VaciarCampos()
{
    ui->lblTopNpres->setText("");
    ui->lblTopcliente->setText("");
    ui->txtnPresupuesto->setText("0");
    ui->txtfecha->setDate(QDate::currentDate());
    ui->txtdValidoHasta->setDate(QDate::currentDate());
    ui->txtcodigo_cliente->setText("");
    ui->txtcif->setText("");
    ui->txtdireccion1->setText("");
    ui->txtdireccion2->setText("");
    ui->txtcp->setText("");
    ui->txtpoblacion->setText("");
    ui->txtprovincia->setText("");
    //ui->txtpais->setText("");
    ui->txttelefono->setText("");
    ui->txtmovil->setText("");
    ui->txtfax->setText("");
    ui->txtcomentario->setPlainText("");
    ui->txtbase->setText("0,00");
    ui->txtsubtotal->setText("0,00");
    ui->txtrDescuento->setText("0,00");
    ui->txttotal->setText("0,00");
    ui->lbimpreso->setVisible(false);
    ui->chklAprovado->setChecked(false);
    ui->txtfechaAprovacion->setDate(QDate::currentDate());
    ui->txtimporteFactura->setText("0,00");
    ui->txtfactura->setText("");
    ui->txtalbaran->setText("");
    ui->txtpedido->setText("");
    ui->cboFormaPago->setCurrentIndex(-1);
    ui->txttLugarEntrega->setPlainText("");
    ui->txtcAtencionde->setText("");
    ui->txtbase1->setText("0,00");
    ui->txtbase2->setText("0,00");
    ui->txtbase3->setText("0,00");
    ui->txtbase4->setText("0,00");
   // ui->txtporc_iva1->setText("0");
   // ui->txtporc_iva2->setText("0");
   // ui->txtporc_iva3->setText("0");
   // ui->txtporc_iva4->setText("0");
    ui->txtiva1->setText("0,00");
    ui->txtiva2->setText("0,00");
    ui->txtiva3->setText("0,00");
    ui->txtiva4->setText("0,00");
    //ui->txtporc_rec1->setText("0");
    //ui->txtporc_rec2->setText("0");
    //ui->txtporc_rec3->setText("0");
    //ui->txtporc_rec4->setText("0");
    ui->txtporc_rec1->setText("0,00");
    ui->txtporc_rec2->setText("0,00");
    ui->txtporc_rec3->setText("0,00");
    ui->txtporc_rec4->setText("0,00");
    ui->txttotal1->setText("0,00");
    ui->txttotal2->setText("0,00");
    ui->txttotal3->setText("0,00");
    ui->txttotal4->setText("0,00");
    ui->txtemail->setText("");
    ui->txtcliente->setText("");

    helper.fillTable("empresa","lin_pre","id_Cab = -1");
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
        ui->txtfechaAprovacion->setVisible(false);
    } else {
        ui->lblFechaAprovacin->setVisible(true);
        ui->txtfechaAprovacion->setVisible(true);
    }
}

void FrmPresupuestosCli::on_btnSiguiente_clicked()
{
    if(oPres->siguiente())
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oPres->id);
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
        QString filter = QString("id_Cab = '%1'").arg(oPres->id);
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
    QString filter = QString("id_Cab = '%1'").arg(ui->txtnPresupuesto->text());
    helper.fillTable("empresa","lin_pre",filter);
    helper.set_tarifa(Configuracion_global->id_tarifa_predeterminada);
    BloquearCampos(false);
    int next = oPres->NuevoNumeroPresupuesto();
    ui->txtnPresupuesto->setText(QString::number(next));
    ui->lblTopNpres->setText(QString::number(next));
    LLenarPresupuesto();
    oPres->AnadirPresupuesto();
    ui->txtcodigo_cliente->setFocus();
    editando = false;
    emit block();
}

void FrmPresupuestosCli::on_btnEditar_clicked()
{
    if (oPres->factura=="0")
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
    searcher.set_selection("id");

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
        QString cid =searcher.selected_value;
        oPres->id_cliente = cid.toInt();
        oClientePres->Recuperar("Select * from clientes where id ="+cid+" order by id limit 1 ");
        helper.set_tarifa(oClientePres->idTarifa);
        LLenarCamposCliente();
    }
}

void FrmPresupuestosCli::totalChanged(double base , double dto ,double subtotal , double iva, double re, double total, QString moneda)
{
    this->moneda = moneda;
    ui->txtbase->setText(QString::number(base)+moneda);
    ui->txtrDescuento->setText(QString::number(dto)+moneda);
    ui->txtsubtotal->setText(QString::number(subtotal)+moneda);
    ui->txtiva->setText(QString::number(iva)+moneda);
    ui->txttotal_recargo->setText(QString::number(re)+moneda);
    ui->txttotal->setText(QString::number(total)+moneda);

    ui->txtbase_total->setText(QString::number(subtotal)+moneda);
    ui->txttotal_iva->setText(QString::number(iva)+moneda);
    ui->txttotal_recargo_2->setText(QString::number(re)+moneda);
    ui->txttotal_2->setText(QString::number(total)+moneda);
}

void FrmPresupuestosCli::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtbase1->setText(QString::number(base));
    ui->txtiva1->setText(QString::number(iva));
    ui->txtporc_rec1->setText(QString::number(re));
    ui->txttotal1->setText(QString::number(total));
}

void FrmPresupuestosCli::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtbase2->setText(QString::number(base));
    ui->txtiva2->setText(QString::number(iva));
    ui->txtporc_rec2->setText(QString::number(re));
    ui->txttotal2->setText(QString::number(total));
}

void FrmPresupuestosCli::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtbase3->setText(QString::number(base));
    ui->txtiva3->setText(QString::number(iva));
    ui->txtporc_rec3->setText(QString::number(re));
    ui->txttotal3->setText(QString::number(total));
}

void FrmPresupuestosCli::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtbase4->setText(QString::number(base));
    ui->txtiva4->setText(QString::number(iva));
    ui->txtporc_rec4->setText(QString::number(re));
    ui->txttotal4->setText(QString::number(total));
}

void FrmPresupuestosCli::on_btnDeshacer_clicked()
{
    VaciarCampos();
    LLenarCampos();
    BloquearCampos(true);
    QString filter = QString("id_Cab = '%1'").arg(ui->txtnPresupuesto->text());
    helper.fillTable("empresa","lin_pre",filter);
    emit unblock();
}

void FrmPresupuestosCli::convertir_epedido()
{
    //TODO FrmPresupuestosCli::convertir_enPresupuesto()
}

void FrmPresupuestosCli::convertir_ealbaran()
{
    //TODO FrmPresupuestosCli::convertir_ealbaran()
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
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;
        QSqlQuery query_lin_ped_pro(QSqlDatabase::database("empresa"));
        query_lin_ped_pro.prepare("INSERT INTO lin_pre (id_Cab, id_articulo, codigo, cantidad,"
                                  "descripcion, pvp, subtotal, dto,"
                                  " porc_dto, porc_iva, total)"
                                  "VALUES (:id_cab,:id_articulo,:codigo,:cantidad,"
                                  ":descripcion,:coste_bruto,:subtotal_coste,:porc_dto,"
                                  ":dto,:porc_iva,:total);");
        query_lin_ped_pro.bindValue(":id_cab", oPres->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_ped_pro.bindValue(":subtotal_coste",ld->subtotal);
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
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery query_lin_ped_pro(QSqlDatabase::database("empresa"));
        query_lin_ped_pro.prepare("UPDATE lin_pre SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo =:codigo_articulo_proveedor,"
                                  "descripcion =:descripcion,"
                                  "cantidad =:cantidad,"
                                  "pvp =:coste_bruto,"
                                  "subtotal =:subtotal_coste,"
                                  "dto =:porc_dto,"
                                  "porc_dto =:dto,"
                                  "porc_iva =:porc_iva,"
                                  "total =:total "
                                  "WHERE id = :id;");

        query_lin_ped_pro.bindValue(":id_cab", oPres->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_ped_pro.bindValue(":subtotal_coste",ld->subtotal);
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
