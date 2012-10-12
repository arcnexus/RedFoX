#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QString>
#include <QSqlDatabase>
#include <QtSql>
#include "configuracion.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString usuario;
    QString empresa;
    //QSqlDatabase dbConfiguracion;
    QSqlQuery QryConfiguracion;
    Configuracion *m_config;

    
private slots:

    void on_btnCerrarSesion_clicked();

    void on_btnClientes_clicked();

    void on_btnFacturaCliente_clicked();




    void on_btnArticulos_clicked();

    void on_botEmpresas_clicked();

    void on_btnProveedores_clicked();

    void on_botConfiguracion_clicked();

    void on_btnAlbaran_clientes_clicked();

    void on_btn_Pedido_cliente_clicked();

    void on_btnPresup_clientes_clicked();

    void on_btnCajaMinuta_clicked();

    void on_btnAgenda_clicked();

private:
    Ui::MainWindow *ui;
    QWorkspace *workspace;
};

#endif // MAINWINDOW_H
