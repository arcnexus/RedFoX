#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "openrptLibs/include/data.h"
#include "../Auxiliares/frmcambiarusuario.h"
Configuracion * Configuracion_global = 0;

void MainWindow::crear_barraMantenimiento()
{
    bool addSpacer = false;
    if(!_mantenModules.isEmpty())
    {
        QVector<MayaModule*>::Iterator mantenIter;
        for(mantenIter = _mantenModules.begin(); mantenIter!=_mantenModules.end();++mantenIter)
        {
            MayaModule * mm = *mantenIter;
            ui->stackedWidget->addWidget(mm);
            connect(mm->ModuleMenuBarButton(),SIGNAL(triggered()),this,SLOT(handle_toolBar()));
            if(mm->ModuleMenuPath().isEmpty())
                ui->menuArchivos->addAction(mm->ModuleMenuBarButton());
            else
            {
                //TODO subPaths
            }
            connect(mm,SIGNAL(block()),this,SLOT(block_main()));
            connect(mm,SIGNAL(unblock()),this,SLOT(unblock_main()));

            bool b;
            QPushButton * pb = mm->wantShortCut(b) ;
            if(b)
            {
                addShortCut(pb);
                addSpacer = true;
            }
        }
    }
    if(!_mantenExtensions.isEmpty())
    {
        QVector<ModuleExtension*>::Iterator iter;
        for(iter=_mantenExtensions.begin();iter!=_mantenExtensions.end();++iter)
        {
            if((*iter)->ExtensionPath().isEmpty())
               ui->menuArchivos->addActions((*iter)->Extensions());
            else
            {
                QMenu* in_menu = new QMenu((*iter)->ExtensionPath(),this);
                in_menu->addActions((*iter)->Extensions());
                ui->menuArchivos->addMenu(in_menu);
            }
        }
    }
    QAction* exit = new QAction("Salir",this);
    connect(exit,SIGNAL(triggered()),this,SLOT(close()));
    ui->menuArchivos->addSeparator();
    ui->menuArchivos->addAction(exit);
    if(addSpacer)
        ui->shortCutContainer->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred));
}

void MainWindow::crear_barraAlmacen()
{
    bool addSpacer = false;
    QMenu* menu =0;
    if(!_almacenModules.isEmpty())
    {
        menu = new QMenu(tr("Almacen"),this);
        QVector<MayaModule*>::Iterator almacenIter;
        for(almacenIter = _almacenModules.begin(); almacenIter!=_almacenModules.end();++almacenIter)
        {
            MayaModule * mm = *almacenIter;
            ui->stackedWidget->addWidget(mm);
            connect(mm->ModuleMenuBarButton(),SIGNAL(triggered()),this,SLOT(handle_toolBar()));
            if(mm->ModuleMenuPath().isEmpty())
                menu->addAction(mm->ModuleMenuBarButton());
            else
            {
                //TODO subPaths
            }
            connect(mm,SIGNAL(block()),this,SLOT(block_main()));
            connect(mm,SIGNAL(unblock()),this,SLOT(unblock_main()));

            bool b;
            QPushButton * pb = mm->wantShortCut(b) ;
            if(b)
            {
                addShortCut(pb);
            addSpacer = true;
        }
        }
    }
    if(!_almacenExtensions.isEmpty())
    {
        if(menu == 0)
            menu = new QMenu(tr("Almacen"),this);
        QVector<ModuleExtension*>::Iterator iter;
        for(iter=_almacenExtensions.begin();iter!=_almacenExtensions.end();++iter)
        {
            if((*iter)->ExtensionPath().isEmpty())
               menu->addActions((*iter)->Extensions());
            else
            {
                QMenu* in_menu = new QMenu((*iter)->ExtensionPath(),this);
                in_menu->addActions((*iter)->Extensions());
                menu->addMenu(in_menu);
            }
        }
    }
    if(menu)
        ui->menubar->addMenu(menu);
    if(addSpacer)
        ui->shortCutContainer->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred));
}

void MainWindow::crear_barraVentas()
{
    bool addSpacer = false;
    if(!_ventasModules.isEmpty())
    {
        QMenu* menu = new QMenu(tr("Ventas"),this);
        QVector<MayaModule*>::Iterator ventasIter;
        for(ventasIter = _ventasModules.begin(); ventasIter!=_ventasModules.end();++ventasIter)
        {
            MayaModule * mm = *ventasIter;
            ui->stackedWidget->addWidget(mm);
            connect(mm->ModuleMenuBarButton(),SIGNAL(triggered()),this,SLOT(handle_toolBar()));
            if(mm->ModuleMenuPath().isEmpty())
                menu->addAction(mm->ModuleMenuBarButton());
            else
            {
                //TODO subPaths
            }
            connect(mm,SIGNAL(block()),this,SLOT(block_main()));
            connect(mm,SIGNAL(unblock()),this,SLOT(unblock_main()));

            bool b;
            QPushButton * pb = mm->wantShortCut(b) ;
            if(b)
            {
                addShortCut(pb);
                addSpacer = true;
            }
        }
        ui->menubar->addMenu(menu);
    }
    if(addSpacer)
        ui->shortCutContainer->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred));
}

void MainWindow::crear_barraCompras()
{
    bool addSpacer = false;
    if(!_comprasModules.isEmpty())
    {
        QMenu* menu = new QMenu(tr("Compras"),this);

        QVector<MayaModule*>::Iterator comprasIter;
        for(comprasIter = _comprasModules.begin(); comprasIter!=_comprasModules.end();++comprasIter)
        {
            MayaModule * mm = *comprasIter;
            ui->stackedWidget->addWidget(mm);
            connect(mm->ModuleMenuBarButton(),SIGNAL(triggered()),this,SLOT(handle_toolBar()));
            if(mm->ModuleMenuPath().isEmpty())
                menu->addAction(mm->ModuleMenuBarButton());
            else
            {
                //TODO subPaths
            }
            connect(mm,SIGNAL(block()),this,SLOT(block_main()));
            connect(mm,SIGNAL(unblock()),this,SLOT(unblock_main()));

            bool b;
            QPushButton * pb = mm->wantShortCut(b) ;
            if(b)
            {
                addShortCut(pb);
                addSpacer = true;
            }
        }

        ui->menubar->addMenu(menu);
    }
    if(addSpacer)
        ui->shortCutContainer->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred));
}

void MainWindow::crear_barraUtils()
{
    bool addSpacer = false;
    if(!_utilsModules.isEmpty())
    {
        QMenu* menu = new QMenu(tr("Utilidades"),this);

        QVector<MayaModule*>::Iterator utilIter;
        for(utilIter = _utilsModules.begin(); utilIter!= _utilsModules.end();++utilIter)
        {
            MayaModule * mm = *utilIter;
            ui->stackedWidget->addWidget(mm);
            connect(mm->ModuleMenuBarButton(),SIGNAL(triggered()),this,SLOT(handle_toolBar()));
            if(mm->ModuleMenuPath().isEmpty())
                menu->addAction(mm->ModuleMenuBarButton());
            else
            {
                //TODO subPaths
            }
            connect(mm,SIGNAL(block()),this,SLOT(block_main()));
            connect(mm,SIGNAL(unblock()),this,SLOT(unblock_main()));

            bool b;
            QPushButton * pb = mm->wantShortCut(b) ;
            if(b)
            {
                addShortCut(pb);
                addSpacer = true;
            }
        }

        ui->menubar->addMenu(menu);
    }
    if(addSpacer)
        ui->shortCutContainer->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred));
}

void MainWindow::crear_barraAdmin()
{
    bool addSpacer = false;
    if(!_adminModules.isEmpty())
    {
        QMenu* menu = new QMenu(tr("Administrador"),this);

        QVector<MayaModule*>::Iterator Iter;
        for(Iter = _adminModules.begin(); Iter!=_adminModules.end();++Iter)
        {
            MayaModule * mm = *Iter;
            ui->stackedWidget->addWidget(mm);
            connect(mm->ModuleMenuBarButton(),SIGNAL(triggered()),this,SLOT(handle_toolBar()));
            if(mm->ModuleMenuPath().isEmpty())
                menu->addAction(mm->ModuleMenuBarButton());
            else
            {
                //TODO subPaths
            }
            connect(mm,SIGNAL(block()),this,SLOT(block_main()));
            connect(mm,SIGNAL(unblock()),this,SLOT(unblock_main()));

            bool b;
            QPushButton * pb = mm->wantShortCut(b) ;
            if(b)
            {
                addShortCut(pb);
                addSpacer = true;
            }
        }

        ui->menubar->addMenu(menu);
    }
    if(addSpacer)
        ui->shortCutContainer->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred));
}

void MainWindow::crear_barraContabilidad()
{
    bool addSpacer = false;
    if(!_contaModules.isEmpty())
    {
        QMenu* menu = new QMenu(tr("Contabilidad"),this);

        QVector<MayaModule*>::Iterator Iter;
        for(Iter = _contaModules.begin(); Iter!=_contaModules.end();++Iter)
        {
            MayaModule * mm = *Iter;
            ui->stackedWidget->addWidget(mm);
            connect(mm->ModuleMenuBarButton(),SIGNAL(triggered()),this,SLOT(handle_toolBar()));
            if(mm->ModuleMenuPath().isEmpty())
                menu->addAction(mm->ModuleMenuBarButton());
            else
            {
                //TODO subPaths
            }
            connect(mm,SIGNAL(block()),this,SLOT(block_main()));
            connect(mm,SIGNAL(unblock()),this,SLOT(unblock_main()));

            bool b;
            QPushButton * pb = mm->wantShortCut(b) ;
            if(b)
            {
                addShortCut(pb);
                addSpacer = true;
            }
        }

        ui->menubar->addMenu(menu);
    }
    if(addSpacer)
        ui->shortCutContainer->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred));
}

void MainWindow::crear_barraClinica()
{
    bool addSpacer = false;
    QMenu* menu = 0;
    if(!_clinicaModules.isEmpty())
    {
        menu = new QMenu(tr("Clinica"),this);

        QVector<MayaModule*>::Iterator almacenIter;
        for(almacenIter = _clinicaModules.begin(); almacenIter!=_clinicaModules.end();++almacenIter)
        {
            MayaModule * mm = *almacenIter;
            ui->stackedWidget->addWidget(mm);
            connect(mm->ModuleMenuBarButton(),SIGNAL(triggered()),this,SLOT(handle_toolBar()));
            if(mm->ModuleMenuPath().isEmpty())
                menu->addAction(mm->ModuleMenuBarButton());
            else
            {
                //TODO subPaths
            }
            connect(mm,SIGNAL(block()),this,SLOT(block_main()));
            connect(mm,SIGNAL(unblock()),this,SLOT(unblock_main()));

            bool b;
            QPushButton * pb = mm->wantShortCut(b) ;
            if(b)
            {
                addShortCut(pb);
                addSpacer = true;
            }
        }
    }
    if(!_clinicaExtensions.isEmpty())
    {
        if(menu == 0)
            menu = new QMenu(tr("Clinica"),this);
        QVector<ModuleExtension*>::Iterator iter;
        for(iter=_clinicaExtensions.begin();iter!=_clinicaExtensions.end();++iter)
        {
            if((*iter)->ExtensionPath().isEmpty())
               menu->addActions((*iter)->Extensions());
            else
            {
                QMenu* in_menu = new QMenu((*iter)->ExtensionPath(),this);
                in_menu->addActions((*iter)->Extensions());
                menu->addMenu(in_menu);
            }
        }
    }
    if(menu)
        ui->menubar->addMenu(menu);
}

void MainWindow::addShortCut(QPushButton *button)
{

    _shortCuts.insert(button,(QWidget*)button->parent());
    ui->shortCutContainer->addWidget(button);
    button->setMinimumWidth(34);
    button->setMinimumHeight(34);
    button->setIconSize(QSize(30,30));
    connect(button,SIGNAL(clicked()),this,SLOT(handle_toolBar()));
}

void MainWindow::loadMantenModules(QSplashScreen* splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de clientes") ,Qt::AlignLeft,Qt::white);

    frmClientes* Clientes = new frmClientes(this);
    if(Clientes->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _mantenModules.append(Clientes);
    }
    else
        delete Clientes;

    splash->showMessage(tr("Cargando modulos... Modulo de proveedores")  ,Qt::AlignLeft,Qt::white);
    frmProveedores * Proveedores = new frmProveedores(this);
    if(Proveedores->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _mantenModules.append(Proveedores);
    }
    else
        delete Proveedores;


    splash->showMessage(tr("Cargando modulos... Modulo de Transportistas")  ,Qt::AlignLeft,Qt::white);
    FrmTransportistas * Transportistas = new FrmTransportistas(this);
    if(Transportistas->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _mantenModules.append(Transportistas);
    }
    else
        delete Transportistas;

    splash->showMessage(tr("Cargando modulos... Modulo de Formas de Pago")  ,Qt::AlignLeft,Qt::white);
    FrmFormas_pago * fpagos = new FrmFormas_pago(this);
    if(fpagos->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _mantenModules.append(fpagos);
    }
    else
        delete fpagos;


    ArchivosGeneralesExt* e = new ArchivosGeneralesExt(this);
    if(!e->Extensions().isEmpty())
        _mantenExtensions.append(e);
    else
        delete e;
}

void MainWindow::loadAlmacenModules(QSplashScreen* splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de articulos") ,Qt::AlignLeft,Qt::white );
    FrmArticulos* Articulos = new FrmArticulos(this);
    if(Articulos->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _almacenModules.append(Articulos);
        QTimer::singleShot(0,Articulos,SLOT(init()));
    }
    else
        delete Articulos;

    splash->showMessage(tr("Cargando modulos... Modulo de articulos") ,Qt::AlignLeft,Qt::white );
    frmInventario* Inventario = new frmInventario(this);
    if(Inventario->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _almacenModules.append(Inventario);
    }
    else
        Inventario->deleteLater();
    DivisionAlmacenExt* x = new DivisionAlmacenExt(this);
    if(!x->Extensions().isEmpty())
        _almacenExtensions.append(x);
    else
        delete x;
}

void MainWindow::loadVentasModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de presupuestos")  ,Qt::AlignLeft,Qt::white);

    FrmPresupuestosCli* Presupcli = new FrmPresupuestosCli(this);
    if(Presupcli->userHaveAcces(Configuracion_global->id_usuario_activo))
        _ventasModules.append(Presupcli);
    else
        delete Presupcli;

    splash->showMessage(tr("Cargando modulos... Modulo de pedidos")  ,Qt::AlignLeft,Qt::white);

    FrmPedidos* Pedidos = new FrmPedidos(this);
    if(Pedidos->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(Pedidos);
    }
    else
        delete Pedidos;

    splash->showMessage(tr("Cargando modulos... Modulo de albaranes")  ,Qt::AlignLeft,Qt::white);

    FrmAlbaran* Albaran = new FrmAlbaran(this);
    if(Albaran->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(Albaran);
    }
    else
        delete Albaran;

    splash->showMessage(tr("Cargando modulos... Modulo de facturación de albaranes")  ,Qt::AlignLeft,Qt::white);

    FrmFacturarAlabaranes* frmFactura_multiple = new FrmFacturarAlabaranes(this);
    if(frmFactura_multiple->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(frmFactura_multiple);
    }
    else
        delete frmFactura_multiple;

    splash->showMessage(tr("Cargando modulos... Modulo de facturas")  ,Qt::AlignLeft,Qt::white);

    frmFacturas* Facturas = new frmFacturas(this);
    if(Facturas->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(Facturas);
    }
    else
        delete Facturas;

    splash->showMessage(tr("Cargando modulos... Modulo de Ventas: gestión de cobros")  ,Qt::AlignLeft,Qt::white);

    frmGestionCobros* frmgestcobros = new frmGestionCobros(this);
    if(frmgestcobros->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(frmgestcobros);
    }
    else
        delete frmgestcobros;

    splash->showMessage(tr("Cargando modulos... Modulo de TPV")  ,Qt::AlignLeft,Qt::white);

    FrmTPV * frm_tpv = new FrmTPV(this);

    if(frm_tpv->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(frm_tpv);
    }
    else
        delete frm_tpv;
}

void MainWindow::loadComprasModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de Compras: Orden de Pedido") ,Qt::AlignLeft,Qt::white );

    FrmOrden_Pedido_Producto * frmOrden_Ped_pro = new FrmOrden_Pedido_Producto(this);
    if(frmOrden_Ped_pro->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _comprasModules.append(frmOrden_Ped_pro);
    }
    else
        delete frmOrden_Ped_pro;

    splash->showMessage(tr("Cargando modulos... Modulo de Compras: pedidos")  ,Qt::AlignLeft,Qt::white);

    FrmPedidosProveedor* FrmPedidos_pro = new FrmPedidosProveedor(this);
    if(FrmPedidos_pro->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _comprasModules.append(FrmPedidos_pro);
    }
    else
        delete FrmPedidos_pro;


    splash->showMessage(tr("Cargando modulos... Modulo de Compras: Recepción de Pedidos")  ,Qt::AlignLeft,Qt::white);
    Frmrecepcion_pedidos* frmRecep_pedidos = new Frmrecepcion_pedidos(this);
    if(frmRecep_pedidos->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _comprasModules.append(frmRecep_pedidos);
    }
    else
        delete frmRecep_pedidos;

    splash->showMessage(tr("Cargando modulos... Modulo de Compras: albaranes")  ,Qt::AlignLeft,Qt::white);
    FrmAlbaranProveedor* FrmAlbaran_pro = new FrmAlbaranProveedor(this);
    if(FrmAlbaran_pro->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _comprasModules.append(FrmAlbaran_pro);
    }
    else
        delete FrmAlbaran_pro;

    splash->showMessage(tr("Cargando modulos... Modulo de Compras: facturas") ,Qt::AlignLeft,Qt::white );
    FrmFacturasProveedor* frmFacturas_pro = new FrmFacturasProveedor(this);
    if(frmFacturas_pro->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _comprasModules.append(frmFacturas_pro);
    }
    else
        delete frmFacturas_pro;
}

void MainWindow::loadUtilsModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de Utilidades: Agenda")  ,Qt::AlignLeft,Qt::white);
    AgendaForm* agendaForm = new AgendaForm(this);
    if(agendaForm->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _utilsModules.append(agendaForm);
    }
    else
        delete agendaForm;

    splash->showMessage(tr("Cargando modulos... Editor de reportes") ,Qt::AlignLeft,Qt::white );
    RepDesignModule * reportWindow = new RepDesignModule(this);
    if(reportWindow->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _utilsModules.append(reportWindow);
    }
    else
        delete reportWindow;
}

void MainWindow::loadAminModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de Administracion: Configuración general")  ,Qt::AlignLeft,Qt::white);
    frmConfigmaya* c = new frmConfigmaya(this);
    if(c->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _adminModules.append(c);
    }
    else
        delete c;

    splash->showMessage(tr("Cargando modulos... Modulo de Administracion: Configurar empresa")  ,Qt::AlignLeft,Qt::white);
    FrmEmpresas* e = new FrmEmpresas(this);
    if(e->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _adminModules.append(e);
    }
    else
        delete e;

    splash->showMessage(tr("Cargando modulos... Modulo de Administracion: Usuarios")  ,Qt::AlignLeft,Qt::white);
    FrmUsuarios* u = new FrmUsuarios(this);
    if(u->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _adminModules.append(u);
    }
    else
        delete u;
}

void MainWindow::loadContaModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de Contabilidad: Cuadro de cuentas")  ,Qt::AlignLeft,Qt::white);
    frmCuadro_cuentas* f = new frmCuadro_cuentas(this);
    if(f->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _contaModules.append(f);
    }
    else
        delete f;


    splash->showMessage(tr("Cargando modulos... Modulo de Contabilidad: Diario de apuntes")  ,Qt::AlignLeft,Qt::white);
    FrmEntrada_apuntes * frmentrada_apuntes = new FrmEntrada_apuntes(this);
    if(frmentrada_apuntes->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _contaModules.append(frmentrada_apuntes);
    }
    else
        delete frmentrada_apuntes;

}

void MainWindow::loadSecMedModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Extension de Clinica")  ,Qt::AlignLeft,Qt::white);
    ClinicaExt* e = new ClinicaExt(this);
    if(!e->Extensions().isEmpty())
        _clinicaExtensions.append(e);
    else
        delete e;
}

void MainWindow::updateDivisas()
{
    if(Configuracion_global->actualizar_divisas)
    {
        connect(Configuracion_global,SIGNAL(cambioReady(float,QString)),this,SLOT(actualizar_divisas(float,QString)));
        QSqlQuery divisas(Configuracion_global->groupDB);
        if(divisas.exec("select * from monedas"))
        {
            while (divisas.next()) {
               if(divisas.record().value("fecha_cambio").toDate()!=QDate::currentDate())
                {
                    Configuracion_global->getCambio("EUR",divisas.record().value("nombre_corto").toString(),1);
                }
            }
        }
    }
}

void MainWindow::loadModules()
{
    QPixmap pixmap(":/Icons/PNG/Neux_red_fox.png");
    QSplashScreen splash(pixmap);
    splash.show();

    loadMantenModules(&splash);
    crear_barraMantenimiento();

    loadAlmacenModules(&splash);
    crear_barraAlmacen();

    loadComprasModules(&splash);
    crear_barraCompras();

    loadVentasModules(&splash);
    crear_barraVentas();

    loadUtilsModules(&splash);
    crear_barraUtils();

    loadContaModules(&splash);
    crear_barraContabilidad();

    loadSecMedModules(&splash);
    crear_barraClinica();

    loadAminModules(&splash);
    crear_barraAdmin();

    MayaForm = new init_form(this);
    ui->stackedWidget->addWidget(MayaForm);
    ui->stackedWidget->setCurrentWidget(MayaForm);

    QApplication::processEvents();

    updateDivisas();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{    
    ui->setupUi(this);

    this->installEventFilter(this);
    this->setMouseTracking(true);

    m_avisos = new BarraAvisos(this);
    m_avisos->show();
    m_avisos->setGeometry(0,70,20,80);


    connect(m_avisos,SIGNAL(hideMe()),this,SLOT(hideAvisos()));
    connect(m_avisos,SIGNAL(showMe()),this,SLOT(showAvisos()));

    on_edit = false;

 //   QTimer *timer = new QTimer(this);
 //   connect(timer, SIGNAL(timeout()), this, SLOT(llenaravisos()));
 //   timer->start(300000);
    llenaravisos();

//    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);

// QString error;
// QStringList w;
// w << "id < 10" << "id > 5";
// QMap<int,QString> idnombre = SqlCalls::SelectMap<int,QString>("clientes","idpatata","nombre_fiscal",w,Configuracion_global->groupDB,error);
// qDebug() << idnombre;
// qDebug() << idnombre.value(3);
// qDebug() << error;

    // Add record.
// QHash<QString,QVariant> v;
// v["nombre_fiscal"]="Paco";
// v["telefono1"]="555333222";
// v["bloqueado"]=true;

// int id = SqlCalls::SqlInsert(v,"clientes",Configuracion_global->groupDB,error);
// if(id < 0)
// qDebug() << error;
// else
// qDebug() << id;
    // Recuperar valores registro
// QMap<int, QSqlRecord> map = SqlCalls::SelectRecord("clientes", "id<10",Configuracion_global->groupDB, error);
// QMapIterator<int, QSqlRecord> i(map);
// while (i.hasNext())
// {
// i.next();
// qDebug() << i.value();
// }
// qDebug() << map.value(7).value("nombre").toString();   
    keepAlive.setInterval(360000);

    connect(&keepAlive, &QTimer::timeout,[this]{
        QSqlQuery q(Configuracion_global->groupDB);
        q.exec("Select * from monedas limit 1");
        QSqlQuery q2(Configuracion_global->empresaDB);
        q2.exec("Select * from lin_fac limit 1");
        QSqlQuery q3(Configuracion_global->contaDB);
        q3.exec("Select * from diario limit 1");
        if(Configuracion_global->medic)
        {
            //TODO reactivar bd medic
           // QSqlQuery q4(Configuracion_global->medicaDB);
           // q4.exec("Select * from pacientes limit 1");
        }
    });
    keepAlive.start(360000);
}

void MainWindow::block_main()
{
    blockMe(true);
}

void MainWindow::unblock_main()
{
    blockMe(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showInfo()
{
    if(!Configuracion_global->medic)
        this->setWindowTitle("RedFox SGC - "+tr("Software GNU de gestión empresarial. (Empresa activa:")+empresa+
                             tr(" - Ejercicio activo: ")+Configuracion_global->cEjercicio+")");
    else

    this->setWindowTitle("RedFox SGC - "+tr("Software GNU para los profesionales de la salud. (Empresa activa:")+empresa+
                         tr(" - Ejercicio activo: ")+Configuracion_global->cEjercicio+")");
  // ui->lineEmpresaActiva->setText(empresa);
    ui->lineUsuarioActivo->setText(user);
    Configuracion_global->cUsuarioActivo = user;
    Configuracion_global->nombreEmpresa = empresa;
}

void MainWindow::actualizar_divisas(float valor_divisa, QString divisaDest)
{
    QSqlQuery valor(Configuracion_global->groupDB);
    QString fecha_hoy = QDate::currentDate().toString("yyyy.MM.dd");
    valor.prepare("update monedas set cambio =:divisa,fecha_cambio =:fecha where nombre_corto =:id");
    valor.bindValue(":divisa",valor_divisa);
    valor.bindValue(":fecha",fecha_hoy);
    valor.bindValue(":id",divisaDest);
    valor.exec();
}

void MainWindow::on_btn_bloquear_clicked()
{
    block_Maya_form form(this);
    form.set_user(user,pass);
    this->hide();
    form.exec();
    this->show();
}

void MainWindow::handle_toolBar()
{
    QAction * a = qobject_cast<QAction *>(sender());
    if(a)
       ui->stackedWidget->setCurrentWidget((QWidget*)a->parent());

    QPushButton * p = qobject_cast<QPushButton *>(sender());
    if(p)
        ui->stackedWidget->setCurrentWidget(_shortCuts.value(p));
}

void MainWindow::closeEvent(QCloseEvent * e)
{
    if(on_edit)
    {
        if(!QMessageBox::question(this,
                                 tr("Cerrando"),
                                 tr("Existen cambios sin guardar.\n"
                                    "Si continua se perderan dichos cambios.\n"
                                    "¿Desea continuar?"),
                                 tr("Cancelar"),
                                  tr("&Continuar"))== QMessageBox::Accepted)
        {
            e->ignore();
            return;
        }
    }
}

void MainWindow::blockMe(bool state)
{
    ui->menubar->setEnabled(!state);
    QList<QPushButton*> buttons = ui->frameusuario->findChildren<QPushButton*>();
    QPushButton* b;
    foreach (b, buttons) {
        if(b!= ui->btn_bloquear)
            if(b!=ui->btn_salir)
                b->setEnabled(!state);
    }
    on_edit = state;
}

void MainWindow::showAvisos()
{
    m_avisos->setGeometry(0,70,0,this->height()-80);
    QPropertyAnimation* animation0 = new QPropertyAnimation(m_avisos, "size",this);

    connect(animation0,SIGNAL(finished()),animation0,SLOT(deleteLater()));

    animation0->setDuration(1000);
    animation0->setEasingCurve(QEasingCurve::OutElastic);

    animation0->setStartValue(QSize(20,m_avisos->height()));
    animation0->setEndValue(QSize(650,m_avisos->height()));

    animation0->start();
    m_avisos->setShow(true);
    m_avisos->llenarAvisos();

    //test
    m_avisos->setColor(QColor(100,0,0));
}


void MainWindow::handle_permisosAgenda()
{
    PermisosAgendaForm form(this);
    form.exec();
}

//void MainWindow::btn_cuentas_clicked()
//{
// ui->stackedWidget->setCurrentWidget(frmcuentas);
//}


void MainWindow::llenaravisos()
{
   // m_avisos->setColor(QColor(50,50,50));
    /*QMap <int,QSqlRecord> map;
    QStringList clausulas,headers;
    QString error;
    clausulas << QString("id_usuario = %1").arg(Configuracion_global->id_usuario_activo);
    map = SqlCalls::SelectRecord("avisos",clausulas,Configuracion_global->groupDB,error);
    ui->tabla_avisos->setColumnCount(4);
    headers <<"id" <<tr("ORIGEN") << tr("AVISO") <<tr("FECHA/HORA");
    ui->tabla_avisos->setHorizontalHeaderLabels(headers);
    QVariantList  sizes;
    sizes <<0 << 150 <<250 << 110;
    for(int i = 0; i< sizes.size();i++ )
        ui->tabla_avisos->setColumnWidth(i,sizes.at(i).toInt());
    QMapIterator <int,QSqlRecord> it(map);
    int pos = 0;
    QColor rowcolor;
    while (it.hasNext())
    {
        it.next();
        if(pos%2==0)
            rowcolor.setRgb(240,240,255);
        else
            rowcolor.setRgb(255,255,255);
        ui->tabla_avisos->setRowCount(pos+1);
        QTableWidgetItem *item_col0 = new QTableWidgetItem(QString::number(it.value().value("id").toInt()));
        item_col0->setFlags(item_col0->flags() & (~Qt::ItemIsEditable));
        item_col0->setBackgroundColor(rowcolor);

        ui->tabla_avisos->setItem(pos,0,item_col0);
        ui->tabla_avisos->setColumnHidden(0,true);

        QTableWidgetItem *item_col1 = new QTableWidgetItem("User Origen");
        item_col1->setFlags(item_col1->flags() & (~Qt::ItemIsEditable));
        item_col1->setBackgroundColor(rowcolor);

        ui->tabla_avisos->setItem(pos,1,item_col1);

        QTableWidgetItem *item_col2 = new QTableWidgetItem(it.value().value("aviso").toString());
        item_col2->setFlags(item_col2->flags() & (~Qt::ItemIsEditable));
        item_col2->setBackgroundColor(rowcolor);

        ui->tabla_avisos->setItem(pos,2,item_col2);

        QTableWidgetItem *item_col3 = new QTableWidgetItem(it.value().value("fecha_hora_aviso").toDateTime().toString("dd/MM/yyyy-hh:mm"));
        item_col3->setFlags(item_col3->flags() & (~Qt::ItemIsEditable));
        item_col3->setBackgroundColor(rowcolor);

        ui->tabla_avisos->setItem(pos,3,item_col3);
        pos ++;
    }*/
}

void MainWindow::hideAvisos()
{
    QPropertyAnimation* animation0 = new QPropertyAnimation(m_avisos, "size",this);

    connect(animation0,SIGNAL(finished()),animation0,SLOT(deleteLater()));
    connect(animation0,SIGNAL(finished()),this,SLOT(update()));

    animation0->setDuration(100);
    animation0->setEasingCurve(QEasingCurve::Linear);

    animation0->setStartValue(QSize(m_avisos->width(),m_avisos->height()));
    animation0->setEndValue(QSize(20,m_avisos->height()));

    animation0->start();
    m_avisos->setPagina(0);
    m_avisos->setShow(false);
}



bool MainWindow::eventFilter(QObject * t, QEvent * e)
{
    if(e->type() == QEvent::Move)
    {
        if(m_avisos)
            m_avisos->moveGloblo(reinterpret_cast<QMoveEvent *>(e)->pos());
    }
    if(e->type() == QEvent::WindowStateChange)
    {
        QWindowStateChangeEvent * ev = reinterpret_cast<QWindowStateChangeEvent*>(e);
        if(ev->oldState() != Qt::WindowMinimized){
            if(m_avisos)
                m_avisos->hideGlobo();
        }
        else
            if(m_avisos)
                m_avisos->restoreGlobo();
    }
    if(e->type() == QEvent::Resize)
    {
        m_avisos->resize(m_avisos->width(),this->height()-60);
    }
    return false;
}

void MainWindow::on_lineUsuarioActivo_clicked()
{
    frmCambiarUsuario fUsuarios;
    fUsuarios.exec();
}

void MainWindow::on_txtEjercicio_editingFinished()
{
    Configuracion_global->cEjercicio = QString::number(ui->txtEjercicio->date().year());
}
