#include "frmClientes.h"
#include "ui_frmClientes.h"
#include "cliente.h"
#include "../columnafecha.h"
#include "frmfichapaciente.h"
#include "../sqlcalls.h"
#include "paciente.h"
#include "frmaddtipocliente.h"
#include "frmpersonascontactocliente.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/datedelegate.h"
#include "../Almacen/frmexcepciones.h"
#include "../Zona_Ventas/frmgestioncobros2.h"

frmClientes::frmClientes(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmClientes),
    menuButton(QIcon(":/Icons/PNG/clientes_2.png"),tr("Clientes"),this),
      push(new QPushButton(QIcon(":/Icons/PNG/clientes_2.png"),"",this))
{

    //CONFIG SHORTCUT
    if (Configuracion_global->medic){
        push->setToolTip(tr("Gestión del fichero de pacientes/clientes"));
        menuButton.setText("Gestión\nPacientes");
    }
    else{
        push->setToolTip(tr("Gestión del fichero de clientes"));
        menuButton.setText("Gestión\nClientes");
    }
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    __init = false;
    this->installEventFilter(this);
    this->Altas = false;
}

void frmClientes::init_querys()
{
    if(!__init)
        return;
    qTarifa->setQuery("select id,descripcion from codigotarifa",Configuracion_global->groupDB);
    m_clientes->setQuery("select id, codigo_cliente,nombre_fiscal,cif_nif, direccion1,poblacion,telefono1,movil,email from clientes",Configuracion_global->groupDB);
    queryTransportistas->setQuery("Select id,transportista from transportista",Configuracion_global->groupDB);
    queryAgentes->setQuery("Select id,nombre from agentes",Configuracion_global->groupDB);
    qTarifa->setQuery("select id,descripcion from codigotarifa",Configuracion_global->groupDB);
    qmidiomas->setQuery("select id,idioma from idiomas order by idioma", Configuracion_global->groupDB);


    qModeldireccion->setQuery("select * from cliente_direcciones where id_cliente = "+QString::number(oCliente->id),Configuracion_global->groupDB);
    ui->lista_direccionesAlternativas->setModelColumn(1);

    ui->cbotransportista   ->setModel(queryTransportistas);
    ui->cboagente          ->setModel(queryAgentes);
    ui->cbotarifa_cliente  ->setModel(qTarifa);
    ui->cboidiomaDocumentos->setModel(qmidiomas);

    ui->cbotransportista->setModelColumn(1);
    ui->cboagente->setModelColumn(1);
    ui->cbotarifa_cliente ->setModelColumn(1);
    ui->cboidiomaDocumentos->setModelColumn(1);

    ui->tabWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(1);
}

void frmClientes::init()
{
    if(__init)
        return;
    ui->setupUi(this);
    if (Configuracion_global->medic)
        ui->textoTitulo->setText(tr("Gestión de Pacientes - datos administrativos"));
    else
        ui->textoTitulo->setText(tr("Gestión de Clientes"));

    ui->stackedWidget->setCurrentIndex(1);
    oCliente = new Cliente(this);
     //ALLOCS MODELS
    m_clientes          = new QSqlQueryModel(this);
    deudas              = new QSqlQueryModel(this);
    qModeldireccion     = new QSqlQueryModel(this);
    Pedidos             = new QSqlQueryModel(this);
    Albaranes           = new QSqlQueryModel(this);
    Facturas            = new QSqlQueryModel(this);
    Presupuestos        = new QSqlQueryModel(this);
    Vales               = new QSqlQueryModel(this);
    Tickets             = new QSqlQueryModel(this);
    modelAsientos       = new QSqlQueryModel(this);
    queryTransportistas = new QSqlQueryModel(this);
    queryAgentes        = new QSqlQueryModel(this);
    qTarifa             = new QSqlQueryModel(this);    
    qmidiomas           = new QSqlQueryModel(this);
    modelHistorial      = new QSqlQueryModel(this);

    pob_completer_model = new QSqlTableModel(this,QSqlDatabase::database("calles"));

    pob_completer = new QCompleter(pob_completer_model,this);
    pob_completer_model->setTable("municipios");

    pob_completer->setCaseSensitivity(Qt::CaseInsensitive);
    pob_completer->setCompletionColumn(3);
    ui->txtpoblacion->setCompleter(pob_completer);
    ui->txtpoblacionAlternativa->setCompleter(pob_completer);

    calle_completer_model = new QSqlTableModel(this,QSqlDatabase::database("calles"));
    calle_completer_model->setTable("calles");
    calle_completer = new QCompleter(calle_completer_model,this);
    calle_completer->setCaseSensitivity(Qt::CaseInsensitive);
    calle_completer->setCompletionColumn(2);
    ui->txtdireccion1->setCompleter(calle_completer);
    ui->txtdireccion2->setCompleter(calle_completer);
    ui->txtdireccion1Alternativa1->setCompleter(calle_completer);
    ui->txtdireccion1Alternativa2->setCompleter(calle_completer);

    //DELEGATES
    ui->tablaAsientos->setItemDelegateForColumn(2, new DateDelegate(this));
    ui->tablaAsientos->setItemDelegateForColumn(5, new MonetaryDelegate(this,false));
    ui->tablaAsientos->setItemDelegateForColumn(8, new MonetaryDelegate(this,false));

    ui->tablaTickets->setItemDelegateForColumn(2, new ColumnaFecha(this));
    ui->tablaTickets->setItemDelegateForColumn(3, new ColumnaFecha(this));

    ui->tablaVales->setItemDelegateForColumn(2, new DateDelegate(this));
    ui->tablaVales->setItemDelegateForColumn(3, new DateDelegate(this));

    ui->tablaPresupuestos->setItemDelegateForColumn(2, new DateDelegate(this));
    ui->tablaPresupuestos->setItemDelegateForColumn(3, new DateDelegate(this));
    ui->tablaPresupuestos->setItemDelegateForColumn(4, new MonetaryDelegate(this,false));

    ui->tablaFacturas->setItemDelegateForColumn(2, new DateDelegate(this));
    ui->tablaFacturas->setItemDelegateForColumn(3, new DateDelegate(this));

    ui->TablaAlbaranes->setItemDelegateForColumn(2, new DateDelegate(this));

    ui->TablaDeudas->setItemDelegateForColumn(2,new MonetaryDelegate(this,false));
    ui->TablaDeudas->setItemDelegateForColumn(3,new MonetaryDelegate(this,false));
    ui->TablaDeudas->setItemDelegateForColumn(4,new MonetaryDelegate(this,false));
    ui->TablaDeudas->setItemDelegateForColumn(5,new DateDelegate(this));
    ui->TablaDeudas->setItemDelegateForColumn(6,new DateDelegate(this));

    ui->tablahistorial_deudas->setItemDelegateForColumn(2,  new DateDelegate(this));
    ui->tablahistorial_deudas->setItemDelegateForColumn(3,  new MonetaryDelegate(this));
    ui->tablahistorial_deudas->setItemDelegateForColumn(4,  new MonetaryDelegate(this));
    ui->tablahistorial_deudas->setItemDelegateForColumn(5,  new MonetaryDelegate(this));
    ui->tablahistorial_deudas->setItemDelegateForColumn(6,  new MonetaryDelegate(this));
    ui->tablahistorial_deudas->setItemDelegateForColumn(11, new MonetaryDelegate(this));
    ui->tablahistorial_deudas->setItemDelegateForColumn(12, new MonetaryDelegate(this));
    ui->tablahistorial_deudas->setItemDelegateForColumn(13, new MonetaryDelegate(this));
    ui->tablahistorial_deudas->setItemDelegateForColumn(14, new MonetaryDelegate(this));
    ui->tablahistorial_deudas->setItemDelegateForColumn(15, new MonetaryDelegate(this));
    ui->tablahistorial_deudas->setItemDelegateForColumn(16, new MonetaryDelegate(this));
    ui->tablahistorial_deudas->setItemDelegateForColumn(17, new MonetaryDelegate(this));


    //SET MODELS
    ui->tablaFacturas                ->setModel(Facturas);
    ui->tablaPresupuestos            ->setModel(Presupuestos);
    ui->tablaVales                   ->setModel(Vales);
    ui->tablaTickets                 ->setModel(Tickets);
    ui->lista_direccionesAlternativas->setModel(qModeldireccion);
    ui->tablaAsientos                ->setModel(modelAsientos);
    ui->TablaAlbaranes               ->setModel(Albaranes);
    ui->TablaDeudas                  ->setModel(deudas);
    ui->tabla_busquedas              ->setModel(m_clientes);    

    ui->tablahistorial_deudas        ->setModel(modelHistorial);
    ui->tablaPedidos                 ->setModel(Pedidos);

    ui->cboforma_pago                ->setModel(Configuracion_global->formapago_model);
    ui->cboDivisa                    ->setModel(Configuracion_global->divisas_model);    
    ui->cboPais                      ->setModel(Configuracion_global->paises_model);
    ui->cbopaisAlternativa           ->setModel(Configuracion_global->paises_model);


    //CONFIG COMBOS    
    ui->cboforma_pago   ->setModelColumn(2);
    ui->cboDivisa       ->setModelColumn(1);
    ui->cboPais         ->setModelColumn(1);
    ui->cbopaisAlternativa->setModelColumn(1);


    //SEARCH HASH
    h_Buscar["Población"]="poblacion";
    h_Buscar["Teléfono"] = "telefono1";
    h_Buscar["Código cliente"]="codigo_cliente";
    h_Buscar["Cif / Nif"] = "cif_nif";
    h_Buscar["Nombre Fiscal"]="nombre_fiscal";   


    ValidarCC();
    ui->btnFichaPaciente->setVisible(Configuracion_global->medic);
    ui->chkClienteEmpresa->setVisible(Configuracion_global->irpf);
    ui->blinkink->setVisible(false);

    //Connect signals /slots.
    connect(ui->txtPrimerApellido ,SIGNAL(editingFinished()),this,SLOT(txtApellido_Nombre_editingFinished()));
    connect(ui->txtSegundoApellido,SIGNAL(editingFinished()),this,SLOT(txtApellido_Nombre_editingFinished()));
    connect(ui->txtnombre         ,SIGNAL(editingFinished()),this,SLOT(txtApellido_Nombre_editingFinished()));

    connect(ui->txtprovincia,SIGNAL(editingFinished()),this,SLOT(txtprovincia_editingFinished()));
    connect(ui->txtcif_nif,SIGNAL(editingFinished()),this,SLOT(txtcif_nif_editingFinished()));
    connect(ui->txtrRiesgoPermitido,SIGNAL(editingFinished()),this,SLOT(txtrRiesgoPermitido_editingFinished()));
    connect(ui->lista_direccionesAlternativas,SIGNAL(clicked(QModelIndex)),this,SLOT(CargardireccionAlternativa(QModelIndex)));
    connect(ui->btnVer_OtrosContactos,SIGNAL(clicked()),this,SLOT(Contactos()));
    connect(ui->btnEditardireccionAlternativa,SIGNAL(clicked()),this,SLOT(EditardireccionAlternativa()));
    connect(ui->txtentidad_bancaria,SIGNAL(editingFinished()),this,SLOT(ValidarCC()));
    connect(ui->txtoficina_bancaria,SIGNAL(editingFinished()),this,SLOT(ValidarCC()));
    connect(ui->txtdc,SIGNAL(editingFinished()),this,SLOT(ValidarCC()));
    connect(ui->txtcuenta_contable,SIGNAL(editingFinished()),this,SLOT(ValidarCC()));
    connect(ui->btnValidarVIES,SIGNAL(clicked()),this,SLOT(validarNifIntrac()));
    connect(ui->txtnombre_fiscal,SIGNAL(editingFinished()),this,SLOT(set_blink()));
    connect(ui->txtcif_nif,SIGNAL(editingFinished()),this,SLOT(set_blink()));
    connect(ui->txtdireccion1,SIGNAL(editingFinished()),this,SLOT(set_blink()));
    connect(ui->txtcp,SIGNAL(editingFinished()),this,SLOT(set_blink()));
    connect(ui->txtpoblacion,SIGNAL(editingFinished()),this,SLOT(set_blink()));
    connect(ui->txtprovincia,SIGNAL(editingFinished()),this,SLOT(set_blink()));
    connect(ui->btnBorrardireccion,SIGNAL(clicked()),this,SLOT(BorrardireccionAlternativa()));    
    connect(ui->TablaDeudas,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menu_deudas(QPoint)));

    ui->TablaDeudas->setContextMenuPolicy(Qt::CustomContextMenu);

    // Busquedas
    setUpBusqueda();
    formato_tabla_busquedas();
    bloquearCampos(true);

    // Procesar eventos
    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;
    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);
    __init = true;
}

frmClientes::~frmClientes()
{
    delete ui;
}
void frmClientes::LLenarCampos()
{
    if(!__init)
        return;
    /* Global */
    ui->txtNombreFiscal->setText(oCliente->nombre_fiscal);

    /*  tab_datos */
    if (oCliente->codigo_cliente.isEmpty())
        oCliente->codigo_cliente = oCliente->Nuevocodigo_cliente();
    ui->txtcodigo_cliente->setText(oCliente->codigo_cliente);
    ui->txtPrimerApellido->setText(oCliente->apellido1);
    ui->txtSegundoApellido->setText(oCliente->apellido2);
    ui->txtnombre->setText(oCliente->nombre);    
    ui->txtnombre_fiscal->setText(oCliente->nombre_fiscal);
    ui->txtcif_nif->setText(oCliente->cif_nif);
    ui->txtCifIntracomunitario->setText(oCliente->cifVies);
    ui->txtnombre_comercial->setText(oCliente->nombre_comercial);
    ui->txtcp->setText(oCliente->cp);
    ui->txtpoblacion->setText(oCliente->poblacion);
    ui->txtdireccion1->setText(oCliente->direccion1);
    ui->txtdireccion2->setText(oCliente->direccion2);
    ui->txtprovincia->setText(oCliente->provincia);
    for(auto i=0; i< Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oCliente->id_pais)
        {
            ui->cboPais->setCurrentIndex(i);
            break;
        }
    }
    ui->txttelefono1->setText(oCliente->telefono1);
    ui->txttelefono2->setText(oCliente->telefono2);
    ui->txtfax->setText(oCliente->fax);
    ui->txtmovil->setText(oCliente->movil);
    ui->txtemail->setText(oCliente->email);
    ui->txtweb->setText(oCliente->web);
    ui->txtObservaciones->setText(oCliente->observaciones);

    set_blink();
    llenar_tipoCliente();

    /*  tab direcciones  */
    qModeldireccion->setQuery("select * from cliente_direcciones where id_cliente = "+QString::number(oCliente->id),
                              Configuracion_global->groupDB);

    /* tab_Datos_bancarios_financieros */

    ui->txtvisa1_caduca_ano->setText(QString::number(oCliente->visa1_caduca_ano));
    ui->txtvisa1_caduca_mes->setText(QString::number(oCliente->visa1_caduca_mes));
    ui->txtvisa1_cod_valid->setText(QString::number(oCliente->visa1_cod_valid));
    ui->txtvisa2_caduca_ano->setText(QString::number(oCliente->visa2_caduca_ano));
    ui->txtvisa2_caduca_mes->setText(QString::number(oCliente->visa2_caduca_mes));
    ui->txtvisa2_cod_valid->setText(QString::number(oCliente->visa2_cod_valid));

    ui->txtvisa_distancia1->setText(oCliente->visa_distancia1);
    ui->txtvisa_distancia2->setText(oCliente->visa_distancia2);
    ui->txtimporte_a_cuenta->setText( Configuracion_global->toFormatoMoneda(QString::number(oCliente->importe_a_cuenta,'f',Configuracion_global->decimales)));
    ui->txtvales->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente->vales,'f',Configuracion_global->decimales)));
    ui->txtentidad_bancaria->setText(oCliente->entidad_bancaria);
    ui->txtoficina_bancaria->setText(oCliente->oficina_bancaria);
    ui->txtdc->setText(oCliente->dc);
    ui->txtcuenta_corriente->setText(oCliente->cuenta_corriente);
    ui->chkrecargo_equivalencia->setChecked(oCliente->recargo_equivalencia);
    ui->chkClienteEmpresa->setChecked(oCliente->lIRPF);
    ui->txtcuenta_contable->setText(oCliente->cuenta_contable);
    ui->txtcuenta_deudas->setText(oCliente->cuenta_deudas);
    ui->txtcuenta_cobros->setText(oCliente->cuenta_cobros);
    ui->txtdia_pago1->setValue(oCliente->dia_pago1);
    ui->txtdia_pago2->setValue(oCliente->dia_pago2);
    ui->txtporc_dto_cliente->setText(QString::number(oCliente->porc_dto_cliente));
    ui->txtcuenta_iva_repercutido->setText(oCliente->cuenta_iva_repercutido);

    switch (oCliente->grupo_iva) {
    case 1:
       ui->radGeneral->setChecked(true);
        break;
    case 2:
        ui->radUE->setChecked(true);
        break;
    case 3:
        ui->radExcento->setChecked(true);
        break;
    case 4:
        ui->radExportacion->setChecked(true);
        break;
    }

    for(auto i=0; i< qTarifa->rowCount();i++)
    {
        if(qTarifa->record(i).value("id").toInt() == oCliente->idTarifa)
        {
            ui->cbotarifa_cliente->setCurrentIndex(i);
            break;
        }
    }
    for(auto i=0; i< Configuracion_global->divisas_model->rowCount();i++)
    {
        if(Configuracion_global->divisas_model->record(i).value("id").toInt() == oCliente->id_divisa)
        {
            ui->cboDivisa->setCurrentIndex(i);
            break;
        }
    }
    for(auto i=0; i< Configuracion_global->formapago_model->rowCount();i++)
    {
        if(Configuracion_global->formapago_model->record(i).value("id").toInt() == oCliente->id_forma_pago)
        {
            ui->cboforma_pago->setCurrentIndex(i);
            break;
        }
    }
    ValidarCC();

    /* tab_estadistica */

    ui->txtventas_ejercicio->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente->ventas_ejercicio,'f',
                                                                                           Configuracion_global->decimales_campos_totales)));

    ui->txtimporteAcumulado->setText(Configuracion_global->toFormatoMoneda(QString::number( oCliente->acumulado_ventas,'f',
                                                                                            Configuracion_global->decimales_campos_totales)));

    ui->txtfecha_ultima_compra->setDate(oCliente->fechaCompra);
    ui->txtdeuda_actual->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente->deuda_actual,'f',
                                                                                       Configuracion_global->decimales_campos_totales)));

    QMap <int,QSqlRecord> acum;
    QString error;
    acum = SqlCalls::SelectRecord("acum_clientes",QString("id_cliente =%1 and id_empresa=%2").arg(oCliente->id).arg(Configuracion_global->idEmpresa),
                                  Configuracion_global->groupDB,error);
    QMapIterator <int,QSqlRecord> i(acum);
    while(i.hasNext())
    {
        i.next();
        ui->txtEnero->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_enero").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtFebrero->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_febrero").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtMarzo->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_marzo").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtAbril->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_abril").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtMayo->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_mayo").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtJunio->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_junio").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtjulio->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_julio").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtAgosto->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_agosto").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtSeptiembre->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_septiembre").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtOctubre->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_octubre").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtNoviembre->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_noviembre").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtDiciembre->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_diciembre").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
        ui->txtventas_ejercicio->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(i.value().value("acum_ejercicio").toDouble(),'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
    }
    refrescar_grafica();

    /*  tab_deudas */

    QString cSQL;
    if(ui->radPendientes->isChecked())
    {
        cSQL= "Select id,documento,importe,pagado,pendiente_cobro,fecha,vencimiento from clientes_deuda where id_cliente = "
                + QString::number(oCliente->id)+" and id_empresa= "+QString::number(Configuracion_global->idEmpresa) +" and pendiente_cobro >0";
    }
    else
    {
        cSQL= "Select id,documento,importe,pagado,pendiente_cobro,fecha,vencimiento from clientes_deuda where id_cliente = "
                + QString::number(oCliente->id)+" and id_empresa= "+QString::number(Configuracion_global->idEmpresa) + " and pendiente_cobro <=0";
    }
    deudas->setQuery(cSQL,Configuracion_global->groupDB);

    ui->tablahistorial_deudas->setColumnHidden(0,true);
    ui->tablahistorial_deudas->setColumnHidden(1,true);

    ui->TablaDeudas->setColumnHidden(0,true);
    deudas->setHeaderData(1,Qt::Horizontal,tr("Documento"));
    deudas->setHeaderData(2,Qt::Horizontal,tr("Importe"));
    deudas->setHeaderData(3,Qt::Horizontal,tr("Pagado"));
    deudas->setHeaderData(4,Qt::Horizontal,tr("Pendiente"));
    deudas->setHeaderData(5,Qt::Horizontal,tr("Fecha"));
    deudas->setHeaderData(6,Qt::Horizontal,tr("Vencimiento"));

    /*  tab_coments  */

    ui->txtcomentarios->setText(oCliente->comentarios);
    ui->txtfecha_nacimiento->setDate(oCliente->fecha_nacimiento);
    ui->chklBloqueoCliente->setChecked(oCliente->bloqueado);
    ui->txtcomentario_bloqueo->setText(oCliente->comentario_bloqueo);
    ui->txtacceso_web->setText(oCliente->acceso_web);
    ui->txtpassword_web->setText(oCliente->password_web);
    ui->cboidiomaDocumentos->setCurrentIndex(ui->cboidiomaDocumentos->findText(oCliente->idioma));
    ui->txtfecha_alta->setDate(oCliente->fecha_alta);
    ui->txtrRiesgoPermitido->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente->riesgo_maximo,'f',
                                                                                           Configuracion_global->decimales_campos_totales)));

    for(auto i = 0; i<queryTransportistas->rowCount();i++)
    {
        if(queryTransportistas->record(i).value("id").toInt() == oCliente->id_transportista)
        {
            ui->cbotransportista->setCurrentIndex(i);
            break;
        }
    }
    for(auto i = 0; i<queryAgentes->rowCount();i++)
    {
        if(queryAgentes->record(i).value("id").toInt() == oCliente->id_agente)
        {
            ui->cboagente->setCurrentIndex(i);
            break;
        }
    }

    /* tab_historial */

         /********************************************************************
         * PEDIDOS
         ********************************************************************/
    cSQL= "Select id,pedido,fecha,fecha_limite_entrega,albaran,total_pedido from ped_cli where id_cliente =" + QString::number(oCliente->id);
    Pedidos->setQuery(cSQL,Configuracion_global->empresaDB);

    ui->tablaPedidos->setColumnWidth(0,0);
    ui->tablaPedidos->setColumnWidth(1,85);
    ui->tablaPedidos->setColumnWidth(2,85);
    ui->tablaPedidos->setColumnWidth(3,85);
    ui->tablaPedidos->setColumnWidth(4,85);
    ui->tablaPedidos->setColumnWidth(5,85);

    Pedidos->setHeaderData(1, Qt::Horizontal, QObject::tr("PEDIDO"));
    Pedidos->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
    Pedidos->setHeaderData(3, Qt::Horizontal, QObject::tr("FECHA LIMITE"));
    Pedidos->setHeaderData(4, Qt::Horizontal, QObject::tr("ALBARÁN"));
    Pedidos->setHeaderData(5, Qt::Horizontal, QObject::tr("TOTAL"));
        /********************************************************************
        * ALBARANES
        *******************************************************************/
    cSQL= "Select id,albaran,fecha,factura,total_albaran from cab_alb where id_cliente =" + QString::number(oCliente->id);
    Albaranes->setQuery(cSQL,Configuracion_global->empresaDB);

    ui->TablaAlbaranes->setColumnWidth(0,0);
    ui->TablaAlbaranes->setColumnWidth(1,85);
    ui->TablaAlbaranes->setColumnWidth(2,85);
    ui->TablaAlbaranes->setColumnWidth(3,85);
    ui->TablaAlbaranes->setColumnWidth(4,85);       

    Albaranes->setHeaderData(1, Qt::Horizontal, QObject::tr("ALBARAN"));
    Albaranes->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
    Albaranes->setHeaderData(3, Qt::Horizontal, QObject::tr("FACTURA"));
    Albaranes->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));

        /********************************************************************
       * FACTURAS
       *******************************************************************/
    cSQL= "Select id,factura,fecha,fecha_cobro,total from cab_fac where id_cliente =" + QString::number(oCliente->id);
    Facturas->setQuery(cSQL,Configuracion_global->empresaDB);

    ui->tablaFacturas->setColumnWidth(0,0);
    ui->tablaFacturas->setColumnWidth(1,85);
    ui->tablaFacturas->setColumnWidth(2,85);
    ui->tablaFacturas->setColumnWidth(3,85);
    ui->tablaFacturas->setColumnWidth(4,85);

    Facturas->setHeaderData(1, Qt::Horizontal, QObject::tr("FACTURA"));
    Facturas->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
    Facturas->setHeaderData(3, Qt::Horizontal, QObject::tr("FECHA COBRO"));
    Facturas->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));

      /********************************************************************
      * PRESUPUESTOS
      *******************************************************************/
    cSQL= "Select id,presupuesto,fecha,valido_hasta,total from cab_pre where id_cliente =" + QString::number(oCliente->id);
    Presupuestos->setQuery(cSQL,Configuracion_global->empresaDB);

    ui->tablaPresupuestos->setColumnWidth(0,0);
    ui->tablaPresupuestos->setColumnWidth(1,85);
    ui->tablaPresupuestos->setColumnWidth(2,85);
    ui->tablaPresupuestos->setColumnWidth(3,85);
    ui->tablaPresupuestos->setColumnWidth(4,85);

    Presupuestos->setHeaderData(1, Qt::Horizontal, QObject::tr("PRES"));
    Presupuestos->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
    Presupuestos->setHeaderData(3, Qt::Horizontal, QObject::tr("FECHA APRO."));
    Presupuestos->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));

     /********************************************************************
     * VALES
     *******************************************************************/
    cSQL= "Select id,Numero,fecha,fecha_vencimiento_vale,importe from vales where id_cliente =" + QString::number(oCliente->id);
    Vales->setQuery(cSQL,Configuracion_global->empresaDB);

    ui->tablaVales->setColumnWidth(0,0);
    ui->tablaVales->setColumnWidth(1,85);
    ui->tablaVales->setColumnWidth(2,85);
    ui->tablaVales->setColumnWidth(3,85);
    ui->tablaVales->setColumnWidth(4,85);

    Vales->setHeaderData(1, Qt::Horizontal, QObject::tr("VALE"));
    Vales->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
    Vales->setHeaderData(3, Qt::Horizontal, QObject::tr("VTO."));
    Vales->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));
    /********************************************************************
    * TICKETS
    *******************************************************************/
    cSQL= "Select id,ticket,fecha,hora,importe from cab_tpv where id_cliente =" + QString::number(oCliente->id);
    Tickets->setQuery(cSQL,Configuracion_global->empresaDB);

    ui->tablaTickets->setColumnWidth(0,0);
    ui->tablaTickets->setColumnWidth(1,85);
    ui->tablaTickets->setColumnWidth(2,85);
    ui->tablaTickets->setColumnWidth(3,85);
    ui->tablaTickets->setColumnWidth(4,85);

    Tickets->setHeaderData(1, Qt::Horizontal, QObject::tr("TICKET"));
    Tickets->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
    Tickets->setHeaderData(3, Qt::Horizontal, QObject::tr("HORA"));
    Tickets->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));

    //----------------------
    // Asientos contables
    //----------------------
    modelAsientos->setQuery("select id,asiento,fecha_asiento,cuenta_d,descripcion_d, importe_d,cuenta_h,descripcion_h,importe_h "
                            "from diario where cta_principal = '"+oCliente->cuenta_contable +
                            "' order by asiento + ' '+pos_en_asiento",
                            Configuracion_global->contaDB);


    ui->tablaAsientos->setColumnHidden(0,true);
    ui->tablaAsientos->setColumnWidth(1,55);
    ui->tablaAsientos->setColumnWidth(2,63);
    ui->tablaAsientos->setColumnWidth(3,55);
    ui->tablaAsientos->setColumnWidth(4,75);
    ui->tablaAsientos->setColumnWidth(5,60);
    ui->tablaAsientos->setColumnWidth(6,55);
    ui->tablaAsientos->setColumnWidth(7,75);
    ui->tablaAsientos->setColumnWidth(8,60);   
}

void frmClientes::VaciarCampos()
{
    QDate fecha;

    ui->txtcodigo_cliente->setText("");
    ui->txtPrimerApellido->setText("");
    ui->txtSegundoApellido->setText("");
    ui->txtnombre->setText("");
    ui->txtnombre_fiscal->setText("");
    ui->txtnombre_comercial->setText("");
    ui->txtcif_nif->setText("");
    ui->txtCifIntracomunitario->setText("");
    ui->txtcp->setText("");
    ui->txtdireccion1->setText("");
    ui->txtdireccion2->setText("");
    ui->txtpoblacion->setText("");
    ui->txtprovincia->setText("");
    ui->cboPais->clearEditText();
    ui->txtcpPoblacionAlternativa->setText("");
    ui->txtpoblacionAlternativa->setText("");
    ui->txtprovinciaAlternativa->setText("");
    ui->cbopaisAlternativa->clearEditText();
    ui->cboforma_pago->setCurrentIndex(-1);
    ui->txttelefono1->setText("");
    ui->txttelefono2->setText("");
    ui->txtfax->setText("");
    ui->txtmovil->setText("");
    ui->txtemail->setText("");
    ui->txtweb->setText("");
    ui->txtfecha_alta->setDate(fecha.currentDate());
    ui->txtfecha_ultima_compra->setDate(fecha.currentDate());
    ui->txtimporteAcumulado->setText("");
    ui->txtventas_ejercicio->setText("");
    ui->txtrRiesgoPermitido->setText("");
    ui->txtdeuda_actual->setText("");
    ui->txtcomentarios->setText("");
    ui->chklBloqueoCliente->setChecked(false);
    ui->txtcomentario_bloqueo->setText( "");
    ui->txtporc_dto_cliente->setText("0");
    ui->chkrecargo_equivalencia->setChecked(false);
    ui->txtcuenta_contable->setText("");
    ui->txtcuenta_iva_repercutido->setText("");
    ui->txtcuenta_deudas->setText("");
    ui->txtcuenta_cobros->setText("");


    ui->cboforma_pago->setCurrentIndex(0);
    ui->txtdia_pago1->setValue(0);
    ui->txtdia_pago2->setValue(0);

    ui->txtimporte_a_cuenta->setText("");
    ui->txtvales->setText("");
    ui->txtentidad_bancaria->setText("");
    ui->txtoficina_bancaria->setText("");
    ui->txtdc->setText("");
    ui->txtcuenta_corriente->setText("");
    ui->txtfecha_nacimiento->setDate(fecha.currentDate());
    ui->txtacceso_web->setText("");
    ui->txtpassword_web->setText("");
    ui->txtPrimerApellido->setFocus();
    ui->chkClienteEmpresa->setChecked(false);
    ui->txtObservaciones->setText("");
    ui->cboagente->setCurrentIndex(-1);
    ui->cbotransportista->setCurrentIndex(-1);
    ui->txtvisa1_caduca_ano->setText("");
    ui->txtvisa1_caduca_mes->setText("");
    ui->txtvisa1_cod_valid->setText("");
    ui->txtvisa2_caduca_ano->setText("");
    ui->txtvisa2_caduca_mes->setText("");
    ui->txtvisa2_cod_valid->setText("");
    ui->txtvisa_distancia1->setText("");
    ui->txtvisa_distancia2->setText("");
    ui->txtNombreFiscal->setText("");


    // Tipos de clientes
    ui->lista_tipos->clear();
    oCliente->codigo_cliente= "";

    for(auto i=0; i< qTarifa->rowCount();i++)
    {
        if(qTarifa->record(i).value("id").toInt() == 1)
        {
            ui->cbotarifa_cliente->setCurrentIndex(i);
            break;
        }
    }
    for(auto i=0; i< Configuracion_global->divisas_model->rowCount();i++)
    {
        if(Configuracion_global->divisas_model->record(i).value("id").toInt() == 1)
        {
            ui->cboDivisa->setCurrentIndex(i);
            break;
        }
    }
    for(auto i=0; i< Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == 1)
        {
            ui->cboPais->setCurrentIndex(i);
            ui->cbopaisAlternativa->setCurrentIndex(i);
            break;
        }
    }
}

void frmClientes::LLenarCliente()
{
    oCliente->codigo_cliente= ui->txtcodigo_cliente->text();
    oCliente->apellido1=ui->txtPrimerApellido->text();
    oCliente->apellido2=ui->txtSegundoApellido->text();
    oCliente->nombre=ui->txtnombre->text();
    oCliente->nombre_fiscal=ui->txtnombre_fiscal->text();
    oCliente->nombre_comercial=ui->txtnombre_comercial->text();
    oCliente->cif_nif=ui->txtcif_nif->text();
    oCliente->cifVies = ui->txtCifIntracomunitario->text();
    oCliente->direccion1=ui->txtdireccion1->text();
    oCliente->direccion2=ui->txtdireccion2->text();
    oCliente->cp=ui->txtcp->text();
    oCliente->poblacion=ui->txtpoblacion->text();
    oCliente->provincia=ui->txtprovincia->text();
    oCliente->pais=ui->cboPais->currentText();
    oCliente->id_pais = oCliente->Buscarid_pais(oCliente->pais);
    oCliente->telefono1=ui->txttelefono1->text();
    oCliente->telefono2=ui->txttelefono2->text();
    oCliente->fax=ui->txtfax->text();
    oCliente->movil=ui->txtmovil->text();
    oCliente->email=ui->txtemail->text();
    oCliente->web=ui->txtweb->text();
    oCliente->fecha_alta=ui->txtfecha_alta->date();
    oCliente->fechaCompra=ui->txtfecha_ultima_compra->date();
    oCliente->acumulado_ventas=ui->txtimporteAcumulado->text().replace(".","").toDouble();
    oCliente->ventas_ejercicio=ui->txtventas_ejercicio->text().replace(".","").toDouble();
    oCliente->riesgo_maximo=ui->txtrRiesgoPermitido->text().replace(".","").toDouble();
    oCliente->deuda_actual=ui->txtdeuda_actual->text().replace(".","").toDouble();
    oCliente->comentarios=ui->txtcomentarios->toPlainText();
    oCliente->bloqueado=ui->chklBloqueoCliente->isChecked();
    oCliente->comentario_bloqueo=ui->txtcomentario_bloqueo->toPlainText();
    oCliente->porc_dto_cliente=ui->txtporc_dto_cliente->text().toInt();
    oCliente->recargo_equivalencia=ui->chkrecargo_equivalencia->isChecked();
    oCliente->cuenta_contable=ui->txtcuenta_contable->text();
    oCliente->cuenta_iva_repercutido=ui->txtcuenta_iva_repercutido->text();
    oCliente->cuenta_deudas=ui->txtcuenta_deudas->text();
    oCliente->cuenta_cobros=ui->txtcuenta_cobros->text();

    oCliente->id_forma_pago=Configuracion_global->formapago_model->record(ui->cboforma_pago->currentIndex()).value("id").toInt();

    oCliente->dia_pago1=ui->txtdia_pago1->value();
    oCliente->dia_pago2=ui->txtdia_pago2->value();
    oCliente->riesgo_maximo=Configuracion_global->MonedatoDouble(ui->txtrRiesgoPermitido->text());

    oCliente->idTarifa= this->qTarifa->record(ui->cbotarifa_cliente->currentIndex()).value("id").toInt();

    oCliente->importe_a_cuenta=ui->txtimporte_a_cuenta->text().toDouble();
    oCliente->vales= ui->txtvales->text().replace(".","").toDouble();
    oCliente->entidad_bancaria=ui->txtentidad_bancaria->text();
    oCliente->oficina_bancaria=ui->txtoficina_bancaria->text();
    oCliente->dc=ui->txtdc->text();
    oCliente->cuenta_corriente=ui->txtcuenta_corriente->text();
    oCliente->fecha_nacimiento=ui->txtfecha_nacimiento->date();
    oCliente->importe_pendiente=ui->txtimporteAcumulado->text().toDouble();
    oCliente->acceso_web=ui->txtacceso_web->text();
    oCliente->password_web=ui->txtpassword_web->text();
    oCliente->ididioma = qmidiomas->record(ui->cboidiomaDocumentos->currentIndex()).value("id").toInt();
    oCliente->idioma = ui->cboidiomaDocumentos->currentText();
    oCliente->observaciones = ui->txtObservaciones->text();
    oCliente->lIRPF =ui->chkClienteEmpresa->isChecked();
    oCliente->visa1_caduca_ano = ui->txtvisa1_caduca_ano->text().toInt();
    oCliente->visa1_caduca_mes = ui->txtvisa1_caduca_mes->text().toInt();
    oCliente->visa1_cod_valid = ui->txtvisa1_cod_valid->text().toInt();
    oCliente->visa2_caduca_ano = ui->txtvisa2_caduca_ano->text().toInt();
    oCliente->visa2_caduca_mes = ui->txtvisa2_caduca_mes->text().toInt();
    oCliente->visa2_cod_valid = ui->txtvisa2_cod_valid->text().toInt();
    oCliente->visa_distancia1 = ui->txtvisa_distancia1->text();
    oCliente->visa_distancia2 = ui->txtvisa_distancia2->text();

    oCliente->id_transportista = queryTransportistas->record(ui->cbotransportista->currentIndex()).value("id").toInt();
    oCliente->id_agente = queryAgentes->record(ui->cboagente->currentIndex()).value("id").toInt();

    if (ui->radGeneral->isChecked())
        oCliente->grupo_iva = 1;
    if(ui->radUE->isChecked())
        oCliente->grupo_iva = 2;
    if (ui->radExcento->isChecked())
        oCliente->grupo_iva = 3;
    if(ui->radExportacion->isChecked())
        oCliente->grupo_iva = 4;

    oCliente->id_divisa = Configuracion_global->divisas_model->record(ui->cboDivisa->currentIndex()).value("id").toInt();
}


void frmClientes::on_btnSiguiente_clicked()
{
    QString cid = QString::number(oCliente->id);
    oCliente->Recuperar("Select * from clientes where id >"+cid+" order by id limit 1 ");
    LLenarCampos();
}

void frmClientes::on_btnAnterior_clicked()
{
    QString cid = QString::number(oCliente->id);
    oCliente->Recuperar("Select * from clientes where id <"+cid+" order by id desc limit 1 ");
    LLenarCampos();
}

void frmClientes::on_btnGuardar_clicked()
{
    QString cVacios;
    if(ui->txtcif_nif->text().isEmpty())
        cVacios = "Falta rellenar CIF/NIF\n";
    if(ui->txtnombre_fiscal->text().isEmpty())
        cVacios = cVacios + "Falta rellenar NombreFiscal\n";
    if (ui->txtdireccion1->text().isEmpty())
        cVacios = cVacios + "Falta rellenar Dirección\n";
    if (ui->txtcp->text().isEmpty())
        cVacios = cVacios + "Falta rellenar CP\n";
    if(ui->txtpoblacion->text().isEmpty())
        cVacios = cVacios + "Falta rellenar Población\n";
    if (ui->txtprovincia->text().isEmpty())
        cVacios = cVacios + "Falta rellenar Provincia\n";
    if(ui->cboPais->currentText().isEmpty())
        cVacios = cVacios + "Falta rellenar Pais";

    if(cVacios.isEmpty()){
        LLenarCliente();
        oCliente->Guardar();
        bloquearCampos(true);
        emit unblock();
        Configuracion_global->groupDB.commit();
    } else
    {
        QMessageBox::warning(this,tr("Guardar ficha"),tr("tiene campos en blanco que no pueden quedar vacíos\n")+ cVacios,
                             tr("Aceptar"));
        ui->txtcif_nif->setFocus();
    }
    set_blink();
}

void frmClientes::on_btnAnadir_clicked()
{
    ocultarBusqueda();
    // -----------------------------
    // Forzamos entrada DNI
    // Comprobamos que no exista ya
    //------------------------------
    QDialog dlg(this);
    dlg.setWindowTitle(tr("NIF/CIF/NIE"));
    dlg.resize(200,130);

    QLineEdit linea(&dlg);
    linea.setStyleSheet("background-color: rgb(255,255,255)");

    QPushButton  btn(tr("Aceptar"),&dlg);

    QVBoxLayout lay(&dlg);
    lay.addWidget(&linea);
    lay.addWidget(&btn);

    dlg.setLayout(&lay);
    connect(&btn,SIGNAL(clicked()),&dlg,SLOT(accept()));

    dlg.exec();

    linea.setText(Configuracion_global->letraDNI(linea.text()));
    //---------------
    // Buscamos DNI
    //---------------
    QMap <int, QSqlRecord> h;
    QString error;
    h= SqlCalls::SelectRecord("clientes",QString("cif_nif ='%1'").arg(linea.text().trimmed()),Configuracion_global->groupDB,error);
    QString cif;
    int id;
    QMapIterator<int, QSqlRecord> i(h);
    while (i.hasNext())
    {
        i.next();
        cif = i.value().value("cif_nif").toString();
        id = i.value().value("id").toInt();
    }

    if( cif == linea.text())
    {
        QMessageBox::information(this,tr("Altas de clientes"),tr("Ya existe un cliente con ese numero de identificación"),
                                 tr("Aceptar"));
        ui->stackedWidget->setCurrentIndex(0);
        oCliente->Recuperar(id);
        LLenarCampos();
        bloquearCampos(true);
        emit unblock();

    } else
    {
        Configuracion_global->groupDB.transaction();

        VaciarCampos();

        this->Altas = true;
        ui->txtcodigo_cliente->setText(oCliente->Nuevocodigo_cliente());      
        ui->txtcuenta_contable->setText(ui->txtcodigo_cliente->text());
        ui->txtcodigo_cliente->setFocus();
        ui->cboidiomaDocumentos->setCurrentIndex(1);
        ui->cboforma_pago->setCurrentIndex(1);
        LLenarCliente();

        if(oCliente->Anadir())
        {
            ui->stackedWidget->setCurrentIndex(0);
            LLenarCampos();
            ui->txtcif_nif->setText(linea.text());
            oCliente->cif_nif = linea.text();
            set_blink();
            bloquearCampos(false);
            emit block();
        }
        else
        {
            Configuracion_global->rollback();
        }
    }
}

void frmClientes::txtApellido_Nombre_editingFinished()
{
    ui->txtPrimerApellido->setText(ui->txtPrimerApellido->text().toUpper());
    ui->txtSegundoApellido->setText(ui->txtSegundoApellido->text().toUpper());
    ui->txtnombre->setText(ui->txtnombre->text().toUpper());

    if(!ui->txtPrimerApellido->text().isEmpty()
       && !ui->txtSegundoApellido->text().isEmpty()
       && !ui->txtnombre->text().isEmpty())
    {
        ui->txtnombre_fiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtnombre->text());
        ui->txtNombreFiscal->setText(ui->txtnombre_fiscal->text());
    }
}

void frmClientes::txtprovincia_editingFinished()
{
    ui->txtprovincia->setText(ui->txtprovincia->text().toUpper());
}

void frmClientes::txtcif_nif_editingFinished()
{
    blockSignals(true);
    if(oCliente->cif_nif != ui->txtcif_nif->text())
    {
        if(QMessageBox::question(this,tr("Gestión de clientes"),tr("¿Desea cambiar el CIF/DNI del cliente?"),
                                 tr("No"),tr("Sí")) == QMessageBox::Accepted)
        {
            if (ui->txtcif_nif->text().length() == 8)
            {
                QString cif = Configuracion::letraDNI(ui->txtcif_nif->text());
                ui->txtcif_nif->setText(cif.toUpper());
                oCliente->cif_nif = cif.toUpper();
            }
            else if (ui->txtcif_nif->text().length() < 8)
            {
                TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("DNI Demasiado pequeño para calcular letra"));
            }
        }
        else
        {
           ui->txtcif_nif->setText(oCliente->cif_nif);
        }
    }
    blockSignals(false);
}

void frmClientes::on_btnEditar_clicked()
{
    if(ui->stackedWidget->currentIndex() == 1 )
    {
        if(!ui->tabla_busquedas->currentIndex().isValid())
            return;
        on_tabla_busquedas_doubleClicked(ui->tabla_busquedas->currentIndex());
    }

    Configuracion_global->groupDB.transaction();

    emit block();
    bloquearCampos(false);
    ui->txtcodigo_cliente->setEnabled(false);
    set_blink();
    ui->txtcif_nif->setFocus();
    ocultarBusqueda();
}
void frmClientes::bloquearCampos(bool state) {

    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(state);
        //qDebug() << lineEdit->objectName();
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(!state);
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    QSpinBox *SpinBox;
    foreach (SpinBox, SpinBoxList) {
        SpinBox->setReadOnly(state);
        //qDebug() << lineEdit->objectName();
    }
    // DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(state);
        //qDebug() << lineEdit->objectName();
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!state);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(state);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!state);
        //qDebug() << lineEdit->objectName();
    }

    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    ui->btnBorrar->setEnabled(state);
    ui->btnBuscar->setEnabled(state);
    ui->btnDeshacer->setEnabled(!state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->btnAnadirdireccion->setEnabled(!state);
    ui->btnBorrardireccion->setEnabled(!state);
    ui->btnEdita_tipoCliente->setEnabled(!state);
    ui->btnVer_OtrosContactos->setEnabled(state);
    ui->btnEditardireccionAlternativa->setEnabled(!state);
    m_busqueda->block(!state);
}

void frmClientes::on_btnDeshacer_clicked()
{
    Configuracion_global->groupDB.rollback();
    if(this->Altas)
    {
        oCliente->Recuperar("Select * from clientes order by id desc limit 1 ");
        this->Altas = false;
    }

    LLenarCampos();
    bloquearCampos(true);
    set_blink();
    emit unblock();
}

void frmClientes::on_btnBorrar_clicked()
{
    if(ui->stackedWidget->currentIndex() == 1 )
    {
        if(!ui->tabla_busquedas->currentIndex().isValid())
            return;
        on_tabla_busquedas_doubleClicked(ui->tabla_busquedas->currentIndex());
    }
    oCliente->Borrar(oCliente->id);
    VaciarCampos();
}

void frmClientes::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    mostrarBusqueda();
}

void frmClientes::txtrRiesgoPermitido_editingFinished()
{
    ui->txtrRiesgoPermitido->setText(Configuracion_global->toFormatoMoneda(ui->txtrRiesgoPermitido->text()));
}

void frmClientes::on_btnFichaPaciente_clicked()
{
    if(ui->txtcodigo_cliente->text().isEmpty()) {
        QMessageBox::warning(this,tr("Ficha Paciente"),tr("Debe tener un cliente seleccionado para poder acceder a la ficha de paciente"),
                             tr("Aceptar"));
        return;
    }

    int id_paciente;
    //if not patient record in database, create it now.
    Paciente oPaciente;
    oPaciente.RecuperarPaciente(oCliente->id);

    if(oPaciente.getid()==0){
        int id_cliente = oCliente->id;
        id_paciente = oPaciente.AnadirPaciente(id_cliente);
        if (id_paciente!=0)
            oPaciente.RecuperarPaciente(id_paciente);
    }
    // open patient form


    FrmFichaPaciente frmPaciente(this);
    frmPaciente.setWindowModality(Qt::WindowModal);

    frmPaciente.recibedatospaciente(oCliente->id,oCliente->nombre_fiscal);
    frmPaciente.cargarDatos(oCliente->id);
    frmPaciente.setWindowState(Qt::WindowMaximized);
    frmPaciente.exec();
}

void frmClientes::DeshacerdireccionAlternativa()
{
    ui->btnGuardar->setEnabled(true);
    ui->btnDeshacer->setEnabled(true);
    ui->btnGuardardireccionAlternativa->setEnabled(false);
    ui->btnDeshacerdireccionAlternativa->setEnabled(false);
    ui->btnEditardireccionAlternativa->setEnabled(true);
    CargardireccionAlternativa(ui->lista_direccionesAlternativas->currentIndex());
}

void frmClientes::BorrardireccionAlternativa()
{
    if(QMessageBox::question(this,tr("direcciones alternativas"),tr("va a borrar una dirección alternativa\n"
                                                                    "¿desea continuar?"),
                             tr("Cancelar"),tr("Borrar"))==QMessageBox::Accepted)
    {
        QSqlQuery qdireccion(Configuracion_global->groupDB);
        if(!qdireccion.exec("delete from cliente_direcciones where id="+QString::number(iddireccionAlternativa)))
            QMessageBox::warning(this,tr("direcciones Alternativas"),tr("No se ha podido borrar la direccion"),tr("Aceptar"));

    }
    QString cSQL = "select * from cliente_direcciones where id_cliente = "+QString::number(oCliente->id);
    qModeldireccion->setQuery(cSQL,Configuracion_global->groupDB);

    ui->txtdescripcion_direccion->setText("");
    ui->txtdireccion1Alternativa1->setText("");
    ui->txtdireccion1Alternativa2->setText("");
    ui->txtcpPoblacionAlternativa->setText("");
    ui->txtpoblacionAlternativa->setText("");
    ui->txtprovinciaAlternativa->setText("");
    ui->txtemail_alternativa->clear();
    ui->txtcomentarios_alternativa->clear();
    ui->cbopaisAlternativa->setCurrentIndex(-1);
}

void frmClientes::EditardireccionAlternativa()
{
    ui->btnGuardar->setEnabled(false);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditardireccionAlternativa->setEnabled(false);
    ui->btnGuardardireccionAlternativa->setEnabled(true);
    ui->btnDeshacerdireccionAlternativa->setEnabled(true);
    ui->txtdescripcion_direccion->setEnabled(true);
    ui->txtdireccion1Alternativa1->setEnabled(true);
    ui->txtdireccion1Alternativa2->setEnabled(true);
    ui->txtcpPoblacionAlternativa->setEnabled(true);
    ui->txtpoblacionAlternativa->setEnabled(true);
    ui->txtprovinciaAlternativa->setEnabled(true);
    ui->cbopaisAlternativa->setEnabled(true);
    Anadirdireccion = false;
}

void frmClientes::CargardireccionAlternativa(QModelIndex index)
{
    if(!index.isValid())
        return;
    QSqlRecord r = qModeldireccion->record(index.row());

    iddireccionAlternativa = r.value("id").toInt();
    ui->txtcpPoblacionAlternativa->setText (r.value("cp").toString());
    ui->txtdescripcion_direccion->setText  (r.value("descripcion").toString());
    ui->txtdireccion1Alternativa1->setText (r.value("direccion1").toString());
    ui->txtdireccion1Alternativa2->setText (r.value("direccion2").toString());
    ui->txtpoblacionAlternativa->setText   (r.value("poblacion").toString());
    ui->txtprovinciaAlternativa->setText   (r.value("provincia").toString());
    ui->txtemail_alternativa->setText      (r.value("email").toString());
    ui->txtcomentarios_alternativa->setText(r.value("comentarios").toString());

    for(auto i=0; i< Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == r.value("id_pais").toInt())
        {
            ui->cbopaisAlternativa->setCurrentIndex(i);
            break;
        }
    }

    if(!ui->btnEditar->isEnabled())
        ui->btnEditardireccionAlternativa->setEnabled(true);
}

void frmClientes::ValidarCC()
{
    QString cOk;
    if(!ui->txtentidad_bancaria->text().isEmpty() && !ui->txtoficina_bancaria->text().isEmpty() &&
            !ui->txtdc->text().isEmpty() && !ui->txtcuenta_corriente->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtentidad_bancaria->text(),ui->txtoficina_bancaria->text(),
                                 ui->txtdc->text(),ui->txtcuenta_corriente->text());
    if(cOk == "1"){
        ui->lblCuentavalida->setText(tr("La cuenta es válida"));
        ui->lblCuentavalida->setStyleSheet("QLabel { background-color :rgb(0,170,0)}");
    }
    else {
        ui->lblCuentavalida->setText(tr("La cuenta no es válida"));
        ui->lblCuentavalida->setStyleSheet("QLabel { background-color :rgb(170,0,0)}");
    }
}

void frmClientes::validarNifIntrac()
{
    bool Ok = Configuracion_global->comprobarNIF(ui->cboPais->currentText(),ui->txtCifIntracomunitario->text());
    if(!Ok)
        QMessageBox::warning(this,tr("Validación VIES"),tr("El cif introducido no es válido a nive de la UE"),tr("Aceptar"));
    else
        QMessageBox::warning(this,tr("Validacion VIES"),tr("El Cif VIES introducido es corecto"),tr("Aceptar"));
}

void frmClientes::Contactos()
{
    frmPersonasContactoCliente contacto;
    contacto.nid_cliente = oCliente->id;
    contacto.RefrescarTabla();
    contacto.exec();
}

void frmClientes::menu_deudas(QPoint position)
{
    QPoint globalPos = ui->TablaDeudas->mapToGlobal(position);
    QMenu myMenu;
    QAction *actionCobrar = new QAction(tr("cobrar vencimiento"),this);
    myMenu.addAction(actionCobrar);
    QAction *actionFraccion = new QAction(tr("Cobro parcial"),this);
    myMenu.addAction(actionFraccion);
    QAction *actionAsiento = new QAction(tr("Ver asiento contable"),this);
    myMenu.addAction(actionAsiento);

    connect(actionCobrar, SIGNAL(triggered()), this, SLOT(cobrar_deuda()));
    connect(actionFraccion,SIGNAL(triggered()),this,SLOT(cobrar_fraccion()));
    connect(actionAsiento,SIGNAL(triggered()),this,SLOT(ver_asiento()));

    myMenu.exec(globalPos);

    actionAsiento->deleteLater();
    actionCobrar->deleteLater();
    actionFraccion->deleteLater();
}

void frmClientes::cobrar_deuda()
{
// TODO COBRAR DEUDA
}

void frmClientes::cobrar_fraccion()
{
// TODO COBRAR fraccion
}

void frmClientes::ver_asiento()
{
// TODO ver_asiento
}

void frmClientes::refrescar_grafica()
{
    if(!__init)
        return;
    ui->graficaEstadistica->Clear();
    ui->graficaEstadistica->setTipo(OpenChart::BarraMultiple);
    ui->graficaEstadistica->addMulibarColor("Compras",Qt::darkBlue);
    QVector <float> enero;
    enero << Configuracion_global->MonedatoDouble(ui->txtEnero->text()); //Añadir tantos colores como elementos tenga el vector!

    ui->graficaEstadistica->addItem("Ene",enero);


    QVector <float> febrero;
    febrero <<Configuracion_global->MonedatoDouble(ui->txtFebrero->text());//Añadir tantos colores como elementos tenga el vector!

    ui->graficaEstadistica->addItem("Feb",febrero);

    QVector <float> marzo;
    marzo <<Configuracion_global->MonedatoDouble(ui->txtMarzo->text());//Añadir tantos colores como elementos tenga el vector!

    ui->graficaEstadistica->addItem("Mar",marzo);

    QVector <float> abril;
    abril <<Configuracion_global->MonedatoDouble(ui->txtAbril->text());//Añadir tantos colores como elementos tenga el vector!

    ui->graficaEstadistica->addItem("Abr",abril);

    QVector <float> mayo;
    mayo <<Configuracion_global->MonedatoDouble(ui->txtMayo->text());//Añadir tantos colores como elementos tenga el vector!

    ui->graficaEstadistica->addItem("May",mayo);

    QVector <float> junio;
    junio <<Configuracion_global->MonedatoDouble(ui->txtJunio->text());

    ui->graficaEstadistica->addItem("jun",junio);

    QVector <float> julio;
    julio <<Configuracion_global->MonedatoDouble(ui->txtjulio->text());

    ui->graficaEstadistica->addItem("Jul",julio);

    QVector <float> agosto;
    agosto <<Configuracion_global->MonedatoDouble(ui->txtAgosto->text());
    ui->graficaEstadistica->addItem("Ago",agosto);

    QVector <float> septiembre;
    septiembre <<Configuracion_global->MonedatoDouble(ui->txtSeptiembre->text());
    ui->graficaEstadistica->addItem("Sep",septiembre);

    QVector <float> octubre;
    octubre <<Configuracion_global->MonedatoDouble(ui->txtOctubre->text());
    ui->graficaEstadistica->addItem("Oct",octubre);

    QVector <float> noviembre;
    noviembre <<Configuracion_global->MonedatoDouble(ui->txtNoviembre->text());
    ui->graficaEstadistica->addItem("Nov",noviembre);

    QVector <float> diciembre;
    diciembre <<Configuracion_global->MonedatoDouble(ui->txtDiciembre->text());
    ui->graficaEstadistica->addItem("Dic",diciembre);

    ui->graficaEstadistica->repaint();
}

void frmClientes::set_blink()
{
    QString text_bottom  = "";

    if (ui->txtcodigo_cliente->text().isEmpty())
        text_bottom =tr("Código de cliente\n");

    if(ui->txtnombre_fiscal->text().isEmpty())
        text_bottom.append(tr("Nombre fiscal\n"));

    if(ui->txtcif_nif->text().isEmpty())
        text_bottom.append(tr("Cif/nif\n"));

    if(ui->txtdireccion1->text().isEmpty())
        text_bottom.append(tr("Direccion\n"));

    if(ui->txtcp->text().isEmpty())
        text_bottom.append(tr("CP\n"));

    if(ui->txtpoblacion->text().isEmpty())
        text_bottom.append(tr("Población\n"));

    if(ui->txtprovincia->text().isEmpty())
        text_bottom.append(tr("Provincia"));

    if (!text_bottom.isEmpty())
    {
        ui->blinkink->setVisible(true);
        ui->blink_stack->setCurrentIndex(1);
        ui->blinkink->settopTex(tr("ATENCIÓN TIENE\nCAMPOS OBLIGATORIOS\nPENDIENTES\nDE RELLENAR"));
        ui->blinkink->setbottomTex(text_bottom);
        ui->blinkink->setblink(true);
        ui->blinkink->setcolor(QColor (255,96,43));
    }
    else
    {
        if(ui->btnGuardar->isEnabled())
        {
            ui->blink_stack->setCurrentIndex(0);
            ui->blinkink->setVisible(false);
            ui->blinkink->setcolor(QColor (0,190,0));
            ui->blinkink->setblink(false);
        }
        else
        {
            ui->blinkink->setVisible(false);
            ui->blink_stack->setCurrentIndex(0);
        }
    }
}

void frmClientes::on_tabla_busquedas_doubleClicked(const QModelIndex &index)
{
    int id = ui->tabla_busquedas->model()->data(ui->tabla_busquedas->model()->index(index.row(),0),Qt::EditRole).toInt();
    oCliente->Recuperar(id);
    LLenarCampos();
    ocultarBusqueda();
    ui->stackedWidget->setCurrentIndex(0);
}

void frmClientes::formato_tabla_busquedas()
{
    ui->tabla_busquedas->setColumnHidden(0,true);
    QList<int> variant;
    variant << 20 << 80 <<230 <<100 <<140 <<140 <<90 <<90 <<120;

    for(int pos= 0; pos <variant.count();pos++)
            ui->tabla_busquedas->setColumnWidth(pos,variant.at(pos));
}

void frmClientes::filter_table(QString texto, QString orden, QString modo)
{
    QString index = h_Buscar.value(orden);

    if(ui->stackedWidget->currentIndex() ==0)
        ui->stackedWidget->setCurrentIndex(1);
    if(modo == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";
    QString cSQL = "select id, codigo_cliente,nombre_fiscal,cif_nif, direccion1, poblacion,telefono1,movil,email from clientes"
            " where "+index+" like '%"+texto.trimmed()+"%' order by "+index+" "+modo;
    m_clientes->setQuery(cSQL,Configuracion_global->groupDB);

    //formato_tabla_busquedas();

    ui->tabla_busquedas->selectRow(0);

}

void frmClientes::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);    

    QStringList orden;
    orden  <<  tr("Nombre Fiscal") << tr("Cif / Nif") << tr("Código cliente") << tr("Población");
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

    //QPushButton* print = new QPushButton(QIcon(":/Icons/PNG/print2.png"),tr("Imprimir"),this);
    //TODO print button @Clientes
    //m_busqueda->addWidget(print);

    QPushButton* del = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    connect(del,SIGNAL(clicked()),this,SLOT(on_btnBorrar_clicked()));
    m_busqueda->addWidget(del);

    m_busqueda->addSpacer();

    QPushButton* exec = new QPushButton(QIcon(":/Icons/PNG/excepciones.png"),tr("Excepciones"),this);
    connect(exec,SIGNAL(clicked()),this,SLOT(btnExcepciones_clicked()));
    m_busqueda->addWidget(exec);
    connect(m_busqueda,SIGNAL(key_Down_Pressed()),ui->tabla_busquedas,SLOT(setFocus()));
    //connect(m_busqueda,SIGNAL(key_F1_Pressed()),this,SLOT(ocultarBusqueda()));
}

bool frmClientes::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Show && obj == this)
    {
        if(!__init)
            init();
        init_querys();
    }

    if(event->type() == QEvent::Resize)
    {
        if(!__init)
            init();
        _resizeBarraBusqueda(m_busqueda);
    }
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(ui->stackedWidget->currentIndex()==1)
        {
            if(keyEvent->key() == Qt::Key_Return)
            {
                if(ui->tabla_busquedas->currentIndex().isValid())
                    on_tabla_busquedas_doubleClicked(ui->tabla_busquedas->currentIndex());
                return true;
            }
        }
        if(keyEvent->key() == Qt::Key_Escape)
            return true;
        if(keyEvent->key() == Qt::Key_F1)
        {
            if(ui->btnEditar->isEnabled())
            {
                if(m_busqueda->isShow())
                    ocultarBusqueda();
                else
                    mostrarBusqueda();
            }
        }
    }
    return MayaModule::eventFilter(obj,event);
}

void frmClientes::llenar_tipoCliente()
{
    if(!__init)
        return;
    ui->lista_tipos->clear();
    QString query = QString("SELECT id_tipo_cliente as id,tipocliente_def.nombre as tipo, tiposubcliente_def.nombre as subtipo "
                    "FROM `tipocliente` "
                    "INNER JOIN `tipocliente_def` on tipocliente.id_tipo_cliente = tipocliente_def.id "
                    "INNER JOIN `tiposubcliente_def` on tipocliente.id_subtipo_cliente = tiposubcliente_def.id "
                    "where id_cliente = %1 ;").arg(oCliente->id);
    QSqlQuery q(Configuracion_global->groupDB);
    if(q.exec(query))
    {
        QMap<int,QString> tipos;
        QMap<int,QStringList> subtipos;
        while(q.next())
        {
            QSqlRecord r = q.record();
            tipos[r.value("id").toInt()] = r.value("tipo").toString();
            subtipos[r.value("id").toInt()].append(r.value("subtipo").toString());
        }
        QMapIterator<int,QStringList> _it(subtipos);
        while(_it.hasNext())
        {
            _it.next();
            int id = _it.key();
            QTreeWidgetItem * root = new QTreeWidgetItem(ui->lista_tipos);
            root->setText(0,tipos.value(id));
            QStringListIterator _list_it(_it.value());
            while(_list_it.hasNext())
            {
                QTreeWidgetItem * child = new QTreeWidgetItem(root);
                child->setText(0,_list_it.next());
                root->addChild(child);
            }
            ui->lista_tipos->addTopLevelItem(root);
        }
    }
    ui->lista_tipos->expandAll();
}

void frmClientes::on_btnGuardardireccionAlternativa_clicked()
{
    if(ui->cbopaisAlternativa->currentText().isEmpty())
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Debe rellenar el campo pais para poder guardar"));
    }
    else
    {
        ui->txtdescripcion_direccion->setEnabled(false);
        ui->txtdireccion1Alternativa1->setEnabled(false);
        ui->txtdireccion1Alternativa2->setEnabled(false);
        ui->txtcpPoblacionAlternativa->setEnabled(false);
        ui->txtpoblacionAlternativa->setEnabled(false);
        ui->txtprovinciaAlternativa->setEnabled(false);
        ui->cbopaisAlternativa->setEnabled(false);
        ui->txtemail_alternativa->setEnabled(false);
        ui->txtcomentarios_alternativa->setEnabled(false);

        if (Anadirdireccion)
            oCliente->Guardardireccion(true,ui->txtdescripcion_direccion->text(),ui->txtdireccion1Alternativa1->text(),
                                   ui->txtdireccion1Alternativa2->text(),ui->txtcpPoblacionAlternativa->text(),
                                   ui->txtpoblacionAlternativa->text(),ui->txtprovinciaAlternativa->text(),
                                   ui->cbopaisAlternativa->currentText(),oCliente->id,ui->txtemail_alternativa->text(),
                                   ui->txtcomentarios_alternativa->toPlainText(),0);
        else
            oCliente->Guardardireccion(false,ui->txtdescripcion_direccion->text(),ui->txtdireccion1Alternativa1->text(),
                                   ui->txtdireccion1Alternativa2->text(),ui->txtcpPoblacionAlternativa->text(),
                                   ui->txtpoblacionAlternativa->text(),ui->txtprovinciaAlternativa->text(),
                                   ui->cbopaisAlternativa->currentText(),oCliente->id,ui->txtemail_alternativa->text(),
                                   ui->txtcomentarios_alternativa->toPlainText(),iddireccionAlternativa);
        Anadirdireccion = false;
        //-----------------
        // direcciones
        //-----------------

        qModeldireccion->setQuery("select * from cliente_direcciones where id_cliente = "+QString::number(oCliente->id),
                                  Configuracion_global->groupDB);

        ui->btnGuardar->setEnabled(true);
        ui->btnDeshacer->setEnabled(true);
        ui->btnGuardardireccionAlternativa->setEnabled(false);
        ui->btnDeshacerdireccionAlternativa->setEnabled(false);
        ui->btnAnadirdireccion->setEnabled(true);
        ui->btnBorrardireccion->setEnabled(true);
        ui->btnEditardireccionAlternativa->setEnabled(true);
    }
}

void frmClientes::on_btnAnadirdireccion_clicked()
{
    Anadirdireccion = true;
    ui->txtdescripcion_direccion->setEnabled(true);
    ui->txtdescripcion_direccion->setText("");
    ui->txtdireccion1Alternativa1->setEnabled(true);
    ui->txtdireccion1Alternativa1->setText("");
    ui->txtdireccion1Alternativa2->setEnabled(true);
    ui->txtdireccion1Alternativa2->setText("");
    ui->txtcpPoblacionAlternativa->setEnabled(true);
    ui->txtcpPoblacionAlternativa->setText("");
    ui->txtpoblacionAlternativa->setText("");
    ui->txtpoblacionAlternativa->setEnabled(true);
    ui->txtprovinciaAlternativa->setText("");
    ui->txtprovinciaAlternativa->setEnabled(true);
    ui->txtemail_alternativa->clear();
    ui->txtcomentarios_alternativa->clear();
    ui->cbopaisAlternativa->setEnabled(true);
    ui->btnGuardardireccionAlternativa->setEnabled(true);
    ui->btnDeshacerdireccionAlternativa->setEnabled(true);
    ui->btnAnadirdireccion->setEnabled(false);
    ui->btnBorrardireccion->setEnabled(false);
    ui->txtdescripcion_direccion->setFocus();
    ui->btnGuardardireccionAlternativa->setEnabled(true);
    ui->btnDeshacerdireccionAlternativa->setEnabled(true);
    ui->btnAnadirdireccion->setEnabled(false);
    ui->btnBorrardireccion->setEnabled(false);
    ui->btnGuardar->setEnabled(false);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditardireccionAlternativa->setEnabled(false);
    ui->txtemail_alternativa->setEnabled(true);
    ui->txtcomentarios_alternativa->setEnabled(true);
    ui->btnAnadirdireccion->setEnabled(false);
    ui->btnBorrardireccion->setEnabled(false);
    ui->btnEditardireccionAlternativa->setEnabled(false);
    ui->btnGuardardireccionAlternativa->setEnabled(true);
    ui->btnDeshacerdireccionAlternativa->setEnabled(true);
    ui->txtdescripcion_direccion->setFocus();
}

void frmClientes::btnExcepciones_clicked()
{
    QModelIndex index = ui->tabla_busquedas->currentIndex();
    int id = ui->tabla_busquedas->model()->data(ui->tabla_busquedas->model()->index(index.row(),0),Qt::EditRole).toInt();
    oCliente->Recuperar(id);
    FrmExcepciones excepciones(this);
    excepciones.cargar_articulo(id);
    excepciones.exec();
}

void frmClientes::mostrarBusqueda()
{
    ui->stackedWidget->setCurrentIndex(1);
    _showBarraBusqueda(m_busqueda);
    m_busqueda->doFocustoText();

}

void frmClientes::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}

void frmClientes::on_btnCobroTotal_clicked()
{
    QModelIndex index;
    index = ui->TablaDeudas->currentIndex();
    int id= ui->TablaDeudas->model()->index(index.row(),0).data().toInt();
    int id_factura,id_ticket;
    double pendiente_cobro;

    QMap <int,QSqlRecord> d;
    QString error;
    d = SqlCalls::SelectRecord("clientes_deuda",QString("id= %1 and id_empresa=%2").arg(id).arg(Configuracion_global->idEmpresa),Configuracion_global->groupDB,error);
    if(d.count()>0)
    {
        id_ticket = d.value(id).value("id_ticket").toInt();
        id_factura = d.value(id).value("id_factura").toInt();
        pendiente_cobro = d.value(id).value("pendiente_cobro").toDouble();

    }
    FrmGestionCobros2 gc2(this);
    gc2.setId_factura(id_factura);
    gc2.setId_ticket(id_ticket);
    gc2.setImporte(pendiente_cobro,id);
    gc2.exec();
    QString cSQL;
    if(ui->radPendientes->isChecked())
        cSQL= "Select id,documento,importe,pagado,pendiente_cobro,fecha,vencimiento from clientes_deuda where id_cliente = "
                + QString::number(oCliente->id)+" and id_empresa= "+QString::number(Configuracion_global->idEmpresa) + " and pendiente_cobro >0";
    else
        cSQL= "Select id,documento,importe,pagado,pendiente_cobro,fecha,vencimiento from clientes_deuda where id_cliente = "
                + QString::number(oCliente->id)+" and id_empresa= "+QString::number(Configuracion_global->idEmpresa) + " and pendiente_cobro <=0";
    deudas->setQuery(cSQL,Configuracion_global->groupDB);
    on_TablaDeudas_clicked(index);

}

void frmClientes::on_radPendientes_toggled(bool checked)
{
    if(!__init)
        return;
    QString cSQL;
    if(checked)
    {
        cSQL= "Select id,documento,importe,pagado,pendiente_cobro,fecha,vencimiento from clientes_deuda where id_cliente = "
                + QString::number(oCliente->id)+" and id_empresa= "+QString::number(Configuracion_global->idEmpresa) + " and pendiente_cobro >0";
    }else
    {
        cSQL= "Select id,documento,importe,pagado,pendiente_cobro,fecha,vencimiento from clientes_deuda where id_cliente = "
                + QString::number(oCliente->id)+" and id_empresa= "+QString::number(Configuracion_global->idEmpresa) + " and pendiente_cobro <=0";

    }
    deudas->setQuery(cSQL,Configuracion_global->groupDB);
}

void frmClientes::on_TablaDeudas_clicked(const QModelIndex &index)
{
    // ------------------------------
    // Recupero Id desde tabla madre
    //-------------------------------
    int id = ui->TablaDeudas->model()->index(index.row(),0).data().toInt();
    //-------------------------
    // Refresco modelo hijo
    //-------------------------
    modelHistorial->setQuery(QString("select * from histo_clientes_deuda where id_cab = %1 and id_empresa = %").arg(id).arg(Configuracion_global->idEmpresa),
                             Configuracion_global->groupDB);
    QStringList headers;
    headers << "id" <<"id_cab" <<tr("Fecha Movim.") << tr("Imp.Ant.") <<tr("pagado") <<tr("cambio") <<tr("Pendiente");
    headers <<tr("Entidad") << tr("oficina") <<tr("dc") <<tr("cuenta") << tr("efectivo") << tr("tarjeta") ;
    headers << tr("cheque") <<tr("Transf.") << tr("Domic.") <<("PyPal") <<tr("Vales");

    QVariantList sizes;
    sizes << 0 << 0 <<100<<90 <<90 <<90 <<90 <<90 <<90 <<90 <<90 <<90 <<90 << 90 <<90 <<90 <<90 <<90;
    for(int i = 0; i< sizes.size(); i++)
    {
        modelHistorial->setHeaderData(i,Qt::Horizontal,headers.at(i));
        ui->tablahistorial_deudas->setColumnWidth(i,sizes.at(i).toInt());
    }
}

void frmClientes::on_txtcp_editingFinished()
{
    if(!QSqlDatabase::database("calles").isOpen())
        return;
    QString cp = QString("CodPostal = '%1'").arg(ui->txtcp->text());
    pob_completer_model->setFilter(cp);
    pob_completer_model->select();

    calle_completer_model->setFilter(cp);
    calle_completer_model->select();

    if(pob_completer_model->rowCount() > 0)
    {
        ui->txtpoblacion->setText(pob_completer_model->record(0).value("Municipio").toString());
        ui->txtprovincia->setText(pob_completer_model->record(0).value("CodProv").toString());
        ui->cboPais->setCurrentText("España");

        if(pob_completer_model->rowCount() > 1){
            ui->txtpoblacion->setText("");
            ui->txtpoblacion->setFocus();
        }
        else
            ui->txtdireccion1->setFocus();
    }
}

void frmClientes::on_txtdireccion2_editingFinished()
{
    if(!ui->txtprovincia->text().isEmpty())
        ui->txttelefono1->setFocus();
}

void frmClientes::on_btnEdita_tipoCliente_clicked()
{
    FrmAddTipoCliente f(this,oCliente->id);
    f.exec();
    llenar_tipoCliente();
}

void frmClientes::on_txtcpPoblacionAlternativa_editingFinished()
{
    if(!QSqlDatabase::database("calles").isOpen())
        return;
    QString cp = QString("CodPostal = '%1'").arg(ui->txtcpPoblacionAlternativa->text());
    pob_completer_model->setFilter(cp);
    pob_completer_model->select();

    calle_completer_model->setFilter(cp);
    calle_completer_model->select();

    if(pob_completer_model->rowCount() > 0)
    {
        ui->txtpoblacionAlternativa->setText(pob_completer_model->record(0).value("Municipio").toString());
        ui->txtprovinciaAlternativa->setText(pob_completer_model->record(0).value("CodProv").toString());
        ui->cbopaisAlternativa->setCurrentText("España");

        if(pob_completer_model->rowCount() > 1){
            ui->txtpoblacionAlternativa->setText("");
            ui->txtpoblacionAlternativa->setFocus();
        }
        else
            ui->txtdireccion1Alternativa1->setFocus();
    }
}

void frmClientes::on_txtdireccion1Alternativa2_editingFinished()
{
    if(!ui->txtprovinciaAlternativa->text().isEmpty())
        ui->txtemail_alternativa->setFocus();
}

void frmClientes::on_btnDeshacerdireccionAlternativa_clicked()
{
    ui->btnAnadirdireccion->setEnabled(true);
    ui->btnEditardireccionAlternativa->setEnabled(true);
    ui->btnBorrardireccion->setEnabled(true);
    ui->btnGuardardireccionAlternativa->setEnabled(false);
    ui->btnDeshacerdireccionAlternativa->setEnabled(false);
    CargardireccionAlternativa(ui->lista_direccionesAlternativas->currentIndex());
}
