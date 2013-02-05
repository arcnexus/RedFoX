#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Auxiliares/Globlal_Include.h"

//
#include "Zona_Pacientes/frmClientes.h"
#include "frmfacturas.h"
#include "frmarticulos.h"
#include "frmproveedores.h"
#include "frmalbaran.h"
#include "frmpedidos.h"
#include "frmpresupuestoscli.h"
#include "TPV/frmcajaminuta.h"
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

private slots:
	void init();

    void block_main();

    void unblock_main();

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

 //4   void on_btnAgenda_clicked();

    void cambiarEstilo(int);

    void on_btn_bloquear_clicked();

    void divisiones_almacen();

    void handle_doctores();
    void handle_bancos();
    void handle_tiposIVA();
    void handle_fomasPago();
    void handle_paises();
    void hande_avisos();
    void handle_tiposImagen();
    void handle_tipoAnalitica();
    void handle_campoAnalitica();
    void handle_motivoInterConsulta();
private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *e);
    void blockMe(bool state);
    void Mantenimientos();
    void Ventas();

    //TOOLBARS
    QToolBar *m_modulesBar;
    QToolBar *m_MantenimientosBar;
    QToolBar *m_VentasBar;
    QToolBar *m_ComprasBar;
    QToolBar *m_AdminBar;
    //WIDGETS
    frmClientes *frmClientes1;
    frmFacturas *frmFacturas1;
    FrmArticulos *frmArticulos1;
    frmProveedores *frmProveedores1;
    FrmAlbaran *frmAlbaran1;
    FrmPedidos *frmPedidos1;
    FrmPresupuestosCli *frmPresupcli;
    FrmCajaMinuta *frmCajaMinuta;
    QString user;
    QString pass;
    bool on_edit;
};

#endif // MAINWINDOW_H
