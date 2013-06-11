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


#ifdef WIN32
    //#define and &&
#endif

frmClientes::frmClientes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmClientes)
{
    ui->setupUi(this);

    oCliente = new Cliente(this);
    oCliente->id = 0;
    ui->txtNombreFiscal->setText("");
    ValidarCC();
    //oCliente->Actualizar_de_web();
    // Rellenar formas de pago
    modelFP = new QSqlQueryModel();
    modelFP->setQuery("Select cFormaPago,id from FormPago",QSqlDatabase::database("Maya"));

    // Ocultar campos según configuración
    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
    int nNivel = settings.value("nNivelAcceso").toInt();
    //QString cCategoria = settings.value("cCategoria").toString();
    if( Configuracion_global->medic == false) {
        this->setWindowTitle(tr("Gestion de clientes"));
        ui->textoTitulo->setText(("GESTIÓN de CLIENTES - Datos Administrativos"));
    }
    if (nNivel >7 && Configuracion_global->medic == true)
        ui->btnFichaPaciente->setVisible(true);
    else
        ui->btnFichaPaciente->setVisible(false);
    //Configuracion_global->CargarDatos();
    if(Configuracion_global->nIRPF)
        ui->chkClienteEmpresa->setVisible(true);
    else
        ui->chkClienteEmpresa->setVisible(false);

    //FP
    ui->cbocFormaPago->setModel(modelFP);
    //ui->tableView_5->setModel(model);

    // Rellenar Tarifas:
    QSqlQueryModel *qTarifa = new QSqlQueryModel(this);
    qTarifa->setQuery("select descripcion, id from codigotarifa",QSqlDatabase::database("Maya"));
    ui->cbonTarifaCliente->setModel(qTarifa);

    // Rellenar Paises:
    ui->cboPais->setModel(Configuracion_global->paises_model);
    ui->cboPais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    ui->cbopaisAlternativa->setModel(Configuracion_global->paises_model);
    ui->cbopaisAlternativa->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    // rellenar combo idiomas
    QSqlQueryModel *qmIdiomas = new QSqlQueryModel(this);
    qmIdiomas->setQuery("select idioma from idiomas", QSqlDatabase::database("Maya"));
    ui->cboIdiomaDocumentos->setModel(qmIdiomas);
    //oCliente->Recuperar("Select * from clientes");
    //LLenarCampos();

    bloquearCampos();
    this->Altas = false;
    //Connect signals /slots.

    connect(ui->txtPrimerApellido,SIGNAL(editingFinished()),this,SLOT(txtPrimerApellido_editingFinished()));
    connect(ui->txtSegundoApellido,SIGNAL(editingFinished()),this,SLOT(txtSegundoApellido_editingFinished()));
    connect(ui->txtcNombre,SIGNAL(editingFinished()),this,SLOT(txtcNombre_editingFinished()));
    connect(ui->txtcPoblacion,SIGNAL(editingFinished()),this,SLOT(txtcPoblacion_editingFinished()));
    connect(ui->txtcProvincia,SIGNAL(editingFinished()),this,SLOT(txtcProvincia_editingFinished()));
    connect(ui->txtcCifNif,SIGNAL(editingFinished()),this,SLOT(txtcCifNif_editingFinished()));
    connect(ui->txtcCp,SIGNAL(editingFinished()),this,SLOT(txtcCp_editingFinished()));
    connect(ui->txtrRiesgoPermitido,SIGNAL(editingFinished()),this,SLOT(txtrRiesgoPermitido_editingFinished()));
    connect(ui->btnGuardarDireccionAlternativa,SIGNAL(clicked()),this,SLOT(GuardarDireccionAlternativa()));
    connect(ui->btnDeshacerDireccionAlternativa,SIGNAL(clicked()),this,SLOT(DeshacerDireccionAlternativa()));
    connect(ui->lista_DireccionesAlternativas,SIGNAL(clicked(QModelIndex)),this,SLOT(CargarDireccionAlternativa(QModelIndex)));
    connect(ui->btnVer_OtrosContactos,SIGNAL(clicked()),this,SLOT(Contactos()));
    connect(ui->txtcCpPoblacionAlternativa,SIGNAL(editingFinished()),this,SLOT(txtcCPAlternativa_editingFinished()));
    connect(ui->txtcPoblacionAlternativa,SIGNAL(editingFinished()),this,SLOT(txtcPoblacionAlternativa_editingFinished()));
    connect(ui->btnEditarDireccionAlternativa,SIGNAL(clicked()),this,SLOT(EditarDireccionAlternativa()));
    connect(ui->txtcEntidadBancaria,SIGNAL(editingFinished()),this,SLOT(ValidarCC()));
    connect(ui->txtcOficinaBancaria,SIGNAL(editingFinished()),this,SLOT(ValidarCC()));
    connect(ui->txtcDc,SIGNAL(editingFinished()),this,SLOT(ValidarCC()));
    connect(ui->txtcCuentaContable,SIGNAL(editingFinished()),this,SLOT(ValidarCC()));
    connect(ui->btnValidarVIES,SIGNAL(clicked()),this,SLOT(validarNifIntrac()));

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
    if (oCliente->cCodigoCliente.isEmpty()){
        oCliente->cCodigoCliente = oCliente->NuevoCodigoCliente();
        QMessageBox::warning(this,tr("ATENCIÓN"),tr("Faltan datos en la ficha del cliente, deberá rellenarlos ahora"),
                             tr("Aceptar"));
        ui->btnEditar->click();
    }
    ui->txtcCodigoCliente->setText(oCliente->cCodigoCliente);
    ui->txtPrimerApellido->setText(oCliente->cApellido1);
    ui->txtSegundoApellido->setText(oCliente->cApellido2);
    ui->txtcNombre->setText(oCliente->cNombre);
    ui->txtcNombreFiscal->setText(oCliente->cNombreFiscal);;
    ui->txtNombreFiscal->setText(oCliente->cNombreFiscal);;
    ui->txtcNombreComercial->setText(oCliente->cNombreComercial);
    ui->txtcCifNif->setText(oCliente->cCifNif);
    ui->txtCifIntracomunitario->setText(oCliente->cCifVies);
    ui->txtcDireccion1->setText(oCliente->cDireccion1);
    ui->txtcDireccion2->setText(oCliente->cDireccion2);
    ui->txtcCp->setText(oCliente->cCp);
    ui->txtcPoblacion->setText(oCliente->cPoblacion);
    ui->txtcProvincia->setText(oCliente->cProvincia);
    int nindex = ui->cboPais->findText(oCliente->RecuperarPais(oCliente->idPais));
    if (nindex >-1)
        ui->cboPais->setCurrentIndex(nindex);
    ui->txtcTelefono1->setText(oCliente->cTelefono1);
    ui->txtcTelefono2->setText(oCliente->cTelefono2);
    ui->txtcFax->setText(oCliente->cFax);
    ui->txtcMovil->setText(oCliente->cMovil);
    ui->txtcEMail->setText(oCliente->cEmail);
    ui->txtcWeb->setText(oCliente->cWeb);
    ui->txtdFechaAlta->setDate(oCliente->dFechaalta);
    ui->txtdFechaUltimaCompra->setDate(oCliente->dFechaCompra);
    ui->txtrImporteAcumulado->setText(Configuracion_global->FormatoNumerico(QString::number( oCliente->rAcumuladoVentas)));
    ui->txtrVentasEjercicio->setText(Configuracion_global->FormatoNumerico(QString::number(oCliente->rVentasEjercicio)));
    ui->txtrRiesgoPermitido->setText(Configuracion_global->FormatoNumerico(QString::number(oCliente->rRiesgoMaximo)));
    ui->txtrDeudaActual->setText(Configuracion_global->FormatoNumerico(QString::number(oCliente->rDeudaActual)));
    ui->txttComentarios->setText(oCliente->tComentarios);


    if(oCliente->lBloqueado)
        ui->chklBloqueoCliente->setChecked(true);
    else
        ui->chklBloqueoCliente->setChecked(false);

    ui->txttComentarioBloqueo->setText(oCliente->tComentarioBloqueo);
    ui->txtnPorcDtoCliente->setText(QString::number(oCliente->nPorcDtoCliente));

    if(oCliente->lRecargoEquivalencia)
        ui->chklRecargoEquivalencia->setChecked(true);
    else
        ui->chklRecargoEquivalencia->setChecked(false);

    ui->txtcCuentaContable->setText(oCliente->cCuentaContable);
    ui->txtcCuentaIvaRepercutido->setText(oCliente->cCuentaIvaRepercutido);
    ui->txtcCuentaDeudas->setText(oCliente->cCuentaDeudas);
    ui->txtcCuentaCobros->setText(oCliente->cCuentaCobros);
    int indice=ui->cbocFormaPago->findText(oCliente->cFormaPago);

    if(indice!=-1)
        ui->cbocFormaPago->setCurrentIndex(indice);

    //ui->cbocFormaPago->setItemText);
    ui->txtnDiaPago1->setValue(oCliente->nDiaPago1);
    ui->txtnDiaPago2->setValue(oCliente->nDiaPago2);
   // ui->cbonTarifaCliente->lineEdit)->setText(oCliente->nTarifaCliente));
    ui->txtrImporteACuenta->setText( Configuracion_global->FormatoNumerico(QString::number(oCliente->rImporteACuenta,'f',2)));
    ui->txtrVales->setText(Configuracion_global->FormatoNumerico(QString::number(oCliente->rVales,'f',2)));
    ui->txtcEntidadBancaria->setText(oCliente->cEntidadBancaria);
    ui->txtcOficinaBancaria->setText(oCliente->cOficinaBancaria);
    ui->txtcDc->setText(oCliente->cDc);
    ui->txtcCuentaCorriente->setText(oCliente->cCuentaCorriente);
    ui->txtdFechaNacimiento->setDate(oCliente->dFechaNacimiento);
    ui->txtcAccesoWeb->setText(oCliente->cAccesoWeb);
    ui->txtcPasswordWeb->setText(oCliente->cPasswordWeb);
    if (oCliente->lIRPF)
        ui->chkClienteEmpresa->setChecked(true);
    else
        ui->chkClienteEmpresa->setChecked(false);
    indice = ui->cboIdiomaDocumentos->findText(oCliente->idioma);
    if(indice >-1)
        ui->cboIdiomaDocumentos->setCurrentIndex(indice);
    else
        ui->cboIdiomaDocumentos->setCurrentIndex(-1);
    ValidarCC();

    // Tablas
    //----------------
    // DEUDAS
    //----------------

    QSqlQueryModel *deudas = new QSqlQueryModel(this);
    QString cSQL;
    cSQL= "Select id,cDocumento,rImporte,rPagado,rPendienteCobro,dFecha,dVencimiento from clientes_deuda where id_cliente = "
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
    cSQL= "Select id,nAlbaran,dFecha,nFactura,rTotalAlbaran from cab_alb where id_Cliente =" + QString::number(oCliente->id);
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
    cSQL= "Select id,cFactura,dFecha,dFechaCobro,rTotal from cab_fac where iId_Cliente =" + QString::number(oCliente->id);
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

    cSQL= "Select id,nPresupuesto,dFecha,dValidoHasta,rTotal from cab_pre where id_Cliente =" + QString::number(oCliente->id);
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

    cSQL= "Select id,nNumero,dFecha,dVto,rImporte from vales where id_Cliente =" + QString::number(oCliente->id);
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

    cSQL= "Select id,nTicket,dFecha,cHora,rImporte from cab_tpv where id_Cliente =" + QString::number(oCliente->id);
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

    modelAsientos->setQuery("select id,asiento,fechaAsiento,cuentaD,descripcionD, importeD,cuentaH,descripcionH,importeH "
                            "from diario where cta_principal = '"+oCliente->cCuentaContable +
                            "' order by asiento + ' '+posenasiento",
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
    qModelTipos->setQuery("select tipocliente from tipocliente where idcliente = "+QString::number(oCliente->id),
                          QSqlDatabase::database("Maya"));
    ui->lista_tipos->setModel(qModelTipos);

    //-----------------
    // Direcciones
    //-----------------
    QSqlQueryModel *qModelDireccion = new QSqlQueryModel(this);
    qModelDireccion->setQuery("select descripcion,id from cliente_direcciones where idcliente = "+QString::number(oCliente->id),
                              QSqlDatabase::database("Maya"));
    ui->lista_DireccionesAlternativas->setModel(qModelDireccion);
    // -------------------
    // Grafica estadística
    //--------------------
    refrescar_grafica();
}
void frmClientes::VaciarCampos()
{
    QDate dFecha;

    ui->txtcCodigoCliente->setText("");
    ui->txtPrimerApellido->setText("");
    ui->txtSegundoApellido->setText("");
    ui->txtcNombre->setText("");
    ui->txtcNombreFiscal->setText("");
    ui->txtcNombreComercial->setText("");
    ui->txtcCifNif->setText("");
    ui->txtCifIntracomunitario->setText("");
    ui->txtcDireccion1->setText("");
    ui->txtcDireccion2->setText("");
    ui->txtcPoblacion->setText("");
    ui->txtcProvincia->setText("");
    ui->cboPais->clearEditText();
    ui->txtcTelefono1->setText("");
    ui->txtcTelefono2->setText("");
    ui->txtcFax->setText("");
    ui->txtcMovil->setText("");
    ui->txtcEMail->setText("");
    ui->txtcWeb->setText("");
    ui->txtdFechaAlta->setDate(dFecha.currentDate());
    ui->txtdFechaUltimaCompra->setDate(dFecha.currentDate());
    ui->txtrImporteAcumulado->setText("");
    ui->txtrVentasEjercicio->setText("");
    ui->txtrRiesgoPermitido->setText("");
    ui->txtrDeudaActual->setText("");
    ui->txttComentarios->setText("");
    ui->chklBloqueoCliente->setChecked(false);
    ui->txttComentarioBloqueo->setText( "");
    ui->txtnPorcDtoCliente->setText("0");
    ui->chklRecargoEquivalencia->setChecked(false);
    ui->txtcCuentaContable->setText("");
    ui->txtcCuentaIvaRepercutido->setText("");
    ui->txtcCuentaDeudas->setText("");
    ui->txtcCuentaCobros->setText("");


    //ui->cbocFormaPago->setItemText();
    ui->txtnDiaPago1->setValue(0);
    ui->txtnDiaPago2->setValue(0);
   // ui->cbonTarifaCliente->lineEdit()->setText(oCliente->getnTarifaCliente());
    ui->txtrImporteACuenta->setText("");
    ui->txtrVales->setText("");
    ui->txtcEntidadBancaria->setText("");
    ui->txtcOficinaBancaria->setText("");
    ui->txtcDc->setText("");
    ui->txtcCuentaCorriente->setText("");
    ui->txtdFechaNacimiento->setDate(dFecha.currentDate());
    ui->txtcAccesoWeb->setText("");
    ui->txtcPasswordWeb->setText("");
    ui->txtPrimerApellido->setFocus();
    ui->chkClienteEmpresa->setChecked(false);

    //------------------
    // Tipos de clientes
    //------------------
    QSqlQueryModel *qModelTipos = new QSqlQueryModel(this);
    qModelTipos->setQuery("select tipocliente from tipocliente where idcliente = -1",
                          QSqlDatabase::database("Maya"));
    ui->lista_tipos->setModel(qModelTipos);


}

void frmClientes::LLenarCliente()
{
    oCliente->cCodigoCliente= ui->txtcCodigoCliente->text();
    oCliente->cApellido1=ui->txtPrimerApellido->text();
    oCliente->cApellido2=ui->txtSegundoApellido->text();
    oCliente->cNombre=ui->txtcNombre->text();
    oCliente->cNombreFiscal=ui->txtcNombreFiscal->text();
    oCliente->cNombreComercial=ui->txtcNombreComercial->text();
    oCliente->cCifNif=ui->txtcCifNif->text();
    oCliente->cCifVies = ui->txtCifIntracomunitario->text();
    oCliente->cDireccion1=ui->txtcDireccion1->text();
    oCliente->cDireccion2=ui->txtcDireccion2->text();
    oCliente->cCp=ui->txtcCp->text();
    oCliente->cPoblacion=ui->txtcPoblacion->text();
    oCliente->cProvincia=ui->txtcProvincia->text();
    oCliente->cPais=ui->cboPais->currentText();
    oCliente->idPais = oCliente->BuscaridPais(oCliente->cPais);
    oCliente->cTelefono1=ui->txtcTelefono1->text();
    oCliente->cTelefono2=ui->txtcTelefono2->text();
    oCliente->cFax=ui->txtcFax->text();
    oCliente->cMovil=ui->txtcMovil->text();
    oCliente->cEmail=ui->txtcEMail->text();
    oCliente->cWeb=ui->txtcWeb->text();
    oCliente->dFechaalta=ui->txtdFechaAlta->date();
    oCliente->dFechaCompra=ui->txtdFechaUltimaCompra->date();
    oCliente->rAcumuladoVentas=ui->txtrImporteAcumulado->text().replace(".","").toDouble();
    oCliente->rVentasEjercicio=ui->txtrVentasEjercicio->text().replace(".","").toDouble();
    oCliente->rRiesgoMaximo=ui->txtrRiesgoPermitido->text().replace(".","").toDouble();
    oCliente->rDeudaActual=ui->txtrDeudaActual->text().replace(".","").toDouble();
    oCliente->tComentarios=ui->txttComentarios->toPlainText();
    oCliente->lBloqueado=ui->chklBloqueoCliente->isChecked();
    oCliente->tComentarioBloqueo=ui->txttComentarioBloqueo->toPlainText();
    oCliente->nPorcDtoCliente=ui->txtnPorcDtoCliente->text().toInt();
    oCliente->lRecargoEquivalencia=ui->chklRecargoEquivalencia->isChecked();
    oCliente->cCuentaContable=ui->txtcCuentaContable->text();
    oCliente->cCuentaIvaRepercutido=ui->txtcCuentaIvaRepercutido->text();
    oCliente->cCuentaDeudas=ui->txtcCuentaDeudas->text();
    oCliente->cCuentaCobros=ui->txtcCuentaCobros->text();
    oCliente->cFormaPago=ui->cbocFormaPago->currentText();
    oCliente->nDiaPago1=ui->txtnDiaPago1->value();
    oCliente->nDiaPago2=ui->txtnDiaPago2->value();
    oCliente->nTarifaCliente=ui->cbonTarifaCliente->currentText().toDouble();
    oCliente->rImporteACuenta=ui->txtrImporteACuenta->text().toDouble();
    oCliente->rVales= ui->txtrVales->text().replace(".","").toDouble();
    oCliente->cEntidadBancaria=ui->txtcEntidadBancaria->text();
    oCliente->cOficinaBancaria=ui->txtcOficinaBancaria->text();
    oCliente->cDc=ui->txtcDc->text();
    oCliente->cCuentaCorriente=ui->txtcCuentaCorriente->text();
    oCliente->dFechaNacimiento=ui->txtdFechaNacimiento->date();
    oCliente->rImportePendiente=ui->txtrImporteAcumulado->text().toDouble();
    oCliente->cAccesoWeb=ui->txtcAccesoWeb->text();
    oCliente->cPasswordWeb=ui->txtcPasswordWeb->text();
    oCliente->idIdioma = Configuracion_global->Devolver_id_idioma(ui->cboIdiomaDocumentos->currentText());
    oCliente->idioma = ui->cboIdiomaDocumentos->currentText();
    if (ui->chkClienteEmpresa)
        oCliente->lIRPF =true;
    else
        oCliente->lIRPF=false;

    QSqlQuery qTarifa(QSqlDatabase::database("Maya"));
    qTarifa.prepare("select id from codigotarifa where descripcion = '"+ui->cbonTarifaCliente->currentText()+"'");
    if(qTarifa.exec()){
        qTarifa.next();
        oCliente->nTarifaCliente = qTarifa.record().field("id").value().toInt();
    }
}


void frmClientes::on_btnSiguiente_clicked()
{
    QString cId = QString::number(oCliente->id);
    oCliente->Recuperar("Select * from clientes where id >"+cId+" order by id limit 1 ");
    LLenarCampos();
}

void frmClientes::on_btnAnterior_clicked()
{
    QString cId = QString::number(oCliente->id);
    oCliente->Recuperar("Select * from clientes where id <"+cId+" order by id desc limit 1 ");
    LLenarCampos();
}

void frmClientes::on_btnGuardar_clicked()
{
    QString cVacios;
    if(ui->txtcCifNif->text().isEmpty())
        cVacios = "Falta rellenar CIF/NIF\n";
    if(ui->txtcNombreFiscal->text().isEmpty())
        cVacios = cVacios + "Falta rellenar NombreFiscal\n";
    if (ui->txtcDireccion1->text().isEmpty())
        cVacios = cVacios + "Falta rellenar Dirección\n";
    if (ui->txtcCp->text().isEmpty())
        cVacios = cVacios + "Falta rellenar CP\n";
    if(ui->txtcPoblacion->text().isEmpty())
        cVacios = cVacios + "Falta rellenar Población\n";
    if (ui->txtcProvincia->text().isEmpty())
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
        ui->txtcCifNif->setFocus();
    }
}

void frmClientes::on_btnAnadir_clicked()
{
    emit block();
    desbloquearCampos();
    VaciarCampos();
    this->Altas = true;
    ui->txtcCodigoCliente->setText(oCliente->NuevoCodigoCliente());
    ui->txtcCuentaContable->setText(ui->txtcCodigoCliente->text());
    ui->txtcCodigoCliente->setFocus();
    LLenarCliente();
    oCliente->Anadir();

}

void frmClientes::txtPrimerApellido_editingFinished()
{
    ui->txtPrimerApellido->setText(ui->txtPrimerApellido->text().toUpper());

    if(!ui->txtPrimerApellido->text().isEmpty()
       && !ui->txtSegundoApellido->text().isEmpty()
       && !ui->txtcNombre->text().isEmpty())
    {
        ui->txtcNombreFiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtcNombre->text());
        ui->txtNombreFiscal->setText(ui->txtcNombreFiscal->text());
    }
}

void frmClientes::txtSegundoApellido_editingFinished()
{
    ui->txtSegundoApellido->setText(ui->txtSegundoApellido->text().toUpper());

    if(!ui->txtPrimerApellido->text().isEmpty()
        && !ui->txtSegundoApellido->text().isEmpty()
        && !ui->txtcNombre->text().isEmpty())
    {
        ui->txtcNombreFiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtcNombre->text());
        ui->txtNombreFiscal->setText(ui->txtcNombreFiscal->text());
    }

}

void frmClientes::txtcNombre_editingFinished()
{
    ui->txtcNombre->setText(ui->txtcNombre->text().toUpper());

    if(!ui->txtPrimerApellido->text().isEmpty()
       && !ui->txtSegundoApellido->text().isEmpty()
       && !ui->txtcNombre->text().isEmpty())
    {
        ui->txtcNombreFiscal->setText(ui->txtPrimerApellido->text() + " "+ ui->txtSegundoApellido->text() + ", "+ui->txtcNombre->text());
        ui->txtNombreFiscal->setText(ui->txtcNombreFiscal->text());
    }

}

void frmClientes::txtcPoblacion_editingFinished()
{
    ui->txtcPoblacion->setText(ui->txtcPoblacion->text().toUpper());
    if (ui->txtcCp->text().isEmpty() && !ui->txtcPoblacion->text().isEmpty())
    {
       FrmBuscarPoblacion BuscarPoblacion;
       BuscarPoblacion.setcPoblacion(ui->txtcPoblacion->text(),1);
       if(BuscarPoblacion.exec())
       {
        //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
         int nId = BuscarPoblacion.DevolverID();
         qDebug() << "nID: " <<nId;
         if(nId > 0)
         {
             QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
             QString cId;
             cId = QString::number(nId);
             qPoblacion.prepare("select poblacion,cp,provincia from poblaciones where id = :cId");
             qPoblacion.bindValue(":cId",cId);
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
                     ui->txtcPoblacion->setText(qPoblacion.value(0).toString());
                     ui->txtcCp->setText(qPoblacion.value(1).toString());
                     ui->txtcProvincia->setText(qPoblacion.value(2).toString());
                    // ui->txtcPais->setText("ESPAÑA");
                     oCliente->idPais =oCliente->BuscaridPais("ESPAÑA");
                 }
             }
         }
       }
      // BuscarPoblacion.close();
    }
}

void frmClientes::txtcProvincia_editingFinished()
{
    ui->txtcProvincia->setText(ui->txtcProvincia->text().toUpper());
}

void frmClientes::txtcCifNif_editingFinished()
{
    if (ui->txtcCifNif->text().length() == 8){
        QString cCIF = Configuracion::letraDNI(ui->txtcCifNif->text());
        ui->txtcCifNif->setText(cCIF.toUpper());
   }
}

void frmClientes::on_btnEditar_clicked()
{
    emit block();
        desbloquearCampos();
        ui->txtcCodigoCliente->setEnabled(false);
        ui->txtcCifNif->setFocus();
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
    ui->btnAnadirDireccion->setEnabled(false);
    ui->btnBorrarDireccion->setEnabled(false);
    ui->btnEditarDireccionAlternativa->setEnabled(false);
    ui->btnGuardarDireccionAlternativa->setEnabled(false);
    ui->btnDeshacerDireccionAlternativa->setEnabled(false);
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
    ui->txtrDeudaActual->setEnabled(false);
    ui->txtrImporteAcumulado->setEnabled(false);
    ui->txtrVentasEjercicio->setEnabled(false);
    ui->btnAnadirDireccion->setEnabled(true);
    ui->btnBorrarDireccion->setEnabled(true);
    ui->btnEditarDireccionAlternativa->setEnabled(true);
    ui->btnAdd_TipoCliente->setEnabled(true);
    ui->btndel_TipoCliente->setEnabled(true);
    ui->txtcDescripcionDireccion->setEnabled(false);
    ui->txtcDireccionAlternativa1->setEnabled(false);
    ui->txtcDireccionAlternativa2->setEnabled(false);
    ui->txtcCpPoblacionAlternativa->setEnabled(false);
    ui->txtcPoblacionAlternativa->setEnabled(false);
    ui->txtcProvinciaAlternativa->setEnabled(false);
    ui->cbopaisAlternativa->setEnabled(false);

}

void frmClientes::on_btnDeshacer_clicked()
{
    if(this->Altas){
        oCliente->Borrar(oCliente->id);
        this->Altas = false;
    }
    QString cId = QString::number(oCliente->id);
    oCliente->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
    LLenarCampos();
    bloquearCampos();
    emit unblock();
}

void frmClientes::on_btnBorrar_clicked()
{
    int id_Cliente = oCliente->id;
    id_Cliente --;

    oCliente->Borrar(oCliente->id);
    VaciarCampos();
}

void frmClientes::on_btnBuscar_clicked()
{
    FrmBuscarCliente BuscarClientes;
    if(BuscarClientes.exec() == QDialog::Accepted)
    {
        int nId = BuscarClientes.DevolverID();
        qDebug() << nId;
        QString cId = QString::number(nId);

        oCliente->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
        LLenarCampos();
    }
}

void frmClientes::txtcCp_editingFinished()
{
    if (!ui->txtcCp->text().isEmpty() && ui->txtcPoblacion->text().isEmpty())
    {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
        if(BuscarPoblacion.exec())
        {

            int nId = BuscarPoblacion.DevolverID();
            //qDebug() <<nId;
            if(nId > 0)
            {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cId;
                cId = QString::number(nId);
                qPoblacion.prepare("select poblacion, cp,provincia from poblaciones where id = :cId");
                qPoblacion.bindValue(":cId",cId);
                if(!qPoblacion.exec())
                {
                    /* qDebug() << qPoblacion.lastQuery();*/
                    QMessageBox::critical(qApp->activeWindow(),tr("Asociar Población"),tr("Ha fallado la busqueda de población"),tr("&Aceptar"));
                }
                else
                {
                    qDebug() << "Poblacion" <<qPoblacion.value(0).toString();
                    qDebug() << qPoblacion.lastQuery() <<" ID:= " << cId;
                    if (qPoblacion.next())
                    {
                        ui->txtcCp->setText(qPoblacion.value(1).toString());
                        ui->txtcPoblacion->setText(qPoblacion.value(0).toString());
                        ui->txtcProvincia->setText(qPoblacion.value(2).toString());

                       // ui->txtcPais->setText("ESPAÑA");
                        oCliente->idPais =oCliente->BuscaridPais("ESPAÑA");
                    }
                }
            }
        }
    }
}


void frmClientes::txtcCPAlternativa_editingFinished()
{
    if (!ui->txtcCpPoblacionAlternativa->text().isEmpty() && ui->txtcPoblacionAlternativa->text().isEmpty())
    {
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcCpPoblacionAlternativa->text(),0);
        if(BuscarPoblacion.exec())
        {
            //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
            int nId = BuscarPoblacion.DevolverID();
            //qDebug() <<nId;
            if(nId > 0)
            {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cId;
                cId = QString::number(nId);
                qPoblacion.prepare("select poblacion, cp,provincia from poblaciones where id = :cId");
                qPoblacion.bindValue(":cId",cId);
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
                        ui->txtcPoblacionAlternativa->setText(qPoblacion.value(0).toString());
                        ui->txtcProvinciaAlternativa->setText(qPoblacion.value(2).toString());
                        //TODO - poner pais configuración
                       // ui->txtcPaisAlternativa->setText("ESPAÑA");
                    }
                }
            }
        }
        // BuscarPoblacion.close();
    }
}

void frmClientes::txtcPoblacionAlternativa_editingFinished()
{
    if (ui->txtcCpPoblacionAlternativa->text().isEmpty() && !ui->txtcPoblacionAlternativa->text().isEmpty())
    {
        ui->txtcPoblacionAlternativa->setText(ui->txtcPoblacionAlternativa->text().toUpper());
        FrmBuscarPoblacion BuscarPoblacion;
        BuscarPoblacion.setcPoblacion(ui->txtcPoblacionAlternativa->text(),1);
        if(BuscarPoblacion.exec() == QDialog::Accepted)
        {
            //  BuscarPoblacion.setcPoblacion(ui->txtcCp->text(),0);
            int nId = BuscarPoblacion.DevolverID();
            //qDebug() <<nId;
            if(nId > 0)
            {
                QSqlQuery qPoblacion(QSqlDatabase::database("Maya"));
                QString cId;
                cId = QString::number(nId);
                qPoblacion.prepare("select poblacion,cp,provincia from poblaciones where id = :cId");
                qPoblacion.bindValue(":cId",cId);
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
                        ui->txtcPoblacionAlternativa->setText(qPoblacion.record().value("poblacion").toString());
                        ui->txtcCpPoblacionAlternativa->setText(qPoblacion.record().value("cp").toString());
                        ui->txtcProvinciaAlternativa->setText(qPoblacion.record().value("provincia").toString());
                        int indice;
                        indice = ui->cbopaisAlternativa->findText(Configuracion_global->cPais);
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
    ui->txtrRiesgoPermitido->setText(Configuracion_global->FormatoNumerico(ui->txtrRiesgoPermitido->text()));
}

void frmClientes::on_btnFichaPaciente_clicked()
{
    //NOTE - no mostar subform si no hay ningun cliente select
    if(ui->txtcCodigoCliente->text().isEmpty()) {
        QMessageBox::warning(this,tr("Ficha Paciente"),tr("Debe tener un cliente seleccionado para poder acceder a la ficha de paciente"),
                             tr("Aceptar"));
        return;
    }

    int idPaciente;
    //if not patient record in database, create it now.
    Paciente oPaciente;
    oPaciente.RecuperarPaciente(oCliente->id);

    if(oPaciente.getid()==0){
        int idCliente = oCliente->id;
        idPaciente = oPaciente.AnadirPaciente(idCliente);
        if (idPaciente!=0)
            oPaciente.RecuperarPaciente(idPaciente);
    }
    // open patient form


    FrmFichaPaciente frmPaciente(this);
    frmPaciente.setWindowModality(Qt::WindowModal);

    frmPaciente.recibedatospaciente(oCliente->id,oCliente->cNombreFiscal);
    frmPaciente.cargarDatos(oCliente->id);
    frmPaciente.setWindowState(Qt::WindowMaximized);
    frmPaciente.exec();
}

void frmClientes::AddCustomerType()
{

   FrmAddTipoCliente AddTipoCliente(this);
   if(AddTipoCliente.exec() == QDialog::Accepted){
       QString valor = AddTipoCliente.cFamiliaRetorno;
       QSqlQuery qTipo(QSqlDatabase::database("Maya"));
       qTipo.prepare("INSERT INTO tipocliente (tipocliente,idcliente) VALUES (:tipo,:idcliente)");
       qTipo.bindValue(":tipo",valor);
       qTipo.bindValue(":idcliente",oCliente->id);
       if (!qTipo.exec())
           QMessageBox::warning(this,tr("Añadir tipo cliente"),
                                tr("No se pudo añadir el tipo de cliente :%1").arg(qTipo.lastError().text()),tr("Aceptar"));
       QSqlQueryModel *qModelTipos = new QSqlQueryModel(this);
       qModelTipos->setQuery("select tipocliente from tipocliente where idcliente = "+QString::number(oCliente->id),
                             QSqlDatabase::database("Maya"));

        ui->lista_tipos->setModel(qModelTipos);
   }
}

void frmClientes::AnadirDireccionAlternativa()
{
 AnadirDireccion = true;
 ui->txtcDescripcionDireccion->setEnabled(true);
 ui->txtcDescripcionDireccion->setText("");
 ui->txtcDireccionAlternativa1->setEnabled(true);
 ui->txtcDireccionAlternativa1->setText("");
 ui->txtcDireccionAlternativa2->setEnabled(true);
 ui->txtcDireccionAlternativa2->setText("");
 ui->txtcCpPoblacionAlternativa->setEnabled(true);
 ui->txtcCpPoblacionAlternativa->setText("");
 ui->txtcPoblacionAlternativa->setText("");
 ui->txtcPoblacionAlternativa->setEnabled(true);
 ui->txtcProvinciaAlternativa->setText("");
 ui->txtcProvinciaAlternativa->setEnabled(true);
 ui->cbopaisAlternativa->setEnabled(true);
 ui->btnGuardarDireccionAlternativa->setEnabled(true);
 ui->btnDeshacerDireccionAlternativa->setEnabled(true);
 ui->btnAnadirDireccion->setEnabled(false);
 ui->btnBorrarDireccion->setEnabled(false);
 ui->txtcDescripcionDireccion->setFocus();
 ui->btnGuardarDireccionAlternativa->setEnabled(true);
 ui->btnDeshacerDireccionAlternativa->setEnabled(true);
 ui->btnAnadirDireccion->setEnabled(false);
 ui->btnBorrarDireccion->setEnabled(false);
 ui->btnGuardar->setEnabled(false);
 ui->btnDeshacer->setEnabled(false);
 ui->btnEditarDireccionAlternativa->setEnabled(false);
}

void frmClientes::GuardarDireccionAlternativa()
{
    if(ui->cbopaisAlternativa->currentText().isEmpty()){
        TimedMessageBox * t = new TimedMessageBox(this,tr("Debe rellenar el campo pais para poder guardar"));
    } else {
        ui->txtcDescripcionDireccion->setEnabled(false);
        ui->txtcDireccionAlternativa1->setEnabled(false);
        ui->txtcDireccionAlternativa2->setEnabled(false);
        ui->txtcCpPoblacionAlternativa->setEnabled(false);
        ui->txtcPoblacionAlternativa->setEnabled(false);
        ui->txtcProvinciaAlternativa->setEnabled(false);
        ui->cbopaisAlternativa->setEnabled(false);

        if (AnadirDireccion)
            oCliente->GuardarDireccion(true,ui->txtcDescripcionDireccion->text(),ui->txtcDireccionAlternativa1->text(),
                                   ui->txtcDireccionAlternativa2->text(),ui->txtcCpPoblacionAlternativa->text(),
                                   ui->txtcPoblacionAlternativa->text(),ui->txtcProvinciaAlternativa->text(),
                                   ui->cbopaisAlternativa->currentText(),oCliente->id,NULL);
        else
            oCliente->GuardarDireccion(false,ui->txtcDescripcionDireccion->text(),ui->txtcDireccionAlternativa1->text(),
                                   ui->txtcDireccionAlternativa2->text(),ui->txtcCpPoblacionAlternativa->text(),
                                   ui->txtcPoblacionAlternativa->text(),ui->txtcProvinciaAlternativa->text(),
                                   ui->cbopaisAlternativa->currentText(),oCliente->id,idDireccionAlternativa);
        AnadirDireccion = false;
        //-----------------
        // Direcciones
        //-----------------
        QSqlQueryModel *qModelDireccion = new QSqlQueryModel(this);
        qModelDireccion->setQuery("select descripcion,id from cliente_direcciones where idcliente = "+QString::number(oCliente->id),
                                  QSqlDatabase::database("Maya"));
        ui->lista_DireccionesAlternativas->setModel(qModelDireccion);
        ui->btnGuardar->setEnabled(true);
        ui->btnDeshacer->setEnabled(true);
        ui->btnGuardarDireccionAlternativa->setEnabled(false);
        ui->btnDeshacerDireccionAlternativa->setEnabled(false);
        ui->btnAnadirDireccion->setEnabled(true);
        ui->btnBorrarDireccion->setEnabled(true);
        ui->btnEditarDireccionAlternativa->setEnabled(true);
    }
}

void frmClientes::DeshacerDireccionAlternativa()
{
    //-----------------
    // Direcciones
    //-----------------
    qModelDireccion = new QSqlQueryModel(this);
    qModelDireccion->setQuery("select descripcion,id from cliente_direcciones where idcliente = "+QString::number(oCliente->id),
                              QSqlDatabase::database("Maya"));
    ui->lista_DireccionesAlternativas->setModel(qModelDireccion);
    ui->btnGuardar->setEnabled(true);
    ui->btnDeshacer->setEnabled(true);
    ui->btnGuardarDireccionAlternativa->setEnabled(false);
    ui->btnDeshacerDireccionAlternativa->setEnabled(false);
    ui->btnEditarDireccionAlternativa->setEnabled(true);

    QSqlQuery diralt(QSqlDatabase::database("Maya"));
    diralt.prepare("select * from cliente_direcciones where id = :id");
    diralt.bindValue(":id",idDireccionAlternativa);
    if(diralt.exec()){
        diralt.next();
        ui->txtcDescripcionDireccion->setText(diralt.record().value("descripcion").toString());
        ui->txtcDireccionAlternativa1->setText(diralt.record().value("direccion1").toString());
        ui->txtcDireccionAlternativa2->setText(diralt.record().value("direccion2").toString());
        ui->txtcCpPoblacionAlternativa->setText(diralt.record().value("cp").toString());
        ui->txtcPoblacionAlternativa->setText(diralt.record().value("poblacion").toString());
        ui->txtcProvinciaAlternativa->setText(diralt.record().value("provincia").toString());
        int indice;
        indice = ui->cbopaisAlternativa->findText(diralt.record().value("pais").toString());
        ui->cbopaisAlternativa->setCurrentIndex(indice);

    }

}



void frmClientes::BorrarDireccionAlternativa()
{
    if(QMessageBox::question(this,tr("Direcciones alternativas"),tr("va a borrar una dirección alternativa\n"
                                                                    "¿desea continuar?"),
                             tr("Cancelar"),tr("Borrar"))==QMessageBox::Accepted)
    {
        QSqlQuery qDireccion(QSqlDatabase::database("Maya"));
        if(!qDireccion.exec("delete from cliente_direcciones where id="+QString::number(idDireccionAlternativa)))
            QMessageBox::warning(this,tr("Direcciones Alternativas"),tr("No se ha podido borrar la direccion"),tr("Aceptar"));

    }
    QString cSQL = "select descripcion,id from cliente_direcciones where idcliente = "+QString::number(oCliente->id);
    qModelDireccion = new QSqlQueryModel();
    qModelDireccion->setQuery(cSQL,QSqlDatabase::database("Maya"));
    ui->lista_DireccionesAlternativas->setModel(qModelDireccion);
    ui->txtcDescripcionDireccion->setText("");
    ui->txtcDireccionAlternativa1->setText("");
    ui->txtcDireccionAlternativa2->setText("");
    ui->txtcCpPoblacionAlternativa->setText("");
    ui->txtcPoblacionAlternativa->setText("");
    ui->txtcProvinciaAlternativa->setText("");
    ui->cbopaisAlternativa->setCurrentIndex(-1);
}

void frmClientes::EditarDireccionAlternativa()
{
    ui->btnGuardar->setEnabled(false);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditarDireccionAlternativa->setEnabled(false);
    ui->btnGuardarDireccionAlternativa->setEnabled(true);
    ui->btnDeshacerDireccionAlternativa->setEnabled(true);
    ui->txtcDescripcionDireccion->setEnabled(true);
    ui->txtcDireccionAlternativa1->setEnabled(true);
    ui->txtcDireccionAlternativa2->setEnabled(true);
    ui->txtcCpPoblacionAlternativa->setEnabled(true);
    ui->txtcPoblacionAlternativa->setEnabled(true);
    ui->txtcProvinciaAlternativa->setEnabled(true);
    ui->cbopaisAlternativa->setEnabled(true);
    AnadirDireccion = false;
}

void frmClientes::CargarDireccionAlternativa(QModelIndex index)
{
    QSqlQueryModel* modelo = (QSqlQueryModel*)ui->lista_DireccionesAlternativas->model();
    int nId = modelo->record(index.row()).value("id").toInt();
    QSqlQuery qDirecciones(QSqlDatabase::database("Maya"));
    qDirecciones.prepare("select * from cliente_direcciones where id = :id");
    qDirecciones.bindValue(":id",nId);
    if(qDirecciones.exec()){
        qDirecciones.next();
        idDireccionAlternativa = nId;
        ui->txtcDescripcionDireccion->setText(qDirecciones.record().field("descripcion").value().toString());
        ui->txtcDireccionAlternativa1->setText(qDirecciones.record().field("direccion1").value().toString());
        ui->txtcDireccionAlternativa2->setText(qDirecciones.record().field("direccion2").value().toString());
        ui->txtcCpPoblacionAlternativa->setText(qDirecciones.record().field("cp").value().toString());
        ui->txtcPoblacionAlternativa->setText(qDirecciones.record().field("poblacion").value().toString());
        ui->txtcProvinciaAlternativa->setText(qDirecciones.record().field("provincia").value().toString());
        QString cPais = oCliente->RecuperarPais(qDirecciones.record().field("idpais").value().toInt());
        int nIndex = ui->cbopaisAlternativa->findText(cPais);
        if (nIndex <-1)
            ui->cbopaisAlternativa->setCurrentIndex(nIndex);

    }
}

void frmClientes::ValidarCC()
{
    QString cOk;
    if(!ui->txtcEntidadBancaria->text().isEmpty() && !ui->txtcOficinaBancaria->text().isEmpty() &&
            !ui->txtcDc->text().isEmpty() && !ui->txtcCuentaCorriente->text().isEmpty())
        cOk = Configuracion_global->ValidarCC(ui->txtcEntidadBancaria->text(),ui->txtcOficinaBancaria->text(),
                                 ui->txtcDc->text(),ui->txtcCuentaCorriente->text());
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
        QMessageBox::warning(this,tr("Validacion VIES"),tr("El Cif VIES introducido es correcto"),tr("Aceptar"));
}

void frmClientes::Contactos()
{
    frmPersonasContactoCliente contacto;
    contacto.nIdCliente = oCliente->id;
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
