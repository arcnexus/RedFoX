#include "frmpresupuestoscli.h"
#include "ui_frmpresupuestoscli.h"
#include <QSqlQueryModel>

#include "../Busquedas/frmbuscarcliente.h"
#include "../Almacen/articulo.h"
#include "../Busquedas/db_consulta_view.h"
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
    QSqlTableModel* model = new QSqlTableModel(ui->cboFormaPago,QSqlDatabase::database("maya"));
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
//    if(oPres->siguiente())
//    {
//        LLenarCampos();
//        QString filter = QString("id_Cab = '%1'").arg(oPres->id);
//        helper.fillTable("empresa","lin_pre",filter);
//        ui->btnAnterior->setEnabled(false);
//    }
//    else
//    {
        ui->btnBorrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btn_convertir->setEnabled(false);
        ui->btnAnterior->setEnabled(false);
        ui->btnSiguiente->setEnabled(true);
        ui->btnBuscar->setEnabled(true);
        ui->btnImprimir->setEnabled(false);
//    }
}

FrmPresupuestosCli::~FrmPresupuestosCli()
{
    delete ui;
    delete oPres;
    delete oClientePres;
}

void FrmPresupuestosCli::LLenarCampos()
{
    ui->txtpresupuesto->setText(QString::number(oPres->presupuesto));
    ui->lblTopNpres->setText(QString::number(oPres->presupuesto));
    ui->lblTopcliente->setText(oPres->cliente);
    ui->txtfecha->setDate(oPres->fecha);
    ui->txtvalido_hasta->setDate(oPres->valido_hasta);
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
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base,'f',2)+moneda));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->subtotal,'f',2)+moneda));
    ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->dto,'f',2)+moneda));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total,'f',2)+moneda));
    ui->lbimpreso->setVisible(oPres->impreso);
    ui->chklAprovado->setChecked(oPres->aprobado);
    ui->txtfechaAprovacion->setDate(oPres->fecha_aprobacion);
    ui->txtimporte_factura->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->importe_factura,'f',2)));
    ui->txtfactura->setText(oPres->factura);
    ui->txtalbaran->setText(QString::number(oPres->albaran));
    ui->txtpedido->setText(QString::number(oPres->pedido));

    int nIndex =  ui->cboFormaPago->findText(oPres->descripcionFormaPago);
    if (!(nIndex ==-1))
        ui->cboFormaPago->setCurrentIndex(nIndex);

    ui->txtlugar_entrega->setPlainText(oPres->lugar_entrega);
    ui->txtatencion_de->setText(oPres->atencion_de);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base1,'f',2)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base2,'f',2)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base3,'f',2)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base4,'f',2)));
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base,'f',2)+moneda));

    ui->txtporc_iva1->setText(QString::number(oPres->iva1));
    ui->txtporc_iva2->setText(QString::number(oPres->iva2));
    ui->txtporc_iva3->setText(QString::number(oPres->iva3));
    ui->txtporc_iva4->setText(QString::number(oPres->iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva1,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva2,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva3,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva4,'f',2)));
    ui->txttotal_iva->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_iva,'f',2)+moneda));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_iva,'f',2)+moneda));
    ui->txtporc_rec1->setText(QString::number(oPres->porc_rec1));
    ui->txtporc_rec2->setText(QString::number(oPres->porc_rec2));
    ui->txtporc_rec3->setText(QString::number(oPres->porc_rec3));
    ui->txtporc_rec4->setText(QString::number(oPres->porc_rec4));
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_recargo,'f',2)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_recargo,'f',2)+moneda));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec1,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec2,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec3,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec4,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total1,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total2,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total3,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total4,'f',2)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total,'f',2)+moneda));
    ui->txtemail->setText(oPres->email);
    ui->chklporc_rec->setChecked(oPres->recargo_equivalencia);
    oClientePres->Recuperar("Select * from clientes where id ="+QString::number(oPres->id_cliente));
    helper.set_tarifa(oClientePres->tarifa_cliente);
    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();
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
    ui->txtatencion_de->setText(oClientePres->apellido1 + " "+ oClientePres->apellido2 + ", "+oClientePres->nombre);
    ui->txtmovil->setText(oClientePres->movil);
    ui->txtfax->setText(oClientePres->fax);

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
    oPres->id_cliente = oClientePres->id;
}

void FrmPresupuestosCli::LLenarPresupuesto()
{
    oPres->presupuesto = (ui->txtpresupuesto->text().toInt());
    oPres->fecha = (ui->txtfecha->date());
    oPres->valido_hasta = (ui->txtvalido_hasta->date());
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

    oPres->base = (ui->txtbase->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->dto = (ui->txtdto->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->total = (ui->txttotal->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());

    oPres->aprobado = ui->chklAprovado->isChecked();

    oPres->fecha_aprobacion = (ui->txtfechaAprovacion->date());
    oPres->importe_factura = (ui->txtimporte_factura->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());

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

    oPres->lugar_entrega = (ui->txtlugar_entrega->toPlainText());
    oPres->atencion_de = (ui->txtatencion_de->text());
    oPres->base1 = (ui->txtbase1->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->base2 = (ui->txtbase2->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->base3 = (ui->txtbase3->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->base4 = (ui->txtbase4->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->iva1 = (ui->txtporc_iva1->text().toDouble());
    oPres->iva2 = (ui->txtporc_iva2->text().toDouble());
    oPres->iva3 = (ui->txtporc_iva3->text().toDouble());
    oPres->iva4 = (ui->txtporc_iva4->text().toDouble());
    oPres->iva1 = (ui->txtiva1->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->iva2 = (ui->txtiva2->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->iva3 = (ui->txtiva3->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->iva4 = (ui->txtiva4->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->porc_rec1 = (ui->txtporc_rec1->text().toDouble());
    oPres->porc_rec2 = (ui->txtporc_rec2->text().toDouble());
    oPres->porc_rec3 = (ui->txtporc_rec3->text().toDouble());
    oPres->porc_rec4 = (ui->txtporc_rec4->text().toDouble());
    oPres->recargo_equivalencia = ui->chklporc_rec->isChecked();
    oPres->rec1 = (ui->txtporc_rec1->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->rec2 = (ui->txtporc_rec2->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->rec3 = (ui->txtporc_rec3->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->rec4 = (ui->txtporc_rec4->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->total1 = (ui->txttotal1->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->total2 = (ui->txttotal2->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->total3 = (ui->txttotal3->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->total4 = (ui->txttotal4->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    oPres->total_iva = ui->txttotal_iva->text().replace(".","").replace(moneda,"").replace(",",".").toDouble();
    oPres->total_recargo = ui->txttotal_recargo_2->text().replace(".","").replace(moneda,"").replace(",",".").toDouble();
    oPres->email = (ui->txtemail->text());
    oPres->subtotal = (ui->txtsubtotal->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
}
void FrmPresupuestosCli::VaciarCampos()
{
    ui->lblTopNpres->setText("");
    ui->lblTopcliente->setText("");
    ui->txtpresupuesto->setText("0");
    ui->txtfecha->setDate(QDate::currentDate());
    ui->txtvalido_hasta->setDate(QDate::currentDate());
    ui->txtcodigo_cliente->setText("");
    ui->txtcif->setText("");
    ui->txtdireccion1->setText("");
    ui->txtdireccion2->setText("");
    ui->txtcp->setText("");
    ui->txtpoblacion->setText("");
    ui->txtprovincia->setText("");
    ui->combo_Pais->setCurrentIndex(-1);
    ui->txttelefono->setText("");
    ui->txtmovil->setText("");
    ui->txtfax->setText("");
    ui->txtcomentario->setPlainText("");
    ui->txtbase->setText("0,00");
    ui->txtsubtotal->setText("0,00");
    ui->txtdto->setText("0,00");
    ui->txttotal->setText("0,00");
    ui->lbimpreso->setVisible(false);
    ui->chklAprovado->setChecked(false);
    ui->txtfechaAprovacion->setDate(QDate::currentDate());
    ui->txtimporte_factura->setText("0,00");
    ui->txtfactura->setText("");
    ui->txtalbaran->setText("");
    ui->txtpedido->setText("");
    ui->cboFormaPago->setCurrentIndex(-1);
    ui->txtlugar_entrega->setPlainText("");
    ui->txtatencion_de->setText("");
    ui->txtbase1->setText("0,00");
    ui->txtbase2->setText("0,00");
    ui->txtbase3->setText("0,00");
    ui->txtbase4->setText("0,00");
    ui->txtporc_iva1->setText("0");
    ui->txtporc_iva2->setText("0");
    ui->txtporc_iva3->setText("0");
    ui->txtporc_iva4->setText("0");
    ui->txtiva1->setText("0,00");
    ui->txtiva2->setText("0,00");
    ui->txtiva3->setText("0,00");
    ui->txtiva4->setText("0,00");
    ui->txtporc_rec1->setText("0");
    ui->txtporc_rec2->setText("0");
    ui->txtporc_rec3->setText("0");
    ui->txtporc_rec4->setText("0");
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
    ui->btndeshacer->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->btnAnadirLinea->setEnabled(!state);
    ui->btn_borrarLinea->setEnabled(!state);
    ui->botBuscarCliente->setEnabled(!state);
    ui->txtpresupuesto->setReadOnly(true);
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
    QString filter = QString("id_Cab = '%1'").arg(ui->txtpresupuesto->text());
    helper.fillTable("empresa","lin_pre",filter);
    helper.set_tarifa(Configuracion_global->id_tarifa_predeterminada);
    BloquearCampos(false);
    int next = oPres->NuevoNumeroPresupuesto();
    ui->txtpresupuesto->setText(QString::number(next));
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
    db_consulta_view consulta;
    QStringList campos;
    campos <<"codigo_cliente" <<"nombre_fiscal" << "cif_nif"<< "poblacion" << "telefono1";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("clientes");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,codigo_cliente,nombre_fiscal,cif_nif,poblacion,telefono1 from clientes");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Código") << tr("Nombre") << tr("CIF/NIF") << tr("Población") << tr("Teléfono");
    tamanos <<"0" << "100" << "300" << "100" << "180" <<"130";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Clientes");
    if(consulta.exec())
    {
        int id = consulta.get_id();
        oClientePres->Recuperar("select * from clientes where id="+QString::number(id));
        LLenarCamposCliente();
        helper.set_tarifa(oClientePres->tarifa_cliente);
    }
}

void FrmPresupuestosCli::totalChanged(double base , double dto ,double subtotal , double iva, double re, double total, QString moneda)
{
    this->moneda = moneda;
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',2))+moneda);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',2))+moneda);
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);

    ui->txttotal_base->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txttotal_iva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);
}

void FrmPresupuestosCli::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmPresupuestosCli::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmPresupuestosCli::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmPresupuestosCli::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmPresupuestosCli::on_btnDeshacer_clicked()
{
    VaciarCampos();
    LLenarCampos();
    BloquearCampos(true);
    QString filter = QString("id_Cab = '%1'").arg(ui->txtpresupuesto->text());
    helper.fillTable("empresa","lin_pre",filter);
    emit unblock();
}

void FrmPresupuestosCli::convertir_epedido()
{
    //TODO FrmPresupuestosCli::convertir_epresupuesto()
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
        q.prepare("DELETE FROM cab_pre WHERE presupuesto = "+ui->txtpresupuesto->text());
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
        QSqlQuery lin_pre(QSqlDatabase::database("empresa"));
        lin_pre.prepare("INSERT INTO lin_pre (id_cab, id_articulo, codigo, cantidad,"
                                  "descripcion, precio, subtotal, porc_dto,"
                                  " dto, porc_iva, iva, total)"
                                  "VALUES (:id_cab,:id_articulo,:codigo,:cantidad,"
                                  ":descripcion,:precio,:subtotal,:porc_dto,"
                                  ":dto,:porc_iva,:iva,:total);");
        lin_pre.bindValue(":id_cab", oPres->id);
        lin_pre.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        lin_pre.bindValue(":codigo",ld->codigo);
        lin_pre.bindValue(":descripcion",ld->descripcion);
        lin_pre.bindValue(":cantidad",ld->cantidad);
        lin_pre.bindValue(":precio",ld->precio);
        lin_pre.bindValue(":subtotal",ld->subtotal);
        lin_pre.bindValue(":porc_dto",ld->dto_perc);
        lin_pre.bindValue(":dto",ld->dto);
        lin_pre.bindValue(":iva",(ld->subtotal * ld->iva_perc)/100);
        lin_pre.bindValue(":total",ld->total);
        if (!lin_pre.exec()){
            ok_empresa = false;
            QMessageBox::warning(this,tr("Gestión de albaranes"),
                                 tr("Ocurrió un error al insertar la nueva línea: %1").arg(lin_pre.lastError().text()),
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

        ld->idLinea = lin_pre.lastInsertId().toInt();

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

        QSqlQuery lin_pre(QSqlDatabase::database("empresa"));
        lin_pre.prepare("UPDATE lin_pre SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo =:codigo,"
                                  "descripcion =:descripcion,"
                                  "cantidad =:cantidad,"
                                  "precio =:precio,"
                                  "subtotal =:subtotal,"
                                  "porc_dto =:porc_dto,"
                                  "dto =:dto,"
                                  "porc_iva =:porc_iva,"
                                  "iva =:iva,"
                                  "total =:total "
                                  "WHERE id = :id;");

        lin_pre.bindValue(":id_cab", oPres->id);
        lin_pre.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        lin_pre.bindValue(":codigo",ld->codigo);
        lin_pre.bindValue(":descripcion",ld->descripcion);
        lin_pre.bindValue(":cantidad",ld->cantidad);
        lin_pre.bindValue(":precio",ld->precio);
        lin_pre.bindValue(":subtotal",ld->subtotal);
        lin_pre.bindValue(":porc_dto",ld->dto_perc);
        lin_pre.bindValue(":dto",ld->dto);
        lin_pre.bindValue(":porc_iva",ld->iva_perc);
        lin_pre.bindValue(":iva",(ld->subtotal * ld->iva_perc)/100);
        lin_pre.bindValue(":total",ld->total);
        lin_pre.bindValue(":id",ld->idLinea);

        if (!lin_pre.exec()) {
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Ocurrió un error al guardar la línea: %1").arg(lin_pre.lastError().text()),
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



void FrmPresupuestosCli::on_btndeshacer_clicked()
{
    QSqlDatabase::database("Maya").rollback();
    QSqlDatabase::database("empresa").rollback();
    BloquearCampos(true);
    LLenarCampos();
}

void FrmPresupuestosCli::on_txtcodigo_cliente_editingFinished()
{
    if(ui->txtcodigo_cliente->text() != oPres->codigo_cliente){
        oClientePres->Recuperar("select * from clientes where codigo_cliente='"+ui->txtcodigo_cliente->text()+"'");
        LLenarCamposCliente();
        helper.set_tarifa(oClientePres->tarifa_cliente);
    }
}
