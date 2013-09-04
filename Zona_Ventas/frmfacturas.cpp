#include "frmfacturas.h"
#include "ui_frmfacturas.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Busquedas/frmBuscarFactura.h"
#include "../Zona_Contabilidad/apuntes.h"
#include "../Auxiliares/frmaddentregascuenta.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/datedelegate.h"
#include "../Almacen/frmarticulos.h"

#include "../Almacen/articulo.h"

frmFacturas::frmFacturas( QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmFacturas),
    helper(this),
    toolButton(tr("Facturas"),":/Icons/PNG/Factura.png",this),
    menuButton(QIcon(":/Icons/PNG/Factura.png"),tr("Facturas"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/Factura.png"),"",this))

{
    oFactura = new Factura();
    oCliente1 = new Cliente();
    ui->setupUi(this);
    // Escondo/muestro campos según configuración
    if(Configuracion_global->lProfesional) {
        ui->txtimporte_irpf->setVisible(true);
        ui->txtimporte_irpf_2->setVisible(true);
        ui->lblIRPF_2->setVisible(true);
    } else {
        ui->txtimporte_irpf->setVisible(false);
        ui->txtimporte_irpf_2->setVisible(false);
        ui->lblIRPF_2->setVisible(false);
    }

    ui->comboPais->setModel(Configuracion_global->paises_model);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de facturas a clientes"));
    // Pongo valores por defecto
    ui->lbcontabilizada->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lblFacturaImpresa->setVisible(false);
    //-------------------------
    // Rellenar formas de pago
    //-------------------------
    QSqlQueryModel *  modelFP = new QSqlQueryModel(this);
    modelFP->setQuery("Select forma_pago,id from formpago",Configuracion_global->groupDB);
    ui->txtforma_pago->setModel(modelFP);
    // valores edicion
    this->Altas = false;
    //ui->txtcodigoArticulo->setFocus();
    ui->stackedWidget->setCurrentIndex(1);


    // --------------------------
    // Ver Facturas o Borradores
    // --------------------------
    ui->cboVer->addItem(tr("Facturas"));
    ui->cboVer->addItem(tr("Borradores"));

    // --------------------------
    // campo busquedas
    // --------------------------
    h_Buscar["Factura"]="factura";
    h_Buscar["Código cliente"] = "codigo_cliente";
    h_Buscar["Cliente"] = "cliente";
    h_Buscar["Fecha Factura"] = "fecha";
    h_Buscar["CIF / NIF"] = "cif";
    h_Buscar["Total"] = "total";

    ui->cboBuscar->addItem("Factura");
    ui->cboBuscar->addItem("Código cliente");
    ui->cboBuscar->addItem("Cliente");
    ui->cboBuscar->addItem("Fecha Factura");
    ui->cboBuscar->addItem("CIF/NIF");
    ui->cboBuscar->addItem("Total");

    //-----------------
    // MODO
    //-----------------
    QStringList modo;
    modo  <<tr("Z-A") << tr("A-Z");
    ui->cboModo->addItems(modo);

    //-----------------
    // tablafacturas
    //-----------------
    m_facturas = new QSqlQueryModel(this);
    m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                         " where factura <> 'BORRADOR'  and ejercicio = "+Configuracion_global->cEjercicio+
                         " order by fecha+serie+factura desc limit 0,500",Configuracion_global->empresaDB);
    ui->tabla_facturas->setModel(m_facturas);
    formato_tabla_facturas(*m_facturas);

    //----------------
    // Cargar Series
    //----------------
    QSqlQueryModel * model_series = new QSqlQueryModel(this);
    model_series->setQuery("select serie from series",Configuracion_global->empresaDB);
    ui->cbo_serie->setModel(model_series);
    QSqlQuery series2(Configuracion_global->empresaDB);
    series2.exec("select serie from series");
    QStringList series2_l;
    series2_l.append(tr("TODAS"));
    while (series2.next())
        series2_l.append(series2.record().value("serie").toString());
    ui->cboseries->addItems(series2_l);
    if(!Configuracion_global->serie.isEmpty())
    {
        int index = ui->cboseries->findText(Configuracion_global->serie);
        ui->cboseries->setCurrentIndex(index);
    }

    BloquearCampos(true);
    /* -----------------------------------------
     *CONEXIONES
     *----------------------------------------*/
    //connect(ui->txtcodigoArticulo,SIGNAL(editingFinished()),this,SLOT(on_txtcodigoArticulo_lostFocus()));

    helper.set_Tipo(false);
    helper.help_table(ui->Lineas);

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

    helper.set_tarifa(Configuracion_global->id_tarifa_predeterminada);

    connect(ui->chkrecargo_equivalencia,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));


    actionGuardaBorrador = new QAction("Guardar borrador",this);
    actionGuardaFactura = new QAction("Guardar factura",this);
    menu_guardar = new QMenu(this);

    connect(actionGuardaBorrador,SIGNAL(triggered()),this,SLOT(Guardar_factura()));
    connect(actionGuardaFactura,SIGNAL(triggered()),this,SLOT(Guardar_factura()));

    menu_guardar->addAction(actionGuardaFactura);
    menu_guardar->addAction(actionGuardaBorrador);

    ui->btnGuardar->setMenu(menu_guardar);

    VaciarCampos();
    ui->btnEditar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);
    ui->btnAnterior->setEnabled(false);
    ui->btnImprimir->setEnabled(false);
    ui->btnBuscar->setEnabled(true);
    ui->cboVer->setEnabled(true);
    ui->cboBuscar->setEnabled(true);
    ui->txtBuscar->setEnabled(true);
    ui->txtBuscar->setReadOnly(false);
    ui->cboseries->setEnabled(true);
    oFactura->id = -1;

    //-----------------------
    //

}

frmFacturas::~frmFacturas()
{
    delete ui;
    delete oCliente1;
    delete oFactura;
}

void frmFacturas::LLenarCampos() {
    int lEstado;
    ui->lblFactura->setText(oFactura->factura);
    ui->lblCliente->setText(oFactura->cliente);
    ui->txtcodigo_cliente->setText(oFactura->codigo_cliente);
    ui->txtfactura->setText(oFactura->factura);
    int index = ui->cbo_serie->findText(oFactura->serie);
    ui->cbo_serie->setCurrentIndex(index);
    ui->txtfecha->setDate(oFactura->fecha);
    ui->txtfecha_cobro->setDate(oFactura->fecha_cobro);
    ui->txtcliente->setText(oFactura->cliente);
    ui->txtdireccion1->setText(oFactura->direccion1);
    ui->txtdireccion2->setText(oFactura->direccion2);
    ui->txtcp->setText(oFactura->cp);
    ui->txtpoblacion->setText(oFactura->poblacion);
    ui->txtprovincia->setText(oFactura->provincia);
    index = ui->comboPais->findText(Configuracion_global->Devolver_pais(oFactura->id_pais));
    ui->comboPais->setCurrentIndex(index);

    ui->txtcif->setText(oFactura->cif);
     lEstado = oFactura->recargo_equivalencia;
    if ((lEstado== 1)) {
        ui->chkrecargo_equivalencia->setChecked(true);
    } else {
        ui->chkrecargo_equivalencia->setChecked(false);
    }
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda( QString::number(oFactura->subtotal,'f',Configuracion_global->decimales)));
    //ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->dto,'f',Configuracion_global->decimales)));
    //ui->txtdto_pp->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->dto_pp,'f',Configuracion_global->decimales)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->dto,'f',Configuracion_global->decimales)));
    //ui->txtimporte_descuento_pp->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->importe_descuento_pp,'f',Configuracion_global->decimales)));
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number( oFactura->base,'f',Configuracion_global->decimales)));
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number( oFactura->iva,'f',Configuracion_global->decimales)));
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total_recargo,'f',Configuracion_global->decimales)));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva,'f',Configuracion_global->decimales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total,'f',Configuracion_global->decimales)));

    if((oFactura->impreso)) {
        ui->lblFacturaImpresa->setVisible(true);
    } else {
        ui->lblFacturaImpresa->setVisible(false);
    }
    if ((oFactura->cobrado )) {
        ui->lblFacturaCobrada->setVisible(true);
    } else {
        ui->lblFacturaCobrada->setVisible(false);
    }
    if((oFactura->contablilizada)) {
        ui->lbcontabilizada->setVisible(true);
    } else {
        ui->lbcontabilizada->setVisible(false);
    }
    int indice=ui->txtforma_pago->findText(oFactura->forma_pago);
    if(indice!=-1)
     ui->txtforma_pago->setCurrentIndex(indice);
    ui->txtcomentario->setText(oFactura->comentario);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base1,'f',Configuracion_global->decimales)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base2,'f',Configuracion_global->decimales)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base3,'f',Configuracion_global->decimales)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base4,'f',Configuracion_global->decimales)));
    ui->txtporc_iva1->setText(QString::number(oFactura->porc_iva1));
    ui->txtporc_iva2->setText(QString::number(oFactura->porc_iva2));
    ui->txtporc_iva3->setText(QString::number(oFactura->porc_iva3));
    ui->txtporc_iva4->setText(QString::number(oFactura->porc_iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva1,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva2,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva3,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva4,'f',Configuracion_global->decimales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total1,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total2,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total3,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total4,'f',Configuracion_global->decimales)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec1,'f',Configuracion_global->decimales)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec2,'f',Configuracion_global->decimales)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec3,'f',Configuracion_global->decimales)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec4,'f',Configuracion_global->decimales)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec1,'f',Configuracion_global->decimales)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec2,'f',Configuracion_global->decimales)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec3,'f',Configuracion_global->decimales)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec4,'f',Configuracion_global->decimales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total_recargo,'f',Configuracion_global->decimales)));
    ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->entregado_a_cuenta,'f',Configuracion_global->decimales)));
    ui->txtimporte_pendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->importe_pendiente,'f',Configuracion_global->decimales)));
    ui->txtcodigo_entidad->setText(oFactura->codigo_entidad);
    ui->txtoficina_entidad->setText(oFactura->oficina_entidad);
    ui->txtdc_cuenta->setText(oFactura->dc_cuenta);
    ui->txtnumero_cuenta->setText(oFactura->cuenta_corriente);
    ui->txtpedido_cliente->setText(QString::number(oFactura->pedido_cliente));
    if(!oFactura->irpf)
        ui->lblIRPF_3->setVisible(true);
    else
        ui->lblIRPF_3->setVisible(false);
    if(oFactura->cobrado){
        ui->lblFacturaCobrada->setVisible(true);
    } else {
        ui->lblFacturaCobrada->setVisible(false);
    }
    ui->txtimporte_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->irpf,'f',Configuracion_global->decimales)));
    ui->txtimporte_irpf_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->irpf,'f',Configuracion_global->decimales)));
    oCliente1->Recuperar("Select * from clientes where id ="+QString::number(oFactura->id_cliente));
    ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente1->importe_a_cuenta,'f',Configuracion_global->decimales)));
    helper.set_tarifa(oCliente1->tarifa_cliente);
    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();
    oFactura->id_cliente = oCliente1->id;
    ui->txtTransportista->setText( Configuracion_global->devolver_transportista(oFactura->id_transportista));
    ui->txtAsiento->setText(QString::number(oFactura->apunte));
    ui->txt_tipo_dto_tarifa->setText(QString::number(oCliente1->tipo_dto_tarifa));

    ui->spin_porc_iva_gasto1->setValue(oFactura->porc_iva_gasto1);
    ui->spin_porc_iva_gasto2->setValue(oFactura->porc_iva_gasto2);
    ui->spin_porc_iva_gasto3->setValue(oFactura->porc_iva_gasto3);
    ui->spin_iva_gasto1->setValue(oFactura->iva_gasto1);
    ui->spin_iva_gasto2->setValue(oFactura->iva_gasto2);
    ui->spin_iva_gasto3->setValue(oFactura->iva_gasto3);

    QString filter = QString("id_Cab = '%1'").arg(oFactura->id);
    helper.fillTable("empresa","lin_fac",filter);
    helper.set_tipo_dto_tarifa(oCliente1->tipo_dto_tarifa);
}

void frmFacturas::LLenarCamposCliente()
{
    oCliente1->Recuperar("select * from clientes where id=" +QString::number(oFactura->id_cliente));
    ui->lblCliente->setText(oCliente1->nombre_fiscal);
    ui->txtcodigo_cliente->setText(oCliente1->codigo_cliente);
    ui->txtcliente->setText(oCliente1->nombre_fiscal);
    ui->txtdireccion1->setText(oCliente1->direccion1);
    ui->txtdireccion2->setText(oCliente1->direccion2);
    ui->txtcp->setText(oCliente1->cp);
    ui->txtpoblacion->setText(oCliente1->poblacion);
    ui->txtprovincia->setText(oCliente1->provincia);
    //ui->txtpais->setText(oCliente1->getpais());
    ui->txtcif->setText(oCliente1->cif_nif);
    ui->chkrecargo_equivalencia->setChecked(oCliente1->recargo_equivalencia);

    if(oCliente1->recargo_equivalencia)
    {
        ui->txtporc_rec1->setText(QString::number(Configuracion_global->reList.at(0).toFloat(),'f',2));
        ui->txtporc_rec2->setText(QString::number(Configuracion_global->reList.at(1).toFloat(),'f',2));
        ui->txtporc_rec3->setText(QString::number(Configuracion_global->reList.at(2).toFloat(),'f',2));
        ui->txtporc_rec4->setText(QString::number(Configuracion_global->reList.at(3).toFloat(),'f',2));
    } else
    {
        ui->txtporc_rec1->setText("0.00");
        ui->txtporc_rec2->setText("0.00");
        ui->txtporc_rec3->setText("0.00");
        ui->txtporc_rec4->setText("0.00");
    }


    if (Configuracion_global->irpf == true) {
        ui->lblIRPF_3->setVisible(true);
        oFactura->irpf = (Configuracion_global->irpf);
    } else {
        ui->lblIRPF_3->setVisible(false);
        oFactura->irpf = (Configuracion_global->irpf);
    }
    oCliente1->Recuperar("Select * from clientes where id ="+QString::number(oFactura->id_cliente));
    ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente1->importe_a_cuenta,'f',Configuracion_global->decimales)));
    helper.set_tarifa(oCliente1->tarifa_cliente);

    helper.porc_iva1 = ui->txtporc_iva1->text().toFloat();
    helper.porc_iva2 = ui->txtporc_iva2->text().toFloat();
    helper.porc_iva3 = ui->txtporc_iva3->text().toFloat();
    helper.porc_iva4 = ui->txtporc_iva4->text().toFloat();

    oFactura->id_cliente = oCliente1->id;
    oFactura->id_transportista = oCliente1->id_transportista;
    ui->txtTransportista->setText( Configuracion_global->devolver_transportista(oCliente1->id_transportista));
    ui->txtGastoDist1->setText(oFactura->desc_gasto1);
    ui->txtGastoDist2->setText(oFactura->desc_gasto2);
    ui->txtGastoDist3->setText(oFactura->desc_gasto3);
    ui->SpinGastoDist1->setValue(oFactura->imp_gasto1);
    ui->SpinGastoDist2->setValue(oFactura->imp_gasto2);
    ui->SpinGastoDist3->setValue(oFactura->imp_gasto3);
    ui->txt_tipo_dto_tarifa->setText(QString::number(oCliente1->tipo_dto_tarifa));
    helper.set_tipo_dto_tarifa(oCliente1->tipo_dto_tarifa);
}

void frmFacturas::VaciarCampos()
{
    QDate fecha;
    ui->lblCliente->setText("");
    ui->lblFactura->setText("");
    ui->txtcodigo_cliente->setText("");
    ui->txtfactura->setText("");
    ui->txtfecha->setDate(fecha.currentDate());
    ui->txtfecha_cobro->setDate(fecha.currentDate());
    ui->txtcliente->setText("");
    ui->txtdireccion1->setText("");
    ui->txtdireccion2->setText("");
    ui->txtcp->setText("");
    ui->txtpoblacion->setText("");
    ui->txtprovincia->setText("");
    //ui->txtpais->setText("");
    ui->txtcif->setText("");
    ui->txtsubtotal->setText(0);
    //ui->txtdto->setText(0);
    //ui->txtdto_pp->setText(0);
    ui->txtimporte_descuento->setText("0,00");
    //ui->txtimporte_descuento_pp->setText("0,00");
    ui->txtbase->setText("0,00");
    ui->txtiva_total->setText(0);
    ui->txtiva->setText("0,00");
    ui->txttotal->setText("0,00");
    ui->lblFacturaImpresa->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lbcontabilizada->setVisible(false);
    int indice=ui->txtforma_pago->findText("Contado");
    if(indice!=-1) ui->txtforma_pago->setCurrentIndex(indice);
    ui->txtcomentario->setText("");
    ui->txtbase1->setText(0);
    ui->txtbase2->setText(0);
    ui->txtbase3->setText(0);
    ui->txtbase4->setText(0);

    ui->txtporc_iva1->setText(QString::number(Configuracion_global->ivaList.at(0).toFloat()));
    ui->txtporc_iva2->setText(QString::number(Configuracion_global->ivaList.at(1).toFloat()));
    ui->txtporc_iva3->setText(QString::number(Configuracion_global->ivaList.at(2).toFloat()));
    ui->txtporc_iva4->setText(QString::number(Configuracion_global->ivaList.at(3).toFloat()));
    ui->txtporc_rec1->setText(QString::number(Configuracion_global->reList.at(0).toFloat()));
    ui->txtporc_rec2->setText(QString::number(Configuracion_global->reList.at(1).toFloat()));
    ui->txtporc_rec3->setText(QString::number(Configuracion_global->reList.at(2).toFloat()));
    ui->txtporc_rec4->setText(QString::number(Configuracion_global->reList.at(3).toFloat()));
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
    ui->txttotal_recargo->setText(0);
    ui->txtentregado_a_cuenta->setText("0,00");
    ui->txtimporte_pendiente->setText("0,00");
    ui->txtcodigo_entidad->setText("");
    ui->txtoficina_entidad->setText("");
    ui->txtdc_cuenta->setText("");
    ui->txtnumero_cuenta->setText("");
    ui->txtpedido_cliente->setText("");
    ui->chkrecargo_equivalencia->setCheckState(Qt::Unchecked);
    ui->txtGastoDist1->setText("");
    ui->txtGastoDist2->setText("");
    ui->txtGastoDist3->setText("");
    ui->SpinGastoDist1->setValue(0);
    ui->SpinGastoDist2->setValue(0);
    ui->SpinGastoDist3->setValue(0);

    helper.fillTable("empresa","lin_fac","id_Cab = -1");
}

void frmFacturas::BloquearCampos(bool state)
{
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

    // Qdoblespinbox
    QList<QDoubleSpinBox *> SpinEditList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *SpinEdit;
    foreach (SpinEdit, SpinEditList) {
        SpinEdit->setEnabled(!state);
    }

    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    ui->btnBuscar->setEnabled(state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btndeshacer->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->botBuscarCliente->setEnabled(!state);
    ui->btnImprimir->setEnabled(true);
    ui->btnCobrar->setEnabled(state);
    ui->btnAnadirLinea->setEnabled(!state);
    ui->btn_borrarLinea->setEnabled(!state);
    helper.blockTable(state);
    ui->txtfactura->setReadOnly(true);
    ui->btnAsignarTransportista->setEnabled(!state);
    ui->btnBuscar->setEnabled(state);
    ui->cboVer->setEnabled(state);
    ui->cboBuscar->setEnabled(state);
    ui->txtBuscar->setEnabled(state);
    ui->txtBuscar->setReadOnly(!state);
    ui->cboseries->setEnabled(state);
    ui->cboModo->setEnabled(state);
}

void frmFacturas::LLenarFactura() {
    oFactura->codigo_cliente = (ui->txtcodigo_cliente->text());
    oFactura->factura = (ui->txtfactura->text());
    oFactura->serie = ui->cbo_serie->currentText();
    oFactura->fecha = (ui->txtfecha->date());
    oFactura->fecha_cobro = (ui->txtfecha_cobro->date());
    oFactura->cliente = (ui->txtcliente->text());
    oFactura->direccion1 = (ui->txtdireccion1->text());
    oFactura->direccion2 = (ui->txtdireccion2->text());
    oFactura->cp = (ui->txtcp->text());
    oFactura->poblacion = (ui->txtpoblacion->text());
    oFactura->provincia = (ui->txtprovincia->text());
    oFactura->id_pais = Configuracion_global->paises[ui->comboPais->currentText()].value("id").toInt();
    oFactura->cif = (ui->txtcif->text());
    if  (ui->chkrecargo_equivalencia->isChecked())
    {
        oFactura->recargo_equivalencia = (1);
    }
    else
    {
        oFactura->recargo_equivalencia = (0);
    }
    oFactura->subtotal = (ui->txtsubtotal->text().replace(".","").replace(",",".").replace(moneda,"").toDouble());
    oFactura->porc_dto_pp = (ui->spinPorc_dto->text().replace(".","").replace(",",".").toDouble());
    oFactura->dto = (ui->txtimporte_descuento->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oFactura->dto_pp = (ui->spinPorc_dto_pp->text().replace(".","").replace(",",".").toDouble());
    oFactura->base = (ui->txtbase->text().replace(".","").replace(",",".").replace(moneda,"").toDouble());
    oFactura->iva = (ui->txtiva->text().replace(".","").replace(",",".").replace(moneda,"").toDouble());
    oFactura->total = (ui->txttotal->text().replace(".","").replace(",",".").replace(moneda,"").toDouble());
    oFactura->forma_pago = (ui->txtforma_pago->currentText());
    oFactura->comentario = (ui->txtcomentario->toPlainText());
    oFactura->base1 = (ui->txtbase1->text().replace(".","").replace(",",".").toDouble());
    oFactura->base2 = (ui->txtbase2->text().replace(".","").replace(",",".").toDouble());
    oFactura->base3 = (ui->txtbase3->text().replace(".","").replace(",",".").toDouble());
    oFactura->base4 = (ui->txtbase4->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_iva1 = (ui->txtporc_iva1->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_iva2 = (ui->txtporc_iva2->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_iva3 = (ui->txtporc_iva3->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_iva4 = (ui->txtporc_iva4->text().replace(".","").replace(",",".").toDouble());
    oFactura->iva1 = (ui->txtiva1->text().replace(".","").replace(",",".").toDouble());
    oFactura->iva2 = (ui->txtiva2->text().replace(".","").replace(",",".").toDouble());
    oFactura->iva3 = (ui->txtiva3->text().replace(".","").replace(",",".").toDouble());
    oFactura->iva4 = (ui->txtiva4->text().replace(".","").replace(",",".").toDouble());
    oFactura->total1 = (ui->txttotal1->text().replace(".","").replace(",",".").toDouble());
    oFactura->total2 = (ui->txttotal2->text().replace(".","").replace(",",".").toDouble());
    oFactura->total3 = (ui->txttotal3->text().replace(".","").replace(",",".").toDouble());
    oFactura->total4 = (ui->txttotal4->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_rec1 = (ui->txtporc_rec1->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_rec2 = (ui->txtporc_rec2->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_rec3 = (ui->txtporc_rec3->text().replace(".","").replace(",",".").toDouble());
    oFactura->porc_rec4 = (ui->txtporc_rec4->text().replace(".","").replace(",",".").toDouble());
    oFactura->rec1 = (ui->txtrec1->text().replace(".","").replace(",",".").toDouble());
    oFactura->rec2 = (ui->txtrec2->text().replace(".","").replace(",",".").toDouble());
    oFactura->rec3 = (ui->txtrec3->text().replace(".","").replace(",",".").toDouble());
    oFactura->rec4 = (ui->txtrec4->text().replace(".","").replace(",",".").toDouble());
    oFactura->total_recargo = (ui->txttotal_recargo->text().replace(".","").replace(",",".").toDouble());
    oFactura->entregado_a_cuenta = (ui->txtentregado_a_cuenta->text().replace(".","").replace(",",".").toDouble());
    oFactura->importe_pendiente = (ui->txtimporte_pendiente->text().replace(".","").replace(",",".").toDouble());
    oFactura->codigo_entidad = (ui->txtcodigo_entidad->text());
    oFactura->oficina_entidad = (ui->txtoficina_entidad->text());
    oFactura->dc_cuenta = (ui->txtdc_cuenta->text());
    oFactura->cuenta_corriente = (ui->txtnumero_cuenta->text());
    bool ok;
    int nPed = ui->txtpedido_cliente->text().toInt(&ok);

    oFactura->pedido_cliente = ok ? nPed : 0;
    oFactura->irpf = (ui->txtimporte_irpf->text().replace(".","").replace(",",".").toDouble());
    oFactura->desc_gasto1 = ui->txtGastoDist1->text();
    oFactura->desc_gasto2 = ui->txtGastoDist2->text();
    oFactura->desc_gasto3 = ui->txtGastoDist3->text();
    oFactura->imp_gasto1 = ui->SpinGastoDist1->value();
    oFactura->imp_gasto2 = ui->SpinGastoDist2->value();
    oFactura->imp_gasto3 = ui->SpinGastoDist3->value();
}

void frmFacturas::



on_btnSiguiente_clicked()
{
    QString id = QString::number(oFactura->id);
    QString cSQL;
    if (ui->cboVer->currentText() == "Borradores")
        cSQL ="select * from cab_fac "
                " where factura = 'BORRADOR' and id > "+id+" and ejercicio = "+Configuracion_global->cEjercicio+
                             " order by id limit 0,1";
    else
        cSQL ="select * from cab_fac "
                " where factura <> 'BORRADOR' and factura > '"+oFactura->factura+"' and ejercicio = "+Configuracion_global->cEjercicio+
                             " order by factura limit 0,1";

    if(oFactura->RecuperarFactura(cSQL))
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oFactura->id);
        helper.fillTable("empresa","lin_fac",filter);
       // ui->btn_borrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
        ui->btnAnterior->setEnabled(true);
        ui->btnImprimir->setEnabled(true);
        ui->btnCobrar->setEnabled(true);
    }
    else
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Se ha llegado al final del archivo.\n"
                                                          "No hay mas facturas disponibles"));
        VaciarCampos();
      //  ui->btn_borrar->setEnabled(false);
        //ui->btnEditar->setEnabled(false);
        ui->btnSiguiente->setEnabled(false);
        oFactura->id++;
    }
}
void frmFacturas::on_btnAnterior_clicked()
{
    QString id = QString::number(oFactura->id);
    QString cSQL;
    if (ui->cboVer->currentText() == "Borradores")
        cSQL ="select * from cab_fac "
                " where factura = 'BORRADOR' and id < "+id+" and ejercicio = "+Configuracion_global->cEjercicio+
                             " order by id desc limit 0,1";
    else
        cSQL ="select * from cab_fac "
                " where factura <> 'BORRADOR' and factura <'"+oFactura->factura+"' and ejercicio = "+Configuracion_global->cEjercicio+
                             " order by factura desc limit 0,1";
    if(oFactura->RecuperarFactura(cSQL))
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oFactura->id);
        helper.fillTable("empresa","lin_fac",filter);
        //ui->btn_borrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
        ui->btnSiguiente->setEnabled(true);
    }
    else
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Se ha llegado al final del archivo.\n"
                                                          "No hay mas presupuestos disponibles"));
        VaciarCampos();
     //   ui->btn_borrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btnAnterior->setEnabled(false);
        oFactura->id = -1;
    }
}


void frmFacturas::Guardar_factura()
{

    Configuracion_global->empresaDB.transaction();
    Configuracion_global->contaDB.transaction();
    bool succes = true;
    LLenarFactura();

    if(sender()==actionGuardaBorrador)
        oFactura->factura = tr("BORRADOR");
    else
        if(oFactura->factura.isEmpty())
        oFactura->factura = oFactura->NuevoNumeroFactura(ui->cbo_serie->currentText());

    succes = oFactura->GuardarFactura(oFactura->id,false);
    if(succes)
    {
        LLenarCampos();
        if(Configuracion_global->contabilidad && oFactura->factura !=tr("BORRADOR"))
            succes = crear_asiento();
        if(!succes)

            QMessageBox::critical(this,tr("Error"),
                                  tr("Error al crear el asiento contable")+Configuracion_global->empresaDB.lastError().text(),
                                  tr("&Aceptar"));
        else
            ui->txtAsiento->setText(QString::number(oFactura->apunte));
    }
    if(succes)
    {
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.commit();
        Configuracion_global->empresaDB.commit();
        TimedMessageBox * t = new TimedMessageBox(this,tr("Factura guardada con éxito"));
        BloquearCampos(true);
        emit unblock();
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),
                              tr("Error al guardar la factura.\n")+Configuracion_global->empresaDB.lastError().text(),
                              tr("&Aceptar"));
        Configuracion_global->empresaDB.rollback();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.rollback();
    }

}

void frmFacturas::on_btnAnadir_clicked()
{
    ui->btnGuardar->setMenu(menu_guardar);
    BloquearCampos(false);
    in_edit = false;
    VaciarCampos();    
    ui->chkrecargo_equivalencia->setCheckState(Qt::Unchecked);
    ui->txtcodigo_cliente->setFocus();
    oFactura->AnadirFactura();
    helper.set_tarifa(Configuracion_global->id_tarifa_predeterminada);
    ui->stackedWidget->setCurrentIndex(0);
    helper.resizeTable();
    emit block();
}

void frmFacturas::on_btnDeshacer_clicked()
{
    BloquearCampos(true);
    QString cid = QString::number(oFactura->id);
    oFactura->RecuperarFactura("Select * from cab_fac where id ="+cid+" order by id limit 1 ");
    LLenarCampos();
    emit unblock();
}

void frmFacturas::on_botBuscarCliente_clicked()
{
    blockSignals(true);
    oCliente1->clear();
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
        oCliente1->Recuperar("select * from clientes where id="+QString::number(id));
        oFactura->id_cliente = id;
        LLenarCamposCliente();
    }
    blockSignals(false);

}

void frmFacturas::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void frmFacturas::on_btnImprimir_clicked()
{
    oCliente1->Recuperar(oFactura->id_cliente);
    FrmDialogoImprimir dlg_print(this);
    dlg_print.set_email(oCliente1->email);
    dlg_print.set_preview(false);
    if(dlg_print.exec() == dlg_print.Accepted)
    {
        bool ok = oFactura->set_impresa(true);
        if(ok)
        {
            ui->lblFacturaImpresa->setVisible(true);
            int valor = dlg_print.get_option();
             QMap <QString,QVariant> parametros;

             parametros["id_cliente"]= oFactura->id_cliente;
             parametros["id_cab"] = oFactura->id;
             parametros["id_empresa"] = 1;
             parametros["id_transportista"] =1;

            switch (valor) {
            case 1: // Impresora
                //Configuracion_global->imprimir("factura_"+QString::number(oCliente1->ididioma),false,false,parametros,this);
                break;
            case 2: // email
                // TODO - enviar pdf por mail
                break;
            case 3: // PDF
                //Configuracion_global->imprimir("factura_"+QString::number(oCliente1->ididioma),true,false,parametros,this);
                break;
            case 4: //preview
                //Configuracion_global->imprimir("factura_"+QString::number(oCliente1->ididioma),false,true,parametros,this);
                break;
            default:
                break;
            }

        }
    }
}

void frmFacturas::totalChanged(double base, double dto, double subtotal, double iva, double re, double total, QString moneda)
{
    if(!ui->chkrecargo_equivalencia->isChecked())
        re = 0;
    this->moneda = moneda;
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total+iva+re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total+iva+re,'f',Configuracion_global->decimales_campos_totales)));

}

void frmFacturas::desglose1Changed(double base, double iva, double re, double total)
{
    if(!ui->chkrecargo_equivalencia->isChecked())
        re = 0;
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total+iva+re,'f',Configuracion_global->decimales)));
}

void frmFacturas::desglose2Changed(double base, double iva, double re, double total)
{
    if(!ui->chkrecargo_equivalencia->isChecked())
        re = 0;
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total+iva+re,'f',Configuracion_global->decimales)));
}

void frmFacturas::desglose3Changed(double base, double iva, double re, double total)
{
    if(!ui->chkrecargo_equivalencia->isChecked())
        re = 0;
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total+iva+re,'f',Configuracion_global->decimales)));
}

void frmFacturas::desglose4Changed(double base, double iva, double re, double total)
{
    if(!ui->chkrecargo_equivalencia->isChecked())
        re = 0;
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total+iva+re,'f',Configuracion_global->decimales)));
}

void frmFacturas::on_btnEditar_clicked()
{
    if (oFactura->editable)
    {
        BloquearCampos(false);
        in_edit = true;
        emit block();
        ui->txtcodigo_cliente->setFocus();
    }
}

void frmFacturas::lineaReady(lineaDetalle * ld)
{
    //-----------------------------------------------------
    // Insertamos línea de pedido y controlamos acumulados
    //-----------------------------------------------------

    bool ok_empresa,ok_Maya;
    ok_empresa = true;
    ok_Maya = true;
    Configuracion_global->empresaDB.transaction();
    Configuracion_global->groupDB.transaction();
    //------------
    //Nueva linea
    //------------
    if (ld->idLinea == -1)
    {
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;
        QSqlQuery q_lin_fac(Configuracion_global->empresaDB);
        q_lin_fac.prepare("INSERT INTO lin_fac (id_Cab, id_articulo, codigo, cantidad,"
                                  "descripcion, precio, subtotal, porc_dto,"
                                  " dto, porc_iva,iva,porc_rec,rec,total)"
                                  "VALUES (:id_cab,:id_articulo,:codigo,:cantidad,"
                                  ":descripcion,:precio,:subtotal,:porc_dto,"
                                  ":dto,:porc_iva,:iva,:porc_rec,:rec,:total);");
        q_lin_fac.bindValue(":id_cab", oFactura->id);
        q_lin_fac.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        q_lin_fac.bindValue(":codigo",ld->codigo);
        q_lin_fac.bindValue(":descripcion",ld->descripcion);
        q_lin_fac.bindValue(":cantidad",ld->cantidad);
        q_lin_fac.bindValue(":precio",ld->precio);
        q_lin_fac.bindValue(":subtotal",ld->subtotal);
        q_lin_fac.bindValue(":porc_dto",ld->dto_perc);
        q_lin_fac.bindValue(":porc_iva",ld->iva_perc);
        double base = ld->subtotal - (ld->subtotal*ld->dto);
        q_lin_fac.bindValue(":iva",base * ld->iva_perc/100);
        q_lin_fac.bindValue(":porc_rec",ld->rec_perc);
        q_lin_fac.bindValue(":rec",base *ld->rec_perc/100);
        q_lin_fac.bindValue(":dto",ld->dto);
        q_lin_fac.bindValue(":total",ld->total);
        if (!q_lin_fac.exec()){
            ok_empresa = false;
            QMessageBox::warning(this,tr("Gestión de albaranes"),
                                 tr("Ocurrió un error al insertar la nueva línea: %1").arg(q_lin_fac.lastError().text()),
                                 tr("Aceptar"));
        } else{
            double pendiente = ui->txtimporte_pendiente->text().replace(".","").replace(",",".").toDouble();
            pendiente += ld->total;
            ui->txtimporte_pendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(pendiente,'f',Configuracion_global->decimales)));
        }

        //TODO control de stock alb clientes

        if(queryArticulos.exec() && ok_empresa){
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        } else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
        }

        ld->idLinea = q_lin_fac.lastInsertId().toInt();

    } else // Editando linea
    {
        //TODO control de stock editando en alb clientes
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery q_lin_fac(Configuracion_global->empresaDB);
        q_lin_fac.prepare("UPDATE lin_fac SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo =:codigo,"
                                  "descripcion =:descripcion,"
                                  "cantidad =:cantidad,"
                                  "precio =:precio,"
                                  "subtotal =:subtotal,"
                                  "porc_dto =:porc_dto,"
                                  "dto =:dto,"
                                  "porc_iva =:porc_iva,"
                                  "iva=:iva,"
                                  "porc_rec =:porc_rec,"
                                  "rec =:rec,"
                                  "total =:total "
                                  "WHERE id = :id;");

        q_lin_fac.bindValue(":id_cab", oFactura->id);
        q_lin_fac.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        q_lin_fac.bindValue(":codigo",ld->codigo);
        q_lin_fac.bindValue(":descripcion",ld->descripcion);
        q_lin_fac.bindValue(":cantidad",ld->cantidad);
        q_lin_fac.bindValue(":precio",ld->precio);
        q_lin_fac.bindValue(":subtotal",ld->subtotal);
        q_lin_fac.bindValue(":porc_dto",ld->dto_perc);
        q_lin_fac.bindValue(":dto",ld->dto);
        q_lin_fac.bindValue(":porc_iva",ld->iva_perc);
        double base = ld->subtotal - (ld->subtotal*ld->dto);
        q_lin_fac.bindValue(":iva",base * ld->iva_perc/100);
        q_lin_fac.bindValue(":porc_rec",ld->rec_perc);
        q_lin_fac.bindValue(":rec",base *ld->rec_perc/100);
        q_lin_fac.bindValue(":total",ld->total);
        q_lin_fac.bindValue(":id",ld->idLinea);

        if (!q_lin_fac.exec()) {
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Ocurrió un error al guardar la línea: %1").arg(q_lin_fac.lastError().text()),
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

void frmFacturas::lineaDeleted(lineaDetalle * ld)
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
        q.prepare("delete from lin_fac where id =:id");
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



void frmFacturas::on_tabWidget_2_currentChanged(int index)
{
    Q_UNUSED(index);
    helper.resizeTable();
}


bool frmFacturas::crear_asiento()
{
    bool creado;
    oCliente1->Recuperar("select * from clientes where id = "+QString::number(oFactura->id_cliente));
    if(oFactura->apunte == 0)
        creado = oFactura->Apunte();
    else
        creado = oFactura->EditApunte(oFactura->apunte);
    return creado;
}

void frmFacturas::on_btndeshacer_clicked()
{
    Configuracion_global->groupDB.rollback();
    Configuracion_global->empresaDB.rollback();
    LLenarCampos();
    BloquearCampos(true);
}

void frmFacturas::on_txtcodigo_cliente_editingFinished()
{
    if((ui->txtcodigo_cliente->text() != oFactura->codigo_cliente)&& !ui->txtcodigo_cliente->text().isEmpty()){
        oCliente1->Recuperar("select * from clientes where codigo_cliente='"+ui->txtcodigo_cliente->text()+"'");
        LLenarCamposCliente();
        helper.set_tarifa(oCliente1->tarifa_cliente);
    }
}

void frmFacturas::on_anadirEntrega_clicked()
{
    frmAddEntregasCuenta entregas(this);
    entregas.set_id_cliente(oCliente1->id);
    entregas.set_concepto(tr("A cta ntra factura: ")+ui->txtfactura->text());
    if(entregas.exec() ==QDialog::Accepted)
    {
        oCliente1->Recuperar("select * from clientes where codigo_cliente='"+ui->txtcodigo_cliente->text()+"'");
        ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente1->importe_a_cuenta)));

    }

}



void frmFacturas::on_radBuscar_toggled(bool checked)
{
    if (checked)
        ui->stackedWidget->setCurrentIndex(1);
    else
        ui->stackedWidget->setCurrentIndex(0);
}

void frmFacturas::on_radEditar_toggled(bool checked)
{
    if(checked)
    {
        helper.resizeTable();
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
        ui->stackedWidget->setCurrentIndex(1);
}

void frmFacturas::on_cboVer_currentTextChanged(const QString &arg1)
{
    m_facturas = new QSqlQueryModel(this);
    QString indice_tabla = h_Buscar.value(arg1);
    if(indice_tabla.isEmpty())
        indice_tabla = "factura";
    QString cSQL;
    QString modo;
    if (ui->cboModo->currentText() == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";
    QString order;
    if(indice_tabla == "factura")
        order = "fecha+serie+factura";
    else
        order = indice_tabla;
    if(ui->cboseries->currentText() == tr("TODAS"))
    {
        if (ui->cboVer->currentText() == "Borradores")
            m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where factura = 'BORRADOR' and "+indice_tabla+
                                 " like '%"+ui->txtBuscar->text()+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " order by "+order +" "+modo,Configuracion_global->empresaDB);
        else
            m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where factura <> 'BORRADOR' and "+indice_tabla+
                                 " like '%"+ui->txtBuscar->text()+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " order by "+order +" "+modo,Configuracion_global->empresaDB);
    } else
    {
        if (ui->cboVer->currentText() == "Borradores")
            m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where factura = 'BORRADOR' and "+indice_tabla+
                                 " like '%"+ui->txtBuscar->text()+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " and serie ='"+ui->cboseries->currentText() +"'"+
                                 " order by "+order +" "+modo,Configuracion_global->empresaDB);
        else
            m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where factura <> 'BORRADOR' and "+indice_tabla+
                                 " like '%"+ui->txtBuscar->text()+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " and serie ='"+ui->cboseries->currentText() +"'"+
                                 " order by "+order +" "+modo,Configuracion_global->empresaDB);
    }

    m_facturas->setQuery(cSQL,Configuracion_global->empresaDB);
    ui->tabla_facturas->setModel(m_facturas);
    formato_tabla_facturas(*m_facturas);
}

void frmFacturas::on_txtBuscar_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    filtro_tabla();
}

void frmFacturas::formato_tabla_facturas(QSqlQueryModel &modelo)
{
    ui->tabla_facturas->setItemDelegateForColumn(6, new MonetaryDelegate);
    ui->tabla_facturas->setItemDelegateForColumn(3,new DateDelegate);
    ui->tabla_facturas->setItemDelegateForColumn(4, new DateDelegate);
    ui->tabla_facturas->setColumnHidden(0,true);
    QVariantList lista;
    QStringList  titulos;
    lista << 0 << 50<< 120 <<120 <<120 <<120 <<120 <<300;
    titulos <<tr("id") <<tr("S") << tr("FACTURA") <<tr("FECHA") << tr("FECHA COBRO") << tr("CIF/NIF") << tr("TOTAL") << tr("CLIENTE");
    for(int pos = 0;pos<lista.size();pos++)
    {
        ui->tabla_facturas->setColumnWidth(pos,lista.at(pos).toInt());
        modelo.setHeaderData(pos,Qt::Horizontal,titulos.at(pos));
    }
}

void frmFacturas::filtro_tabla()
{
    QString indice_tabla = h_Buscar.value(ui->cboBuscar->currentText());
    if(indice_tabla.isEmpty())
        indice_tabla = "factura";
    QString modo;
    if (ui->cboModo->currentText() == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";
    QString order;
    if(indice_tabla == "factura")
        order = "fecha+serie+factura";
    else
        order = indice_tabla;

    if(ui->cboseries->currentText() == tr("TODAS"))
    {
        if (ui->cboVer->currentText() == "Borradores")
            m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where factura = 'BORRADOR' and "+indice_tabla+
                                 " like '%"+ui->txtBuscar->text()+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " order by "+order+" " +modo,Configuracion_global->empresaDB);
        else
            m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where factura <> 'BORRADOR' and "+indice_tabla+
                                 " like '%"+ui->txtBuscar->text()+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " order by "+order +" "+modo,Configuracion_global->empresaDB);
    } else
    {
        if (ui->cboVer->currentText() == "Borradores")
            m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where factura = 'BORRADOR' and "+indice_tabla+
                                 " like '%"+ui->txtBuscar->text()+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " and serie ='"+ui->cboseries->currentText() +"'"+
                                 " order by "+order +" "+modo,Configuracion_global->empresaDB);
        else
            m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where factura <> 'BORRADOR' and "+indice_tabla+
                                 " like '%"+ui->txtBuscar->text()+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " and serie ='"+ui->cboseries->currentText() +"'"
                                 " order by "+order +" "+modo,Configuracion_global->empresaDB);

    }
    //qDebug() << m_facturas->query().lastQuery();

    ui->tabla_facturas->setModel(m_facturas);
    formato_tabla_facturas(*m_facturas);

}

bool frmFacturas::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj == ui->txtcodigo_cliente)
        {
            if(keyEvent->key() == Qt::Key_F1)
                on_botBuscarCliente_clicked();
        }
        if(obj == ui->txtcp)
        {
            if(keyEvent->key() == Qt::Key_F1)
                buscar_poblacion(1);
        }
        if(obj == ui->txtCp_entrega)
        {
            if(keyEvent->key() == Qt::Key_F1)
                buscar_poblacion(2);
        }
        return false;
    }
}

void frmFacturas::buscar_poblacion(int tipo)
{
    // 1 general // 2 Entrega
    db_consulta_view consulta;
    QStringList campos;
    campos  <<"poblacion" <<"cp" << "provinca";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("poblaciones");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,poblacion,cp,provincia from poblaciones");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Población") << tr("C.P.") << tr("Provincia");
    tamanos <<"0" << "300" << "70" << "150";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Poblaciones");
    if(consulta.exec())
    {
        int id = consulta.get_id();
        QMap <int,QSqlRecord> m;
        QString error;
        int index;
        m = SqlCalls::SelectRecord("poblaciones",QString("id=%1").arg(id),Configuracion_global->groupDB,error);
        if(error.isEmpty())
            if(tipo == 1)
            {
                ui->txtcp->setText(m.value(id).value("cp").toString());
                ui->txtpoblacion->setText(m.value(id).value("poblacion").toString());
                ui->txtprovincia->setText(m.value(id).value("provincia").toString());
                index = ui->comboPais->findText(Configuracion_global->Devolver_pais(m.value(id).value("id_pais").toInt()));
                ui->comboPais->setCurrentIndex(index);

            } else
            {
                ui->txtCp_entrega->setText(m.value(id).value("cp").toString());
                ui->txtPoblacion_entrega->setText(m.value(id).value("poblacion").toString());
                ui->txtProvincia_entrega->setText(m.value(id).value("provincia").toString());
                index = ui->cboPais_entrega->findText(Configuracion_global->Devolver_pais(m.value(id).value("id_pais").toInt()));
                ui->cboPais_entrega->setCurrentIndex(index);

            }
        else
            QMessageBox::warning(this,tr("Gestión Facturas"),tr("Falló la recuperación de la población : %1").arg(error),
                                 tr("Aceptar"));
    }

}

void frmFacturas::on_btnAsignarTransportista_clicked()
{
    db_consulta_view consulta;
    QStringList campos;
    campos <<"codigo" <<"transportista" << "telefono1" <<"movil" << "poblacion"<< "provincia" << "pais";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("transportistas");
    consulta.set_db("Group");
    consulta.set_SQL("select id,codigo,transportista,telefono1,movil,poblacion,provincia,pais from transportista");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Código") << tr("Transportista") << tr("Teléfono") << tr("Movil") << tr("Población") << tr("Provincia") << tr("Pais");

    tamanos <<"0" << "100" << "250" << "120" << "120" <<"150" << "90" << "90";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de transportistas");
    if(consulta.exec())
    {
        oFactura->id_transportista = consulta.get_id();
        ui->txtTransportista->setText(Configuracion_global->devolver_transportista(oFactura->id_transportista));
   }
}

void frmFacturas::on_tabla_facturas_doubleClicked(const QModelIndex &index)
{
     int id = ui->tabla_facturas->model()->data(ui->tabla_facturas->model()->index(index.row(),0),Qt::EditRole).toInt();
     oFactura->RecuperarFactura(id);
     LLenarCampos();
     ui->stackedWidget->setCurrentIndex(0);
     ui->btnEditar->setEnabled(true);
     ui->btnImprimir->setEnabled(true);
}

void frmFacturas::on_tabla_facturas_clicked(const QModelIndex &index)
{
    int id = ui->tabla_facturas->model()->data(ui->tabla_facturas->model()->index(index.row(),0),Qt::EditRole).toInt();
    oFactura->RecuperarFactura(id);
    //LLenarCampos();

}


void frmFacturas::on_btnArticulos_clicked()
{
    FrmArticulos artic;
    artic.exec();
}



void frmFacturas::on_cboseries_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    filtro_tabla();
}

void frmFacturas::on_btnClear_clicked()
{
    ui->cboseries->setCurrentIndex(0);
    ui->cboBuscar->setCurrentIndex(0);
    ui->txtBuscar->setText("");
    m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                         " where factura <> 'BORRADOR'  and ejercicio = "+Configuracion_global->cEjercicio+
                         " order by fecha+serie+factura desc limit 0,500",Configuracion_global->empresaDB);
    ui->txtBuscar->setFocus();

}

void frmFacturas::on_cboBuscar_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    filtro_tabla();

}

void frmFacturas::on_cboModo_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    filtro_tabla();
}
