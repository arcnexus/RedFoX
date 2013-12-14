#include "frmpedidos.h"
#include "ui_frmpedidos.h"


#include "../Almacen/articulo.h"
#include "../Zona_Ventas/albaran.h"

#include "../Busquedas/db_consulta_view.h"
#include "../Zona_Ventas/factura.h"
#include"../Auxiliares/datedelegate.h"
#include"../Auxiliares/monetarydelegate.h"
#include"../Zona_Maestros/transportistas.h"
#include "../vencimientos.h"

FrmPedidos::FrmPedidos(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmPedidos),
    helper(this),
    menuButton(QIcon(":/Icons/PNG/pedidos_cli.png"),tr("Pedidos"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/pedidos_cli.png"),"",this))
{
    ui->setupUi(this);
//    QSqlQueryModel *pais = new QSqlQueryModel(this);
//    pais->setQuery("select pais from paises order by pais",Configuracion_global->groupDB);

    ui->cboPais->setModel(Configuracion_global->paises_model);
    ui->cbopais_entrega->setModel(Configuracion_global->paises_model);
    ui->cboPais->setCurrentIndex(-1);
    ui->cbopais_entrega->setCurrentIndex(-1);
    // Pongo valores por defecto
    ui->lbfacturado->setVisible(false);
    ui->lbimpreso->setVisible(false);

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
    connect(ui->tabla->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_table_row_changed(QModelIndex,QModelIndex)));
    //ui->tabla->selectRow(0);

    //--------------------
    // %Iva combos gastos
    //--------------------
    QSqlQueryModel *iva = new QSqlQueryModel(this);
    iva->setQuery("select iva from tiposiva",Configuracion_global->groupDB);
    ui->cboporc_iva_gasto1->setModel(iva);
    int index = ui->cboporc_iva_gasto1->findText(Configuracion_global->ivaList.at(0));
    ui->cboporc_iva_gasto1->setCurrentIndex(index);
    ui->cboporc_iva_gasto2->setModel(iva);
    index = ui->cboporc_iva_gasto2->findText(Configuracion_global->ivaList.at(0));
    ui->cboporc_iva_gasto1->setCurrentIndex(index);
    ui->cboporc_iva_gasto3->setModel(iva);
    index = ui->cboporc_iva_gasto3->findText(Configuracion_global->ivaList.at(0));
    ui->cboporc_iva_gasto1->setCurrentIndex(index);

    //-------------------
    // Modo Busqueda
    //-------------------

    ui->stackedWidget->setCurrentIndex(1);


    connect(ui->cboporc_iva_gasto1,SIGNAL(currentIndexChanged(int)),
            this,SLOT(cboporc_iva_gasto1_currentIndexChanged(int)));
    connect(ui->cboporc_iva_gasto2,SIGNAL(currentIndexChanged(int)),
            this,SLOT(cboporc_iva_gasto1_currentIndexChanged(int)));
    connect(ui->cboporc_iva_gasto3,SIGNAL(currentIndexChanged(int)),
            this,SLOT(cboporc_iva_gasto1_currentIndexChanged(int)));
    connect(ui->SpinGastoDist1,SIGNAL(valueChanged(double)),
            this,SLOT(SpinGastoDist1_valueChanged(double)));
    connect(ui->SpinGastoDist2,SIGNAL(valueChanged(double)),
            this,SLOT(SpinGastoDist2_valueChanged(double)));
    connect(ui->SpinGastoDist3,SIGNAL(valueChanged(double)),
            this,SLOT(SpinGastoDist3_valueChanged(double)));

    //----------------------
    // Forma de pago
    //----------------------
    QSqlQueryModel *formas_pago = new QSqlQueryModel(this);
    formas_pago->setQuery("select forma_pago from formpago",Configuracion_global->groupDB);
    ui->cboFormapago->setModel(formas_pago);

    //-----------------------
    // Tarifas
    //-----------------------
    QSqlQueryModel * modeloDivisa = new QSqlQueryModel(this);
    modeloDivisa->setQuery("select moneda from monedas order by moneda",Configuracion_global->groupDB);
    ui->cboDivisa->setModel(modeloDivisa);



    //----------------------
    // Control de eventos
    //----------------------
//    ui->txtcodigo_cliente->installEventFilter(this);
//    ui->txtcodigo_transportista->installEventFilter(this);
    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;

    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);

    setUpBusqueda();

    BloquearCampos(true);
    ui->tabla->selectRow(0);
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

    int index = ui->cboPais->findText(Configuracion_global->Devolver_pais(oPedido->id_pais));
    ui->cboPais->setCurrentIndex(index);

    ui->txtcif->setText(oPedido->cif);
    ui->chklporc_rec->setChecked(oPedido->recargo_equivalencia==1);
        helper.set_tarifa(oCliente3->tarifa_cliente);
    ui->txtsubtotal->setText(QString::number(oPedido->subtotal));
    ui->spin_porc_dto_especial->setValue(oPedido->porc_dto);
    ui->spin_porc_dto_pp->setValue(oPedido->porc_dto_pp);
    ui->txtdto->setText(QString::number(oPedido->dto));
    //oPedido->dto;
    ui->txtbase1->setText(QString::number(oPedido->base1));
    ui->txtbase2->setText(QString::number(oPedido->base2));
    ui->txtbase3->setText(QString::number(oPedido->base3));
    ui->txtbase4->setText(QString::number(oPedido->base4));
    ui->txtporc_iva1->setText(QString::number(oPedido->porc_iva1));
    ui->txtporc_iva2->setText(QString::number(oPedido->porc_iva2));
    ui->txtporc_iva3->setText(QString::number(oPedido->porc_iva3));
    ui->txtporc_iva4->setText(QString::number(oPedido->porc_iva4));
    ui->txtiva1->setText(QString::number(oPedido->iva1));
    ui->txtiva2->setText(QString::number(oPedido->iva2));
    ui->txtiva3->setText(QString::number(oPedido->iva3));
    ui->txtiva4->setText(QString::number(oPedido->iva4));
    ui->txtporc_rec1->setText(QString::number(oPedido->porc_rec1));
    ui->txtporc_rec2->setText(QString::number(oPedido->porc_rec2));
    ui->txtporc_rec3->setText(QString::number(oPedido->porc_rec3));
    ui->txtporc_rec4->setText(QString::number(oPedido->porc_rec4));
    ui->txtrec1->setText(QString::number(oPedido->rec1));
    ui->txtrec2->setText(QString::number(oPedido->rec2));
    ui->txtrec3->setText(QString::number(oPedido->rec3));
    ui->txtrec4->setText(QString::number(oPedido->rec4));
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
    ui->txtcNumFra->setText(oPedido->factura);
    ui->txtfecha_factura->setDate(oPedido->fecha_factura);
    index = ui->cboFormapago->findText(Configuracion_global->Devolver_forma_pago(oPedido->id_forma_pago));
    ui->cboFormapago->setCurrentIndex(index);

    ui->txtcomentario->setText(oPedido->comentario);
    ui->txtentregado_a_cuenta->setText(QString::number(oPedido->entregado_a_cuenta));
    //oPedido->traspasado_albaran;
    //oPedido->traspasado_factura;
    ui->txtdireccion1_entrega->setText(oPedido->direccion_entrega1);
    ui->txtdireccion2_entrega->setText(oPedido->direccion_entrega2);
    ui->txtcp_entrega->setText(oPedido->cp_entrega);
    ui->txtpoblacion_entrega->setText(oPedido->poblacion_entrega);
    ui->txtprovincia_entrega->setText(oPedido->provincia_entrega);
    index = ui->cbopais_entrega->findText(Configuracion_global->Devolver_pais(oPedido->id_pais_entrega));
    ui->cbopais_entrega->setCurrentIndex(index);
    ui->txtemail_alternativo->setText(oPedido->email_entrega);
    ui->txtcomentarios_alternativo->setText(oPedido->comentarios_entrega);
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
    helper.setId_cliente(oCliente3->id);
    //----------------
    // Transportista
    //----------------
    transportistas oTrans(this);
    QStringList condiciones,extras;
    condiciones << QString("id=%1").arg(oPedido->id_transportista);
    extras << "";
    oTrans.recuperar(condiciones,extras);
    ui->txtcodigo_transportista->setText(oTrans.h_transportista.value("codigo").toString());
    ui->txtTransportista->setText(oTrans.h_transportista.value("transportista").toString());
    //-------------------
    // divisas
    //-------------------
    QString divisa = Configuracion_global->Devolver_moneda(oPedido->id_divisa);
    index = ui->cboDivisa->findText(divisa);
    ui->cboDivisa->setCurrentIndex(index);

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
    int index = ui->cboPais->findText(oCliente3->pais);
    ui->cboPais->setCurrentIndex(index);

    ui->txtcif->setText(oCliente3->cif_nif);
    helper.set_tarifa(oCliente3->tarifa_cliente);

    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();

    if (oCliente3->recargo_equivalencia==1) {
        ui->chklporc_rec->setChecked(true);
        oPedido->recargo_equivalencia = (1);
        ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
        ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
        ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
        ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));
        oPedido->porc_rec1 = ui->txtporc_rec1->text().toFloat();
        oPedido->porc_rec2 = ui->txtporc_rec2->text().toFloat();
        oPedido->porc_rec3 = ui->txtporc_rec3->text().toFloat();
        oPedido->porc_rec4 = ui->txtporc_rec4->text().toFloat();
    } else {
        ui->chklporc_rec->setChecked(false);
        oPedido->recargo_equivalencia = (0);
        ui->txtporc_rec1->setText("0.00");
        ui->txtporc_rec2->setText("0.00");
        ui->txtporc_rec3->setText("0.00");
        ui->txtporc_rec4->setText("0.00");
        oPedido->porc_rec1 = 0;
        oPedido->porc_rec2 = 0;
        oPedido->porc_rec3 = 0;
        oPedido->porc_rec4 = 0;

    }
    oCliente3->Recuperar("Select * from clientes where id ="+QString::number(oPedido->id_cliente));
    helper.set_tarifa(oPedido->tarifa_cliente);
    helper.set_tipo_dto_tarifa(oCliente3->tipo_dto_tarifa);
    helper.setId_cliente(oCliente3->id);


    //---------------------------------
    // Comprobar direccion alternativa
    //---------------------------------
    QMap <int,QSqlRecord> rec;
    QString error;
    QStringList condiciones;
    condiciones << QString("id_cliente = %1").arg(oCliente3->id) << "direccion_envio = 1";
    rec = SqlCalls::SelectRecord("cliente_direcciones",condiciones,Configuracion_global->groupDB,error);

    QMapIterator <int,QSqlRecord> i(rec);
    while(i.hasNext())
    {
        i.next();
        ui->txtdireccion1_entrega->setText(i.value().value("direccion1").toString());
        ui->txtdireccion2_entrega->setText(i.value().value("direccion2").toString());
        ui->txtcp_entrega->setText(i.value().value("cp").toString());
        ui->txtpoblacion_entrega->setText(i.value().value("poblacion").toString());
        ui->txtprovincia_entrega->setText(i.value().value("provincia").toString());
        int index = ui->cbopais_entrega->findText(Configuracion_global->Devolver_pais(i.value().value("pais").toInt()));
        ui->cbopais_entrega->setCurrentIndex(index);
        ui->txtemail_alternativo->setText(i.value().value("email").toString());
        ui->txtcomentarios_alternativo->setPlainText(i.value().value("comentarios").toString());

    }
    index = ui->cboFormapago->findText(Configuracion_global->Devolver_forma_pago(oCliente3->id_forma_pago));
    ui->cboFormapago->setCurrentIndex(index);
    //-------------------
    // divisas
    //-------------------
    QString divisa = Configuracion_global->Devolver_moneda(oCliente3->id_divisa);
    index = ui->cboDivisa->findText(divisa);
    ui->cboDivisa->setCurrentIndex(index);
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
    ui->txtcomentario->setText("");
    ui->txtbase1->setText(0);
    ui->txtbase2->setText(0);
    ui->txtbase3->setText(0);
    ui->txtbase4->setText(0);
    //Configuracion_global->CargarDatos();
    //QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    ui->txtporc_iva1->setText(QString::number(Configuracion_global->ivaList.at(0).toDouble()));
    ui->txtporc_iva2->setText(QString::number(Configuracion_global->ivaList.at(1).toDouble()));
    ui->txtporc_iva3->setText(QString::number(Configuracion_global->ivaList.at(2).toDouble()));
    ui->txtporc_iva4->setText(QString::number(Configuracion_global->ivaList.at(3).toDouble()));
    ui->txtporc_rec1->setText("0.00");
    ui->txtporc_rec2->setText("0.00");
    ui->txtporc_rec3->setText("0.00");
    ui->txtporc_rec4->setText("0.00");
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


void FrmPedidos::LLenarPedido()
{
    oPedido->albaran= ui->txtalbaran->text().toInt();
    oPedido->pedido=ui->txtpedido->text().toInt();
    oPedido->fecha=ui->txtfecha->date();
    oPedido->id_divisa = Configuracion_global->Devolver_id_moneda(ui->cboDivisa->currentText());
    //oPedido->pedido;
    //oPedido->id_cliente;
    oPedido->codigo_cliente=ui->txtcodigo_cliente->text();
    oPedido->cliente=ui->txtcliente->text();
    oPedido->direccion1=ui->txtdireccion1->text();
    oPedido->direccion2=ui->txtdireccion2->text();
    oPedido->poblacion=ui->txtpoblacion->text();
    oPedido->provincia=ui->txtprovincia->text();
    oPedido->cp=ui->txtcp->text();
    oPedido->id_pais = Configuracion_global->Devolver_id_pais(ui->cboPais->currentText());
    oPedido->cif=ui->txtcif->text();
    oPedido->recargo_equivalencia=ui->chklporc_rec->isChecked();
    oPedido->subtotal=ui->txtsubtotal->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->dto=ui->txtdto->text().replace(_moneda,"").replace(".","").replace(",",".").toDouble();
    oPedido->porc_dto = ui->spin_porc_dto_especial->value();
    oPedido->porc_dto_pp = ui->spin_porc_dto_pp->value();
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
    oPedido->porc_rec1=ui->txtporc_rec1->text().toDouble();
    oPedido->porc_rec2=ui->txtporc_rec2->text().toDouble();
    oPedido->porc_rec3=ui->txtporc_rec3->text().toDouble();
    oPedido->porc_rec4=ui->txtporc_rec4->text().toDouble();
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
    oPedido->direccion_entrega1 = ui->txtdireccion1_entrega->text();
    oPedido->direccion_entrega2 = ui->txtdireccion2_entrega->text();
    oPedido->cp_entrega=ui->txtcp_entrega->text();
    oPedido->poblacion_entrega=ui->txtpoblacion_entrega->text();
    oPedido->provincia_entrega=ui->txtprovincia_entrega->text();
    oPedido->id_pais_entrega=Configuracion_global->Devolver_id_pais(ui->cbopais_entrega->currentText());
    oPedido->email_entrega = ui->txtemail_alternativo->text();
    oPedido->comentarios_entrega = ui->txtcomentarios_alternativo->toPlainText();
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
    oPedido->id_forma_pago = Configuracion_global->Devolver_id_forma_pago(ui->cboFormapago->currentText());
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

void FrmPedidos::filter_table(QString texto, QString orden, QString modo)
{
    ui->stackedWidget->setCurrentIndex(1);
    //  tr("Pedido") <<tr("Cliente") <<tr("Fecha")
    QHash <QString,QString> h;
    h[tr("Pedido")] = "pedido";
    h[tr("Cliente")] = "cliente";
    h[tr("Fecha")] = "fecha";
    h[tr("Total")] = "total_pedido";
    QString order = h.value(orden);

    if (modo == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    m->setQuery("select id, pedido, fecha,total_pedido, cliente from ped_cli "
                    "where "+order+" like '%" + texto.trimmed() + "%' order by "+order+" "+modo,Configuracion_global->empresaDB);

    ui->tabla->selectRow(0);
}

void FrmPedidos::on_table_row_changed(QModelIndex actual, QModelIndex previous)
{
    Q_UNUSED(previous);
    on_tabla_clicked(actual);
}

void FrmPedidos::calcular_iva_gastos()
{
    ui->spiniva_gasto1->setValue((ui->SpinGastoDist1->value()*(ui->cboporc_iva_gasto1->currentText().toDouble()/100)));
    ui->spiniva_gasto2->setValue((ui->SpinGastoDist2->value()*(ui->cboporc_iva_gasto2->currentText().toDouble()/100)));
    ui->spiniva_gasto3->setValue((ui->SpinGastoDist3->value()*(ui->cboporc_iva_gasto3->currentText().toDouble()/100)));
    helper.calculatotal();
}

void FrmPedidos::buscar_transportista()
{
    db_consulta_view consulta;
    QStringList campos;
    campos <<"codigo" <<"transportista";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("transportista");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,codigo,transportista from transportista");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Código") << tr("Nombre");
    tamanos <<"0" << "100" << "300";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Transportistas");
    if(consulta.exec())
    {
        int id = consulta.get_id();
        transportistas oTrans(this);
        QStringList condiciones,extras;
        condiciones <<QString("id=%1").arg(id);
        extras << "";
        oTrans.recuperar(condiciones,extras);
        ui->txtcodigo_transportista->setText(oTrans.h_transportista.value("codigo").toString());
        ui->txtTransportista->setText(oTrans.h_transportista.value("transportista").toString());
        oPedido->id_transportista = oTrans.h_transportista.value("id").toInt();
    }
}

bool FrmPedidos::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj == ui->txtcodigo_transportista)
        {
            if(keyEvent->key() == Qt::Key_F1)
                buscar_transportista();
        }
        if(obj == ui->txtcodigo_cliente)
        {
            if(keyEvent->key() == Qt::Key_F1)
                on_botBuscarCliente_clicked();
        }
        if(keyEvent->key() == Qt::Key_F1)
        {
            if(ui->btnEditar->isEnabled())
            {
                if(m_busqueda->isShow())
                    ocultarBusqueda();
                else
                    mostrarBusqueda();
            }
            return true;
        }
        if(keyEvent->key() == Qt::Key_Return)
            if(ui->stackedWidget->currentIndex() == 1)
                on_tabla_doubleClicked(ui->tabla->currentIndex());
        if(keyEvent->key() == Qt::Key_Escape)
            return true;
    }
    if(event->type() == QEvent::Resize)
        _resizeBarraBusqueda(m_busqueda);
    return MayaModule::eventFilter(obj,event);
}

void FrmPedidos::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);
    this->installEventFilter(this);

    QStringList orden;
    orden  << tr("Pedido") <<tr("Cliente") <<tr("Fecha");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(m_busqueda,SIGNAL(hideMe()),this,SLOT(ocultarBusqueda()));
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));


    QPushButton* add = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir"),this);
    connect(add,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(add);

    QPushButton* edit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar"),this);
    connect(edit,SIGNAL(clicked()),this,SLOT(on_btnEditar_clicked()));
    m_busqueda->addWidget(edit);

    QPushButton* print = new QPushButton(QIcon(":/Icons/PNG/print2.png"),tr("Imprimir"),this);
   // connect(print,SIGNAL(clicked()),this,SLOT(on_btnEditar_2_clicked()));//TODO
    m_busqueda->addWidget(print);

    QPushButton* del = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    connect(del,SIGNAL(clicked()),this,SLOT(on_btn_borrar_clicked()));
    m_busqueda->addWidget(del);

    connect(m_busqueda,SIGNAL(key_Down_Pressed()),ui->tabla,SLOT(setFocus()));
    connect(m_busqueda,SIGNAL(key_F2_Pressed()),this,SLOT(ocultarBusqueda()));

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
    // DoubleSpinBox
    QList<QDoubleSpinBox *> DblSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DblSpinBox;
    foreach (DblSpinBox, DblSpinBoxList) {
        DblSpinBox->setEnabled(!state);
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

    ui->chklporc_rec->setEnabled(false);
    ui->spiniva_gasto1->setEnabled(false);
    ui->spiniva_gasto2->setEnabled(false);
    ui->spiniva_gasto3->setEnabled(false);

    m_busqueda->block(!state);
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
    ui->stackedWidget->setCurrentIndex(0);
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
    ui->stackedWidget->setCurrentIndex(0);
    if (oPedido->editable)
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
    Q_UNUSED(moneda);
    subtotal +=(ui->SpinGastoDist1->value() + ui->SpinGastoDist2->value() + ui->SpinGastoDist3->value());
    if(dto == 0)
        dto = subtotal *(ui->spin_porc_dto_especial->value()/100.0);
    double dtopp = subtotal *(ui->spin_porc_dto_pp->value()/100.0);
    ui->txtDto_pp->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(dtopp,Configuracion_global->decimales_campos_totales)));
    base = subtotal -(dto+dtopp);

    base +=(ui->SpinGastoDist1->value() + ui->SpinGastoDist2->value() + ui->SpinGastoDist3->value());
    //calcular_iva_gastos();
    iva += (ui->spiniva_gasto1->value() + ui->spiniva_gasto2->value() + ui->spiniva_gasto3->value());
    total = base + iva + re;

    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales_campos_totales)));

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales_campos_totales)));
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
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
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
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
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
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
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
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
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
                                  "total,cantidad_a_servir,iva,porc_rec,rec) VALUES (:id_cab,:id_articulo,:codigo,"
                                  ":descripcion,:cantidad,:precio,:subtotal,:porc_dto,:dto,"
                                  ":porc_iva,:total,:cantidad_pendiente,:iva,:porc_rec,:rec);");
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
        query_lin_ped_pro.bindValue(":iva",ld->iva);
        query_lin_ped_pro.bindValue(":porc_rec",ld->rec_perc);
        query_lin_ped_pro.bindValue(":rec",ld->rec);
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
        h["pedido_cliente"] = oPedido->pedido;
        h["id_cliente"] = oPedido->id_cliente;
        h["codigo_cliente"] = oPedido->codigo_cliente;
        h["id_divisa"] =oPedido->id_divisa;
        h["cliente"] = oPedido->cliente;
        h["direccion1"] = oPedido->direccion1;
        h["direccion2"] = oPedido->direccion2;
        h["poblacion"] = oPedido->poblacion;
        h["provincia"] = oPedido->provincia;
        h["cp"] = oPedido->cp;
        h["id_pais"] = oPedido->id_pais;
        h["direccion1_entrega"] = oPedido->direccion_entrega1;
        h["direccion2_entrega"] = oPedido->direccion_entrega2;
        h["poblacion_entrega"] = oPedido->poblacion_entrega;
        h["provincia_entrega"] = oPedido->provincia_entrega;
        h["cp_entrega"] = oPedido->cp_entrega;
        h["id_pais_entrega"] = oPedido->id_pais_entrega;
        h["email_entrega"] = oPedido->email_entrega;
        h["comentarios_entrega"] = oPedido->comentarios_entrega;
        h["cif"] = oPedido->cif;
        h["telefono"] = oCliente3->telefono1;
        h["fax"] = oCliente3->movil;
        h["email"] = oCliente3->email;
        h["recargo_equivalencia"] = oPedido->recargo_equivalencia;
        h["subtotal"] = oPedido->subtotal;
        h["dto"] = oPedido->dto;
        h["porc_dto"] = oPedido->porc_dto;
        h["porc_dto_pp"] = oPedido->porc_dto_pp;
        h["dto_pp"] = oPedido->dto_pp;
        h["base1"] = oPedido->base1;
        h["base2"] = oPedido->base2;
        h["base3"] = oPedido->base3;
        h["base4"] = oPedido->base4;
        h["porc_iva1"] = oPedido->porc_iva1;
        h["porc_iva2"] = oPedido->porc_iva2;
        h["porc_iva3"] = oPedido->porc_iva3;
        h["porc_iva4"] = oPedido->porc_iva4;
        h["iva1"] = oPedido->iva1;
        h["iva2"] = oPedido->iva2;
        h["iva3"] = oPedido->iva3;
        h["iva4"] = oPedido->iva4;
        h["porc_rec1"] = oPedido->porc_rec1;
        h["porc_rec2"] = oPedido->porc_rec2;
        h["porc_rec3"] = oPedido->porc_rec3;
        h["porc_rec4"] = oPedido->porc_rec4;
        h["rec1"] = oPedido->rec1;
        h["rec2"] = oPedido->rec2;
        h["rec3"] = oPedido->rec3;
        h["rec4"] = oPedido->rec4;
        h["total1"] = oPedido->total1;
        h["total2"] = oPedido->total2;
        h["total3"] = oPedido->total3;
        h["total4"] = oPedido->total4;
        h["base_total"] = oPedido->base_total;
        h["iva_total"] = oPedido->iva_total;
        h["rec_total"] = oPedido->rec_total;
        h["total_albaran"] = oPedido->total_albaran;
        h["impreso"] = oPedido->impreso;
        h["facturado"] = oPedido->facturado;
        h["factura"] = oPedido->factura;
        h["fecha_factura"] = oPedido->fecha_factura;
        h["comentario"] = oPedido->comentario;
        h["entregado_a_cuenta"] = oPedido->entregado_a_cuenta;
        h["id_forma_pago"] = oPedido->id_forma_pago;
        h["desc_gasto1"] = oPedido->gasto1;
        h["desc_gasto2"] = oPedido->gasto2;
        h["desc_gasto3"] = oPedido->gasto3;
        h["imp_gasto1"] = oPedido->imp_gasto1;
        h["imp_gasto2"] = oPedido->imp_gasto2;
        h["imp_gasto3"] = oPedido->imp_gasto3;
        h["porc_iva_gasto1"] = oPedido->porc_iva_gasto1;
        h["porc_iva_gasto2"] = oPedido->porc_iva_gasto2;
        h["porc_iva_gasto3"] = oPedido->porc_iva_gasto3;
        h["iva_gasto1"] = oPedido->iva_gasto1;
        h["iva_gasto2"] = oPedido->iva_gasto2;
        h["iva_gasto3"] = oPedido->iva_gasto3;
        h["id_transportista"] = oPedido->id_transportista;
        h["ejercicio"] = Configuracion_global->cEjercicio;
        h["editable"] = true;


        Albaran oAlbaran;
        Configuracion_global->empresaDB.transaction();
        QString serie;
        if(Configuracion_global->serie.isEmpty())
        {
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

             serie = box->currentText();
            dlg->deleteLater();
        } else
            serie = Configuracion_global->serie;

        int num = oAlbaran.NuevoNumeroAlbaran(serie);
        h["albaran"] =num;
        h["serie"] = serie;
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
        if(added_l == -1 || added == -1)
        {
            Configuracion_global->empresaDB.rollback();
//            qDebug() <<added;
//            qDebug() <<error;
        } else
        {
            QHash <QString, QVariant> p;
            p["albaran"] = num;
            c = "id="+QString::number(oPedido->id);
            bool updated = SqlCalls::SqlUpdate(p,"ped_cli",Configuracion_global->empresaDB,c,error);

            if(updated)
            {

                t = new TimedMessageBox(qApp->activeWindow(),
                                                          QObject::tr("Se ha creado el albarán num:")+QString::number(num));
                oPedido->RecuperarPedido("select * from cab_alb where id ="+QString::number(oPedido->id));
                //-------------------------------------
                // Insertamos datos albaran en pedido
                //-------------------------------------
                oPedido->albaran =oAlbaran.albaran;
                oPedido->GuardarPedido(oPedido->id);
                ui->txtalbaran->setText(QString::number(num));
                bool commited =Configuracion_global->empresaDB.commit();

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
    if(oPedido->editable)
    {
        if(QMessageBox::question(this,tr("Gestión de pedidos"),tr("¿Desea realmente facturar este pedido?"),
                                 tr("No"),tr("Sí"))==QMessageBox::Accepted)
        {
            LLenarPedido();
            if(ui->txtalbaran->text().isEmpty())
                convertir_ealbaran();

            QString serie;
            if(Configuracion_global->serie.isEmpty())
            {
                //--------------------
                // Preguntamos serie
                //--------------------
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

                serie = box->currentText();
                dlg->deleteLater();
            } else
                serie = Configuracion_global->serie;
            Factura oFactura(this);
            Configuracion_global->empresaDB.transaction();
            bool transaccion = true;
            //-----------------------
            // LLeno objeto factura
            //-----------------------
            oFactura.AnadirFactura();
            oCliente3->Recuperar("select * from clientes where codigo_cliente ='"+ui->txtcodigo_cliente->text()+"'");
            oFactura.serie = serie;
            oFactura.cif = oPedido->cif;
            oFactura.cliente = oPedido->cliente;
            oFactura.id_divisa = oPedido->id_divisa;
            oFactura.codigo_cliente = oPedido->codigo_cliente;
            oFactura.id_cliente = oCliente3->id;
            oFactura.codigo_entidad = oCliente3->entidad_bancaria;
            oFactura.cuenta_corriente = oCliente3->cuenta_corriente;
            oFactura.oficina_entidad = oCliente3->oficina_bancaria;
            oFactura.dc_cuenta = oCliente3->dc;
            oFactura.cp = oPedido->cp;
            oFactura.direccion1 = oPedido->direccion1;
            oFactura.direccion2 = oPedido->direccion2;
            oFactura.id_forma_pago = oPedido->id_forma_pago;

            oFactura.pedido_cliente = ui->txtpedido->text().toInt();
            oFactura.poblacion =oPedido->poblacion;
            oFactura.provincia = oPedido->provincia;
            oFactura.fecha = QDate::currentDate();
            oFactura.id_pais = oPedido->id_pais;
            oFactura.direccion1 = oPedido->direccion1;
            oFactura.direccion2 = oPedido->direccion2;
            oFactura.poblacion =oPedido->poblacion;
            oFactura.provincia = oPedido->provincia;
            oFactura.id_pais = oPedido->id_pais;

            oFactura.direccion1_entrega = oPedido->direccion_entrega1;
            oFactura.direccion2_entrega = oPedido->direccion_entrega2;
            oFactura.poblacion_entrega =oPedido->poblacion_entrega;
            oFactura.provincia_entrega = oPedido->provincia_entrega;
            oFactura.id_pais_entrega = oPedido->id_pais_entrega;

            oFactura.id_forma_pago = Configuracion_global->Devolver_id_forma_pago(oCliente3->forma_pago);

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
            oFactura.factura = oFactura.NuevoNumeroFactura(serie);
            oFactura.GuardarFactura(oFactura.id,true);
            //-------------------
            //  INSERTAR LÍNEAS
            //-------------------
            QSqlQuery lineas_ped(Configuracion_global->empresaDB);

            if(lineas_ped.exec("Select * from lin_ped where id_Cab ="+QString::number(oPedido->id)))
            {
                QHash <QString, QVariant> h_lineas_fac;
                QString error;
                while (lineas_ped.next()) {

                    h_lineas_fac["id_cab"] = oFactura.id;
                    h_lineas_fac["id_articulo"] = lineas_ped.record().value("id_articulo").toInt();
                    h_lineas_fac["codigo"] = lineas_ped.record().value("codigo").toString();
                    h_lineas_fac["cantidad"] =  lineas_ped.record().value("cantidad").toInt();
                    h_lineas_fac["descripcion"] = lineas_ped.record().value("descripcion").toString();
                    h_lineas_fac["precio"] = lineas_ped.record().value("precio").toDouble();
                    h_lineas_fac["subtotal"] = lineas_ped.record().value("subtotal").toDouble();
                    h_lineas_fac["porc_dto"] = lineas_ped.record().value("porc_dto").toDouble();
                    h_lineas_fac["dto"] = lineas_ped.record().value("dto").toDouble();
                    h_lineas_fac["porc_iva"] = lineas_ped.record().value("porc_iva").toDouble();
                    h_lineas_fac["total"] = lineas_ped.record().value("total").toDouble();
                    int new_id = SqlCalls::SqlInsert(h_lineas_fac,"lin_fac",Configuracion_global->empresaDB,error);

                    if(new_id == -1)
                    {
                        QMessageBox::warning(this,tr("Pedidos cliente"),
                                             tr("Ocurrió un error al crear las líneas de factura: %1").arg(error));

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
                oPedido->editable = false;
                oPedido->GuardarPedido(oPedido->id);
                ui->txtcNumFra->setText(oPedido->factura);
                Configuracion_global->empresaDB.commit();
                //------------------------------------
                // Actualizamos factura en albarán
                //------------------------------------
                QString error;
                int albaran = SqlCalls::SelectOneField("ped_cli","albaran",QString("albaran=%1").arg(oPedido->albaran),
                                                        Configuracion_global->empresaDB,error).toInt();
                if(albaran >0)
                {
                    QHash <QString, QVariant> a;
                    a["factura"] = oFactura.factura;
                    bool updated = SqlCalls::SqlUpdate(a,"cab_alb",Configuracion_global->empresaDB,
                                                       QString("albaran =%1").arg(oPedido->albaran),
                                                       error);
                    if(!updated)
                        TimedMessageBox *t = new TimedMessageBox(this,tr("No se pudo guardar el numero de albarán en la factura: %1").arg(error));
                }



                vencimientos vto(this);
                //-----------------
                // Asiento contable
                //-----------------
                if(Configuracion_global->contabilidad)
                {

                    bool creado;
                    oCliente3->Recuperar("select * from clientes where id = "+QString::number(oFactura.id_cliente));
                    if(oFactura.apunte == 0)
                        creado = oFactura.Apunte();
                    else
                        creado = oFactura.EditApunte(oFactura.apunte);
                    if(!creado)
                        Configuracion_global->empresaDB.rollback();
                }
                //------------------
                // Vencimiento
                //------------------
                vto.calcular_vencimiento(oFactura.fecha,oFactura.id_cliente,0,oFactura.id,(oFactura.serie+"/"+oFactura.factura),1,
                                         "V",oFactura.total);
                oPedido->RecuperarPedido("select * from cab_alb where id ="+QString::number(oPedido->id));
                LLenarCampos();

            } else
            {

                QMessageBox::warning(this,tr("Traspaso a factura"),tr("No se pudo crear la factura "),tr("Aceptar"));

                Configuracion_global->empresaDB.rollback();
            }





            } else
            {
                Configuracion_global->empresaDB.rollback();
                QMessageBox::warning(this,tr("Gestión de pedidos"),
                                     tr("No se ha podido crear la factura"),tr("Aceptar"));
            }

    } else
        QMessageBox::warning(this,tr("Gestión de pedidos"),tr("Este pedido ya ha sido traspasado"),tr("Aceptar"));

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
        {     bool ok  /* = oFactura->set_impresa(true)*/;
            if(ok)
            {
                ui->lbimpreso->setVisible(true);
                int valor = dlg_print.get_option();
                QMap <QString,QString> parametros_sql;
                parametros_sql["Empresa.ped_cli"] = QString("id = %1").arg(oPedido->id);
                parametros_sql["Empresa.lin_ped"] = QString("id_cab = %1").arg(oPedido->id);
                QString report = "ped_cli_"+QString::number(oCliente3->ididioma);


                QMap <QString,QString> parametros;
                //TODO parametros
                switch (valor) {
                case 1: // Impresora
                    Configuracion::ImprimirDirecto(report,parametros_sql,parametros);
                    break;
                case 2: // email
                    // TODO - enviar pdf por mail
                    break;
                case 3: // PDF
                    Configuracion::ImprimirPDF(report,parametros_sql,parametros);
                    break;
                case 4: //preview
                    Configuracion::ImprimirPreview(report,parametros_sql,parametros);
                    break;
                default:
                    break;
                }

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

void FrmPedidos::on_tabla_clicked(const QModelIndex &index)
{
    int id = ui->tabla->model()->data(ui->tabla->model()->index(index.row(),0),Qt::EditRole).toInt();
    oPedido->RecuperarPedido("select * from ped_cli where id ="+QString::number(id));
    //LLenarCampos();
    BloquearCampos(true);
}

void FrmPedidos::on_tabla_doubleClicked(const QModelIndex &index)
{
    int id = ui->tabla->model()->data(ui->tabla->model()->index(index.row(),0),Qt::EditRole).toInt();
    oPedido->RecuperarPedido("select * from ped_cli where id ="+QString::number(id));
    LLenarCampos();
    ui->stackedWidget->setCurrentIndex(0);
    ocultarBusqueda();
    BloquearCampos(true);
}


void FrmPedidos::on_btnImprimir_2_clicked()
{
    on_btnImprimir_clicked();
}


void FrmPedidos::on_spin_porc_dto_pp_editingFinished()
{
    float dto_pp;
    dto_pp = (Configuracion_global->MonedatoDouble(ui->txtsubtotal->text())*(ui->spin_porc_dto_pp->value()/100.0));
    ui->txtDto_pp->setText(Configuracion_global->toFormatoMoneda(QString::number(dto_pp,'f',Configuracion_global->decimales_campos_totales)));
    oPedido->dto_pp = dto_pp;
    helper.fillTable("empresa","lin_ped",QString("id_cab =%1").arg(oPedido->id));
}

void FrmPedidos::on_spin_porc_dto_especial_editingFinished()
{

    //--------------------------------------------
    // Cambio dto en líneas
    //--------------------------------------------
    QMap <int,QSqlRecord> rec;
    QString error;
    QStringList clauses;
    Articulo oArt(this);
    int id_art,id_lin;
    float porc_dto_esp, porc_dto_lin;
    double subtotal;

    clauses << QString("id_cab = %1").arg(oPedido->id);

    rec = SqlCalls::SelectRecord("lin_ped",clauses,Configuracion_global->empresaDB,error);
    QMapIterator <int, QSqlRecord> i(rec);
    while (i.hasNext())
    {
        i.next();
        id_lin = i.value().value("id").toInt();
        id_art = i.value().value("id_articulo").toInt();
        subtotal = i.value().value("subtotal").toDouble();
        porc_dto_lin = i.value().value("porc_dto").toFloat();

        porc_dto_esp = oArt.asigna_dto_linea(id_art,oCliente3->id,ui->spin_porc_dto_especial->value(),porc_dto_lin);
        QHash <QString,QVariant> f;
        clauses.clear();
        clauses << QString("id = %1").arg(id_lin);
        f["porc_dto"] = porc_dto_esp;
        f["dto"] = subtotal *(porc_dto_esp/100.0);

        bool upd = SqlCalls::SqlUpdate(f,"lin_ped",Configuracion_global->empresaDB,clauses,error);
        if(!upd)
            QMessageBox::warning(this,tr("Cambio DTO"),tr("No se pudo realizar el cambio en las líneas, error:%1").arg(error),
                                 tr("Aceptar"));
        else
            helper.calculatotal();
    }
    //--------------------------
    // Calculo dto total
    //--------------------------
    double dto = (Configuracion_global->MonedatoDouble(ui->txtsubtotal->text())*(ui->spin_porc_dto_especial->value()/100.0));
    ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales_campos_totales)));
    oPedido->dto = dto;
    QString filter = QString("id_cab = '%1'").arg(oPedido->id);
    helper.fillTable("empresa","lin_ped",filter);

}


void FrmPedidos::cboporc_iva_gasto1_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    calcular_iva_gastos();
}

void FrmPedidos::cboporc_iva_gasto2_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    calcular_iva_gastos();
}

void FrmPedidos::cboporc_iva_gasto3_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    calcular_iva_gastos();
}

void FrmPedidos::SpinGastoDist1_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    calcular_iva_gastos();
}
void FrmPedidos::SpinGastoDist2_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    calcular_iva_gastos();
}
void FrmPedidos::SpinGastoDist3_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    calcular_iva_gastos();
}

void FrmPedidos::mostrarBusqueda()
{
    _showBarraBusqueda(m_busqueda);
    m_busqueda->doFocustoText();
}

void FrmPedidos::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}
