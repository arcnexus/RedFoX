#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "Zona_Administrador/frmempresas.h"
#include "frmconfiguracion.h"
#include "frmagendavisitas.h"
#include "block_terra_form.h"
#include "db_table_view.h"

Configuracion * Configuracion_global = 0;
bool medic = false;
bool internacional = true;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_edit = false;

    if (medic)
        ui->btnClientes->setText(tr("Pacientes"));
    else
    {
        ui->actionClinica->deleteLater();
        ui->btnClientes->setText(tr("Clientes"));
        ui->menuClinica->deleteLater();
    }

    // ----------------------------------------------------------------------------
    // Conexiones
    // ----------------------------------------------------------------------------
    connect(ui->btnSalir,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->btn_salir,SIGNAL(clicked()),this,SLOT(close()));

    connect(ui->actionManten,SIGNAL(triggered()),this,SLOT(btnMantenimientos_clicked()));
    connect(ui->action_ventas,SIGNAL(triggered()),this,SLOT(btnVentas_clicked()));

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
    connect(ui->actionTarifas,SIGNAL(triggered()), this,SLOT(handle_tipostarifa()));
    connect(ui->actionCampos_de_analitica,SIGNAL(triggered()),this,SLOT(handle_campoAnalitica()));
    connect(ui->actionMotivos_de_interconsulta,SIGNAL(triggered()),this,SLOT(handle_motivoInterConsulta()));


    connect(ui->btnClientes,SIGNAL(triggered()),this,SLOT(btnClientes_clicked()));
    connect(ui->btnArt_culos,SIGNAL(triggered()),this,SLOT(btnArticulos_clicked()));
    connect(ui->btnProveedores,SIGNAL(triggered()),this,SLOT(btnProveedores_clicked()));
    connect(ui->btn_clientes,SIGNAL(clicked()),this,SLOT(btnClientes_clicked()));
    connect(ui->btn_almacen,SIGNAL(clicked()),this,SLOT(btnArticulos_clicked()));
    connect(ui->btn_proovedores,SIGNAL(clicked()),this,SLOT(btnProveedores_clicked()));

    connect(ui->actionPresupuestos,SIGNAL(triggered()),this,SLOT(btnPresup_clientes_clicked()));
    connect(ui->actionPedidos,SIGNAL(triggered()),this,SLOT(btn_Pedido_cliente_clicked()));
    connect(ui->actionAlbaranes,SIGNAL(triggered()),this,SLOT(btnAlbaran_clientes_clicked()));
    connect(ui->actionFacturas,SIGNAL(triggered()),this,SLOT(btnFacturaCliente_clicked()));
    connect(ui->actionVentas_Contado,SIGNAL(triggered()),this,SLOT(btnCajaMinuta_clicked()));
    connect(ui->btn_presupuesto,SIGNAL(clicked()),this,SLOT(btnPresup_clientes_clicked()));
    connect(ui->btn_pedido,SIGNAL(clicked()),this,SLOT(btn_Pedido_cliente_clicked()));
    connect(ui->btn_albaran,SIGNAL(clicked()),this,SLOT(btnAlbaran_clientes_clicked()));
    connect(ui->btn_factura,SIGNAL(clicked()),this,SLOT(btnFacturaCliente_clicked()));
    connect(ui->btn_tpv,SIGNAL(clicked()),this,SLOT(btnCajaMinuta_clicked()));

    QTimer::singleShot(0,this,SLOT(init()));
}
void MainWindow::init()
{
        ui->lineUsuarioActivo->setText(user);
        Configuracion_global->cUsuarioActivo = user;

		QSettings settings("infint", "terra");
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
            QDesktopWidget *desktop = QApplication::desktop();
            QProgressBar progress;

            progress.setWindowFlags(Qt::FramelessWindowHint);
            progress.setAlignment(Qt::AlignCenter);
            progress.resize(600 , 40);
            progress.setMaximum(15);
            progress.setVisible(true);
            progress.show();
            progress.move(desktop->width()/2 - progress.width()/2 , desktop->height()/2 - progress.height()/2);
            QApplication::processEvents();
			QSqlRecord record = QryEmpresa.record();

			// DBEMpresa
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

            progress.setValue(1);
            QApplication::processEvents();

			//DBMedica
            Configuracion_global->cDriverBDMedica = record.field("driverBDMedica").value().toString();
            Configuracion_global->cHostBDMedica = record.field("hostBDMedica").value().toString();
            Configuracion_global->cNombreBDMedica =record.field("nombreBDMedica").value().toString();
            Configuracion_global->cPasswordBDMedica =record.field("contrasenaBDMedica").value().toString();
            Configuracion_global->cRutaBdMedica = record.field("RutaBDMedicaSqLite").value().toString();
            Configuracion_global->cUsuarioBDMedica = record.field("userBDMedica").value().toString();
            progress.setValue(2);
            QApplication::processEvents();

			// Varios
            Configuracion_global->cSerie = record.field("serie").value().toString();
            Configuracion_global->nDigitosCuentasContables = record.field("ndigitoscuenta").value().toInt();
            Configuracion_global->cCuentaAcreedores = record.field("codigocuentaacreedores").value().toString();
            Configuracion_global->cCuentaClientes = record.field("codigocuentaclientes").value().toString();
            Configuracion_global->cCuentaProveedores = record.field("codigocuentaproveedores").value().toString();
            progress.setValue(3);
            QApplication::processEvents();

			// Guardo preferencias
			QSettings settings("infint", "terra");
            settings.setValue("cSerie",Configuracion_global->cSerie);
            settings.setValue("nDigitosCuentas",Configuracion_global->nDigitosCuentasContables);
            settings.setValue("cCuentaClientes",Configuracion_global->cCuentaClientes);
            settings.setValue("cCuentaProveedores",Configuracion_global->cCuentaProveedores);
            settings.setValue("cCuentaAcreedores",Configuracion_global->cCuentaAcreedores);
            settings.setValue("Clave1",record.field("clave1").value().toString());
            settings.setValue("Clave2",record.field("clave2").value().toString());
            progress.setValue(4);
            QApplication::processEvents();

			// Abro empresa activa
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
            progress.setValue(5);
            QApplication::processEvents();

			// Abro bdmedica activa
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
            progress.setValue(6);

            Configuracion_global->Cargar_iva();
            Configuracion_global->Cargar_paises();

            //Widgets
            frmClientes1 = new frmClientes(this);
            progress.setValue(7);

            frmFacturas1 = new frmFacturas(this);
            connect(frmFacturas1,SIGNAL(block()),this,SLOT(block_main()));
            connect(frmFacturas1,SIGNAL(unblock()),this,SLOT(unblock_main()));
            progress.setValue(8);

            frmArticulos1 = new FrmArticulos(this);
            progress.setValue(9);

            frmProveedores1 = new frmProveedores(this);
            progress.setValue(10);

            frmAlbaran1 = new FrmAlbaran(this);
            connect(frmAlbaran1,SIGNAL(block()),this,SLOT(block_main()));
            connect(frmAlbaran1,SIGNAL(unblock()),this,SLOT(unblock_main()));
            progress.setValue(11);

            frmPedidos1 = new FrmPedidos(this);
            connect(frmPedidos1,SIGNAL(block()),this,SLOT(block_main()));
            connect(frmPedidos1,SIGNAL(unblock()),this,SLOT(unblock_main()));
            progress.setValue(12);

            frmPresupcli = new FrmPresupuestosCli(this);
            connect(frmPresupcli,SIGNAL(block()),this,SLOT(block_main()));
            connect(frmPresupcli,SIGNAL(unblock()),this,SLOT(unblock_main()));
            progress.setValue(13);

            frmCajaMinuta = new FrmCajaMinuta(this);
            progress.setValue(14);

            ui->stackedWidget->addWidget(frmClientes1);
            ui->stackedWidget->addWidget(frmFacturas1);
            ui->stackedWidget->addWidget(frmArticulos1);
            ui->stackedWidget->addWidget(frmProveedores1);
            ui->stackedWidget->addWidget(frmAlbaran1);
            ui->stackedWidget->addWidget(frmPedidos1);
            ui->stackedWidget->addWidget(frmPresupcli);
            ui->stackedWidget->addWidget(frmCajaMinuta);
            progress.setValue(15);

            QApplication::processEvents();
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
    headers << tr("Pais");
    form.set_table_headers(headers);

    form.set_columnHide(0);
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


void MainWindow::handle_tipostarifa()
{
    Db_table_View form(this);
    form.set_db("terra");
    form.set_table("codigotarifa");

    form.setWindowTitle(tr("Tipos de tarifa"));

    QStringList headers;
    headers << tr("Descripción Tarifa");
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
    ui->toolBox->setEnabled(!state);
    ui->menubar->setEnabled(!state);
    on_edit = state;
}
