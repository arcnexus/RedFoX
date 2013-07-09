#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openrptLibs/include/data.h"
Configuracion * Configuracion_global = 0;

void MainWindow::crear_barraMantenimiento()
{
    bool addSpacer = false;
    if(!_mantenModules.isEmpty())
    {
        QWidget* container = new QWidget(this);
        QScrollArea * scroll = new QScrollArea(this);
        scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scroll->setWidgetResizable(true);
        QVBoxLayout * box = new QVBoxLayout(container);
        QVector<MayaModule*>::Iterator mantenIter;
        for(mantenIter = _mantenModules.begin(); mantenIter!=_mantenModules.end();++mantenIter)
        {
            MayaModule * mm = *mantenIter;
            ui->stackedWidget->addWidget(mm);
            box->addWidget(mm->ModuleToolBarButton());
            connect(mm->ModuleToolBarButton(),SIGNAL(clicked()),this,SLOT(handle_toolBar()));
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
        box->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding));
        container->setLayout(box);
        scroll->setWidget(container);
        ui->modulesStack->addWidget(scroll);
        ui->comboBox->addItem(tr("Mantenimiento"));
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
    QMenu* menu  =0;
    if(!_almacenModules.isEmpty())
    {
        menu = new QMenu(tr("Almacen"),this);
        QWidget* container = new QWidget(this);
        QScrollArea * scroll = new QScrollArea(this);
        scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scroll->setWidgetResizable(true);
        QVBoxLayout * box = new QVBoxLayout(container);
        QVector<MayaModule*>::Iterator almacenIter;
        for(almacenIter = _almacenModules.begin(); almacenIter!=_almacenModules.end();++almacenIter)
        {
            MayaModule * mm = *almacenIter;
            ui->stackedWidget->addWidget(mm);
            box->addWidget(mm->ModuleToolBarButton());
            connect(mm->ModuleToolBarButton(),SIGNAL(clicked()),this,SLOT(handle_toolBar()));
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
        box->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding));
        container->setLayout(box);
        scroll->setWidget(container);
        ui->modulesStack->addWidget(scroll);
        ui->comboBox->addItem(tr("Almacen"));
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
        QWidget* container = new QWidget(this);
        QScrollArea * scroll = new QScrollArea(this);
        scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scroll->setWidgetResizable(true);
        QVBoxLayout * box = new QVBoxLayout(container);

        QVector<MayaModule*>::Iterator ventasIter;
        for(ventasIter = _ventasModules.begin(); ventasIter!=_ventasModules.end();++ventasIter)
        {
            MayaModule * mm = *ventasIter;
            ui->stackedWidget->addWidget(mm);
            box->addWidget(mm->ModuleToolBarButton());
            connect(mm->ModuleToolBarButton(),SIGNAL(clicked()),this,SLOT(handle_toolBar()));
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
        box->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding));
        container->setLayout(box);
        scroll->setWidget(container);
        ui->modulesStack->addWidget(scroll);
        ui->comboBox->addItem(tr("Ventas"));
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
        QWidget* container = new QWidget(this);
        QScrollArea * scroll = new QScrollArea(this);
        scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scroll->setWidgetResizable(true);
        QVBoxLayout * box = new QVBoxLayout(container);
        QVector<MayaModule*>::Iterator comprasIter;
        for(comprasIter = _comprasModules.begin(); comprasIter!=_comprasModules.end();++comprasIter)
        {
            MayaModule * mm = *comprasIter;
            ui->stackedWidget->addWidget(mm);
            box->addWidget(mm->ModuleToolBarButton());
            connect(mm->ModuleToolBarButton(),SIGNAL(clicked()),this,SLOT(handle_toolBar()));
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
        box->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding));
        container->setLayout(box);
        scroll->setWidget(container);
        ui->modulesStack->addWidget(scroll);
        ui->comboBox->addItem(tr("Compras"));
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
        QWidget* container = new QWidget(this);
        QScrollArea * scroll = new QScrollArea(this);
        scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scroll->setWidgetResizable(true);
        QVBoxLayout * box = new QVBoxLayout(container);
        QVector<MayaModule*>::Iterator utilIter;
        for(utilIter =  _utilsModules.begin(); utilIter!= _utilsModules.end();++utilIter)
        {
            MayaModule * mm = *utilIter;
            ui->stackedWidget->addWidget(mm);
            box->addWidget(mm->ModuleToolBarButton());
            connect(mm->ModuleToolBarButton(),SIGNAL(clicked()),this,SLOT(handle_toolBar()));
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
        box->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding));
        container->setLayout(box);
        scroll->setWidget(container);
        ui->modulesStack->addWidget(scroll);
        ui->comboBox->addItem(tr("Utilidades"));
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
        QWidget* container = new QWidget(this);
        QScrollArea * scroll = new QScrollArea(this);
        scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scroll->setWidgetResizable(true);
        QVBoxLayout * box = new QVBoxLayout(container);
        QVector<MayaModule*>::Iterator Iter;
        for(Iter =  _adminModules.begin(); Iter!=_adminModules.end();++Iter)
        {
            MayaModule * mm = *Iter;
            ui->stackedWidget->addWidget(mm);
            box->addWidget(mm->ModuleToolBarButton());
            connect(mm->ModuleToolBarButton(),SIGNAL(clicked()),this,SLOT(handle_toolBar()));
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
        box->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding));
        container->setLayout(box);
        scroll->setWidget(container);
        ui->modulesStack->addWidget(scroll);
        ui->comboBox->addItem(tr("Administrador"));
        ui->menubar->addMenu(menu);
    }
    if(addSpacer)
        ui->shortCutContainer->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Preferred));
}

void MainWindow::crear_barraContabilidad()
{
    bool addSpacer = false;
    //btn_cuentas = new ToolBarButton(tr("Cuentas"),":/Icons/PNG/asientos.png",this);
    if(!_contaModules.isEmpty())
    {
        QMenu* menu = new QMenu(tr("Contabilidad"),this);
        QWidget* container = new QWidget(this);
        QScrollArea * scroll = new QScrollArea(this);
        scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scroll->setWidgetResizable(true);
        QVBoxLayout * box = new QVBoxLayout(container);
        QVector<MayaModule*>::Iterator Iter;
        for(Iter =  _contaModules.begin(); Iter!=_contaModules.end();++Iter)
        {
            MayaModule * mm = *Iter;
            ui->stackedWidget->addWidget(mm);
            box->addWidget(mm->ModuleToolBarButton());
            connect(mm->ModuleToolBarButton(),SIGNAL(clicked()),this,SLOT(handle_toolBar()));
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
        box->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding));
        container->setLayout(box);
        scroll->setWidget(container);
        ui->modulesStack->addWidget(scroll);
        ui->comboBox->addItem(tr("Contabilidad"));
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
        QWidget* container = new QWidget(this);
        QScrollArea * scroll = new QScrollArea(this);
        scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        scroll->setWidgetResizable(true);
        QVBoxLayout * box = new QVBoxLayout(container);
        QVector<MayaModule*>::Iterator almacenIter;
        for(almacenIter = _clinicaModules.begin(); almacenIter!=_clinicaModules.end();++almacenIter)
        {
            MayaModule * mm = *almacenIter;
            ui->stackedWidget->addWidget(mm);
            box->addWidget(mm->ModuleToolBarButton());
            connect(mm->ModuleToolBarButton(),SIGNAL(clicked()),this,SLOT(handle_toolBar()));
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
        box->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Preferred,QSizePolicy::Expanding));
        container->setLayout(box);
        scroll->setWidget(container);
        ui->modulesStack->addWidget(scroll);
        ui->comboBox->addItem(tr("Almacen"));
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
    splash->showMessage(tr("Cargando modulos... Modulo de clientes") );

    frmClientes* Clientes = new frmClientes(this);
    if(Clientes->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _mantenModules.append(Clientes);
    }
    else
        Clientes->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de proveedores") );
    frmProveedores * Proveedores = new frmProveedores(this);
    if(Proveedores->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _mantenModules.append(Proveedores);
    }
    else
        Proveedores->deleteLater();

    ArchivosGeneralesExt* e = new ArchivosGeneralesExt(this);
    if(!e->Extensions().isEmpty())
        _mantenExtensions.append(e);
    else
        e->deleteLater();
}

void MainWindow::loadAlmacenModules(QSplashScreen* splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de articulos") );
    FrmArticulos* Articulos = new FrmArticulos(this);
    if(Articulos->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _almacenModules.append(Articulos);
    }
    else
        Articulos->deleteLater();

    DivisionAlmacenExt* x = new DivisionAlmacenExt(this);
    if(!x->Extensions().isEmpty())
        _almacenExtensions.append(x);
    else
        x->deleteLater();
}


void MainWindow::loadVentasModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de presupuestos") );

    FrmPresupuestosCli* Presupcli = new FrmPresupuestosCli(this);
    if(Presupcli->userHaveAcces(Configuracion_global->id_usuario_activo))
        _ventasModules.append(Presupcli);
    else
        Presupcli->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de pedidos") );

    FrmPedidos* Pedidos = new FrmPedidos(this);
    if(Pedidos->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(Pedidos);
    }
    else
        Pedidos->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de albaranes") );

    FrmAlbaran* Albaran = new FrmAlbaran(this);
    if(Albaran->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(Albaran);
    }
    else
        Albaran->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de facturación de albaranes") );

    FrmFacturarAlabaranes* frmFactura_multiple = new FrmFacturarAlabaranes(this);
    if(frmFactura_multiple->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(frmFactura_multiple);
    }
    else
        frmFactura_multiple->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de facturas") );

    frmFacturas* Facturas = new frmFacturas(this);
    if(Facturas->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(Facturas);
    }
    else
        Facturas->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de Ventas: gestión de cobros") );

    frmGestionCobros* frmgestcobros = new frmGestionCobros(this);
    if(frmgestcobros->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(frmgestcobros);
    }
    else
        frmgestcobros->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de TPV") );

    FrmCajaMinuta * frmCajaMinuta = new FrmCajaMinuta(this);
    if(frmCajaMinuta->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _ventasModules.append(frmCajaMinuta);
    }
    else
        frmCajaMinuta->deleteLater();
}

void MainWindow::loadComprasModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de Compras: Orden de Pedido") );

    FrmOrden_Pedido_Producto * frmOrden_Ped_pro = new FrmOrden_Pedido_Producto(this);
    if(frmOrden_Ped_pro->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _comprasModules.append(frmOrden_Ped_pro);
    }
    else
        frmOrden_Ped_pro->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de Compras: pedidos") );

    FrmPedidosProveedor* FrmPedidos_pro = new FrmPedidosProveedor(this);
    if(FrmPedidos_pro->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _comprasModules.append(FrmPedidos_pro);
    }
    else
        FrmPedidos_pro->deleteLater();


    splash->showMessage(tr("Cargando modulos... Modulo de Compras: Recepción de Pedidos") );
    Frmrecepcion_pedidos* frmRecep_pedidos = new Frmrecepcion_pedidos(this);
    if(frmRecep_pedidos->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _comprasModules.append(frmRecep_pedidos);
    }
    else
        frmRecep_pedidos->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de Compras: albaranes") );
    FrmAlbaranProveedor* FrmAlbaran_pro = new FrmAlbaranProveedor(this);
    if(FrmAlbaran_pro->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _comprasModules.append(FrmAlbaran_pro);
    }
    else
        FrmAlbaran_pro->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de Compras: facturas") );
    FrmFacturasProveedor* frmFacturas_pro = new FrmFacturasProveedor(this);
    if(frmFacturas_pro->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _comprasModules.append(frmFacturas_pro);
    }
    else
        frmFacturas_pro->deleteLater();
}

void MainWindow::loadUtilsModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de Utilidades: Agenda") );
    AgendaForm* agendaForm = new AgendaForm(this);
    if(agendaForm->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _utilsModules.append(agendaForm);
    }
    else
        agendaForm->deleteLater();

    splash->showMessage(tr("Cargando modulos... Editor de reportes") );
    ReportModule* reportWindow = new ReportModule(this);
    if(reportWindow->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _utilsModules.append(reportWindow);
    }
    else
        reportWindow->deleteLater();
}

void MainWindow::loadAminModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de Administracion: Configuración general") );
    frmConfigmaya* c = new frmConfigmaya(this);
    if(c->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _adminModules.append(c);
    }
    else
        c->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de Administracion: Configurar empresa") );
    FrmEmpresas* e = new FrmEmpresas(this);
    if(e->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _adminModules.append(e);
    }
    else
        e->deleteLater();

    splash->showMessage(tr("Cargando modulos... Modulo de Administracion: Usuarios") );
    FrmUsuarios* u = new FrmUsuarios(this);
    if(u->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _adminModules.append(u);
    }
    else
        u->deleteLater();
}

void MainWindow::loadContaModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Modulo de Contabilidad: Cuadro de cuentas") );
    frmCuadro_cuentas* f = new frmCuadro_cuentas(this);
    if(f->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _contaModules.append(f);
    }
    else
        f->deleteLater();


    splash->showMessage(tr("Cargando modulos... Modulo de Contabilidad: Diario de apuntes") );
    FrmEntrada_apuntes * frmentrada_apuntes = new FrmEntrada_apuntes(this);
    if(frmentrada_apuntes->userHaveAcces(Configuracion_global->id_usuario_activo))
    {
        _contaModules.append(frmentrada_apuntes);
    }
    else
        frmentrada_apuntes->deleteLater();

}

void MainWindow::loadSecMedModules(QSplashScreen *splash)
{
    splash->showMessage(tr("Cargando modulos... Extension de Clinica") );
    ClinicaExt* e = new ClinicaExt(this);
    if(!e->Extensions().isEmpty())
        _clinicaExtensions.append(e);
    else
        e->deleteLater();
}

void MainWindow::updateDivisas()
{
    if(Configuracion_global->actualizar_divisas)
    {
        connect(Configuracion_global,SIGNAL(cambioReady(float,QString)),this,SLOT(actualizar_divisas(float,QString)));
        QSqlQuery divisas(QSqlDatabase::database("Maya"));
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_edit = false;

    QPixmap pixmap(":/Icons/PNG/mayafondo_blanco.png");
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

    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
    ui->txtcCategoria->setText(settings.value("cCategoria").toString());

    updateDivisas();
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
    this->setWindowTitle("Maya"+tr("Software para los profesionales de la salud.            (Empresa activa:")+empresa+
                         tr("  -  Ejercicio activo: ")+Configuracion_global->cEjercicio+")");
  //  ui->lineEmpresaActiva->setText(empresa);
    ui->lineUsuarioActivo->setText(user);
    Configuracion_global->cUsuarioActivo = user;
}

void MainWindow::actualizar_divisas(float valor_divisa, QString divisaDest)
{
    QSqlQuery valor(QSqlDatabase::database("Maya"));
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
    ToolBarButton * t = qobject_cast<ToolBarButton *>(sender());
    if(t)
        ui->stackedWidget->setCurrentWidget(t->linkedWidget());

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
    ui->modulesStack->setEnabled(!state);
    ui->comboBox->setEnabled(!state);
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

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->modulesStack->setCurrentIndex(index);
}

void MainWindow::handle_permisosAgenda()
{
    PermisosAgendaForm form(this);
    form.exec();
}

//void MainWindow::btn_cuentas_clicked()
//{
//    ui->stackedWidget->setCurrentWidget(frmcuentas);
//}

