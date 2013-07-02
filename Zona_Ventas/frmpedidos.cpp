#include "frmpedidos.h"
#include "ui_frmpedidos.h"


#include "../Almacen/articulo.h"

#include "../Busquedas/frmbuscarcliente.h"
#include "../Zona_Ventas/factura.h"

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

    if(oPedido->RecuperarPedido("Select * from ped_cli where pedido > -1 order by pedido  limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(ui->txtpedido->text());
        helper.fillTable("empresa","lin_ped",filter);
    }
    else
    {
        VaciarCampos();
        ui->btn_borrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btn_convertir->setEnabled(false);
        ui->btnSiguiente->setEnabled(false);
        ui->btnAnterior->setEnabled(false);
        ui->btnImprimir->setEnabled(false);
        ui->btnBuscar->setEnabled(false);
        oPedido->id = -1;
    }
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
    ui->txtimporte_descuento->setText(QString::number(oPedido->dto));
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

    ui->lbimpreso->setVisible(oPedido->impreso == 1);

    ui->lbfacturado->setVisible(oPedido->facturado == 1);
    ui->txtfecha_factura->setVisible(oPedido->facturado == 1);
    ui->txtcNumFra->setVisible(oPedido->facturado == 1);
    ui->txtcNumFra->setText(oPedido->factura);
    ui->txtfecha_factura->setDate(oPedido->fecha_factura);

    ui->txtcomentario->setText(oPedido->comentario);
    ui->txtentregado_a_cuenta->setText(QString::number(oPedido->entregado_a_cuenta));
    //oPedido->lTraspasadoAlb;
    //oPedido->lTraspasadoFac;
    ui->txtdireccion1Entrega->setText(oPedido->direccion1Entrega);
    ui->txtdireccion1Entrega_2->setText(oPedido->direccion1Entrega2);
    ui->txtcpEntrega->setText(oPedido->cpEntrega);
    ui->txtpoblacionEntrega->setText(oPedido->poblacionEntrega);
    ui->txtprovinciaEntrega->setText(oPedido->provinciaEntrega);
    ui->txtpaisEntrega->setText(oPedido->paisEntrega);
    ui->chklEnviado->setChecked(oPedido->lEnviado==1);
    ui->chklCompleto->setChecked(oPedido->lCompleto==1);
    ui->chklEntregado->setChecked(oPedido->lEntregado==1);
    ui->txtfechaLimiteEntrega->setDate(oPedido->fechaLimiteEntrega);
    ui->txttotal->setText(QString::number(oPedido->totalPedido));
}

void FrmPedidos::LLenarCamposCliente()
{
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
    ui->txtimporte_descuento->setText("0,00");
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
    ui->txttotal_recargo->setText(0);
    ui->txtentregado_a_cuenta->setText("0,00");
    ui->txtpedido_cliente->setText("");
    ui->txttotal_iva_2->setText("0,00");
    ui->txtbase_total_2->setText("0,00");
    ui->txttotal_recargo_2->setText("0,00");
    ui->txttotal_2->setText("0,00");
    ui->txtsubtotal->setText("0,00");
    ui->txtpedido->setReadOnly(true);

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
    //ui->btnDeshacer->setEnabled(!state);
    ui->btnEditar->setEnabled(state);    
    ui->btnGuardar->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->botBuscarCliente->setEnabled(!state);
    ui->txtpedido->setReadOnly(true);
    ui->btn_borrar->setEnabled(state);
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
    oPedido->subtotal=ui->txtsubtotal->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->dto=ui->txtimporte_descuento->text().replace(_moneda,"").replace(",",".").toDouble();
    //oPedido->dto;
    oPedido->base1=ui->txtbase1->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->base2=ui->txtbase2->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->base3=ui->txtbase3->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->base4=ui->txtbase4->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->porc_iva1=ui->txtporc_iva1->text().replace(_moneda,"").replace(",",".").toDouble();

    oPedido->porc_iva2=ui->txtporc_iva2->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->porc_iva3=ui->txtporc_iva3->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->porc_iva4=ui->txtporc_iva4->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->iva1=ui->txtiva1->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->iva2=ui->txtiva2->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->iva3=ui->txtiva3->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->iva4=ui->txtiva4->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->porc_rec1=ui->txtporc_rec1->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->porc_rec2=ui->txtporc_rec2->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->porc_rec3=ui->txtporc_rec3->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->porc_rec4=ui->txtporc_rec4->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->rec1=ui->txtporc_rec1->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->rec2=ui->txtporc_rec2->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->rec3=ui->txtporc_rec3->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->rec4=ui->txtporc_rec4->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->total1=ui->txttotal1->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->total2=ui->txttotal2->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->total3=ui->txttotal3->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->total4=ui->txttotal4->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->base_total=ui->txtbase_total_2->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->iva_total=ui->txttotal_iva_2->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->rec_total=ui->txttotal_recargo_2->text().replace(_moneda,"").replace(",",".").toDouble();
    oPedido->total_albaran=ui->txttotal_2->text().replace(_moneda,"").replace(",",".").toDouble();

    oPedido->impreso = ui->lbimpreso->isVisible();

   // oPedido->factura=ui->txtcNumFra->text().toDouble();
    oPedido->fecha_factura=ui->txtfecha_factura->date();

    oPedido->comentario=ui->txtcomentario->toPlainText();
    oPedido->entregado_a_cuenta=ui->txtentregado_a_cuenta->text().replace(_moneda,"").replace(",",".").toDouble();
    //oPedido->lTraspasadoAlb;
    //oPedido->lTraspasadoFac;
    oPedido->direccion1Entrega=ui->txtdireccion1Entrega->text();
    oPedido->direccion1Entrega2=ui->txtdireccion1Entrega_2->text();
    oPedido->cpEntrega=ui->txtcpEntrega->text();
    oPedido->poblacionEntrega=ui->txtpoblacionEntrega->text();
    oPedido->provinciaEntrega=ui->txtprovinciaEntrega->text();
    oPedido->paisEntrega=ui->txtpaisEntrega->text();
    oPedido->lEnviado=ui->chklEnviado->isChecked();
    oPedido->lCompleto=ui->chklCompleto->isChecked();
    oPedido->lEntregado=ui->chklEntregado->isChecked();
    oPedido->fechaLimiteEntrega=ui->txtfechaLimiteEntrega->date();
    oPedido->totalPedido=ui->txttotal->text().replace(_moneda,"").replace(",",".").toDouble();;
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
    QSqlDatabase::database("empresa").transaction();
    bool succes = true;

    succes &= oPedido->GuardarPedido(oPedido->id);

    if(succes)
    {
        succes = QSqlDatabase::database("empresa").commit();
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
                              tr("Error al guardar el pedido.\n")+QSqlDatabase::database("empresa").lastError().text(),
                              tr("&Aceptar"));
        QSqlDatabase::database("empresa").rollback();
    }
}

void FrmPedidos::on_botBuscarCliente_clicked()
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
        oPedido->id_cliente = cid.toInt();
        oCliente3->Recuperar("Select * from clientes where id ="+cid+" order by id limit 1 ");
        helper.set_tarifa(oCliente3->idTarifa);
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
        QSqlDatabase::database("empresa").transaction();

        QSqlQuery q(QSqlDatabase::database("empresa"));
        succes &= oPedido->BorrarLineas(oPedido->id);
        q.prepare("DELETE FROM ped_cli WHERE id = "+QString::number(oPedido->id));
        succes &= q.exec();

        //TODO control de stock
        if(succes)
            succes &= QSqlDatabase::database("empresa").commit();

        if(succes)
        {
            TimedMessageBox * t = new TimedMessageBox(this,tr("Pedido borrado con éxito"));
            VaciarCampos();
            oPedido->id=-1;
            on_btnSiguiente_clicked();
        }
        else
            QSqlDatabase::database("empresa").rollback();
    }
}

void FrmPedidos::totalChanged(double base , double dto ,double subtotal , double iva, double re, double total, QString moneda)
{
    _moneda = moneda;
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',2))+moneda);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',2))+moneda);
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);
}

void FrmPedidos::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmPedidos::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmPedidos::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmPedidos::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmPedidos::lineaReady(lineaDetalle * ld)
{
    //-----------------------------------------------------
    // Insertamos línea de pedido y controlamos acumulados
    //-----------------------------------------------------

    bool ok_empresa,ok_Maya;
    ok_empresa = true;
    ok_Maya = true;
    QSqlDatabase::database("empresa").transaction();
    QSqlDatabase::database("Maya").transaction();
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
        query_lin_ped_pro.prepare("INSERT INTO lin_ped (id_Cab,id_articulo,codigo,"
                                  "descripcion, cantidad, pvp,subtotal,dto,dto,porc_iva,"
                                  "total,cantidadaservir) VALUES (:id_cab,:id_articulo,:codigo_articulo_proveedor,"
                                  ":descripcion,:cantidad,:coste_bruto,:subtotal_coste,:porc_dto,:dto,"
                                  ":porc_iva,:total,:cantidad_pendiente);");
        query_lin_ped_pro.bindValue(":id_cab", oPedido->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":cantidad_pendiente",ld->cantidad);
        query_lin_ped_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_ped_pro.bindValue(":subtotal_coste",ld->subtotal);
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
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;
        QSqlQuery query_lin_ped_pro(QSqlDatabase::database("empresa"));

        query_lin_ped_pro.prepare("UPDATE lin_ped SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo =:codigo_articulo_proveedor,"
                                  "descripcion =:descripcion,"
                                  "cantidad =:cantidad,"
                                  "cantidadaservir =:cantidad_pendiente,"
                                  "pvp =:coste_bruto,"
                                  "subtotal =:subtotal_coste,"
                                  "dto =:porc_dto,"
                                  "dto =:dto,"
                                  "porc_iva =:porc_iva,"
                                  "total =:total "
                                  "WHERE id = :id;");

        query_lin_ped_pro.bindValue(":id_cab", oPedido->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":cantidadaservir",ld->cantidad);
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
        //TODO control stock en pedidos cli
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

void FrmPedidos::lineaDeleted(lineaDetalle * ld)
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
        q.prepare("delete from lin_ped where id =:id");
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

void FrmPedidos::convertir_ealbaran()
{
}

void FrmPedidos::convertir_enFactura()
{
    if(oPedido->factura.isEmpty())
    {
        if(QMessageBox::question(this,tr("Pedidos a proveedores"),tr("¿Desea realmente facturar este pedido?"),
                                 tr("No"),tr("Sí"))==QMessageBox::Accepted)
        {
            LLenarPedido();
            Factura oFactura(this);
            QSqlDatabase::database("empresa").transaction();
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
            oFactura.numero_cuenta = oCliente3->cuenta_corriente;
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
            oFactura.importe_descuento = oPedido->dto;
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
            oFactura.total = oPedido->totalPedido;
            oFactura.total1 = oPedido->total1;
            oFactura.total2 = oPedido->total2;
            oFactura.total3 = oPedido->total3;
            oFactura.total4 = oPedido->total4;
            oFactura.total_recargo = oPedido->rec_total;

            // ----------------------------------
            // Creamos la cabecera de la factura
            //-----------------------------------

            oFactura.factura = oFactura.NuevoNumeroFactura();
            oFactura.GuardarFactura(oFactura.id,true);
            //-------------------
            //  INSERTAR LÍNEAS
            //-------------------
            QSqlQuery lineas_ped(QSqlDatabase::database("empresa"));
            QSqlQuery lineas_fac(QSqlDatabase::database("empresa"));

            if(lineas_ped.exec("Select * from lin_ped where id_Cab ="+QString::number(oPedido->id)))
            {
                while (lineas_ped.next()) {
                    lineas_fac.prepare("INSERT INTO lin_fac(id_Cab, id_articulo, codigo, cantidad,"
                                       "descripcion, pvp, subtotal, dto, dto, porc_iva, total)"
                                       " VALUES (:id_Cab, :id_articulo,:codigo,:cantidad,"
                                       ":descripcion,:pvp,:subtotal,:dto,:dto,:porc_iva,"
                                       ":total);");
                    lineas_fac.bindValue(":id_Cab", oFactura.id);
                    lineas_fac.bindValue(":id_articulo",lineas_ped.record().value("id_articulo").toInt());
                    lineas_fac.bindValue(":codigo",lineas_ped.record().value("codigo").toString());
                    lineas_fac.bindValue(":cantidad", lineas_ped.record().value("cantidad").toInt());
                    lineas_fac.bindValue(":descripcion",lineas_ped.record().value("descripcion").toString());
                    lineas_fac.bindValue(":pvp",lineas_ped.record().value("pvp").toDouble());
                    lineas_fac.bindValue(":subtotal",lineas_ped.record().value("subtotal").toDouble());
                    lineas_fac.bindValue(":dto",lineas_ped.record().value("dto").toDouble());
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
                           tr("\n y de importe: ")+QString::number(oFactura.total,'f',2);
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
                QSqlDatabase::database("empresa").commit();
            } else
            {
                QSqlDatabase::database("empresa").rollback();
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
