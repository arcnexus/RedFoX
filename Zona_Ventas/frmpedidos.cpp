#include "frmpedidos.h"
#include "ui_frmpedidos.h"


#include "../Almacen/articulo.h"
#include "../Zona_Ventas/albaran.h"

#include "../Busquedas/db_consulta_view.h"
#include "../Zona_Ventas/factura.h"
#include"../Auxiliares/datedelegate.h"
#include"../Auxiliares/monetarydelegate.h"


FrmPedidos::FrmPedidos(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmPedidos),
    helper(this),
    toolButton(tr("Pedidos"),":/Icons/PNG/pedidos_cli.png",this),
    menuButton(QIcon(":/Icons/PNG/pedidos_cli.png"),tr("Pedidos"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/pedidos_cli.png"),"",this))
{
    ui->setupUi(this);
    ui->combo_pais->setModel(Configuracion_global->paises_model);
    ui->combo_pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));
    // Pongo valores por defecto
    ui->lbfacturado->setVisible(false);
    ui->lbimpreso->setVisible(false);
    ui->lblNumFactura->setVisible(false);
    ui->txtcNumFra->setVisible(false);
    ui->lblAlbaran->setVisible(false);
    ui->txtalbaran->setVisible(false);
    ui->txtfecha_factura->setVisible(false);

    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de pedidos de clientes"));


    oPedido = new Pedidos();
    oCliente3 = new Cliente();

    helper.set_Tipo(false);
    helper.help_table(ui->Lineas);
    helper.set_tarifa(0);

    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(ui->chklporc_rec,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

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

    helper.set_tarifa(Configuracion_global->id_tarifa_predeterminada);

    aAlbaran_action = new QAction(tr("En albaran"),this);
    aFactura_action = new QAction(tr("En factura"),this);

    connect(aAlbaran_action,SIGNAL(triggered()),this,SLOT(convertir_ealbaran()));
    connect(aFactura_action,SIGNAL(triggered()),this,SLOT(convertir_enFactura()));

    convertir_menu = new QMenu(this);
    convertir_menu->addAction(aAlbaran_action);
    convertir_menu->addAction(aFactura_action);

    ui->btn_convertir->setMenu(convertir_menu);
    BloquearCampos(true);

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));

    //--------------
    // LLenar tabla
    //--------------
    m = new QSqlQueryModel(this);
    m->setQuery("select id, pedido, fecha,total_pedido, cliente from ped_cli order by pedido desc",Configuracion_global->empresaDB);
    ui->tabla->setModel(m);
    formato_tabla();

    //-------------
    // ordenar por
    //-------------
    QStringList order;
    order << tr("Pedido") <<tr("Cliente") <<tr("Fecha");
    ui->cboOrden->addItems(order);

    //-------------
    // MODO
    //-------------
    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    ui->cboModo->addItems(modo);

    //--------------------
    // %Iva combos gastos
    //--------------------
    QSqlQueryModel *iva = new QSqlQueryModel(this);
    iva->setQuery("select iva from tiposiva",Configuracion_global->groupDB);
    ui->cboporc_iva_gasto1->setModel(iva);
    int index = ui->cboporc_iva_gasto1->findText(Configuracion_global->ivaList.at(0));
    ui->cboporc_iva_gasto1->setCurrentIndex(index);
    ui->cboporc_iva_gasto2->setModel(iva);
    ui->cboporc_iva_gasto3->setModel(iva);

    //-------------------
    // Modo Busqueda
    //-------------------
    ui->radBusqueda->setChecked(true);
    ui->stackedWidget->setCurrentIndex(1);
}

FrmPedidos::~FrmPedidos()
{
    delete ui;
    delete oPedido;
    delete oCliente3;
}

void FrmPedidos::LLenarCampos()
{
    ui->lblTopcliente->setText(oPedido->cliente);
    ui->lblToppedido->setText(QString::number(oPedido->pedido));
    ui->txtalbaran->setText(QString::number(oPedido->albaran));
    ui->txtpedido->setText(QString::number(oPedido->pedido));
    ui->txtfecha->setDate(oPedido->fecha);
    //oPedido->pedido;
    //oPedido->id_cliente;
    ui->txtcodigo_cliente->setText(oPedido->codigo_cliente);
    ui->txtcliente->setText(oPedido->cliente);
    ui->txtdireccion1->setText(oPedido->direccion1);
    ui->txtdireccion2->setText(oPedido->direccion2);
    ui->txtpoblacion->setText(oPedido->poblacion);
    ui->txtprovincia->setText(oPedido->provincia);
    ui->txtcp->setText(oPedido->cp);

    QList<QString> keys = Configuracion_global->paises.uniqueKeys();
    for (int i = 0;i<keys.size();i++)
    {
        if(Configuracion_global->paises[keys.at(i)].value("id").toInt() == oPedido->id_pais)
        {
            int index = ui->combo_pais->findText(keys.at(i));
            ui->combo_pais->setCurrentIndex(index);
        }
    }

    ui->txtcif->setText(oPedido->cif);
    ui->chklporc_rec->setChecked(oPedido->recargo_equivalencia==1);
        helper.set_tarifa(oCliente3->tarifa_cliente);
    ui->txtsubtotal->setText(QString::number(oPedido->subtotal));
    ui->txtdto->setText(QString::number(oPedido->dto));
    //oPedido->dto;
    ui->txtbase1->setText(QString::number(oPedido->base1));
    ui->txtbase2->setText(QString::number(oPedido->base2));
    ui->txtbase3->setText(QString::number(oPedido->base3));
    ui->txtbase4->setText(QString::number(oPedido->base4));
    //ui->txtporc_iva1->setText(QString::number(oPedido->porc_iva1));
    //ui->txtporc_iva2->setText(QString::number(oPedido->porc_iva2));
    //ui->txtporc_iva3->setText(QString::number(oPedido->porc_iva3));
    //ui->txtporc_iva4->setText(QString::number(oPedido->porc_iva4));
    ui->txtiva1->setText(QString::number(oPedido->iva1));
    ui->txtiva2->setText(QString::number(oPedido->iva2));
    ui->txtiva3->setText(QString::number(oPedido->iva3));
    ui->txtiva4->setText(QString::number(oPedido->iva4));
    //ui->txtporc_rec1->setText(QString::number(oPedido->porc_rec1));
    //ui->txtporc_rec2->setText(QString::number(oPedido->porc_rec2));
    //ui->txtporc_rec3->setText(QString::number(oPedido->porc_rec3));
    //ui->txtporc_rec4->setText(QString::number(oPedido->porc_rec4));
    ui->txtporc_rec1->setText(QString::number(oPedido->rec1));
    ui->txtporc_rec2->setText(QString::number(oPedido->rec2));
    ui->txtporc_rec3->setText(QString::number(oPedido->rec3));
    ui->txtporc_rec4->setText(QString::number(oPedido->rec4));
    ui->txttotal1->setText(QString::number(oPedido->total1));
    ui->txttotal2->setText(QString::number(oPedido->total2));
    ui->txttotal3->setText(QString::number(oPedido->total3));
    ui->txttotal4->setText(QString::number(oPedido->total4));
    ui->txtbase_total_2->setText(QString::number(oPedido->base_total));
    ui->txttotal_iva_2->setText(QString::number(oPedido->iva_total));
    ui->txttotal_recargo_2->setText(QString::number(oPedido->rec_total));
    ui->txttotal_2->setText(QString::number(oPedido->total_albaran));
    ui->txtGastoDist1->setText(oPedido->gasto1);
    ui->txtGastoDist2->setText(oPedido->gasto2);
    ui->txtGastoDist3->setText(oPedido->gasto3);
    ui->SpinGastoDist1->setValue(oPedido->imp_gasto1);
    ui->SpinGastoDist2->setValue(oPedido->imp_gasto2);
    ui->SpinGastoDist3->setValue(oPedido->imp_gasto3);
    ui->lbimpreso->setVisible(oPedido->impreso == 1);

    ui->lbfacturado->setVisible(oPedido->facturado == 1);
    ui->txtfecha_factura->setVisible(oPedido->facturado == 1);
    ui->txtcNumFra->setVisible(oPedido->facturado == 1);
    ui->txtcNumFra->setText(oPedido->factura);
    ui->txtfecha_factura->setDate(oPedido->fecha_factura);

    ui->txtcomentario->setText(oPedido->comentario);
    ui->txtentregado_a_cuenta->setText(QString::number(oPedido->entregado_a_cuenta));
    //oPedido->traspasado_albaran;
    //oPedido->traspasado_factura;
    ui->txtdireccion_entrega1->setText(oPedido->direccion_entrega1);
    ui->txtdireccion_entrega1_2->setText(oPedido->direccion_entrega2);
    ui->txtcp_entrega->setText(oPedido->cp_entrega);
    ui->txtpoblacion_entrega->setText(oPedido->poblacion_entrega);
    ui->txtprovincia_entrega->setText(oPedido->provincia_entrega);
    ui->txtpais_entrega->setText(oPedido->pais_entrega);
    ui->chkenviado->setChecked(oPedido->enviado==1);
    ui->chkcompleto->setChecked(oPedido->completo==1);
    ui->chkentregado->setChecked(oPedido->entregado==1);
    ui->txtfecha_limite_entrega->setDate(oPedido->fecha_limite_entrega);
    ui->txttotal->setText(QString::number(oPedido->total_pedido));
    oCliente3->Recuperar("Select * from clientes where id ="+QString::number(oPedido->id_cliente));
    helper.set_tarifa(oPedido->tarifa_cliente);
    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();
    QString filter = QString("id_Cab = '%1'").arg(oPedido->id);
    helper.fillTable("empresa","lin_ped",filter);
    helper.set_tipo_dto_tarifa(oCliente3->tipo_dto_tarifa);
}

void FrmPedidos::LLenarCamposCliente()
{
    oPedido->id_cliente = oCliente3->id;
    ui->lblTopcliente->setText(oCliente3->nombre_fiscal);
    ui->txtcodigo_cliente->setText(oCliente3->codigo_cliente);
    ui->txtcliente->setText(oCliente3->nombre_fiscal);
    ui->txtdireccion1->setText(oCliente3->direccion1);
    ui->txtdireccion2->setText(oCliente3->direccion2);
    ui->txtcp->setText(oCliente3->cp);
    ui->txtpoblacion->setText(oCliente3->poblacion);
    ui->txtprovincia->setText(oCliente3->provincia);
    //ui->txtpais->setText(oCliente3->pais);
    ui->txtcif->setText(oCliente3->cif_nif);
    helper.set_tarifa(oCliente3->tarifa_cliente);
    if (oCliente3->lIRPF==1) {
        ui->chklporc_rec->setChecked(true);
        oPedido->recargo_equivalencia = (1);
    } else {
        ui->chklporc_rec->setChecked(false);
        oPedido->recargo_equivalencia = (0);
    }
    oCliente3->Recuperar("Select * from clientes where id ="+QString::number(oPedido->id_cliente));
    helper.set_tarifa(oPedido->tarifa_cliente);
    helper.set_tipo_dto_tarifa(oCliente3->tipo_dto_tarifa);
}

void FrmPedidos::VaciarCampos()
{
    ui->lblTopcliente->setText("");
    ui->lblToppedido->setText("");
    QDate fecha;
    ui->txtcodigo_cliente->setText("");
    ui->txtpedido->setText("");
    ui->txtfecha->setDate(fecha.currentDate());
    ui->txtfecha_factura->setDate(fecha.currentDate());
    ui->txtcliente->setText("");
    ui->txtdireccion1->setText("");
    ui->txtdireccion2->setText("");
    ui->txtcp->setText("");
    ui->txtpoblacion->setText("");
    ui->txtprovincia->setText("");
    //ui->txtpais->setText("");
    ui->txtcif->setText("");
    ui->txtsubtotal->setText(0);
    ui->txtdto->setText("0,00");
    ui->txtbase->setText("0,00");
    ui->txtiva->setText("0,00");
    ui->txttotal->setText("0,00");
    ui->lbimpreso->setVisible(false);
    ui->lbfacturado->setVisible(false);
    ui->lblNumFactura->setVisible(false);
    ui->txtcNumFra->setVisible(false);
    ui->txtcomentario->setText("");
    ui->txtbase1->setText(0);
    ui->txtbase2->setText(0);
    ui->txtbase3->setText(0);
    ui->txtbase4->setText(0);
    //Configuracion_global->CargarDatos();
    //QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    //ui->txtporc_iva1->setText(QString::number(Configuracion_global->ivas[keys.at(0)].value("iva").toDouble()));
    //ui->txtporc_iva2->setText(QString::number(Configuracion_global->ivas[keys.at(1)].value("iva").toDouble()));
    //ui->txtporc_iva3->setText(QString::number(Configuracion_global->ivas[keys.at(2)].value("iva").toDouble()));
    //ui->txtporc_iva4->setText(QString::number(Configuracion_global->ivas[keys.at(3)].value("iva").toDouble()));
    //ui->txtporc_rec1->clear();
    //ui->txtporc_rec2->clear();
    //ui->txtporc_rec3->clear();
    //ui->txtporc_rec4->clear();
    ui->txtiva1->setText(0);
    ui->txtiva2->setText(0);
    ui->txtiva3->setText(0);
    ui->txtiva4->setText(0);
    ui->txttotal1->setText(0);
    ui->txttotal2->setText(0);
    ui->txttotal3->setText(0);
    ui->txttotal4->setText(0);
    ui->txtporc_rec1->setText(0);
    ui->txtporc_rec2->setText(0);
    ui->txtporc_rec3->setText(0);
    ui->txtporc_rec4->setText(0);
    ui->txttotal_recargo_2->setText(0);
    ui->txtrec->setText("0,00");
    ui->txtentregado_a_cuenta->setText("0,00");
    ui->txtpedido_cliente->setText("");
    ui->txttotal_iva_2->setText("0,00");
    ui->txtbase_total_2->setText("0,00");
    ui->txttotal_recargo_2->setText("0,00");
    ui->txttotal_2->setText("0,00");
    ui->txtsubtotal->setText("0,00");
    ui->txtpedido->setReadOnly(true);
    ui->txtGastoDist1->clear();
    ui->txtGastoDist2->clear();
    ui->txtGastoDist3->clear();
    ui->SpinGastoDist1->clear();
    ui->SpinGastoDist2->clear();
    ui->SpinGastoDist3->clear();

    helper.fillTable("empresa","lin_ped","id_Cab = -1");
}

void FrmPedidos::BloquearCampos(bool state)
{
    helper.blockTable(state);
    ui->btnAnadirLinea->setEnabled(!state);
    ui->btn_borrarLinea->setEnabled(!state);
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
    ui->btnBuscar->setEnabled(state);
    ui->btnDeshacer->setEnabled(!state);
    ui->btnEditar->setEnabled(state);    
    ui->btnGuardar->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->botBuscarCliente->setEnabled(!state);
    ui->txtpedido->setReadOnly(true);
    ui->btn_borrar->setEnabled(state);
    ui->txtBuscar->setReadOnly(!state);
    ui->cboOrden->setEnabled(state);
    ui->txtBuscar->setFocus();
    ui->cboModo->setEnabled(state);
}

void FrmPedidos::LLenarPedido()
{
    oPedido->albaran= ui->txtalbaran->text().toInt();
    oPedido->pedido=ui->txtpedido->text().toInt();
    oPedido->fecha=ui->txtfecha->date();
    //oPedido->pedido;
    //oPedido->id_cliente;
    oPedido->codigo_cliente=ui->txtcodigo_cliente->text();
    oPedido->cliente=ui->txtcliente->text();
    oPedido->direccion1=ui->txtdireccion1->text();
    oPedido->direccion2=ui->txtdireccion2->text();
    oPedido->poblacion=ui->txtpoblacion->text();
    oPedido->provincia=ui->txtprovincia->text();
    oPedido->cp=ui->txtcp->text();


    oPedido->id_pais = Configuracion_global->paises[ui->combo_pais->currentText()].value("id").toInt();

    int i = ui->combo_pais->findText(oPedido->pais);
    ui->combo_pais->setCurrentIndex(i);

    oPedido->cif=ui->txtcif->text();
    oPedido->recargo_equivalencia=ui->chklporc_rec->isChecked();
    oPedido->subtotal=ui->txtsubtotal->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->dto=ui->txtdto->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    //oPedido->dto;
    oPedido->base1=ui->txtbase1->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->base2=ui->txtbase2->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->base3=ui->txtbase3->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->base4=ui->txtbase4->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->porc_iva1=ui->txtporc_iva1->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();

    oPedido->porc_iva2=ui->txtporc_iva2->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->porc_iva3=ui->txtporc_iva3->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->porc_iva4=ui->txtporc_iva4->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->iva1=ui->txtiva1->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->iva2=ui->txtiva2->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->iva3=ui->txtiva3->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->iva4=ui->txtiva4->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->porc_rec1=ui->txtporc_rec1->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->porc_rec2=ui->txtporc_rec2->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->porc_rec3=ui->txtporc_rec3->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->porc_rec4=ui->txtporc_rec4->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->rec1=ui->txtporc_rec1->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->rec2=ui->txtporc_rec2->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->rec3=ui->txtporc_rec3->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->rec4=ui->txtporc_rec4->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->total1=ui->txttotal1->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->total2=ui->txttotal2->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->total3=ui->txttotal3->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->total4=ui->txttotal4->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->base_total=ui->txtbase_total_2->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->iva_total=ui->txttotal_iva_2->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->rec_total=ui->txttotal_recargo_2->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->total_albaran=ui->txttotal_2->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();

    oPedido->impreso = ui->lbimpreso->isVisible();

   // oPedido->factura=ui->txtcNumFra->text().toDouble();
    oPedido->fecha_factura=ui->txtfecha_factura->date();

    oPedido->comentario=ui->txtcomentario->toPlainText();
    oPedido->entregado_a_cuenta=ui->txtentregado_a_cuenta->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    //oPedido->traspasado_albaran;
    //oPedido->traspasado_factura;
    oPedido->direccion_entrega1=ui->txtdireccion_entrega1->text();
    oPedido->direccion_entrega2=ui->txtdireccion_entrega1_2->text();
    oPedido->cp_entrega=ui->txtcp_entrega->text();
    oPedido->poblacion_entrega=ui->txtpoblacion_entrega->text();
    oPedido->provincia_entrega=ui->txtprovincia_entrega->text();
    oPedido->pais_entrega=ui->txtpais_entrega->text();
    oPedido->enviado=ui->chkenviado->isChecked();
    oPedido->completo=ui->chkcompleto->isChecked();
    oPedido->entregado=ui->chkentregado->isChecked();
    oPedido->fecha_limite_entrega=ui->txtfecha_limite_entrega->date();
    oPedido->total_pedido=ui->txttotal->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->gasto1 = ui->txtGastoDist1->text();
    oPedido->gasto2 = ui->txtGastoDist2->text();
    oPedido->gasto3 = ui->txtGastoDist3->text();
    oPedido->imp_gasto1 = ui->SpinGastoDist1->value();
    oPedido->imp_gasto2 = ui->SpinGastoDist2->value();
    oPedido->imp_gasto3 = ui->SpinGastoDist3->value();
}

void FrmPedidos::formato_tabla()
{
    // id, pedido, fecha, cliente
    QStringList headers;
    QVariantList size;
    headers << "id" <<tr("Pedido") << tr("fecha") <<tr("Total") <<tr("cliente");
    size <<0 <<120 <<120 <<120 <<300;
    for(int i = 0;i<headers.length();i++)
    {
        ui->tabla->setColumnWidth(i,size.at(i).toInt());
        m->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
    ui->tabla->setItemDelegateForColumn(2,new DateDelegate);
    ui->tabla->setItemDelegateForColumn(3, new MonetaryDelegate);
}

void FrmPedidos::filter_table()
{
    //  tr("Pedido") <<tr("Cliente") <<tr("Fecha")
    QHash <QString,QString> h;
    h[tr("Pedido")] = "pedido";
    h[tr("Cliente")] = "cliente";
    h[tr("Fecha")] = "fecha";
    h[tr("Total")] = "total_pedido";
    QString order = h.value(ui->cboOrden->currentText());
    QString arg1 = ui->txtBuscar->text();
    QString modo;
    if (ui->cboModo->currentText() == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    m->setQuery("select id, pedido, fecha,total_pedido, cliente from ped_cli "
                    "where "+order+" like '%" + arg1.trimmed() + "%' order by "+order+" "+modo,Configuracion_global->empresaDB);

}

void FrmPedidos::calcular_iva_gastos()
{
    ui->spiniva_gasto1->setValue((ui->SpinGastoDist1->value()*(ui->cboporc_iva_gasto1->currentText().toDouble()/100)));
    ui->spiniva_gasto2->setValue((ui->SpinGastoDist2->value()*(ui->cboporc_iva_gasto2->currentText().toDouble()/100)));
    ui->spiniva_gasto3->setValue((ui->SpinGastoDist3->value()*(ui->cboporc_iva_gasto3->currentText().toDouble()/100)));
}

void FrmPedidos::on_btnSiguiente_clicked()
{
    int pedido = oPedido->id;
    if(oPedido->RecuperarPedido("Select * from ped_cli where id >'"+QString::number(pedido)+"' order by pedido  limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oPedido->id);
        helper.fillTable("empresa","lin_ped",filter);
        ui->btn_borrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
        ui->btn_convertir->setEnabled(true);
        ui->btnAnterior->setEnabled(true);
        ui->btnImprimir->setEnabled(true);
    }
    else
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Se ha llegado al final del archivo.\n"
                                                          "No hay mas presupuestos disponibles"));
        VaciarCampos();
        ui->btn_borrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btn_convertir->setEnabled(false);
        ui->btnSiguiente->setEnabled(false);
        oPedido->id++;
    }
}

void FrmPedidos::on_btnAnterior_clicked()
{
    int pedido = oPedido->id;
    if(oPedido->RecuperarPedido("Select * from ped_cli where id <'"+QString::number(pedido)+"' order by pedido desc limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oPedido->id);
        helper.fillTable("empresa","lin_ped",filter);
        ui->btn_borrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
        ui->btn_convertir->setEnabled(true);
        ui->btnSiguiente->setEnabled(true);
    }
    else
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Se ha llegado al final del archivo.\n"
                                                          "No hay mas presupuestos disponibles"));
        VaciarCampos();
        ui->btn_borrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btn_convertir->setEnabled(false);
        ui->btnAnterior->setEnabled(false);
        oPedido->id = -1;
    }
}

void FrmPedidos::on_btnAnadir_clicked()
{
    VaciarCampos();
    BloquearCampos(false);
    ui->txtpedido->setText(QString::number(oPedido->NuevoNumeroPedido()));
    ui->lblToppedido->setText(QString::number(oPedido->NuevoNumeroPedido()));
    LLenarPedido();
    oPedido->AnadirPedido();
    ui->txtcodigo_cliente->setFocus();
    editando = false;
    emit block();
}



void FrmPedidos::on_btnEditar_clicked()
{
    if (!oPedido->facturado)
    {
        BloquearCampos(false);
        editando = true;
        emit block();
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Editar Pedido"),tr("No se puede editar un pedido que ha sido facturado, solo los NO facturados se pueden editar")+
                             tr("<p><b> Si necesita modificar algo genere una factura nueva y realice el abono correspondiente</b>")+
                                tr("y luego si es preciso realice un nuevo pedido y facture de nuevo "),tr("OK"));
    }
}

void FrmPedidos::on_btnGuardar_clicked()
{
    LLenarPedido();
    Configuracion_global->empresaDB.transaction();
    bool succes = true;

    succes &= oPedido->GuardarPedido(oPedido->id);

    if(succes)
    {
        succes = Configuracion_global->empresaDB.commit();
    }
    if(succes)
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Pedido guardado con éxito"));
        BloquearCampos(true);
        emit unblock();
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),
                              tr("Error al guardar el pedido.\n")+Configuracion_global->empresaDB.lastError().text(),
                              tr("&Aceptar"));
        Configuracion_global->empresaDB.rollback();
    }
}

void FrmPedidos::on_botBuscarCliente_clicked()
{
    db_consulta_view consulta;
    QStringList campos;
    campos <<"nombre_fiscal" <<"codigo_cliente" << "cif_nif"<< "poblacion" << "telefono1";
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
        oCliente3->Recuperar("select * from clientes where id="+QString::number(id));
        LLenarCamposCliente();
    }
}

void FrmPedidos::on_btnDeshacer_clicked()
{
    BloquearCampos(true);
    QString cid = (ui->txtpedido->text());
    oPedido->RecuperarPedido("Select * from ped_cli where id ="+cid+" order by id limit 1 ");
    LLenarCampos();
    QString filter = QString("id_Cab = '%1'").arg(ui->txtpedido->text());
    helper.fillTable("empresa","lin_ped",filter);
    emit unblock();
}

void FrmPedidos::on_btn_borrar_clicked()
{
    if (QMessageBox::question(this,tr("Borrar"),
                              tr("Esta acción no se puede deshacer.\n¿Desea continuar?"),
                              tr("Cancelar"),
                               tr("&Continuar"))== QMessageBox::Accepted)
    {
        bool succes = true;
        Configuracion_global->empresaDB.transaction();

        QSqlQuery q(Configuracion_global->empresaDB);
        succes &= oPedido->BorrarLineas(oPedido->id);
        q.prepare("DELETE FROM ped_cli WHERE id = "+QString::number(oPedido->id));
        succes &= q.exec();

        //TODO control de stock
        if(succes)
            succes &= Configuracion_global->empresaDB.commit();

        if(succes)
        {
            TimedMessageBox * t = new TimedMessageBox(this,tr("Pedido borrado con éxito"));
            VaciarCampos();
            oPedido->id=-1;
            on_btnSiguiente_clicked();
        }
        else
            Configuracion_global->empresaDB.rollback();
    }
}

void FrmPedidos::totalChanged(double base , double dto ,double subtotal , double iva, double re, double total, QString moneda)
{
    _moneda = moneda;
    base +=(ui->SpinGastoDist1->value() + ui->SpinGastoDist2->value() + ui->SpinGastoDist3->value());
    calcular_iva_gastos();
    iva += (ui->spiniva_gasto1->value() + ui->spiniva_gasto2->value() + ui->spiniva_gasto3->value());
    total = base + iva + re;

    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales))+moneda);
    ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales_campos_totales))+moneda);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales))+moneda);
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales))+moneda);
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales))+moneda);
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales_campos_totales))+moneda);

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales))+moneda);
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales))+moneda);
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales))+moneda);
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales_campos_totales))+moneda);
}

void FrmPedidos::desglose1Changed(double base, double iva, double re, double total)
{
    if(ui->cboporc_iva_gasto1->currentText() == QString::number(oPedido->porc_iva1))
        base +=ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText() == QString::number(oPedido->porc_iva1))
        base +=ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText() == QString::number(oPedido->porc_iva1))
        base +=ui->SpinGastoDist3->value();
    total = base + iva + re;
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPedidos::desglose2Changed(double base, double iva, double re, double total)
{
    if(ui->cboporc_iva_gasto1->currentText() == QString::number(oPedido->porc_iva2))
        base +=ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText() == QString::number(oPedido->porc_iva2))
        base +=ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText() == QString::number(oPedido->porc_iva2))
        base +=ui->SpinGastoDist3->value();
    total = base + iva + re;
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPedidos::desglose3Changed(double base, double iva, double re, double total)
{
    if(ui->cboporc_iva_gasto1->currentText() == QString::number(oPedido->porc_iva3))
        base +=ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText() == QString::number(oPedido->porc_iva3))
        base +=ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText() == QString::number(oPedido->porc_iva3))
        base +=ui->SpinGastoDist3->value();
    total = base + iva + re;
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPedidos::desglose4Changed(double base, double iva, double re, double total)
{
    if(ui->cboporc_iva_gasto1->currentText() == QString::number(oPedido->porc_iva4))
        base +=ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText() == QString::number(oPedido->porc_iva4))
        base +=ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText() == QString::number(oPedido->porc_iva4))
        base +=ui->SpinGastoDist3->value();
    total = base + iva + re;
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPedidos::lineaReady(lineaDetalle * ld)
{
    //-----------------------------------------------------
    // Insertamos línea de pedido y controlamos acumulados
    //-----------------------------------------------------

    bool ok_empresa,ok_Maya;
    ok_empresa = true;
    ok_Maya = true;
    Configuracion_global->empresaDB.transaction();
    Configuracion_global->groupDB.transaction();
    if (ld->idLinea == -1)
    {
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery query_lin_ped_pro(Configuracion_global->empresaDB);
        query_lin_ped_pro.prepare("INSERT INTO lin_ped (id_Cab,id_articulo,codigo,"
                                  "descripcion, cantidad, precio,subtotal,porc_dto,dto,porc_iva,"
                                  "total,cantidad_a_servir) VALUES (:id_cab,:id_articulo,:codigo,"
                                  ":descripcion,:cantidad,:precio,:subtotal,:porc_dto,:dto,"
                                  ":porc_iva,:total,:cantidad_pendiente);");
        query_lin_ped_pro.bindValue(":id_cab", oPedido->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":cantidad_pendiente",ld->cantidad);
        query_lin_ped_pro.bindValue(":precio",ld->precio);
        query_lin_ped_pro.bindValue(":subtotal",ld->subtotal);
        query_lin_ped_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_ped_pro.bindValue(":dto",ld->dto);
        query_lin_ped_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_ped_pro.bindValue(":total",ld->total);
        if (!query_lin_ped_pro.exec()){
            ok_empresa = false;
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Ocurrió un error al insertar la nueva línea: %1").arg(query_lin_ped_pro.lastError().text()),
                                 tr("Aceptar"));
        }
        //TODO control stock en pedidos cli

        ld->idLinea = query_lin_ped_pro.lastInsertId().toInt();

    } else
    {
        //TODO control stock en pedidos cli
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;
        QSqlQuery query_lin_ped_pro(Configuracion_global->empresaDB);

        query_lin_ped_pro.prepare("UPDATE lin_ped SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo =:codigo,"
                                  "descripcion =:descripcion,"
                                  "cantidad =:cantidad,"
                                  "cantidad_a_servir =:cantidad_pendiente,"
                                  "precio =:precio,"
                                  "subtotal =:subtotal,"
                                  "porc_dto =:porc_dto,"
                                  "dto =:dto,"
                                  "porc_iva =:porc_iva,"
                                  "total =:total "
                                  "WHERE id = :id;");

        query_lin_ped_pro.bindValue(":id_cab", oPedido->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":cantidad_a_servir",ld->cantidad);
        query_lin_ped_pro.bindValue(":precio",ld->precio);
        query_lin_ped_pro.bindValue(":subtotal",ld->subtotal);
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
        //TODO control stock en pedidos cli
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

void FrmPedidos::lineaDeleted(lineaDetalle * ld)
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
        q.prepare("delete from lin_ped where id =:id");
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

void FrmPedidos::convertir_ealbaran()
{
    if(oPedido->albaran ==0)
    {
        QString c = QString("id = %1").arg(oPedido->id);
        QString error;
        QHash <QString, QVariant> h;
        QMap<int, QSqlRecord> records = SqlCalls::SelectRecord("ped_cli",c,Configuracion_global->empresaDB,error);
        QSqlRecord r = records.value(oPedido->id);
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
        h.insert("total_albaran",h.value("total_pedido").toDouble());
        h.remove("total_pedido");
        h.remove("lugar_entrega");
        h.remove("importe_factura");
        h.remove("presupuesto");
        h.remove("id");
        h.insert("pedido_cliente",h.value("pedido").toInt());
        h.remove("pedido");
        h.remove("fecha_limite_entrega");
        h.remove("traspasado_albaran");
        h.remove("entregado");
        h.remove("traspasado_factura");
        h.insert("id_pais_entrega",h.value("pais_entrega"));
        h.remove("pais_entrega");
        h.remove("completo");
        h.remove("enviado");
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
        l->setQuery("select serie from series",Configuracion_global->empresaDB);
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
        QString d = QString("id_cab = %1").arg(oPedido->id);
        QMap<int, QSqlRecord> map = SqlCalls::SelectRecord("lin_ped", d,Configuracion_global->empresaDB, error);
          QMapIterator<int, QSqlRecord> i_l(map);
          while (i_l.hasNext())
          {
              i_l.next();
              QSqlRecord r_l = i_l.value();
              for(int i= 0; i< r_l.count();i++)
                  h_l.insert(r_l.fieldName(i),r_l.value(i));
              h_l.remove("id");
              h_l["id_cab"] = added;
              h_l.remove("cantidad_a_servir");
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
            p["albaran"] = num;
            c = "id="+QString::number(oPedido->id);
            bool updated = SqlCalls::SqlUpdate(p,"ped_cli",Configuracion_global->empresaDB,c,error);

            if(updated)
            {
                Configuracion_global->empresaDB.commit();
                t = new TimedMessageBox(qApp->activeWindow(),
                                                          QObject::tr("Se ha creado el albarán num:")+QString::number(num));
                oPedido->RecuperarPedido("select * from cab_alb where id ="+QString::number(oPedido->id));
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

void FrmPedidos::convertir_enFactura()
{
    if(oPedido->factura == "0" || oPedido->factura.isEmpty())
    {
        if(QMessageBox::question(this,tr("Pedidos a proveedores"),tr("¿Desea realmente facturar este pedido?"),
                                 tr("No"),tr("Sí"))==QMessageBox::Accepted)
        {
            LLenarPedido();
            Factura oFactura(this);
            Configuracion_global->empresaDB.transaction();
            bool transaccion = true;
            //-----------------------
            // LLeno objeto factura
            //-----------------------
            oFactura.AnadirFactura();
            oCliente3->Recuperar("select * from clientes where codigo_cliente ='"+ui->txtcodigo_cliente->text()+"'");
            oFactura.cif = oCliente3->cif_nif;
            oFactura.cliente = oCliente3->nombre_fiscal;
            oFactura.codigo_cliente = oCliente3->codigo_cliente;
            oFactura.codigo_entidad = oCliente3->entidad_bancaria;
            oFactura.cp = oCliente3->cp;
            oFactura.dc_cuenta = oCliente3->dc;
            oFactura.direccion1 = oCliente3->direccion1;
            oFactura.direccion2 = oCliente3->direccion2;
            oFactura.forma_pago = oCliente3->forma_pago;
            oFactura.cuenta_corriente = oCliente3->cuenta_corriente;
            oFactura.oficina_entidad = oCliente3->oficina_bancaria;
            oFactura.pedido_cliente = ui->txtpedido->text().toInt();
            oFactura.poblacion =oCliente3->poblacion;
            oFactura.provincia = oCliente3->provincia;
            oFactura.fecha = QDate::currentDate();
            oFactura.id_pais = oCliente3->id_pais;
            oFactura.id_forma_pago = Configuracion_global->Devolver_id_forma_pago(oCliente3->forma_pago);
            oFactura.id_cliente = oCliente3->id;
            oFactura.recargo_equivalencia = oCliente3->recargo_equivalencia;
            oFactura.dto = oCliente3->porc_dto_cliente;
            oFactura.porc_iva1 = Configuracion_global->ivaList.at(0).toInt();
            oFactura.porc_iva2 = Configuracion_global->ivaList.at(1).toInt();
            oFactura.porc_iva3 = Configuracion_global->ivaList.at(2).toInt();
            oFactura.porc_iva4 = Configuracion_global->ivaList.at(3).toInt();
            oFactura.porc_rec1 = Configuracion_global->reList.at(0).toInt();
            oFactura.porc_rec2 = Configuracion_global->reList.at(1).toInt();
            oFactura.porc_rec3 = Configuracion_global->reList.at(2).toInt();
            oFactura.porc_rec4 = Configuracion_global->reList.at(3).toInt();
            oFactura.base = oPedido->base_total;
            oFactura.base1 = oPedido->base1;
            oFactura.base2 = oPedido->base2;
            oFactura.base3 = oPedido->base3;
            oFactura.base4 = oPedido->base4;
            oFactura.dto = oPedido->dto;
            oFactura.iva = oPedido->iva_total;
            oFactura.iva1 = oPedido->iva1;
            oFactura.iva2 = oPedido->iva2;
            oFactura.iva3 = oPedido->iva3;
            oFactura.iva4 = oPedido->iva4;
            oFactura.porc_rec1 = oPedido->rec1;
            oFactura.porc_rec2 = oPedido->rec2;
            oFactura.porc_rec3 = oPedido->rec3;
            oFactura.porc_rec4 = oPedido->rec4;
            oFactura.subtotal = oPedido->subtotal;
            oFactura.total = oPedido->total_pedido;
            oFactura.total1 = oPedido->total1;
            oFactura.total2 = oPedido->total2;
            oFactura.total3 = oPedido->total3;
            oFactura.total4 = oPedido->total4;
            oFactura.total_recargo = oPedido->rec_total;

            // ----------------------------------
            // Creamos la cabecera de la factura
            //-----------------------------------
            // TODO - pedir serie factura.
            oFactura.factura = oFactura.NuevoNumeroFactura("A");
            oFactura.GuardarFactura(oFactura.id,true);
            //-------------------
            //  INSERTAR LÍNEAS
            //-------------------
            QSqlQuery lineas_ped(Configuracion_global->empresaDB);
            QSqlQuery lineas_fac(Configuracion_global->empresaDB);

            if(lineas_ped.exec("Select * from lin_ped where id_Cab ="+QString::number(oPedido->id)))
            {
                while (lineas_ped.next()) {
                    lineas_fac.prepare("INSERT INTO lin_fac(id_Cab, id_articulo, codigo, cantidad,"
                                       "descripcion, pvp, subtotal, porc_dto, dto, porc_iva, total)"
                                       " VALUES (:id_Cab, :id_articulo,:codigo,:cantidad,"
                                       ":descripcion,:pvp,:subtotal,:porc_dto,:dto,:porc_iva,"
                                       ":total);");
                    lineas_fac.bindValue(":id_Cab", oFactura.id);
                    lineas_fac.bindValue(":id_articulo",lineas_ped.record().value("id_articulo").toInt());
                    lineas_fac.bindValue(":codigo",lineas_ped.record().value("codigo").toString());
                    lineas_fac.bindValue(":cantidad", lineas_ped.record().value("cantidad").toInt());
                    lineas_fac.bindValue(":descripcion",lineas_ped.record().value("descripcion").toString());
                    lineas_fac.bindValue(":pvp",lineas_ped.record().value("pvp").toDouble());
                    lineas_fac.bindValue(":subtotal",lineas_ped.record().value("subtotal").toDouble());
                    lineas_fac.bindValue(":porc_dto",lineas_ped.record().value("porc_dto").toDouble());
                    lineas_fac.bindValue(":dto",lineas_ped.record().value("dto").toDouble());
                    lineas_fac.bindValue(":porc_iva",lineas_ped.record().value("porc_iva").toDouble());
                    lineas_fac.bindValue(":total",lineas_ped.record().value("total").toDouble());

                    if(!lineas_fac.exec())
                    {
                        QMessageBox::warning(this,tr("Pedidos cliente"),
                                             tr("Ocurrió un error al crear las líneas de factura: %1").arg(lineas_fac.lastError().text()));

                        transaccion = false;
                    }
                }
            }




            if(transaccion) {
                ui->btn_convertir->setEnabled(false);
                QString texto;
                texto = tr("Se ha creado una nueva factura.\ncon el número ")+ oFactura.factura+
                           tr("\n y de importe: ")+QString::number(oFactura.total,'f',Configuracion_global->decimales);
                TimedMessageBox * t = new TimedMessageBox(this,texto);
                //-------------------------------------
                // Insertamos datos factura en pedido
                //-------------------------------------
                oPedido->factura =oFactura.factura;
                oPedido->facturado = true;
                oPedido->GuardarPedido(oPedido->id);
                ui->txtcNumFra->setText(oPedido->factura);
                ui->txtcNumFra->setVisible(true);
                ui->lbfacturado->setVisible(true);
                Configuracion_global->empresaDB.commit();
            } else
            {
                Configuracion_global->empresaDB.rollback();
                QMessageBox::warning(this,tr("Pedidos de proveedor"),
                                     tr("No se ha podido crear la factura"),tr("Aceptar"));
            }
        }
    } else
        QMessageBox::warning(this,tr("Pedidos a Proveedor"),tr("Este pedido ya ha sido traspasado"),tr("Aceptar"));

}

void FrmPedidos::on_tabWidget_2_currentChanged(int index)
{
    Q_UNUSED(index);
    helper.resizeTable();
}


void FrmPedidos::on_txtcodigo_cliente_editingFinished()
{
    if(ui->txtcodigo_cliente->text() != oPedido->codigo_cliente){
        oCliente3->Recuperar("select * from clientes where codigo_cliente='"+ui->txtcodigo_cliente->text()+"'");
        LLenarCamposCliente();
        helper.set_tarifa(oCliente3->tarifa_cliente);
    }
}

void FrmPedidos::on_btnImprimir_clicked()
{
    FrmDialogoImprimir dlg_print(this);
    dlg_print.set_email(oCliente3->email);
    dlg_print.set_preview(false);
    if(dlg_print.exec() == dlg_print.Accepted)
    {
        bool ok /*= oPedido->set_impreso(true)*/;
        if(ok)
        {
            ui->lbimpreso->setVisible(true);
            int valor = dlg_print.get_option();
             QMap <QString,QVariant> parametros;

            switch (valor) {
            case 1: // Impresora
                //Configuracion_global->imprimir("pedido_"+QString::number(oCliente3->ididioma),false,false,parametros,this);
                break;
            case 2: // email
                // TODO - enviar pdf por mail
                break;
            case 3: // PDF
                //Configuracion_global->imprimir("pedido_"+QString::number(oCliente3->ididioma),true,false,parametros,this);
                break;
            case 4: //preview
                //Configuracion_global->imprimir("pedido_"+QString::number(oCliente3->ididioma),false,true,parametros,this);
                break;
            default:
                break;
            }

        }
    }
}

void FrmPedidos::on_radBusqueda_toggled(bool checked)
{
    if(checked)
        ui->stackedWidget->setCurrentIndex(1);
    else
        ui->stackedWidget->setCurrentIndex(0);

}


void FrmPedidos::on_txtBuscar_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    filter_table();


}

void FrmPedidos::on_tabla_clicked(const QModelIndex &index)
{
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tabla->model());
    int id = Configuracion_global->devolver_id_tabla(model,index);
    oPedido->RecuperarPedido("select * from ped_cli where id ="+QString::number(id));
    LLenarCampos();
    BloquearCampos(true);
}

void FrmPedidos::on_tabla_doubleClicked(const QModelIndex &index)
{
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tabla->model());
    int id = Configuracion_global->devolver_id_tabla(model,index);
    oPedido->RecuperarPedido("select * from ped_cli where id ="+QString::number(id));
    LLenarCampos();
    ui->radEdicion->setChecked(true);
    BloquearCampos(true);
}

void FrmPedidos::on_btnBuscar_clicked()
{
    ui->radBusqueda->setChecked(true);
}

void FrmPedidos::on_btnLimpiar_clicked()
{
    ui->txtBuscar->clear();
    ui->cboModo->setCurrentIndex(0);
    ui->cboOrden->setCurrentIndex(0);

}

void FrmPedidos::on_cboModo_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    filter_table();
}

void FrmPedidos::on_cboOrden_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    filter_table();
}
