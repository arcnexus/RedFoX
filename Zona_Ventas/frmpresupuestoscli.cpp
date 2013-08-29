#include "frmpresupuestoscli.h"
#include "ui_frmpresupuestoscli.h"
#include <QSqlQueryModel>

#include "../Busquedas/frmbuscarcliente.h"
#include "../Almacen/articulo.h"
#include "../Busquedas/db_consulta_view.h"
#include <QMessageBox>
#include "pedidos.h"
#include "albaran.h"
#include "factura.h"


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
   // ui->combo_Pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de presupuestos a clientes"));

    // cargar datos FormaPago
    ui->cboFormaPago->setInsertPolicy(QComboBox::NoInsert);
    ui->cboFormaPago->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    QSqlTableModel* model = new QSqlTableModel(ui->cboFormaPago,Configuracion_global->groupDB);
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
        ui->btnBorrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btn_convertir->setEnabled(false);
        ui->btnAnterior->setEnabled(false);
        ui->btnSiguiente->setEnabled(true);
        ui->btnBuscar->setEnabled(true);
        ui->btnImprimir->setEnabled(false);
//    }
        //-----------------------
        // tabla
        //-----------------------
        m = new QSqlQueryModel(this);
        m->setQuery("select id,presupuesto,fecha,telefono,movil,cliente from cab_pre order by presupuesto desc",
                    Configuracion_global->empresaDB);
        ui->tabla->setModel(m);
        formato_tabla();

        //------------------------
        // orden
        //------------------------
        QStringList orden;
        orden << tr("presupuesto") <<tr("cliente") << tr("teléfono");
        ui->cboOrden->addItems(orden);

        //--------------------
        // modo
        //--------------------
        QStringList modo;
        modo << tr("A-Z") <<tr("Z-A");
        ui->cboModo->addItems(modo);
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
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base,'f',Configuracion_global->decimales)+moneda));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->subtotal,'f',Configuracion_global->decimales)+moneda));
    ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->dto,'f',Configuracion_global->decimales)+moneda));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total,'f',Configuracion_global->decimales)+moneda));
    ui->lbimpreso->setVisible(oPres->impreso);
    ui->chklAprovado->setChecked(oPres->aprobado);
    ui->txtfechaAprovacion->setDate(oPres->fecha_aprobacion);
    ui->txtimporte_factura->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->importe_factura,'f',Configuracion_global->decimales)));
    ui->txtfactura->setText(oPres->factura);
    ui->txtalbaran->setText(QString::number(oPres->albaran));
    ui->txtpedido->setText(QString::number(oPres->pedido));

    int nIndex =  ui->cboFormaPago->findText(oPres->descripcionFormaPago);
    if (!(nIndex ==-1))
        ui->cboFormaPago->setCurrentIndex(nIndex);

    ui->txtlugar_entrega->setPlainText(oPres->lugar_entrega);
    ui->txtatencion_de->setText(oPres->atencion_de);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base1,'f',Configuracion_global->decimales)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base2,'f',Configuracion_global->decimales)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base3,'f',Configuracion_global->decimales)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base4,'f',Configuracion_global->decimales)));
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base,'f',Configuracion_global->decimales)+moneda));

    ui->txtporc_iva1->setText(QString::number(oPres->porc_iva1));
    ui->txtporc_iva2->setText(QString::number(oPres->porc_iva2));
    ui->txtporc_iva3->setText(QString::number(oPres->porc_iva3));
    ui->txtporc_iva4->setText(QString::number(oPres->porc_iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva1,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva2,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva3,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva4,'f',Configuracion_global->decimales)));
    ui->txttotal_iva->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_iva,'f',Configuracion_global->decimales)+moneda));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_iva,'f',Configuracion_global->decimales)+moneda));
    ui->txtporc_rec1->setText(QString::number(oPres->porc_rec1));
    ui->txtporc_rec2->setText(QString::number(oPres->porc_rec2));
    ui->txtporc_rec3->setText(QString::number(oPres->porc_rec3));
    ui->txtporc_rec4->setText(QString::number(oPres->porc_rec4));
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_recargo,'f',Configuracion_global->decimales)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_recargo,'f',Configuracion_global->decimales)+moneda));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec1,'f',Configuracion_global->decimales)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec2,'f',Configuracion_global->decimales)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec3,'f',Configuracion_global->decimales)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec4,'f',Configuracion_global->decimales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total1,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total2,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total3,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total4,'f',Configuracion_global->decimales)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total,'f',Configuracion_global->decimales)+moneda));
    ui->txtemail->setText(oPres->email);
    ui->chklporc_rec->setChecked(oPres->recargo_equivalencia);
    oClientePres->Recuperar("Select * from clientes where id ="+QString::number(oPres->id_cliente));
    helper.set_tarifa(oClientePres->tarifa_cliente);
    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();
    QString filter = QString("id_Cab = '%1'").arg(oPres->id);
    helper.fillTable("empresa","lin_pre",filter);
    helper.set_tipo_dto_tarifa(oClientePres->tipo_dto_tarifa);
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
    helper.set_tipo_dto_tarifa(oClientePres->tipo_dto_tarifa);
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

    oPres->base = (ui->txtbase->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->dto = (ui->txtdto->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->total = (ui->txttotal->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());

    oPres->aprobado = ui->chklAprovado->isChecked();

    oPres->fecha_aprobacion = (ui->txtfechaAprovacion->date());
    oPres->importe_factura = (ui->txtimporte_factura->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());

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
    oPres->base1 = (ui->txtbase1->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->base2 = (ui->txtbase2->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->base3 = (ui->txtbase3->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->base4 = (ui->txtbase4->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->iva1 = (ui->txtporc_iva1->text().toDouble());
    oPres->iva2 = (ui->txtporc_iva2->text().toDouble());
    oPres->iva3 = (ui->txtporc_iva3->text().toDouble());
    oPres->iva4 = (ui->txtporc_iva4->text().toDouble());
    oPres->iva1 = (ui->txtiva1->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->iva2 = (ui->txtiva2->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->iva3 = (ui->txtiva3->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->iva4 = (ui->txtiva4->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->porc_rec1 = (ui->txtporc_rec1->text().toDouble());
    oPres->porc_rec2 = (ui->txtporc_rec2->text().toDouble());
    oPres->porc_rec3 = (ui->txtporc_rec3->text().toDouble());
    oPres->porc_rec4 = (ui->txtporc_rec4->text().toDouble());
    oPres->recargo_equivalencia = ui->chklporc_rec->isChecked();
    oPres->rec1 = (ui->txtporc_rec1->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->rec2 = (ui->txtporc_rec2->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->rec3 = (ui->txtporc_rec3->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->rec4 = (ui->txtporc_rec4->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->total1 = (ui->txttotal1->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->total2 = (ui->txttotal2->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->total3 = (ui->txttotal3->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->total4 = (ui->txttotal4->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->total_iva = ui->txttotal_iva->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble();
    oPres->total_recargo = ui->txttotal_recargo_2->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble();
    oPres->email = (ui->txtemail->text());
    oPres->subtotal = (ui->txtsubtotal->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
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
    ui->txtBuscar->setReadOnly(!state);
    ui->cboOrden->setEnabled(state);
    ui->cboModo->setEnabled(state);
    ui->btnImprimir->setEnabled(true);
    ui->btn_convertir->setEnabled(state);
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
        ui->btnSiguiente->setEnabled(true);
        ui->btnImprimir->setEnabled(true);
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
    Configuracion_global->empresaDB.transaction();
    bool succes = true;

    succes &= oPres->GuardarPres(oPres->id);

    if(succes)
    {
        LLenarCampos();
        succes = Configuracion_global->empresaDB.commit();
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
                              tr("Error al guardar el presupuesto.\n")+Configuracion_global->empresaDB.lastError().text(),
                              tr("&Aceptar"));
        Configuracion_global->empresaDB.rollback();
    }
}

void FrmPresupuestosCli::on_btnBuscar_clicked()
{
    ui->radBusqueda->setChecked(true);
    ui->txtBuscar->setFocus();
}

void FrmPresupuestosCli::on_botBuscarCliente_clicked()
{    
    db_consulta_view consulta;
    QStringList campos;
    campos  <<"nombre_fiscal" <<"codigo_cliente" << "cif_nif"<< "poblacion" << "telefono1";
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
    double total1 = base +iva;
    total = total1;
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales))+moneda);
    ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales))+moneda);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales))+moneda);
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales))+moneda);
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales))+moneda);
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales))+moneda);

    ui->txttotal_base->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales))+moneda);
    ui->txttotal_iva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales))+moneda);
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales))+moneda);
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales))+moneda);
}

void FrmPresupuestosCli::desglose1Changed(double base, double iva, double re, double total)
{
    total = base +iva;
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPresupuestosCli::desglose2Changed(double base, double iva, double re, double total)
{
    total = base +iva;
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPresupuestosCli::desglose3Changed(double base, double iva, double re, double total)
{
        total = base +iva;
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPresupuestosCli::desglose4Changed(double base, double iva, double re, double total)
{
    total = base +iva;
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
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

    QString c = QString("id = %1").arg(oPres->id);
    QString error;
    QHash <QString, QVariant> h;
    QMap<int, QSqlRecord> records = SqlCalls::SelectRecord("cab_pre",c,Configuracion_global->empresaDB,error);
    QSqlRecord r = records.value(oPres->id);
    for(int i= 0; i< r.count();i++)
        h.insert(r.fieldName(i),r.value(i));
    // ---------------------------------
    // Ajustes campos antes de insertar
    //----------------------------------
    h.insert("imp_gasto1",h.value("importe_gasto1").toDouble());
    h.remove("importe_gasto1");
    h.insert("imp_gasto2",h.value("importe_gasto2").toDouble());
    h.remove("importe_gasto2");
    h.insert("imp_gasto3",h.value("importe_gasto3").toDouble());
    h.remove("importe_gasto3");

    h.insert("comentario",h.value("comentarios").toString());
    h.remove("comentarios");
    h.insert("rec_total",h.value("total_recargo").toDouble());
    h.remove("total_recargo");
    h.remove("aprobado");
    h.remove("impreso");
    h.remove("fecha_aprobacion");
    h.insert("iva_total",h.value("total_iva").toDouble());
    h.remove("total_iva");
    h.remove("atencion_de");
    h.remove("importe_pendiente");
    h.insert("subtotal",h.value("importe").toDouble());
    h.remove("importe");
    h.insert("desc_gasto1",h.value("gastos_distribuidos1").toString());
    h.remove("gastos_distribuidos1");
    h.insert("desc_gasto2",h.value("gastos_distribuidos2").toString());
    h.remove("gastos_distribuidos2");
    h.insert("desc_gasto3",h.value("gastos_distribuidos3").toString());
    h.remove("gastos_distribuidos3");
    h.insert("base_total",h.value("base").toDouble());
    h.remove("base");
    h.remove("valido_hasta");
    h.insert("total_pedido",h.value("total").toDouble());
    h.remove("total");
    h.remove("lugar_entrega");
    h.remove("importe_factura");
    h.remove("presupuesto");
    h.remove("id");
    Pedidos oPedido;
    Configuracion_global->empresaDB.transaction();
    int num = oPedido.NuevoNumeroPedido();
    h.insert("pedido",num);
    h["fecha"] = QDate::currentDate();

    int added = SqlCalls::SqlInsert(h,"ped_cli",Configuracion_global->empresaDB,error);
    // ----------------
    // Lineas de pedido
    // ----------------
    int added_l;
    QHash <QString, QVariant> h_l;
    QString d = QString("id_cab = %1").arg(oPres->id);
    QMap<int, QSqlRecord> map = SqlCalls::SelectRecord("lin_pre", d,Configuracion_global->empresaDB, error);
      QMapIterator<int, QSqlRecord> i_l(map);
      while (i_l.hasNext())
      {
          i_l.next();
          QSqlRecord r_l = i_l.value();
          for(int i= 0; i< r_l.count();i++)
              h_l.insert(r_l.fieldName(i),r_l.value(i));
          h_l.remove("id");
          h_l["id_cab"] = added;
         added_l = SqlCalls::SqlInsert(h_l,"lin_ped",Configuracion_global->empresaDB,error);
         if(added_l == -1)
             break;
      }
    if(added_l == -1 && added == -1)
    {
        Configuracion_global->empresaDB.rollback();
        qDebug() <<added;
        qDebug() <<error;
    } else
    {
        QHash <QString, QVariant> p;
        if(!oPres->aprobado)
        {
            p["aprobado"] = true;
            p["fecha_aprobacion"] = QDate::currentDate();
        }
        p["pedido"] = num;
        c = "id="+QString::number(oPres->id);
        bool updated = SqlCalls::SqlUpdate(p,"cab_pre",Configuracion_global->empresaDB,c,error);

        if(updated)
        {
            Configuracion_global->empresaDB.commit();
            t = new TimedMessageBox(qApp->activeWindow(),
                                                      QObject::tr("Se ha creado el pedido num:")+QString::number(num));
            oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(oPres->id));
            LLenarCampos();
        } else
        {
            QMessageBox::warning(this,tr("Traspaso a pedidos"),tr("No se pudo crear el pedido"),tr("Aceptar"));
            Configuracion_global->empresaDB.rollback();
        }
    }

//    qDebug() << "id:" << h.value("id_cliente");

//    h.remove("columna que no esta o dxa error");
//    //o si falta
//    h.insert("la que falta",valor)
}

void FrmPresupuestosCli::convertir_ealbaran()
{
    if(QString(oPres->albaran).isEmpty())
    {
        QString c = QString("id = %1").arg(oPres->id);
        QString error;
        QHash <QString, QVariant> h;
        QMap<int, QSqlRecord> records = SqlCalls::SelectRecord("cab_pre",c,Configuracion_global->empresaDB,error);
        QSqlRecord r = records.value(oPres->id);
        for(int i= 0; i< r.count();i++)
            h.insert(r.fieldName(i),r.value(i));
        // ---------------------------------
        // Ajustes campos antes de insertar
        //----------------------------------
        h.insert("imp_gasto1",h.value("importe_gasto1").toDouble());
        h.remove("importe_gasto1");
        h.insert("imp_gasto2",h.value("importe_gasto2").toDouble());
        h.remove("importe_gasto2");
        h.insert("imp_gasto3",h.value("importe_gasto3").toDouble());
        h.remove("importe_gasto3");

        h.insert("comentario",h.value("comentarios").toString());
        h.remove("comentarios");
        h.insert("rec_total",h.value("total_recargo").toDouble());
        h.remove("total_recargo");
        h.remove("aprobado");
        h.remove("impreso");
        h.remove("fecha_aprobacion");
        h.insert("iva_total",h.value("total_iva").toDouble());
        h.remove("total_iva");
        h.remove("atencion_de");
        h.remove("importe_pendiente");
        h.insert("subtotal",h.value("importe").toDouble());
        h.remove("importe");
        h.insert("desc_gasto1",h.value("gastos_distribuidos1").toString());
        h.remove("gastos_distribuidos1");
        h.insert("desc_gasto2",h.value("gastos_distribuidos2").toString());
        h.remove("gastos_distribuidos2");
        h.insert("desc_gasto3",h.value("gastos_distribuidos3").toString());
        h.remove("gastos_distribuidos3");
        h.insert("base_total",h.value("base").toDouble());
        h.remove("base");
        h.remove("valido_hasta");
        h.insert("total_albaran",h.value("total").toDouble());
        h.remove("total");
        h.remove("lugar_entrega");
        h.remove("importe_factura");
        h.remove("presupuesto");
        h.remove("id");
        h.insert("pedido_cliente",h.value("pedido").toInt());
        h.remove("pedido");
        Albaran oAlbaran;
        Configuracion_global->empresaDB.transaction();

        //-----------------------
        // Elección de serie
        //-----------------------
        QDialog* dlg = new QDialog(this);
        dlg->setWindowTitle(tr("Seleccione serie albarán"));
        dlg->resize(170,150);
        QComboBox* box = new QComboBox(dlg);
        QPushButton*  btn = new QPushButton("Aceptar",dlg);
        QVBoxLayout lay(dlg);

        lay.addWidget(box);
        lay.addWidget(btn);

        dlg->setLayout(&lay);

        QSqlQueryModel *l = new QSqlQueryModel(this);
        l->setQuery("select serie from series order by serie",Configuracion_global->empresaDB);
        box->setModel(l);

        connect(btn,SIGNAL(clicked()),dlg,SLOT(accept()));
        dlg->exec();//aki se podria poner otro boton y cancelar todo?

        QString serie = box->currentText();
        dlg->deleteLater();



        int num = oAlbaran.NuevoNumeroAlbaran(serie);
        h.insert("albaran",num);
        h["fecha"] = QDate::currentDate();

        int added = SqlCalls::SqlInsert(h,"cab_alb",Configuracion_global->empresaDB,error);
        // ----------------
        // Lineas de albaran
        // ----------------
        int added_l;
        QHash <QString, QVariant> h_l;
        QString d = QString("id_cab = %1").arg(oPres->id);
        QMap<int, QSqlRecord> map = SqlCalls::SelectRecord("lin_pre", d,Configuracion_global->empresaDB, error);
          QMapIterator<int, QSqlRecord> i_l(map);
          while (i_l.hasNext())
          {
              i_l.next();
              QSqlRecord r_l = i_l.value();
              for(int i= 0; i< r_l.count();i++)
                  h_l.insert(r_l.fieldName(i),r_l.value(i));
              h_l.remove("id");
              h_l["id_cab"] = added;
             added_l = SqlCalls::SqlInsert(h_l,"lin_alb",Configuracion_global->empresaDB,error);
             if(added_l == -1)
                 break;
          }
        if(added_l == -1 && added == -1)
        {
            Configuracion_global->empresaDB.rollback();
            qDebug() <<added;
            qDebug() <<error;
        } else
        {
            QHash <QString, QVariant> p;
            if(!oPres->aprobado)
            {
                p["aprobado"] = true;
                p["fecha_aprobacion"] = QDate::currentDate();
            }
            p["albaran"] = num;
            c = "id="+QString::number(oPres->id);
            bool updated = SqlCalls::SqlUpdate(p,"cab_pre",Configuracion_global->empresaDB,c,error);

            if(updated)
            {
                Configuracion_global->empresaDB.commit();
                t = new TimedMessageBox(qApp->activeWindow(),
                                                          QObject::tr("Se ha creado el albarán num:")+QString::number(num));
                oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(oPres->id));
                LLenarCampos();
            } else
            {
                QMessageBox::warning(this,tr("Traspaso a albarán"),tr("No se pudo crear el albarán"),tr("Aceptar"));
                Configuracion_global->empresaDB.rollback();
            }
        }
    } else
        QMessageBox::information(this,tr("Traspasar presupuesto"),tr("Este presupuesto ya ha sido traspasado\nNo puede traspasar dos veces el mismo presupuesto"),
                                 tr("Aceptar"));
}

void FrmPresupuestosCli::convertir_enFactura()
{
    if(QString(oPres->factura).isEmpty()|| oPres->factura == "0")
    {
        QString c = QString("id = %1").arg(oPres->id);
        QString error;
        QHash <QString, QVariant> h;
        QMap<int, QSqlRecord> records = SqlCalls::SelectRecord("cab_pre",c,Configuracion_global->empresaDB,error);
        QSqlRecord r = records.value(oPres->id);
        for(int i= 0; i< r.count();i++)
            h.insert(r.fieldName(i),r.value(i));
        // ---------------------------------
        // Ajustes campos antes de insertar
        //----------------------------------
        h.insert("imp_gasto1",h.value("importe_gasto1").toDouble());
        h.remove("importe_gasto1");
        h.insert("imp_gasto2",h.value("importe_gasto2").toDouble());
        h.remove("importe_gasto2");
        h.insert("imp_gasto3",h.value("importe_gasto3").toDouble());
        h.remove("importe_gasto3");

        h.insert("comentario",h.value("comentarios").toString());
        h.remove("comentarios");
        h.remove("aprobado");
        h.remove("impreso");
        h.remove("fecha_aprobacion");
        h.insert("iva",h.value("total_iva").toDouble());
        h.remove("total_iva");
        h.remove("atencion_de");
        h.remove("importe_pendiente");
        h.insert("subtotal",h.value("importe").toDouble());
        h.remove("importe");
        h.insert("desc_gasto1",h.value("gastos_distribuidos1").toString());
        h.remove("gastos_distribuidos1");
        h.insert("desc_gasto2",h.value("gastos_distribuidos2").toString());
        h.remove("gastos_distribuidos2");
        h.insert("desc_gasto3",h.value("gastos_distribuidos3").toString());
        h.remove("gastos_distribuidos3");
        h.remove("valido_hasta");
        h.remove("lugar_entrega");
        h.remove("importe_factura");
        h.remove("presupuesto");
        h.remove("id");
        h.insert("pedido_cliente",h.value("pedido").toInt());
        h.remove("pedido");
        h.remove("albaran");
       // h["total"] == ui->txttotal->text().replace(".","").replace(",",".").toDouble();
        Factura oFactura;
        Configuracion_global->empresaDB.transaction();

        QDialog* dlg = new QDialog(this);
        dlg->setWindowTitle(tr("Seleccione serie factura"));
        dlg->resize(270,150);
        QComboBox* box = new QComboBox(dlg);
        QPushButton*  btn = new QPushButton("Aceptar",dlg);
        QVBoxLayout lay(dlg);

        lay.addWidget(box);
        lay.addWidget(btn);

        dlg->setLayout(&lay);

        QSqlQueryModel *l = new QSqlQueryModel(this);
        l->setQuery("select serie from series",Configuracion_global->empresaDB);
        box->setModel(l);

        connect(btn,SIGNAL(clicked()),dlg,SLOT(accept()));
        dlg->exec();//aki se podria poner otro boton y cancelar todo?

        QString serie = box->currentText();
        dlg->deleteLater();
        QString fra =  oFactura.NuevoNumeroFactura(serie);
        QString num = serie+"/"+ fra;
        h.insert("factura",fra);
        h["fecha"] = QDate::currentDate();
        h["serie"] = serie;

        int added = SqlCalls::SqlInsert(h,"cab_fac",Configuracion_global->empresaDB,error);
        if(added>-1)
        {
            // -----------------
            // Lineas de factura
            // -----------------
            int added_l;
            QHash <QString, QVariant> h_l;
            QString d = QString("id_cab = %1").arg(oPres->id);
            QMap<int, QSqlRecord> map = SqlCalls::SelectRecord("lin_pre", d,Configuracion_global->empresaDB, error);
              QMapIterator<int, QSqlRecord> i_l(map);
              while (i_l.hasNext())
              {
                  i_l.next();
                  QSqlRecord r_l = i_l.value();
                  for(int i= 0; i< r_l.count();i++)
                      h_l.insert(r_l.fieldName(i),r_l.value(i));
                  h_l.remove("id");
                  h_l["id_cab"] = added;
                 added_l = SqlCalls::SqlInsert(h_l,"lin_fac",Configuracion_global->empresaDB,error);
                 if(added_l == -1)
                     break;
              }
            if(added_l == -1)
            {
                Configuracion_global->empresaDB.rollback();
                qDebug() <<added;
                qDebug() <<error;
            } else
            {
                QHash <QString, QVariant> p;
                if(!oPres->aprobado)
                {
                    p["aprobado"] = true;
                    p["fecha_aprobacion"] = QDate::currentDate();
                }
                p["factura"] = num;
                p["importe_factura"] = h.value("total").toDouble();
                c = "id="+QString::number(oPres->id);
                bool updated = SqlCalls::SqlUpdate(p,"cab_pre",Configuracion_global->empresaDB,c,error);

                if(updated)
                {
                    Configuracion_global->empresaDB.commit();
                    t = new TimedMessageBox(qApp->activeWindow(),
                                                              QObject::tr("Se ha creado la factura num:")+
                                            Configuracion_global->serie+"/"+num);
                    oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(oPres->id));
                    LLenarCampos();
                } else
                {
                    QMessageBox::warning(this,tr("Traspaso a factura"),tr("No se pudo crear la factura"),tr("Aceptar"));
                    Configuracion_global->empresaDB.rollback();
                }
            }
        } else
        {
            QMessageBox::warning(this,tr("Traspaso a factura"),
                               QString(tr("No se pudo crear la factura: %1").arg(error)),tr("Aceptar"));
            Configuracion_global->empresaDB.rollback();
        }

    } else
        QMessageBox::information(this,tr("Traspasar presupuesto"),tr("Este presupuesto ya ha sido traspasado\nNo puede traspasar dos veces el mismo presupuesto"),
                                 tr("Aceptar"));
}

void FrmPresupuestosCli::on_btnBorrar_clicked()
{
    if (QMessageBox::question(this,tr("Borrar"),
                              tr("Esta acción no se puede deshacer.\n¿Desea continuar?"),
                              tr("Cancelar"),
                               tr("&Continuar"))== QMessageBox::Accepted)
    {
        bool succes = true;
        Configuracion_global->empresaDB.transaction();

        QSqlQuery q(Configuracion_global->empresaDB);
        succes &= oPres->BorrarLineas(oPres->id);
        q.prepare("DELETE FROM cab_pre WHERE presupuesto = "+ui->txtpresupuesto->text());
        succes &= q.exec();        

        if(succes)
            succes &= Configuracion_global->empresaDB.commit();

        if(succes)
        {
            TimedMessageBox * t = new TimedMessageBox(this,tr("Borrado con exito"));
            VaciarCampos();
            oPres->id = -1;
            on_btnSiguiente_clicked();
        }
        else
            Configuracion_global->empresaDB.rollback();
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
    Configuracion_global->empresaDB.transaction();
    Configuracion_global->groupDB.transaction();

    //Nueva linea
    if (ld->idLinea == -1)
    {
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;
        QSqlQuery lin_pre(Configuracion_global->empresaDB);
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
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        } else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
        }

        ld->idLinea = lin_pre.lastInsertId().toInt();

    } else // Editando linea
    {
        //TODO control de stock editando en pres clientes
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery lin_pre(Configuracion_global->empresaDB);
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
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        } else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
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
    Configuracion_global->empresaDB.transaction();
    Configuracion_global->groupDB.transaction();
    if(ld->idLinea >-1)
    {
        //TODO control de stock
        QSqlQuery q(Configuracion_global->empresaDB);
        q.prepare("delete from lin_pre where id =:id");
        q.bindValue(":id",ld->idLinea);
        if(q.exec() && ok_Maya)
        {
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        } else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
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
    Configuracion_global->groupDB.rollback();
    Configuracion_global->empresaDB.rollback();
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

void FrmPresupuestosCli::on_btnImprimir_clicked()
{
    FrmDialogoImprimir dlg_print(this);
    dlg_print.set_email(oClientePres->email);
    dlg_print.set_preview(false);
    if(dlg_print.exec() == dlg_print.Accepted)
    {
        bool ok  /* = oFactura->set_impresa(true)*/;
        if(ok)
        {
            ui->lbimpreso->setVisible(true);
            int valor = dlg_print.get_option();
            QMap <QString,QString> parametros;
            parametros["Empresa.cab_pre"] = QString("id = %1").arg(oPres->id);
            parametros["Empresa.lin_pre"] = QString("id_cab = %1").arg(oPres->id);
            QString report = "presupuesto_"+QString::number(oClientePres->ididioma);
            switch (valor) {
            case 1: // Impresora
                Configuracion::ImprimirDirecto(report,parametros);
                break;
            case 2: // email
                // TODO - enviar pdf por mail
                break;
            case 3: // PDF
                Configuracion::ImprimirPDF(report,parametros);
                break;
            case 4: //preview
                Configuracion::ImprimirPreview(report,parametros);
                break;
            default:
                break;
            }

        }
    }
}



void FrmPresupuestosCli::on_radBusqueda_toggled(bool checked)
{
    if(checked)
        ui->stackedWidget->setCurrentIndex(1);
    else
        ui->stackedWidget->setCurrentIndex(0);
}



void FrmPresupuestosCli::formato_tabla()
{
    QStringList headers;
    QVariantList sizes;
    headers << "id" << tr("presupuesto") << tr("fecha") <<tr("teléfono") <<tr("movil")  <<tr("cliente");
    sizes << 0 << 120 << 120 << 120 << 120 << 550;
    for(int i = 0; i < headers.length();i++)
    {
        ui->tabla->setColumnWidth(i,sizes.at(i).toInt());
        m->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
}

void FrmPresupuestosCli::filter_table()
{

    QHash <QString,QString> h;
    h[tr("presupuesto")] = "presupuesto";
    h[tr("cliente")] = "cliente";
    h[tr("teléfono")] = "telefono";
    QString arg1 = ui->txtBuscar->text();
    QString orden = h.value(ui->cboOrden->currentText());
    QString modo;
    if(ui->cboModo->currentText() == tr("A-Z"))
        modo = "";
    else
        modo = "Desc";

    m->setQuery("select id,presupuesto,fecha,telefono,movil,cliente from cab_pre where "+orden+
                " like '%"+arg1+"%' order by "+orden+" "+modo, Configuracion_global->empresaDB);
}

void FrmPresupuestosCli::on_txtBuscar_textEdited(const QString &arg1)
{
    filter_table();
}

void FrmPresupuestosCli::on_tabla_clicked(const QModelIndex &index)
{
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tabla->model());
    int id = Configuracion_global->devolver_id_tabla(model,index);
    oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(id));
    LLenarCampos();

}

void FrmPresupuestosCli::on_tabla_doubleClicked(const QModelIndex &index)
{
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tabla->model());
    int id = Configuracion_global->devolver_id_tabla(model,index);
    oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(id));
    LLenarCampos();
    ui->radEdicion->setChecked(true);
    BloquearCampos(true);
}

void FrmPresupuestosCli::on_btn_convertir_clicked()
{

}

void FrmPresupuestosCli::on_cboOrden_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    filter_table();
}

void FrmPresupuestosCli::on_cboModo_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    filter_table();
}
