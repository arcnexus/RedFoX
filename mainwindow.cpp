#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QErrorMessage>
#include<QSqlError>
#include "frmClientes.h"
#include "frmfacturas.h"
#include "frmarticulos.h"
#include "login.h"
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QFormLayout>
#include <QDebug>
#include <QSettings>
#include "configuracion.h"
#include <QSqlQuery>
#include "frmempresas.h"
#include "frmproveedores.h"
#include "frmconfiguracion.h"
#include "frmalbaran.h"
#include "frmpedidos.h"
#include "frmpresupuestoscli.h"
#include "frmcajaminuta.h"
#include "frmagendavisitas.h"
#include <QStyleFactory>
#include <QToolBar>

#include <QtCore/QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ----------------------------------------------------------------------------
    // Barra de herramientas Modulos
    // ----------------------------------------------------------------------------
    m_modulesBar = new QToolBar(tr("Modulos"), this);
    m_modulesBar->setAllowedAreas(Qt::LeftToolBarArea);
    m_modulesBar->setIconSize(QSize(80, 48));
    m_modulesBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_modulesBar->setMovable(false);
    m_modulesBar->setFloatable(false);
    m_modulesBar->setStyleSheet("background-color:rgb(219, 228, 255)");

   this->addToolBar(Qt::LeftToolBarArea, m_modulesBar);

    // MODULO MANTENIMIENTO.
    QToolButton *BtnMantenimiento = new QToolButton(m_modulesBar);
    BtnMantenimiento->setText(tr("Manten."));
    BtnMantenimiento->setIcon(QIcon(":Icons/PNG/Maintenance.png"));
    BtnMantenimiento->setToolTip(tr("Módulo de mantenimientos"));
    BtnMantenimiento->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // MODULO VENTAS/ENTRADAS.
    QToolButton *BtnEntradas = new QToolButton(m_modulesBar);
    BtnEntradas->setText(tr("Entradas"));
    BtnEntradas->setIcon(QIcon(":Icons/PNG/ventas2.png"));
    BtnEntradas->setToolTip(tr("Módulo de ventas/entradas clinica"));
    BtnEntradas->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // MODULO GASTOS/SALIDAS.
    QToolButton *BtnSalidas = new QToolButton(m_modulesBar);
    BtnSalidas->setText(tr("Salidas"));
    BtnSalidas->setIcon(QIcon(":Icons/PNG/Gastos.png"));
    BtnSalidas->setToolTip(tr("Módulo de Gastos/Salidas clinica"));
    BtnSalidas->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // INF. MEDICA.
    QToolButton *BtnMedica = new QToolButton(m_modulesBar);
    BtnMedica->setText(tr("Clínica"));
    BtnMedica->setIcon(QIcon(":Icons/PNG/PatientFile2.png"));
    BtnMedica->setToolTip(tr("Módulo de Gestión especificamente clínica"));
    BtnMedica->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // Salir
    QToolButton *BtnCerrar = new QToolButton(m_modulesBar);
    BtnCerrar->setText(tr("Salir"));
    BtnCerrar->setIcon(QIcon(":Icons/PNG/Exit.png"));
    BtnCerrar->setToolTip(tr("Salir de Terra"));
    BtnCerrar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_modulesBar->addWidget(BtnMantenimiento);
    m_modulesBar->addWidget(BtnEntradas);
    m_modulesBar->addWidget(BtnSalidas);
    m_modulesBar->addWidget(BtnMedica);
    m_modulesBar->addWidget(BtnCerrar);
    //-------------------------------
    // Cargo Barras Herramientas
    //-------------------------------
    Mantenimientos();
    Ventas();
    btnMantenimientos_clicked();


 //   m_modulesBarMapper = new QSignalMapper(this);
 //   connect(m_modulesBarMapper, SIGNAL(mapped(QWidget*)), this->centralWidget(),
   //         SLOT(setCurrentWidget(QWidget*)));

    // ----------------------------------------------------------------------------
    // Conexiones
    // ----------------------------------------------------------------------------
    connect(ui->btnSalir_2,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->btnSalir,SIGNAL(triggered()),this,SLOT(close()));
    connect(BtnMantenimiento,SIGNAL(clicked()),this,SLOT(btnMantenimientos_clicked()));
    connect(BtnEntradas,SIGNAL(clicked()),this,SLOT(btnVentas_clicked()));
    connect(BtnCerrar,SIGNAL(clicked()),this,SLOT(close()));



    QSqlDatabase dbEmp;
    m_config = new Configuracion();
    QSettings settings("infint", "terra");
    // Cambiar por parametros fichero configuración.
    m_config->cDriverBDTerra = settings.value("cDriverBDTerra").toString();
    m_config->cRutaBdTerra = settings.value("cRutaDBTerra").toString();
   // m_config->cRutaBdTerra = "C:/Users/Marco/Documents/Visual Studio 2010/Projects/terra-master/DB/terra.sqlite";/*settings.value("cRutaDBTerra").toString();*/
    m_config->cHostBDTerra = settings.value("cHostBDTerra").toString();
    m_config->cUsuarioBDTerra  =   settings.value("cUserBDTerra").toString();
    m_config->cPasswordBDTerra = settings.value("cPasswordBDTerra").toString();
    m_config->cPais = settings.value("cPais").toString();
    m_config->cEjercicio = settings.value("cEjercicioActivo").toString();
    m_config->nDigitosFactura = settings.value(("nDigitosFactura")).toInt();


    // Abro Base de Datos
    QSqlDatabase dbTerra  = QSqlDatabase::addDatabase(m_config->cDriverBDTerra,"terra");
    //dbTerra.addDatabase(m_config->cDriverBDTerra,"terra");

    if (m_config->cDriverBDTerra == "QSQLITE")
	{
        dbTerra.setDatabaseName(m_config->cRutaBdTerra);
    }
	else
	{
        dbTerra.setDatabaseName(m_config->cNombreBDTerra);
        dbTerra.setHostName(m_config->cHostBDTerra);
        dbTerra.open(m_config->cUsuarioBDTerra,m_config->cPasswordBDTerra);
    }

    if (m_config->cDriverBDTerra == "QSQLITE")
	{
        dbTerra.open();
    }
	else
	{
        dbTerra.open(m_config->cUsuarioBDTerra,m_config->cPasswordBDTerra);
    }

    if (dbTerra.lastError().isValid())
    {
        QMessageBox::critical(0, "error:", dbTerra.lastError().text());
    }

    // Preparo espacio de trabajo para poder acojer ventanas dentro de él
    workspace = new QMdiArea(ui->widget);
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(workspace, 0, 0);
    ui->widget->setLayout(gridLayout);
    
	// Identificación de usuario
    QTimer::singleShot(0,this,SLOT(init()));
}
void MainWindow::init()
{
    //Login *dlg = new Login(m_config);
    //NOTE - Fixed: puntero no borrado
    QScopedPointer<Login>dlg(new Login(m_config));
	if ( dlg->exec()==QDialog::Accepted) 
	{
		// capturo usuario
		QString usuario = dlg->getUsuario();
		ui->lineUsuarioActivo->setText(usuario);
		m_config->cUsuarioActivo = ui->lineUsuarioActivo->text();
		QSettings settings("infint", "terra");
		ui->txtnNivel->setText(QString::number( settings.value("nNivelAcceso").toInt()));
		ui->txtcCategoria->setText(settings.value("cCategoria").toString());
		// Oculto controles según categoría
		//TODO - Ocultar controles

		// capturo empresa
		QString Empresa = dlg->getEmpresa();
		ui->lineEmpresaActiva->setText(Empresa);

		// Configuramos valores empresa activa
		QSqlQuery QryEmpresa(QSqlDatabase::database("terra"));
		QryEmpresa.prepare("Select * from empresas where nombre = :nombre");
		QryEmpresa.bindValue(":nombre",Empresa.trimmed());
		if (QryEmpresa.exec()) 
		{
			QryEmpresa.next();
			QSqlRecord record = QryEmpresa.record();
			// DBEMpresa
			m_config->cDriverBDEmpresa = record.field("driverBD").value().toString();
			m_config->cHostBDEmpresa = record.field("host").value().toString();
			m_config->cNombreBDEmpresa =record.field("nombreBD").value().toString();
			m_config->cPasswordBDEmpresa =record.field("contrasena").value().toString();
			m_config->cRutaBdEmpresa = record.field("RutaBDSqLite").value().toString();
			m_config->cUsuarioBDEmpresa = record.field("user").value().toString();
			//DBMedica
			m_config->cDriverBDMedica = record.field("driverBDMedica").value().toString();
			m_config->cHostBDMedica = record.field("hostBDMedica").value().toString();
			m_config->cNombreBDMedica =record.field("nombreBDMedica").value().toString();
			m_config->cPasswordBDMedica =record.field("contrasenaBDMedica").value().toString();
			m_config->cRutaBdMedica = record.field("RutaBDMedicaSqLite").value().toString();
			m_config->cUsuarioBDMedica = record.field("userBDMedica").value().toString();
			// Varios
			m_config->cSerie = record.field("serie").value().toString();
			m_config->nDigitosCuentasContables = record.field("ndigitoscuenta").value().toInt();
			m_config->cCuentaAcreedores = record.field("codigocuentaacreedores").value().toString();
			m_config->cCuentaClientes = record.field("codigocuentaclientes").value().toString();
			m_config->cCuentaProveedores = record.field("codigocuentaproveedores").value().toString();
			// Guardo preferencias
			QSettings settings("infint", "terra");
			settings.setValue("cSerie",m_config->cSerie);
			settings.setValue("nDigitosCuentas",m_config->nDigitosCuentasContables);
			settings.setValue("cCuentaClientes",m_config->cCuentaClientes);
			settings.setValue("cCuentaProveedores",m_config->cCuentaProveedores);
			settings.setValue("cCuentaAcreedores",m_config->cCuentaAcreedores);
			settings.setValue("Clave1",record.field("clave1").value().toString());
			settings.setValue("Clave2",record.field("clave2").value().toString());

			// Abro empresa activa
			QSqlDatabase dbEmpresa = QSqlDatabase::addDatabase(m_config->cDriverBDEmpresa,"empresa");
			if (m_config->cDriverBDEmpresa =="QSQLITE") {
				dbEmpresa.setDatabaseName(m_config->cRutaBdEmpresa);
				// qDebug() << "Empresa:" << m_config->cRutaBdEmpresa;
				dbEmpresa.open();
			} else {
				dbEmpresa.setDatabaseName(m_config->cNombreBDEmpresa);
				dbEmpresa.setHostName(m_config->cHostBDEmpresa);
				dbEmpresa.open(m_config->cUsuarioBDEmpresa,m_config->cPasswordBDEmpresa);

			}
			// Abro bdmedica activa
			QSqlDatabase dbMedica = QSqlDatabase::addDatabase(m_config->cDriverBDEmpresa,"dbmedica");
			if (m_config->cDriverBDMedica =="QSQLITE") {
				dbMedica.setDatabaseName(m_config->cRutaBdMedica);
				// qDebug() << "Medica:" << m_config->cRutaBdMedica;
				if(!dbMedica.open())
					QMessageBox::warning(NULL,tr("ERROR DB"),tr("No se ha podido abrir la BD medica"),
					tr("Aceptar"));
			} else {
				dbMedica.setDatabaseName(m_config->cNombreBDMedica);
				dbMedica.setHostName(m_config->cHostBDMedica);
				dbMedica.open(m_config->cUsuarioBDMedica,m_config->cPasswordBDMedica);

			}
			if (dbMedica.lastError().isValid())
			{
				QMessageBox::critical(0, "error:", dbMedica.lastError().text());
			}
			this->show();
		} 
		else
			qDebug() <<"Fallo la conexión al fichero Medico";
	} 
	else
        qApp->quit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnMantenimientos_clicked()
{
    m_MantenimientosBar->show();
    m_VentasBar->hide();
  //  m_ComprasBar->hide();
  //  m_AdminBar->hide();
}

void MainWindow::btnVentas_clicked()
{
    m_MantenimientosBar->hide();
    m_VentasBar->show();
  //  m_ComprasBar->hide();
  //  m_AdminBar->hide();
}

void MainWindow::Mantenimientos()
{

    // ----------------------------------------------------------------------------
    // Barra de herramientas Mantenimientos
    // ----------------------------------------------------------------------------
    m_MantenimientosBar = new QToolBar(tr("Mantenimiento"), this);
    m_MantenimientosBar->setAllowedAreas(Qt::TopToolBarArea);
    m_MantenimientosBar->setIconSize(QSize(32,32));
    m_MantenimientosBar->setStyleSheet("background-color:rgb(255, 235, 167)");
    //m_MantenimientosBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_MantenimientosBar->setMovable(false);
    m_MantenimientosBar->setFloatable(false);

   this->addToolBar(Qt::TopToolBarArea, m_MantenimientosBar);

    // PACIENTES
    QToolButton *BtnClientes = new QToolButton(m_MantenimientosBar);
    BtnClientes->setText("Pacientes");
    BtnClientes->setIcon(QIcon(":Icons/PNG/clientes.png"));
    BtnClientes->setToolTip("Gestión de Clientes/Pacientes");
    BtnClientes->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // PROVEEDORES
    QToolButton *BtnProveedores = new QToolButton(m_MantenimientosBar);
    BtnProveedores->setText("Proveedores");
    BtnProveedores->setIcon(QIcon(":Icons/PNG/proveedores.png"));
    BtnProveedores->setToolTip("Gestión de Proveedores");
    BtnProveedores->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // ARTICULOS
    QToolButton *BtnArticulos = new QToolButton(m_MantenimientosBar);
    BtnArticulos->setText("Articulos");
    BtnArticulos->setIcon(QIcon(":Icons/PNG/articulos.png"));
    BtnArticulos->setToolTip("Gestión de Articulos");
    BtnArticulos->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);


    // AGENDA
    QToolButton *BtnAgenda = new QToolButton(m_MantenimientosBar);
    BtnAgenda->setText("Agenda");
    BtnAgenda->setIcon(QIcon(":Icons/PNG/Calendar2.png"));
    BtnAgenda->setToolTip("Gestión de Agenda");
    BtnAgenda->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_MantenimientosBar->addWidget(BtnClientes);
    m_MantenimientosBar->addWidget(BtnProveedores);
    m_MantenimientosBar->addWidget(BtnArticulos);
    m_MantenimientosBar->addSeparator();
    m_MantenimientosBar->addWidget(BtnAgenda);



    //--------------------------
    // Conexiones
    //--------------------------
    connect(BtnClientes,SIGNAL(clicked()),this,SLOT(btnClientes_clicked()));
    connect(BtnArticulos,SIGNAL(clicked()),this,SLOT(btnArticulos_clicked()));
    connect(BtnProveedores,SIGNAL(clicked()),this,SLOT(btnProveedores_clicked()));

    //connect(ui->btnClientes,SIGNAL(triggered()),this,SLOT(btnClientes_clicked()));
    connect(ui->btnArt_culos,SIGNAL(triggered()),this,SLOT(btnArticulos_clicked()));
    connect(ui->btnProveedores,SIGNAL(triggered()),this,SLOT(btnProveedores_clicked()));
}


void MainWindow::Ventas()
{

    // ----------------------------------------------------------------------------
    // Barra de herramientas Ventas
    // ----------------------------------------------------------------------------
    m_VentasBar = new QToolBar(tr("Ventas"), this);
    m_VentasBar->setAllowedAreas(Qt::TopToolBarArea);
    m_VentasBar->setIconSize(QSize(32,32));
    m_VentasBar->setStyleSheet("background-color:rgb(255, 235, 167)");
    //m_VentasBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_VentasBar->setMovable(false);
    m_VentasBar->setFloatable(false);

   this->addToolBar(Qt::TopToolBarArea, m_VentasBar);

    // PRESUPUESTOS
    QToolButton *BtnPresupuestos_cli = new QToolButton(m_VentasBar);
    BtnPresupuestos_cli->setText("Presup.");
    BtnPresupuestos_cli->setIcon(QIcon(":Icons/PNG/presupuestos.png"));
    BtnPresupuestos_cli->setToolTip("Gestión de Presupuestos a Clientes/Pacientes");
    BtnPresupuestos_cli->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // PEDIDOS
    QToolButton *BtnPedidos_cli = new QToolButton(m_VentasBar);
    BtnPedidos_cli->setText("Pedidos");
    BtnPedidos_cli->setIcon(QIcon(":Icons/PNG/pedidos_cli.png"));
    BtnPedidos_cli->setToolTip("Pedido de Paciente");
    BtnPedidos_cli->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // ALBARANES
    QToolButton *BtnAlbaranes_cli = new QToolButton(m_VentasBar);
    BtnAlbaranes_cli->setText("Albaranes");
    BtnAlbaranes_cli->setIcon(QIcon(":Icons/PNG/albaran.png"));
    BtnAlbaranes_cli->setToolTip("Gestión de Alabaranes a pacientes");
    BtnAlbaranes_cli->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

   // FACTURA
    QToolButton *BtnFactura_cli = new QToolButton(m_VentasBar);
    BtnFactura_cli->setText("Facturas");
    BtnFactura_cli->setIcon(QIcon(":Icons/PNG/Factura.png"));
    BtnFactura_cli->setToolTip("Gestión de Facturas");
    BtnFactura_cli->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // CAJA
     QToolButton *BtnCaja = new QToolButton(m_VentasBar);
     BtnCaja->setText("Caja día");
     BtnCaja->setIcon(QIcon(":Icons/PNG/tpv.png"));
     BtnCaja->setToolTip("Gestión de Caja día");
     BtnCaja->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_VentasBar->addWidget(BtnPresupuestos_cli);
    m_VentasBar->addWidget(BtnPedidos_cli);
    m_VentasBar->addWidget(BtnAlbaranes_cli);
    m_VentasBar->addWidget(BtnFactura_cli);
    m_VentasBar->addWidget(BtnCaja);



    //--------------------------
    // Conexiones
    //--------------------------
   connect(BtnPresupuestos_cli,SIGNAL(clicked()),this,SLOT(btnPresup_clientes_clicked()));
   connect(BtnPedidos_cli,SIGNAL(clicked()),this,SLOT(btn_Pedido_cliente_clicked()));
   connect(BtnAlbaranes_cli,SIGNAL(clicked()),this,SLOT(btnAlbaran_clientes_clicked()));
   connect(BtnFactura_cli,SIGNAL(clicked()),this,SLOT(btnFacturaCliente_clicked()));
   connect(BtnCaja,SIGNAL(clicked()),this,SLOT(btnCajaMinuta_clicked()));

   connect(ui->actionPresupuestos,SIGNAL(triggered()),this,SLOT(btnPresup_clientes_clicked()));
   connect(ui->actionPedidos,SIGNAL(triggered()),this,SLOT(btn_Pedido_cliente_clicked()));
   connect(ui->actionAlbaranes,SIGNAL(triggered()),this,SLOT(btnAlbaran_clientes_clicked()));
   connect(ui->actionFacturas,SIGNAL(triggered()),this,SLOT(btnFacturaCliente_clicked()));
   connect(ui->actionVentas_Contado,SIGNAL(triggered()),this,SLOT(btnCajaMinuta_clicked()));
}


void MainWindow::btnClientes_clicked()
{
    ui->btnClientes->setEnabled(false);
    frmClientes *frmClientes1 = new frmClientes(m_config,this);
    workspace->addSubWindow(frmClientes1);
    frmClientes1->setWindowState(Qt::WindowMaximized);
    frmClientes1->exec();
    cerrarSubWindows();
    ui->btnClientes->setEnabled(true);
}
void MainWindow::btnFacturaCliente_clicked()
{
 //   ui->btnFacturaCliente->setEnabled(false);
    frmFacturas *frmFacturas1 = new frmFacturas(m_config);
    workspace->addSubWindow(frmFacturas1);
    frmFacturas1->setWindowState(Qt::WindowMaximized);
    frmFacturas1->exec();
    cerrarSubWindows();
   // ui->btnFacturaCliente->setEnabled(true);
}

void MainWindow::btnArticulos_clicked()
{
   // ui->btnArticulos->setEnabled(false);
    FrmArticulos *frmArticulos1 = new FrmArticulos(m_config);
    workspace->addSubWindow(frmArticulos1);
    frmArticulos1->setWindowState(Qt::WindowMaximized);
    frmArticulos1->exec();
    cerrarSubWindows();
   // ui->btnArticulos->setEnabled(true);
}

//void MainWindow::on_botEmpresas_clicked()
//{
//    //ui->botEmpresas->setEnabled(false);
//    FrmEmpresas *frmEmpresa1 = new FrmEmpresas();
//    workspace->addSubWindow(frmEmpresa1);
//    frmEmpresa1->setWindowState(Qt::WindowMaximized);
//    frmEmpresa1->exec();
//    cerrarSubWindows();
//    //ui->botEmpresas->setEnabled(true);
//}

void MainWindow::btnProveedores_clicked()
{
    ui->btnProveedores->setEnabled(false);
    frmProveedores *frmProveedores1 = new frmProveedores();
    workspace->addSubWindow(frmProveedores1);
    frmProveedores1->setWindowState(Qt::WindowMaximized);
    frmProveedores1->exec();
    cerrarSubWindows();
    ui->btnProveedores->setEnabled(true);

}

//void MainWindow::on_botConfiguracion_clicked()
//{
//   // ui->botConfiguracion->setEnabled(false);
//    FrmConfiguracion *frmConfiguracion1 = new FrmConfiguracion();
//    workspace->addSubWindow(frmConfiguracion1);
//    frmConfiguracion1->setWindowState(Qt::WindowMaximized);
//    frmConfiguracion1->exec();
//    cerrarSubWindows();
//   // ui->botConfiguracion->setEnabled(true);
//}

void MainWindow::btnAlbaran_clientes_clicked()
{
    //->setEnabled(false);
    FrmAlbaran *frmAlbaran1 = new FrmAlbaran(this);
    workspace->addSubWindow(frmAlbaran1);
    frmAlbaran1->setWindowState(Qt::WindowMaximized);
    frmAlbaran1->exec();
    cerrarSubWindows();
  //  ui->btnAlbaran_clientes->setEnabled(true);
}

void MainWindow::btn_Pedido_cliente_clicked()
{
    //ui->btn_Pedido_cliente->setEnabled(false);
    FrmPedidos *frmPedidos1 = new FrmPedidos();
    workspace->addSubWindow(frmPedidos1);
    frmPedidos1->setWindowState(Qt::WindowMaximized);
    frmPedidos1->exec();
    cerrarSubWindows();
    //ui->btn_Pedido_cliente->setEnabled(true);
}

void MainWindow::btnPresup_clientes_clicked()
{
    //ui->btnPresup_clientes->setEnabled(false);
    FrmPresupuestosCli *frmPresupcli = new FrmPresupuestosCli();
    workspace->addSubWindow(frmPresupcli);
    frmPresupcli->setWindowState(Qt::WindowMaximized);
    frmPresupcli->exec();
    cerrarSubWindows();
   // ui->btnPresup_clientes->setEnabled(true);
}

void MainWindow::btnCajaMinuta_clicked()
{
    FrmCajaMinuta *frmCajaMinuta = new FrmCajaMinuta();
    workspace->addSubWindow(frmCajaMinuta);
    frmCajaMinuta->setWindowState(Qt::WindowMaximized);
    frmCajaMinuta->exec();
    cerrarSubWindows();
   // ui->btnCajaMinuta->setEnabled(true);
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

void MainWindow::cambiarEstilo(int estado)
{
//    QString style;

//    if (estado ==2)
//        style = "GTK+";
//    else
//        style = "fusion";

//    QApplication::setStyle(style);
}

void MainWindow::cerrarSubWindows()
{
    QList<QMdiSubWindow *> wnd = workspace->subWindowList();

    QList<QMdiSubWindow *>::iterator i;

    for (i=wnd.begin();i!= wnd.end();++i) {
        (*i)->hide();
        //NOTE - Fixed Libera la memoria tmb
        (*i)->deleteLater();
    }
}






