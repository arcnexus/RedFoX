#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QErrorMessage>
#include<QSqlError>
#include "frmClientes.h"
#include "frmfacturas.h"
#include "frmarticulos.h"
#include <login.h>
#include <QWorkspace>
#include <QFormLayout>
#include <QDebug>
#include <QSettings>
#include "configuracion.h"
#include <QSqlQuery>
#include "frmempresas.h"
#include "frmproveedores.h"
#include "frmconfiguracion.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSqlDatabase dbEmp;
    m_config = new Configuracion();
            QSettings settings("infint", "terra");
    // Cambiar por parametros fichero configuración.
    m_config->cDriverBDTerra = settings.value("cDriverBDTerra").toString();
    m_config->cRutaBdTerra = settings.value("cRutaDBTerra").toString();
    m_config->cHostBDTerra = settings.value("cHostBDTerra").toString();
    m_config->cUsuarioBDTerra  =   settings.value("cUserBDTerra").toString();
    m_config->cPasswordBDTerra = settings.value("cPasswordBDTerra").toString();
    m_config->cPais = settings.value("cPais").toString();
    m_config->cEjercicio = settings.value("cEjercicioActivo").toString();
    m_config->nDigitosFactura = settings.value(("nDigitosFactura")).toInt();

    // Abro Base de Datos
    QSqlDatabase dbTerra  = QSqlDatabase::addDatabase(m_config->cDriverBDTerra,"terra");
    //dbTerra.addDatabase(m_config->cDriverBDTerra,"terra");

    if (m_config->cDriverBDTerra == "QSQLITE"){
        dbTerra.setDatabaseName(m_config->cRutaBdTerra);
      } else {
        dbTerra.setDatabaseName(m_config->cNombreBDTerra);
        dbTerra.setHostName(m_config->cHostBDTerra);
        dbTerra.open(m_config->cUsuarioBDTerra,m_config->cPasswordBDTerra);

    }
    if (m_config->cDriverBDTerra == "QSQLITE") {

        dbTerra.open();
    } else {
        dbTerra.open(m_config->cUsuarioBDTerra,m_config->cPasswordBDTerra);

    }

    if (dbTerra.lastError().isValid())
        {
            QMessageBox::critical(0, "error:", dbTerra.lastError().text());

        }


    // Fin TODO


    ui->tabWidget->setStyleSheet("background-color: rgb(255, 227, 171);;border-bottom-color: #000000");

    // Preparo espacio de trabajo para poder acojer ventanas dentro de él
    workspace = new QWorkspace(ui->widget);
    QGridLayout *gridLayout = new QGridLayout;
     gridLayout->addWidget(workspace, 0, 0);
     ui->widget->setLayout(gridLayout);
    // Identificación de usuario

     Login *dlg = new Login(m_config);
     if ( dlg->exec()==QDialog::Accepted) {
         // capturo usuario
         QString usuario = dlg->getUsuario();
         ui->lineUsuarioActivo->setText(usuario);
         m_config->cUsuarioActivo = ui->lineUsuarioActivo->text();
        // capturo empresa
         QString Empresa = dlg->getEmpresa();
         ui->lineEmpresaActiva->setText(Empresa);
         // Configuramos valores empresa activa
         QSqlQuery QryEmpresa(QSqlDatabase::database("terra"));
         QryEmpresa.prepare("Select * from empresas where nombre = :nombre");
         QryEmpresa.bindValue(":nombre",Empresa.trimmed());
         if (QryEmpresa.exec()) {
                QryEmpresa.next();
                QSqlRecord record = QryEmpresa.record();
               m_config->cDriverBDEmpresa = record.field("driverBD").value().toString();
               m_config->cHostBDEmpresa = record.field("host").value().toString();
               m_config->cNombreBDEmpresa =record.field("nombreBD").value().toString();
               m_config->cPasswordBDEmpresa =record.field("contrasena").value().toString();
               m_config->cRutaBdEmpresa = record.field("RutaBDSqLite").value().toString();
               m_config->cUsuarioBDEmpresa = record.field("user").value().toString();
               m_config->cSerie = record.field("serie").value().toString();

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
              if (dbEmpresa.lastError().isValid())
                  {
                      QMessageBox::critical(0, "error:", dbEmpresa.lastError().text());

                  }
         } else
             qDebug() <<"Fallo la conexión al fichero de empresas";


     } else
         exit(0);
     }


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnCerrarSesion_clicked()
{
    ui->lineUsuarioActivo->setText("");
    ui->lineEmpresaActiva->setVisible(false);
}
void MainWindow::on_btnClientes_clicked()
{
    frmClientes *frmClientes1 = new frmClientes(m_config);
    workspace->addWindow(frmClientes1);
    frmClientes1->setWindowState(Qt::WindowMaximized);
    frmClientes1->setVisible(true);
}
void MainWindow::on_btnFacturaCliente_clicked()
{
    frmFacturas *frmFacturas1 = new frmFacturas(m_config);
    workspace->addWindow(frmFacturas1);
    frmFacturas1->setWindowState(Qt::WindowMaximized);
    frmFacturas1->setVisible(true);
}



void MainWindow::on_btnArticulos_clicked()
{
    ui->btnArticulos->setEnabled(false);
    FrmArticulos *frmArticulos1 = new FrmArticulos(m_config);
    workspace->addWindow(frmArticulos1);
    //frmArticulos1->showNormal();
    frmArticulos1->setWindowState(Qt::WindowMaximized);
    frmArticulos1->exec();
    ui->btnArticulos->setEnabled(true);
}


void MainWindow::on_botEmpresas_clicked()
{
    ui->botEmpresas->setEnabled(false);
    FrmEmpresas *frmEmpresa1 = new FrmEmpresas();
    workspace->addWindow(frmEmpresa1);
    frmEmpresa1->setWindowState(Qt::WindowMaximized);
    frmEmpresa1->exec();
    ui->botEmpresas->setEnabled(true);
}

void MainWindow::on_btnProveedores_clicked()
{
    ui->btnProveedores->setEnabled(false);
    frmProveedores *frmProveedores1 = new frmProveedores();
    workspace->addWindow(frmProveedores1);
    //frmArticulos1->showNormal();
    frmProveedores1->setWindowState(Qt::WindowMaximized);
    frmProveedores1->exec();
    ui->btnProveedores->setEnabled(true);

}

void MainWindow::on_botConfiguracion_clicked()
{
    ui->botConfiguracion->setEnabled(false);
    FrmConfiguracion *frmConfiguracion1 = new FrmConfiguracion();
    workspace->addWindow(frmConfiguracion1);
    frmConfiguracion1->setWindowState(Qt::WindowMaximized);
    frmConfiguracion1->exec();
    ui->botConfiguracion->setEnabled(true);
}
