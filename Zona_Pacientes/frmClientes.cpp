#include "frmClientes.h"
#include "ui_frmClientes.h"
#include "cliente.h"
#include "../Busquedas/frmbuscarcliente.h"
#include "../Busquedas/frmbuscarpoblacion.h"
#include "../columnafecha.h"
#include "frmfichapaciente.h"
#include "../sqlcalls.h"
#include "paciente.h"
#include "frmaddtipocliente.h"
#include "frmpersonascontactocliente.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/datedelegate.h"
#include "frmcobrardeuda.h"

frmClientes::frmClientes(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmClientes),
    toolButton(tr("Clientes"),":/Icons/PNG/clientes_2.png",this),
    menuButton(QIcon(":/Icons/PNG/clientes_2.png"),tr("Clientes"),this),
      push(new QPushButton(QIcon(":/Icons/PNG/clientes_2.png"),"",this))
{
    ui->setupUi(this);

    if (Configuracion_global->medic)
    {
        toolButton.setText(tr("Pacientes"));
    }

    oCliente = new Cliente(this);
    oCliente->id = 0;
    ui->txtNombreFiscal->setText("");
    if (Configuracion_global->medic){
        push->setToolTip(tr("Gestión del fichero de pacientes/clientes"));
        ui->textoTitulo->setText(tr("Gestión de Pacientes - datos administrativos"));
        menuButton.setText("Gestión\nPacientes");
    }
    else{
        push->setToolTip(tr("Gestión del fichero de clientes"));
        ui->textoTitulo->setText(tr("Gestión de Clientes"));
        menuButton.setText("Gestión\nClientes");
    }
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    ValidarCC();
    //oCliente->Actualizar_de_web();
    // Rellenar formas de pago
    modelFP = new QSqlQueryModel();
    modelFP->setQuery("Select forma_pago,id from formpago",QSqlDatabase::database("Maya"));

    // Ocultar campos según configuración
    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
    int nNivel = settings.value("nNivelAcceso").toInt();
    //QString cCategoria = settings.value("cCategoria").toString();

    if (nNivel >7 && Configuracion_global->medic == true)
        ui->btnFichaPaciente->setVisible(true);
    else
        ui->btnFichaPaciente->setVisible(false);
    //Configuracion_global->CargarDatos();
    if(Configuracion_global->irpf)
        ui->chkClienteEmpresa->setVisible(true);
    else
        ui->chkClienteEmpresa->setVisible(false);

    //FP
    ui->cboforma_pago->setModel(modelFP);
    //ui->tableView_5->setModel(model);

    // Rellenar Tarifas:
    QSqlQueryModel *qTarifa = new QSqlQueryModel(this);
    qTarifa->setQuery("select descripcion, id from codigotarifa",QSqlDatabase::database("Maya"));
    ui->cbotarifa_cliente->setModel(qTarifa);

    // Rellenar Paises:
    ui->cboPais->setModel(Configuracion_global->paises_model);
    ui->cboPais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    ui->cbopaisAlternativa->setModel(Configuracion_global->paises_model);
    ui->cbopaisAlternativa->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    // rellenar combo idiomas
    QSqlQueryModel *qmidiomas = new QSqlQueryModel(this);
    qmidiomas->setQuery("select idioma from idiomas order by idioma", QSqlDatabase::database("Maya"));
    ui->cboidiomaDocumentos->setModel(qmidiomas);
    //oCliente->Recuperar("Select * from clientes");
    //LLenarCampos();

    bloquearCampos();
    this->Altas = false;
    ui->blinkink->setVisible(false);
    //Connect signals /slots.

    connect(ui->txtPrimerApellido,SIGNAL(editingFinished()),this,SLOT(txtPrimerApellido_editingFinished()));
    connect(ui->txtSegundoApellido,SIGNAL(editingFinished()),this,SLOT(txtSegundoApellido_editingFinished()));
    connect(ui->txtnombre,SIGNAL(editingFinished()),this,SLOT(txtnombre_editingFinished()));
    connect(ui->txtpoblacion,SIGNAL(editingFinished()),this,SLOT(txtpoblacion_editingFinished()));
    connect(ui->txtprovincia,SIGNAL(editingFinished()),this,SLOT(txtprovincia_editingFinished()));
    connect(ui->txtcif_nif,SIGNAL(editingFinished()),this,SLOT(txtcif_nif_editingFinished()));
    connect(ui->txtcp,SIGNAL(editingFinished()),this,SLOT(txtcp_editingFinished()));
    connect(ui->txtrRiesgoPermitido,SIGNAL(editingFinished()),this,SLOT(txtrRiesgoPermitido_editingFinished()));
    connect(ui->btnGuardardireccionAlternativa,SIGNAL(clicked()),this,SLOT(GuardardireccionAlternativa()));
    connect(ui->btnDeshacerdireccionAlternativa,SIGNAL(clicked()),this,SLOT(DeshacerdireccionAlternativa()));
    connect(ui->lista_direccionesAlternativas,SIGNAL(clicked(QModelIndex)),this,SLOT(CargardireccionAlternativa(QModelIndex)));
    connect(ui->btnVer_OtrosContactos,SIGNAL(clicked()),this,SLOT(Contactos()));
    connect(ui->txtcpPoblacionAlternativa,SIGNAL(editingFinished()),this,SLOT(txtcpAlternativa_editingFinished()));
    connect(ui->txtpoblacionAlternativa,SIGNAL(editingFinished()),this,SLOT(txtpoblacionAlternativa_editingFinished()));
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
    ui->TablaDeudas->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->TablaDeudas,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(menu_deudas(QPoint)));


}

frmClientes::~frmClientes()
{
    delete ui;

    delete modelFP;
    //delete modelFacturas;
    //delete modelPoblaciones;
    //delete llamadasSQL;
    //FIXME: borrar cando se use realmente...si no se usan suprimirlos
}
void frmClientes::LLenarCampos()
{


    if (oCliente->codigo_cliente.isEmpty())
        oCliente->codigo_cliente = oCliente->Nuevocodigo_cliente();
    ui->txtcodigo_cliente->setText(oCliente->codigo_cliente);
    ui->txtPrimerApellido->setText(oCliente->apellido1);
    ui->txtSegundoApellido->setText(oCliente->apellido2);
    ui->txtnombre->setText(oCliente->nombre);
    ui->txtnombre_fiscal->setText(oCliente->nombre_fiscal);;
    ui->txtNombreFiscal->setText(oCliente->nombre_fiscal);;
    ui->txtnombre_comercial->setText(oCliente->nombre_comercial);
    ui->txtcif_nif->setText(oCliente->cif_nif);
    ui->txtCifIntracomunitario->setText(oCliente->cifVies);
    ui->txtdireccion1->setText(oCliente->direccion1);
    ui->txtdireccion2->setText(oCliente->direccion2);
    ui->txtcp->setText(oCliente->cp);
    ui->txtpoblacion->setText(oCliente->poblacion);
    ui->txtprovincia->setText(oCliente->provincia);
    int nindex = ui->cboPais->findText(oCliente->RecuperarPais(oCliente->id_pais));
    if (nindex >-1)
        ui->cboPais->setCurrentIndex(nindex);
    ui->txttelefono1->setText(oCliente->telefono1);
    ui->txttelefono2->setText(oCliente->telefono2);
    ui->txtfax->setText(oCliente->fax);
    ui->txtmovil->setText(oCliente->movil);
    ui->txtemail->setText(oCliente->email);
    ui->txtweb->setText(oCliente->web);
    ui->txtfecha_alta->setDate(oCliente->fecha_alta);
    ui->txtfecha_ultima_compra->setDate(oCliente->fechaCompra);
    ui->txtimporteAcumulado->setText(Configuracion_global->toFormatoMoneda(QString::number( oCliente->acumulado_ventas)));
    ui->txtventas_ejercicio->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente->ventas_ejercicio)));
    ui->txtrRiesgoPermitido->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente->riesgo_maximo)));
    ui->txtdeuda_actual->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente->deuda_actual)));
    ui->txtcomentarios->setText(oCliente->comentarios);


    if(oCliente->bloqueado)
        ui->chklBloqueoCliente->setChecked(true);
    else
        ui->chklBloqueoCliente->setChecked(false);

    ui->txtcomentario_bloqueo->setText(oCliente->comentario_bloqueo);
    ui->txtporc_dto_cliente->setText(QString::number(oCliente->porc_dto_cliente));

    if(oCliente->recargo_equivalencia)
        ui->chkrecargo_equivalencia->setChecked(true);
    else
        ui->chkrecargo_equivalencia->setChecked(false);

    ui->txtcuenta_contable->setText(oCliente->cuenta_contable);
    ui->txtcuenta_iva_repercutido->setText(oCliente->cuenta_iva_repercutido);
    ui->txtcuenta_deudas->setText(oCliente->cuenta_deudas);
    ui->txtcuenta_cobros->setText(oCliente->cuenta_cobros);
    int indice=ui->cboforma_pago->findText(oCliente->forma_pago);

    if(indice!=-1)
        ui->cboforma_pago->setCurrentIndex(indice);

    //ui->cboforma_pago->setItemText);
    ui->txtdia_pago1->setValue(oCliente->dia_pago1);
    ui->txtdia_pago2->setValue(oCliente->dia_pago2);
   // ui->cbotarifa_cliente->lineEdit)->setText(oCliente->tarifa_cliente));
    ui->txtimporte_a_cuenta->setText( Configuracion_global->toFormatoMoneda(QString::number(oCliente->importe_a_cuenta,'f',2)));
    ui->txtvales->setText(Configuracion_global->toFormatoMoneda(QString::number(oCliente->vales,'f',2)));
    ui->txtentidad_bancaria->setText(oCliente->entidad_bancaria);
    ui->txtoficina_bancaria->setText(oCliente->oficina_bancaria);
    ui->txtdc->setText(oCliente->dc);
    ui->txtcuenta_corriente->setText(oCliente->cuenta_corriente);
    ui->txtfecha_nacimiento->setDate(oCliente->fecha_nacimiento);
    ui->txtacceso_web->setText(oCliente->acceso_web);
    ui->txtpassword_web->setText(oCliente->password_web);
    if (oCliente->lIRPF)
        ui->chkClienteEmpresa->setChecked(true);
    else
        ui->chkClienteEmpresa->setChecked(false);
    indice = ui->cboidiomaDocumentos->findText(oCliente->idioma);
    if(indice >-1)
        ui->cboidiomaDocumentos->setCurrentIndex(indice);
    else
        ui->cboidiomaDocumentos->setCurrentIndex(-1);
    ValidarCC();

    // Tablas
    //----------------
    // DEUDAS
    //----------------

    QSqlQueryModel *deudas = new QSqlQueryModel(this);
    QString cSQL;
    cSQL= "Select id,documento,importe,pagado,pendiente_cobro,fecha,vencimiento from clientes_deuda where id_cliente = "
            + QString::number(oCliente->id);
    deudas->setQuery(cSQL,QSqlDatabase::database("Maya"));
    ui->TablaDeudas->setModel(deudas);
    ui->TablaDeudas->setColumnHidden(0,true);
    deudas->setHeaderData(1,Qt::Horizontal,tr("Documento"));
    deudas->setHeaderData(2,Qt::Horizontal,tr("Importe"));
    ui->TablaDeudas->setItemDelegateForColumn(2,new MonetaryDelegate(this,false));
    deudas->setHeaderData(3,Qt::Horizontal,tr("Pagado"));
    ui->TablaDeudas->setItemDelegateForColumn(3,new MonetaryDelegate(this,false));
    deudas->setHeaderData(4,Qt::Horizontal,tr("Pendiente"));
    ui->TablaDeudas->setItemDelegateForColumn(4,new MonetaryDelegate(this,false));
    deudas->setHeaderData(5,Qt::Horizontal,tr("Fecha"));
    ui->TablaDeudas->setItemDelegateForColumn(5,new DateDelegate);
    deudas->setHeaderData(6,Qt::Horizontal,tr("Vencimiento"));
    ui->TablaDeudas->setItemDelegateForColumn(6,new DateDelegate);

    /********************************************************************
    * ALBARANES
    *******************************************************************/

    QSqlQueryModel *Albaranes = new QSqlQueryModel(this);
    cSQL= "Select id,albaran,fecha,nFactura,total_albaran from cab_alb where id_cliente =" + QString::number(oCliente->id);
    Albaranes->setQuery(cSQL,QSqlDatabase::database("empresa"));
    ui->TablaAlbaranes->setModel(Albaranes);
    ui->TablaAlbaranes->setItemDelegateForColumn(2,new DateDelegate);
    //  ui->TablaAlbaranes->setItemDelegateForColumn(4,columnaMoneda);
    //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
    QHeaderView *CabeceraAlb = new QHeaderView(Qt::Horizontal,this);
    // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
    ui->TablaAlbaranes->setHorizontalHeader(CabeceraAlb);
    /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0".  */
    // CabeceraAlb->setSectionResizeMode(0,QHeaderView::Fixed);
    CabeceraAlb->resizeSection(0,0);
    //  CabeceraAlb->setSectionResizeMode(1,QHeaderView::Fixed);
    CabeceraAlb->resizeSection(1,85);
    // CabeceraAlb->setSectionResizeMode(2,QHeaderView::Fixed);
    CabeceraAlb->resizeSection(2,85);
    // CabeceraAlb->setSectionResizeMode(3,QHeaderView::Fixed);
    CabeceraAlb->resizeSection(3,85);
    // CabeceraAlb->setSectionResizeMode(4,QHeaderView::Fixed);
    CabeceraAlb->resizeSection(4,85);
    Albaranes->setHeaderData(1, Qt::Horizontal, QObject::tr("ALBARAN"));
    Albaranes->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
    Albaranes->setHeaderData(3, Qt::Horizontal, QObject::tr("FACTURA"));
    Albaranes->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));
    CabeceraAlb->setVisible(true);

    /********************************************************************
   * FACTURAS
   *******************************************************************/

    QSqlQueryModel *Facturas = new QSqlQueryModel(this);
    cSQL= "Select id,factura,fecha,fecha_cobro,total from cab_fac where id_cliente =" + QString::number(oCliente->id);
    Facturas->setQuery(cSQL,QSqlDatabase::database("empresa"));
    ui->tablaFacturas->setModel(Facturas);
    ui->tablaFacturas->setItemDelegateForColumn(2,new DateDelegate);
    ui->tablaFacturas->setItemDelegateForColumn(3,new DateDelegate);
    //  ui->tablaFacturas->setItemDelegateForColumn(4,columnaMoneda);
    //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
    QHeaderView *CabeceraFac = new QHeaderView(Qt::Horizontal,this);
    // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
    ui->tablaFacturas->setHorizontalHeader(CabeceraFac);
    /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0".  */
    // CabeceraFac->setSectionResizeMode(0,QHeaderView::Fixed);
    CabeceraFac->resizeSection(0,0);
    // CabeceraFac->setSectionResizeMode(1,QHeaderView::Fixed);
    CabeceraFac->resizeSection(1,85);
    // CabeceraFac->setSectionResizeMode(2,QHeaderView::Fixed);
    CabeceraFac->resizeSection(2,85);
    // CabeceraFac->setSectionResizeMode(3,QHeaderView::Fixed);
    CabeceraFac->resizeSection(3,85);
    // CabeceraFac->setSectionResizeMode(4,QHeaderView::Fixed);
    CabeceraFac->resizeSection(4,85);
    Facturas->setHeaderData(1, Qt::Horizontal, QObject::tr("FACTURA"));
    Facturas->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
    Facturas->setHeaderData(3, Qt::Horizontal, QObject::tr("FECHA COBRO"));
    Facturas->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));
    CabeceraFac->setVisible(true);

  /********************************************************************
  * PRESUPUESTOS
  *******************************************************************/

    QSqlQueryModel *Presupuestos = new QSqlQueryModel(this);

    cSQL= "Select id,nPresupuesto,fecha,dValidoHasta,total from cab_pre where id_cliente =" + QString::number(oCliente->id);
    Presupuestos->setQuery(cSQL,QSqlDatabase::database("empresa"));
    ui->tablaPresupuestos->setModel(Presupuestos);
    ui->tablaPresupuestos->setItemDelegateForColumn(2,new MonetaryDelegate(this,false));
    ui->tablaPresupuestos->setItemDelegateForColumn(3, new MonetaryDelegate(this,false));
    //ui->tablaPresupuestos->setItemDelegateForColumn(4,columnaMoneda);
    //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
    QHeaderView *CabeceraPre = new QHeaderView(Qt::Horizontal,this);
    // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
    ui->tablaPresupuestos->setHorizontalHeader(CabeceraPre);
    /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0".  */
    //CabeceraPre->setSectionResizeMode(0,QHeaderView::Fixed);
    CabeceraPre->resizeSection(0,0);
    //CabeceraPre->setSectionResizeMode(1,QHeaderView::Fixed);
    CabeceraPre->resizeSection(1,85);
    //CabeceraPre->setSectionResizeMode(2,QHeaderView::Fixed);
    CabeceraPre->resizeSection(2,85);
    //CabeceraPre->setSectionResizeMode(3,QHeaderView::Fixed);
    CabeceraPre->resizeSection(3,85);
    //CabeceraPre->setSectionResizeMode(4,QHeaderView::Fixed);
    CabeceraPre->resizeSection(4,85);
    //CabeceraPre->set
    Presupuestos->setHeaderData(1, Qt::Horizontal, QObject::tr("PRES"));
    Presupuestos->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
    Presupuestos->setHeaderData(3, Qt::Horizontal, QObject::tr("FECHA APRO."));
    Presupuestos->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));
    CabeceraPre->setVisible(true);


 /********************************************************************
 * VALES
 *******************************************************************/

    QSqlQueryModel *Vales = new QSqlQueryModel(this);

    cSQL= "Select id,nNumero,fecha,dVto,importe from vales where id_cliente =" + QString::number(oCliente->id);
    Vales->setQuery(cSQL,QSqlDatabase::database("Maya"));
    ui->tablaVales->setModel(Vales);
    ui->tablaVales->setItemDelegateForColumn(2,new DateDelegate);
    ui->tablaVales->setItemDelegateForColumn(3,new DateDelegate);
    //ui->tablaVales->setItemDelegateForColumn(4,columnaMoneda);
    //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
    QHeaderView *CabeceraVal = new QHeaderView(Qt::Horizontal,this);
    // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
    ui->tablaVales->setHorizontalHeader(CabeceraVal);
    /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0".  */
    //CabeceraVal->setSectionResizeMode(0,QHeaderView::Fixed);
    CabeceraVal->resizeSection(0,0);
    //CabeceraVal->setSectionResizeMode(1,QHeaderView::Fixed);
    CabeceraVal->resizeSection(1,85);
    //CabeceraVal->setSectionResizeMode(2,QHeaderView::Fixed);
    CabeceraVal->resizeSection(2,85);
    //CabeceraVal->setSectionResizeMode(3,QHeaderView::Fixed);
    CabeceraVal->resizeSection(3,85);
    //CabeceraVal->setSectionResizeMode(4,QHeaderView::Fixed);
    CabeceraVal->resizeSection(4,85);
    Vales->setHeaderData(1, Qt::Horizontal, QObject::tr("VALE"));
    Vales->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
    Vales->setHeaderData(3, Qt::Horizontal, QObject::tr("VTO."));
    Vales->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));
    CabeceraVal->setVisible(true);

    /********************************************************************
    * TICKETS
    *******************************************************************/

    QSqlQueryModel *Tickets = new QSqlQueryModel(this);

    cSQL= "Select id,ticket,fecha,hora,importe from cab_tpv where id_cliente =" + QString::number(oCliente->id);
    Tickets->setQuery(cSQL,QSqlDatabase::database("empresa"));
    ui->tablaTickets->setModel(Tickets);
    ui->tablaTickets->setItemDelegateForColumn(2,new ColumnaFecha);
    ui->tablaTickets->setItemDelegateForColumn(3,new ColumnaFecha);
    //ui->tablaTickets->setItemDelegateForColumn(4,columnaMoneda);
    //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
    QHeaderView *CabeceraTic = new QHeaderView(Qt::Horizontal,this);
    // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
    ui->tablaTickets->setHorizontalHeader(CabeceraTic);
    /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0".  */
    //CabeceraTic->setSectionResizeMode(0,QHeaderView::Fixed);
    CabeceraTic->resizeSection(0,0);
    //CabeceraTic->setSectionResizeMode(1,QHeaderView::Fixed);
    CabeceraTic->resizeSection(1,85);
    //CabeceraTic->setSectionResizeMode(2,QHeaderView::Fixed);
    CabeceraTic->resizeSection(2,85);
    //CabeceraTic->setSectionResizeMode(3,QHeaderView::Fixed);
    CabeceraTic->resizeSection(3,85);
    //CabeceraTic->setSectionResizeMode(4,QHeaderView::Fixed);
    CabeceraTic->resizeSection(4,85);
    Tickets->setHeaderData(1, Qt::Horizontal, QObject::tr("TICKET"));
    Tickets->setHeaderData(2, Qt::Horizontal, QObject::tr("FECHA"));
    Tickets->setHeaderData(3, Qt::Horizontal, QObject::tr("HORA"));
    Tickets->setHeaderData(4, Qt::Horizontal, QObject::tr("TOTAL"));
    CabeceraTic->setVisible(true);


    //----------------------
    // Asientos contables
    //----------------------
    QSqlQueryModel *modelAsientos = new QSqlQueryModel(this);

    modelAsientos->setQuery("select id,asiento,fecha_asiento,cuenta_d,descripcion_d, importe_d,cuenta_h,descripcion_h,importe_h "
                            "from diario where cta_principal = '"+oCliente->cuenta_contable +
                            "' order by asiento + ' '+pos_en_asiento",
                            QSqlDatabase::database("dbconta"));


    ui->tablaAsientos->setModel(modelAsientos);
    ui->tablaAsientos->setColumnHidden(0,true);

    ui->tablaAsientos->setColumnWidth(1,55);
    ui->tablaAsientos->setColumnWidth(2,63);
    ui->tablaAsientos->setItemDelegateForColumn(2, new DateDelegate);
    ui->tablaAsientos->setColumnWidth(3,55);
    ui->tablaAsientos->setColumnWidth(4,75);
    ui->tablaAsientos->setColumnWidth(5,60);
    ui->tablaAsientos->setItemDelegateForColumn(5, new MonetaryDelegate(this,false));
    ui->tablaAsientos->setColumnWidth(6,55);
    ui->tablaAsientos->setColumnWidth(7,75);
    ui->tablaAsientos->setColumnWidth(8,60);
    ui->tablaAsientos->setItemDelegateForColumn(8, new MonetaryDelegate(this,false));




    //------------------
    // Tipos de clientes
    //------------------
    QSqlQueryModel *qModelTipos = new QSqlQueryModel(this);
    qModelTipos->setQuery("select tipocliente from tipocliente where id_cliente = "+QString::number(oCliente->id),
                          QSqlDatabase::database("Maya"));
    ui->lista_tipos->setModel(qModelTipos);

    //-----------------
    // direcciones
    //-----------------
    QSqlQueryModel *qModeldireccion = new QSqlQueryModel(this);
    qModeldireccion->setQuery("select descripcion,id from cliente_direcciones where id_cliente = "+QString::number(oCliente->id),
                              QSqlDatabase::database("Maya"));
    ui->lista_direccionesAlternativas->setModel(qModeldireccion);
    // -------------------
    // Grafica estadística
    //--------------------
    refrescar_grafica();
    set_blink();
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
    ui->txtdireccion1->setText("");
    ui->txtdireccion2->setText("");
    ui->txtpoblacion->setText("");
    ui->txtprovincia->setText("");
    ui->cboPais->clearEditText();
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


    //ui->cboforma_pago->setItemText();
    ui->txtdia_pago1->setValue(0);
    ui->txtdia_pago2->setValue(0);
   // ui->cbotarifa_cliente->lineEdit()->setText(oCliente->gettarifa_cliente());
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

    //------------------
    // Tipos de clientes
    //------------------
    QSqlQueryModel *qModelTipos = new QSqlQueryModel(this);
    qModelTipos->setQuery("select tipocliente from tipocliente where id_cliente = -1",
                          QSqlDatabase::database("Maya"));
    ui->lista_tipos->setModel(qModelTipos);


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
    oCliente->forma_pago=ui->cboforma_pago->currentText();
    oCliente->dia_pago1=ui->txtdia_pago1->value();
    oCliente->dia_pago2=ui->txtdia_pago2->value();
    oCliente->tarifa_cliente=ui->cbotarifa_cliente->currentText().toDouble();
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
    oCliente->ididioma = Configuracion_global->Devolver_id_idioma(ui->cboidiomaDocumentos->currentText());
    oCliente->idioma = ui->cboidiomaDocumentos->currentText();
    if (ui->chkClienteEmpresa)
        oCliente->lIRPF =true;
    else
        oCliente->lIRPF=false;

    QSqlQuery qTarifa(QSqlDatabase::database("Maya"));
    qTarifa.prepare("select id from codigotarifa where descripcion = '"+ui->cbotarifa_cliente->currentText()+"'");
    if(qTarifa.exec()){
        qTarifa.next();
        oCliente->tarifa_cliente = qTarifa.record().field("id").value().toInt();
    }
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
        bloquearCampos();
        Configuracion_global->CargarClientes();
        emit unblock();
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
    emit block();
    desbloquearCampos();
    VaciarCampos();
    set_blink();
    this->Altas = true;
    ui->txtcodigo_cliente->setText(oCliente->Nuevocodigo_cliente());
    ui->txtcuenta_contable->setText(ui->txtcodigo_cliente->text());
    ui->txtcodigo_cliente->setFocus();
    LLenarCliente();
    ui->cboidiomaDocumentos->setCurrentIndex(1);
    oCliente->Anadir();

}

void frmClientes::txtPrimerApellido_editingFinished()
{
    ui->txtPrimerApellido->setText(ui->txtPrimerApellido->text().toUpper());

    if(!ui->txtPrimerApellido->text().isEmpty()
       && !ui->txtSegundoApellido->text().isEmpty()
       && !ui->txtnombre->text().isEmpty())
    {
        ui->txtnombre_fiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtnombre->text());
        ui->txtNombreFiscal->setText(ui->txtnombre_fiscal->text());
    }
}

void frmClientes::txtSegundoApellido_editingFinished()
{
    ui->txtSegundoApellido->setText(ui->txtSegundoApellido->text().toUpper());

    if(!ui->txtPrimerApellido->text().isEmpty()
        && !ui->txtSegundoApellido->text().isEmpty()
        && !ui->txtnombre->text().isEmpty())
    {
        ui->txtnombre_fiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtnombre->text());
        ui->txtNombreFiscal->setText(ui->txtnombre_fiscal->text());
    }

}

void frmClientes::txtnombre_editingFinished()
{
    ui->txtnombre->setText(ui->txtnombre->text().toUpper());

    if(!ui->txtPrimerApellido->text().isEmpty()
       && !ui->txtSegundoApellido->text().isEmpty()
       && !ui->txtnombre->text().isEmpty())
    {
        ui->txtnombre_fiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtnombre->text());
        ui->txtNombreFiscal->setText(ui->txtnombre_fiscal->text());
    }

}

void frmClientes::txtpoblacion_editingFinished()
{
    ui->txtpoblacion->setText(ui->txtpoblacion->text().toUpper());
    if (ui->txtcp->text().isEmpty() && !ui->txtpoblacion->text().isEmpty())
    {
       FrmBuscarPoblacion BuscarPoblacion;
       BuscarPoblacion.setpoblacion(ui->txtpoblacion->text(),1);
       if(BuscarPoblacion.exec())
       {
        //  BuscarPoblacion.setpoblacion(ui->txtcp->text(),0);
         int nid = BuscarPoblacion.Devolverid();
         qDebug() << "nid: " <<nid;
         if(nid > 0)
         {
             QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
             QString cid;
             cid = QString::number(nid);
             qPoblacion.prepare("select poblacion,cp,provincia from poblaciones where id = :cid");
             qPoblacion.bindValue(":cid",cid);
             if(!qPoblacion.exec())
             {
                 /* qDebug() << qPoblacion.lastQuery();
                 qDebug() << qPoblacion.value(0).toString(); */
                 QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
             }
             else
             {
                 if (qPoblacion.next())
                 {
                     ui->txtpoblacion->setText(qPoblacion.value(0).toString());
                     ui->txtcp->setText(qPoblacion.value(1).toString());
                     ui->txtprovincia->setText(qPoblacion.value(2).toString());
                    // ui->txtpais->setText("ESPAÑA");
                     oCliente->id_pais =oCliente->Buscarid_pais("ESPAÑA");
                 }
             }
         }
       }
      // BuscarPoblacion.close();
    }
}

void frmClientes::txtprovincia_editingFinished()
{
    ui->txtprovincia->setText(ui->txtprovincia->text().toUpper());
}

void frmClientes::txtcif_nif_editingFinished()
{
    if (ui->txtcif_nif->text().length() == 8){
        QString cif = Configuracion::letraDNI(ui->txtcif_nif->text());
        ui->txtcif_nif->setText(cif.toUpper());
   }
}

void frmClientes::on_btnEditar_clicked()
{
    emit block();
        desbloquearCampos();
        ui->txtcodigo_cliente->setEnabled(false);
        set_blink();
        ui->txtcif_nif->setFocus();
}
void frmClientes::bloquearCampos() {

    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    QSpinBox *SpinBox;
    foreach (SpinBox, SpinBoxList) {
        SpinBox->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }

    ui->btnAnadir->setEnabled(true);
    ui->btnAnterior->setEnabled(true);
    ui->btnBorrar->setEnabled(true);
    ui->btnBuscar->setEnabled(true);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);
    ui->btnAnadirdireccion->setEnabled(false);
    ui->btnBorrardireccion->setEnabled(false);
    ui->btnEditardireccionAlternativa->setEnabled(false);
    ui->btnGuardardireccionAlternativa->setEnabled(false);
    ui->btnDeshacerdireccionAlternativa->setEnabled(false);
    ui->btnAdd_TipoCliente->setEnabled(false);
    ui->btndel_TipoCliente->setEnabled(false);
    ui->btnVer_OtrosContactos->setEnabled(true);

}
void frmClientes::desbloquearCampos()
{
    // LineEdit
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    QSpinBox *SpinBox;
    foreach (SpinBox, SpinBoxList) {
        SpinBox->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    ui->btnAnadir->setEnabled(false);
    ui->btnAnterior->setEnabled(false);
    ui->btnBorrar->setEnabled(false);
    ui->btnBuscar->setEnabled(false);
    ui->btnDeshacer->setEnabled(true);
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnSiguiente->setEnabled(false);
    ui->txtdeuda_actual->setEnabled(false);
    ui->txtimporteAcumulado->setEnabled(false);
    ui->txtventas_ejercicio->setEnabled(false);
    ui->btnAnadirdireccion->setEnabled(true);
    ui->btnBorrardireccion->setEnabled(true);
    ui->btnEditardireccionAlternativa->setEnabled(true);
    ui->btnAdd_TipoCliente->setEnabled(true);
    ui->btndel_TipoCliente->setEnabled(true);
    ui->txtdescripcion_direccion->setEnabled(false);
    ui->txtdireccion1Alternativa1->setEnabled(false);
    ui->txtdireccion1Alternativa2->setEnabled(false);
    ui->txtcpPoblacionAlternativa->setEnabled(false);
    ui->txtpoblacionAlternativa->setEnabled(false);
    ui->txtprovinciaAlternativa->setEnabled(false);
    ui->cbopaisAlternativa->setEnabled(false);

}

void frmClientes::on_btnDeshacer_clicked()
{
    if(this->Altas){
        oCliente->Borrar(oCliente->id);
        this->Altas = false;
    }
    QString cid = QString::number(oCliente->id);
    oCliente->Recuperar("Select * from clientes where id ="+cid+" order by id limit 1 ");
    LLenarCampos();
    bloquearCampos();
    set_blink();
    emit unblock();
}

void frmClientes::on_btnBorrar_clicked()
{
    int id_cliente = oCliente->id;
    id_cliente --;

    oCliente->Borrar(oCliente->id);
    VaciarCampos();
}

void frmClientes::on_btnBuscar_clicked()
{
    FrmBuscarCliente BuscarClientes;
    if(BuscarClientes.exec() == QDialog::Accepted)
    {
        int nid = BuscarClientes.Devolverid();
        qDebug() << nid;
        QString cid = QString::number(nid);

        oCliente->Recuperar("Select * from clientes where id ="+cid+" order by id limit 1 ");
        LLenarCampos();
    }
}

void frmClientes::txtcp_editingFinished()
{
    if (!ui->txtcp->text().isEmpty() && ui->txtpoblacion->text().isEmpty())
    {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setpoblacion(ui->txtcp->text(),0);
        if(BuscarPoblacion.exec())
        {

            int nid = BuscarPoblacion.Devolverid();
            //qDebug() <<nid;
            if(nid > 0)
            {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cid;
                cid = QString::number(nid);
                qPoblacion.prepare("select poblacion, cp,provincia from poblaciones where id = :cid");
                qPoblacion.bindValue(":cid",cid);
                if(!qPoblacion.exec())
                {
                    /* qDebug() << qPoblacion.lastQuery();*/
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                }
                else
                {
                    qDebug() << "Poblacion" <<qPoblacion.value(0).toString();
                    qDebug() << qPoblacion.lastQuery() <<" id:= " << cid;
                    if (qPoblacion.next())
                    {
                        ui->txtcp->setText(qPoblacion.value(1).toString());
                        ui->txtpoblacion->setText(qPoblacion.value(0).toString());
                        ui->txtprovincia->setText(qPoblacion.value(2).toString());

                       // ui->txtpais->setText("ESPAÑA");
                        oCliente->id_pais =oCliente->Buscarid_pais("ESPAÑA");
                    }
                }
            }
        }
    }
}


void frmClientes::txtcpAlternativa_editingFinished()
{
    if (!ui->txtcpPoblacionAlternativa->text().isEmpty() && ui->txtpoblacionAlternativa->text().isEmpty())
    {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setpoblacion(ui->txtcpPoblacionAlternativa->text(),0);
        if(BuscarPoblacion.exec())
        {
            //  BuscarPoblacion.setpoblacion(ui->txtcp->text(),0);
            int nid = BuscarPoblacion.Devolverid();
            //qDebug() <<nid;
            if(nid > 0)
            {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cid;
                cid = QString::number(nid);
                qPoblacion.prepare("select poblacion, cp,provincia from poblaciones where id = :cid");
                qPoblacion.bindValue(":cid",cid);
                if(!qPoblacion.exec())
                {
                    // qDebug() << qPoblacion.lastQuery();
                    // qDebug() << qPoblacion.value(0).toString();
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                }
                else
                {
                    //qDebug() << qPoblacion.lastQuery();
                    if (qPoblacion.next())
                    {
                        qDebug() << qPoblacion.value(0).toString();
                        ui->txtpoblacionAlternativa->setText(qPoblacion.value(0).toString());
                        ui->txtprovinciaAlternativa->setText(qPoblacion.value(2).toString());
                        //TODO - poner pais configuración
                       // ui->txtpaisAlternativa->setText("ESPAÑA");
                    }
                }
            }
        }
        // BuscarPoblacion.close();
    }
}

void frmClientes::txtpoblacionAlternativa_editingFinished()
{
    if (ui->txtcpPoblacionAlternativa->text().isEmpty() && !ui->txtpoblacionAlternativa->text().isEmpty())
    {
        ui->txtpoblacionAlternativa->setText(ui->txtpoblacionAlternativa->text().toUpper());
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setpoblacion(ui->txtpoblacionAlternativa->text(),1);
        if(BuscarPoblacion.exec() == QDialog::Accepted)
        {
            //  BuscarPoblacion.setpoblacion(ui->txtcp->text(),0);
            int nid = BuscarPoblacion.Devolverid();
            //qDebug() <<nid;
            if(nid > 0)
            {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cid;
                cid = QString::number(nid);
                qPoblacion.prepare("select poblacion,cp,provincia from poblaciones where id = :cid");
                qPoblacion.bindValue(":cid",cid);
                if(!qPoblacion.exec())
                {
                    // qDebug() << qPoblacion.lastQuery();
                    //qDebug() << qPoblacion.value(0).toString();
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                }
                else
                {
                    // qDebug() << qPoblacion.lastQuery();
                    if (qPoblacion.next())
                    {
                        ui->txtpoblacionAlternativa->setText(qPoblacion.record().value("poblacion").toString());
                        ui->txtcpPoblacionAlternativa->setText(qPoblacion.record().value("cp").toString());
                        ui->txtprovinciaAlternativa->setText(qPoblacion.record().value("provincia").toString());
                        int indice;
                        indice = ui->cbopaisAlternativa->findText(Configuracion_global->pais);
                        ui->cbopaisAlternativa->setCurrentIndex(indice);
                   }
                }
            }
        }
      // BuscarPoblacion.close();
    }
}



void frmClientes::TablaDeudas_clicked(const QModelIndex &index)
{

}

void frmClientes::txtrRiesgoPermitido_editingFinished()
{
    ui->txtrRiesgoPermitido->setText(Configuracion_global->toFormatoMoneda(ui->txtrRiesgoPermitido->text()));
}

void frmClientes::on_btnFichaPaciente_clicked()
{
    //NOTE - no mostar subform si no hay ningun cliente select
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

void frmClientes::AddCustomerType()
{

   FrmAddTipoCliente AddTipoCliente(this);
   if(AddTipoCliente.exec() == QDialog::Accepted){
       QString valor = AddTipoCliente.familiaRetorno;
       QSqlQuery qTipo(QSqlDatabase::database("Maya"));
       qTipo.prepare("INSERT INTO tipocliente (tipocliente,id_cliente) VALUES (:tipo,:id_cliente)");
       qTipo.bindValue(":tipo",valor);
       qTipo.bindValue(":id_cliente",oCliente->id);
       if (!qTipo.exec())
           QMessageBox::warning(this,tr("Añadir tipo cliente"),
                                tr("No se pudo añadir el tipo de cliente :%1").arg(qTipo.lastError().text()),tr("Aceptar"));
       QSqlQueryModel *qModelTipos = new QSqlQueryModel(this);
       qModelTipos->setQuery("select tipocliente from tipocliente where id_cliente = "+QString::number(oCliente->id),
                             QSqlDatabase::database("Maya"));

        ui->lista_tipos->setModel(qModelTipos);
   }
}

void frmClientes::AnadirdireccionAlternativa()
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
}

void frmClientes::GuardardireccionAlternativa()
{
    if(ui->cbopaisAlternativa->currentText().isEmpty()){
        TimedMessageBox * t = new TimedMessageBox(this,tr("Debe rellenar el campo pais para poder guardar"));
    } else {
        ui->txtdescripcion_direccion->setEnabled(false);
        ui->txtdireccion1Alternativa1->setEnabled(false);
        ui->txtdireccion1Alternativa2->setEnabled(false);
        ui->txtcpPoblacionAlternativa->setEnabled(false);
        ui->txtpoblacionAlternativa->setEnabled(false);
        ui->txtprovinciaAlternativa->setEnabled(false);
        ui->cbopaisAlternativa->setEnabled(false);

        if (Anadirdireccion)
            oCliente->Guardardireccion(true,ui->txtdescripcion_direccion->text(),ui->txtdireccion1Alternativa1->text(),
                                   ui->txtdireccion1Alternativa2->text(),ui->txtcpPoblacionAlternativa->text(),
                                   ui->txtpoblacionAlternativa->text(),ui->txtprovinciaAlternativa->text(),
                                   ui->cbopaisAlternativa->currentText(),oCliente->id,NULL);
        else
            oCliente->Guardardireccion(false,ui->txtdescripcion_direccion->text(),ui->txtdireccion1Alternativa1->text(),
                                   ui->txtdireccion1Alternativa2->text(),ui->txtcpPoblacionAlternativa->text(),
                                   ui->txtpoblacionAlternativa->text(),ui->txtprovinciaAlternativa->text(),
                                   ui->cbopaisAlternativa->currentText(),oCliente->id,iddireccionAlternativa);
        Anadirdireccion = false;
        //-----------------
        // direcciones
        //-----------------
        QSqlQueryModel *qModeldireccion = new QSqlQueryModel(this);
        qModeldireccion->setQuery("select descripcion,id from cliente_direcciones where id_cliente = "+QString::number(oCliente->id),
                                  QSqlDatabase::database("Maya"));
        ui->lista_direccionesAlternativas->setModel(qModeldireccion);
        ui->btnGuardar->setEnabled(true);
        ui->btnDeshacer->setEnabled(true);
        ui->btnGuardardireccionAlternativa->setEnabled(false);
        ui->btnDeshacerdireccionAlternativa->setEnabled(false);
        ui->btnAnadirdireccion->setEnabled(true);
        ui->btnBorrardireccion->setEnabled(true);
        ui->btnEditardireccionAlternativa->setEnabled(true);
    }
}

void frmClientes::DeshacerdireccionAlternativa()
{
    //-----------------
    // direcciones
    //-----------------
    qModeldireccion = new QSqlQueryModel(this);
    qModeldireccion->setQuery("select descripcion,id from cliente_direcciones where id_cliente = "+QString::number(oCliente->id),
                              QSqlDatabase::database("Maya"));
    ui->lista_direccionesAlternativas->setModel(qModeldireccion);
    ui->btnGuardar->setEnabled(true);
    ui->btnDeshacer->setEnabled(true);
    ui->btnGuardardireccionAlternativa->setEnabled(false);
    ui->btnDeshacerdireccionAlternativa->setEnabled(false);
    ui->btnEditardireccionAlternativa->setEnabled(true);

    QSqlQuery diralt(QSqlDatabase::database("Maya"));
    diralt.prepare("select * from cliente_direcciones where id = :id");
    diralt.bindValue(":id",iddireccionAlternativa);
    if(diralt.exec()){
        diralt.next();
        ui->txtdescripcion_direccion->setText(diralt.record().value("descripcion").toString());
        ui->txtdireccion1Alternativa1->setText(diralt.record().value("direccion1").toString());
        ui->txtdireccion1Alternativa2->setText(diralt.record().value("direccion2").toString());
        ui->txtcpPoblacionAlternativa->setText(diralt.record().value("cp").toString());
        ui->txtpoblacionAlternativa->setText(diralt.record().value("poblacion").toString());
        ui->txtprovinciaAlternativa->setText(diralt.record().value("provincia").toString());
        int indice;
        indice = ui->cbopaisAlternativa->findText(diralt.record().value("pais").toString());
        ui->cbopaisAlternativa->setCurrentIndex(indice);

    }

}



void frmClientes::BorrardireccionAlternativa()
{
    if(QMessageBox::question(this,tr("direcciones alternativas"),tr("va a borrar una dirección alternativa\n"
                                                                    "¿desea continuar?"),
                             tr("Cancelar"),tr("Borrar"))==QMessageBox::Accepted)
    {
        QSqlQuery qdireccion(QSqlDatabase::database("Maya"));
        if(!qdireccion.exec("delete from cliente_direcciones where id="+QString::number(iddireccionAlternativa)))
            QMessageBox::warning(this,tr("direcciones Alternativas"),tr("No se ha podido borrar la direccion"),tr("Aceptar"));

    }
    QString cSQL = "select descripcion,id from cliente_direcciones where id_cliente = "+QString::number(oCliente->id);
    qModeldireccion = new QSqlQueryModel();
    qModeldireccion->setQuery(cSQL,QSqlDatabase::database("Maya"));
    ui->lista_direccionesAlternativas->setModel(qModeldireccion);
    ui->txtdescripcion_direccion->setText("");
    ui->txtdireccion1Alternativa1->setText("");
    ui->txtdireccion1Alternativa2->setText("");
    ui->txtcpPoblacionAlternativa->setText("");
    ui->txtpoblacionAlternativa->setText("");
    ui->txtprovinciaAlternativa->setText("");
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
    QSqlQueryModel* modelo = (QSqlQueryModel*)ui->lista_direccionesAlternativas->model();
    int nid = modelo->record(index.row()).value("id").toInt();
    QSqlQuery qdirecciones(QSqlDatabase::database("Maya"));
    qdirecciones.prepare("select * from cliente_direcciones where id = :id");
    qdirecciones.bindValue(":id",nid);
    if(qdirecciones.exec()){
        qdirecciones.next();
        iddireccionAlternativa = nid;
        ui->txtdescripcion_direccion->setText(qdirecciones.record().field("descripcion").value().toString());
        ui->txtdireccion1Alternativa1->setText(qdirecciones.record().field("direccion1").value().toString());
        ui->txtdireccion1Alternativa2->setText(qdirecciones.record().field("direccion2").value().toString());
        ui->txtcpPoblacionAlternativa->setText(qdirecciones.record().field("cp").value().toString());
        ui->txtpoblacionAlternativa->setText(qdirecciones.record().field("poblacion").value().toString());
        ui->txtprovinciaAlternativa->setText(qdirecciones.record().field("provincia").value().toString());
        QString pais = oCliente->RecuperarPais(qdirecciones.record().field("id_pais").value().toInt());
        int nIndex = ui->cbopaisAlternativa->findText(pais);
        if (nIndex <-1)
            ui->cbopaisAlternativa->setCurrentIndex(nIndex);

    }
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
}

void frmClientes::cobrar_deuda()
{
    frmCobrarDeuda cobrard(this);
    cobrard.exec();
}

void frmClientes::cobrar_fraccion()
{

}

void frmClientes::ver_asiento()
{

}

void frmClientes::refrescar_grafica()
{
    ui->graficaEstadistica->Clear();
    ui->graficaEstadistica->setTipo(OpenChart::BarraMultiple);
    ui->graficaEstadistica->addMulibarColor("Compras",Qt::darkRed);
    ui->graficaEstadistica->addMulibarColor("Ventas",Qt::darkGreen);
    QVector <float> enero;
    enero << ui->txtEnero->text().toFloat(); //Añadir tantos colores como elementos tenga el vector!

    ui->graficaEstadistica->addItem("Ene",enero);


    QVector <float> febrero;
    febrero <<ui->txtFebrero->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

    ui->graficaEstadistica->addItem("Feb",febrero);

    QVector <float> marzo;
    marzo <<ui->txtMarzo->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

    ui->graficaEstadistica->addItem("Mar",marzo);

    QVector <float> abril;
    abril <<ui->txtAbril->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

    ui->graficaEstadistica->addItem("Abr",abril);

    QVector <float> mayo;
    mayo <<ui->txtMayo->text().toFloat();//Añadir tantos colores como elementos tenga el vector!

    ui->graficaEstadistica->addItem("May",mayo);

    QVector <float> junio;
    junio <<ui->txtJunio->text().toFloat();

    ui->graficaEstadistica->addItem("jun",junio);

    QVector <float> julio;
    julio <<ui->txtjulio->text().toFloat();

    ui->graficaEstadistica->addItem("Jul",julio);

    QVector <float> agosto;
    agosto <<ui->txtAgosto->text().toFloat();
    ui->graficaEstadistica->addItem("Ago",agosto);

    QVector <float> septiembre;
    septiembre <<ui->txtSeptiembre->text().toFloat();
    ui->graficaEstadistica->addItem("Sep",septiembre);

    QVector <float> octubre;
    octubre <<ui->txtOctubre->text().toFloat();
    ui->graficaEstadistica->addItem("Oct",octubre);

    QVector <float> noviembre;
    noviembre <<ui->txtNoviembre->text().toFloat();
    ui->graficaEstadistica->addItem("Nov",noviembre);

    QVector <float> diciembre;
    diciembre <<ui->txtDiciembre->text().toFloat();
    ui->graficaEstadistica->addItem("Dic",diciembre);

    ui->graficaEstadistica->repaint();
}

void frmClientes::set_blink()
{
    bool campos_pendientes = false;
    QString text_bottom;
    if (oCliente->codigo_cliente.isEmpty())
    {
        text_bottom =tr("codigo de cliente, ");
        campos_pendientes = true;
    }
    if(ui->txtnombre_fiscal->text().isEmpty())
    {
       text_bottom.append(tr("Nombre fiscal, "));
       campos_pendientes = true;
    }
    if(ui->txtcif_nif->text().isEmpty())
    {
        text_bottom.append(tr("Cif/nif, "));
        campos_pendientes = true;
    }
    if(ui->txtdireccion1->text().isEmpty())
    {
        text_bottom.append(tr("direccion, "));
        campos_pendientes = true;
    }
    if(ui->txtcp->text().isEmpty())
    {
        text_bottom.append(tr("CP, "));
        campos_pendientes = true;
    }
    if(ui->txtpoblacion->text().isEmpty())
    {
        text_bottom.append(tr("Población, "));
        campos_pendientes = true;
    }
    if(ui->txtprovincia->text().isEmpty())
    {
        text_bottom.append(tr("Provincia"));
        campos_pendientes = true;
    }
    if (campos_pendientes) {
        ui->blinkink->setVisible(true);
        ui->blinkink->settopTex(tr("ATENCIÓN TIENE CAMPOS OBLIGATORIOS PENDIENTES DE RELLENAR"));
        ui->blinkink->setbottomTex(text_bottom);
        ui->blinkink->setblink(true);
        ui->blinkink->setcolor(QColor (255,96,43));
    } else {
        if(ui->btnGuardar->isEnabled())
        {
            ui->blinkink->setVisible(true);
            ui->blinkink->settopTex("Ficha completa");
            ui->blinkink->setbottomTex(tr("los campos obligatorios estan completos"));
            ui->blinkink->setcolor(QColor (0,190,0));
            ui->blinkink->setblink(false);
        } else
        {
            ui->blinkink->setVisible(false);
        }
    }
}
