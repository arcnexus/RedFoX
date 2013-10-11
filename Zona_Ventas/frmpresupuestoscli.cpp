#include "frmpresupuestoscli.h"
#include "ui_frmpresupuestoscli.h"
#include <QSqlQueryModel>

#include "../Almacen/articulo.h"
#include "../Busquedas/db_consulta_view.h"
#include <QMessageBox>
#include "pedidos.h"
#include "albaran.h"
#include "factura.h"
#include "../vencimientos.h"


FrmPresupuestosCli::FrmPresupuestosCli(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmPresupuestosCli),
    helper(this),
    menuButton(QIcon(":/Icons/PNG/presupuestos.png"),tr("Presupuestos"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/presupuestos.png"),"",this))

{
    ui->setupUi(this);

    ui->cbo_Pais->setModel(Configuracion_global->paises_model);
    ui->cbo_Pais->setCurrentIndex(-1);
    ui->cboPais_entrega->setModel(Configuracion_global->paises_model);
    ui->cboPais_entrega->setCurrentIndex(-1);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de presupuestos a clientes"));

    //------------------------
    // cargar datos FormaPago
    //------------------------
    QSqlQueryModel *fp = new QSqlQueryModel(this);
    fp->setQuery("select forma_pago from formpago",Configuracion_global->groupDB);
    ui->cboFormaPago->setModel(fp);
    ui->cboFormaPago->setCurrentIndex(-1);


    //------------------------
    // Objetos necesarios
    //------------------------
    oPres = new Presupuesto();
    oClientePres = new Cliente();

    helper.set_Tipo(false);
    helper.help_table(ui->tableWidget);

    //---------------------
    // Conexiones
    //---------------------

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
    //-----------------------
    // tabla
    //-----------------------
    m = new QSqlQueryModel(this);
    m->setQuery("select id,presupuesto,fecha,telefono,movil,cliente from cab_pre order by presupuesto desc",
                Configuracion_global->empresaDB);
    ui->tabla->setModel(m);
    formato_tabla();

       //----------------------
    // Busqueda
    //----------------------
    ui->stackedWidget->setCurrentIndex(1);

    //---------------------
    // Eventos
    //---------------------
    ui->txtcodigo_cliente->installEventFilter(this);
    ui->txtcp->installEventFilter(this);
    ui->txtCP_entrega->installEventFilter(this);

    setUpBusqueda();
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
    int index;
    index = ui->cbo_Pais->findText(Configuracion_global->Devolver_pais(oPres->id_pais));
    ui->cbo_Pais->setCurrentIndex(index);
    ui->txtdireccion1_entrega->setText(oPres->direccion1_entrega);
    ui->txtdireccion2_entrega->setText(oPres->direccion2_entrega);
    ui->txtCP_entrega->setText(oPres->cp_entrega);
    ui->txtPoblacion_entrega->setText(oPres->poblacion_entrega);
    ui->txtProvincia_entrega->setText(oPres->provincia_entrega);
    index = ui->cboPais_entrega->findText(Configuracion_global->Devolver_pais(oPres->id_pais_entrega));
    ui->cboPais_entrega->setCurrentIndex(index);
    ui->txtEmail_entrega->setText(oPres->email_entrega);
    ui->txtComentarios_entrega->setPlainText(oPres->comentarios_entrega);
    ui->txttelefono->setText(oPres->telefono);
    ui->txtmovil->setText(oPres->movil);
    ui->txtfax->setText(oPres->fax);
    ui->txtcomentario->setPlainText(oPres->comentarios);
    ui->spin_porc_dto->setValue(oPres->porc_dto);
    ui->spin_porc_dto_pp->setValue(oPres->porc_dto_pp);
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->dto,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total,'f',Configuracion_global->decimales_campos_totales)));
    ui->lbimpreso->setVisible(oPres->impreso);
    ui->chklAprovado->setChecked(oPres->aprobado);
    ui->txtfechaAprovacion->setDate(oPres->fecha_aprobacion);
    ui->txtimporte_factura->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->importe_factura,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtfactura->setText(oPres->factura);
    ui->txtalbaran->setText(QString::number(oPres->albaran));
    ui->txtpedido->setText(QString::number(oPres->pedido));
    ui->txtGastoDist1->setText(oPres->desc_gasto1);
    ui->txtGastoDist2->setText(oPres->desc_gasto2);
    ui->txtGastoDist3->setText(oPres->desc_gasto3);
    ui->SpinGastoDist1->setValue(oPres->importe_gasto1);
    ui->SpinGastoDist2->setValue(oPres->importe_gasto2);
    ui->SpinGastoDist3->setValue(oPres->importe_gasto3);
    ui->spinPorc_iva_gasto1->setValue(oPres->porc_iva_gasto1);
    ui->spinPorc_iva_gasto2->setValue(oPres->porc_iva_gasto2);
    ui->spinPorc_iva_gasto3->setValue(oPres->porc_iva_gasto3);
    ui->spinIvaGasto1->setValue(oPres->iva_gasto1);
    ui->spinIva_gasto2->setValue(oPres->iva_gasto2);
    ui->spinIva_gasto3->setValue(oPres->iva_gasto3);


    index =  ui->cboFormaPago->findText(oPres->descripcionFormaPago);
    ui->cboFormaPago->setCurrentIndex(index);

    ui->txtlugar_entrega->setPlainText(oPres->lugar_entrega);
    ui->txtatencion_de->setText(oPres->atencion_de);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base1,'f',Configuracion_global->decimales)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base2,'f',Configuracion_global->decimales)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base3,'f',Configuracion_global->decimales)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base4,'f',Configuracion_global->decimales)));
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base,'f',Configuracion_global->decimales_campos_totales)));

    ui->txtporc_iva1->setText(QString::number(oPres->porc_iva1));
    ui->txtporc_iva2->setText(QString::number(oPres->porc_iva2));
    ui->txtporc_iva3->setText(QString::number(oPres->porc_iva3));
    ui->txtporc_iva4->setText(QString::number(oPres->porc_iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva1,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva2,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva3,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->iva4,'f',Configuracion_global->decimales)));
    ui->txttotal_iva->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtporc_rec1->setText(QString::number(oPres->porc_rec1));
    ui->txtporc_rec2->setText(QString::number(oPres->porc_rec2));
    ui->txtporc_rec3->setText(QString::number(oPres->porc_rec3));
    ui->txtporc_rec4->setText(QString::number(oPres->porc_rec4));
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_recargo,'f',Configuracion_global->decimales)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total_recargo,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec1,'f',Configuracion_global->decimales)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec2,'f',Configuracion_global->decimales)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec3,'f',Configuracion_global->decimales)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->rec4,'f',Configuracion_global->decimales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total1,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total2,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total3,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total4,'f',Configuracion_global->decimales)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total,'f',Configuracion_global->decimales_campos_totales)));
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
    helper.setId_cliente(oClientePres->id);
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
    int index = ui->cbo_Pais->findText(Configuracion_global->Devolver_pais(oClientePres->id_pais));
    ui->cbo_Pais->setCurrentIndex(index);
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
    ui->spin_porc_dto->setValue(oClientePres->porc_dto_cliente);
    oPres->id_cliente = oClientePres->id;
    if(oClientePres->recargo_equivalencia)
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

    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();

    //---------------------------------
    // Comprobar direccion alternativa
    //---------------------------------
    //-------------------
    // Preparando campos
    //-------------------
    ui->txtdireccion1_entrega->setText("");
    ui->txtdireccion2_entrega->setText("");
    ui->txtCP_entrega->setText("");
    ui->txtPoblacion_entrega->setText("");
    ui->txtProvincia_entrega->setText("");
    ui->cboPais_entrega->setCurrentIndex(-1);
    ui->txtEmail_entrega->setText("");
    ui->txtComentarios_entrega->setPlainText("");

    QMap <int,QSqlRecord> rec;
    QStringList condiciones;
    QString error;
    condiciones << QString("id_cliente = %1").arg(oClientePres->id) << "direccion_envio = 1";
    rec = SqlCalls::SelectRecord("cliente_direcciones",condiciones,Configuracion_global->groupDB,error);

    QMapIterator <int,QSqlRecord> i(rec);
    while(i.hasNext())
    {
        i.next();
        ui->txtdireccion1_entrega->setText(i.value().value("direccion1").toString());
        ui->txtdireccion2_entrega->setText(i.value().value("direccion2").toString());
        ui->txtCP_entrega->setText(i.value().value("cp").toString());
        ui->txtPoblacion_entrega->setText(i.value().value("poblacion").toString());
        ui->txtProvincia_entrega->setText(i.value().value("provincia").toString());
        int index = ui->cboPais_entrega->findText(Configuracion_global->Devolver_pais(i.value().value("pais").toInt()));
        ui->cboPais_entrega->setCurrentIndex(index);
        ui->txtEmail_entrega->setText(i.value().value("email").toString());
        ui->txtComentarios_entrega->setPlainText(i.value().value("comentarios").toString());

    }
    helper.setId_cliente(oClientePres->id);
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

    oPres->id_pais = Configuracion_global->Devolver_id_pais(ui->cbo_Pais->currentText());

    oPres->direccion1_entrega = (ui->txtdireccion1->text());
    oPres->direccion2_entrega = (ui->txtdireccion2->text());
    oPres->cp_entrega = (ui->txtcp->text());
    oPres->poblacion_entrega = (ui->txtpoblacion->text());
    oPres->provincia_entrega = (ui->txtprovincia->text());

    oPres->id_pais_entrega = Configuracion_global->Devolver_id_pais(ui->cboPais_entrega->currentText());
    oPres->email_entrega = ui->txtEmail_entrega->text();
    oPres->comentarios_entrega = ui->txtComentarios_entrega->toPlainText();
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


    oPres->descripcionFormaPago = ui->cboFormaPago->currentText();
    oPres->id_forma_pago = Configuracion_global->Devolver_id_forma_pago(ui->cboFormaPago->currentText());
    oPres->codigoFormaPago = Configuracion_global->Devolver_codigo_forma_pago(oPres->id_forma_pago);

    oPres->lugar_entrega = (ui->txtlugar_entrega->toPlainText());
    oPres->atencion_de = (ui->txtatencion_de->text());
    oPres->porc_dto = ui->spin_porc_dto->value();
    oPres->porc_dto_pp = ui->spin_porc_dto_pp->value();

    oPres->base1 = (ui->txtbase1->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->base2 = (ui->txtbase2->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->base3 = (ui->txtbase3->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->base4 = (ui->txtbase4->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oPres->porc_iva1 = (Configuracion_global->MonedatoDouble(ui->txtporc_iva1->text()));
    oPres->porc_iva2 = (Configuracion_global->MonedatoDouble(ui->txtporc_iva2->text()));
    oPres->porc_iva3 = (Configuracion_global->MonedatoDouble(ui->txtporc_iva3->text()));
    oPres->porc_iva4 = (Configuracion_global->MonedatoDouble(ui->txtporc_iva4->text()));
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
    oPres->desc_gasto1 = ui->txtGastoDist1->text();
    oPres->desc_gasto2 = ui->txtGastoDist2->text();
    oPres->desc_gasto3 = ui->txtGastoDist3->text();
    oPres->importe_gasto1 = ui->SpinGastoDist1->value();
    oPres->importe_gasto2 = ui->SpinGastoDist2->value();
    oPres->importe_gasto3 = ui->SpinGastoDist3->value();
    oPres->porc_iva_gasto1 = ui->spinPorc_iva_gasto1->value();
    oPres->porc_iva_gasto2 = ui->spinPorc_iva_gasto2->value();
    oPres->porc_iva_gasto3 = ui->spinPorc_iva_gasto3->value();
    oPres->iva_gasto1 = ui->spinIvaGasto1->value();
    oPres->iva_gasto2 = ui->spinIva_gasto2->value();
    oPres->iva_gasto3 = ui->spinIva_gasto3->value();

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
    ui->cbo_Pais->setCurrentIndex(-1);
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
    ui->txtporc_iva1->setText("1");
    ui->txtporc_iva2->setText("1");
    ui->txtporc_iva3->setText("1");
    ui->txtporc_iva4->setText("1 casa");
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
    // QDblSpinbox
    QList<QDoubleSpinBox *> DSpinEditList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox  *DblSpin;
    foreach (DblSpin, DSpinEditList) {
        DblSpin->setEnabled(!state);
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
    oPres->AnadirPresupuesto();
    LLenarCampos();
    ui->txtcodigo_cliente->setFocus();
    editando = false;
    emit block();
}

void FrmPresupuestosCli::on_btnEditar_clicked()
{
    if (oPres->editable)
    {
        BloquearCampos(false);
        editando = true;
        emit block();
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Editar Presupuesto"),tr("No se puede editar un Presupuesto que ha sido facturado, solo los NO facturados se pueden editar")+
                             tr("<p><b> Si necesita modificar algo genere una factura nueva y realice el abono correspondiente</b>")+
                                tr("y luego si es preciso realice un nuevo presupuesto y facture de nuevo, o pida al administrador que le desbloquee el presupuesto. "),tr("OK"));
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
    ui->stackedWidget->setCurrentIndex(1);
    mostrarBusqueda();
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
    double dto_pp;
    dto_pp = subtotal *(ui->spin_porc_dto_pp->value()/100.0);
    base = subtotal -(dto_pp +dto);
    base += (ui->SpinGastoDist1->value() + ui->SpinGastoDist2->value() + ui->SpinGastoDist3->value());
    total = base +iva +re;

    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales_campos_totales)));

    ui->txttotal_base->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_iva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales_campos_totales)));
}

void FrmPresupuestosCli::desglose1Changed(double base, double iva, double re, double total)
{

    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
   // ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPresupuestosCli::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPresupuestosCli::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPresupuestosCli::desglose4Changed(double base, double iva, double re, double total)
{
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
    if(ui->txtpedido->text().isEmpty() || ui->txtpedido->text() =="0")
    {
        Pedidos oPedido;
        QString c = QString("id = %1").arg(oPres->id);
        QString error;
        QHash <QString, QVariant> h;
        h["albaran"] = oPres->albaran;
        h["pedido"] = oPedido.NuevoNumeroPedido();
        h["fecha"] =QDate::currentDate();
        h["id_cliente"] = oPres->id_cliente;
        h["codigo_cliente"] = oPres->codigo_cliente;
        h["cliente"] = oPres->cliente;
        h["direccion1"]= oPres->direccion1;
        h["direccion2"] = oPres->direccion2;
        h["poblacion"] = oPres->poblacion;
        h["provincia"] = oPres->provincia;
        h["cp"] = oPres->cp;
        h["id_pais"] = oPres->id_pais;
        h["cif"] = oPres->cif;
        h["telefono"] = oPres->telefono;
        h["fax"] = oPres->fax;
        h["movil"] = oPres->movil;
        h["email"] = oPres->email;
        h["recargo_equivalencia"] = oPres->recargo_equivalencia;
        h["subtotal"] = oPres->subtotal;
        h["porc_dto"] = oPres->porc_dto;
        h["porc_dto_pp"] = oPres->porc_dto_pp;
        h["dto_pp"] = oPres->dto_pp;
        h["dto"] = oPres->dto;
        h["base1"] = oPres->base1;
        h["base2"] = oPres->base2;
        h["base3"] = oPres->base3;
        h["base4"] = oPres->base4;
        h["porc_iva1"] = oPres->porc_iva1;
        h["porc_iva2"] = oPres->porc_iva2;
        h["porc_iva3"] = oPres->porc_iva3;
        h["porc_iva4"] = oPres->porc_iva4;
        h["iva1"] = oPres->iva1;
        h["iva2"] = oPres->iva2;
        h["iva3"] = oPres->iva3;
        h["iva4"] = oPres->iva4;
        h["porc_rec1"] = oPres->porc_rec1;
        h["porc_rec2"] = oPres->porc_rec2;
        h["porc_rec3"] = oPres->porc_rec3;
        h["porc_rec4"] = oPres->porc_rec4;
        h["rec1"] = oPres->rec1;
        h["rec2"] = oPres->rec2;
        h["rec3"] = oPres->rec3;
        h["rec4"] = oPres->rec4;
        h["total1"] = oPres->total1;
        h["total2"] = oPres->total2;
        h["total3"] = oPres->total3;
        h["total4"] = oPres->total4;
        h["base_total"] = oPres->base;
        h["iva_total"] = oPres->total_iva;
        h["rec_total"] = oPres->total_recargo;
        h["total_albaran"] = oPres->total;
        h["impreso"] = false;
        h["facturado"] = false;
        h["comentario"] = oPres->comentarios;
        h["direccion_entrega1"] = oPres->direccion1_entrega;
        h["direccion_entrega2"] = oPres->direccion2_entrega;
        h["cp_entrega"] = oPres->cp_entrega;
        h["poblacion_entrega"] = oPres->poblacion_entrega;
        h["provincia_entrega"] = oPres->provincia_entrega;
        h["id_pais_entrega"] = oPres->id_pais_entrega;
        h["email_entrega"] = oPres->email_entrega;
        h["comentarios_entrega"] = oPres->comentarios_entrega;
        h["id_forma_pago"] = oPres->id_forma_pago;
        h["total_pedido"] = oPres->total;
        h["desc_gasto1"] = oPres->desc_gasto1;
        h["desc_gasto2"] = oPres->desc_gasto2;
        h["desc_gasto3"] = oPres->desc_gasto3;
        h["imp_gasto1"] = oPres->importe_gasto1;
        h["imp_gasto2"] = oPres->importe_gasto2;
        h["imp_gasto3"] = oPres->importe_gasto3;
        h["porc_iva_gasto1"] = oPres->porc_iva_gasto1;
        h["porc_iva_gasto2"] = oPres->porc_iva_gasto2;
        h["porc_iva_gasto3"] = oPres->porc_iva_gasto3;
        h["iva_gasto1"] = oPres->iva_gasto1;
        h["iva_gasto2"] = oPres->iva_gasto2;
        h["iva_gasto3"] = oPres->iva_gasto3;
        h["ejercicio"] = oPres->ejercicio;
        h["editable"] = true;
        h["id_transportista"] = oPres->id_transportista;

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
        if(added_l == -1 || added == -1)
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
            p["pedido"] = h["pedido"];
            c = "id="+QString::number(oPres->id);
            bool updated = SqlCalls::SqlUpdate(p,"cab_pre",Configuracion_global->empresaDB,c,error);

            if(updated)
            {
                Configuracion_global->empresaDB.commit();
                QString num = h["num"].toString();
                t = new TimedMessageBox(qApp->activeWindow(),
                                        QObject::tr("Se ha creado el pedido num:")+num);
                oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(oPres->id));
                LLenarCampos();
            } else
            {
                QMessageBox::warning(this,tr("Traspaso a pedidos"),tr("No se pudo crear el pedido"),tr("Aceptar"));
                Configuracion_global->empresaDB.rollback();
            }
        }
    } else
        TimedMessageBox *t = new TimedMessageBox(this,
                           tr("Este presupuesto ya ha sido pasado al pedido %1").arg(QString::number(oPres->pedido)));
//    qDebug() << "id:" << h.value("id_cliente");

//    h.remove("columna que no esta o dxa error");
//    //o si falta
//    h.insert("la que falta",valor)
}

void FrmPresupuestosCli::convertir_ealbaran()
{
    if(oPres->albaran ==0)
    {
        QString c = QString("id = %1").arg(oPres->id);
        QString error;
        QHash <QString, QVariant> h;
        h["pedido_cliente"] = oPres->pedido;
        h["id_cliente"] = oPres->id_cliente;
        h["codigo_cliente"] = oPres->codigo_cliente;
        h["cliente"] = oPres->cliente;
        h["direccion1"] = oPres->direccion1;
        h["direccion2"] = oPres->direccion2;
        h["poblacion"] = oPres->poblacion;
        h["provincia"] = oPres->provincia;
        h["cp"] = oPres->cp;
        h["id_pais"] = oPres->id_pais;
        h["direccion1_entrega"] = oPres->direccion1_entrega;
        h["direccion2_entrega"] = oPres->direccion2_entrega;
        h["poblacion_entrega"] = oPres->poblacion_entrega;
        h["provincia_entrega"] = oPres->provincia_entrega;
        h["cp_entrega"] = oPres->cp_entrega;
        h["id_pais_entrega"] = oPres->id_pais_entrega;
        h["email_entrega"] = oPres->email_entrega;
        h["comentarios_entrega"] = oPres->comentarios_entrega;
        h["cif"] = oPres->cif;
        h["telefono"] = oPres->telefono;
        h["fax"] = oPres->movil;
        h["email"] = oPres->email;
        h["recargo_equivalencia"] = oPres->recargo_equivalencia;
        h["subtotal"] = oPres->subtotal;
        h["dto"] = oPres->dto;
        h["porc_dto"] = oPres->porc_dto;
        h["porc_dto_pp"] = oPres->porc_dto_pp;
        h["dto_pp"] = oPres->dto_pp;
        h["base1"] = oPres->base1;
        h["base2"] = oPres->base2;
        h["base3"] = oPres->base3;
        h["base4"] = oPres->base4;
        h["porc_iva1"] = oPres->porc_iva1;
        h["porc_iva2"] = oPres->porc_iva2;
        h["porc_iva3"] = oPres->porc_iva3;
        h["porc_iva4"] = oPres->porc_iva4;
        h["iva1"] = oPres->iva1;
        h["iva2"] = oPres->iva2;
        h["iva3"] = oPres->iva3;
        h["iva4"] = oPres->iva4;
        h["porc_rec1"] = oPres->porc_rec1;
        h["porc_rec2"] = oPres->porc_rec2;
        h["porc_rec3"] = oPres->porc_rec3;
        h["porc_rec4"] = oPres->porc_rec4;
        h["rec1"] = oPres->rec1;
        h["rec2"] = oPres->rec2;
        h["rec3"] = oPres->rec3;
        h["rec4"] = oPres->rec4;
        h["total1"] = oPres->total1;
        h["total2"] = oPres->total2;
        h["total3"] = oPres->total3;
        h["total4"] = oPres->total4;
        h["base_total"] = oPres->base;
        h["iva_total"] = oPres->total_iva;
        h["rec_total"] = oPres->total_recargo;
        h["total_albaran"] = oPres->total;
        h["impreso"] = oPres->impreso;
        h["comentario"] = oPres->comentarios;
        h["id_forma_pago"] = oPres->id_forma_pago;
        h["desc_gasto1"] = oPres->desc_gasto1;
        h["desc_gasto2"] = oPres->desc_gasto2;
        h["desc_gasto3"] = oPres->desc_gasto3;
        h["imp_gasto1"] = oPres->importe_gasto1;
        h["imp_gasto2"] = oPres->importe_gasto2;
        h["imp_gasto3"] = oPres->importe_gasto3;
        h["porc_iva_gasto1"] = oPres->porc_iva_gasto1;
        h["porc_iva_gasto2"] = oPres->porc_iva_gasto2;
        h["porc_iva_gasto3"] = oPres->porc_iva_gasto3;
        h["iva_gasto1"] = oPres->iva_gasto1;
        h["iva_gasto2"] = oPres->iva_gasto2;
        h["iva_gasto3"] = oPres->iva_gasto3;
        h["id_transportista"] = oPres->id_transportista;
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
            c = "id="+QString::number(oPres->id);
            bool updated = SqlCalls::SqlUpdate(p,"cab_pre",Configuracion_global->empresaDB,c,error);

            if(updated)
            {

                t = new TimedMessageBox(qApp->activeWindow(),
                                                          QObject::tr("Se ha creado el albarán num:")+QString::number(num));
                oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(oPres->id));
                //-------------------------------------
                // Insertamos datos albaran en pedido
                //-------------------------------------
                oPres->albaran =oAlbaran.albaran;
                oPres->GuardarPres(oPres->id);
                ui->txtalbaran->setText(QString::number(num));
                bool commited =Configuracion_global->empresaDB.commit();
                if(!commited)
                    QMessageBox::warning(this,tr("Gestión de presupuestos"),
                                         tr("Error en la transacción"),tr("Aceptar"));

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
    if(oPres->editable && (ui->txtfactura->text() == "0" || ui->txtfactura->text().isEmpty()))
    {
        if(QMessageBox::question(this,tr("Presupuestos a clientes"),tr("¿Desea realmente facturar este presupuesto?"),
                                 tr("No"),tr("Sí"))==QMessageBox::Accepted)
        {
            LLenarPresupuesto();
            if(ui->txtpedido->text().isEmpty() || ui->txtpedido->text().trimmed() =="0")
                convertir_epedido();
            if(ui->txtalbaran->text().isEmpty() || ui->txtalbaran->text().trimmed() =="0")
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
            // LLENAR CABECERA
            //-----------------------
            QHash <QString, QVariant > cab_fac;

            cab_fac["codigo_cliente"] = oPres->codigo_cliente;
            cab_fac["serie"] = serie;
            cab_fac["factura"] = oFactura.NuevoNumeroFactura(serie);
            cab_fac["fecha"] = QDate::currentDate();
            cab_fac["id_cliente"] = oPres->id_cliente;
            cab_fac["cliente"] = oPres->cliente;
            cab_fac["direccion1"] = oPres->direccion1;
            cab_fac["direccion2"] = oPres->direccion2;
            cab_fac["cp"] = oPres->cp;
            cab_fac["poblacion"] = oPres->poblacion;
            cab_fac["provincia"] = oPres->provincia;
            cab_fac["id_pais"] = oPres->id_pais;
            cab_fac["direccion1_entrega"] = oPres->direccion1_entrega;
            cab_fac["direccion2_entrega"] = oPres->direccion2_entrega;
            cab_fac["poblacion_entrega"] = oPres->poblacion_entrega;
            cab_fac["provincia_entrega"] = oPres->provincia_entrega;
            cab_fac["cp_entrega"] = oPres->cp_entrega;
            cab_fac["id_pais_entrega"] = oPres->id_pais_entrega;
            cab_fac["email_entrega"] = oPres->email_entrega;
            cab_fac["comentarios_entrega"] = oPres->comentarios_entrega;
            cab_fac["cif"] = oPres->cif;
            cab_fac["telefono"] = oPres->telefono;
            cab_fac["fax"] = oPres->fax;
            cab_fac["movil"] = oPres->movil;
            cab_fac["email"] = oPres->email;
            cab_fac["recargo_equivalencia"] = oPres->recargo_equivalencia;
            cab_fac["subtotal"] = oPres->subtotal;
            cab_fac["porc_dto"] = oPres->porc_dto;
            cab_fac["porc_dto_pp"] = oPres->porc_dto_pp;
            cab_fac["dto"] = oPres->dto;
            cab_fac["dto_pp"] = oPres->dto_pp;
            cab_fac["base"] = oPres->base;
            //cab_fac["porc_irpf"] = oPres->porc_irpf;
            //cab_fac["irpf"] = oPres->irpf;
            cab_fac["iva"] = oPres->total_iva;
            cab_fac["total"] = oPres->total;
            cab_fac["impreso"]= oPres->impreso;
            cab_fac["cobrado"] = false;
            cab_fac["contabilizado"] = false;
            cab_fac["id_forma_pago"] = oPres->id_forma_pago;
            cab_fac["forma_pago"] = Configuracion_global->Devolver_forma_pago(oPres->id_forma_pago);
            cab_fac["comentario"] = oPres->comentarios;
            cab_fac["base1"] = oPres->base1;
            cab_fac["base2"] = oPres->base2;
            cab_fac["base3"] = oPres->base3;
            cab_fac["base4"] = oPres->base4;
            cab_fac["porc_iva1"] = oPres->porc_iva1;
            cab_fac["porc_iva2"] = oPres->porc_iva2;
            cab_fac["porc_iva3"] = oPres->porc_iva3;
            cab_fac["porc_iva4"] = oPres->porc_iva4;
            cab_fac["iva1"] = oPres->iva1;
            cab_fac["iva2"] = oPres->iva2;
            cab_fac["iva3"] = oPres->iva3;
            cab_fac["iva4"] = oPres->iva4;
            cab_fac["total1"] = oPres->total1;
            cab_fac["total2"] = oPres->total2;
            cab_fac["total3"] = oPres->total3;
            cab_fac["total4"] = oPres->total4;
            cab_fac["porc_rec1"] = oPres->porc_rec1;
            cab_fac["porc_rec2"] = oPres->porc_rec2;
            cab_fac["porc_rec3"] = oPres->porc_rec3;
            cab_fac["porc_rec4"] = oPres->porc_rec4;
            cab_fac["rec1"] = oPres->rec1;
            cab_fac["rec2"] = oPres->rec2;
            cab_fac["rec3"] = oPres->rec3;
            cab_fac["rec4"] = oPres->rec4;
            cab_fac["total_recargo"] = oPres->total_recargo;
            cab_fac["importe_pendiente"] = oPres->importe_pendiente;
            cab_fac["codigo_entidad"] = oClientePres->entidad_bancaria;
            cab_fac["oficina_entidad"] = oClientePres->oficina_bancaria;
            cab_fac["dc_cuenta"] = oClientePres->dc;
            cab_fac["cuenta_corriente"] = oClientePres->cuenta_corriente;
            cab_fac["desc_gasto1"] = oPres->desc_gasto1;
            cab_fac["desc_gasto2"] = oPres->desc_gasto2;
            cab_fac["desc_gasto3"] = oPres->desc_gasto3;
            cab_fac["imp_gasto1"] = oPres->importe_gasto1;
            cab_fac["imp_gasto2"] = oPres->importe_gasto2;
            cab_fac["imp_gasto3"] = oPres->importe_gasto3;
            cab_fac["porc_iva_gasto1"] = oPres->porc_iva_gasto1;
            cab_fac["iva_gasto1"] = oPres->iva_gasto1;
            cab_fac["iva_gasto2"] = oPres->iva_gasto2;
            cab_fac["iva_gasto3"] = oPres->iva_gasto3;
            cab_fac["id_transportista"] = oPres->id_transportista;
            cab_fac["ejercicio"] = Configuracion_global->cEjercicio.toInt();
            cab_fac["editable"] = true;
            cab_fac["pendiente"] = oPres->total;
            QString error;
            int new_id = SqlCalls::SqlInsert(cab_fac,"cab_fac",Configuracion_global->empresaDB,error);

            if(new_id > -1)
            {
                //------------------
                // CARGAR FACTURA
                //------------------
                oFactura.RecuperarFactura(new_id);
                //-------------------
                //  INSERTAR LÍNEAS
                //-------------------
                QSqlQuery lineas_pre(Configuracion_global->empresaDB);

                if(lineas_pre.exec("Select * from lin_pre where id_cab ="+QString::number(oPres->id)))
                {
                    QHash <QString, QVariant> h_lineas_fac;
                    while (lineas_pre.next()) {

                        h_lineas_fac["id_cab"] = new_id;
                        h_lineas_fac["id_articulo"] = lineas_pre.record().value("id_articulo").toInt();
                        h_lineas_fac["codigo"] = lineas_pre.record().value("codigo").toString();
                        h_lineas_fac["cantidad"] =  lineas_pre.record().value("cantidad").toInt();
                        h_lineas_fac["descripcion"] = lineas_pre.record().value("descripcion").toString();
                        h_lineas_fac["precio"] = lineas_pre.record().value("precio").toDouble();
                        h_lineas_fac["subtotal"] = lineas_pre.record().value("subtotal").toDouble();
                        h_lineas_fac["porc_dto"] = lineas_pre.record().value("porc_dto").toDouble();
                        h_lineas_fac["dto"] = lineas_pre.record().value("dto").toDouble();
                        h_lineas_fac["porc_iva"] = lineas_pre.record().value("porc_iva").toDouble();
                        h_lineas_fac["total"] = lineas_pre.record().value("total").toDouble();
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
                    texto = tr("Se ha creado una nueva factura.\ncon el número ")+ cab_fac.value("factura").toString()+
                               tr("\n y de importe: ")+QString::number(cab_fac.value("total").toDouble(),'f',Configuracion_global->decimales);
                    TimedMessageBox * t = new TimedMessageBox(this,texto);
                    //-----------------------------------------
                    // Insertamos datos factura en presupuesto
                    //-----------------------------------------
                    oPres->factura =oFactura.factura;
                    oPres->editable = false;
                    oPres->GuardarPres(oPres->id);
                    ui->txtfactura->setText(oPres->factura);
                    Configuracion_global->empresaDB.commit();
                    //------------------------------------
                    // Actualizamos factura en albarán
                    //------------------------------------
                    QString error;
                    int albaran = SqlCalls::SelectOneField("cab_pre","albaran",QString("albaran=%1").arg(oPres->albaran),
                                                            Configuracion_global->empresaDB,error).toInt();
                    if(albaran >0)
                    {
                        QHash <QString, QVariant> a;
                        a["factura"] = oFactura.factura;
                        bool updated = SqlCalls::SqlUpdate(a,"cab_alb",Configuracion_global->empresaDB,
                                                           QString("albaran =%1").arg(oPres->albaran),
                                                           error);
                        if(!updated)
                            TimedMessageBox *t = new TimedMessageBox(this,tr("No se pudo guardar el numero de factura en el albarán: %1").arg(error));
                    }



                    vencimientos vto(this);
                    //-----------------
                    // Asiento contable
                    //-----------------
                    if(Configuracion_global->contabilidad)
                    {

                        bool creado;
                        oClientePres->Recuperar("select * from clientes where id = "+QString::number(oFactura.id_cliente));
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
                    oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(oPres->id));
                    LLenarCampos();

                } else
                {

                    QMessageBox::warning(this,tr("Traspaso a factura"),tr("No se pudo crear la factura "),tr("Aceptar"));

                    Configuracion_global->empresaDB.rollback();
                }





                } else
                {
                    Configuracion_global->empresaDB.rollback();
                    QMessageBox::warning(this,tr("Presupuestos"),
                                         tr("No se ha podido crear la factura"),tr("Aceptar"));
                }
        }
    } else
        QMessageBox::warning(this,tr("Presupuestos"),tr("Este presupuesto ya ha sido facturado"),tr("Aceptar"));
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

        QHash <QString,QVariant> lin_pre;
        QString error;
        lin_pre["id_cab"] = oPres->id;
        lin_pre["id_articulo"] = queryArticulos.record().value("id").toInt();
        lin_pre["codigo"] = ld->codigo;
        lin_pre["cantidad"] = ld->cantidad;
        lin_pre["descripcion"] = ld->descripcion;
        lin_pre["precio"] = ld->precio;
        lin_pre["subtotal"] = ld->subtotal;
        lin_pre["porc_dto"] = ld->dto_perc;
        lin_pre["dto"] = ld->dto;
        lin_pre["porc_iva"]= ld->iva_perc;
        lin_pre["iva"] = ld->iva;
        lin_pre["porc_rec"] = ld->rec_perc;
        lin_pre["rec"] = ld->rec;
        lin_pre["total"] = ld->total;
        int new_id = SqlCalls::SqlInsert(lin_pre,"lin_pre",Configuracion_global->empresaDB,error);

//        QSqlQuery lin_pre(Configuracion_global->empresaDB);
//        lin_pre.prepare("INSERT INTO lin_pre (id_cab, id_articulo, codigo, cantidad,"
//                                  "descripcion, precio, subtotal, porc_dto,"
//                                  " dto, porc_iva, iva, total)"
//                                  "VALUES (:id_cab,:id_articulo,:codigo,:cantidad,"
//                                  ":descripcion,:precio,:subtotal,:porc_dto,"
//                                  ":dto,:porc_iva,:iva,:total);");
//        id, id_cab, id_articulo, codigo, cantidad, descripcion, precio, subtotal, porc_dto, dto, porc_iva, iva, porc_rec, rec, total
//        lin_pre.bindValue(":id_cab", oPres->id);
//        lin_pre.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
//        lin_pre.bindValue(":codigo",ld->codigo);
//        lin_pre.bindValue(":descripcion",ld->descripcion);
//        lin_pre.bindValue(":cantidad",ld->cantidad);
//        lin_pre.bindValue(":precio",ld->precio);
//        lin_pre.bindValue(":subtotal",ld->subtotal);
//        lin_pre.bindValue(":porc_dto",ld->dto_perc);
//        lin_pre.bindValue(":dto",ld->dto);
//        ld->po
//        lin_pre.bindValue(":iva",(ld->subtotal * ld->iva_perc)/100);
//        lin_pre.bindValue(":total",ld->total);
        if (new_id == -1){
            ok_empresa = false;
            QMessageBox::warning(this,tr("Gestión de albaranes"),
                                 tr("Ocurrió un error al insertar la nueva línea: %1").arg(error),
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

        ld->idLinea = new_id;

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

void FrmPresupuestosCli::filter_table(QString texto, QString orden, QString modo)
{

    QHash <QString,QString> h;
    h[tr("presupuesto")] = "presupuesto";
    h[tr("cliente")] = "cliente";
    h[tr("teléfono")] = "telefono";

    QString order = h.value(orden);

    if(modo == tr("A-Z"))
        modo = "";
    else
        modo = "Desc";

    m->setQuery("select id,presupuesto,fecha,telefono,movil,cliente from cab_pre where "+order+
                " like '%"+texto+"%' order by "+order+" "+modo, Configuracion_global->empresaDB);
}

void FrmPresupuestosCli::buscar_poblacion(int tipo)
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
                index = ui->cbo_Pais->findText(Configuracion_global->Devolver_pais(m.value(id).value("id_pais").toInt()));
                ui->cbo_Pais->setCurrentIndex(index);

            } else
            {
                ui->txtCP_entrega->setText(m.value(id).value("cp").toString());
                ui->txtPoblacion_entrega->setText(m.value(id).value("poblacion").toString());
                ui->txtProvincia_entrega->setText(m.value(id).value("provincia").toString());
                index = ui->cboPais_entrega->findText(Configuracion_global->Devolver_pais(m.value(id).value("id_pais").toInt()));
                ui->cboPais_entrega->setCurrentIndex(index);

            }
        else
            QMessageBox::warning(this,tr("Gestión presupuestos"),tr("Falló la recuperación de la población : %1").arg(error),
                                 tr("Aceptar"));
    }


}

void FrmPresupuestosCli::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);
    this->installEventFilter(this);

    QStringList orden;
    orden  << tr("presupuesto") <<tr("cliente") << tr("teléfono");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(this,&MayaModule::hideBusqueda,this,&FrmPresupuestosCli::ocultarBusqueda);
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));


    QPushButton* add = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir forma de pago"),this);
    connect(add,SIGNAL(clicked()),this,SLOT(on_btnAnadir_3_clicked()));
    m_busqueda->addWidget(add);

    QPushButton* edit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar forma de pago"),this);
    connect(edit,SIGNAL(clicked()),this,SLOT(on_btnEditar_2_clicked()));
    m_busqueda->addWidget(edit);

    QPushButton* print = new QPushButton(QIcon(":/Icons/PNG/print2.png"),tr("Imprimir forma de pago"),this);
   // connect(print,SIGNAL(clicked()),this,SLOT(on_btnEditar_2_clicked()));//TODO
    m_busqueda->addWidget(print);

    QPushButton* del = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar forma de pago"),this);
    connect(del,SIGNAL(clicked()),this,SLOT(on_btn_borrar_clicked()));
    m_busqueda->addWidget(del);
}

void FrmPresupuestosCli::on_tabla_clicked(const QModelIndex &index)
{
    int id = ui->tabla->model()->data(ui->tabla->model()->index(index.row(),0),Qt::EditRole).toInt();
    oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(id));
    //LLenarCampos();
}

void FrmPresupuestosCli::on_tabla_doubleClicked(const QModelIndex &index)
{
    int id = ui->tabla->model()->data(ui->tabla->model()->index(index.row(),0),Qt::EditRole).toInt();
    oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(id));
    LLenarCampos();
    ui->stackedWidget->setCurrentIndex(0);
    BloquearCampos(true);
}

void FrmPresupuestosCli::on_btn_convertir_clicked()
{

}

void FrmPresupuestosCli::on_btnDesbloquear_clicked()
{
    QModelIndex index = ui->tabla->currentIndex();
    int id = ui->tabla->model()->data(ui->tabla->model()->index(index.row(),0),Qt::EditRole).toInt();
    QHash <QString, QVariant> h;
    h["editable"] = true;
    QString error;
    bool updated = SqlCalls::SqlUpdate(h,"cab_pre",Configuracion_global->empresaDB,QString("id=%1").arg(id),error);
    if(!updated)
        QMessageBox::warning(this,tr("Gestión de presupuestos"),tr("Ocurrió un error al desbloquear: %1").arg(error),
                             tr("Aceptar"));
    oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(id));
    LLenarCampos();
    ui->stackedWidget->setCurrentIndex(0);
    BloquearCampos(true);
}

void FrmPresupuestosCli::on_btnAnadir_2_clicked()
{
    on_btnAnadir_clicked();
    ui->stackedWidget->setCurrentIndex(0);
}

void FrmPresupuestosCli::on_btnEditar_2_clicked()
{
    on_tabla_doubleClicked(ui->tabla->currentIndex());
    on_btnEditar_clicked();
}

void FrmPresupuestosCli::on_btnImprimir_2_clicked()
{
    // TODO -  Imprimir
}

void FrmPresupuestosCli::on_spin_porc_dto_editingFinished()
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

    clauses << QString("id_cab = %1").arg(oPres->id);

    rec = SqlCalls::SelectRecord("lin_pre",clauses,Configuracion_global->empresaDB,error);
    QMapIterator <int, QSqlRecord> i(rec);
    while (i.hasNext())
    {
        i.next();
        id_lin = i.value().value("id").toInt();
        id_art = i.value().value("id_articulo").toInt();
        subtotal = i.value().value("subtotal").toDouble();
        porc_dto_lin = i.value().value("porc_dto").toFloat();

        porc_dto_esp = oArt.asigna_dto_linea(id_art,oClientePres->id,ui->spin_porc_dto->value(),porc_dto_lin);
        QHash <QString,QVariant> f;
        clauses.clear();
        clauses << QString("id = %1").arg(id_lin);
        f["porc_dto"] = porc_dto_esp;
        f["dto"] = subtotal *(porc_dto_esp/100.0);

        bool upd = SqlCalls::SqlUpdate(f,"lin_pre",Configuracion_global->empresaDB,clauses,error);
        if(!upd)
            QMessageBox::warning(this,tr("Cambio DTO"),tr("No se pudo realizar el cambio en las líneas, error:%1").arg(error),
                                 tr("Aceptar"));
        else
            helper.calculatotal();
    }
    //--------------------------
    // Calculo dto total
    //--------------------------
    double dto = (Configuracion_global->MonedatoDouble(ui->txtsubtotal->text())*(ui->spin_porc_dto->value()/100.0));
    ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales_campos_totales)));
    oPres->dto = dto;
    QString filter = QString("id_cab = '%1'").arg(oPres->id);
    helper.fillTable("empresa","lin_pre",filter);
}

void FrmPresupuestosCli::on_spin_porc_dto_pp_editingFinished()
{
    float dto_pp;
    dto_pp = (Configuracion_global->MonedatoDouble(ui->txtsubtotal->text())*(ui->spin_porc_dto_pp->value()/100.0));
    ui->txtDto_pp->setText(Configuracion_global->toFormatoMoneda(QString::number(dto_pp,'f',Configuracion_global->decimales_campos_totales)));
    oPres->dto_pp = dto_pp;
    QString filter = QString("id_cab = '%1'").arg(oPres->id);
    helper.fillTable("empresa","lin_pre",filter);
}

void FrmPresupuestosCli::mostrarBusqueda()
{
    _showBarraBusqueda(m_busqueda);
}

void FrmPresupuestosCli::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}

bool FrmPresupuestosCli::eventFilter(QObject *obj, QEvent *event)
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
        if(obj == ui->txtCP_entrega)
        {
            if(keyEvent->key() == Qt::Key_F1)
                buscar_poblacion(2);
        }
    }
    if(event->type() == QEvent::Resize)
        _resizeBarraBusqueda(m_busqueda);
    return MayaModule::eventFilter(obj,event);
}
