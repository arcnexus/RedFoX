#include "frmfacturas.h"
#include "ui_frmfacturas.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Busquedas/frmBuscarFactura.h"
#include "../Zona_Contabilidad/apuntes.h"

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
        ui->txtirpf->setVisible(true);
        ui->txtimporte_irpf->setVisible(true);
        ui->txtimporte_irpf_2->setVisible(true);
        ui->lblIRPF->setVisible(true);
        ui->lblIRPF_2->setVisible(true);
    } else {
        ui->txtirpf->setVisible(false);
        ui->txtimporte_irpf->setVisible(false);
        ui->txtimporte_irpf_2->setVisible(false);
        ui->lblIRPF->setVisible(false);
        ui->lblIRPF_2->setVisible(false);
    }

    ui->comboPais->setModel(Configuracion_global->paises_model);
    ui->comboPais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de facturas a clientes"));
    // Pongo valores por defecto
    ui->lbcontabilizada->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lblFacturaImpresa->setVisible(false);
    // Rellenar formas de pago
    QSqlQueryModel *  modelFP = new QSqlQueryModel();
    modelFP->setQuery("Select forma_pago,id from formpago",QSqlDatabase::database("empresa"));
    ui->txtforma_pago->setModel(modelFP);
    // valores edicion
    this->Altas = false;
    //ui->txtcodigoArticulo->setFocus();
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

//    if(oFactura->RecuperarFactura("Select * from cab_fac where id > -1 order by factura  limit 1 "))
//    {
//        LLenarCampos();
//        QString filter = QString("id_Cab = '%1'").arg(oFactura->id);
//        helper.fillTable("empresa","lin_fac",filter);
//    }
//    else
//    {
        VaciarCampos();
      //  ui->btn_borrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btnSiguiente->setEnabled(true);
        ui->btnAnterior->setEnabled(false);
        ui->btnImprimir->setEnabled(false);
        ui->btnBuscar->setEnabled(false);
        oFactura->id = -1;
   // }
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
    ui->txtfecha->setDate(oFactura->fecha);
    ui->txtfecha_cobro->setDate(oFactura->fecha_cobro);
    ui->txtcliente->setText(oFactura->cliente);
    ui->txtdireccion1->setText(oFactura->direccion1);
    ui->txtdireccion2->setText(oFactura->direccion2);
    ui->txtcp->setText(oFactura->cp);
    ui->txtpoblacion->setText(oFactura->poblacion);
    ui->txtprovincia->setText(oFactura->provincia);

    QList<QString> keys = Configuracion_global->paises.uniqueKeys();
    for (int i = 0;i<keys.size();i++)
    {
        if(Configuracion_global->paises[keys.at(i)].value("id").toInt() == oFactura->id_pais)
        {
            int index = ui->comboPais->findText(keys.at(i));
            ui->comboPais->setCurrentIndex(index);
        }
    }

    ui->txtcif->setText(oFactura->cif);
     lEstado = oFactura->recargo_equivalencia;
    if ((lEstado== 1)) {
        ui->chkrecargo_equivalencia->setChecked(true);
    } else {
        ui->chkrecargo_equivalencia->setChecked(false);
    }
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda( QString::number(oFactura->subtotal,'f',2)));
    //ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->dto,'f',2)));
    //ui->txtdto_pp->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->dto_pp,'f',2)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->dto,'f',2)));
    //ui->txtimporte_descuento_pp->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->importe_descuento_pp,'f',2)));
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number( oFactura->base,'f',2)));
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number( oFactura->iva,'f',2)));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva,'f',2)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total,'f',2)));
    lEstado = oFactura->impreso;
    if((lEstado == 1)) {
        ui->lblFacturaImpresa->setVisible(true);
    } else {
        ui->lblFacturaImpresa->setVisible(false);
    }
    lEstado = oFactura->cobrado;
    if ((lEstado == 1)) {
        ui->lblFacturaCobrada->setVisible(true);
        ui->txtfecha_cobro->setVisible(true);
    } else {
        ui->lblFacturaCobrada->setVisible(false);
        ui->txtfecha_cobro->setVisible(false);
    }
    lEstado = oFactura->lContablilizada;

    if((lEstado == 1)) {
        ui->lbcontabilizada->setVisible(true);
    } else {
        ui->lbcontabilizada->setVisible(false);
    }
    int indice=ui->txtforma_pago->findText(oFactura->forma_pago);
    if(indice!=-1)
     ui->txtforma_pago->setCurrentIndex(indice);
    ui->txtcomentario->setText(oFactura->comentario);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base1,'f',2)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base2,'f',2)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base3,'f',2)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base4,'f',2)));
    ui->txtporc_iva1->setText(QString::number(oFactura->porc_iva1));
    ui->txtporc_iva2->setText(QString::number(oFactura->porc_iva2));
    ui->txtporc_iva3->setText(QString::number(oFactura->porc_iva3));
    ui->txtporc_iva4->setText(QString::number(oFactura->porc_iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva1,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva2,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva3,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva4,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total1,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total2,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total3,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total4,'f',2)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec1,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec2,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec3,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec4,'f',2)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec1,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec2,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec3,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->porc_rec4,'f',2)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total_recargo,'f',2)));
    ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->entregado_a_cuenta,'f',2)));
    ui->txtimporte_pendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->importe_pendiente,'f',2)));
    ui->txtcodigo_entidad->setText(oFactura->codigo_entidad);
    ui->txtoficina_entidad->setText(oFactura->oficina_entidad);
    ui->txtdc_cuenta->setText(oFactura->dc_cuenta);
    ui->txtnumero_cuenta->setText(oFactura->cuenta_corriente);
    ui->txtpedido_cliente->setText(QString::number(oFactura->pedido_cliente));
    if(oFactura->irpf!=0)
        ui->lblIRPF_3->setVisible(true);
    else
        ui->lblIRPF_3->setVisible(false);
    if(oFactura->cobrado==1){
        ui->lblFacturaCobrada->setVisible(true);
        ui->txtfecha_cobro->setVisible(true);
    } else {
        ui->lblFacturaCobrada->setVisible(false);
        ui->txtfecha_cobro->setVisible(false);
    }
    ui->txtirpf->setText(QString::number(oFactura->irpf));
    ui->txtimporte_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->irpf,'f',2)));
    ui->txtimporte_irpf_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->irpf,'f',2)));
    oCliente1->Recuperar("Select * from clientes where id ="+QString::number(oFactura->id_cliente));
    helper.set_tarifa(oCliente1->tarifa_cliente);
    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();
    oFactura->id_cliente = oCliente1->id;

    QString filter = QString("id_Cab = '%1'").arg(oFactura->id);
    helper.fillTable("empresa","lin_fac",filter);
}

void frmFacturas::LLenarCamposCliente()
{
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
    int lEstado = 0;
     lEstado = oCliente1->recargo_equivalencia;
    if ((lEstado= 1)) {
        ui->chkrecargo_equivalencia->setChecked(true);
    } else {
        ui->chkrecargo_equivalencia->setChecked(false);
    }
    if (oCliente1->lIRPF == true) {
        ui->lblIRPF_3->setVisible(true);
        ui->txtirpf->setText(QString::number(Configuracion_global->irpf));
        oFactura->irpf = (Configuracion_global->irpf);
    } else {
        ui->lblIRPF_3->setVisible(false);
        ui->txtirpf->setText("0,00");
        oFactura->irpf = (Configuracion_global->irpf);
    }
    oCliente1->Recuperar("Select * from clientes where id ="+QString::number(oFactura->id_cliente));
    helper.set_tarifa(oCliente1->tarifa_cliente);
    helper.porc_iva1 = ui->txtporc_iva1->text().toFloat();
    helper.porc_iva2 = ui->txtporc_iva2->text().toFloat();
    helper.porc_iva3 = ui->txtporc_iva3->text().toFloat();
    helper.porc_iva4 = ui->txtporc_iva4->text().toFloat();
    oFactura->id_cliente = oCliente1->id;
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

    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    ui->btnBuscar->setEnabled(state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btndeshacer->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->botBuscarCliente->setEnabled(!state);

    ui->btnAnadirLinea->setEnabled(!state);
    ui->btn_borrarLinea->setEnabled(!state);
    helper.blockTable(state);
    ui->txtfactura->setReadOnly(true);
}

void frmFacturas::LLenarFactura() {
    oFactura->codigo_cliente = (ui->txtcodigo_cliente->text());
    oFactura->factura = (ui->txtfactura->text());
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
    oFactura->subtotal = (ui->txtsubtotal->text().replace(",",".").replace(moneda,"").toDouble());
    //oFactura->dto = (ui->txtdto->text().replace(",",".").toDouble());
    //oFactura->dto_pp = (ui->txtdto_pp->text().replace(",",".").toDouble());
    oFactura->dto = (ui->txtimporte_descuento->text().replace(".","").replace(moneda,"").replace(",",".").toDouble());
    //oFactura->importe_descuento_pp = (ui->txtimporte_descuento_pp->text().replace(",",".").toDouble());
    oFactura->base = (ui->txtbase->text().replace(",",".").replace(moneda,"").toDouble());
    oFactura->iva = (ui->txtiva->text().replace(",",".").replace(moneda,"").toDouble());
    oFactura->total = (ui->txttotal->text().replace(",",".").replace(moneda,"").toDouble());
    oFactura->forma_pago = (ui->txtforma_pago->currentText());
    oFactura->comentario = (ui->txtcomentario->toPlainText());
    oFactura->base1 = (ui->txtbase1->text().replace(",",".").toDouble());
    oFactura->base2 = (ui->txtbase2->text().replace(",",".").toDouble());
    oFactura->base3 = (ui->txtbase3->text().replace(",",".").toDouble());
    oFactura->base4 = (ui->txtbase4->text().replace(",",".").toDouble());
    oFactura->porc_iva1 = (ui->txtporc_iva1->text().replace(",",".").toDouble());
    oFactura->porc_iva2 = (ui->txtporc_iva2->text().replace(",",".").toDouble());
    oFactura->porc_iva3 = (ui->txtporc_iva3->text().replace(",",".").toDouble());
    oFactura->porc_iva4 = (ui->txtporc_iva4->text().replace(",",".").toDouble());
    oFactura->iva1 = (ui->txtiva1->text().replace(",",".").toDouble());
    oFactura->iva2 = (ui->txtiva2->text().replace(",",".").toDouble());
    oFactura->iva3 = (ui->txtiva3->text().replace(",",".").toDouble());
    oFactura->iva4 = (ui->txtiva4->text().replace(",",".").toDouble());
    oFactura->total1 = (ui->txttotal1->text().replace(",",".").toDouble());
    oFactura->total2 = (ui->txttotal2->text().replace(",",".").toDouble());
    oFactura->total3 = (ui->txttotal3->text().replace(",",".").toDouble());
    oFactura->total4 = (ui->txttotal4->text().replace(",",".").toDouble());
    oFactura->porc_rec1 = (ui->txtporc_rec1->text().replace(",",".").toDouble());
    oFactura->porc_rec2 = (ui->txtporc_rec2->text().replace(",",".").toDouble());
    oFactura->porc_rec3 = (ui->txtporc_rec3->text().replace(",",".").toDouble());
    oFactura->porc_rec4 = (ui->txtporc_rec4->text().replace(",",".").toDouble());
    oFactura->porc_rec1 = (ui->txtporc_rec1->text().replace(",",".").toDouble());
    oFactura->porc_rec2 = (ui->txtporc_rec2->text().replace(",",".").toDouble());
    oFactura->porc_rec3 = (ui->txtporc_rec3->text().replace(",",".").toDouble());
    oFactura->porc_rec4 = (ui->txtporc_rec4->text().replace(",",".").toDouble());
    oFactura->total_recargo = (ui->txttotal_recargo->text().replace(",",".").toDouble());
    oFactura->entregado_a_cuenta = (ui->txtentregado_a_cuenta->text().replace(",",".").toDouble());
    oFactura->importe_pendiente = (ui->txtimporte_pendiente->text().replace(",",".").toDouble());
    oFactura->codigo_entidad = (ui->txtcodigo_entidad->text());
    oFactura->oficina_entidad = (ui->txtoficina_entidad->text());
    oFactura->dc_cuenta = (ui->txtdc_cuenta->text());
    oFactura->cuenta_corriente = (ui->txtnumero_cuenta->text());
    bool ok;
    int nPed = ui->txtpedido_cliente->text().toInt(&ok);

    oFactura->pedido_cliente = ok ? nPed : 0;
    oFactura->irpf = (ui->txtirpf->text().replace(",",".").toDouble());
    oFactura->irpf = (ui->txtimporte_irpf->text().replace(",",".").toDouble());
}

void frmFacturas::on_btnSiguiente_clicked()
{
    QString factura = QString::number(oFactura->id);
    if(oFactura->RecuperarFactura("Select * from cab_fac where id >'"+factura+"' order by factura  limit 1 "))
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
    QString factura = QString::number(oFactura->id);
    if(oFactura->RecuperarFactura("Select * from cab_fac where id <'"+factura+"' order by factura desc limit 1 "))
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

    QSqlDatabase::database("empresa").transaction();
    QSqlDatabase::database("dbconta").transaction();
    bool succes = true;
    LLenarFactura();

    if(sender()==actionGuardaBorrador)
        oFactura->factura = tr("BORRADOR");
    else
        if(oFactura->factura.isEmpty())
        oFactura->factura = oFactura->NuevoNumeroFactura();

    succes = oFactura->GuardarFactura(oFactura->id,false);
    if(succes)
    {
        LLenarCampos();
        if(Configuracion_global->contabilidad && oFactura->factura !=tr("BORRADOR"))
            succes = crear_asiento();
        if(!succes)

            QMessageBox::critical(this,tr("Error"),
                                  tr("Error al crear el asiento contable")+QSqlDatabase::database("empresa").lastError().text(),
                                  tr("&Aceptar"));
    }
    if(succes)
    {
        if(Configuracion_global->contabilidad)
            QSqlDatabase::database("dbconta").commit();
        QSqlDatabase::database("empresa").commit();
        TimedMessageBox * t = new TimedMessageBox(this,tr("Factura guardada con éxito"));
        BloquearCampos(true);
        emit unblock();
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),
                              tr("Error al guardar la factura.\n")+QSqlDatabase::database("empresa").lastError().text(),
                              tr("&Aceptar"));
        QSqlDatabase::database("empresa").rollback();
        if(Configuracion_global->contabilidad)
            QSqlDatabase::database("dbconta").rollback();
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
        oCliente1->Recuperar("select * from clientes where id="+QString::number(id));
        LLenarCamposCliente();
    }

}

void frmFacturas::on_btnBuscar_clicked()
{
    FrmBuscarFactura BuscarFactura;
    BuscarFactura.exec();
    int nid = BuscarFactura.Devolverid();
    QString cid = QString::number(nid);
    oFactura->RecuperarFactura("Select * from cab_fac where id ="+cid+" limit 1 ");
    LLenarCampos();
}

void frmFacturas::on_btnImprimir_clicked()
{
   // KDReports::Report report;
    //TODO Reports
}

void frmFacturas::totalChanged(double base, double dto, double subtotal, double iva, double re, double total, QString moneda)
{
    this->moneda = moneda;
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',2))+moneda);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',2))+moneda);
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);
}

void frmFacturas::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void frmFacturas::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void frmFacturas::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void frmFacturas::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void frmFacturas::on_btnEditar_clicked()
{
    bool editar = true;
    if (oFactura->apunte >0)
    {
        QMessageBox::warning(this,tr("Facturas de clientes"),tr("No se puede modificar una factura que ha sido contabilizada"),
                             tr("Aceptar"));
        editar = false;
    }
    if(editar && oFactura->cobrado)
    {
        QMessageBox::warning(this,tr("Facturas de clientes"),tr("No se puede modificar una factura que ha sido cobrada"),
                             tr("Aceptar"));
        editar = false;        
    }
    if(editar && oFactura->impreso)
    {
        QMessageBox::warning(this,tr("Facturas de clientes"),tr("No se puede modificar una factura que ha sido impresa"),
                             tr("Aceptar"));
        editar = false;
    }
        
    if(editar)
    {
        BloquearCampos(false);
        in_edit = true;
        emit block();
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
        query_lin_ped_pro.prepare("INSERT INTO lin_fac (id_Cab, id_articulo, codigo, cantidad,"
                                  "descripcion, precio, subtotal, porc_dto,"
                                  " dto, porc_iva, total)"
                                  "VALUES (:id_cab,:id_articulo,:codigo,:cantidad,"
                                  ":descripcion,:precio,:subtotal_coste,:porc_dto,"
                                  ":dto,:porc_iva,:total);");
        query_lin_ped_pro.bindValue(":id_cab", oFactura->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":precio",ld->precio);
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
        //TODO control de stock alb clientes

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
        //TODO control de stock editando en alb clientes
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery query_lin_ped_pro(QSqlDatabase::database("empresa"));
        query_lin_ped_pro.prepare("UPDATE lin_fac SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo =:codigo_articulo_proveedor,"
                                  "descripcion =:descripcion,"
                                  "cantidad =:cantidad,"
                                  "precio =:precio,"
                                  "subtotal =:subtotal_coste,"
                                  "porc_dto =:porc_dto,"
                                  "dto =:dto,"
                                  "porc_iva =:porc_iva,"
                                  "porc_rec =:porc_rec,"
                                  "total =:total "
                                  "WHERE id = :id;");

        query_lin_ped_pro.bindValue(":id_cab", oFactura->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":precio",ld->precio);
        query_lin_ped_pro.bindValue(":subtotal_coste",ld->subtotal);
        query_lin_ped_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_ped_pro.bindValue(":dto",ld->dto);
        query_lin_ped_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_ped_pro.bindValue(":porc_rec",ld->rec_perc);
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

void frmFacturas::lineaDeleted(lineaDetalle * ld)
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
        q.prepare("delete from lin_fac where id =:id");
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



void frmFacturas::on_tabWidget_2_currentChanged(int index)
{
    Q_UNUSED(index);
    helper.resizeTable();
}


bool frmFacturas::crear_asiento()
{
    bool creado = oFactura->Apunte();
    return creado;
}

void frmFacturas::on_btndeshacer_clicked()
{
    QSqlDatabase::database("Maya").rollback();
    QSqlDatabase::database("empresa").rollback();
    LLenarCampos();
    BloquearCampos(true);
}
