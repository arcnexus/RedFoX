#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QString>
#include <QSqlDatabase>
#include <QtSql>
#include <QMdiArea>
#include "configuracion.h"
#include <QToolButton>


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
    void btnMantenimientos_clicked();
    void btnVentas_clicked();

    void btnClientes_clicked();

    void btnFacturaCliente_clicked();

    void btnArticulos_clicked();

   // void on_botEmpresas_clicked();

    void btnProveedores_clicked();

  //  void on_botConfiguracion_clicked();

    void btnAlbaran_clientes_clicked();

    void btn_Pedido_cliente_clicked();

    void btnPresup_clientes_clicked();

    void btnCajaMinuta_clicked();

    void GestEmpresas();


 //4   void on_btnAgenda_clicked();

    void cambiarEstilo(int);

private:
    Ui::MainWindow *ui;
    QMdiArea *workspace;
    void cerrarSubWindows();
    void Mantenimientos();
    void Ventas();
    QToolBar *m_modulesBar;
    QToolBar *m_MantenimientosBar;
    QToolBar *m_VentasBar;
    QToolBar *m_ComprasBar;
    QToolBar *m_AdminBar;

    //QSignalMapper *m_modulesBarMapper;
};

#endif // MAINWINDOW_H
