#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include "conexion.h"
#include <QtSql>
#include <QErrorMessage>
#include<QSqlError>
#include <frmClientes.h>
#include <frmfacturas.h>
#include <login.h>
#include <QWorkspace>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Relleno combo empresas
    ui->lineEmpresaActiva->addItem("Pruebas");
    ui->lineEmpresaActiva->addItem("Informatica - Intelligent");
    ui->tabWidget->setStyleSheet("background-color: rgb(255, 227, 171);;border-bottom-color: #000000");
    workspace = new QWorkspace(ui->widget);

    QGridLayout *gridLayout = new QGridLayout;
     gridLayout->addWidget(workspace, 0, 0);
     ui->widget->setLayout(gridLayout);

     Login dlg;
     if ( dlg.exec()==QDialog::Accepted) {
         QString usuario = dlg.getUsuario();
         ui->lineUsuarioActivo->setText(usuario);
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
    frmClientes *frmClientes1 = new frmClientes();
    workspace->addWindow(frmClientes1);
    frmClientes1->setWindowState(Qt::WindowMaximized);
    frmClientes1->setVisible(true);
}
void MainWindow::on_btnFacturaCliente_clicked()
{
    frmFacturas *frmFacturas1 = new frmFacturas();
    workspace->addWindow(frmFacturas1);
    frmFacturas1->setWindowState(Qt::WindowMaximized);
    frmFacturas1->setVisible(true);
}
