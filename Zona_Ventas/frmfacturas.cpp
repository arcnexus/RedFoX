#include "frmfacturas.h"
#include "ui_frmfacturas.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Zona_Contabilidad/apuntes.h"
#include "../Auxiliares/frmaddentregascuenta.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/monetarydelegate_totals.h"
#include "../Auxiliares/numericdelegate.h"
#include "../Auxiliares/datedelegate.h"
#include "../Almacen/frmarticulos.h"
#include "../Zona_Ventas/frmgestioncobros.h"
#include "../vencimientos.h"
#include "../Auxiliares/frmeditline.h"


#include "../Almacen/articulo.h"

frmFacturas::frmFacturas( QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmFacturas),
    //helper(this),
    menuButton(QIcon(":/Icons/PNG/Factura.png"),tr("Facturas"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/Factura.png"),"",this))

{
    oFactura = new Factura(this);
    oCliente1 = new Cliente(this);
    oArticulo = new Articulo(this);
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
    ui->cboPais_entrega->setModel(Configuracion_global->paises_model);

    ui->comboPais->setModelColumn(1);
    ui->cboPais_entrega->setModelColumn(1);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de facturas a clientes"));
    ui->tabWidget_2->setCurrentIndex(0);
    // ------------------------------
    // Modelo y formato tabla lineas
    //-------------------------------
    modelLineas = new QSqlQueryModel(this);
    modelLineas->setQuery("select id,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                          "from lin_fac where id = 0;",Configuracion_global->empresaDB);
    ui->Lineas->setModel(modelLineas);

    QStringList header;
    QVariantList sizes;
    header << tr("id") << tr("Código") << tr("Descripción") << tr("cantidad") << tr("pvp") << tr("pvp recom.") << tr("Subtotal");
    header << tr("porc_dto") << tr("porc_iva") << tr("Total");
    sizes << 0 << 100 << 300 << 100 << 100 <<100 << 100 <<100 << 100 <<110;
    for(int i = 0; i <header.size();i++)
    {
        ui->Lineas->setColumnWidth(i,sizes.at(i).toInt());
        modelLineas->setHeaderData(i,Qt::Horizontal,header.at(i));
    }
    ui->Lineas->setItemDelegateForColumn(3,new NumericDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(4,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(5,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(6,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(7,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(8,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(9,new MonetaryDelegate_totals(this,true));



    //--------------------------
    // Pongo valores por defecto
    //--------------------------
    ui->lbcontabilizada->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lblFacturaImpresa->setVisible(false);

    //-------------------------
    // Rellenar formas de pago
    //-------------------------
    ui->cboforma_pago->setModel(Configuracion_global->formapago_model);
    ui->cboforma_pago->setModelColumn(2);
    //-------------------------
    // valores edicion
    //-------------------------
    this->Altas = false;
    ui->stackedWidget->setCurrentIndex(1);

    //----------------------------
    // Rellenar divisas
    //----------------------------
    ui->cboDivisa->setModel(Configuracion_global->divisas_model);
    ui->cboDivisa->setModelColumn(1);

    //-----------------------
    // Rellenar ivas gastos
    //-----------------------
    ui->cbo_porc_gasto_iva1->addItems(Configuracion_global->ivaList);
    ui->cbo_porc_gasto_iva2->addItems(Configuracion_global->ivaList);
    ui->cbo_porc_gasto_iva3->addItems(Configuracion_global->ivaList);
    ui->cbo_porc_gasto_iva1->setCurrentIndex(0);
    ui->cbo_porc_gasto_iva2->setCurrentIndex(0);
    ui->cbo_porc_gasto_iva3->setCurrentIndex(0);

    // --------------------------
    // campo busquedas
    // --------------------------
    h_Buscar["Factura"]="factura";
    h_Buscar["Código cliente"] = "codigo_cliente";
    h_Buscar["Cliente"] = "cliente";
    h_Buscar["Fecha Factura"] = "fecha";
    h_Buscar["CIF / NIF"] = "cif";
    h_Buscar["Total"] = "total";


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
    cboSeries = new QComboBox(this);
    QSqlQueryModel * model_series = new QSqlQueryModel(this);
    model_series->setQuery("select serie from series",Configuracion_global->empresaDB);
    ui->cbo_serie->setModel(model_series);
    QSqlQuery series2(Configuracion_global->empresaDB);
    series2.exec("select serie from series");
    series2_l.append(tr("TODAS"));
    while (series2.next())
        series2_l.append(series2.record().value("serie").toString());
    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));

    VaciarCampos();
    ui->btnEditar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);
    ui->btnAnterior->setEnabled(false);
    ui->btnImprimir->setEnabled(false);
    ui->btnBuscar->setEnabled(true);
    oFactura->id = -1;


    //-------------
    // Eventos
    //-------------
    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;

    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);

    setUpBusqueda();

    BloquearCampos(true);
    ui->tabla_facturas->selectRow(0);
}

frmFacturas::~frmFacturas()
{
    delete ui;
    delete oCliente1;
    delete oFactura;
}

void frmFacturas::LLenarCampos() {
    int lEstado;
    ui->lblFactura->setText(oFactura->factura.replace("\n","").replace("\r",""));
    if(ui->lblFactura->text() == tr("BORRADOR"))
        ui->btnBorrar->setVisible(true);
    else
        ui->btnBorrar->setVisible(false);
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

    for(int i =0;i<Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oFactura->id_pais)
        {
            int ind_pais = ui->comboPais->findText(Configuracion_global->paises_model->record(i).value("pais").toString());
            ui->comboPais->setCurrentIndex(ind_pais);
            break;
        }
        else
        {
            ui->comboPais->setCurrentIndex(-1);
        }
    }

(Configuracion_global->toFormatoMoneda(QString::number(oFactura->dto,'f',Configuracion_global->decimales)));
    //ui->txtdto_pp->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->dto_pp,'f',Configuracion_global->decimales)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->dto,'f',Configuracion_global->decimales)));
    //ui->txtimporte_descuento_pp->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->importe_descuento_pp,'f',Configuracion_global->decimales)));
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number( oFactura->base,'f',Configuracion_global->decimales)));
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number( oFactura->iva,'f',Configuracion_global->decimales)));
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total_recargo,'f',Configuracion_global->decimales)));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva,'f',
                                                                          Configuracion_global->decimales_campos_totales)));
    ui->txtcif->setText(oFactura->cif);
     lEstado = oFactura->recargo_equivalencia;
    if ((lEstado== 1)) {
        ui->chkrecargo_equivalencia->setChecked(true);
    } else {
        ui->chkrecargo_equivalencia->setChecked(false);
    }
    ui->spinporc_irpf->setValue(oFactura->porc_irpf);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda( QString::number(oFactura->subtotal,'f',Configuracion_global->decimales)));
                    //ui->txtdto->setTextglobal->toFormatoMoneda(QString::number(oFactura->iva,'f',Configuracion_global->decimales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total,'f',Configuracion_global->decimales)));

    ui->lblFacturaImpresa->setVisible(oFactura->impreso);
    ui->lblFacturaCobrada->setVisible(oFactura->cobrado);
    ui->btnCobrar->setVisible(!oFactura->cobrado);
    ui->lbcontabilizada->setVisible(oFactura->contablilizada);
    if(Configuracion_global->formapago_model->rowCount() >0)
    {
        for(int i =0;i<Configuracion_global->formapago_model->rowCount();i++)
        {
            if(Configuracion_global->formapago_model->record(i).value("id").toInt() == oFactura->id_forma_pago)
            {
                int ind_fp = ui->cboforma_pago->findText(Configuracion_global->formapago_model->record(i).value(
                                                             "forma_pago").toString());
                ui->cboforma_pago->setCurrentIndex(ind_fp);
                break;
            }
            else
            {
                ui->cboforma_pago->setCurrentIndex(-1);
            }

        }
    }
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
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->rec1,'f',Configuracion_global->decimales)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->rec2,'f',Configuracion_global->decimales)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->rec3,'f',Configuracion_global->decimales)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->rec4,'f',Configuracion_global->decimales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total_recargo,'f',Configuracion_global->decimales)));
    ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->entregado_a_cuenta,'f',Configuracion_global->decimales)));
    ui->txtimporte_pendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->importe_pendiente,'f',Configuracion_global->decimales)));
    ui->txtcodigo_entidad->setText(oFactura->codigo_entidad);
    ui->txtoficina_entidad->setText(oFactura->oficina_entidad);
    ui->txtdc_cuenta->setText(oFactura->dc_cuenta);
    ui->txtnumero_cuenta->setText(oFactura->cuenta_corriente);
    ui->txtpedido_cliente->setText(QString::number(oFactura->pedido_cliente));
    if (oFactura->porc_irpf != 0)
    {
        ui->lblIRPF_3->setVisible(true);
        ui->lblIrpf->setVisible(true);
        ui->spinporc_irpf->setVisible(true);
    } else {
        ui->lblIrpf->setVisible(false);
        ui->lblIRPF_3->setVisible(false);
        ui->spinporc_irpf->setVisible(false);
    }
    ui->lblFacturaCobrada->setVisible(oFactura->cobrado);
    ui->txtimporte_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->irpf,'f',Configuracion_global->decimales)));
    ui->txtimporte_irpf_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->irpf,'f',Configuracion_global->decimales)));
    oCliente1->Recuperar("Select * from clientes where id ="+QString::number(oFactura->id_cliente));
    ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente1->importe_a_cuenta,'f',Configuracion_global->decimales)));
    oFactura->id_cliente = oCliente1->id;
    ui->txtTransportista->setText( Configuracion_global->devolver_transportista(oFactura->id_transportista));
    ui->txtAsiento->setText(QString::number(oFactura->apunte));
    QString error;
    ui->txt_tarifa->setText(SqlCalls::SelectOneField("codigotarifa","descripcion",QString("id=%1").arg(oFactura->tarifa_cliente),
                                                     Configuracion_global->groupDB,error).toString());
    ui->txtGastoDist1->setText(oFactura->desc_gasto1);
    ui->txtGastoDist2->setText(oFactura->desc_gasto2);
    ui->txtGastoDist3->setText(oFactura->desc_gasto3);
    ui->SpinGastoDist1->setValue(oFactura->imp_gasto1);
    ui->SpinGastoDist2->setValue(oFactura->imp_gasto2);
    ui->SpinGastoDist3->setValue(oFactura->imp_gasto3);
    index = ui->cbo_porc_gasto_iva1->findText(QString::number(oFactura->porc_iva_gasto1));
    ui->cbo_porc_gasto_iva1->setCurrentIndex(index);
    index = ui->cbo_porc_gasto_iva2->findText(QString::number(oFactura->porc_iva_gasto2));
    ui->cbo_porc_gasto_iva2->setCurrentIndex(index);
    index = ui->cbo_porc_gasto_iva3->findText(QString::number(oFactura->porc_iva_gasto3));
    ui->cbo_porc_gasto_iva3->setCurrentIndex(index);
    ui->spin_iva_gasto1->setValue(oFactura->iva_gasto1);
    ui->spin_iva_gasto2->setValue(oFactura->iva_gasto2);
    ui->spin_iva_gasto3->setValue(oFactura->iva_gasto3);
    ui->txtbase_total->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->base,'f',
                                                                                     Configuracion_global->decimales_campos_totales)));
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->iva,'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(oFactura->total_recargo,'f',
                                                                                        Configuracion_global->decimales_campos_totales)));
    ui->txttotal_total->setText(Configuracion_global->toFormatoMoneda(QString::number((oFactura->total),'f',
                                                                                      Configuracion_global->decimales_campos_totales)));

    //------------------------------
    // DIRECCION ALTERNATIVA
    //------------------------------
    ui->txtDireccion1_entrega->setText(oFactura->direccion1_entrega);
    ui->txtDireccion2_entrega->setText(oFactura->direccion2_entrega);
    ui->txtCp_entrega->setText(oFactura->cp_entrega);
    ui->txtPoblacion_entrega->setText(oFactura->poblacion_entrega);
    ui->txtProvincia_entrega->setText(oFactura->poblacion_entrega);
    for(int i =0;i<Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oFactura->id_pais)
        {
            int ind_pais = ui->cboPais_entrega->findText(Configuracion_global->paises_model->record(i).value("pais").toString());
            ui->cboPais_entrega->setCurrentIndex(ind_pais);
            break;
        }
        else
        {
            ui->cboPais_entrega->setCurrentIndex(-1);
        }
    }

    //------------------------------
    // CBO DIRECCIONES ALTERNATIVAS
    //------------------------------
    ui->cboDireccionesEntrega->clear();
    QMap <int, QSqlRecord> di;

    di = SqlCalls::SelectRecord("cliente_direcciones",QString("id_cliente = %1").arg(oCliente1->id),Configuracion_global->groupDB,
                                error);

    if(error.isEmpty())
    {
        QMapIterator <int,QSqlRecord> i(di);
        while (i.hasNext()) {
            i.next();
            ui->cboDireccionesEntrega->addItem(i.value().value("descripcion").toString());
        }
    } else
    {
        if(error !="No se encuentra")
            QMessageBox::warning(this,tr("Facturas a clientes"),
                             tr("Ocurrió un error al recuperar las direcciones alternativas:\n %1").arg(error));

    }
    //-------------------------------------- FIN DIRECCIONES ALTERNATIVAS

    //---------------------------
    // DIVISA FACTURA
    //---------------------------
    QString divisa = Configuracion_global->Devolver_moneda(oFactura->id_divisa);
    index = ui->cboDivisa->findText(divisa);
    ui->cboDivisa->setCurrentIndex(index);

    //----------------------------
    // Lineas Facturas
    //----------------------------
    modelLineas->setQuery(QString("select id,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                          "from lin_fac where id_cab = %1;").arg(oFactura->id),Configuracion_global->empresaDB);

}

void frmFacturas::LLenarCamposCliente()
{
    QString divisa,fp;
    oCliente1->Recuperar("select * from clientes where id=" +QString::number(oFactura->id_cliente));
    ui->lblCliente->setText(oCliente1->nombre_fiscal);
    ui->txtcodigo_cliente->setText(oCliente1->codigo_cliente);
    ui->txtcliente->setText(oCliente1->nombre_fiscal);
    ui->txtdireccion1->setText(oCliente1->direccion1);
    ui->txtdireccion2->setText(oCliente1->direccion2);
    ui->txtcp->setText(oCliente1->cp);
    ui->txtpoblacion->setText(oCliente1->poblacion);
    ui->txtprovincia->setText(oCliente1->provincia);
    for(int i =0;i<Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oCliente1->id_pais)
        {
            int ind_pais = ui->comboPais->findText(Configuracion_global->paises_model->record(i).value("pais").toString());
            ui->comboPais->setCurrentIndex(ind_pais);
            break;
        }
    }

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

    QString error;

    oCliente1->Recuperar("Select * from clientes where id ="+QString::number(oFactura->id_cliente));
    ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente1->importe_a_cuenta,'f',Configuracion_global->decimales)));
    ui->txt_tarifa->setText(SqlCalls::SelectOneField("codigotarifa","descripcion",QString("id=%1").arg(oCliente1->idTarifa),
                                                     Configuracion_global->groupDB,error).toString());


    oFactura->id_cliente = oCliente1->id;
    oFactura->id_transportista = oCliente1->id_transportista;
    ui->txtTransportista->setText( Configuracion_global->devolver_transportista(oCliente1->id_transportista));

    for(int i =0;i<Configuracion_global->divisas_model->rowCount();i++)
    {
        if(Configuracion_global->divisas_model->record(i).value("id").toInt() == oCliente1->id_divisa)
        {
            int ind_divisa = ui->cboDivisa->findText(Configuracion_global->divisas_model->record(i).value("moneda").toString());
            ui->cboDivisa->setCurrentIndex(ind_divisa);
            break;
        } else
        {
            ui->cboDivisa->setCurrentIndex(-1);
        }

    }
    for(int i =0;i<Configuracion_global->formapago_model->rowCount();i++)
    {
        if(Configuracion_global->formapago_model->record(i).value("id").toInt() == oCliente1->id_forma_pago)
        {
            int ind_formapago = ui->cboforma_pago->findText(Configuracion_global->formapago_model->record(i).value("forma_pago").toString());
            ui->cboforma_pago->setCurrentIndex(ind_formapago);
            break;
        }
        else {
            ui->cboforma_pago->setCurrentIndex(-1);
        }
    }

    oFactura->id_forma_pago = oCliente1->id_forma_pago;
    oFactura->tarifa_cliente = SqlCalls::SelectOneField("codigotarifa","id",QString("descripcion = '%1'").arg(ui->txt_tarifa->text()),
                                                        Configuracion_global->groupDB,error).toInt();

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
    ui->comboPais->setCurrentIndex(-1);
    ui->txtcif->setText("");
    ui->txtsubtotal->setText(0);
    ui->txtimporte_descuento->setText("0,00");
    //ui->txtimporte_descuento_pp->setText("0,00");
    ui->txtbase->setText("0,00");
    ui->txtiva_total->setText(0);
    ui->txtiva->setText("0,00");
    ui->txttotal->setText("0,00");
    ui->lblFacturaImpresa->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lbcontabilizada->setVisible(false);
    ui->cboforma_pago->setCurrentIndex(0);
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
    ui->txtGastoDist1->setText(tr("Portes"));
    ui->txtGastoDist2->setText("");
    ui->txtGastoDist3->setText("");
    ui->SpinGastoDist1->setValue(0);
    ui->SpinGastoDist2->setValue(0);
    ui->SpinGastoDist3->setValue(0);
    ui->cbo_porc_gasto_iva1->setCurrentIndex(ui->cbo_porc_gasto_iva1->findText(Configuracion_global->ivaList.at(0)));
    ui->cbo_porc_gasto_iva2->setCurrentIndex(ui->cbo_porc_gasto_iva2->findText(Configuracion_global->ivaList.at(0)));
    ui->cbo_porc_gasto_iva3->setCurrentIndex(ui->cbo_porc_gasto_iva3->findText(Configuracion_global->ivaList.at(0)));
    ui->spin_iva_gasto1->setValue(0);
    ui->spin_iva_gasto2->setValue(0);
    ui->spin_iva_gasto3->setValue(0);
    ui->cboDivisa->setCurrentIndex( -1);
    ui->txtbase_total->clear();
    ui->txtiva_total->clear();
    ui->txttotal_recargo->clear();
    ui->txttotal_total->clear();

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
    //helper.blockTable(state);
    ui->txtfactura->setReadOnly(true);
    ui->btnAsignarTransportista->setEnabled(!state);
    ui->btnBuscar->setEnabled(state);
    ui->spin_iva_gasto1->setEnabled(false);
    ui->spin_iva_gasto2->setEnabled(false);
    ui->spin_iva_gasto3->setEnabled(false);

    m_busqueda->block(!state);

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
    oFactura->id_pais =Configuracion_global->Devolver_id_pais(ui->comboPais->currentText());
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
    oFactura->porc_dto = (ui->spinPorc_dto->value());
    oFactura->dto = (ui->txtimporte_descuento->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble());
    oFactura->porc_dto_pp = (ui->spinPorc_dto_pp->value());
    oFactura->dto = Configuracion_global->MonedatoDouble(ui->txtimporte_descuento->text());
    oFactura->dto_pp = Configuracion_global->MonedatoDouble(ui->txtDtoPP->text());
    oFactura->base = (ui->txtbase->text().replace(".","").replace(",",".").replace(moneda,"").toDouble());
    oFactura->iva = (ui->txtiva->text().replace(".","").replace(",",".").replace(moneda,"").toDouble());
    oFactura->total = (ui->txttotal->text().replace(".","").replace(",",".").replace(moneda,"").toDouble());
    oFactura->forma_pago = (ui->cboforma_pago->currentText());
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
    oFactura->pedido_cliente = ui->txtpedido_cliente->text().toInt();
    oFactura->irpf = (ui->txtimporte_irpf->text().replace(".","").replace(",",".").toDouble());
    oFactura->desc_gasto1 = ui->txtGastoDist1->text();
    oFactura->desc_gasto2 = ui->txtGastoDist2->text();
    oFactura->desc_gasto3 = ui->txtGastoDist3->text();
    oFactura->imp_gasto1 = ui->SpinGastoDist1->value();
    oFactura->imp_gasto2 = ui->SpinGastoDist2->value();
    oFactura->imp_gasto3 = ui->SpinGastoDist3->value();
    oFactura->porc_iva_gasto1 = ui->cbo_porc_gasto_iva1->currentText().toFloat();
    oFactura->porc_iva_gasto2 = ui->cbo_porc_gasto_iva2->currentText().toFloat();
    oFactura->porc_iva_gasto3 = ui->cbo_porc_gasto_iva1->currentText().toFloat();
    oFactura->iva_gasto1 = ui->spin_iva_gasto1->value();
    oFactura->iva_gasto2 = ui->spin_iva_gasto2->value();
    oFactura->iva_gasto3 = ui->spin_iva_gasto3->value();
    oFactura->id_divisa  = Configuracion_global->Devolver_id_moneda(ui->cboDivisa->currentText());
    QString error;
    oFactura->tarifa_cliente = SqlCalls::SelectOneField("codigotarifa","id",QString("descripcion = '%1'").arg(ui->txt_tarifa->text()),
                                                        Configuracion_global->groupDB,error).toInt();


}

void frmFacturas::on_btnSiguiente_clicked()
{
    QString id = QString::number(oFactura->id);
    QString cSQL;
    cSQL ="select * from cab_fac "
                " where factura > '"+oFactura->factura+"' and ejercicio = "+Configuracion_global->cEjercicio+
                             " order by factura limit 0,1";

    if(oFactura->RecuperarFactura(cSQL))
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oFactura->id);
        //helper.fillTable("empresa","lin_fac",filter);
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
    cSQL ="select * from cab_fac "
                " where factura <'"+oFactura->factura+"' and ejercicio = "+Configuracion_global->cEjercicio+
                             " order by factura desc limit 0,1";
    if(oFactura->RecuperarFactura(cSQL))
    {
        LLenarCampos();
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

void frmFacturas::on_btnAnadir_clicked()
{
    m_busqueda->hideMe();
    //ui->btnGuardar->setMenu(menu_guardar);
    BloquearCampos(false);
    in_edit = false;
    VaciarCampos();    
    ui->chkrecargo_equivalencia->setCheckState(Qt::Unchecked);
    ui->btnCobrar->setVisible(true);
    ui->txtcodigo_cliente->setFocus();
    oFactura->AnadirFactura();
    //helper.set_tarifa(Configuracion_global->id_tarifa_predeterminada);
    ui->stackedWidget->setCurrentIndex(0);
    //helper.resizeTable();
    ui->spinporc_irpf->setValue(oFactura->porc_irpf);
    modelLineas->setQuery(QString("select id,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                              "from lin_fac where id_cab = %1;").arg(oFactura->id),Configuracion_global->empresaDB);
    calcular_factura();
    ui->Lineas->setFocus();
    emit block();
    ui->txtGastoDist1->setText(tr("Portes:"));
    ui->stackedWidget->setCurrentIndex(0);
    ui->tabWidget_2->setCurrentIndex(0);
}


void frmFacturas::on_botBuscarCliente_clicked()
{
    blockSignals(true);
    oCliente1->clear();
    db_consulta_view consulta(this);
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
    if(ui->stackedWidget->currentIndex()==1){
        QModelIndex index = ui->tabla_facturas->currentIndex();
       oFactura->id = ui->tabla_facturas->model()->data(ui->tabla_facturas->model()->index(index.row(),0),Qt::EditRole).toInt();
       oFactura->RecuperarFactura(oFactura->id);
    }
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
            QMap <QString,QString> parametros_sql;
            parametros_sql["Empresa.cab_fac"] = QString("id = %1").arg(oFactura->id);
            parametros_sql["Empresa.lin_fac"] = QString("id_cab = %1").arg(oFactura->id);
            QString report = "factura_"+QString::number(oCliente1->ididioma);


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

void frmFacturas::mostrar_factura()
{
    on_tabla_facturas_doubleClicked(ui->tabla_facturas->currentIndex());
}

void frmFacturas::refrescar_modelo()
{
    modelLineas->setQuery(QString("select id,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                              "from lin_fac where id_cab = %1;").arg(oFactura->id),Configuracion_global->empresaDB);
    calcular_factura();
}



void frmFacturas::on_btnEditar_clicked()
{
    if (oFactura->editable)
    {
        BloquearCampos(false);
        m_busqueda->hideMe();
        in_edit = true;
        emit block();
        // ----------------------------
        // Guardo datos para acumulados
        //-----------------------------
        acumulados["id_cliente"] = oFactura->id_cliente;
        acumulados["total"] = oFactura->total;
        Configuracion_global->empresaDB.transaction();
        Configuracion_global->groupDB.transaction();
        ui->txtcodigo_cliente->setFocus();
        ui->stackedWidget->setCurrentIndex(0);
    } else
        QMessageBox::warning(this,tr("Facturas a clientes"),tr("No se puede editar la factura"),tr("Aceptar"));

}

void frmFacturas::desbloquear_factura()
{
 if(QMessageBox::question(this,tr("Gestion de facturas"),tr("¿Desbloquear factura?"),
                          tr("No"),tr("Si")) == QMessageBox::Accepted)
 {
     QModelIndex Model = ui->tabla_facturas->currentIndex();
     int id_fact = ui->tabla_facturas->model()->data(ui->tabla_facturas->model()->index(Model.row(),0)).toInt();
     QHash <QString, QVariant> h;
     h["editable"] = 1;
     QString error;
     bool success = SqlCalls::SqlUpdate(h,"cab_fac",Configuracion_global->empresaDB,QString("id=%1").arg(id_fact),error);
     if(!success)
         QMessageBox::warning(this,tr("Edicion de facturas"),tr("No se pudo desbloquear: %1").arg(error),
                              tr("Aceptar"));

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
        QHash <QString, QVariant> lin_fac;
        QString error;
        lin_fac["id_cab"] =  oFactura->id;
        lin_fac["id_articulo"] =  queryArticulos.record().value("id").toInt();
        lin_fac["codigo"] = ld->codigo;
        lin_fac["descripcion"] = ld->descripcion;
        lin_fac["cantidad"] = ld->cantidad;
        lin_fac["precio"] = ld->precio;
        lin_fac["subtotal"] = ld->subtotal;
        lin_fac["porc_dto"] = ld->dto_perc;
        lin_fac["porc_iva"] = ld->iva_perc;
        double base = ld->subtotal - (ld->subtotal*ld->dto);
        lin_fac["iva"] = base * ld->iva_perc/100;
        lin_fac["porc_rec"] = ld->rec_perc;
        lin_fac["rec"] = base *ld->rec_perc/100;
        lin_fac["dto"] = ld->dto;
        lin_fac["total"] = ld->total;
        lin_fac["promocion"] = SqlCalls::SelectOneField("articulos","articulo_promocionado",
                                                        QString("id = %1").arg(lin_fac.value("id_articulo").toInt()),
                                                        Configuracion_global->groupDB,error);
        int new_id = SqlCalls::SqlInsert(lin_fac,"lin_fac",Configuracion_global->empresaDB,error);

        if (new_id ==-1){
            ok_empresa = false;
            QMessageBox::warning(this,tr("Gestión de albaranes"),
                                 tr("Ocurrió un error al insertar la nueva línea: %1").arg(error),
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

        ld->idLinea = new_id;

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
    //helper.resizeTable();
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
    if(Configuracion_global->contabilidad)
        Configuracion_global->contaDB.rollback();
    oFactura->RecuperarFactura(oFactura->id);
    LLenarCampos();
    BloquearCampos(true);
    emit unblock();

}

void frmFacturas::on_txtcodigo_cliente_editingFinished()
{
    if(ui->txtcodigo_cliente->text() != oFactura->codigo_cliente)
    {
        oCliente1->decrementar_acumulados(oCliente1->id,oFactura->total,QDate::currentDate());
    }
    if((ui->txtcodigo_cliente->text() != oFactura->codigo_cliente)&& !ui->txtcodigo_cliente->text().isEmpty()){
        if(ui->txtcodigo_cliente->text().trimmed().size() < Configuracion_global->digitos_cuentas_contables)
        {
            QString ccod= Configuracion_global->cuenta_clientes;
            QString ccli = ui->txtcodigo_cliente->text().trimmed();
            int tam = Configuracion_global->digitos_cuentas_contables - (ccod.size() + ccli.size());
            QString zeros;
            for(int i = 0; i<tam;i++)
            {
                zeros.append("0");
            }
            ui->txtcodigo_cliente->setText(ccod+zeros+ccli);

        }
        oCliente1->Recuperar("select * from clientes where codigo_cliente='"+ui->txtcodigo_cliente->text()+"'");
        oFactura->id_cliente = oCliente1->id;
        LLenarCamposCliente();
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
        //helper.resizeTable();
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
        ui->stackedWidget->setCurrentIndex(1);
}

void frmFacturas::on_cboVer_currentTextChanged(const QString &arg1)
{
    /*
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
    formato_tabla_facturas(*m_facturas);*/
}


void frmFacturas::formato_tabla_facturas(QSqlQueryModel &modelo)
{
    ui->tabla_facturas->setItemDelegateForColumn(6, new MonetaryDelegate(this));
    ui->tabla_facturas->setItemDelegateForColumn(3,new DateDelegate(this));
    ui->tabla_facturas->setItemDelegateForColumn(4, new DateDelegate(this));
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

void frmFacturas::filter_table(QString texto, QString orden, QString modo)
{
    this->texto = texto;
    this->orden = orden;
    this->modo = modo;
    this->serie = cboSeries->currentText();

    ui->stackedWidget->setCurrentIndex(1);
    QString indice_tabla = h_Buscar.value(orden);
    if(indice_tabla.isEmpty())
        indice_tabla = "factura";

    if (modo == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    QString order;
    if(indice_tabla == "factura")
        order = "fecha+serie+factura";
    else
        order = indice_tabla;


    if(cboSeries->currentText() == tr("TODAS"))
    {
        m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where "+indice_tabla+
                                 " like '%"+texto+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " order by "+order +" "+modo,Configuracion_global->empresaDB);
    } else
    {
        m_facturas->setQuery("select id,serie,factura,fecha,fecha_cobro,cif,total,cliente from cab_fac "
                                 " where  "+indice_tabla+
                                 " like '%"+texto+"%'  and ejercicio = "+Configuracion_global->cEjercicio+
                                 " and serie ='"+this->serie +"'"
                                 " order by "+order +" "+modo,Configuracion_global->empresaDB);

    }
    //qDebug() << m_facturas->query().lastQuery();



}

bool frmFacturas::eventFilter(QObject *obj, QEvent *event)
{

    if (event->type() == QEvent::KeyPress) {

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj == ui->Lineas){
            if (keyEvent->key() == Qt::Key_Plus)
            {
                on_btnAnadirLinea_clicked();
            }
            if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
                on_Lineas_doubleClicked(ui->Lineas->currentIndex());

        }
        if(obj == ui->txtcodigo_cliente)
        {
            if(keyEvent->key() == Qt::Key_F1 && ui->btnGuardar->isEnabled())
                on_botBuscarCliente_clicked();
        }
        if(obj == ui->txtcp)
        {
            if(keyEvent->key() == Qt::Key_F1 && ui->btnGuardar->isEnabled())
                buscar_poblacion(1);
        }
        if(obj == ui->txtCp_entrega)
        {
            if(keyEvent->key() == Qt::Key_F1 && ui->btnGuardar->isEnabled())
                buscar_poblacion(2);
        }

        if(keyEvent->key() == Qt::Key_F1)
        {
            if(ui->btnEditar->isEnabled()){
                if(m_busqueda->isShow())
                    ocultarBusqueda();
                else
                    mostrarBusqueda();
            }
            return true;
        }
        if (obj == ui->tabla_facturas)
            if(keyEvent->key() == Qt::Key_Return)
                on_tabla_facturas_doubleClicked(ui->tabla_facturas->currentIndex());
        if(keyEvent->key() == Qt::Key_Escape)
            return true;
    }
    if(event->type() == QEvent::Resize)
        _resizeBarraBusqueda(m_busqueda);
    return MayaModule::eventFilter(obj,event);
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

void frmFacturas::calcular_factura()
{
    double subtotal,dto,dtopp,base,iva,re,total;
    double base1,iva1,re1,total1;
    double base2,iva2,re2,total2;
    double base3,iva3,re3,total3;
    double base4,iva4,re4,total4;
    base1= 0; base2 = 0; base3 =0;base4 = 0;
    iva1=0; iva2 =0; iva3=0; iva4 = 0;
    re1=0;re2=0;re3=0;re4 =0;
    total1 =0; total2 =0;total3=0; total4=0;
    subtotal =0; dto=0; dtopp =0; base =0; iva =0; re =0; total =0;
    QMap <int,QSqlRecord> l;
    QString error;
    l = SqlCalls::SelectRecord("lin_fac",QString("id_cab=%1").arg(oFactura->id),Configuracion_global->empresaDB,error);
    QMapIterator <int,QSqlRecord> li(l);
    while(li.hasNext())
    {
        li.next();
        subtotal += li.value().value("subtotal").toDouble();
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva1->text().toFloat())
        {
            // base1
            base1 += li.value().value("total").toDouble();
            iva1 = base1*(li.value().value("porc_iva").toFloat()/100);
            re1 = base1*(li.value().value("porc_re").toFloat()/100);
            total1 = base1 + iva1 + re1;
        }
        // base2
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva2->text().toFloat())
        {
            base2 += li.value().value("total").toDouble();
            iva2 = base2*(li.value().value("porc_iva").toFloat()/100);
            re1 = base2*(li.value().value("porc_re").toFloat()/100);
            total2 = base2 + iva2 + re2;

        }
        // base3
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva3->text().toFloat())
        {
            base3 += li.value().value("total").toDouble();
            iva3 = base3*(li.value().value("porc_iva").toFloat()/100);
            re3 = base3*(li.value().value("porc_re").toFloat()/100);
            total3 = base3 + iva3 + re3;

        }
        // base4
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva4->text().toFloat())
        {
            base4 += li.value().value("total").toDouble();
            iva4 = base4*(li.value().value("porc_iva").toFloat()/100);
            re4 = base4*(li.value().value("porc_re").toFloat()/100);
            total4 = base4 + iva4 + re4;
        }

    }
    // añadir gastos extras
    if(ui->cbo_porc_gasto_iva1->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist1->value();
    if(ui->cbo_porc_gasto_iva2->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist2->value();
    if(ui->cbo_porc_gasto_iva3->currentText().toFloat()== ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist3->value();
    iva1 = base1 * (ui->txtporc_iva1->text().toFloat()/100);
    if(ui->chkrecargo_equivalencia)
        re1 = base1 * (ui->txtrec1->text().toFloat()/100);

    if(ui->cbo_porc_gasto_iva1->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist1->value();
    if(ui->cbo_porc_gasto_iva2->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist2->value();
    if(ui->cbo_porc_gasto_iva3->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist3->value();
    iva2 = base2 * (ui->txtporc_iva2->text().toFloat()/100);
    if(ui->chkrecargo_equivalencia)
        re2 = base2 * (ui->txtrec1->text().toFloat()/100);

    if(ui->cbo_porc_gasto_iva1->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist1->value();
    if(ui->cbo_porc_gasto_iva2->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist2->value();
    if(ui->cbo_porc_gasto_iva3->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist3->value();
    iva3 = base3 * (ui->txtporc_iva3->text().toFloat()/100);
    if(ui->chkrecargo_equivalencia)
        re3 = base3 * (ui->txtrec3->text().toFloat()/100);

    if(ui->cbo_porc_gasto_iva1->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist1->value();
    if(ui->cbo_porc_gasto_iva2->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist2->value();
    if(ui->cbo_porc_gasto_iva3->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist3->value();
    iva4 = base4 * (ui->txtporc_iva4->text().toFloat()/100);
    if(ui->chkrecargo_equivalencia)
        re4 = base4 * (ui->txtrec4->text().toFloat()/100);

    // TOTALES PARCIALES
    total1 = base1+iva1+re1;
    total2 = base2+iva2+re2;
    total3 = base3+iva3+re3;
    total4 = base4+iva4+re4;

    // TOTALES GENERALES

    if(l.size() >0)
        dto += li.value().value("dto").toDouble();
    base = base1 + base2+base3+base4;

    dtopp = subtotal *(ui->spinPorc_dto_pp->value()/100.0);
    base -= dtopp;
    ui->txtDtoPP->setText(Configuracion_global->toFormatoMoneda(QString::number(dtopp,'f',
                                                Configuracion_global->decimales_campos_totales)));


    // Desglose llenar controles
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base1,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva1,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re1,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total1,'f',
                                                                 Configuracion_global->decimales_campos_totales)));

    // Desglose llenar controles
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base2,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva2,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re2,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total2,'f',
                                                                 Configuracion_global->decimales_campos_totales)));
    // Desglose llenar controles
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base3,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva3,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re3,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total3,'f',
                                                                 Configuracion_global->decimales_campos_totales)));
    // Desglose llenar controles
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base4,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva4,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re4,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total4,'f',
                                                                 Configuracion_global->decimales_campos_totales)));

    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(dtopp,Configuracion_global->decimales_campos_totales)));
    iva = iva1 + iva2 +iva3+iva4;
    re  = re1 +re2 + re3 + re4;
    total = base + iva +re;

    double irpf = base * (ui->spinporc_irpf->value()/100.0);
    ui->txtimporte_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(irpf,'f',2)));
    ui->txtimporte_irpf_2->setText(Configuracion_global->toFormatoMoneda(QString::number(irpf,'f',2)));
    this->moneda = moneda;
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));

    ui->txtbase_total->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_total->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));

}

void frmFacturas::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);
    this->installEventFilter(this);

    QStringList orden;
    orden  <<  tr("Factura") << tr("Código cliente") << tr("Cliente") << tr("Fecha Factura") << tr("CIF/NIF") << tr("Total");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(m_busqueda,SIGNAL(hideMe()),this,SLOT(ocultarBusqueda()));
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));


    QLabel *lblserie = new QLabel(tr("Serie:"));
    cboSeries->addItems(series2_l);
    QHBoxLayout *layoutserie = new QHBoxLayout;
    layoutserie->addWidget(lblserie);
    layoutserie->addWidget(cboSeries);
    m_busqueda->addLayoutZ1(layoutserie);
    connect(cboSeries,SIGNAL(currentIndexChanged(QString)),this,SLOT(on_cboseries_currentIndexChanged(QString)));

    QPushButton* open = new QPushButton(QIcon(":/Icons/PNG/Factura.png"),tr("Mostrar"),this);
    connect(open,SIGNAL(clicked()),this,SLOT(mostrar_factura()));
    m_busqueda->addWidget(open);

    QPushButton* unlock = new QPushButton(QIcon(":/Icons/PNG/abrecaja.png"),tr("Desbloquear"),this);
    connect(unlock,SIGNAL(clicked()),this,SLOT(desbloquear_factura()));
    m_busqueda->addWidget(unlock);

    QPushButton* add = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir"),this);
    connect(add,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(add);

//    QPushButton* edit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar"),this);
//    connect(edit,SIGNAL(clicked()),this,SLOT(on_btnEditar_clicked()));
//    m_busqueda->addWidget(edit);

    QPushButton* print = new QPushButton(QIcon(":/Icons/PNG/print2.png"),tr("Imprimir"),this);
    connect(print,SIGNAL(clicked()),this,SLOT(on_btnImprimir_clicked()));
    m_busqueda->addWidget(print);

    QPushButton* del = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    connect(del,SIGNAL(clicked()),this,SLOT(on_btnBorrar_clicked()));
    m_busqueda->addWidget(del);

    connect(m_busqueda,SIGNAL(key_Down_Pressed()), ui->tabla_facturas,SLOT(setFocus()));
    connect(m_busqueda,SIGNAL(key_F2_Pressed()),this,SLOT(ocultarBusqueda()));
}

void frmFacturas::on_btnAsignarTransportista_clicked()
{
    db_consulta_view consulta;
    QStringList campos;
    campos <<"codigo" <<"transportista" << "telefono1" <<"movil" << "poblacion"<< "provincia" << "pais";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("transportistas");
    consulta.set_db("Group");
    consulta.set_SQL("select id,codigo,transportista from transportista");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Código") << tr("Transportista");

    tamanos <<"0" << "100" << "250" ;
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
     QString filter = QString("id_Cab = '%1'").arg(oFactura->id);
     //helper.fillTable("empresa","lin_fac",filter);
     ui->stackedWidget->setCurrentIndex(0);
     ui->btnEditar->setEnabled(true);
     ui->btnImprimir->setEnabled(true);
     ocultarBusqueda();
}

void frmFacturas::on_tabla_facturas_clicked(const QModelIndex &index)
{
//    int id = ui->tabla_facturas->model()->data(ui->tabla_facturas->model()->index(index.row(),0),Qt::EditRole).toInt();
//    oFactura->RecuperarFactura(id);
//    //LLenarCampos();

}


void frmFacturas::on_btnArticulos_clicked()
{
    FrmArticulos artic;
    artic.exec();
}



void frmFacturas::on_cboseries_currentIndexChanged(const QString &arg1)
{
    this->serie = arg1;
    filter_table(this->texto,this->orden,this->modo);
}


void frmFacturas::on_cboBuscar_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

}

void frmFacturas::on_cboModo_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
}

void frmFacturas::on_btnBorrar_clicked()
{
    oFactura->id = ui->tabla_facturas->model()->data(ui->tabla_facturas->model()->index(
                                                         ui->tabla_facturas->currentIndex().row(),0)).toInt();

    oFactura->borrar(oFactura->id);
    oFactura->clear();
    LLenarCampos();
    filter_table(this->texto,this->orden,this->modo);

}

void frmFacturas::on_spinPorc_dto_editingFinished()
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

    clauses << QString("id_cab = %1").arg(oFactura->id);

    rec = SqlCalls::SelectRecord("lin_fac",clauses,Configuracion_global->empresaDB,error);
    QMapIterator <int, QSqlRecord> i(rec);
    while (i.hasNext())
    {
        i.next();
        id_lin = i.value().value("id").toInt();
        id_art = i.value().value("id_articulo").toInt();
        subtotal = i.value().value("subtotal").toDouble();
        porc_dto_lin = i.value().value("porc_dto").toFloat();

        porc_dto_esp = oArt.asigna_dto_linea(id_art,oCliente1->id,ui->spinPorc_dto->value(),porc_dto_lin);
        QHash <QString,QVariant> f;
        clauses.clear();
        clauses << QString("id = %1").arg(id_lin);
        f["porc_dto"] = porc_dto_esp;
        f["dto"] = subtotal *(porc_dto_esp/100.0);

        bool upd = SqlCalls::SqlUpdate(f,"lin_fac",Configuracion_global->empresaDB,clauses,error);
        if(!upd)
            QMessageBox::warning(this,tr("Cambio DTO"),tr("No se pudo realizar el cambio en las líneas, error:%1").arg(error),
                                 tr("Aceptar"));
        //else
           // helper.calculatotal();
    }
    //--------------------------
    // Calculo dto total
    //--------------------------
    double dto = (Configuracion_global->MonedatoDouble(ui->txtsubtotal->text())*(ui->spinPorc_dto->value()/100.0));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales_campos_totales)));
    oFactura->dto = dto;
    QString filter = QString("id_cab = '%1'").arg(oFactura->id);
    //helper.fillTable("empresa","lin_fac",filter);
}

void frmFacturas::on_spinPorc_dto_pp_editingFinished()
{
    float dto_pp;
    dto_pp = (Configuracion_global->MonedatoDouble(ui->txtsubtotal->text())*(ui->spinPorc_dto_pp->value()/100.0));
    ui->txtDtoPP->setText(Configuracion_global->toFormatoMoneda(QString::number(dto_pp,'f',Configuracion_global->decimales_campos_totales)));
    oFactura->dto_pp = dto_pp;
    QString filter = QString("id_cab = '%1'").arg(oFactura->id);
    //helper.fillTable("empresa","lin_fac",filter);
}

void frmFacturas::on_btnCobrar_clicked()
{
    frmGestionCobros gc(this);
    gc.buscar_deuda(oCliente1->id);
    gc.setId_factura(oFactura->id);
    gc.setOcultarBoton_cerrar(false);

    gc.titulo(QString("Pagos pendientes de: %1").arg(oFactura->cliente));
    gc.exec();
    QString error;
    oFactura->cobrado = SqlCalls::SelectOneField("cab_fac","cobrado",QString("id=%1").arg(oFactura->id),
                                                 Configuracion_global->empresaDB,error).toBool();
    ui->btnCobrar->setVisible(!oFactura->cobrado);
    ui->lblFacturaCobrada->setVisible(oFactura->cobrado);
}

void frmFacturas::on_btnGuardar_clicked()
{
    bool succes = true;

    LLenarFactura();
    if(oFactura->factura.isEmpty())
        oFactura->factura = oFactura->NuevoNumeroFactura(ui->cbo_serie->currentText());

    //------------------------
    // Acumulados clientes
    //------------------------
    double diferencia_total = oFactura->total - acumulados.value("total").toDouble();
    if(acumulados["id_cliente"].toInt() != oFactura->id_cliente)
    {
        oCliente1->decrementar_acumulados(acumulados.value("id_cliente").toInt(),acumulados.value("total").toDouble(),
                                          QDate::currentDate());
        succes = oCliente1->incrementar_acumulados(oFactura->id_cliente,acumulados.value("total").toDouble(),QDate::currentDate());
    } else
        succes = oCliente1->incrementar_acumulados(oFactura->id_cliente,diferencia_total,QDate::currentDate());

    if(succes)
    {
        succes = oFactura->GuardarFactura(oFactura->id,false);
        if(succes)
        {
            LLenarCampos();
            if(Configuracion_global->contabilidad && oFactura->apunte == 0)
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


            TimedMessageBox * t = new TimedMessageBox(this,tr("Factura guardada con éxito"));

            // -------------------------------------------------
            // Borrar vencimientos si existen para no duplicar
            //--------------------------------------------------
            QString clausula = QString("id_factura = %1").arg(oFactura->id);
            QString error;
            SqlCalls::SqlDelete("clientes_deuda",Configuracion_global->groupDB,clausula,error);
            //----------------------
            // Crear vencimientos
            //----------------------
            vencimientos oVto(this);
            oVto.calcular_vencimiento(oFactura->fecha,oFactura->id_cliente,0,oFactura->id,oFactura->serie+"/"+oFactura->factura,1,
                                      "c",oFactura->total);
            BloquearCampos(true);
            emit unblock();
            if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.commit();
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        }
        else
        {
            QMessageBox::critical(this,tr("Error"),
                                  tr("Error al guardar la factura.\n")+Configuracion_global->empresaDB.lastError().text(),
                                  tr("Aceptar"));
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
            if(Configuracion_global->contabilidad)
                Configuracion_global->contaDB.rollback();
        }
    }


}


void frmFacturas::mostrarBusqueda()
{
     _showBarraBusqueda(m_busqueda);
     ui->stackedWidget->setCurrentIndex(1);
     m_busqueda->doFocustoText();
}

void frmFacturas::ocultarBusqueda()
{
     _hideBarraBusqueda(m_busqueda);
}

void frmFacturas::on_cboDireccionesEntrega_currentIndexChanged(const QString &arg1)
{
    QMap <int,QSqlRecord> di;
    QString error;
    di = SqlCalls::SelectRecord("cliente_direcciones",QString("descripcion ='%1'").arg(arg1),
                                Configuracion_global->groupDB,error);
    if(!error.isEmpty())
    {
        QMessageBox::warning(this,tr("Facturas a clientes"),tr("No se puede cargar la direccion alternativa:%1").arg(error),
                             tr("Aceptar"));
    } else
    {
        QMapIterator <int,QSqlRecord> i(di);
        while (i.hasNext()) {
            i.next();
            ui->txtDireccion1_entrega->setText(i.value().value("direccion1").toString());
            ui->txtDireccion2_entrega->setText(i.value().value("direccion2").toString());
            ui->txtCp_entrega->setText(i.value().value("cp").toString());
            ui->txtPoblacion_entrega->setText(i.value().value("poblacion").toString());
            ui->txtProvincia_entrega->setText(i.value().value("provincia").toString());
            int index = ui->cboPais_entrega->findText(i.value().value("pais").toString());
            ui->cboPais_entrega->setCurrentIndex(index);
        }

    }
}



void frmFacturas::on_SpinGastoDist1_editingFinished()
{
    double importe_iva = ui->SpinGastoDist1->value() *(ui->cbo_porc_gasto_iva1->currentText().toFloat()/100);
    ui->spin_iva_gasto1->setValue(importe_iva);
    calcular_factura();
    //QString filter = QString("id_cab = '%1'").arg(oFactura->id);

    //helper.fillTable("empresa","lin_fac",filter);
}

void frmFacturas::on_spin_porc_iva_gasto1_editingFinished()
{
    double importe_iva = ui->SpinGastoDist1->value() *(ui->cbo_porc_gasto_iva1->currentText().toFloat()/100);
    ui->spin_iva_gasto1->setValue(importe_iva);
    calcular_factura();
}

void frmFacturas::on_SpinGastoDist2_editingFinished()
{
    double importe_iva = ui->SpinGastoDist2->value() *(ui->cbo_porc_gasto_iva2->currentText().toFloat()/100);
    ui->spin_iva_gasto2->setValue(importe_iva);
    calcular_factura();
}

void frmFacturas::on_spin_porc_iva_gasto2_editingFinished()
{
    double importe_iva = ui->SpinGastoDist2->value() *(ui->cbo_porc_gasto_iva2->currentText().toFloat()/100);
    ui->spin_iva_gasto2->setValue(importe_iva);
    calcular_factura();

}

void frmFacturas::on_SpinGastoDist3_editingFinished()
{
    double importe_iva = ui->SpinGastoDist3->value() *(ui->cbo_porc_gasto_iva3->currentText().toFloat()/100);
    ui->spin_iva_gasto3->setValue(importe_iva);
    calcular_factura();
}

void frmFacturas::on_spin_porc_iva_gasto3_editingFinished()
{
    double importe_iva = ui->SpinGastoDist3->value() *(ui->cbo_porc_gasto_iva3->currentText().toFloat()/100);
    ui->spin_iva_gasto3->setValue(importe_iva);
    calcular_factura();
}

void frmFacturas::on_btnAnadirLinea_clicked()
{
    if(ui->btnGuardar->isEnabled())
    {
            frmEditLine frmeditar(this);
            connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(refrescar_modelo()));
            frmeditar.set_linea(0,"lin_fac");
            frmeditar.set_tabla("lin_fac");
            frmeditar.set_id_cliente(oCliente1->id);
            frmeditar.set_id_tarifa(oFactura->tarifa_cliente);
            frmeditar.set_id_cab(oFactura->id);
            frmeditar.set_tipo("V");
            if(!frmeditar.exec() == QDialog::Accepted)
                modelLineas->setQuery(QString("select id,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                                      "from lin_fac where id_cab = %1;").arg(oFactura->id),Configuracion_global->empresaDB);
                calcular_factura();

    } else{
        QMessageBox::warning(this,tr("Gestión de facturas"),tr("Debe editar la factura para añadir líneas"),
                             tr("Aceptar"));
    }
}

void frmFacturas::on_Lineas_doubleClicked(const QModelIndex &index)
{
    if(ui->btnGuardar->isEnabled())
    {
        int id_lin = ui->Lineas->model()->data(index.model()->index(index.row(),0)).toInt();
        if(id_lin >0)
        {

            frmEditLine frmeditar(this);
            connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(refrescar_modelo()));
            frmeditar.set_id_cliente(oCliente1->id);
            frmeditar.set_id_tarifa(oFactura->tarifa_cliente);
            frmeditar.set_id_cab(oFactura->id);
            frmeditar.set_tipo("V");
            frmeditar.set_linea(id_lin,"lin_fac");
            frmeditar.set_tabla("lin_fac");
            frmeditar.set_editando();
            frmeditar.exec();

            //refrescar_modelo();
            calcular_factura();
            ui->Lineas->setFocus();

        } else
        {
            QMessageBox::warning(this, tr("Gestión de facturas"),tr("Debe editar la factura para poder modificar las líneas"),
                                 tr("Aceptar"));

        }
    }
}


void frmFacturas::on_btn_borrarLinea_clicked()
{
    if(QMessageBox::question(this,tr("Lineas de facturas"), tr("¿Borrar la linea?"),
                             tr("No"),tr("Borrar")) == QMessageBox::Accepted)
    {
        QModelIndex index = ui->Lineas->currentIndex();
        int id_lin = ui->Lineas->model()->index(index.row(),0).data().toInt();
        oFactura->BorrarLineasFactura(id_lin);
        modelLineas->setQuery(QString("select id,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                                      "from lin_fac where id_cab = %1;").arg(oFactura->id),Configuracion_global->empresaDB);
        calcular_factura();
        ui->Lineas->setFocus();

    }
}
