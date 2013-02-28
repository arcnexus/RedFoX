#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "Zona_Administrador/frmempresas.h"
#include "Zona_Compras/frmpedidosproveedor.h"
//#include "frmconfiguracion.h"
#include "block_terra_form.h"
#include "db_table_view.h"
#include "Agenda/permisosagendaform.h"
#include <QSplashScreen>
#include <Almacen/frmtipostarifa.h>

Configuracion * Configuracion_global = 0;


bool medic = true;
bool internacional = true;

void MainWindow::crear_barraMantenimiento()
{
    btn_clientes = new ToolBarButton(tr("Clientes"),":/Icons/PNG/clientes_2.png",this);
    btn_proovedores = new ToolBarButton(tr("Proveedores"),":/Icons/PNG/proveedores_2.png",this);
    btn_almacen = new ToolBarButton(tr("Almacen"),":/Icons/PNG/Box.png",this);
    btn_agenda = new ToolBarButton(tr("Agenda"),":/Icons/PNG/Calender.png",this);

    QFrame*  line = new QFrame(ui->page_manten);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    ui->verticalLayout_manten->addWidget(btn_clientes);
    ui->verticalLayout_manten->addWidget(btn_proovedores);
    ui->verticalLayout_manten->addWidget(btn_almacen);
    ui->verticalLayout_manten->addWidget(line);
    ui->verticalLayout_manten->addWidget(btn_agenda);
    ui->verticalLayout_manten->addSpacerItem(new QSpacerItem(20, 87, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(btn_clientes,SIGNAL(clicked()),this,SLOT(btnClientes_clicked()));
    connect(btn_almacen,SIGNAL(clicked()),this,SLOT(btnArticulos_clicked()));
    connect(btn_proovedores,SIGNAL(clicked()),this,SLOT(btnProveedores_clicked()));
    connect(btn_agenda,SIGNAL(clicked()),this,SLOT(showAgenda()));

    //Barra de menu
    connect(ui->btnClientes,SIGNAL(triggered()),this,SLOT(btnClientes_clicked()));
    connect(ui->btnArt_culos,SIGNAL(triggered()),this,SLOT(btnArticulos_clicked()));
    connect(ui->btnProveedores,SIGNAL(triggered()),this,SLOT(btnProveedores_clicked()));
    connect(ui->btnAgenda,SIGNAL(triggered()),this,SLOT(showAgenda()));
    connect(ui->actionPermisos_de_Agenda,SIGNAL(triggered()),this,SLOT(handle_permisosAgenda()));
}

void MainWindow::crear_barraVentas()
{
    btn_presupuestos = new ToolBarButton(tr("Presupuestos"),":/Icons/PNG/presupuestos.png",this);
    btn_pedidos = new ToolBarButton(tr("Pedidos"),":/Icons/PNG/pedidos_cli.png",this);
    btn_albaranes = new ToolBarButton(tr("Albaranes"),":/Icons/PNG/albaran.png",this);
    btn_factura_mult = new ToolBarButton(tr("Fact. \nalbaranes"),":Icons/PNG/factmult.png",this);
    btn_facturas = new ToolBarButton(tr("Facturas"),":/Icons/PNG/Factura.png",this);
    btn_tpv = new ToolBarButton(tr("TPV"),":/Icons/PNG/tpv.png",this);
    btn_gestionCobros = new ToolBarButton(tr("Gest. Cobros"),":/Icons/PNG/Cobros.png",this);

    QFrame*  line = new QFrame(ui->page_ventas);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    ui->verticalLayout_ventas->addWidget(btn_presupuestos);
    ui->verticalLayout_ventas->addWidget(btn_pedidos);
    ui->verticalLayout_ventas->addWidget(btn_albaranes);
    ui->verticalLayout_ventas->addWidget(btn_factura_mult);
    ui->verticalLayout_ventas->addWidget(btn_facturas);
    ui->verticalLayout_ventas->addWidget(line);
    ui->verticalLayout_ventas->addWidget(btn_tpv);
    ui->verticalLayout_ventas->addWidget(line);
    ui->verticalLayout_ventas->addWidget(btn_gestionCobros);
    ui->verticalLayout_ventas->addSpacerItem(new QSpacerItem(20, 87, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(btn_presupuestos,SIGNAL(clicked()),this,SLOT(btnPresup_clientes_clicked()));
    connect(btn_pedidos,SIGNAL(clicked()),this,SLOT(btn_Pedido_cliente_clicked()));
    connect(btn_albaranes,SIGNAL(clicked()),this,SLOT(btnAlbaran_clientes_clicked()));
    connect(btn_facturas,SIGNAL(clicked()),this,SLOT(btnFacturaCliente_clicked()));
    connect(btn_factura_mult,SIGNAL(clicked()),this,SLOT(btnFactura_multiple_clicked()));
    connect(btn_tpv,SIGNAL(clicked()),this,SLOT(btnCajaMinuta_clicked()));

    //barra de menu
    connect(ui->actionPresupuestos,SIGNAL(triggered()),this,SLOT(btnPresup_clientes_clicked()));
    connect(ui->actionPedidos,SIGNAL(triggered()),this,SLOT(btn_Pedido_cliente_clicked()));
    connect(ui->actionAlbaranes_2,SIGNAL(triggered()),this,SLOT(btnAlbaran_clientes_clicked()));
    connect(ui->actionFacturas,SIGNAL(triggered()),this,SLOT(btnFacturaCliente_clicked()));
    connect(ui->actionVentas_Contado,SIGNAL(triggered()),this,SLOT(btnCajaMinuta_clicked()));
}

void MainWindow::crear_barraCompras()
{
    btn_pedidos_pro= new ToolBarButton(tr("Pedidos \nProv."),":/Icons/PNG/pedido_pro.png",this);
    btn_albaranes_pro = new ToolBarButton(tr("Albaranes \nProv."),":/Icons/PNG/albaran_pro.png",this);
    btn_facturas_pro = new ToolBarButton(tr("Facturas \nProv."),":/Icons/PNG/Factura_pro.png",this);
    btn_Orden_pedido = new ToolBarButton(tr("Orden \nde pedido"),":/Icons/PNG/opedido.png",this);


    QFrame*  line = new QFrame(ui->page_compras);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    ui->verticalLayout_compras->addWidget(btn_Orden_pedido);
    ui->verticalLayout_compras->addWidget(btn_pedidos_pro);
    ui->verticalLayout_compras->addWidget(btn_albaranes_pro);
    ui->verticalLayout_compras->addWidget(btn_facturas_pro);


    ui->verticalLayout_compras->addSpacerItem(new QSpacerItem(20, 87, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(btn_Orden_pedido,SIGNAL(clicked()),this,SLOT(btnOrden_pedido_clicked()));
    connect(btn_pedidos_pro,SIGNAL(clicked()),this,SLOT(btn_pedidos_pro_clicked()));
    connect(btn_albaranes_pro,SIGNAL(clicked()),this,SLOT(btn_albaranes_pro_clicked()));
    connect(btn_facturas_pro,SIGNAL(clicked()),this,SLOT(btn_facturas_pro_clicked()));

    //barra de menu
    connect(ui->actionOrdenes_de_pedidos,SIGNAL(triggered()),this,SLOT(btnOrden_pedido_clicked()));
    connect(ui->actionPresupuestos,SIGNAL(triggered()),this,SLOT(btnPresup_clientes_clicked()));
    connect(ui->actionPedidos,SIGNAL(triggered()),this,SLOT(btn_Pedido_cliente_clicked()));
    connect(ui->actionAlbaranes_2,SIGNAL(triggered()),this,SLOT(btnAlbaran_clientes_clicked()));
    connect(ui->actionFacturas,SIGNAL(triggered()),this,SLOT(btnFacturaCliente_clicked()));
    connect(ui->actionVentas_Contado,SIGNAL(triggered()),this,SLOT(btnCajaMinuta_clicked()));
}

void MainWindow::crear_barraAlmacen()
{
    btn_articulos_2 = new ToolBarButton(tr("Artículos "),":/Icons/PNG/Box.png",this);

    QFrame*  line = new QFrame(ui->page_almacen);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    ui->verticalLayout_almacen->addWidget(btn_articulos_2);

    ui->verticalLayout_almacen->addSpacerItem(new QSpacerItem(20, 87, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(btn_articulos_2,SIGNAL(clicked()),this,SLOT(btnArticulos_2_clicked()));

    //barra de menu
    connect(ui->actionPresupuestos,SIGNAL(triggered()),this,SLOT(btnPresup_clientes_clicked()));
    connect(ui->actionPedidos,SIGNAL(triggered()),this,SLOT(btn_Pedido_cliente_clicked()));
    connect(ui->actionAlbaranes_2,SIGNAL(triggered()),this,SLOT(btnAlbaran_clientes_clicked()));
    connect(ui->actionFacturas,SIGNAL(triggered()),this,SLOT(btnFacturaCliente_clicked()));
        connect(ui->actionVentas_Contado,SIGNAL(triggered()),this,SLOT(btnCajaMinuta_clicked()));
}

void MainWindow::crear_barraAdmin()
{
    btn_reports = new ToolBarButton(tr("Editar \nreportes "),":/Icons/PNG/reports.png",this);

    QFrame*  line = new QFrame(ui->page_admin);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    ui->verticalLayout_admin->addWidget(btn_reports);

    ui->verticalLayout_admin->addSpacerItem(new QSpacerItem(20, 87, QSizePolicy::Minimum, QSizePolicy::Expanding));

    connect(btn_reports,SIGNAL(clicked()),this,SLOT(btn_reports_clicked()));

    //barra de menu
//    connect(ui->actionPresupuestos,SIGNAL(triggered()),this,SLOT(btnPresup_clientes_clicked()));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_edit = false;
    Configuracion_global->CargarDatos();
    QStringList modulos;
    modulos << "Mantenimiento"  << "Compras" << "Ventas" << "Almacen " << "Administrador";
    ui->comboBox->addItems(modulos);
    crear_barraMantenimiento();

    crear_barraVentas();

    crear_barraCompras();

    crear_barraAlmacen();

    crear_barraAdmin();

    {

    ui->stackedWidget_2->setCurrentIndex(0);
    if (medic)
    {
        ui->btnClientes->setText(tr("Pacientes"));
        btn_clientes->setText(tr("Pacientes"));
    }
    else
    {
        ui->btnClientes->setText(tr("Clientes"));
        ui->menuClinica->deleteLater();
    }

    // ----------------------------------------------------------------------------
    // Conexiones
    // ----------------------------------------------------------------------------
    connect(ui->btnSalir,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->btn_salir,SIGNAL(clicked()),this,SLOT(close()));

    connect(ui->actionGestion_de_Secciones,SIGNAL(triggered()),this,SLOT(divisiones_almacen()));
    connect(ui->actionGestion_de_Familias,SIGNAL(triggered()),this,SLOT(divisiones_almacen()));
    connect(ui->actionGestion_de_subfamilias,SIGNAL(triggered()),this,SLOT(divisiones_almacen()));
    connect(ui->actionSubSubFamilias,SIGNAL(triggered()),this,SLOT(divisiones_almacen()));
    connect(ui->actionGrupos,SIGNAL(triggered()),this,SLOT(divisiones_almacen()));

    connect(ui->actionDoctores,SIGNAL(triggered()),this,SLOT(handle_doctores()));
    connect(ui->actionBancos,SIGNAL(triggered()),this,SLOT(handle_bancos()));
    connect(ui->btnTipos_de_Iva,SIGNAL(triggered()),this,SLOT(handle_tiposIVA()));
    connect(ui->btnFormas_de_Pago,SIGNAL(triggered()),this,SLOT(handle_fomasPago()));
    connect(ui->actionPaises,SIGNAL(triggered()),this,SLOT(handle_paises()));
    connect(ui->actionAvisos,SIGNAL(triggered()),this,SLOT(hande_avisos()));
    connect(ui->actionTipos_de_imagen,SIGNAL(triggered()),this,SLOT(handle_tiposImagen()));
    connect(ui->actionTipos_de_anal_tica,SIGNAL(triggered()),this,SLOT(handle_tipoAnalitica()));
    connect(ui->actionTarifas,SIGNAL(triggered()), this,SLOT(tipostarifa()));
    connect(ui->actionCampos_de_analitica,SIGNAL(triggered()),this,SLOT(handle_campoAnalitica()));
    connect(ui->actionMotivos_de_interconsulta,SIGNAL(triggered()),this,SLOT(handle_motivoInterConsulta()));
    connect(ui->actionMonedas,SIGNAL(triggered()),this,SLOT(handle_monedas()));

    QTimer::singleShot(0,this,SLOT(init()));
}
}
void MainWindow::init()
{
   // ui->toolBox->setStyleSheet("QToolBox::tab {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);	border-radius: 5px;color: darkgray;}QToolBox::tab:selected { /* italicize selected tabs */font: italic;color: black;}");
    //ui->toolBox->setStyleSheet("QToolBox::tab {background:#304163; 	border-radius: 5px;color: darkgray;}QToolBox::tab:selected { /* italicize selected tabs */font: italic;color: #FFFFFF;}");
        ui->lineUsuarioActivo->setText(user);
        Configuracion_global->cUsuarioActivo = user;

		QSettings settings(qApp->applicationDirPath()+"/TerraConfig.ini", QSettings::IniFormat);
		ui->txtnNivel->setText(QString::number( settings.value("nNivelAcceso").toInt()));
		ui->txtcCategoria->setText(settings.value("cCategoria").toString());

        ui->lineEmpresaActiva->setText(empresa);

		// Configuramos valores empresa activa
		QSqlQuery QryEmpresa(QSqlDatabase::database("terra"));
		QryEmpresa.prepare("Select * from empresas where nombre = :nombre");
        QryEmpresa.bindValue(":nombre",empresa.trimmed());
		if (QryEmpresa.exec()) 
        {
			QryEmpresa.next();

            QPixmap pixmap(":/Icons/PNG/Logo2.png");
            QSplashScreen splash(pixmap);
            splash.show();

            QApplication::processEvents();
			QSqlRecord record = QryEmpresa.record();

			// DBEMpresa
            splash.showMessage(tr("Cargando configuración de base de datos"),Qt::AlignBottom);
            Configuracion_global->cDriverBDEmpresa = record.field("driverBD").value().toString();
            Configuracion_global->cHostBDEmpresa = record.field("host").value().toString();
            Configuracion_global->cNombreBDEmpresa =record.field("nombreBD").value().toString();
            Configuracion_global->cPasswordBDEmpresa =record.field("contrasena").value().toString();
            Configuracion_global->cRutaBdEmpresa = record.field("RutaBDSqLite").value().toString();
            Configuracion_global->cUsuarioBDEmpresa = record.field("user").value().toString();

            if(record.field("medica").value().toInt()==1)
                medic = true;
            else
                medic = false;

            if(record.field("internacional").value().toInt()==1)
                internacional = true;
            else
                internacional = false;


            QApplication::processEvents();

			//DBMedica
            splash.showMessage(tr("Cargando configuración médica"),Qt::AlignBottom);
            Configuracion_global->cDriverBDMedica = record.field("driverBDMedica").value().toString();
            Configuracion_global->cHostBDMedica = record.field("hostBDMedica").value().toString();
            Configuracion_global->cNombreBDMedica =record.field("nombreBDMedica").value().toString();
            Configuracion_global->cPasswordBDMedica =record.field("contrasenaBDMedica").value().toString();
            Configuracion_global->cRutaBdMedica = record.field("RutaBDMedicaSqLite").value().toString();
            Configuracion_global->cUsuarioBDMedica = record.field("userBDMedica").value().toString();

            QApplication::processEvents();

			// Varios
            splash.showMessage(tr("Cargando configuración financiera"),Qt::AlignBottom);
            Configuracion_global->cSerie = record.field("serie").value().toString();
            qDebug() << "desde db mainwindow" << record./*field("ndigitoscuenta").*/value("ndigitoscuenta").toInt();
            Configuracion_global->nDigitosCuentasContables = record./*field("ndigitoscuenta").*/value("ndigitoscuenta").toInt();
            Configuracion_global->cCuentaAcreedores = record.field("codigocuentaacreedores").value().toString();
            Configuracion_global->cCuentaClientes = record.field("codigocuentaclientes").value().toString();
            Configuracion_global->cCuentaProveedores = record.field("codigocuentaproveedores").value().toString();

            QApplication::processEvents();

			// Guardo preferencias
			QSettings settings(qApp->applicationDirPath()+"/TerraConfig.ini", QSettings::IniFormat);
            settings.setValue("cSerie",Configuracion_global->cSerie);
            settings.setValue("ndigitoscuenta",Configuracion_global->nDigitosCuentasContables);
            settings.setValue("cCuentaClientes",Configuracion_global->cCuentaClientes);
            settings.setValue("cCuentaProveedores",Configuracion_global->cCuentaProveedores);
            settings.setValue("cCuentaAcreedores",Configuracion_global->cCuentaAcreedores);
            settings.setValue("Clave1",record.field("clave1").value().toString());
            settings.setValue("Clave2",record.field("clave2").value().toString());

            QApplication::processEvents();

			// Abro empresa activa
            splash.showMessage(tr("Cargando datos de la empresa activa"),Qt::AlignBottom);
            QSqlDatabase dbEmpresa = QSqlDatabase::addDatabase(Configuracion_global->cDriverBDEmpresa,"empresa");
            if (Configuracion_global->cDriverBDEmpresa =="QSQLITE")
            {
                dbEmpresa.setDatabaseName(Configuracion_global->cRutaBdEmpresa);
				dbEmpresa.open();
            }
            else
            {
                dbEmpresa.setDatabaseName(Configuracion_global->cNombreBDEmpresa);
                dbEmpresa.setHostName(Configuracion_global->cHostBDEmpresa);
                dbEmpresa.open(Configuracion_global->cUsuarioBDEmpresa,Configuracion_global->cPasswordBDEmpresa);
			}

            QApplication::processEvents();

			// Abro bdmedica activa
            splash.showMessage(tr("Abriendo base de datos médica"),Qt::AlignBottom);
            QSqlDatabase dbMedica = QSqlDatabase::addDatabase(Configuracion_global->cDriverBDEmpresa,"dbmedica");
            if (Configuracion_global->cDriverBDMedica =="QSQLITE")
            {
                dbMedica.setDatabaseName(Configuracion_global->cRutaBdMedica);
                qDebug() << "Medica:" << Configuracion_global->cRutaBdMedica;
				if(!dbMedica.open())
					QMessageBox::warning(qApp->activeWindow(),tr("ERROR DB"),tr("No se ha podido abrir la BD medica"),
					tr("Aceptar"));
            }
            else
            {
                dbMedica.setDatabaseName(Configuracion_global->cNombreBDMedica);
                dbMedica.setHostName(Configuracion_global->cHostBDMedica);
                dbMedica.open(Configuracion_global->cUsuarioBDMedica,Configuracion_global->cPasswordBDMedica);
			}
			if (dbMedica.lastError().isValid())
			{
                QMessageBox::critical(this, "error:", dbMedica.lastError().text());
			}

            Configuracion_global->Cargar_iva();

            Configuracion_global->Cargar_paises();
            Configuracion_global->CargarClientes();
            Configuracion_global->CargarUsuarios();
            //Widgets

            splash.showMessage(tr("Cargando modulos... Modulo de clientes"),Qt::AlignBottom);
            frmClientes1 = new frmClientes(this);
            connect(frmClientes1,SIGNAL(block()),this,SLOT(block_main()));
            connect(frmClientes1,SIGNAL(unblock()),this,SLOT(unblock_main()));

            splash.showMessage(tr("Cargando modulos... Modulo de facturas"),Qt::AlignBottom);
            frmFacturas1 = new frmFacturas(this);
            connect(frmFacturas1,SIGNAL(block()),this,SLOT(block_main()));
            connect(frmFacturas1,SIGNAL(unblock()),this,SLOT(unblock_main()));

            splash.showMessage(tr("Cargando modulos... Modulo de articulos"),Qt::AlignBottom);
            frmArticulos1 = new FrmArticulos(this);

            splash.showMessage(tr("Cargando modulos... Modulo de proveedores"),Qt::AlignBottom);
            frmProveedores1 = new frmProveedores(this);

            splash.showMessage(tr("Cargando modulos... Modulo de albaranes"),Qt::AlignBottom);
            frmAlbaran1 = new FrmAlbaran(this);
            connect(frmAlbaran1,SIGNAL(block()),this,SLOT(block_main()));
            connect(frmAlbaran1,SIGNAL(unblock()),this,SLOT(unblock_main()));


            splash.showMessage(tr("Cargando modulos... Modulo de facturación de albaranes"),Qt::AlignBottom);
            frmFactura_multiple = new FrmFacturarAlabaranes(this);
            connect(frmFactura_multiple,SIGNAL(block()),this,SLOT(block_main()));
            connect(frmFactura_multiple,SIGNAL(unblock()),this,SLOT(unblock_main()));

            splash.showMessage(tr("Cargando modulos... Modulo de pedidos"),Qt::AlignBottom);
            frmPedidos1 = new FrmPedidos(this);
            connect(frmPedidos1,SIGNAL(block()),this,SLOT(block_main()));
            connect(frmPedidos1,SIGNAL(unblock()),this,SLOT(unblock_main()));

            splash.showMessage(tr("Cargando modulos... Modulo de presupuestos"),Qt::AlignBottom);
            frmPresupcli = new FrmPresupuestosCli(this);
            connect(frmPresupcli,SIGNAL(block()),this,SLOT(block_main()));
            connect(frmPresupcli,SIGNAL(unblock()),this,SLOT(unblock_main()));

            splash.showMessage(tr("Cargando modulos... Modulo de TPV"),Qt::AlignBottom);
            frmCajaMinuta = new FrmCajaMinuta(this);
            connect(frmCajaMinuta,SIGNAL(block()),this,SLOT(block_main()));
            connect(frmCajaMinuta,SIGNAL(unblock()),this,SLOT(unblock_main()));

            splash.showMessage(tr("Cargando modulos... Modulo de Compras: pedidos"),Qt::AlignBottom);
            FrmPedidos_pro = new FrmPedidosProveedor(this);

            splash.showMessage(tr("Cargando modulos... Modulo de Compras: albaranes"),Qt::AlignBottom);
            FrmAlbaran_pro = new FrmAlbaranProveedor(this);


            splash.showMessage(tr("Cargando modulos... Modulo de Compras: facturas"),Qt::AlignBottom);
            frmFacturas_pro = new FrmFacturasProveedor(this);

            splash.showMessage(tr("Cargando modulos... Modulo de Compras: Orden de Pedido"),Qt::AlignBottom);
            frmOrden_Ped_pro = new FrmOrden_Pedido_Producto(this);

            splash.showMessage(tr("Cargando modulos... Editor de reportes"),Qt::AlignBottom);
            reportWindow = new ReportWriterWindow();

            splash.showMessage(tr("Cargando modulos... Modulo de Compras: Orden de Pedido"),Qt::AlignBottom);
            agendaForm = new AgendaForm(this);

            splash.showMessage(tr("Integrando modulos"),Qt::AlignBottom);

            ui->stackedWidget->addWidget(frmClientes1);
            ui->stackedWidget->addWidget(frmFacturas1);
            ui->stackedWidget->addWidget(frmArticulos1);
            ui->stackedWidget->addWidget(frmProveedores1);
            ui->stackedWidget->addWidget(frmAlbaran1);
            ui->stackedWidget->addWidget(frmPedidos1);
            ui->stackedWidget->addWidget(frmPresupcli);
            ui->stackedWidget->addWidget(frmCajaMinuta);
            ui->stackedWidget->addWidget(FrmPedidos_pro);
            ui->stackedWidget->addWidget(FrmAlbaran_pro);
            ui->stackedWidget->addWidget(frmFacturas_pro);
            ui->stackedWidget->addWidget(frmFactura_multiple);
            ui->stackedWidget->addWidget(frmOrden_Ped_pro);
            ui->stackedWidget->addWidget(reportWindow);
          ui->stackedWidget->addWidget(agendaForm);
            TerraForm = new init_form(this);
            ui->stackedWidget->addWidget(TerraForm);
            ui->stackedWidget->setCurrentWidget(TerraForm);

            QApplication::processEvents();
            this->setWindowState(Qt::WindowMaximized);
            this->move(0,0);
            this->showMaximized();
		} 
		else
            QMessageBox::critical(this,"Error","Fallo la conexión al fichero Medico");
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
    delete reportWindow;
}

void MainWindow::btnMantenimientos_clicked()
{
    //ui->manten_ToolBar->show();
    //ui->ventas_toolBar->hide();
}

void MainWindow::btnVentas_clicked()
{
   // ui->manten_ToolBar->hide();
   // ui->ventas_toolBar->show();
}

void MainWindow::btnClientes_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmClientes1);
}
void MainWindow::btnFacturaCliente_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmFacturas1);
}

void MainWindow::btnArticulos_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmArticulos1);
}

void MainWindow::btnProveedores_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmProveedores1);
}

void MainWindow::btnAlbaran_clientes_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmAlbaran1);
}

void MainWindow::btnFactura_multiple_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmFactura_multiple);
}

void MainWindow::btn_Pedido_cliente_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmPedidos1);
}

void MainWindow::btnPresup_clientes_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmPresupcli);
}

void MainWindow::btnCajaMinuta_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmCajaMinuta);
    frmCajaMinuta->setFocus();
}

void MainWindow::btn_pedidos_pro_clicked()
{
    ui->stackedWidget->setCurrentWidget(FrmPedidos_pro);
}

void MainWindow::btn_albaranes_pro_clicked()
{
    ui->stackedWidget->setCurrentWidget(FrmAlbaran_pro);
}

void MainWindow::btn_facturas_pro_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmFacturas_pro);
}

void MainWindow::btnArticulos_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmArticulos1);
}

void MainWindow::btnOrden_pedido_clicked()
{
    ui->stackedWidget->setCurrentWidget(frmOrden_Ped_pro);
}

void MainWindow::btn_reports_clicked()
{
    ui->stackedWidget->setCurrentWidget(reportWindow);
}

//void MainWindow::on_btnAgenda_clicked()
//{
//    ui->btnAgenda->setEnabled(false);
//    frmAgendaVisitas *frmAgenda1 = new frmAgendaVisitas();
//    frmAgenda1->setWindowState(Qt::WindowMaximized);
//    frmAgenda1->exec();
//    cerrarSubWindows();
//    ui->btnAgenda->setEnabled(true);
//}


void MainWindow::on_btn_bloquear_clicked()
{
    block_terra_form form(this);
    form.set_user(user,pass);
    this->hide();
    form.exec();
    this->show();
}

void MainWindow::divisiones_almacen()
{
    if(sender()== ui->actionGestion_de_Secciones)
    {
        Db_table_View form(this);
        form.set_db("terra");
        form.set_table("secciones");

        form.setWindowTitle(tr("Secciones"));

        QStringList headers;
        headers << tr("Seccion");
        form.set_table_headers(headers);

        form.set_columnHide(0);
        form.set_columnHide(2);
        form.exec();
    }
    else if (sender() == ui->actionGestion_de_Familias)
    {
        Db_table_View form(this);
        form.set_db("terra");
        form.set_table("familias");

        form.setWindowTitle(tr("Familias"));

        QStringList headers;
        headers << tr("Codigo") << tr("Familia") << tr("Pertenece a");
        form.set_table_headers(headers);

        form.set_relation(3,QSqlRelation("secciones","id","cSeccion"));

        form.set_columnHide(0);
        form.exec();
    }
    else if (sender() == ui->actionGestion_de_subfamilias)
    {
        Db_table_View form(this);
        form.set_db("terra");
        form.set_table("subfamilias");

        form.setWindowTitle(tr("Subfamilias"));

        QStringList headers;
        headers << tr("SubFamilia") << tr("Pertenece a");
        form.set_table_headers(headers);
                                 //tabla a buscar, columna relacionada y lo que quiero mostar
        form.set_relation(2,QSqlRelation("familias","id","cFamilia"));

        form.set_columnHide(0);
        form.exec();
    }
    else if (sender() == ui->actionSubSubFamilias)
    {
        Db_table_View form(this);
        form.set_db("terra");
        form.set_table("subsubfamilias");

        form.setWindowTitle(tr("SubSubfamilias"));

        QStringList headers;
        headers << tr("SubSubFamilia") << tr("Pertenece a");
        form.set_table_headers(headers);

        form.set_relation(2,QSqlRelation("subfamilias","id","cSubfamilia"));

        form.set_columnHide(0);
        form.exec();
    }
    else if (sender() == ui->actionGrupos)
    {
        Db_table_View form(this);
        form.set_db("terra");
        form.set_table("grupoart");

        form.setWindowTitle(tr("Grupo de Articulos"));

        QStringList headers;
        headers << tr("Grupo") << tr("Pertenece a");
        form.set_table_headers(headers);

        form.set_relation(2,QSqlRelation("subsubfamilias","id","subsubfamilia"));

        form.set_columnHide(0);
        form.exec();
    }
}

void MainWindow::handle_doctores()
{
    Db_table_View form(this);
    form.set_db("dbmedica");
    form.set_table("doctores");

    form.setWindowTitle(tr("Doctores"));

    QStringList headers;
    headers << tr("Nombre") << tr("Nº Colegiado") << tr("Teléfono") << tr("Especialidad 1");
    headers << tr("Especialidad 2") << tr("Dirección") << tr("C.P.") << tr("Población");
    headers << tr("Provincia") << tr("Pais") << tr("Movil");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.set_columnHide(12);
    form.set_columnHide(13);
    form.set_columnHide(14);
    form.exec();
}

void MainWindow::handle_bancos()
{
    Db_table_View form(this);
    form.set_db("terra");
    form.set_table("bancos");

    form.setWindowTitle(tr("Bancos"));

    QStringList headers;
    headers << tr("Descripción") << tr("Entidad") << tr("Oficina") << tr("Dc");
    headers << tr("Cuenta") << tr("Saldo");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.exec();
}

void MainWindow::handle_tiposIVA()
{
    Db_table_View form(this);
    form.set_db("terra");
    form.set_table("tiposiva");

    form.setWindowTitle(tr("Tipos de I.V.A"));


    QStringList headers;
    headers << "" << tr("Tipo") << tr("Decripción") << tr("I.V.A") << tr("Recargo equivalencia");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.set_columnHide(1);
    form.set_noInsertDeleteRows();
    form.exec();
    Configuracion_global->Cargar_iva();
}

void MainWindow::handle_fomasPago()
{
    Db_table_View form(this);
    form.set_db("terra");
    form.set_table("FormPago");

    form.setWindowTitle(tr("Formas de pago"));

    QStringList headers;
    headers << tr("Codigo") << tr("Forma de pago") << tr("Dia de pago 1") << tr("Dia de pago 2");
    headers << tr("Dia de pago 3") << tr("Dia de pago 4") << tr("Dia 1") << tr("Dia 2") << tr("Dia 3")<< tr("Dia 4");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.exec();
}

void MainWindow::handle_paises()
{
    Db_table_View form(this);
    form.set_db("terra");
    form.set_table("paises");

    form.setWindowTitle(tr("Paises"));

    QStringList headers;
    headers << tr("Pais") << tr("Moneda");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.set_relation(2,QSqlRelation("monedas","id","moneda"));
    form.set_printFile("/home/arcnexus/TerraReports/clientes.xml");//TODO hacer directorio general de reports
    form.exec();
    Configuracion_global->Cargar_paises();
}

void MainWindow::hande_avisos()
{
    Db_table_View form(this);
    form.set_db("terra");
    form.set_table("avisos");

    form.setWindowTitle(tr("Avisos"));

    QStringList headers;
    headers << tr("Motivo") << tr("Inicio") << tr("Final") << tr("Descripcion") << tr("Estado");
    headers << tr("Tipo de aviso") << tr("Avisar a");
    form.set_table_headers(headers);

    form.set_relation(6,QSqlRelation("tiposaviso","id","tipoaviso"));
    form.set_relation(7,QSqlRelation("usuarios","id","nombre"));
    form.set_columnHide(0);
    form.exec();
}

void MainWindow::handle_tiposImagen()
{
    Db_table_View form(this);
    form.set_db("dbmedica");
    form.set_table("tiposimagen");

    form.setWindowTitle(tr("Tipo de imagenes"));

    QStringList headers;
    headers << tr("Descripcion") << tr("Comentarios");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.exec();
}

void MainWindow::handle_tipoAnalitica()
{
    Db_table_View form(this);
    form.set_db("dbmedica");
    form.set_table("tiposanalitica");

    form.setWindowTitle(tr("Tipo de Analitica"));

    QStringList headers;
    headers << tr("Tipos de Analisis");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.exec();
}

void MainWindow::handle_campoAnalitica()
{
    Db_table_View form(this);
    form.set_db("dbmedica");
    form.set_table("tiposanalitica2");

    form.setWindowTitle(tr("Campos para analitica"));

    QStringList headers;
    headers << tr("Tipo") << tr("Descripcion") << tr("Valores de referencia") << tr("Pertenece a:");
    form.set_table_headers(headers);

    form.set_relation(4,QSqlRelation("tiposanalitica","id","tipoanalisis"));
    form.set_columnHide(0);
    form.exec();
}

void MainWindow::handle_motivoInterConsulta()
{
    Db_table_View form(this);
    form.set_db("dbmedica");
    form.set_table("motivosinterconsulta");

    form.setWindowTitle(tr("Motivo de inter consulta"));

    QStringList headers;
    headers << tr("Tipo") << tr("Descripcion") << tr("Comentarios");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.exec();
}


void MainWindow::tipostarifa()
{
//    Db_table_View form(this);
//    form.set_db("terra");
//    form.set_table("codigotarifa");

//    form.setWindowTitle(tr("Tipos de tarifa"));

//    QStringList headers;
//    headers << tr("Descripción Tarifa");
//    form.set_table_headers(headers);

//    form.set_columnHide(400);
//    form.exec();
    FrmTiposTarifa tipotar(this);
    tipotar.exec();

}

void MainWindow::handle_monedas()
{
    Db_table_View form(this);
    form.set_db("terra");
    form.set_table("monedas");

    form.setWindowTitle(tr("Monedas"));

    QStringList headers;
    headers << tr("Moneda");
    form.set_table_headers(headers);

    form.set_columnHide(400);
    form.exec();
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
    ui->stackedWidget_2->setEnabled(!state);
    ui->comboBox->setEnabled(!state);
    ui->menubar->setEnabled(!state);
    on_edit = state;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->stackedWidget_2->setCurrentIndex(index);
}

void MainWindow::showAgenda()
{
    ui->stackedWidget->setCurrentWidget(agendaForm);
}

void MainWindow::handle_permisosAgenda()
{
    PermisosAgendaForm form(this);
    form.exec();
}
