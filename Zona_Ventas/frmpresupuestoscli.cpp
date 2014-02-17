#include "frmpresupuestoscli.h"
#include "ui_frmpresupuestoscli.h"
#include <QSqlQueryModel>

#include "../Almacen/articulo.h"
#include "../Busquedas/db_consulta_view.h"
#include"../Auxiliares/datedelegate.h"
#include"../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/monetarydelegate_totals.h"
#include "../Auxiliares/numericdelegate.h"
#include <QMessageBox>
#include "pedidos.h"
#include "albaran.h"
#include "factura.h"
#include "../vencimientos.h"
#include "../Auxiliares/frmeditline.h"

#include "frmlistadospre.h"
void FrmPresupuestosCli::setConvertirMenu()
{
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
}

void FrmPresupuestosCli::init_querys()
{
    model_busqueda->setQuery(QString("select id,presupuesto,fecha,telefono,movil,cliente from cab_pre order by presupuesto desc"),Configuracion_global->empresaDB);
    modelLineas->setQuery("select id,codigo,descripcion,cantidad,precio_recom,porc_dto,precio,subtotal,porc_iva,total "
                          "from lin_pre where id = 0;",Configuracion_global->empresaDB);

    formato_tablas();
}

FrmPresupuestosCli::FrmPresupuestosCli(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmPresupuestosCli),
    menuButton(QIcon(":/Icons/PNG/presupuestos.png"),tr("Presupuestos"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/presupuestos.png"),"",this))

{
    ui->setupUi(this);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de presupuestos a clientes"));
    ui->stackedWidget->setCurrentIndex(1);

    ui->cbo_Pais->setModel(Configuracion_global->paises_model);
    ui->cboPais_entrega->setModel(Configuracion_global->paises_model);
    ui->cboFormaPago->setModel(Configuracion_global->formapago_model);
    ui->cboDivisa->setModel(Configuracion_global->divisas_model);
    ui->cboAgente->setModel(Configuracion_global->agentes_model);

    ui->cboDivisa      ->setModelColumn(1);
    ui->cbo_Pais       ->setModelColumn(1);
    ui->cboPais_entrega->setModelColumn(1);
    ui->cboFormaPago   ->setModelColumn(2);
    ui->cboAgente      ->setModelColumn(2);

    // Objetos necesarios
    oPres = new Presupuesto(this);
    oClientePres = new Cliente(this);
    model_busqueda = new QSqlQueryModel(this);
    modelLineas    = new QSqlQueryModel(this);
    setConvertirMenu();
    setUpBusqueda();

    ui->Lineas->setItemDelegateForColumn(3,new NumericDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(4,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(5,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(6,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(7,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(8,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(9,new MonetaryDelegate_totals(this,true));

    ui->Lineas->setModel(modelLineas);
    ui->tabla->setModel(model_busqueda);

    formato_tablas();
    ui->tabla->selectRow(0);

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));

    //BUTTONS
    ui->btnBorrar->setEnabled(false);
    ui->btnEditar->setEnabled(false);
    ui->btn_convertir->setEnabled(false);
    ui->btnAnterior->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);
    ui->btnBuscar->setEnabled(true);
    ui->btnImprimir->setEnabled(false);

    // %Iva combos gastos
    ui->cboporc_iva_gasto1->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto1->setModelColumn(4);
    ui->cboporc_iva_gasto1->setCurrentIndex(ui->cboporc_iva_gasto1->findText(Configuracion_global->ivaList.at(0)));

    ui->cboporc_iva_gasto2->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto2->setModelColumn(4);
    ui->cboporc_iva_gasto2->setCurrentIndex(ui->cboporc_iva_gasto2->findText(Configuracion_global->ivaList.at(0)));

    ui->cboporc_iva_gasto3->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto3->setModelColumn(4);
    ui->cboporc_iva_gasto3->setCurrentIndex(ui->cboporc_iva_gasto3->findText(Configuracion_global->ivaList.at(0)));


    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;

    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);

    BloquearCampos(true);
}

FrmPresupuestosCli::~FrmPresupuestosCli()
{
    delete ui;
}

void FrmPresupuestosCli::LLenarCampos()
{
    ui->txtimporte_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->irpf ,'f',Configuracion_global->decimales_campos_totales)));
    ui->spinPorc_irpf->setValue(oPres->porc_irpf);
    ui->txtpresupuesto->setText(oPres->presupuesto);
    ui->lblTopNpres->setText(oPres->presupuesto);
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

    ui->lblAprovado->setVisible(oPres->aprobado);
    ui->lbimpreso->setVisible(oPres->impreso);

    for(auto i = 0; i< Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oPres->id_pais)
        {
            ui->cbo_Pais->setCurrentIndex(i);
            break;
        }
    }

    ui->txtdireccion1_entrega->setText(oPres->direccion1_entrega);
    ui->txtdireccion2_entrega->setText(oPres->direccion2_entrega);
    ui->txtCP_entrega->setText(oPres->cp_entrega);
    ui->txtPoblacion_entrega->setText(oPres->poblacion_entrega);
    ui->txtProvincia_entrega->setText(oPres->provincia_entrega);

    for(auto i = 0; i< Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oPres->id_pais_entrega)
        {
            ui->cboPais_entrega->setCurrentIndex(i);
            break;
        }
    }

    ui->txtEmail_entrega->setText(oPres->email_entrega);
    ui->txtComentarios_entrega->setPlainText(oPres->comentarios_entrega);
    ui->txttelefono->setText(oPres->telefono);
    ui->txtmovil->setText(oPres->movil);
    ui->txtfax->setText(oPres->fax);
    ui->txtcomentario->setPlainText(oPres->comentarios);
    ui->spin_porc_dto_especial->setValue(oPres->porc_dto);
    ui->spinPorc_dto_pp->setValue(oPres->porc_dto_pp);
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtdto->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->dto,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->total,'f',Configuracion_global->decimales_campos_totales)));

    ui->chklAprovado->setChecked(oPres->aprobado);
    ui->txtfechaAprovacion->setDate(oPres->fecha_aprobacion);
    ui->txtimporte_factura->setText(Configuracion_global->toFormatoMoneda(QString::number(oPres->importe_factura,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtfactura->setText(oPres->factura);
    ui->txtalbaran->setText(oPres->albaran);
    ui->txtpedido->setText(oPres->pedido);
    ui->txtGastoDist1->setText(oPres->desc_gasto1);
    ui->txtGastoDist2->setText(oPres->desc_gasto2);
    ui->txtGastoDist3->setText(oPres->desc_gasto3);
    ui->SpinGastoDist1->setValue(oPres->importe_gasto1);
    ui->SpinGastoDist2->setValue(oPres->importe_gasto2);
    ui->SpinGastoDist3->setValue(oPres->importe_gasto3);

    ui->cboporc_iva_gasto1->setCurrentIndex(ui->cboporc_iva_gasto1->findText(QString::number(oPres->porc_iva_gasto1)));
    ui->cboporc_iva_gasto1->setCurrentIndex(ui->cboporc_iva_gasto2->findText(QString::number(oPres->porc_iva_gasto2)));
    ui->cboporc_iva_gasto1->setCurrentIndex(ui->cboporc_iva_gasto3->findText(QString::number(oPres->porc_iva_gasto3)));

    ui->spinIva_gasto1->setValue(oPres->iva_gasto1);
    ui->spinIva_gasto2->setValue(oPres->iva_gasto2);
    ui->spinIva_gasto3->setValue(oPres->iva_gasto3);

    ui->cboFormaPago->setCurrentIndex(ui->cboFormaPago->findText(oPres->descripcionFormaPago));

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
    ui->chkrecargo_equivalencia->setChecked(oPres->recargo_equivalencia);
    oClientePres->Recuperar("Select * from clientes where id ="+QString::number(oPres->id_cliente),false);

    for(auto i = 0; i< Configuracion_global->divisas_model->rowCount();i++)
    {
        if(Configuracion_global->divisas_model->record(i).value("id").toInt() == oPres->id_divisa)
        {
            ui->cboDivisa->setCurrentIndex(i);
            break;
        }
    }
    for (auto a=0; a< Configuracion_global->agentes_model->rowCount();a++)
    {
        if(oPres->id_agente == Configuracion_global->agentes_model->record(a).value("id").toInt())
        {
            ui->cboAgente->setCurrentIndex(a);
            break;
        }
    }
    refrescar_modelo();
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

    for(auto i = 0; i< Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oClientePres->id_pais)
        {
            ui->cbo_Pais->setCurrentIndex(i);
            break;
        }
    }

    ui->txtcif->setText(oClientePres->cif_nif);
    ui->txttelefono->setText(oClientePres->telefono1);
    ui->txtfax->setText(oClientePres->fax);
    ui->txtmovil->setText(oClientePres->movil);
    ui->txtemail->setText(oClientePres->email);


    if (oClientePres->lIRPF==1) {
        ui->chkrecargo_equivalencia->setChecked(true);
        oPres->recargo_equivalencia = 1;
    } else {
        ui->chkrecargo_equivalencia->setChecked(false);
        oPres->recargo_equivalencia = 0;
    }
    ui->spin_porc_dto_especial->setValue(oClientePres->porc_dto_cliente);
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
    if(i.hasNext())
    {
        i.next();
        ui->txtdireccion1_entrega->setText(i.value().value("direccion1").toString());
        ui->txtdireccion2_entrega->setText(i.value().value("direccion2").toString());
        ui->txtCP_entrega->setText(i.value().value("cp").toString());
        ui->txtPoblacion_entrega->setText(i.value().value("poblacion").toString());
        ui->txtProvincia_entrega->setText(i.value().value("provincia").toString());

        for(auto p = 0; p< Configuracion_global->paises_model->rowCount();p++)
        {
            if(Configuracion_global->paises_model->record(p).value("id").toInt() == i.value().value("pais").toInt())
            {
                ui->cbo_Pais->setCurrentIndex(p);
                break;
            }
        }
        ui->txtEmail_entrega->setText(i.value().value("email").toString());
        ui->txtComentarios_entrega->setPlainText(i.value().value("comentarios").toString());
    }

    for (auto d = 0; d < Configuracion_global->divisas_model->rowCount();d++)
    {
        if(Configuracion_global->divisas_model->record(d).value("id").toInt() == oClientePres->id_divisa)
        {
            ui->cboDivisa->setCurrentIndex(d);
            break;
        }
    }
    for (auto fp = 0; fp < Configuracion_global->formapago_model->rowCount(); fp++)
    {
        if(oClientePres->id_forma_pago == Configuracion_global->formapago_model->record(fp).value("id").toInt())
        {
            ui->cboFormaPago->setCurrentIndex(fp);
            break;
        }
    }
    for (auto a=0; a< Configuracion_global->agentes_model->rowCount();a++)
    {
        if(oClientePres->id_agente == Configuracion_global->agentes_model->record(a).value("id").toInt())
        {
            ui->cboAgente->setCurrentIndex(a);
            break;
        }
    }
}

void FrmPresupuestosCli::LLenarPresupuesto()
{
    oPres->ejercicio = ui->txtfecha->date().year();
    oPres->presupuesto = ui->txtpresupuesto->text();
    oPres->id_divisa = Configuracion_global->divisas_model->record(ui->cboDivisa->currentIndex()).value("id").toInt();

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

    oPres->id_pais = Configuracion_global->paises_model->record(ui->cbo_Pais->currentIndex()).value("id").toInt();

    oPres->direccion1_entrega = (ui->txtdireccion1->text());
    oPres->direccion2_entrega = (ui->txtdireccion2->text());
    oPres->cp_entrega = (ui->txtcp->text());
    oPres->poblacion_entrega = (ui->txtpoblacion->text());
    oPres->provincia_entrega = (ui->txtprovincia->text());

    oPres->id_pais_entrega = Configuracion_global->paises_model->record(ui->cboPais_entrega->currentIndex()).value("id").toInt();
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

    oPres->factura = ui->txtfactura->text();
    oPres->albaran = ui->txtalbaran->text();
    oPres->pedido = ui->txtpedido->text();


    oPres->descripcionFormaPago = ui->cboFormaPago->currentText();

    oPres->id_forma_pago = Configuracion_global->formapago_model->record(ui->cboFormaPago->currentIndex()).value("id").toInt();
    oPres->codigoFormaPago = Configuracion_global->formapago_model->record(ui->cboFormaPago->currentIndex()).value("codigo").toString();

    oPres->atencion_de = (ui->txtatencion_de->text());
    oPres->porc_dto = ui->spin_porc_dto_especial->value();
    oPres->porc_dto_pp = ui->spinPorc_dto_pp->value();

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
    oPres->recargo_equivalencia = ui->chkrecargo_equivalencia->isChecked();
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
    oPres->porc_iva_gasto1 = ui->cboporc_iva_gasto1->currentText().toFloat();
    oPres->porc_iva_gasto2 = ui->cboporc_iva_gasto2->currentText().toFloat();
    oPres->porc_iva_gasto3 = ui->cboporc_iva_gasto3->currentText().toFloat();
    oPres->iva_gasto1 = ui->spinIva_gasto1->value();
    oPres->iva_gasto2 = ui->spinIva_gasto2->value();
    oPres->iva_gasto3 = ui->spinIva_gasto3->value();

    oPres->id_agente = Configuracion_global->agentes_model->record(ui->cboAgente->currentIndex()).value("id").toInt();
    oPres->porc_irpf = ui->spinPorc_irpf->value();
    oPres->irpf = oPres->base *(ui->spinPorc_irpf->value()/100.0);

}
void FrmPresupuestosCli::VaciarCampos()
{
    ui->lblTopNpres->setText("");
    ui->lblTopcliente->setText("");
    ui->txtpresupuesto->setText("0");
    QDate d = QDate::currentDate();
    ui->txtfecha->setDate(d);

    if(d.month() < 12)
        d.setDate(d.year(), d.month() , d.day());
    else
        d.setDate(d.year()+1, 1 , d.day());

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
    ui->txtGastoDist1->setText("");
    ui->txtGastoDist2->setText("");
    ui->txtGastoDist3->setText("");
    ui->spinIva_gasto1->setValue(0);
    ui->spinIva_gasto2->setValue(0);
    ui->spinIva_gasto3->setValue(0);
    ui->cboporc_iva_gasto1->setCurrentIndex(0);
    ui->cboporc_iva_gasto2->setCurrentIndex(0);
    ui->cboporc_iva_gasto3->setCurrentIndex(0);
    ui->SpinGastoDist1->setValue(0);
    ui->SpinGastoDist2->setValue(0);
    ui->SpinGastoDist3->setValue(0);
}

void FrmPresupuestosCli::BloquearCampos(bool state)
{
    //helper.blockTable(state);
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
    m_busqueda->block(!state);
}

void FrmPresupuestosCli::on_chklAprovado_toggled(bool checked)
{
    ui->lblFechaAprovacin->setVisible(checked);
    ui->txtfechaAprovacion->setVisible(checked);
}

void FrmPresupuestosCli::listados()
{
    frmListadosPre f(this);
    f.exec();
}

void FrmPresupuestosCli::on_btnSiguiente_clicked()
{
    if(oPres->siguiente())
    {
        LLenarCampos();
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
    Configuracion_global->empresaDB.transaction();
    Configuracion_global->groupDB.transaction();
    if(Configuracion_global->contabilidad)
        Configuracion_global->contaDB.transaction();

    VaciarCampos();
    BloquearCampos(false);

    if(oPres->AnadirPresupuesto())
    {
        LLenarCampos();
        ui->txtcodigo_cliente->setFocus();
        editando = false;

        ui->stackedWidget->setCurrentIndex(0);
        ui->tabWidget->setCurrentIndex(0);
        ocultarBusqueda();
        emit block();
    }
    else
    {
        Configuracion_global->empresaDB.rollback();
        Configuracion_global->groupDB.rollback();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.rollback();
    }
}

void FrmPresupuestosCli::on_btnEditar_clicked()
{
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if(!ui->tabla->currentIndex().isValid())
            return;
        oPres->RecuperarPresupuesto(QString("select * from cab_pre where id =%1")
                                    .arg(model_busqueda->record(ui->tabla->currentIndex().row()).value("id").toInt()));
    }

    if (oPres->editable)
    {
        Configuracion_global->empresaDB.transaction();
        Configuracion_global->groupDB.transaction();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.transaction();

        BloquearCampos(false);
        editando = true;
        emit block();
        ui->stackedWidget->setCurrentIndex(0);
        ui->tabWidget->setCurrentIndex(0);
    }
    else
    {
        QMessageBox::warning(this
                             ,tr("Editar Presupuesto")
                             ,tr("No se puede editar un Presupuesto que ha sido facturado, solo los NO facturados se pueden editar")+
                             tr("<p><b> Si necesita modificar algo genere una factura nueva y realice el abono correspondiente</b>")+
                             tr("y luego si es preciso realice un nuevo presupuesto y facture de nuevo, o pida al administrador que le desbloquee el presupuesto. ")
                             ,tr("OK"));
    }
    ocultarBusqueda();
}

void FrmPresupuestosCli::on_btnGuardar_clicked()
{
    LLenarPresupuesto();
    Configuracion_global->empresaDB.transaction();
    bool succes = true;

    succes &= oPres->GuardarPres(oPres->id);

    if(succes)
    {
        LLenarCampos();
        succes &= Configuracion_global->empresaDB.commit();
        succes &= Configuracion_global->groupDB.commit();
        if(Configuracion_global->contabilidad)
            succes &= Configuracion_global->contaDB.commit();
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
        Configuracion_global->groupDB.rollback();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.rollback();
    }
}

void FrmPresupuestosCli::on_btndeshacer_clicked()
{
    Configuracion_global->empresaDB.rollback();
    Configuracion_global->groupDB.rollback();
    if(Configuracion_global->contabilidad)
        Configuracion_global->contaDB.rollback();

    VaciarCampos();
    LLenarCampos();
    BloquearCampos(true);
    emit unblock();
}

void FrmPresupuestosCli::on_btnBorrar_clicked()
{
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if(!ui->tabla->currentIndex().isValid())
            return;
        oPres->RecuperarPresupuesto(QString("select * from cab_pre where id =%1")
                                    .arg(model_busqueda->record(ui->tabla->currentIndex().row()).value("id").toInt()));
    }

    if (QMessageBox::question(this,tr("Borrar"),
                              tr("Esta acción no se puede deshacer.\n¿Desea continuar?"),
                              tr("Cancelar"),
                              tr("&Continuar"))== QMessageBox::Accepted)
    {
        bool succes = true;
        Configuracion_global->empresaDB.transaction();
        Configuracion_global->groupDB.transaction();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.transaction();

        QString error;

        succes &= oPres->BorrarLineas(oPres->id);
        succes &= SqlCalls::SqlDelete("cab_pre",Configuracion_global->empresaDB,QString("id = %1").arg(oPres->id),error);

        if(succes)
        {
            succes &= Configuracion_global->empresaDB.commit();
            succes &= Configuracion_global->groupDB.commit();
            if(Configuracion_global->contabilidad)
                succes &= Configuracion_global->contaDB.commit();

            VaciarCampos();
            oPres->id = -1;
            if(ui->stackedWidget->currentIndex() == 0)
                on_btnSiguiente_clicked();
            else
                init_querys();

            TimedMessageBox * t = new TimedMessageBox(this,tr("Borrado con exito"));
        }
        else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
            if(Configuracion_global->contabilidad)
                Configuracion_global->contaDB.rollback();
            QMessageBox::critical(this,tr("Error al borrar presupuesto"),error);
        }
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
    }
}


void FrmPresupuestosCli::convertir_epedido()
{
    if(ui->txtpedido->text() =="0" && ui->txtalbaran->text() =="0" && ui->txtfactura->text() =="0")
    {
        Configuracion_global->empresaDB.transaction();
        Configuracion_global->groupDB.transaction();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.transaction();

        Pedidos oPedido;
        oPedido.id = oPres->id_cliente;
        QString error;
        QHash <QString, QVariant> h;
        h["albaran"] = oPres->albaran;
        h["pedido"] = oPedido.NuevoNumeroPedido();
        h["id_divisa"] = oPres->id_divisa;
        h["pedido_cliente"] = oPedido.numPedidoCliente();
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
        h["id_agente"] = oPres->id_agente;

        h["porc_irpf"] = oPres->porc_irpf;
        h["irpf"] = oPres->irpf;

        int added = SqlCalls::SqlInsert(h,"ped_cli",Configuracion_global->empresaDB,error);

        // Lineas de pedido
        int added_l;
        bool updated_art = true;

        QHash <QString, QVariant> h_l;
        QString d = QString("id_cab = %1").arg(oPres->id);
        QMap<int, QSqlRecord> map = SqlCalls::SelectRecord("lin_pre", d,Configuracion_global->empresaDB, error);

        QMapIterator<int, QSqlRecord> i_l(map);
        while (i_l.hasNext())
        {
            i_l.next();

            QSqlRecord r_l = i_l.value();
            //COPY RECORD VALUES TO HASH
            for(int i= 1; i< r_l.count();i++)
                h_l.insert(r_l.fieldName(i),r_l.value(i));

            h_l["id_cab"] = added;

            added_l = SqlCalls::SqlInsert(h_l,"lin_ped",Configuracion_global->empresaDB,error);

            if(added_l == -1)
                break;
            else
            {
                //UPDATE ART
                int id_art = r_l.value("id_articulo").toInt();
                double cant = r_l.value("cantidad").toDouble();
                QSqlQuery queryart(Configuracion_global->groupDB);
                if(!queryart.exec(QString("update articulos set unidades_reservadas = unidades_reservadas + %1 where id = %2")
                                  .arg(cant)
                                  .arg(QString::number(id_art))
                                  ))
                {
                    error = queryart.lastError().text();
                    updated_art = false;
                    break;
                }

            }
        }

        if(added_l == -1 || added == -1 || !updated_art)
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
            if(Configuracion_global->contabilidad)
                Configuracion_global->contaDB.rollback();
            QMessageBox::warning(this,tr("Error al convertir en pedido"),error);
        }
        else //ALL OK
        {
            QHash <QString, QVariant> p;
            if(!oPres->aprobado)
            {
                p["aprobado"] = true;
                p["fecha_aprobacion"] = QDate::currentDate();
            }
            p["pedido"] = h["pedido"];
            p["editable"] = false;
            QString c = QString("id = %1").arg(oPres->id);
            bool updated = SqlCalls::SqlUpdate(p,"cab_pre",Configuracion_global->empresaDB,c,error);

            if(updated)
            {
                Configuracion_global->empresaDB.commit();
                Configuracion_global->empresaDB.commit();
                Configuracion_global->groupDB.commit();
                if(Configuracion_global->contabilidad)
                    Configuracion_global->contaDB.commit();

                QString num = h["pedido"].toString();
                TimedMessageBox* t = new TimedMessageBox(this,
                                                         tr("Se ha creado el pedido num:")+num);
                oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(oPres->id));
                LLenarCampos();
            }
            else
            {
                QMessageBox::warning(this,tr("Error al convertir en pedido"),error);
                Configuracion_global->empresaDB.rollback();
                Configuracion_global->groupDB.rollback();
                if(Configuracion_global->contabilidad)
                    Configuracion_global->contaDB.rollback();
            }
        }
    }
    else //ALREADY CONVERTED
    {
        if(ui->txtfactura->text() != "0")
            TimedMessageBox *t = new TimedMessageBox(this,tr("Este presupuesto ya ha sido pasado a factura: %1").arg(oPres->factura));
        else if(ui->txtalbaran->text() !="0")
            TimedMessageBox *t = new TimedMessageBox(this,tr("Este presupuesto ya ha sido pasado a albarán: %1").arg(oPres->albaran));
        else
            TimedMessageBox *t = new TimedMessageBox(this,tr("Este presupuesto ya ha sido pasado a pedido: %1").arg(oPres->pedido));
    }
}

void FrmPresupuestosCli::convertir_ealbaran()
{
    if(ui->txtpedido->text() =="0" && ui->txtalbaran->text() =="0" && ui->txtfactura->text() =="0")
    {
        Configuracion_global->empresaDB.transaction();
        Configuracion_global->groupDB.transaction();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.transaction();

        QString c = QString("id = %1").arg(oPres->id);
        QString error;
        QHash <QString, QVariant> h;
        h["id_agente"] = oPres->id_agente;
        h["id_divisa"] = oPres->id_divisa;
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
        h["fax"] = oPres->fax;
        h["movil"] = oPres->movil;
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
        h["impreso"] = 0;
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

        h["porc_irpf"] = oPres->porc_irpf;
        h["irpf"] = oPres->irpf;

        Albaran oAlbaran;

        QString serie;
        if(Configuracion_global->serie.isEmpty())
        {
            //-----------------------
            // Elección de serie
            //-----------------------
            QDialog dlg(this);
            dlg.setWindowTitle(tr("Seleccione serie albarán"));
            dlg.resize(170,150);
            QComboBox box(&dlg);
            QPushButton  btn("Aceptar",&dlg);
            QPushButton  cancel("Cancelar",&dlg);
            QGridLayout lay(&dlg);

            lay.addWidget(&box,0,0,2,1);
            lay.addWidget(&btn,1,0,1,1);
            lay.addWidget(&cancel,1,1,1,1);

            dlg.setLayout(&lay);

            QSqlQueryModel *l = new QSqlQueryModel(this);
            l->setQuery("select serie from series",Configuracion_global->empresaDB);
            box.setModel(l);

            connect(&btn,SIGNAL(clicked()),&dlg,SLOT(accept()));
            connect(&cancel,SIGNAL(clicked()),&dlg,SLOT(reject()));
            if(dlg.exec() == QDialog::Rejected)
            {
                Configuracion_global->empresaDB.rollback();
                Configuracion_global->groupDB.rollback();
                if(Configuracion_global->contabilidad)
                    Configuracion_global->contaDB.rollback();
                return;
            }

            serie = box.currentText();
        }
        else
            serie = Configuracion_global->serie;

        int num = oAlbaran.NuevoNumeroAlbaran(serie);
        h["albaran"] =num;
        h["serie"] = serie;
        h["fecha"] = QDate::currentDate();

        int added = SqlCalls::SqlInsert(h,"cab_alb",Configuracion_global->empresaDB,error);

        // Lineas de albaran
        int added_l;
        bool updated_art = true;

        QString d = QString("id_cab = %1").arg(oPres->id);
        QMap<int, QSqlRecord> map = SqlCalls::SelectRecord("lin_pre", d,Configuracion_global->empresaDB, error);
        QMapIterator<int, QSqlRecord> i_l(map);
        while (i_l.hasNext())
        {
            i_l.next();
            QSqlRecord r_l = i_l.value();

            QHash <QString, QVariant> h_l;
            for(int i= 1; i< r_l.count();i++)
                h_l.insert(r_l.fieldName(i),r_l.value(i));

            h_l["id_cab"] = added;

            h_l.remove("cantidad_a_servir");
            added_l = SqlCalls::SqlInsert(h_l,"lin_alb",Configuracion_global->empresaDB,error);
            if(added_l == -1)
                break;
            else
            {
                //UPDATE ART
                int id_art = r_l.value("id_articulo").toInt();
                double cant = r_l.value("cantidad").toDouble();
                double total = r_l.value("total").toDouble();
                if(!Articulo::acumulado_ventas(id_art,cant,total,QDate::currentDate(),"V"))
                {
                    error = Configuracion_global->groupDB.lastError().text();
                    updated_art = false;
                    break;
                }

            }

        }
        if(added_l == -1 || added == -1 ||!updated_art)
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
            if(Configuracion_global->contabilidad)
                Configuracion_global->contaDB.rollback();
            QMessageBox::warning(this,tr("Error al convertir en pedido"),error);
        }
        else
        {
            QHash <QString, QVariant> p;
            if(!oPres->aprobado)
            {
                p["aprobado"] = true;
                p["fecha_aprobacion"] = QDate::currentDate();
            }
            p["albaran"] = QString("%1/%2").arg(serie).arg(num);
            p["editable"] = false;
            c = "id="+QString::number(oPres->id);
            bool updated = SqlCalls::SqlUpdate(p,"cab_pre",Configuracion_global->empresaDB,c,error);

            if(updated)
            {               
                ui->txtalbaran->setText(QString::number(num));

                bool commited = Configuracion_global->empresaDB.commit();
                commited &= Configuracion_global->groupDB.commit();
                if(Configuracion_global->contabilidad)
                    commited &= Configuracion_global->contaDB.commit();

                if(commited)
                    TimedMessageBox* t = new TimedMessageBox(this,tr("Se ha creado el albarán num: ")+QString::number(num));
                else
                    QMessageBox::warning(this,tr("Gestión de presupuestos"),tr("Error en la transacción"),tr("Aceptar"));

            }
            else
            {
                QMessageBox::warning(this,tr("Traspaso a albarán"),tr("No se pudo crear el albarán"),tr("Aceptar"));
                Configuracion_global->empresaDB.rollback();
                Configuracion_global->groupDB.rollback();
                if(Configuracion_global->contabilidad)
                    Configuracion_global->contaDB.rollback();
            }
        }
    }
    else
    {
        if(ui->txtfactura->text() != "0")
            TimedMessageBox *t = new TimedMessageBox(this,tr("Este presupuesto ya ha sido pasado a factura: %1").arg(oPres->factura));
        else if(ui->txtalbaran->text() !="0")
            TimedMessageBox *t = new TimedMessageBox(this,tr("Este presupuesto ya ha sido pasado a albarán: %1").arg(oPres->albaran));
        else
            QMessageBox::information(this,tr("Traspaso anterior"),tr("Este presupuesto ya ha sido pasado a pedido: %1.\nTraspase dicho pedido a albarán.").arg(oPres->pedido));
    }
}

void FrmPresupuestosCli::convertir_enFactura()
{
    if(ui->txtpedido->text() =="0" && ui->txtalbaran->text() =="0" && ui->txtfactura->text() =="0")
    {
        if(QMessageBox::question(this,tr("Presupuestos a clientes"),tr("¿Desea realmente facturar este presupuesto?"),
                                 tr("No"),tr("Sí"))==QMessageBox::Accepted)
        {
            Configuracion_global->empresaDB.transaction();
            Configuracion_global->groupDB.transaction();
            if(Configuracion_global->contabilidad)
                Configuracion_global->contaDB.transaction();

            bool transaccion = true;

            Factura oFactura(this);
            QString serie;
            if(Configuracion_global->serie.isEmpty())
            {
                //-----------------------
                // Elección de serie
                //-----------------------
                QDialog dlg(this);
                dlg.setWindowTitle(tr("Seleccione serie albarán"));
                dlg.resize(170,150);
                QComboBox box(&dlg);
                QPushButton  btn("Aceptar",&dlg);
                QPushButton  cancel("Cancelar",&dlg);
                QGridLayout lay(&dlg);

                lay.addWidget(&box,0,0,2,1);
                lay.addWidget(&btn,1,0,1,1);
                lay.addWidget(&cancel,1,1,1,1);

                dlg.setLayout(&lay);

                QSqlQueryModel *l = new QSqlQueryModel(this);
                l->setQuery("select serie from series",Configuracion_global->empresaDB);
                box.setModel(l);

                connect(&btn,SIGNAL(clicked()),&dlg,SLOT(accept()));
                connect(&cancel,SIGNAL(clicked()),&dlg,SLOT(reject()));
                if(dlg.exec() == QDialog::Rejected)
                {
                    Configuracion_global->empresaDB.rollback();
                    Configuracion_global->groupDB.rollback();
                    if(Configuracion_global->contabilidad)
                        Configuracion_global->contaDB.rollback();
                    return;
                }

                serie = box.currentText();
            }
            else
                serie = Configuracion_global->serie;

            // LLENAR CABECERA
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
            cab_fac["porc_irpf"] = oPres->porc_irpf;
            cab_fac["irpf"] = oPres->irpf;
            cab_fac["iva"] = oPres->total_iva;
            cab_fac["total"] = oPres->total;
            cab_fac["impreso"]= oPres->impreso;
            cab_fac["cobrado"] = false;
            cab_fac["contabilizado"] = false;
            cab_fac["id_forma_pago"] = oPres->id_forma_pago;
            cab_fac["forma_pago"] = oPres->codigoFormaPago;
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
            cab_fac["porc_iva_gasto2"] = oPres->porc_iva_gasto2;
            cab_fac["porc_iva_gasto3"] = oPres->porc_iva_gasto3;
            cab_fac["iva_gasto1"] = oPres->iva_gasto1;
            cab_fac["iva_gasto2"] = oPres->iva_gasto2;
            cab_fac["iva_gasto3"] = oPres->iva_gasto3;
            cab_fac["id_transportista"] = oPres->id_transportista;
            cab_fac["ejercicio"] = Configuracion_global->cEjercicio.toInt();
            cab_fac["editable"] = true;
            cab_fac["pendiente"] = oPres->total;

            cab_fac["id_divisa"] = oPres->id_divisa;
            cab_fac["id_agente"] = oPres->id_agente;
            QString error;

            int new_id = SqlCalls::SqlInsert(cab_fac,"cab_fac",Configuracion_global->empresaDB,error);

            if(new_id > -1)
            {
                // CARGAR FACTURA
                oFactura.RecuperarFactura(new_id);

                //  INSERTAR LÍNEAS
                QSqlQuery lineas_pre(Configuracion_global->empresaDB);

                if(lineas_pre.exec("Select * from lin_pre where id_cab ="+QString::number(oPres->id)))
                {
                    QHash <QString, QVariant> h_lineas_fac;
                    while (lineas_pre.next())
                    {

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
                                                 tr("Ocurrió un error al crear las líneas de factura:\n%1").arg(error));

                            transaccion = false;
                        }
                        else
                        {
                            //UPDATE ART
                            int id_art = h_lineas_fac.value("id_articulo").toInt();
                            double cant = h_lineas_fac.value("cantidad").toDouble();
                            double total = h_lineas_fac.value("total").toDouble();
                            if(!Articulo::acumulado_ventas(id_art,cant,total,QDate::currentDate(),"V"))
                            {
                                QMessageBox::warning(this,tr("Pedidos cliente"),
                                                     tr("Ocurrió un error al crear las líneas de factura:\n%1").arg(Configuracion_global->groupDB.lastError().text()));

                                transaccion = false;
                                break;
                            }
                            //TODO ACUMULADOS CLIENTE
                        }
                    }
                }

                if(transaccion)
                {
                    ui->btn_convertir->setEnabled(false);

                    // Insertamos datos factura en presupuesto
                    oPres->factura = QString("%1/%2").arg(oFactura.serie).arg(oFactura.factura);
                    oPres->editable = false;
                    oPres->aprobado = true;
                    oPres->GuardarPres(oPres->id);
                    ui->txtfactura->setText(oPres->factura);                                        

                    vencimientos vto(this);

                    // Asiento contable
                    bool creado = true;
                    if(Configuracion_global->contabilidad)
                    {

                        oClientePres->Recuperar("select * from clientes where id = "+QString::number(oFactura.id_cliente));
                        if(oFactura.apunte == 0)
                            creado = oFactura.Apunte();
                        else
                            creado = oFactura.EditApunte(oFactura.apunte);
                    }

                    // Vencimiento
                    if(creado && vto.calcular_vencimiento(oFactura.fecha,oFactura.id_cliente,0,oFactura.id,(oFactura.serie+"/"+oFactura.factura),1,
                                             "V",oFactura.total))
                    {
                        QString texto;
                        texto = tr("Se ha creado una nueva factura.\ncon el número ")+ cab_fac.value("factura").toString()+
                                tr("\n y de importe: ")+QString::number(cab_fac.value("total").toDouble(),'f',Configuracion_global->decimales);
                        TimedMessageBox * t = new TimedMessageBox(this,texto);

                        bool commited = Configuracion_global->empresaDB.commit();
                        commited &= Configuracion_global->groupDB.commit();
                        if(Configuracion_global->contabilidad)
                            commited &= Configuracion_global->contaDB.commit();
                        oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(oPres->id));
                        LLenarCampos();
                    }
                    else
                    {
                        Configuracion_global->empresaDB.rollback();
                        Configuracion_global->groupDB.rollback();
                        if(Configuracion_global->contabilidad)
                            Configuracion_global->contaDB.rollback();
                        QMessageBox::warning(this,tr("Traspaso a factura"),tr("No se pudo crear la factura "),tr("Aceptar"));
                    }

                }
                else
                {
                    Configuracion_global->empresaDB.rollback();
                    Configuracion_global->groupDB.rollback();
                    if(Configuracion_global->contabilidad)
                        Configuracion_global->contaDB.rollback();
                    QMessageBox::warning(this,tr("Traspaso a factura"),tr("No se pudo crear la factura "),tr("Aceptar"));                    
                }
            }
            else
            {
                Configuracion_global->empresaDB.rollback();
                Configuracion_global->groupDB.rollback();
                if(Configuracion_global->contabilidad)
                    Configuracion_global->contaDB.rollback();
                QMessageBox::warning(this,tr("Presupuestos"),
                                     tr("No se ha podido crear la factura"),tr("Aceptar"));
            }
        }
    }
    else
    {
        if(ui->txtfactura->text() != "0")
            TimedMessageBox *t = new TimedMessageBox(this,tr("Este presupuesto ya ha sido pasado a factura: %1").arg(oPres->factura));
        else if(ui->txtalbaran->text() !="0")
            QMessageBox::information(this,tr("Traspaso anterior"),tr("Este presupuesto ya ha sido pasado a albarán: %1.\nFacture dicho albarán.").arg(oPres->albaran));
        else
            QMessageBox::information(this,tr("Traspaso anterior"),tr("Este presupuesto ya ha sido pasado a pedido: %1.\nFacture dicho pedido.").arg(oPres->pedido));
    }
}

void FrmPresupuestosCli::on_txtcodigo_cliente_editingFinished()
{
    if(ui->txtcodigo_cliente->text() != oPres->codigo_cliente)
    {
        oClientePres->Recuperar("select * from clientes where codigo_cliente='"+ui->txtcodigo_cliente->text()+"'");
        LLenarCamposCliente();
    }
}

void FrmPresupuestosCli::on_btnImprimir_clicked()
{
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if(!ui->tabla->currentIndex().isValid())
            return;
        oPres->RecuperarPresupuesto(QString("select * from cab_pre where id =%1")
                                    .arg(model_busqueda->record(ui->tabla->currentIndex().row()).value("id").toInt()));
    }

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
            QMap <QString,QString> parametros_sql;
            parametros_sql["Empresa.cab_pre"] = QString("id = %1").arg(oPres->id);
            parametros_sql["Empresa.lin_pre"] = QString("id_cab = %1").arg(oPres->id);
            QString report = "presupuesto_"+QString::number(oClientePres->ididioma);

            QString pdfname = QString("Presupuesto-%1").arg(oPres->presupuesto);
            QString asunto = tr("Envio de presupuesto %1").arg(oPres->presupuesto);
            QString texto = tr("Estimado/a %1:\n"
                             "Le adjunto el presupuesto nº %2.\n\n"
                             "Un saludo.").arg(oClientePres->nombre_fiscal).arg(oPres->presupuesto);
            QMap <QString,QString> parametros;
            //TODO add parametros
            switch (valor) {
            case 1: // Impresora
                Configuracion::ImprimirDirecto(report,parametros_sql,parametros);
                break;
            case 2: // email
                Configuracion::EviarMail(report,parametros_sql,parametros,pdfname,oClientePres->email,oClientePres->nombre_fiscal,asunto,texto);
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

void FrmPresupuestosCli::formato_tablas()
{
    QStringList headers;
    QVariantList sizes;
    headers << "id" << tr("Presupuesto") << tr("Fecha") <<tr("Teléfono") <<tr("Movil")  <<tr("Cliente");//TODO aprovado
    sizes << 0 << 120 << 120 << 120 << 120 << 550;
    for(int i = 0; i < headers.length();i++)
    {
        ui->tabla->setColumnWidth(i,sizes.at(i).toInt());
        model_busqueda->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
    QStringList header2;
    QVariantList sizes2;
    header2 << tr("id") << tr("Código") << tr("Descripción") << tr("cantidad") << tr("pvp recom")<< tr("porc_dto")  << tr("pvp cli.") << tr("Subtotal");
    header2 << tr("porc_iva") << tr("Total");
    sizes2 << 0 << 100 << 300 << 100 << 100 <<100 << 100 <<100 << 100 <<110;
    for(int i = 0; i <header2.size();i++)
    {
        ui->Lineas->setColumnWidth(i,sizes2.at(i).toInt());
        modelLineas->setHeaderData(i,Qt::Horizontal,header2.at(i));
    }
    ui->Lineas->setColumnHidden(0,true);
}

void FrmPresupuestosCli::filter_table(QString texto, QString orden, QString modo)
{

    QHash <QString,QString> h;
    h[tr("presupuesto")] = "presupuesto";
    h[tr("cliente")] = "cliente";
    h[tr("teléfono")] = "telefono";

    ui->stackedWidget->setCurrentIndex(1);
    QString order = h.value(orden);

    if(modo == tr("A-Z"))
        modo = "";
    else
        modo = "Desc";

    model_busqueda->setQuery("select id,presupuesto,fecha,telefono,movil,cliente from cab_pre where "+order+
                             " like '%"+texto+"%' order by "+order+" "+modo, Configuracion_global->empresaDB);
    if(ui->stackedWidget->currentIndex() == 0)
        ui->stackedWidget->setCurrentIndex(1);
    ui->tabla->selectRow(0);
}

void FrmPresupuestosCli::calcular_presupuesto()
{
    double subtotal,dto,dtopp,base,irpf,iva,re,total;
    double base1,iva1,re1,total1;
    double base2,iva2,re2,total2;
    double base3,iva3,re3,total3;
    double base4,iva4,re4,total4;
    base1= 0; base2 = 0; base3 =0;base4 = 0;
    iva1=0; iva2 =0; iva3=0; iva4 = 0;
    re1=0;re2=0;re3=0;re4 =0;
    total1 =0; total2 =0;total3=0; total4=0;
    subtotal =0; dto=0; dtopp =0; base =0; irpf = 0; iva =0; re =0; total =0;

    ui->spinIva_gasto1->setValue((ui->SpinGastoDist1->value()*(ui->cboporc_iva_gasto1->currentText().toDouble()/100)));
    ui->spinIva_gasto2->setValue((ui->SpinGastoDist2->value()*(ui->cboporc_iva_gasto2->currentText().toDouble()/100)));
    ui->spinIva_gasto3->setValue((ui->SpinGastoDist3->value()*(ui->cboporc_iva_gasto3->currentText().toDouble()/100)));

    double _dtoPP = 1-((ui->spinPorc_dto_pp->value()+ui->spin_porc_dto_especial->value())/100);

    QMap <int,QSqlRecord> l;
    QString error;
    l = SqlCalls::SelectRecord("lin_pre",QString("id_cab=%1").arg(oPres->id),Configuracion_global->empresaDB,error);
    QMapIterator <int,QSqlRecord> li(l);
    while(li.hasNext())
    {
        li.next();

        subtotal += li.value().value("subtotal").toDouble();
        // base1
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva1->text().toFloat())
            base1 += li.value().value("total").toDouble()* _dtoPP ;

        // base2
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva2->text().toFloat())
            base2 += li.value().value("total").toDouble()* _dtoPP ;

        // base3
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva3->text().toFloat())
            base3 += li.value().value("total").toDouble()* _dtoPP ;

        // base4
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva4->text().toFloat())
            base4 += li.value().value("total").toDouble()* _dtoPP ;
    }

    // añadir gastos extras
    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist3->value();
    iva1 = base1 * (ui->txtporc_iva1->text().toFloat()/100);

    if(ui->chkrecargo_equivalencia->isChecked())
        re1 = base1 * (ui->txtporc_rec1->text().toFloat()/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist3->value();
    iva2 = base2 * (ui->txtporc_iva2->text().toFloat()/100);

    if(ui->chkrecargo_equivalencia->isChecked())
        re2 = base2 * (ui->txtporc_rec2->text().toFloat()/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist3->value();
    iva3 = base3 * (ui->txtporc_iva3->text().toFloat()/100);

    if(ui->chkrecargo_equivalencia->isChecked())
        re3 = base3 * (ui->txtporc_rec3->text().toFloat()/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist3->value();
    iva4 = base4 * (ui->txtporc_iva4->text().toFloat()/100);
    if(ui->chkrecargo_equivalencia->isChecked())
        re4 = base4 * (ui->txtporc_rec4->text().toFloat()/100);

    // TOTALES GENERALES

    dtopp = subtotal *(ui->spinPorc_dto_pp->value()/100.0);
    double _dto = subtotal *(ui->spin_porc_dto_especial->value()/100.0);
    base = base1 + base2+base3+base4;
    iva = iva1 + iva2 +iva3+iva4;
    re  = re1 +re2 + re3 + re4;
    irpf = base *(ui->spinPorc_irpf->value()/100.0);

    total = (base - irpf) + iva +re;
    total1 = base1 +iva1 +re1;
    total2 = base2 +iva2 +re2;
    total3 = base3 +iva3 +re3;
    total4 = base4 +iva4 +re4;


    ui->txtimporte_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(irpf ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtDto_pp->setText(Configuracion_global      ->toFormatoMoneda(QString::number(dtopp,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtdto->setText(Configuracion_global         ->toFormatoMoneda(QString::number(_dto ,'f',Configuracion_global->decimales_campos_totales)));


    // Desglose llenar controles iva 1
    ui->txtbase1->setText(Configuracion_global ->toFormatoMoneda(QString::number(base1 ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva1->setText(Configuracion_global  ->toFormatoMoneda(QString::number(iva1  ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec1->setText(Configuracion_global  ->toFormatoMoneda(QString::number(re1   ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total1,'f',Configuracion_global->decimales_campos_totales)));

    // Desglose llenar controles iva 2
    ui->txtbase2->setText(Configuracion_global ->toFormatoMoneda(QString::number(base2 ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva2->setText(Configuracion_global  ->toFormatoMoneda(QString::number(iva2  ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec2->setText(Configuracion_global  ->toFormatoMoneda(QString::number(re2   ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total2,'f',Configuracion_global->decimales_campos_totales)));

    // Desglose llenar controles iva 3
    ui->txtbase3->setText(Configuracion_global ->toFormatoMoneda(QString::number(base3 ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva3->setText(Configuracion_global  ->toFormatoMoneda(QString::number(iva3  ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec3->setText(Configuracion_global  ->toFormatoMoneda(QString::number(re3   ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total3,'f',Configuracion_global->decimales_campos_totales)));

    // Desglose llenar controles iva 4
    ui->txtbase4->setText(Configuracion_global ->toFormatoMoneda(QString::number(base4 ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva4->setText(Configuracion_global  ->toFormatoMoneda(QString::number(iva4  ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec4->setText(Configuracion_global  ->toFormatoMoneda(QString::number(re4   ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total4,'f',Configuracion_global->decimales_campos_totales)));

    this->moneda = moneda;
    ui->txtbase->setText(Configuracion_global    ->toFormatoMoneda(QString::number(base                ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal            ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText(Configuracion_global     ->toFormatoMoneda(QString::number(iva                 ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec->setText(Configuracion_global     ->toFormatoMoneda(QString::number(re                  ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global   ->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));

    //ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    //ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re                  ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_2->setText(Configuracion_global        ->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));
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
    connect(m_busqueda,SIGNAL(hideMe()),this, SLOT(ocultarBusqueda()));
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));


    QPushButton* add = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir"),this);
    connect(add,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(add);

    QPushButton* edit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar"),this);
    connect(edit,SIGNAL(clicked()),this,SLOT(on_btnEditar_clicked()));
    m_busqueda->addWidget(edit);

    QPushButton* print = new QPushButton(QIcon(":/Icons/PNG/print2.png"),tr("Imprimir"),this);
    connect(print,SIGNAL(clicked()),this,SLOT(on_btnImprimir_clicked()));
    m_busqueda->addWidget(print);

    QPushButton* del = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    connect(del,SIGNAL(clicked()),this,SLOT(on_btnBorrar_clicked()));
    m_busqueda->addWidget(del);

    m_busqueda->addSpacer();
    QPushButton* list = new QPushButton(QIcon(":/Icons/PNG/reports.png"),tr("Listados"),this);
    connect(list,SIGNAL(clicked()),this,SLOT(listados()));
    m_busqueda->addWidget(list);

    connect(m_busqueda,SIGNAL(key_Down_Pressed()),ui->tabla,SLOT(setFocus()));
    connect(m_busqueda,SIGNAL(key_F2_Pressed()),this,SLOT(ocultarBusqueda()));

}

void FrmPresupuestosCli::on_tabla_doubleClicked(const QModelIndex &index)
{
    int id = ui->tabla->model()->data(ui->tabla->model()->index(index.row(),0),Qt::EditRole).toInt();
    oPres->RecuperarPresupuesto("select * from cab_pre where id ="+QString::number(id));
    LLenarCampos();
    ui->stackedWidget->setCurrentIndex(0);
    BloquearCampos(true);
    ocultarBusqueda();
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

void FrmPresupuestosCli::mostrarBusqueda()
{
    _showBarraBusqueda(m_busqueda);
    m_busqueda->doFocustoText();
}

void FrmPresupuestosCli::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}

bool FrmPresupuestosCli::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Show && obj == this)
    {
        init_querys();
    }

    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if(obj == ui->txtcodigo_cliente && keyEvent->key() == Qt::Key_F1)
            on_botBuscarCliente_clicked();

        else if(keyEvent->key() ==Qt::Key_F1 && ui->btnEditar->isEnabled())
        {
            if(m_busqueda->isShow())
                ocultarBusqueda();
            else
                mostrarBusqueda();
        }
        else if(keyEvent->key() == Qt::Key_Return)
            if(ui->stackedWidget->currentIndex() ==1)
                on_tabla_doubleClicked(ui->tabla->currentIndex());

        if(keyEvent->key() == Qt::Key_Escape)
            return true;
    }
    if(event->type() == QEvent::Resize)
        _resizeBarraBusqueda(m_busqueda);

    return MayaModule::eventFilter(obj,event);
}

void FrmPresupuestosCli::on_btnAnadirLinea_clicked()
{
    if(ui->btnGuardar->isEnabled())
    {
        frmEditLine frmeditar(this);
        connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(refrescar_modelo()));
        frmeditar.set_acumula(false);
        frmeditar.set_reserva(false);
        frmeditar.set_linea(0,"lin_pre");
        frmeditar.set_tabla("lin_pre");
        frmeditar.set_id_cliente(oClientePres->id);
        frmeditar.set_id_tarifa(oClientePres->idTarifa);
        frmeditar.set_id_cab(oPres->id);
        frmeditar.set_tipo("V");
        if(frmeditar.exec() == QDialog::Accepted)
            refrescar_modelo();
        calcular_presupuesto();

    } else{
        QMessageBox::warning(this,tr("Gestión de Pedidos"),tr("Debe editar el pedido para añadir líneas"),
                             tr("Aceptar"));
    }
}

void FrmPresupuestosCli::refrescar_modelo()
{
    modelLineas->setQuery(QString("select id,codigo,descripcion,cantidad,precio_recom, porc_dto, precio,subtotal,porc_iva,total "
                                  "from lin_pre where id_cab = %1;").arg(oPres->id),Configuracion_global->empresaDB);
    formato_tablas();
}

void FrmPresupuestosCli::on_Lineas_doubleClicked(const QModelIndex &index)
{
    if(ui->btnGuardar->isEnabled())
    {
        int id_lin = ui->Lineas->model()->data(index.model()->index(index.row(),0)).toInt();
        if(id_lin >0)
        {

            frmEditLine frmeditar(this);
            connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(refrescar_modelo()));
            frmeditar.set_acumula(false);
            frmeditar.set_reserva(false);
            frmeditar.set_id_cliente(oClientePres->id);
            frmeditar.set_id_tarifa(oClientePres->idTarifa);
            frmeditar.set_id_cab(oPres->id);
            frmeditar.set_tipo("V");
            frmeditar.set_linea(id_lin,"lin_pre");
            frmeditar.set_tabla("lin_pre");
            frmeditar.set_editando();
            frmeditar.exec();

            calcular_presupuesto();
            ui->Lineas->setFocus();

        } else
        {
            QMessageBox::warning(this, tr("Gestión de Pedidos"),tr("Debe editar el pedido para poder modificar las líneas"),
                                 tr("Aceptar"));

        }
    }
}

void FrmPresupuestosCli::on_btnPrforma_clicked()
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
            QMap <QString,QString> parametros_sql;
            parametros_sql["Empresa.cab_pre"] = QString("id = %1").arg(oPres->id);
            parametros_sql["Empresa.lin_pre"] = QString("id_cab = %1").arg(oPres->id);
            QString report = "proforma_"+QString::number(oClientePres->ididioma);

            QString pdfname = QString("Proforma-%1").arg(oPres->presupuesto);
            QString asunto = tr("Envio de proforma %1").arg(oPres->presupuesto);
            QString texto = tr("Estimado/a %1:\n"
                             "Le adjunto el presupuesto nº %2.\n\n"
                             "Un saludo.").arg(oClientePres->nombre_fiscal).arg(oPres->presupuesto);

            QMap <QString,QString> parametros;
            //TODO add parametros
            switch (valor) {
            case 1: // Impresora
                Configuracion::ImprimirDirecto(report,parametros_sql,parametros);
                break;
            case 2: // email
                Configuracion::EviarMail(report,parametros_sql,parametros,pdfname,oClientePres->email,oClientePres->nombre_fiscal,asunto,texto);
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

void FrmPresupuestosCli::on_SpinGastoDist1_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    calcular_presupuesto();
}
void FrmPresupuestosCli::on_SpinGastoDist2_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    calcular_presupuesto();
}
void FrmPresupuestosCli::on_SpinGastoDist3_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    calcular_presupuesto();
}

void FrmPresupuestosCli::on_cboporc_iva_gasto1_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    calcular_presupuesto();
}
void FrmPresupuestosCli::on_cboporc_iva_gasto2_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    calcular_presupuesto();
}
void FrmPresupuestosCli::on_cboporc_iva_gasto3_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    calcular_presupuesto();
}

void FrmPresupuestosCli::on_spinPorc_dto_pp_valueChanged(double)
{
    calcular_presupuesto();
}

void FrmPresupuestosCli::on_spin_porc_dto_especial_valueChanged(double)
{
    calcular_presupuesto();
}

void FrmPresupuestosCli::on_spinPorc_irpf_valueChanged(double)
{
    calcular_presupuesto();
}

void FrmPresupuestosCli::on_chkrecargo_equivalencia_toggled(bool checked)
{
    if (checked) {
        ui->chkrecargo_equivalencia->setChecked(true);
        oPres->recargo_equivalencia = 1;
        ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
        ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
        ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
        ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));
        oPres->porc_rec1 = ui->txtporc_rec1->text().toFloat();
        oPres->porc_rec2 = ui->txtporc_rec2->text().toFloat();
        oPres->porc_rec3 = ui->txtporc_rec3->text().toFloat();
        oPres->porc_rec4 = ui->txtporc_rec4->text().toFloat();
    } else {
        ui->chkrecargo_equivalencia->setChecked(false);
        oPres->recargo_equivalencia = 0;
        ui->txtporc_rec1->setText("0.00");
        ui->txtporc_rec2->setText("0.00");
        ui->txtporc_rec3->setText("0.00");
        ui->txtporc_rec4->setText("0.00");
        oPres->porc_rec1 = 0;
        oPres->porc_rec2 = 0;
        oPres->porc_rec3 = 0;
        oPres->porc_rec4 = 0;
    }
    calcular_presupuesto();
}
