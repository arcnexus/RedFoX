#include "frmarticulos.h"
#include "ui_frmarticulos.h"
#include "../Almacen/frmtarifas.h"
#include "../Almacen/frmasociarproveedor.h"
#include "../Auxiliares/spinboxdelegate.h"
#include "../db_table_view.h"

#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/datedelegate.h"
#include "../Auxiliares/delegatekit.h"

#include "../Auxiliares/readonlydelegate.h"
#include "frmlistadosarticulo.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Almacen/frmexcepciones.h"
#include "../Almacen/frmkit.h"
#include "../Auxiliares/delegatekit.h"

#include <QtConcurrent/QtConcurrent>

FrmArticulos::FrmArticulos(QWidget *parent, bool closeBtn) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmArticulos),
    menuButton(QIcon(":/Icons/PNG/Box.png"),tr("Almacen"),this),
    shortCut(new QPushButton(QIcon(":/Icons/PNG/Box.png"),"",this))

{
    id_oferta = 0;    
    ui->setupUi(this);


    shortCut->setToolTip(tr("Gestión del fichero de artículos"));
    shortCut->setStyleSheet("background-color: rgb(133, 170, 142)");
    ui->btn_cerrar->setVisible(closeBtn);    
}

void FrmArticulos::init()
{
    oArticulo = new Articulo(this);
    ui->lbl_en_promocion->setVisible(false);
    ui->stackedWidget->setCurrentIndex(1);

    //ALLOC MODELS
    modelEmpresa = new QSqlQueryModel(this);
    promociones = new QSqlQueryModel(this);
    volumen = new QSqlQueryModel(this);
    modelBusqueda = new QSqlQueryModel(this);
    modelTarifa = new QSqlQueryModel(this);
    modelProv = new QSqlQueryModel(this);
    modelTrazabilidad1 = new QSqlQueryModel(this);
    modelTrazabilidad2 = new QSqlQueryModel(this);

    tarifa_model = new QSqlTableModel(this,Configuracion_global->groupDB);
    tarifa_model->setTable("viewtarifa");
    tarifa_model->setFilter("id_articulo = "+QString::number(oArticulo->id));

    //SET MODEL ON TABLE
    ui->cboEmpresa2->setModel(modelEmpresa);
    ui->tabla_ofertas->setModel(promociones);
    ui->tablaBusqueda->setModel(modelBusqueda);
    ui->TablaTarifas->setModel(modelTarifa);
    ui->tablaProveedores->setModel(modelProv);
    ui->tablaLotes->setModel(modelTrazabilidad1);
    ui->tablaVentas->setModel(modelTrazabilidad2);
    ui->cboTipoIVA->setModel(Configuracion_global->iva_model);
    ui->cboTipoIVA->setModelColumn(2);

    //SET DELEGATES
    ui->TablaTarifas->setItemDelegateForColumn(5, new MonetaryDelegate(this));
    ui->TablaTarifas->setItemDelegateForColumn(6, new MonetaryDelegate(this));
    ui->TablaTarifas->setItemDelegateForColumn(7, new MonetaryDelegate(this));
    ui->TablaTarifas->setItemDelegateForColumn(8, new MonetaryDelegate(this));

    ui->tablaBusqueda->setItemDelegateForColumn(5, new MonetaryDelegate(this));
    ui->tablaBusqueda->setItemDelegateForColumn(6,new DelegateKit(this));
    ui->tablaBusqueda->setItemDelegateForColumn(7, new MonetaryDelegate(this));
    ui->tablaBusqueda->setItemDelegateForColumn(8, new MonetaryDelegate(this));
    ui->tablaBusqueda->setItemDelegateForColumn(9,new MonetaryDelegate(this));


    ui->tabla_ofertas->setItemDelegateForColumn(1, new DelegateKit(this));

    ui->tablaProveedores->setItemDelegateForColumn(5,new MonetaryDelegate);
    ui->tablaProveedores->setItemDelegateForColumn(4,new MonetaryDelegate);


    //SET UP CHARTS
    ui->graf_prov->Clear();
    ui->graf_prov->verValoresEjeY(true);
    ui->graf_prov->verValores(true);

    // CONEXIONES
    connect(ui->txtcoste,SIGNAL(editingFinished()),Configuracion_global,SLOT(format_text()));
    connect(ui->btnAnadirProveedores,SIGNAL(clicked()),this,SLOT(anadir_proveedor_clicked()));
    connect(ui->tablaLotes,SIGNAL(clicked(QModelIndex)),this,SLOT(TablaTrazabilidad_clicked(QModelIndex)));
    connect(ui->radGrafica_unidades,SIGNAL(clicked()),this,SLOT(GraficaUnidades()));
    connect(ui->radGrafica_importes,SIGNAL(clicked()),this,SLOT(GraficaImportes()));
    connect(ui->btnEditarProveedorFrecuente,SIGNAL(clicked()), this, SLOT(editar_proveedor_clicked()));
    connect(ui->btnAsignarProveedor,SIGNAL(clicked()),this,SLOT(asignar_proveedor_principal_clicked()));
    connect(ui->btnBorrarProveedores,SIGNAL(clicked()),this,SLOT(borrar_proveedor_clicked()));
    connect(ui->btnEditartarifa,SIGNAL(clicked()),this,SLOT(btnEditarTarifa_clicked()));
    connect(ui->btnBorrarTarifa,SIGNAL(clicked()),this,SLOT(btnBorrarTarifa_clicked()));
    connect(ui->cboEmpresa2,SIGNAL(currentIndexChanged(int)),this,SLOT(LLenarGrafica_comparativa(int)));
    connect(ui->radGrafica_importes_2,SIGNAL(toggled(bool)),this,SLOT(MostrarGrafica_comparativa(bool)));
    connect(ui->cboTipoGrafica,SIGNAL(currentIndexChanged(QString)),this,SLOT(graficar(QString)));

    //SEARCH BAR
    setUpBusqueda();

    // CONTROL DE EVENTOS
    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;
    this->installEventFilter(this);
    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);

    bloquearCampos(true);

    modelEmpresa->setQuery("select * from vistaEmpresa",Configuracion_global->groupDB);
    promociones->setQuery("select id,activa,descripcion from articulos_ofertas where id_articulo = 0",Configuracion_global->empresaDB);
    volumen->setQuery("select id,desde,hasta,precio from articulos_volumen",Configuracion_global->groupDB);

    QString cSQL = "select id,codigo, descripcion_reducida, codigo_barras,codigo_fabricante,tipo_iva,kit, pvp,pvp_con_iva,stock_fisico_almacen from vistaart_tarifa "
            "where tarifa ="+QString::number(Configuracion_global->id_tarifa_predeterminada);
    modelBusqueda->setQuery(cSQL, Configuracion_global->groupDB);

    if(modelBusqueda->rowCount() == 0)
        modelBusqueda->setQuery("Select id,codigo, descripcion, codigo_barras,codigo_fabricante,tipo_iva,kit,stock_fisico_almacen from articulos", Configuracion_global->groupDB);

    tarifa_model->select();

    GraficaUnidades();
    //SET TABLE FORMAT
    format_tables();
}

void FrmArticulos::format_tables()
{
    QStringList headers_o;
    QVariantList sizes_o;
    headers_o << "id" << tr("Act.") << tr("Descripción") ;
    sizes_o   <<   0  <<      50    <<        350;

    for(int i = 0; i<sizes_o.size();i++)
    {
        ui->tabla_ofertas->setColumnWidth(i,sizes_o.at(i).toInt());
        promociones->setHeaderData(i,Qt::Horizontal,headers_o.at(i));
    }
    ui->tabla_ofertas->setColumnHidden(0,true);


    QStringList headers;
    headers << "Pais"   << "Moneda" << "Codigo de tarifa" << "Descripción";
    headers << "Margen" << "Margen minimo" << "P.V.P";

    for (int i = 0; i< headers.size();i++)
        tarifa_model->setHeaderData(i+1, Qt::Horizontal, headers.at(i));

    QStringList titulo;
    QVariantList col_size;
    titulo <<"id" << tr("Código") << tr("Descripción")  << tr("C. Barras") << tr("código fabricante") <<tr("%IVA") << tr("KIT") << tr("P.V.P.")
           << tr("PVP+IVA") << tr("S.Alm.") ;

    col_size << 0<<130<<400<<130<<130<<100<<50<<100<<100<<100;
    for(int i =0;i<titulo.size();i++)
    {
        modelBusqueda->setHeaderData(i,Qt::Horizontal,titulo.at(i));
        ui->tablaBusqueda->setColumnWidth(i,col_size.at(i).toInt());
    }
    ui->tablaBusqueda->setColumnHidden(0,true);
}

void FrmArticulos::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);

    QStringList orden;
    orden << tr("Descripción") <<tr("Código") <<tr("Código Barras") <<tr("Ref. Proveedor");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(m_busqueda,SIGNAL(hideMe()),this,SLOT(ocultarBusqueda()));

    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));

    QPushButton * addproduct = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir"),this);
    connect(addproduct,SIGNAL(clicked()),this,SLOT(on_botAnadir_clicked()));
    m_busqueda->addWidget(addproduct);

    QPushButton * editproduct = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar"),this);
    connect(editproduct,SIGNAL(clicked()),this,SLOT(on_botEditar_clicked()));
    m_busqueda->addWidget(editproduct);

    QPushButton * deleteproduct = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    connect(deleteproduct,SIGNAL(clicked()),this,SLOT(on_botBorrar_clicked()));
    m_busqueda->addWidget(deleteproduct);
    m_busqueda->addSpacer();

    QPushButton* excep = new QPushButton(QIcon(":/Icons/PNG/excepciones.png"),tr("Excepciones"),this);
    connect(excep,SIGNAL(clicked()),this,SLOT(on_btnExcepciones_clicked()));
    m_busqueda->addWidget(excep);


    QPushButton* kit = new QPushButton(QIcon(":/Icons/PNG/kits.png"),tr("Crear/Editar Kit"),this);
    connect(kit,SIGNAL(clicked()),this,SLOT(on_btnKit_clicked()));
    m_busqueda->addWidget(kit);

    connect(m_busqueda,SIGNAL(key_Down_Pressed()),ui->tablaBusqueda,SLOT(setFocus()));
}

void FrmArticulos::llenar_tabla_tarifas()
{
    modelTarifa->setQuery("select id,codigo_tarifa,descripcion,pais,moneda,margen, margen_minimo, pvp,pvp_con_iva,simbolo "
                         "from viewtarifa where id_articulo = "+QString::number(oArticulo->id),
                         Configuracion_global->groupDB);
    if (modelTarifa->lastError().isValid())
        return;

    modelTarifa->setHeaderData(0,Qt::Horizontal,tr("id"));
    modelTarifa->setHeaderData(1,Qt::Horizontal,tr("CODIGO"));
    modelTarifa->setHeaderData(2,Qt::Horizontal,tr("DESCRIPCIÓN"));
    modelTarifa->setHeaderData(3,Qt::Horizontal,tr("PAIS"));
    modelTarifa->setHeaderData(4,Qt::Horizontal,tr("DIVISA"));
    modelTarifa->setHeaderData(5,Qt::Horizontal,tr("%MARG."));
    modelTarifa->setHeaderData(6,Qt::Horizontal,tr("%M. MÍN."));
    modelTarifa->setHeaderData(7,Qt::Horizontal,tr("PVP"));
    modelTarifa->setHeaderData(8,Qt::Horizontal,tr("PVP+IVA"));
    modelTarifa->setHeaderData(9,Qt::Horizontal,tr("S"));

    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(0,0);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(1,105);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(2,165);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(3,65);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(4,70);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(5,70);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(6,70);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(7,85);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(8,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(8,85);
    ui->TablaTarifas->horizontalHeader()->setSectionResizeMode(9,QHeaderView::Fixed);
    ui->TablaTarifas->horizontalHeader()->resizeSection(9,20);
}

void FrmArticulos::mostrarBusqueda()
{
    ui->stackedWidget->setCurrentIndex(1);
    _showBarraBusqueda(m_busqueda);
    m_busqueda->doFocustoText();
}

void FrmArticulos::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}

FrmArticulos::~FrmArticulos()
{
    delete ui;
}

void FrmArticulos::on_botAnadir_clicked()
{    
    QSqlQuery querySecciones(Configuracion_global->groupDB);
    querySecciones.prepare("select id from secciones order by id desc limit 1 ");
    if (querySecciones.exec())
    {
        if (!querySecciones.next())
        {
            QMessageBox::warning(this,tr("Gestión de artículos"),tr("Para poder añadir artículos debe tener registros de sección"));
        }
        else
        {
            ocultarBusqueda();
            bloquearCampos(false);

            VaciarCampos();
            oArticulo->Anadir();
            this->anadir = true;

            ui->Pestanas->setCurrentIndex(0);
            LLenarCampos(ui->Pestanas->currentIndex());

            if(Configuracion_global->auto_codigoart)
                ui->txtcodigo->setText("auto_codigo");

            ui->lblDescripcion->setText(tr("Nuevo artículo"));
            ui->lblCodigo->setText("");

            ui->lblDescripcion->repaint();
            ui->lblCodigo->repaint();

            ui->stackedWidget->setCurrentIndex(0);
            ui->txtcodigo->setFocus();
        }        
    }
}

void FrmArticulos::on_botGuardar_clicked()
{
    if(!ui->txtseccion->text().isEmpty())
    {
        bloquearCampos(true);
        CargarCamposEnArticulo();
        oArticulo->Guardar();
        actualizar();
    } else
    {
        QMessageBox::warning(this,tr("Gestion de Productos/servicios"),
                             tr("Es necesario que asocie el registro a una sección antes de poder guardar"),
                             tr("Aceptar"));
    }
}

void FrmArticulos::on_botSiguiente_clicked()
{
    if(oArticulo->Next())
    {
        LLenarCampos(ui->Pestanas->currentIndex());
        ui->botBorrar->setEnabled(true);
        ui->botAnterior->setEnabled(true);
        ui->botEditar->setEnabled(true);
        tarifa_model->setFilter("id_articulo = "+QString::number(oArticulo->id));
        tarifa_model->select();
    }
}

void FrmArticulos::on_botAnterior_clicked()
{
    if(oArticulo->Prev())
    {
        LLenarCampos(ui->Pestanas->currentIndex());
        ui->botBorrar->setEnabled(true);
        ui->botSiguiente->setEnabled(true);
        ui->botEditar->setEnabled(true);
        tarifa_model->setFilter("id_articulo = "+QString::number(oArticulo->id));
        tarifa_model->select();
    }
}

void FrmArticulos::bloquearCampos(bool state) {

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

    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(state);
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!state);
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(state);
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!state);
    }

    ui->botAnadir->setEnabled(state);
    ui->botAnterior->setEnabled(state);
    ui->botBorrar->setEnabled(state);
    ui->botDeshacer->setEnabled(!state);
    ui->botEditar->setEnabled(state);
    ui->botGuardar->setEnabled(!state);
    ui->botSiguiente->setEnabled(state);
    ui->btnKit->setEnabled(state);

    // Botones artículos
    ui->botBuscarSeccion->setEnabled(!state);
    ui->botBuscarFamilia->setEnabled(!state);
    ui->botBuscarSubfamilia->setEnabled(!state);
    ui->botBuscarSubSubFamilia->setEnabled(!state);
    ui->botCambiarImagen->setEnabled(!state);
    ui->botCambiarImagen_2->setEnabled(!state);
    ui->botCambiarImagen_3->setEnabled(!state);
    ui->botCambiarImagen_4->setEnabled(!state);
    ui->btnBorrarImagen_1->setEnabled(!state);
    ui->btnBorrarimagen_2->setEnabled(!state);
    ui->btnBorrarImagen_3->setEnabled(!state);
    ui->btnBorrarimagen_4->setEnabled(!state);
    ui->botBuscarGrupo->setEnabled(!state);
    ui->btnBuscarProveedor->setEnabled(!state);
    ui->btnAnadirProveedores->setEnabled(!state);
    ui->btnBorrarProveedores->setEnabled(!state);
    ui->btnEditarProveedorFrecuente->setEnabled(!state);
    ui->btnAsignarProveedor->setEnabled(!state);
    ui->chkValorGrafica->setEnabled(state);
    ui->cboTipoGrafica->setEnabled(state);
    ui->cboTipoGrafica_2->setEnabled(state);
    ui->radGrafica_importes_2->setEnabled(state);
    ui->radGrafica_unidades_2->setEnabled(state);
    ui->chkmostrarvalores_comparativa->setEnabled(state);
    ui->btnAnadirTarifa->setEnabled(true);
    ui->btnEditartarifa->setEnabled(true);
    ui->btnBorrarTarifa->setEnabled(true);
    ui->btnActivarOferta->setEnabled(!state);
    ui->btnBorrar_oferta->setEnabled(!state);


    //----------------------------------------------------
    // activo controles que deben estar activos.
    //----------------------------------------------------
    ui->chkValorGrafica->setEnabled(true);

    //----------------------------------------------------
    // desactivo controles que deben estar desactivados
    //----------------------------------------------------
    ui->chkOferta_32->setEnabled(false);
    ui->chkOferta_dto->setEnabled(false);
    ui->chkOferta_pvp->setEnabled(false);
    ui->chkOferta_web->setEnabled(false);

    m_busqueda->block(!state);

    ui->Pestanas->blockSignals(!state);
    m_busqueda->blockSignals(!state);

    if(state)
        emit unblock() ;
    else
        emit block();
}

void FrmArticulos::LLenarCampos(int index)
{
    //Widgets globales
    ui->lblkit->setVisible(oArticulo->kit);
    ui->lblCodigo->setText(oArticulo->codigo);
    ui->lblDescripcion->setText(oArticulo->descripcion);
    ui->lbl_en_promocion->setVisible(oArticulo->articulo_promocionado);

    if(index == 0 /*ui->Pestanas->currentWidget() == ui->tab_articulo*/)
    {
        //IVA desde config
        for(auto i=0; i< Configuracion_global->iva_model->rowCount(); i++){
            if(Configuracion_global->iva_model->record(i).value("id") == oArticulo->id_tipos_iva)
            {
                ui->cboTipoIVA->setCurrentText(Configuracion_global->iva_model->record(i).value("tipo").toString());
                break;
            }
        }
        //----------------
        // lleno campos
        //----------------
        ui->txtcodigo->setText(oArticulo->codigo);
        ui->txtcodigo_barras->setText(oArticulo->codigo_barras);
        ui->txtcodigo_fabricante->setText(oArticulo->codigo_fabricante);
        ui->txtdescripcion->setText(oArticulo->descripcion);
        ui->txtdescripcionResumida->setText(oArticulo->descripcion_reducida);
        ui->txtproveedor->setText(oArticulo->proveedor);
        ui->txtcodigo_proveedor->setText(oArticulo->cCodProveedor);
        ui->txtseccion->setText(oArticulo->getseccion(oArticulo->id_seccion));
        ui->txtfamilia->setText(oArticulo->getfamilia(oArticulo->id_familia));
        ui->txtsubfamilia->setText(oArticulo->getsubfamilia(oArticulo->id_subfamilia));
        ui->txtcSubSubFamilia->setText((oArticulo->getcSubSubFamilia(oArticulo->id_subSubFamilia)));
        ui->txtcGupoArt->setText(oArticulo->getcGrupo(oArticulo->id_grupoart));
        ui->txtCoste_real->setText(Configuracion_global->toFormatoMoneda(QString::number(oArticulo->coste_real,'f',Configuracion_global->decimales)));
        ui->txtdto->setText(QString::number(oArticulo->porc_dto,'f',Configuracion_global->decimales));
        ui->txtcoste->setText(Configuracion_global->toFormatoMoneda(QString::number(oArticulo->coste,'f',Configuracion_global->decimales)));
        ui->chkmostrar_web->setChecked(oArticulo->mostrar_web==1);
        ui->txtMargen->setValue(oArticulo->margen);
        ui->txtMargen_min->setValue((oArticulo->margen_min));

        //---------------------
        // Tarifas
        //---------------------
        llenar_tabla_tarifas();
    }

    else if(index == 1 /*ui->Pestanas->currentWidget()==ui->tab_proveedores*/)
    {
        // Proveedores frecuentes
        modelProv->setQuery("Select id,cod_pro,proveedor,codigo,pvd,pvd_real,moneda,descoferta,id_prov from proveedores_frecuentes "
                            "where id_art = "+QString::number(oArticulo->id),
                            Configuracion_global->groupDB);

        if (modelProv->lastError().isValid())
            return;

        modelProv->setHeaderData(1,Qt::Horizontal,tr("COD.PROV."));
        modelProv->setHeaderData(2,Qt::Horizontal,tr("PROVEEDOR"));
        modelProv->setHeaderData(3,Qt::Horizontal,tr("COD.ART.PRO."));
        modelProv->setHeaderData(4,Qt::Horizontal,tr("COSTE"));
        modelProv->setHeaderData(5,Qt::Horizontal,tr("C.REAL"));
        modelProv->setHeaderData(6,Qt::Horizontal,tr("DIVISA"));
        modelProv->setHeaderData(7,Qt::Horizontal,tr("D.OFERTA"));
        ui->tablaProveedores->setColumnHidden(0,true);
        ui->tablaProveedores->setColumnHidden(8,true);

        ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
        ui->tablaProveedores->horizontalHeader()->resizeSection(1,90);
        ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
        ui->tablaProveedores->horizontalHeader()->resizeSection(2,130);
        ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);
        ui->tablaProveedores->horizontalHeader()->resizeSection(3,95);
        ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Fixed);
        ui->tablaProveedores->horizontalHeader()->resizeSection(4,80);
        ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Fixed);
        ui->tablaProveedores->horizontalHeader()->resizeSection(5,80);
        ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Fixed);
        ui->tablaProveedores->horizontalHeader()->resizeSection(6,90);
        ui->tablaProveedores->horizontalHeader()->setSectionResizeMode(7,QHeaderView::Fixed);
        ui->tablaProveedores->horizontalHeader()->resizeSection(7,87);

        // Grafica proveedores
        ui->graf_prov->Clear();
        rellenar_grafica_proveedores();
    }

    else if(index == 2 /*ui->Pestanas->currentWidget() == ui->tab_promociones*/)
    {
        ui->chkArticulo_promocionado->setChecked(oArticulo->articulo_promocionado);
        ui->framePromocion->setEnabled(false);

        ui->chkMostrar_en_cuadro->setChecked(oArticulo->mostrar_en_cuadro);


        //-----------------------------
        // LLENO  OFERTAS/promociones
        //-----------------------------
        promociones->setQuery(QString("select id, activa, descripcion from articulos_ofertas where id_articulo = %1 order by activa desc").arg(oArticulo->id),
                              Configuracion_global->empresaDB);
        ui->tabla_ofertas->selectRow(0);
    }

    else if(index == 3 /*ui->Pestanas->currentWidget() == ui->tab_precios_volumen*/)
    {

    }


    else if(index == 4 /*web*/)
    {
        //--------------------------------------------------------------
        // Recuperamos datos articulo distintivos según web/idioma/pais
        //--------------------------------------------------------------
        // Mostramos el primer pais que encontramos para el artículo
        //--------------------------------------------------------------

    }
    else if(index == 5 /*imagenes*/)
    {
        //-----------------------------
        // Recuperamos imagen desde BD
        //-----------------------------
        ui->lblImagenArticulo_1->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
        ui->lblImagenArticulo_2->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
        ui->lblImagenArticulo_3->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
        ui->lblImagenArticulo_4->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
        oArticulo->CargarImagen(ui->lblImagenArticulo_1,ui->lblImagenArticulo_2,
                                ui->lblImagenArticulo_3,ui->lblImagenArticulo_4);
    }



    else if(index == 6 /*ui->Pestanas->currentWidget() == ui->tab_estadistica*/)
    {
        ui->txtfecha_fecha_ultima_compra->setDate(oArticulo->fecha_ultima_compra);
        ui->txtfechaUltimaVenta->setDate(oArticulo->fecha_ultima_venta);
        ui->txtunidades_compradas->setText(QString::number(oArticulo->unidades_compradas));
        ui->txtunidades_vendidas->setText(QString::number(oArticulo->unidades_vendidas));
        ui->txtimporte_acumulado_compras->setText(Configuracion_global->toFormatoMoneda(QString::number(oArticulo->importe_acumulado_compras,'f',Configuracion_global->decimales)));
        ui->txtimporte_acumulado_ventas->setText(Configuracion_global->toFormatoMoneda(QString::number(oArticulo->importe_acumulado_ventas,'f',Configuracion_global->decimales)));
        ui->txtcomentario->setText(oArticulo->comentario);
        ui->txtstock_maximo->setText(QString::number(oArticulo->stock_maximo));
        ui->txtstock_minimo->setText(QString::number(oArticulo->stock_minimo));
        //ui->txtstock_real->setText(QString::number(oArticulo->stock_real));
        ui->txtstock_real_2->setText(QString::number(oArticulo->stock_real));
        ui->txtstock_fisico_almacen->setText(QString::number(oArticulo->nstock_fisico_almacen));

        if (oArticulo->controlar_stock==1)
            ui->chkcontrolar_stock->setChecked(true);
        else
            ui->chkcontrolar_stock->setChecked(false);

        ui->txtcantidad_pendiente_recibir->setText(QString::number(oArticulo->cantidad_pendiente_recibir));
        ui->txtfecha_prevista_recepcion->setDate(oArticulo->fecha_prevista_recepcion);
        ui->txtunidades_reservadas->setText(QString::number(oArticulo->unidades_reservadas));
        GraficaImportes();
    }

    else if(index == 7 /*ui->Pestanas->currentWidget() == ui->tab_grafica*/)
    {
        LLenarGraficas();
        if(ui->radGrafica_importes->isChecked())
            GraficaImportes();
        else
            GraficaUnidades();
    }

    else if(index == 8 /*ui->Pestanas->currentWidget() == ui->tab_comparativa*/)
    {
        LLenarGrafica_comparativa(1);
    }

    else if(index == 9/*ui->Pestanas->currentWidget() == ui->tab_Trazabilidad*/)
    {
        // -------------------
        // TABLA TRAZABILIDAD
        // -------------------

        modelTrazabilidad1->setQuery( "select * from viewTrazabilidad1 where id_articulo = "+QString::number(oArticulo->id),
                                      Configuracion_global->groupDB);
        if (modelTrazabilidad1->lastError().isValid())
            return;

        ui->tablaLotes->setColumnHidden(0,true);
        ui->tablaLotes->setColumnHidden(6,true);
        modelTrazabilidad1->setHeaderData(1,Qt::Horizontal,tr("LOTE"));
        modelTrazabilidad1->setHeaderData(2,Qt::Horizontal,tr("CANTIAD"));
        modelTrazabilidad1->setHeaderData(3,Qt::Horizontal,tr("STOCK LOTE"));
        modelTrazabilidad1->setHeaderData(4,Qt::Horizontal,tr("STOCK REAL"));
        modelTrazabilidad1->setHeaderData(5,Qt::Horizontal,tr("FAC.COMPRA"));
        modelTrazabilidad1->setHeaderData(6,Qt::Horizontal,tr("ID"));
        modelTrazabilidad1->setHeaderData(7,Qt::Horizontal,tr("FEC.COMPRA"));
        modelTrazabilidad1->setHeaderData(8,Qt::Horizontal,tr("CADUCIDAD"));
        modelTrazabilidad1->setHeaderData(9,Qt::Horizontal,tr("PROVEEDOR"));
        modelTrazabilidad1->setHeaderData(10,Qt::Horizontal,tr("DESCRIPCIÓN"));
        modelTrazabilidad1->setHeaderData(11,Qt::Horizontal,tr("CÓDIGO"));
    }
}

void FrmArticulos::CargarCamposEnArticulo()
{
    oArticulo->codigo= ui->txtcodigo->text();
    oArticulo->codigo_barras =ui->txtcodigo_barras->text();
    oArticulo->codigo_fabricante=ui->txtcodigo_fabricante->text();
    oArticulo->descripcion =ui->txtdescripcion->toPlainText();
    oArticulo->descripcion_reducida = ui->txtdescripcionResumida->text();
    oArticulo->familia=ui->txtfamilia->text();
    oArticulo->seccion=ui->txtseccion->text();
    oArticulo->subfamilia=ui->txtsubfamilia->text();
    oArticulo->tipo_iva=Configuracion_global->ivas[ui->cboTipoIVA->currentText()].value("iva").toDouble();
    oArticulo->id_tipos_iva = Configuracion_global->getidIva(ui->cboTipoIVA->currentText());
    oArticulo->coste=ui->txtcoste->text().replace(".","").replace(",",".").toDouble();
    oArticulo->fecha_ultima_compra= ui->txtfecha_fecha_ultima_compra->date();
    oArticulo->fecha_ultima_venta= ui->txtfechaUltimaVenta->date();
    oArticulo->unidades_compradas= ui->txtunidades_compradas->text().toDouble();
    oArticulo->unidades_vendidas=ui->txtunidades_vendidas->text().toDouble();
    oArticulo->importe_acumulado_compras= ui->txtimporte_acumulado_compras->text().replace(".","").replace(",",".").toDouble();
    oArticulo->importe_acumulado_ventas= ui->txtimporte_acumulado_ventas->text().replace(".","").replace(",",".").toDouble();
    oArticulo->comentario=ui->txtcomentario->toPlainText();
    oArticulo->stock_maximo=ui->txtstock_maximo->text().toInt();
    oArticulo->stock_minimo=ui->txtstock_minimo->text().toInt();

  //  oArticulo->stock_real=ui->txtstock_real->text().toInt();

    if (ui->chkcontrolar_stock->isChecked())
       oArticulo->controlar_stock=1;
    else
        oArticulo->controlar_stock=0;

    oArticulo->cantidad_pendiente_recibir=ui->txtcantidad_pendiente_recibir->text().toInt();
    oArticulo->fecha_prevista_recepcion =ui->txtfecha_prevista_recepcion->date();
    oArticulo->unidades_reservadas =ui->txtunidades_reservadas->text().toInt();
    if (ui->chkmostrar_web->isChecked())
        oArticulo->mostrar_web = 1;
    else
        oArticulo->mostrar_web = 0;
    oArticulo->id_tipos_iva = Configuracion_global->getidIva(ui->cboTipoIVA->currentText());
    oArticulo->id_seccion = oArticulo->getidSeccion(ui->txtseccion->text());
    oArticulo->id_familia  = oArticulo->getidFamilia(ui->txtfamilia->text());
    oArticulo->id_subfamilia = oArticulo->getidSubFamilia(ui->txtsubfamilia->text());
    oArticulo->id_subSubFamilia = oArticulo->getidSubSufFamilia(ui->txtcSubSubFamilia->text());
    oArticulo->id_grupoart = oArticulo->getidGrupo(ui->txtcGupoArt->text());
    oArticulo->cCodProveedor = ui->txtcodigo_proveedor->text();
    oArticulo->proveedor = ui->txtproveedor->text();

//    this->id_web = registro.field("id_web").value().toInt();
  //  oArticulo->stock_fisico_almacen = ui->txtstock_fisico_almacen->text().toInt();
    oArticulo->articulo_promocionado = ui->chkArticulo_promocionado->isChecked();
    oArticulo->porc_dto = ui->txtdto->text().toFloat();
    oArticulo->coste_real = ui->txtCoste_real->text().replace(".","").replace(",",".").toDouble();
    oArticulo->mostrar_en_cuadro = ui->chkMostrar_en_cuadro->isChecked();
   // oArticulo->margen = ui->txtMargen->value();
   // oArticulo->margen_min = ui->txtMargen_min->value();

}

void FrmArticulos::VaciarCampos()
{
   oArticulo->Vaciar();

   ui->txtcodigo->setText("");
   ui->txtcodigo_barras->setText("");
   ui->txtcodigo_fabricante->setText("");
   ui->txtdescripcion->setText("");
   ui->txtdescripcionResumida->setText("");
   ui->txtproveedor->setText("");
   ui->txtfamilia->setText("");
   ui->txtseccion->setText("");
   ui->txtsubfamilia->setText("");
   ui->cboTipoIVA->setEditText("");
   ui->txtcoste->setText("0,00");
   ui->txtdto->setText("0");
   ui->txtfecha_fecha_ultima_compra->setDate(QDate::currentDate());
   ui->txtfechaUltimaVenta->setDate(QDate::currentDate());
   ui->txtunidades_compradas->setText("0");
   ui->txtunidades_vendidas->setText("0");
   ui->txtimporte_acumulado_compras->setText("0,00");
   ui->txtimporte_acumulado_ventas->setText("0,00");
   ui->txtcomentario->setText("");
   ui->txtstock_maximo->setText("0");
   ui->txtstock_minimo->setText("0");
  // ui->txtstock_real->setText("0");
   ui->txtstock_real_2->setText("0");
  // ui->txtstock_fisico_almacen->setText("0");
   ui->chkcontrolar_stock->setChecked(false);
   ui->txtcantidad_pendiente_recibir->setText("0");
   ui->txtfecha_prevista_recepcion->setDate(QDate::currentDate());
   ui->txtunidades_reservadas->setText("0");
   ui->chkmostrar_web->setChecked(false);
   ui->txtcodigo_proveedor->setText("");
   ui->chkArticulo_promocionado->setChecked(false);
   ui->txtoferta_pvp_fijo->setText("0");
   ui->txtCoste_real->setText("0,00");
   //ui->txtMargen->setValue(0);
   //ui->txtMargen_min->setValue(0);


}

void FrmArticulos::ChangeValues_TablaProveedores(int row, int column)
{
    if (column == 4)
       TimedMessageBox * t = new TimedMessageBox(this,
                                                 tr("Se ha cambiado el valor"));
//    else if(column == 1 && !helped_table->item(row,0)->text().isEmpty())
//        comprobarCantidad(row);
//    else if(column == 5 && !helped_table->item(row,4)->text().isEmpty())
//        comprobadto(row);
//    else if(column == 9)
//        comprobarStock(row);

    rellenar_grafica_proveedores();
}

void FrmArticulos::filter_table(QString texto, QString orden, QString modo)
{
    if(ui->stackedWidget->currentIndex() == 0)
        ui->stackedWidget->setCurrentIndex(1);

    // busquedas << tr("Descripción") <<tr("Código") <<tr("Código Barras") <<tr("Ref. Proveedor");
    QHash <QString, QString> lista;
    lista[tr("Descripción")] = "descripcion";
    lista[tr("Código")] = "codigo";
    lista[tr("Código Barras")] = "codigo_barras";
    lista[tr("Ref. Proveedor")] = "codigo_proveedor";

    QString campo = lista.value(orden);
    QString mode;
    if(modo == tr("A-Z"))
        mode = "";
    else
        mode ="DESC";

QString cSQL = "Select id,codigo, descripcion_reducida,  codigo_barras,codigo_fabricante,tipo_iva,kit, pvp,pvp_con_iva,stock_fisico_almacen from vistaart_tarifa where "+
        campo+" like '%"+texto.trimmed()+"%' and tarifa ="+
        QString::number(Configuracion_global->id_tarifa_predeterminada)+" order by "+campo +" "+mode;

    modelBusqueda->setQuery(cSQL,Configuracion_global->groupDB);
    if(modelBusqueda->rowCount() == 0)
    {
        QString cSQL2 = "Select id,codigo, descripcion_reducida, codigo_barras,codigo_fabricante,tipo_iva,kit,stock_fisico_almacen from articulos where "+
                campo+" like '%"+texto.trimmed()+"%' order by "+campo +" "+mode;
        modelBusqueda->setQuery(cSQL2, Configuracion_global->groupDB);
    }

    format_tables();
    ui->tablaBusqueda->selectRow(0);
}

void FrmArticulos::rellenar_grafica_proveedores()
{
    //----------------------
    // GRAFICA SEGÚN pvd_real
    //----------------------
    QSqlQuery queryProveed("select proveedor,pvd_real from proveedores_frecuentes where id_art = "+
                           QString::number(oArticulo->id),Configuracion_global->groupDB);
    if(queryProveed.exec()){

        while (queryProveed.next()) {
            ui->graf_prov->addItem(queryProveed.record().value("proveedor").toString().left(4),
                                   queryProveed.record().value("pvd_real").toFloat()/*,Qt::blue*/);


        }

        ui->graf_prov->repaint();
    }
    else
        qDebug()<< queryProveed.lastError();

}

void FrmArticulos::on_botEditar_clicked()
{
    if(sender() != ui->botEditar)
    {
        QModelIndex index = ui->tablaBusqueda->currentIndex();
        if(!index.isValid())
            return;
        int id = ui->tablaBusqueda->model()->index(index.row(),0).data().toInt();
        oArticulo->Recuperar(id);
    }

    for(auto i= 0; i < ui->Pestanas->count(); i++)
        LLenarCampos(i);

    this->anadir = false;

    ui->stackedWidget->setCurrentIndex(0);
    bloquearCampos(false);
    ui->txtcodigo->setReadOnly(true);

    //-------------------
    // OFERTAS
    //-------------------
    bool promoted = ui->chkArticulo_promocionado->isChecked();
    ui->framePromocion->setEnabled(promoted);
    ui->btnAnadir_oferta->setEnabled(promoted);
    ui->btnEditarOferta->setEnabled(promoted);
    ui->btnBorrar_oferta->setEnabled(promoted);
    ocultarBusqueda();        
}

void FrmArticulos::on_botBorrar_clicked()
{
    int id = oArticulo->id;
    bool isKit = oArticulo->kit;
    QString codigo = oArticulo->codigo;
    if(sender() != ui->botBorrar)
    {
        QModelIndex index = ui->tablaBusqueda->currentIndex();
        if(!index.isValid())
            return;
        id = ui->tablaBusqueda->model()->index(index.row(),0).data().toInt();
        isKit = ui->tablaBusqueda->model()->index(index.row(),6).data().toBool();
        codigo = ui->tablaBusqueda->model()->index(index.row(),1).data().toString();
    }
    oArticulo->Borrar(id,isKit,true,codigo);

    LLenarCampos(ui->Pestanas->currentIndex());
    if(sender() != ui->botBorrar)
    {
        QString cSQL = "select id,codigo, descripcion, codigo_barras,codigo_fabricante,tipo_iva,kit, pvp,pvp_con_iva,,stock_fisico_almacen from vistaart_tarifa "
                "where tarifa ="+QString::number(Configuracion_global->id_tarifa_predeterminada);
        modelBusqueda->setQuery(cSQL, Configuracion_global->groupDB);

        if(modelBusqueda->rowCount() == 0)
            modelBusqueda->setQuery("Select id,codigo, descripcion, codigo_barras,codigo_fabricante,tipo_iva,kit,stock_fisico_almacen from articulos", Configuracion_global->groupDB);
    }
}

void FrmArticulos::on_botDeshacer_clicked()
{
    if(this->anadir)
    {
        oArticulo->Borrar(oArticulo->id,false,false);
        LLenarCampos(ui->Pestanas->currentIndex());
    }
    else
    {
        LLenarCampos(ui->Pestanas->currentIndex());
    }
    bloquearCampos(true);
}

void FrmArticulos::AnadirSeccion()
{
    QLineEdit *txtseccionNueva = new QLineEdit(ventana);
    layout->addWidget(txtseccionNueva,2,1,2,1);
}

bool FrmArticulos::eventFilter(QObject *target, QEvent *event)
{
    if(target == this && event->type() == QEvent::Hide)
    {
       ui->Pestanas->setCurrentIndex(0);
    }
    else if(event->type() == QEvent::Resize)
        _resizeBarraBusqueda(m_busqueda);

    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if(event->type() == QEvent::KeyPress)
    {
        if(keyEvent->key() == Qt::Key_Return && target == ui->tablaBusqueda)
        {
            on_tablaBusqueda_doubleClicked(ui->tablaBusqueda->currentIndex());
            return true;
        }
         if(keyEvent->key() == Qt::Key_Escape)
            return true;
        if(keyEvent->key() == Qt::Key_F1)
        {
            if(ui->botEditar->isEnabled())
            {
                if(m_busqueda->isShow())
                    ocultarBusqueda();
                else
                    mostrarBusqueda();

            }else
            {
                if(target->objectName()=="txtcodigo_proveedor")
                {
                    on_btnBuscarProveedor_clicked();
                } else
                {
                    QMessageBox::warning(this,tr("Gestión artículos"),tr("No puede buscar mientras está editando"),
                                     tr("Aceptar"));
                }
            }
            return true;
        }
    }
    return MayaModule::eventFilter(target,event);
}

void FrmArticulos::on_botBuscarSeccion_clicked()
{
    db_consulta_view consulta(this);
    QStringList campos;
    campos <<"id" << "codigo" <<"seccion";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Secciones");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,codigo, seccion from secciones");
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Id") <<tr("código") << tr("sección");
    tamanos <<"0" << "100"  <<"500";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Secciones");
    consulta.setFocus();
    if(consulta.exec())
    {
        oArticulo->id_seccion = consulta.get_id();
        oArticulo->seccion = consulta.get_record().value("seccion").toString();
        oArticulo->cod_seccion = consulta.get_record().value("codigo").toString();
        ui->txtseccion->setText(consulta.get_record().value("seccion").toString());
    }
}

void FrmArticulos::on_botBuscarFamilia_clicked()
{
    db_consulta_view consulta(this);
    QStringList campos;
    campos <<"id" << "codigo" <<"familia";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Familias");
    consulta.set_db("Maya");
    consulta.set_SQL(QString("select id,codigo, familia from familias where id_seccion = %1").arg(oArticulo->id_seccion));
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Id") <<tr("código") << tr("Familia");
    tamanos <<"0" << "100"  <<"500";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Familias");
    consulta.setFocus();
    if(consulta.exec())
    { 
        oArticulo->id_familia = consulta.get_id();
        oArticulo->familia = consulta.get_record().value("familia").toString();
        oArticulo->cod_familia = consulta.get_record().value("codigo").toString();
        ui->txtfamilia->setText(consulta.get_record().value("familia").toString());
    }
}

void FrmArticulos::on_botBuscarSubfamilia_clicked()
{
    db_consulta_view consulta(this);
    QStringList campos;
    campos <<"id" << "codigo" <<"sub_familia";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Sub-Familias");
    consulta.set_db("Maya");
    consulta.set_SQL(QString("select id,codigo, sub_familia from subfamilias where id_familia = %1").arg(oArticulo->id_familia));
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Id") <<tr("código") << tr("Sub-Familia");
    tamanos <<"0" << "100"  <<"500";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Sub-Familias");
    consulta.setFocus();
    if(consulta.exec())
    {
        oArticulo->id_subfamilia = consulta.get_id();
        oArticulo->subfamilia = consulta.get_record().value("sub_familia").toString();
        oArticulo->cod_subfamilia = consulta.get_record().value("codigo").toString();
        ui->txtsubfamilia->setText(consulta.get_record().value("sub_familia").toString());
    }
}

void FrmArticulos::on_botBuscarSubSubFamilia_clicked()
{
    db_consulta_view consulta(this);
    QStringList campos;
    QString error;
    campos <<"id" << "codigo" <<"subsub_familia";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Sub-Sub-Familias");
    consulta.set_db("Maya");
    consulta.set_SQL(QString("select id,codigo, subsub_familia from subsubfamilias where id_subfamilia = %1").arg(oArticulo->id_subfamilia));
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Id") <<tr("código") << tr("Sub-Sub-Familia");
    tamanos <<"0" << "100"  <<"500";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Sub-Sub-Familias");
    consulta.setFocus();
    if(consulta.exec())
    {
        oArticulo->id_subSubFamilia = consulta.get_id();
        oArticulo->cSubSubFamilia = consulta.get_record().value("subsub_familia").toString();
        oArticulo->cod_SubSubFamilia = consulta.get_record().value("codigo").toString();
        ui->txtcSubSubFamilia->setText(consulta.get_record().value("subsub_familia").toString());
    }
}

void FrmArticulos::on_btnBuscarProveedor_clicked()
{
    db_consulta_view consulta(this);
    QStringList campos;

    campos << "proveedor" << "codigo" <<"cif" <<"telefono1"  << "movil";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Proveedores");
    consulta.set_db("Maya");
    consulta.set_SQL("select id, proveedor, codigo,cif,telefono1,movil from proveedores");
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Proveedor") <<tr("código") << tr("cif/nif/nie") << tr("Teléfono ") << tr("móvil");
    tamanos <<"0" << "300"  <<"100" << "100" << "120" <<"120";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Proveedores");
    consulta.setFocus();
    if(consulta.exec())
    {
        oArticulo->idProveedor = consulta.get_id();
        oArticulo->proveedor = consulta.get_record().value("proveedor").toString();
        oArticulo->cCodProveedor = consulta.get_record().value("codigo").toString();
        ui->txtcodigo_proveedor->setText(consulta.get_record().value("codigo").toString());
        ui->txtproveedor->setText(consulta.get_record().value("proveedor").toString());
    }
}

void FrmArticulos::on_btnAnadirTarifa_clicked()
{
    FrmTarifas addTarifa(this);
    addTarifa.capturar_coste(ui->txtCoste_real->text().replace(".","").replace(",",".").toFloat());

    if(addTarifa.exec() ==QDialog::Accepted)
    {
        QHash<QString,QVariant> _data;
        _data["id_articulo"]= oArticulo->id;
        _data["id_pais"]= addTarifa.id_pais;
        _data["id_monedas"]= addTarifa.id_moneda;
        _data["margen"]= addTarifa.margen;
        _data["margen_minimo"]= addTarifa.margen_min;
        _data["pvp"]= addTarifa.pvpDivisa;
        _data["id_codigo_tarifa"]= addTarifa.codigoTarifa;

        QString error;
        if(SqlCalls::SqlInsert(_data,"tarifas",Configuracion_global->groupDB,error) < 0)
        {
            QMessageBox::information(this,tr("Gestión de Productos/Servicios"),
                                      tr("Ocurrió un error al insertar una tarifa en el artículo: %1").arg(error),
                                     tr("Acceptar"));
        }
        else
        {
            llenar_tabla_tarifas();
        }
    }
}


void FrmArticulos::btnEditarTarifa_clicked()
{
    if(!ui->TablaTarifas->currentIndex().isValid())
        return;
    int id_t = modelTarifa->data(modelTarifa->index(ui->TablaTarifas->currentIndex().row(),0),Qt::DisplayRole).toInt();

    FrmTarifas editTarifa(this);
    editTarifa.capturar_datos(id_t,ui->txtCoste_real->text(),oArticulo->id);

    if(editTarifa.exec() ==QDialog::Accepted)
    {
        QString error;
        QHash<QString,QVariant> _data;
        _data["margen"] = editTarifa.margen;
        _data["margen_minimo"]= editTarifa.margen_min;
        _data["pvp"] = editTarifa.pvpDivisa;

        if(!SqlCalls::SqlUpdate(_data,"tarifas",Configuracion_global->groupDB,QString("id = %1").arg(id_t),error))
            QMessageBox::warning(this,tr("ATENCIÓN"),
                                 tr("Ocurrió un error al actualizar BD: %1").arg(error),
                                 tr("Aceptar"));
        if(ui->TablaTarifas->currentIndex().row() == 0) //Es tarifa pvp
        {
            QHash<QString,QVariant> _dat;
            _dat["pvp"] = editTarifa.pvpDivisa;
            if(!SqlCalls::SqlUpdate(_dat,"articulos",Configuracion_global->groupDB,QString("id = %1").arg(oArticulo->id),error))
                QMessageBox::warning(this,tr("ATENCIÓN"),
                                     tr("Ocurrió un error al actualizar BD: %1").arg(error),
                                     tr("Aceptar"));
        }
        llenar_tabla_tarifas();
    }
}

void FrmArticulos::btnBorrarTarifa_clicked()
{
    if(!ui->TablaTarifas->currentIndex().isValid())
        return;

    QString s = modelTarifa->data(modelTarifa->index(ui->TablaTarifas->currentIndex().row(),1),Qt::DisplayRole).toString();
    if(s == "PVP")
        return; //Prohibido borrar tarifa 1 (PVP)

    int id_t = modelTarifa->data(modelTarifa->index(ui->TablaTarifas->currentIndex().row(),0),Qt::DisplayRole).toInt();

    if(QMessageBox::question(this,tr("Borrar Tarifa"),
                             tr("¿Desea realmente borrar esta tarifa para este artículo?"),
                             tr("No"),
                             tr("Borrar"))==QMessageBox::Accepted)
    {
        QString error;
        if(!SqlCalls::SqlDelete("tarifas",Configuracion_global->groupDB,
                                QString("id = %1").arg(id_t),
                                error))
            QMessageBox::warning(this,tr("Ocurrió un error al borrar"),
                                 error);
        llenar_tabla_tarifas();
    }

}

void FrmArticulos::anadir_proveedor_clicked()
{
    FrmAsociarProveedor frmAsociar;
    frmAsociar.setAnadir();
    if(frmAsociar.exec() == QDialog::Accepted)
    {
        bool ok = oArticulo->agregar_proveedor_alternativo(oArticulo->id,frmAsociar.id_proveedor,frmAsociar.codigo,frmAsociar.pvd,
                                                 frmAsociar.DescOferta,frmAsociar.Oferta,frmAsociar.pvd_real,frmAsociar.id_divisa);
        if (!ok)
            QMessageBox::warning(this,tr("Nuevo proveedor frecuente"),tr("Falló la inserción del proveedor"),tr("Aceptar"));
        LLenarCampos(ui->Pestanas->currentIndex());
    }

    //-----------------------
    // Proveedores frecuentes
    //-----------------------
    modelProv->setQuery("Select id,cod_pro,proveedor,codigo,pvd,pvd_real,moneda,descoferta from proveedores_frecuentes where id_art = "+
                        QString::number(oArticulo->id),
                        Configuracion_global->groupDB);    
    ui->tablaProveedores->setColumnHidden(0,true);
}

void FrmArticulos::editar_proveedor_clicked()
{
    QModelIndex celda=ui->tablaProveedores->currentIndex();
    QModelIndex index1=modelProv->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=modelProv->data(index1,Qt::EditRole);

    FrmAsociarProveedor frmAsociar;
    frmAsociar.seteditar(pKey.toString());
    if(frmAsociar.exec() == QDialog::Accepted) {
        bool ok = oArticulo->guardarProveedorAlternativo(pKey.toInt(),frmAsociar.codigo,frmAsociar.pvd,
                                                 frmAsociar.DescOferta,frmAsociar.Oferta,frmAsociar.pvd_real,frmAsociar.id_divisa);
        if (!ok)
            QMessageBox::warning(this,tr("Guardar proveedor frecuente"),tr("Falló la edición del proveedor"),tr("Aceptar"));
        LLenarCampos(ui->Pestanas->currentIndex());

    }
}

void FrmArticulos::borrar_proveedor_clicked()
{
    QModelIndex celda=ui->tablaProveedores->currentIndex();
    QModelIndex index1=modelProv->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=modelProv->data(index1,Qt::EditRole);


    QMessageBox mensaje(this);
    mensaje.setStyleSheet("background-color: rgb(221,221,221)");
    mensaje.setWindowTitle(tr("Borrar proveedor frecuente"));
    mensaje.setText(tr("¿Desea borrar la ficha del proveedor ?"));
    mensaje.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    mensaje.setButtonText(0,tr("No"));
    mensaje.setButtonText(1,tr("Sí"));

    mensaje.setIcon(QMessageBox::Question);

    if(mensaje.exec()==QMessageBox::Yes) {
        QSqlQuery queryProv(Configuracion_global->groupDB);
        queryProv.prepare("delete from articulos_prov_frec where id =:id");
        queryProv.bindValue(":id",pKey.toString());
        if(!queryProv.exec())
            QMessageBox::warning(this,tr("Borrar proveedor frecuente"),
                                 tr("Se ha producido un error al borrar el proveedor frecuente: %1").arg(queryProv.lastError().text()),
                                 tr("Aceptar"));
        else
            LLenarCampos(ui->Pestanas->currentIndex());
    }
}

void FrmArticulos::asignar_proveedor_principal_clicked()
{
    QModelIndex celda=ui->tablaProveedores->currentIndex();
    QModelIndex index1=modelProv->index(celda.row(),8);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=modelProv->data(index1,Qt::EditRole);


//    QMessageBox mensaje(this);
//    mensaje.setStyleSheet("background-color: rgb(221,221,221)");
//    mensaje.setWindowTitle(tr("Assignar proveedor"));
//    mensaje.setText(tr("¿Desea hacer de este proveedor el principal?"));
//    mensaje.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
//    mensaje.setButtonText(0,tr("No"));
//    mensaje.setButtonText(1,tr("Sí"));
    if (QMessageBox::question(this,tr("Re-asignar proveedor"),
                          tr("¿Desea cambiar el proveedor principal?"),tr("No"),tr("Sí")) == QMessageBox::Accepted){

        bool resultado = oArticulo->cambiarProveedorPrincipal(oArticulo->id,pKey.toInt());
        if(resultado)

            index1=modelProv->index(celda.row(),1);     ///< '0' es la posicion del registro que nos interesa
            pKey=modelProv->data(index1,Qt::EditRole);
            ui->txtcodigo_proveedor->setText(pKey.toString());
            index1=modelProv->index(celda.row(),2);
            pKey=modelProv->data(index1,Qt::EditRole);
            ui->txtproveedor->setText(pKey.toString());
    }

}

void FrmArticulos::trazabilidad2(int id)
{    
    modelTrazabilidad2->setQuery("select codigocuentacliente,cliente, documento_venta,numero_ticket,unidades_vendidas,"
                                 "fecha_venta from trazabilidad2 where id_trazabilidad1 ="+QString::number(id),
                                 Configuracion_global->groupDB);    
    modelTrazabilidad2->setHeaderData(0,Qt::Horizontal,tr("C.CLIENTE"));
    modelTrazabilidad2->setHeaderData(1,Qt::Horizontal,tr("CLIENTE"));
    modelTrazabilidad2->setHeaderData(2,Qt::Horizontal,tr("DOCUMENTO"));
    modelTrazabilidad2->setHeaderData(3,Qt::Horizontal,tr("Ticket"));
    modelTrazabilidad2->setHeaderData(4,Qt::Horizontal,tr("UNid. VEND."));
    modelTrazabilidad2->setHeaderData(5,Qt::Horizontal,tr("FECHA VENTA"));
}

void FrmArticulos::TablaTrazabilidad_clicked(QModelIndex)
{
    trazabilidad2(1);
}

void FrmArticulos::graficar(QString Tipo)
{
    if(ui->radGrafica_unidades->isChecked())
       GraficaUnidades();
    else
       GraficaImportes();
}

void FrmArticulos::GraficaUnidades()
{

    if(ui->cboTipoGrafica->currentText()==tr("Grafica de Barras")) {
        ui->grafica->setTipo(OpenChart::BarraMultiple);
        ui->grafica->Clear();
        ui->grafica->addMulibarColor("Ventas",Qt::darkGreen);
        ui->grafica->addMulibarColor("Compras",Qt::darkBlue);
        QVector <float> enero;
        enero << ui->txtUnid_compras_enero->text().toInt()<<ui->txtUnid_ventas_enero->text().toInt();

        ui->grafica->addItem("Ene",enero);

        QVector <float> febrero;
        febrero <<ui->txtUnid_compras_febrero->text().toInt() <<ui->txtUnid_ventas_febrero->text().toInt();

        ui->grafica->addItem("Feb",febrero);

        QVector <float> marzo;
        marzo <<ui->txtUnid_compras_marzo->text().toInt() <<ui->txtUnid_ventas_marzo->text().toInt();

        ui->grafica->addItem("Mar",marzo);

        QVector <float> abril;
        abril <<ui->txtUnid_compras_abril->text().toInt() <<ui->txtUnid_ventas_abril->text().toInt();

        ui->grafica->addItem("Abr",abril);

        QVector <float> mayo;
        mayo <<ui->txtUnid_compras_mayo->text().toInt() <<ui->txtUnid_ventas_mayo->text().toInt();

        ui->grafica->addItem("May",mayo);

        QVector <float> junio;
        junio <<ui->txtUnid_compras_junio->text().toInt() <<ui->txtUnid_ventas_junio->text().toInt();

        ui->grafica->addItem("jun",junio);

        QVector <float> julio;
        julio <<ui->txtUnid_compras_julio->text().toInt() <<ui->txtUnid_ventas_julio->text().toInt();

        ui->grafica->addItem("Jul",julio);

        QVector <float> agosto;
        agosto <<ui->txtUnid_compras_agosto->text().toInt() <<ui->txtUnid_ventas_agosto->text().toInt();
        ui->grafica->addItem("Ago",agosto);

        QVector <float> septiembre;
        septiembre <<ui->txtUnid_compras_septiembre->text().toInt() <<ui->txtUnid_ventas_septiembre->text().toInt();
        ui->grafica->addItem("Sep",septiembre);

        QVector <float> octubre;
        octubre <<ui->txtUnid_compras_octubre->text().toInt()<<ui->txtUnid_ventas_octubre->text().toInt();
        ui->grafica->addItem("Oct",octubre);

        QVector <float> noviembre;
        noviembre <<ui->txtUnid_compras_noviembre->text().toInt() <<ui->txtUnid_ventas_noviembre->text().toInt();
        ui->grafica->addItem("Nov",noviembre);

        QVector <float> diciembre;
        diciembre <<ui->txtUnid_compras_diciembre->text().toInt() <<ui->txtUnid_ventas_diciembre->text().toInt();
        ui->grafica->addItem("Dic",diciembre);
        ui->grafica->repaint();
    } else{
        ui->grafica->Clear();
        ui->grafica->setTipo(OpenChart::Lineas);
        QStringList lista;
        lista << tr("Ene") << tr("Feb") << tr("Mar") <<tr("Abr") << tr("May") << tr("Jun") << tr("Jul") << tr("Ago")
              <<tr("Sep") <<tr("Oct") <<tr("Nov") << tr("Dic");
        ui->grafica->addLineaStops(lista);
        QVector<float> valoresLineaCompras;
        valoresLineaCompras << ui->txtUnid_compras_enero->text().toFloat() << ui->txtUnid_compras_febrero->text().toFloat()
                     <<ui->txtUnid_compras_marzo->text().toFloat() << ui->txtUnid_compras_abril->text().toFloat()
                     <<ui->txtUnid_compras_mayo->text().toFloat() << ui->txtUnid_compras_junio->text().toFloat()
                     <<ui->txtUnid_compras_julio->text().toFloat() << ui->txtUnid_compras_agosto->text().toFloat()
                     <<ui->txtUnid_compras_septiembre->text().toFloat() << ui->txtUnid_compras_octubre->text().toFloat()
                     <<ui->txtUnid_compras_noviembre->text().toFloat() << ui->txtUnid_compras_diciembre->text().toFloat();
        ui->grafica->addItem("Compras",valoresLineaCompras,Qt::red);

        QVector<float> valoresLineaVentas;
        valoresLineaVentas << ui->txtUnid_ventas_enero->text().toFloat() << ui->txtUnid_ventas_febrero->text().toFloat()
                     <<ui->txtUnid_ventas_marzo->text().toFloat() << ui->txtUnid_ventas_abril->text().toFloat()
                     <<ui->txtUnid_ventas_mayo->text().toFloat() << ui->txtUnid_ventas_junio->text().toFloat()
                     <<ui->txtUnid_ventas_julio->text().toFloat() << ui->txtUnid_ventas_agosto->text().toFloat()
                     <<ui->txtUnid_ventas_septiembre->text().toFloat() << ui->txtUnid_ventas_octubre->text().toFloat()
                     <<ui->txtUnid_ventas_noviembre->text().toFloat() << ui->txtUnid_compras_diciembre->text().toFloat();
        ui->grafica->addItem("Ventas",valoresLineaVentas,Qt::green);
           ui->grafica->repaint();
    }
}

void FrmArticulos::GraficaImportes()
{

    if(ui->cboTipoGrafica->currentText()==tr("Grafica de Barras")) {
        ui->grafica->Clear();
        ui->grafica->setTipo(OpenChart::BarraMultiple);
        ui->grafica->addMulibarColor("Compras",Qt::darkRed);
        ui->grafica->addMulibarColor("Ventas",Qt::darkGreen);
        QVector <float> enero;
        enero << Configuracion_global->MonedatoDouble(ui->txtImporte_compras_enero->text())
              <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_enero->text());//Añadir tantos colores como elementos tenga el vector!

        ui->grafica->addItem("Ene",enero);


        QVector <float> febrero;
        febrero <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_febrero->text())
               <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_febrero->text());//Añadir tantos colores como elementos tenga el vector!

        ui->grafica->addItem("Feb",febrero);

        QVector <float> marzo;
        marzo <<Configuracion_global->MonedatoDouble( ui->txtImporte_compras_marzo->text())
             <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_marzo->text());//Añadir tantos colores como elementos tenga el vector!

        ui->grafica->addItem("Mar",marzo);

        QVector <float> abril;
        abril <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_abril->text())
             <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_abril->text());//Añadir tantos colores como elementos tenga el vector!

        ui->grafica->addItem("Abr",abril);

        QVector <float> mayo;
        mayo <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_mayo->text())
             <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_mayo->text());//Añadir tantos colores como elementos tenga el vector!

        ui->grafica->addItem("May",mayo);

        QVector <float> junio;
        junio <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_junio->text())
              <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_junio->text());

        ui->grafica->addItem("jun",junio);

        QVector <float> julio;
        julio <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_julio->text())
             <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_julio->text());

        ui->grafica->addItem("Jul",julio);

        QVector <float> agosto;
        agosto <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_agosto->text())
               <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_agosto->text());
        ui->grafica->addItem("Ago",agosto);

        QVector <float> septiembre;
        septiembre <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_septiembre->text())
                   <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_septiembre->text());
        ui->grafica->addItem("Sep",septiembre);

        QVector <float> octubre;
        octubre <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_octubre->text())
                <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_octubre->text());
        ui->grafica->addItem("Oct",octubre);

        QVector <float> noviembre;
        noviembre <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_noviembre->text())
                  <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_noviembre->text());
        ui->grafica->addItem("Nov",noviembre);

        QVector <float> diciembre;
        diciembre <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_diciembre->text())
                 <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_diciembre->text());
        ui->grafica->addItem("Dic",diciembre);

        ui->grafica->repaint();
    } else {
        ui->grafica->Clear();
        ui->grafica->setTipo(OpenChart::Lineas);
        QStringList lista;
        lista << tr("Ene") << tr("Feb") << tr("Mar") <<tr("Abr") << tr("May") << tr("Jun") << tr("Jul") << tr("Ago")
              <<tr("Sep") <<tr("Oct") <<tr("Nov") << tr("Dic");
        ui->grafica->addLineaStops(lista);
        QVector<float> valoresLineaCompras;
        valoresLineaCompras <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_enero->text())
                            <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_febrero->text())
                            <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_marzo->text())
                            <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_abril->text())
                            <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_mayo->text())
                            <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_junio->text())
                            <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_julio->text())
                            <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_agosto->text())
                            <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_septiembre->text())
                            <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_octubre->text())
                            <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_noviembre->text())
                            <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_diciembre->text());
        ui->grafica->addItem("Compras",valoresLineaCompras,Qt::red);

        QVector<float> valoresLineaVentas;
        valoresLineaVentas <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_enero->text())
                           <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_febrero->text())
                           <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_marzo->text())
                           <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_abril->text())
                           <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_mayo->text())
                           <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_junio->text())
                           <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_julio->text())
                           <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_agosto->text())
                           <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_septiembre->text())
                           <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_octubre->text())
                           <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_noviembre->text())
                           <<Configuracion_global->MonedatoDouble(ui->txtImporte_compras_diciembre->text());
        ui->grafica->addItem("Ventas",valoresLineaVentas,Qt::green);
           ui->grafica->repaint();
    }

}

void FrmArticulos::GraficaUnidades_comparativa()
{
    if(ui->cboTipoGrafica_2->currentText()==tr("Grafica de Barras")) {
        ui->Grafica_comparativa->setTipo(OpenChart::BarraMultiple);
        ui->Grafica_comparativa->Clear();
        ui->Grafica_comparativa->addMulibarColor("Empresa Activa",Qt::darkGreen);
        ui->Grafica_comparativa->addMulibarColor(ui->cboEmpresa2->currentText(),Qt::darkBlue);
        QVector <float> enero;
        enero <<ui->txtUnid_ventas_enero_EA->text().toInt()<<ui->txtUnid_ventas_enero_3->text().toInt();

        ui->Grafica_comparativa->addItem("Ene",enero);

        QVector <float> febrero;
        febrero <<ui->txtUnid_ventas_febrero_EA->text().toInt() <<ui->txtUnid_ventas_febrero_3->text().toInt();

        ui->Grafica_comparativa->addItem("Feb",febrero);

        QVector <float> marzo;
        marzo <<ui->txtUnid_ventas_marzo_EA->text().toInt() <<ui->txtUnid_ventas_marzo_3->text().toInt();

        ui->Grafica_comparativa->addItem("Mar",marzo);

        QVector <float> abril;
        abril <<ui->txtUnid_ventas_abril_EA->text().toInt() <<ui->txtUnid_ventas_abril_3->text().toInt();

        ui->Grafica_comparativa->addItem("Abr",abril);

        QVector <float> mayo;
        mayo <<ui->txtUnid_ventas_mayo_EA->text().toInt() <<ui->txtUnid_ventas_mayo_3->text().toInt();

        ui->Grafica_comparativa->addItem("May",mayo);

        QVector <float> junio;
        junio <<ui->txtUnid_ventas_junio_EA->text().toInt() <<ui->txtUnid_ventas_junio_3->text().toInt();

        ui->Grafica_comparativa->addItem("jun",junio);

        QVector <float> julio;
        julio <<ui->txtUnid_ventas_julio_EA->text().toInt() <<ui->txtUnid_ventas_julio_3->text().toInt();

        ui->Grafica_comparativa->addItem("Jul",julio);

        QVector <float> agosto;
        agosto <<ui->txtUnid_ventas_agosto_EA->text().toInt() <<ui->txtUnid_ventas_agosto_3->text().toInt();
        ui->Grafica_comparativa->addItem("Ago",agosto);

        QVector <float> septiembre;
        septiembre <<ui->txtUnid_ventas_septiembre_EA->text().toInt() <<ui->txtUnid_ventas_septiembre_3->text().toInt();
        ui->Grafica_comparativa->addItem("Sep",septiembre);

        QVector <float> octubre;
        octubre <<ui->txtUnid_ventas_octubre_EA->text().toInt()<<ui->txtUnid_ventas_octubre_3->text().toInt();
        ui->Grafica_comparativa->addItem("Oct",octubre);

        QVector <float> noviembre;
        noviembre <<ui->txtUnid_ventas_noviembre_EA->text().toInt() <<ui->txtUnid_ventas_noviembre_3->text().toInt();
        ui->Grafica_comparativa->addItem("Nov",noviembre);

        QVector <float> diciembre;
        diciembre <<ui->txtUnid_ventas_diciembre_EA->text().toInt() <<ui->txtUnid_ventas_diciembre_3->text().toInt();
        ui->Grafica_comparativa->addItem("Dic",diciembre);
        ui->Grafica_comparativa->repaint();
    } else{
        // TODO -GRAFICA
        ui->Grafica_comparativa->Clear();
        ui->Grafica_comparativa->setTipo(OpenChart::Lineas);
        QStringList lista;
        lista << "12" << "34" ;
        ui->Grafica_comparativa->addLineaStops(lista);
        QStringList lista1;
        lista1 << "20" << "60";
        ui->Grafica_comparativa->addLineaStops(lista1);

        ui->Grafica_comparativa->repaint();
    }
}

void FrmArticulos::GraficaImportes_comparativa()
{
    ui->Grafica_comparativa->Clear();
    ui->Grafica_comparativa->addMulibarColor("EmpresaActiva",Qt::darkBlue);
    ui->Grafica_comparativa->addMulibarColor(ui->cboEmpresa2->currentText(),Qt::darkGreen);
    QVector <float> enero;
    enero << Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_enero_EA->text())
          <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_enero_3->text());//Añadir tantos colores como elementos tenga el vector!

    ui->Grafica_comparativa->addItem("Ene",enero);


    QVector <float> febrero;
    febrero <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_febrero_EA->text())
            <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_febrero_3->text());//Añadir tantos colores como elementos tenga el vector!

    ui->Grafica_comparativa->addItem("Feb",febrero);

    QVector <float> marzo;
    marzo <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_marzo_EA->text())
          <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_marzo_3->text());//Añadir tantos colores como elementos tenga el vector!

    ui->Grafica_comparativa->addItem("Mar",marzo);

    QVector <float> abril;
    abril <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_abril_EA->text())
          <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_abril_3->text());//Añadir tantos colores como elementos tenga el vector!

    ui->Grafica_comparativa->addItem("Abr",abril);

    QVector <float> mayo;
    mayo <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_mayo_EA->text())
         <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_mayo_3->text());//Añadir tantos colores como elementos tenga el vector!

    ui->Grafica_comparativa->addItem("May",mayo);

    QVector <float> junio;
    junio <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_junio_EA->text())
         <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_junio_3->text());

    ui->Grafica_comparativa->addItem("jun",junio);

    QVector <float> julio;
    julio <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_julio_EA->text())
          <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_julio_3->text());

    ui->Grafica_comparativa->addItem("Jul",julio);

    QVector <float> agosto;
    agosto <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_agosto_EA->text())
           <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_agosto_3->text());
    ui->Grafica_comparativa->addItem("Ago",agosto);

    QVector <float> septiembre;
    septiembre <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_septiembre_EA->text())
               <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_septiembre_3->text());
    ui->Grafica_comparativa->addItem("Sep",septiembre);

    QVector <float> octubre;
    octubre <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_octubre_EA->text())
            <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_octubre_3->text());
    ui->Grafica_comparativa->addItem("Oct",octubre);

    QVector <float> noviembre;
    noviembre <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_noviembre_EA->text())
              <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_noviembre_3->text());
    ui->Grafica_comparativa->addItem("Nov",noviembre);

    QVector <float> diciembre;
    diciembre <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_diciembre_EA->text())
              <<Configuracion_global->MonedatoDouble(ui->txtImporte_ventas_diciembre_3->text());
    ui->Grafica_comparativa->addItem("Dic",diciembre);
}

void FrmArticulos::MostrarGrafica_comparativa(bool grafica_unidades)
{
    if (!grafica_unidades == true)
        GraficaUnidades_comparativa();
    else
        GraficaImportes_comparativa();
}

void FrmArticulos::LLenarGraficas()
{
    QSqlQuery queryAcumulados(Configuracion_global->empresaDB);
    if(!queryAcumulados.exec("select * from acum_articulos where id_articulo=" +QString::number(oArticulo->id)))
    {
        QMessageBox::warning(this,tr("Acumulados ejercicio"),
                             tr("Ocurrió un error al recuperar ficha de acumulados: %1").arg(queryAcumulados.lastError().text()),
                             tr("Aceptar"));
    }
    else
    {
        queryAcumulados.next();
        //--------------------
        // Undidades compradas
        //--------------------
        ui->txtUnid_compras_enero->setText(QString::number(queryAcumulados.record().value("unid_comp_enero").toDouble()));
        ui->txtUnid_compras_febrero->setText(QString::number(queryAcumulados.record().value("unid_comp_febrero").toDouble()));
        ui->txtUnid_compras_marzo->setText(QString::number(queryAcumulados.record().value("unid_comp_marzo").toDouble()));
        ui->txtUnid_compras_abril->setText(QString::number(queryAcumulados.record().value("unid_comp_abril").toDouble()));
        ui->txtUnid_compras_mayo->setText(QString::number(queryAcumulados.record().value("unid_comp_mayo").toDouble()));
        ui->txtUnid_compras_junio->setText(QString::number(queryAcumulados.record().value("unid_comp_junio").toDouble()));
        ui->txtUnid_compras_julio->setText(QString::number(queryAcumulados.record().value("unid_comp_julio").toDouble()));
        ui->txtUnid_compras_agosto->setText(QString::number(queryAcumulados.record().value("unid_comp_agosto").toDouble()));
        ui->txtUnid_compras_septiembre->setText(QString::number(queryAcumulados.record().value("unid_comp_septiembre").toDouble()));
        ui->txtUnid_compras_octubre->setText(QString::number(queryAcumulados.record().value("unid_comp_octubre").toDouble()));
        ui->txtUnid_compras_noviembre->setText(QString::number(queryAcumulados.record().value("unid_comp_noviembre").toDouble()));
        ui->txtUnid_compras_diciembre->setText(QString::number(queryAcumulados.record().value("unid_comp_diciembre").toDouble()));

        //--------------------
        // Undidades vendidas
        //--------------------
        ui->txtUnid_ventas_enero->setText(QString::number(queryAcumulados.record().value("unid_vent_enero").toDouble()));
        ui->txtUnid_ventas_febrero->setText(QString::number(queryAcumulados.record().value("unid_vent_febrero").toDouble()));
        ui->txtUnid_ventas_marzo->setText(QString::number(queryAcumulados.record().value("unid_vent_marzo").toDouble()));
        ui->txtUnid_ventas_abril->setText(QString::number(queryAcumulados.record().value("unid_vent_abril").toDouble()));
        ui->txtUnid_ventas_mayo->setText(QString::number(queryAcumulados.record().value("unid_vent_mayo").toDouble()));
        ui->txtUnid_ventas_junio->setText(QString::number(queryAcumulados.record().value("unid_vent_junio").toDouble()));
        ui->txtUnid_ventas_julio->setText(QString::number(queryAcumulados.record().value("unid_vent_julio").toDouble()));
        ui->txtUnid_ventas_agosto->setText(QString::number(queryAcumulados.record().value("unid_vent_agosto").toDouble()));
        ui->txtUnid_ventas_septiembre->setText(QString::number(queryAcumulados.record().value("unid_vent_septiembre").toDouble()));
        ui->txtUnid_ventas_octubre->setText(QString::number(queryAcumulados.record().value("unid_vent_octubre").toDouble()));
        ui->txtUnid_ventas_noviembre->setText(QString::number(queryAcumulados.record().value("unid_vent_noviembre").toDouble()));
        ui->txtUnid_ventas_diciembre->setText(QString::number(queryAcumulados.record().value("unid_vent_diciembre").toDouble()));

        //--------------------
        // Importe compras
        //--------------------
        ui->txtImporte_compras_enero->setText(Configuracion_global->toFormatoMoneda(
                                                  QString::number(queryAcumulados.record().value("acum_comp_enero").toDouble(),
                                                                  'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_compras_febrero->setText(Configuracion_global->toFormatoMoneda(
                                                    QString::number(queryAcumulados.record().value("acum_comp_febrero").toDouble(),
                                                                    'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_compras_marzo->setText(Configuracion_global->toFormatoMoneda(
                                                  QString::number(queryAcumulados.record().value("acum_comp_marzo").toDouble(),
                                                                  'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_compras_abril->setText(Configuracion_global->toFormatoMoneda(
                                                  QString::number(queryAcumulados.record().value("acum_comp_abril").toDouble(),
                                                                  'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_compras_mayo->setText(Configuracion_global->toFormatoMoneda(
                                                 QString::number(queryAcumulados.record().value("acum_comp_mayo").toDouble(),
                                                                 'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_compras_junio->setText(Configuracion_global->toFormatoMoneda(
                                                  QString::number(queryAcumulados.record().value("acum_comp_junio").toDouble(),
                                                                  'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_compras_julio->setText(Configuracion_global->toFormatoMoneda(
                                                  QString::number(queryAcumulados.record().value("acum_comp_julio").toDouble(),
                                                                  'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_compras_agosto->setText(Configuracion_global->toFormatoMoneda(
                                                   QString::number(queryAcumulados.record().value("acum_comp_agosto").toDouble(),
                                                                   'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_compras_septiembre->setText(Configuracion_global->toFormatoMoneda(
                                                   QString::number(queryAcumulados.record().value("acum_comp_septiembre").toDouble(),
                                                                   'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_compras_octubre->setText(Configuracion_global->toFormatoMoneda(
                                                    QString::number(queryAcumulados.record().value("acum_comp_octubre").toDouble(),
                                                                    'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_compras_noviembre->setText(Configuracion_global->toFormatoMoneda(
                                                   QString::number(queryAcumulados.record().value("acum_comp_noviembre").toDouble(),
                                                                   'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_compras_diciembre->setText(Configuracion_global->toFormatoMoneda(
                                                  QString::number(queryAcumulados.record().value("acum_comp_diciembre").toDouble(),
                                                                  'f',Configuracion_global->decimales_campos_totales)));

        //--------------------
        // Importe Ventas
        //--------------------
        ui->txtImporte_ventas_enero->setText(Configuracion_global->toFormatoMoneda(
                                                 QString::number(queryAcumulados.record().value("acum_vent_enero").toDouble(),
                                                                 'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_ventas_febrero->setText(Configuracion_global->toFormatoMoneda(
                                                 QString::number(queryAcumulados.record().value("acum_vent_febrero").toDouble(),
                                                                 'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_ventas_marzo->setText(Configuracion_global->toFormatoMoneda(
                                                 QString::number(queryAcumulados.record().value("acum_vent_marzo").toDouble(),
                                                                 'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_ventas_abril->setText(Configuracion_global->toFormatoMoneda(
                                                 QString::number(queryAcumulados.record().value("acum_vent_abril").toDouble(),
                                                                 'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_ventas_mayo->setText(Configuracion_global->toFormatoMoneda(
                                                QString::number(queryAcumulados.record().value("acum_vent_mayo").toDouble(),
                                                                'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_ventas_junio->setText(Configuracion_global->toFormatoMoneda(
                                                 QString::number(queryAcumulados.record().value("acum_vent_junio").toDouble(),
                                                                 'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_ventas_julio->setText(Configuracion_global->toFormatoMoneda(
                                                 QString::number(queryAcumulados.record().value("acum_vent_julio").toDouble(),
                                                                 'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_ventas_agosto->setText(Configuracion_global->toFormatoMoneda(
                                                  QString::number(queryAcumulados.record().value("acum_vent_agosto").toDouble(),
                                                                  'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_ventas_septiembre->setText(Configuracion_global->toFormatoMoneda(
                                                      QString::number(queryAcumulados.record().value("acum_vent_septiembre").toDouble(),
                                                                      'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_ventas_octubre->setText(Configuracion_global->toFormatoMoneda(
                                                   QString::number(queryAcumulados.record().value("acum_vent_octubre").toDouble(),
                                                                   'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_ventas_noviembre->setText(Configuracion_global->toFormatoMoneda(
                                                     QString::number(queryAcumulados.record().value("acum_vent_noviembre").toDouble(),
                                                                     'f',Configuracion_global->decimales_campos_totales)));
        ui->txtImporte_ventas_diciembre->setText(Configuracion_global->toFormatoMoneda(
                                              QString::number(queryAcumulados.record().value("acum_vent_diciembre").toDouble(),'f',
                                                              Configuracion_global->decimales_campos_totales)));

        //-----------------------------------------
        // Undidades vendidas (Grafica comparativa)
        //-----------------------------------------
        ui->txtUnid_ventas_enero_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_enero").toDouble()));
        ui->txtUnid_ventas_febrero_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_febrero").toDouble()));
        ui->txtUnid_ventas_marzo_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_marzo").toDouble()));
        ui->txtUnid_ventas_abril_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_abril").toDouble()));
        ui->txtUnid_ventas_mayo_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_mayo").toDouble()));
        ui->txtUnid_ventas_junio_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_junio").toDouble()));
        ui->txtUnid_ventas_julio_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_julio").toDouble()));
        ui->txtUnid_ventas_agosto_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_agosto").toDouble()));
        ui->txtUnid_ventas_septiembre_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_septiembre").toDouble()));
        ui->txtUnid_ventas_octubre_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_octubre").toDouble()));
        ui->txtUnid_ventas_noviembre_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_noviembre").toDouble()));
        ui->txtUnid_ventas_diciembre_EA->setText(QString::number(queryAcumulados.record().value("unid_vent_diciembre").toDouble()));

        //--------------------
        // Importe Ventas
        //--------------------
        ui->txtImporte_ventas_enero_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_enero").toDouble()));
        ui->txtImporte_ventas_febrero_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_febrero").toDouble()));
        ui->txtImporte_ventas_marzo_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_marzo").toDouble()));
        ui->txtImporte_ventas_abril_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_abril").toDouble()));
        ui->txtImporte_ventas_mayo_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_mayo").toDouble()));
        ui->txtImporte_ventas_junio_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_junio").toDouble()));
        ui->txtImporte_ventas_julio_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_julio").toDouble()));
        ui->txtImporte_ventas_agosto_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_agosto").toDouble()));
        ui->txtImporte_ventas_septiembre_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_septiembre").toDouble()));
        ui->txtImporte_ventas_octubre_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_octubre").toDouble()));
        ui->txtImporte_ventas_noviembre_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_noviembre").toDouble()));
        ui->txtImporte_ventas_diciembre_EA->setText(QString::number(queryAcumulados.record().value("acum_vent_diciembre").toDouble()));

    }        
}

void FrmArticulos::LLenarGrafica_comparativa(int index)
{
    // Conecto a la BD de la otra empresa
    QSqlQuery queryEmpresa_2(Configuracion_global->groupDB);
    queryEmpresa_2.prepare("Select * from vistaEmpresa where empresa =:empresa");
    queryEmpresa_2.bindValue(":empresa",ui->cboEmpresa2->currentText().trimmed());
    if (queryEmpresa_2.exec()){
        if(queryEmpresa_2.next()){
            QSqlQuery queryEmpresa(Configuracion_global->groupDB);
            queryEmpresa.exec("select * from empresas where id="+ queryEmpresa_2.record().value("id").toString());
            queryEmpresa.next();
            QString cDriver;
            cDriver =queryEmpresa.record().value("driverBD").toString();
            QSqlDatabase dbEmpresa2  = QSqlDatabase::addDatabase(cDriver,"empresa2");

            if (queryEmpresa.record().value("driverBD").toString() == "QSQLITE")
                {
                dbEmpresa2.setDatabaseName(queryEmpresa.record().value("ruta_bd_sqlite").toString());
                    dbEmpresa2.open();
            } else  {

                dbEmpresa2.setDatabaseName(queryEmpresa.record().value("nombre_bd").toString());
                dbEmpresa2.setHostName(queryEmpresa.record().value("host").toString());
                dbEmpresa2.open(queryEmpresa.record().value("user").toString(),
                                    queryEmpresa.record().value("contrasena").toString());
            }
            if (dbEmpresa2.lastError().isValid()) {
                QMessageBox::critical(0, "error:", dbEmpresa2.lastError().text());
            }
            if(dbEmpresa2.isOpen()) {
                QSqlQuery acum2(QSqlDatabase::database("empresa2"));
                if(acum2.exec("select * from acum_articulos where id_articulo ="+QString::number(oArticulo->id))){
                    acum2.next();
                    //-----------------------------------------
                    // Undidades vendidas (Grafica comparativa)
                    //-----------------------------------------
                    ui->txtUnid_ventas_enero_3->setText(acum2.record().value("unid_vent_enero").toString());
                    ui->txtUnid_ventas_febrero_3->setText(acum2.record().value("unid_vent_febrero").toString());
                    ui->txtUnid_ventas_marzo_3->setText(acum2.record().value("unid_vent_marzo").toString());
                    ui->txtUnid_ventas_abril_3->setText(acum2.record().value("unid_vent_abril").toString());
                    ui->txtUnid_ventas_mayo_3->setText(acum2.record().value("unid_vent_mayo").toString());
                    ui->txtUnid_ventas_junio_3->setText(acum2.record().value("unid_vent_junio").toString());
                    ui->txtUnid_ventas_julio_3->setText(acum2.record().value("unid_vent_julio").toString());
                    ui->txtUnid_ventas_agosto_3->setText(acum2.record().value("unid_vent_agosto").toString());
                    ui->txtUnid_ventas_septiembre_3->setText(acum2.record().value("unid_vent_septiembre").toString());
                    ui->txtUnid_ventas_octubre_3->setText(acum2.record().value("unid_vent_octubre").toString());
                    ui->txtUnid_ventas_noviembre_3->setText(acum2.record().value("unid_vent_noviembre").toString());
                    ui->txtUnid_ventas_diciembre_3->setText(acum2.record().value("unid_vent_diciembre").toString());

                    //--------------------
                    // Importe Ventas
                    //--------------------
                    ui->txtImporte_ventas_enero_3->setText(acum2.record().value("acum_vent_enero").toString());
                    ui->txtImporte_ventas_febrero_3->setText(acum2.record().value("acum_vent_febrero").toString());
                    ui->txtImporte_ventas_marzo_3->setText(acum2.record().value("acum_vent_marzo").toString());
                    ui->txtImporte_ventas_abril_3->setText(acum2.record().value("acum_vent_abril").toString());
                    ui->txtImporte_ventas_mayo_3->setText(acum2.record().value("acum_vent_mayo").toString());
                    ui->txtImporte_ventas_junio_3->setText(acum2.record().value("acum_vent_junio").toString());
                    ui->txtImporte_ventas_julio_3->setText(acum2.record().value("acum_vent_julio").toString());
                    ui->txtImporte_ventas_agosto_3->setText(acum2.record().value("acum_vent_agosto").toString());
                    ui->txtImporte_ventas_septiembre_3->setText(acum2.record().value("acum_vent_septiembre").toString());
                    ui->txtImporte_ventas_octubre_3->setText(acum2.record().value("acum_vent_octubre").toString());
                    ui->txtImporte_ventas_noviembre_3->setText(acum2.record().value("acum_vent_noviembre").toString());
                    ui->txtImporte_ventas_diciembre_3->setText(acum2.record().value("acum_vent_diciembre").toString());
                    if(ui->radGrafica_importes_2->isChecked())
                        GraficaImportes_comparativa();
                    else
                        GraficaUnidades_comparativa();

                }
            } else {
                QMessageBox::warning(this,tr("ERROR:"),
                                     tr("No se encontró el registro de la empresa"),
                                     tr("Aceptar"));
            }
            dbEmpresa2.close();
        }
    }
}

void FrmArticulos::listados()
{
    frmListadosArticulo listados(this);
    listados.exec();
}

void FrmArticulos::on_chkValorGrafica_toggled(bool checked)
{
    ui->chkValorGrafica->setEnabled(false);
    if(checked)
    {
        ui->cajaValores->show();
        QPropertyAnimation* animation = new QPropertyAnimation(ui->cajaValores, "pos",this);

        animation->setDuration(1000);
        animation->setEasingCurve(QEasingCurve::OutElastic);
        QPoint pos = ui->cajaValores->pos();
        animation->setStartValue(QPoint(pos.x()+ui->cajaValores->width(),pos.y()));
        animation->setEndValue(pos);

        QPropertyAnimation* animation0 = new QPropertyAnimation(ui->cajaValores, "size",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation0->setDuration(1000);
        animation0->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz0 = ui->cajaValores->size();
        animation0->setStartValue(QSize(0,siz0.height()));
        animation0->setEndValue(siz0);

        QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
        //this->setGraphicsEffect(fade_effect);
        ui->cajaValores->setGraphicsEffect(fade_effect);
        QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
        animationOp->setEasingCurve(QEasingCurve::Linear);
        animationOp->setDuration(1000);
        animationOp->setStartValue(0.0);
        animationOp->setEndValue(1.0);

        QPropertyAnimation* animation1 = new QPropertyAnimation(ui->frame_9, "size",this);
        //connect(animation1,SIGNAL(finished()),this,SLOT(ani_end()));
        animation1->setDuration(1000);
        animation1->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz = ui->frame_9->size();
        animation1->setStartValue(QSize(siz.width()+siz0.width(),siz.height()));
        animation1->setEndValue(siz);

        QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
        group->addAnimation(animation);
        group->addAnimation(animation0);
        group->addAnimation(animation1);
        group->addAnimation(animationOp);

        connect(group,SIGNAL(finished()),group,SLOT(deleteLater()));
         connect(group,SIGNAL(finished()),this,SLOT(toggleChecbox()));
        group->start();
    }
    else
    {
        QPropertyAnimation* animation = new QPropertyAnimation(ui->cajaValores, "pos",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation->setDuration(1300);
        animation->setEasingCurve(QEasingCurve::OutElastic);
        QPoint pos = ui->cajaValores->pos();
        animation->setStartValue(pos);
        animation->setEndValue(QPoint(pos.x()+ui->cajaValores->width(),pos.y()));

        QPropertyAnimation* animation0 = new QPropertyAnimation(ui->cajaValores, "size",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation0->setDuration(1300);
        animation0->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz0 = ui->cajaValores->size();
        animation0->setStartValue(siz0);
        animation0->setEndValue(QSize(0,siz0.height()));

        QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
        //this->setGraphicsEffect(fade_effect);
        ui->cajaValores->setGraphicsEffect(fade_effect);
        QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
        animationOp->setEasingCurve(QEasingCurve::Linear);
        animationOp->setDuration(1000);
        animationOp->setStartValue(1.0);
        animationOp->setEndValue(0.0);

        QPropertyAnimation* animation1 = new QPropertyAnimation(ui->frame_9, "size",this);
        //connect(animation1,SIGNAL(finished()),this,SLOT(ani_end()));
        animation1->setDuration(1300);
        animation1->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz = ui->frame_9->size();
        animation1->setStartValue(siz);
        animation1->setEndValue(QSize(siz.width()+ui->cajaValores->width(),siz.height()));

        QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
        group->addAnimation(animation);
        group->addAnimation(animation0);
        group->addAnimation(animation1);
        group->addAnimation(animationOp);

        connect(group,SIGNAL(finished()),this,SLOT(ani_end()));
        connect(group,SIGNAL(finished()),this,SLOT(toggleChecbox()));
        group->start();
    }
}

void FrmArticulos::ani_end()
{
    ui->cajaValores->hide();
    sender()->deleteLater();
}

void FrmArticulos::toggleChecbox()
{
    ui->chkValorGrafica->setEnabled(true);
}

void FrmArticulos::on_chkmostrarvalores_comparativa_toggled(bool checked)
{
    ui->chkmostrarvalores_comparativa->setEnabled(false);
    if(checked)
    {
        ui->cajaValores__comparativa->show();
        QPropertyAnimation* animation = new QPropertyAnimation(ui->cajaValores__comparativa, "pos",this);

        animation->setDuration(1000);
        animation->setEasingCurve(QEasingCurve::OutElastic);
        QPoint pos = ui->cajaValores__comparativa->pos();
        animation->setStartValue(QPoint(pos.x()+ui->cajaValores__comparativa->width(),pos.y()));
        animation->setEndValue(pos);

        QPropertyAnimation* animation0 = new QPropertyAnimation(ui->cajaValores__comparativa, "size",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation0->setDuration(1000);
        animation0->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz0 = ui->cajaValores__comparativa->size();
        animation0->setStartValue(QSize(0,siz0.height()));
        animation0->setEndValue(siz0);

        QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
        //this->setGraphicsEffect(fade_effect);
        ui->cajaValores__comparativa->setGraphicsEffect(fade_effect);
        QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
        animationOp->setEasingCurve(QEasingCurve::Linear);
        animationOp->setDuration(1000);
        animationOp->setStartValue(0.0);
        animationOp->setEndValue(1.0);

        QPropertyAnimation* animation1 = new QPropertyAnimation(ui->frame_graficas_comparativas, "size",this);
        //connect(animation1,SIGNAL(finished()),this,SLOT(ani_end()));
        animation1->setDuration(1000);
        animation1->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz = ui->frame_graficas_comparativas->size();
        animation1->setStartValue(QSize(siz.width()+siz0.width(),siz.height()));
        animation1->setEndValue(siz);

        QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
        group->addAnimation(animation);
        group->addAnimation(animation0);
        group->addAnimation(animation1);
        group->addAnimation(animationOp);

        connect(group,SIGNAL(finished()),group,SLOT(deleteLater()));
        connect(group,SIGNAL(finished()),this,SLOT(togglechkmostrarvalores_comparativa()));
        group->start();

    }
    else
    {
        QPropertyAnimation* animation = new QPropertyAnimation(ui->cajaValores__comparativa, "pos",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation->setDuration(1300);
        animation->setEasingCurve(QEasingCurve::OutElastic);
        QPoint pos = ui->cajaValores__comparativa->pos();
        animation->setStartValue(pos);
        animation->setEndValue(QPoint(pos.x()+ui->cajaValores__comparativa->width(),pos.y()));

        QPropertyAnimation* animation0 = new QPropertyAnimation(ui->cajaValores__comparativa, "size",this);
        //connect(animation,SIGNAL(finished()),this,SLOT(ani_end()));
        animation0->setDuration(1300);
        animation0->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz0 = ui->cajaValores__comparativa->size();
        animation0->setStartValue(siz0);
        animation0->setEndValue(QSize(0,siz0.height()));

        QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
        //this->setGraphicsEffect(fade_effect);
        ui->cajaValores__comparativa->setGraphicsEffect(fade_effect);
        QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
        animationOp->setEasingCurve(QEasingCurve::Linear);
        animationOp->setDuration(1000);
        animationOp->setStartValue(1.0);
        animationOp->setEndValue(0.0);

        QPropertyAnimation* animation1 = new QPropertyAnimation(ui->frame_graficas_comparativas, "size",this);
        //connect(animation1,SIGNAL(finished()),this,SLOT(ani_end()));
        animation1->setDuration(1300);
        animation1->setEasingCurve(QEasingCurve::OutElastic);
        QSize siz = ui->frame_graficas_comparativas->size();
        animation1->setStartValue(siz);
        animation1->setEndValue(QSize(siz.width()+ui->cajaValores__comparativa->width(),siz.height()));

        QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
        group->addAnimation(animation);
        group->addAnimation(animation0);
        group->addAnimation(animation1);
        group->addAnimation(animationOp);

        connect(group,SIGNAL(finished()),this,SLOT(ani_comparativas_end()));
        connect(group,SIGNAL(finished()),this,SLOT(togglechkmostrarvalores_comparativa()));
        group->start();
    }
}

void FrmArticulos::ani_comparativas_end()
{
    ui->cajaValores__comparativa->hide();
    sender()->deleteLater();
}

void FrmArticulos::togglechkmostrarvalores_comparativa()
{
        ui->chkmostrarvalores_comparativa->setEnabled(true);
}

void FrmArticulos::on_botCambiarImagen_clicked()
{
    CambiarImagen_clicked(ui->lblImagenArticulo_1,"imagen1");
}

void FrmArticulos::on_botCambiarImagen_2_clicked()
{
    CambiarImagen_clicked(ui->lblImagenArticulo_2,"imagen2");
}

void FrmArticulos::on_botCambiarImagen_3_clicked()
{
    CambiarImagen_clicked(ui->lblImagenArticulo_3,"imagen3");
}

void FrmArticulos::on_botCambiarImagen_4_clicked()
{
    CambiarImagen_clicked(ui->lblImagenArticulo_4,"imagen4");
}

void FrmArticulos::actualizar()
{
    if(!oArticulo->id >= 0)
    {
        oArticulo->Recuperar(oArticulo->id);
        LLenarCampos(ui->Pestanas->currentIndex());
        tarifa_model->setFilter("id_articulo = "+QString::number(oArticulo->id));
        tarifa_model->select();
    }
}



void FrmArticulos::CambiarImagen_clicked(QLabel *label, QString campo)
{
    QString ficheroImagen;
    ficheroImagen = QFileDialog::getOpenFileName(this,tr("Abrir fichero de imagen"),"","Imagenes (*.bmp *.png *.xpm *.jpg)");
    if (!ficheroImagen.isEmpty()) {

        QImage imagen(ficheroImagen);

        label->setPixmap(QPixmap::fromImage(imagen));
        //label->setScaledContents(true);

        QByteArray ba;
        QFile f(ficheroImagen);
        if(f.open(QIODevice::ReadOnly)) {
            ba = f.readAll();
            f.close();
        }
        // Si existe actualizamos, si no existe añadimos.
        QString error;
        int id_art = SqlCalls::SelectOneField("articulos_imagenes","id_articulo",QString("id_articulo =%1").arg(oArticulo->id),
                                 Configuracion_global->groupDB,error).toInt();
        if(id_art >0){
            QSqlQuery Articulo(Configuracion_global->groupDB);

            Articulo.prepare("update articulos_imagenes set "+campo+" =:imagen,id_articulo =:id_art where id_articulo = :nid");
            Articulo.bindValue(":imagen",ba/*.toBase64()*/);
            Articulo.bindValue(":nid",oArticulo->id);
            Articulo.bindValue(":id_art",oArticulo->id);
            if (!Articulo.exec())
            {
                QMessageBox::warning(qApp->activeWindow(),tr("Guardar Imagen"),tr("No se ha podido guardar la imagen en la base de datos"),tr("Ok"));
                qDebug() << Articulo.lastError();
            }
        } else
        {
            QSqlQuery Articulo(Configuracion_global->groupDB);

            Articulo.prepare("insert into articulos_imagenes ("+campo+",id_articulo)  values(:imagen,:id_art) ");
            Articulo.bindValue(":imagen",ba/*.toBase64()*/);
            Articulo.bindValue(":id_art",oArticulo->id);
            if (!Articulo.exec())
            {
                QMessageBox::warning(qApp->activeWindow(),tr("Guardar Imagen"),tr("No se ha podido guardar la imagen en la base de datos"),tr("Ok"));
                qDebug() << Articulo.lastError();
            }
        }
    }
}

void FrmArticulos::on_txtcoste_editingFinished()
{
    double inicio, fin;
    inicio = oArticulo->coste;

    // Doy valor a coste_real
    ui->txtCoste_real->setText(ui->txtcoste->text());

    fin = ui->txtcoste->text().replace(".","").replace(",",".").toDouble();
    if (inicio != fin)
    {
        blockSignals(true);
        // TODO recalcular tarifas
        blockSignals(false);
    }
}

void FrmArticulos::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void FrmArticulos::on_tablaBusqueda_doubleClicked(const QModelIndex &index)
{
    int id = ui->tablaBusqueda->model()->data(ui->tablaBusqueda->model()->index(index.row(),0),Qt::EditRole).toInt();
    oArticulo->Recuperar(id);

    ui->Pestanas->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);

    LLenarCampos(ui->Pestanas->currentIndex());    

    ui->botEditar->setEnabled(true);
    ocultarBusqueda();
}



void FrmArticulos::on_btnExcepciones_2_clicked()
{
    on_btnExcepciones_clicked();
}

void FrmArticulos::on_btnExcepciones_clicked()
{
    QModelIndex index = ui->tablaBusqueda->currentIndex();
    int id = ui->tablaBusqueda->model()->data(ui->tablaBusqueda->model()->index(index.row(),0),Qt::EditRole).toInt();
    oArticulo->Recuperar(id);
    FrmExcepciones excepciones(this);
    excepciones.cargar_articulo(id);
    excepciones.exec();
}

void FrmArticulos::on_btnAnadir_oferta_clicked()
{
    nueva_oferta = true;
    ui->chkArticulo_promocionado->setChecked(true);
    ui->chkOferta_32->setChecked(false);
    ui->chkOferta_dto->setChecked(false);
    ui->chkOferta_pvp->setChecked(false);
    ui->chkOferta_web->setChecked(false);
    ui->chkOferta_32->setEnabled(true);
    ui->chkOferta_dto->setEnabled(true);
    ui->chkOferta_pvp->setEnabled(true);
    ui->chkOferta_web->setEnabled(true);
    ui->txtOferta_Descripcion_promocion->setEnabled(true);
    ui->lblDescripcion_oferta->setEnabled(true);

    ui->txtOfertaDtoOferta->setText("0");
    ui->txtOfertaregalo_de->setText("0");
    ui->txtOferta_Descripcion_promocion->clear();
    ui->txtOferta_Fecha_fin->setDate(QDate::currentDate().addMonths(1));
    ui->txtOferta_Fecha_ini->setDate(QDate::currentDate());
    ui->txtOferta_por_cada->setText("0");
    ui->txtoferta_pvp_fijo->setText("0,00");

    ui->btnguardar_oferta->setEnabled(true);
    ui->btnDeshacer_oferta->setEnabled(true);
    ui->btnAnadir_oferta->setEnabled(false);
    ui->btnEditarOferta->setEnabled(false);
    ui->botGuardar->setEnabled(false);
    ui->botDeshacer->setEnabled(false);
    ui->framePromocion->setEnabled(true);
    ui->frame_comentarios->setEnabled(true);
    ui->txtOferta_Descripcion_promocion->setFocus();
}

void FrmArticulos::on_btnguardar_oferta_clicked()
{
    if(!ui->txtOferta_Descripcion_promocion->text().isEmpty())
    {
        ui->btnguardar_oferta->setEnabled(false);
        ui->btnDeshacer_oferta->setEnabled(false);
        ui->btnAnadir_oferta->setEnabled(true);
        ui->btnEditarOferta->setEnabled(true);
        ui->botGuardar->setEnabled(true);
        ui->botDeshacer->setEnabled(true);

        QHash <QString, QVariant> oferta;

        oferta["id_articulo"] = oArticulo->id;
        oferta["descripcion"] = ui->txtOferta_Descripcion_promocion->text();
        oferta["oferta_32"] =ui->chkOferta_32->isChecked();
        oferta["oferta_dto"] = ui->chkOferta_dto->isChecked();
        oferta["oferta_precio_final"] = ui->chkOferta_pvp->isChecked();
        oferta["oferta_web"] = ui->chkOferta_web->isChecked();
        oferta["unidades"] = ui->txtOferta_por_cada->text().toFloat();
        oferta["regalo"] = ui->txtOfertaregalo_de->text().toFloat();
        oferta["dto_local"] = ui->txtOfertaDtoOferta->text().toFloat();
        oferta["dto_web"] =ui->txtOferta_dto_web->text().toFloat();
        oferta["precio_final"] = ui->txtoferta_pvp_fijo->text().toDouble();
        oferta["comentarios"] = ui->txtOferta_comentarios_promocion->toPlainText();
        oferta["fecha_inicio"] =ui->txtOferta_Fecha_ini->date();
        oferta["fecha_fin"] = ui->txtOferta_Fecha_fin->date();
        int new_id;
        bool success;
        QString error;
        if(nueva_oferta)
        {
            nueva_oferta = false;
            if(QMessageBox::question(this,tr("Gestión de Artículos"),tr("¿Desea guardar como oferta activa?"),
                                     tr("No"),tr("Si")) == QMessageBox::Accepted)
                oferta["activa"] = true;
            else
                oferta["activa"] = false;
            new_id = SqlCalls::SqlInsert(oferta,"articulos_ofertas",Configuracion_global->empresaDB,error);
            if(new_id>-1)
                TimedMessageBox *t = new TimedMessageBox(this,tr("Se ha creado la oferta"));
        } else
        {
            success = SqlCalls::SqlUpdate(oferta,"articulos_ofertas",Configuracion_global->empresaDB,QString("id =%1").arg(this->id_oferta),
                                          error);
            if(!success)
                QMessageBox::warning(this,tr("Gestión de artículos"),tr("Se ha producido un error al actualizar la oferta: %1").arg(error));

        }
        promociones->setQuery(QString("select id,activa, descripcion from articulos_ofertas where id_articulo = %1 order by activa desc").arg(oArticulo->id),
                                Configuracion_global->empresaDB);
    }
    else
        {
            QMessageBox::warning(this,tr("Gestión de artículos"),tr("Debe especificar una descripción antes de poder guardar"),
                                 tr("Aceptar"));
        }
}

void FrmArticulos::on_chkArticulo_promocionado_toggled(bool checked)
{
    if(ui->botGuardar->isEnabled())
    {
        ui->framePromocion->setEnabled(checked);

        ui->btnAnadir_oferta->setEnabled(checked);
        ui->btnEditarOferta->setEnabled(checked);
        ui->btnBorrar_oferta->setEnabled(checked);


    } /*else
        QMessageBox::warning(this,tr("Gestión de artículos"),
                             tr("Debe estar en modo edición de articulo para poder modificar/crear una oferta"),
                             tr("Aceptar"));*/
}

void FrmArticulos::on_btnDeshacer_oferta_clicked()
{
    on_tabla_ofertas_clicked(ui->tabla_ofertas->currentIndex());
}

void FrmArticulos::on_tabla_ofertas_clicked(const QModelIndex &index)
{
    int id = ui->tabla_ofertas->model()->data(ui->tabla_ofertas->model()->index(index.row(),0),Qt::EditRole).toInt();
    QMap <int,QSqlRecord> oferta;
    QString error;
    oferta = SqlCalls::SelectRecord("articulos_ofertas",QString("id = %1").arg(id),Configuracion_global->empresaDB,error);
    if(error.isEmpty())
    {
        ui->chkOferta_32->setChecked(oferta.value(id).value("oferta_32").toBool());
        ui->chkOferta_dto->setChecked(oferta.value(id).value("oferta_dto").toBool());
        ui->chkOferta_pvp->setChecked(oferta.value(id).value("oferta_precio_final").toBool());
        ui->chkOferta_web->setChecked(oferta.value(id).value("oferta_web").toBool());
        ui->txtOferta_por_cada->setText(QString::number(oferta.value(id).value("unidades").toFloat(),'f',2));
        ui->txtOfertaregalo_de->setText(QString::number(oferta.value(id).value("regalo").toFloat(),'f',2));
        ui->txtOfertaDtoOferta->setText(oferta.value(id).value("dto_local").toString());
        ui->txtOferta_dto_web->setText(oferta.value(id).value("dto_web").toString());
        ui->txtoferta_pvp_fijo->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                    oferta.value(id).value("precio_final").toFloat(),
                                                                    'f',Configuracion_global->decimales_campos_totales)));
        ui->txtOferta_comentarios_promocion->setText(oferta.value(id).value("comentarios").toString());
        ui->txtOferta_Fecha_ini->setDate(oferta.value(id).value("fecha_inicio").toDate());
        ui->txtOferta_Fecha_fin->setDate(oferta.value(id).value("fecha_fin").toDate());
        ui->txtOferta_Descripcion_promocion->setText((oferta.value(id).value("descripcion").toString()));
    }
    ui->btnAnadir_oferta->setEnabled(true);
    ui->btnEditarOferta->setEnabled(true);
    ui->btnguardar_oferta->setEnabled(false);
    ui->btnDeshacer_oferta->setEnabled(false);
}


void FrmArticulos::on_btnExcepciones_3_clicked()
{
    on_btnExcepciones_clicked();
}

void FrmArticulos::on_btnKit_clicked()
{
    if(sender() != ui->btnKit)
    {
        QModelIndex index = ui->tablaBusqueda->currentIndex();
        if(!index.isValid())
            return;
        int id = ui->tablaBusqueda->model()->data(ui->tablaBusqueda->model()->index(index.row(),0),Qt::DisplayRole).toInt();
        oArticulo->Recuperar(id);
    }

    if(!oArticulo->kit)
    {
        if(QMessageBox::question(this,tr("Gestión de Artículos"),tr("¿El Artículo no es un kit, ¿desea convertirlo a kit?"),
                                 tr("No"),tr("Convertir"))== QMessageBox::Accepted)
        {
            QHash <QString, QVariant> h;
            QString error;
            h["kit"] = true;
            bool success = SqlCalls::SqlUpdate(h,"articulos",Configuracion_global->groupDB,QString("id=%1").arg(oArticulo->id),
                                               error);
            if(success)
            {
                oArticulo->kit = true;
                TimedMessageBox *t = new TimedMessageBox(this,tr("Articulo convertido en kit"));
            }
            else
            {
                QMessageBox::warning(this,tr("Gestión de Artículos"),
                                     tr("Atención: No se puede convertir: %1").arg(error));
            }
        }
    }
    if(oArticulo->kit)
    {
        FrmKit kit(this);
        kit.set_articulo(oArticulo->codigo, oArticulo->descripcion, oArticulo->nstock_fisico_almacen);
        kit.setWindowState(kit.windowState() | Qt::WindowMaximized);
        kit.exec();
        double newCoste = kit.getCoste();
        if(newCoste != oArticulo->coste_real)
        {
            QHash <QString, QVariant> h;
            QString error;
            h["coste"] = h["coste_real"] = newCoste;
            bool success = SqlCalls::SqlUpdate(h,"articulos",Configuracion_global->groupDB,QString("id=%1").arg(oArticulo->id),
                                               error);
            if(success)
            {
                oArticulo->coste = oArticulo->coste_real = newCoste;
                QMessageBox::information(this,tr("Coste actualizado"),tr("Asegurese de revisas sus tarifas"));
            }
            else
            {
                QMessageBox::critical(this,tr("Error al actualizar coste"),error);
            }
        }
        LLenarCampos(ui->Pestanas->currentIndex());
    }
}

void FrmArticulos::on_btnAnadir_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    on_botAnadir_clicked();
}

void FrmArticulos::on_btnEditarOferta_clicked()
{
    ui->txtOferta_Descripcion_promocion->setEnabled(true);
    ui->chkOferta_32->setEnabled(true);
    ui->chkOferta_dto->setEnabled(true);
    ui->chkOferta_web->setEnabled(true);
    ui->chkOferta_pvp->setEnabled(true);
    ui->frame_tipo_32->setEnabled(ui->chkOferta_32->isChecked());
    ui->btnAnadir_oferta->setEnabled(false);
    ui->btnEditarOferta->setEnabled(false);
    ui->btnguardar_oferta->setEnabled(true);
    ui->btnDeshacer_oferta->setEnabled(true);
}



void FrmArticulos::on_chkOferta_32_toggled(bool checked)
{
    ui->frame_tipo_32->setEnabled(checked);
    ui->txtOferta_por_cada->setEnabled(checked);
    ui->txtOfertaregalo_de->setEnabled(checked);
    ui->lbl_por_cada->setEnabled(checked);
    ui->lbl_unidades->setEnabled(checked);
    ui->lbl_regalo_de->setEnabled(checked);
    ui->lbl_unidades_2->setEnabled(checked);
    ui->txtOferta_por_cada->setFocus();

}

void FrmArticulos::on_chkOferta_dto_toggled(bool checked)
{
    ui->frame_dto->setEnabled(checked);
    ui->txtOfertaDtoOferta->setEnabled(checked);
    ui->lbl_oferta_dto->setEnabled(checked);
    ui->txtOfertaDtoOferta->setFocus();
}

void FrmArticulos::on_chkOferta_web_toggled(bool checked)
{
    ui->frame_ofertaweb->setEnabled(checked);
    ui->lbl_dto_web->setEnabled(checked);
    ui->lbl_dto_web2->setEnabled(checked);
    ui->txtOferta_dto_web->setEnabled(checked);
    ui->txtOferta_dto_web->setFocus();
}

void FrmArticulos::on_chkOferta_pvp_toggled(bool checked)
{
    ui->frame_pvp_fijo->setEnabled(checked);
    ui->lbl_oferta_importe->setEnabled(checked);
    ui->txtoferta_pvp_fijo->setEnabled(checked);
    ui->txtoferta_pvp_fijo->setFocus();
}

void FrmArticulos::on_btnActivarOferta_clicked()
{
    if(QMessageBox::question(this,tr("Gestión de artíclos"),
                             tr("¿Desea cambiar la oferta activa para este producto?"),tr("No"),tr("Sí"))== QMessageBox::Accepted)
    {
        if(!ui->tabla_ofertas->currentIndex().isValid())
            return;

        QModelIndex index = ui->tabla_ofertas->currentIndex();
        int id = ui->tabla_ofertas->model()->data(ui->tabla_ofertas->currentIndex().model()->index(index.row(),0)).toInt();
        QHash <QString , QVariant > oferta;
        QString error;
        oferta["activa"] = false;
        bool success = SqlCalls::SqlUpdate(oferta,"articulos_ofertas",Configuracion_global->empresaDB,
                                           QString("id_articulo= %1").arg(oArticulo->id),error);
        if(success)
        {
            oferta["activa"] = true;
            success = SqlCalls::SqlUpdate(oferta,"articulos_ofertas",Configuracion_global->empresaDB,QString("id=%1").arg(id),error);
            if(!success)
                QMessageBox::warning(this,tr("Gestión de artículos"),tr("Ocurrió un error al modificar la oferta activa:%1").arg(error),
                                     tr("Aceptar"));
            else
                promociones->setQuery(QString("select id,activa, descripcion from articulos_ofertas where id_articulo = %1 order by activa desc").arg(oArticulo->id),
                                        Configuracion_global->empresaDB);
        } else
        {
            QMessageBox::warning(this,tr("Gestión de artículos"),tr("Ocurrió un error al modificar oferta activa: %1").arg(error),
                                 tr("Aceptar"));
        }
    }


}

void FrmArticulos::on_btnBorrar_oferta_clicked()
{
    if(QMessageBox::question(this,tr("Gestión de artículos"),tr("¿Desea borrar la oferta?\nEsta opción no se puede deshacer"),
                             tr("No"),tr("Sí"))==QMessageBox::Accepted)
    {
        if(!ui->tabla_ofertas->currentIndex().isValid())
            return;

        QModelIndex index = ui->tabla_ofertas->currentIndex();
        int id = ui->tabla_ofertas->model()->data(ui->tabla_ofertas->currentIndex().model()->index(index.row(),0)).toInt();
        QString error;
        bool success = SqlCalls::SqlDelete("articulos_ofertas",Configuracion_global->empresaDB,QString("id=%1").arg(id),error);
        if(success)
            promociones->setQuery(QString("select id,activa, descripcion from articulos_ofertas where id_articulo = %1 order by activa desc").arg(oArticulo->id),
                                    Configuracion_global->empresaDB);
        else
            QMessageBox::warning(this,tr("Gestión de artículos"),tr("Ocurrió un error al borrar oferta: %1").arg(error),
                                 tr("Aceptar"));
    }
}



void FrmArticulos::on_btnBorrarImagen_1_clicked()
{
    QHash <QString, QVariant> h;
    QString error;
    h["imagen"] = "NULL";
    bool succes = SqlCalls::SqlUpdate(h,"articulos",Configuracion_global->groupDB,QString("id=%1").arg(oArticulo->id),error);
    if(!succes)
        QMessageBox::warning(this,tr("Gestión de Artículos"),tr("No se pudo borrar la imagen: %1").arg(error));
    else
          ui->lblImagenArticulo_1->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
}

void FrmArticulos::on_btnBorrarimagen_2_clicked()
{
    QHash <QString, QVariant> h;
    QString error;
    h["imagen2"] = "";
    bool succes = SqlCalls::SqlUpdate(h,"articulos",Configuracion_global->groupDB,QString("id=%1").arg(oArticulo->id),error);
    if(!succes)
        QMessageBox::warning(this,tr("Gestión de Artículos"),tr("No se pudo borrar la imagen: %1").arg(error));
    else
          ui->lblImagenArticulo_2->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
}

void FrmArticulos::on_btnBorrarImagen_3_clicked()
{
    QHash <QString, QVariant> h;
    QString error;
    h["imagen3"] = "";
    bool succes = SqlCalls::SqlUpdate(h,"articulos",Configuracion_global->groupDB,QString("id=%1").arg(oArticulo->id),error);
    if(!succes)
        QMessageBox::warning(this,tr("Gestión de Artículos"),tr("No se pudo borrar la imagen: %1").arg(error));
    else
          ui->lblImagenArticulo_3->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
}

void FrmArticulos::on_btnBorrarimagen_4_clicked()
{
    QHash <QString, QVariant> h;
    QString error;
    h["imagen4"] = "";
    bool succes = SqlCalls::SqlUpdate(h,"articulos",Configuracion_global->groupDB,QString("id=%1").arg(oArticulo->id),error);
    if(!succes)
        QMessageBox::warning(this,tr("Gestión de Artículos"),tr("No se pudo borrar la imagen: %1").arg(error));
    else
          ui->lblImagenArticulo_4->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
}



void FrmArticulos::on_Pestanas_currentChanged(int index)
{
    LLenarCampos(index);
}

void FrmArticulos::on_botBuscarGrupo_clicked()
{
    db_consulta_view consulta(this);
    QStringList campos;
    campos <<"id" << "codigo" <<"grupo_art";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Grupos");
    consulta.set_db("Maya");
    consulta.set_SQL(QString("select id,codigo, grupo_art from gruposart where id_subsubfamilia = %1").arg(oArticulo->id_subSubFamilia));
    QStringList cabecera;
    QVariantList tamanos;

    cabecera  << tr("Id") <<tr("código") << tr("Grupo");
    tamanos <<"0" << "100"  <<"500";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Grupos");
    consulta.setFocus();
    if(consulta.exec())
    {
        oArticulo->id_grupoart = consulta.get_id();
        oArticulo->cGrupoArt = consulta.get_record().value("grupo_art").toString();
        oArticulo->cod_GrupoArt = consulta.get_record().value("codigo").toString();
        ui->txtcGupoArt->setText(consulta.get_record().value("grupo_art").toString());
    }
}


void FrmArticulos::on_btnResArt_clicked()
{
    QStringList hd;
    hd << tr("codigo") << tr("Descripción") <<tr("Stock almacen") <<tr("Pend.Recibir") <<tr("stock real");
    QString error;
    if(!Configuracion_global->SqlToODS("art_1.ods",QString("select codigo,descripcion_reducida,stock_fisico_almacen,cantidad_pendiente_recibir,stock_real from articulos order "
                                                       "by stock_fisico_almacen"),Configuracion_global->groupDB,hd,error))
        QMessageBox::warning(this,tr("Articulos"),tr("Ocurrió un error: %1").arg(error),
                             tr("Aceptar"));
    else
        TimedMessageBox *m = new TimedMessageBox(this,tr("El documento se ha creado en la carpeta del programa con el nombre art_1.ods"));
}

void FrmArticulos::on_btnResArt2_clicked()
{
    QStringList hd;
    hd << tr("codigo") << tr("Descripción") <<tr("PVP") <<tr("%IVA") ;
    QString error;
    if(!Configuracion_global->SqlToODS("art_2.ods",QString("select codigo,descripcion_reducida,pvp,iva from articulos right"
                                                            " join  tiposiva on  articulos.id_tipos_iva = tiposiva.id;"
                                                       "by stock_fisico_almacen"),Configuracion_global->groupDB,hd,error))
        QMessageBox::warning(this,tr("Articulos"),tr("Ocurrió un error: %1").arg(error),
                             tr("Aceptar"));
    else
        TimedMessageBox *m = new TimedMessageBox(this,tr("El documento se ha creado en la carpeta del programa con el nombre art_2.ods"));

}
