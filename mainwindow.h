#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Auxiliares/Globlal_Include.h"

//
#include "Zona_Pacientes/frmClientes.h"
#include "frmfacturas.h"
#include "Almacen/frmarticulos.h"
#include "frmproveedores.h"
#include "frmalbaran.h"
#include "frmpedidos.h"
#include "frmpresupuestoscli.h"
#include "TPV/frmcajaminuta.h"
#include "Auxiliares/init_form.h"
#include "Auxiliares/toolbarbutton.h"
#include "Zona_Compras/frmpedidosproveedor.h"
#include "Zona_Compras/frmalbaranproveedor.h"
#include "Zona_Compras/frmfacturasproveedor.h"
#include "Zona_Ventas/frmfacturaralabaranes.h"
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

    QString user;
    QString pass;

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

    void btnFactura_multiple_clicked();

    void btn_Pedido_cliente_clicked();

    void btnPresup_clientes_clicked();

    void btnCajaMinuta_clicked();

    // Zona Compras
    void btn_pedidos_pro_clicked();

    void btn_albaranes_pro_clicked();
    void btn_facturas_pro_clicked();
    // Zona Almacen
    void btnArticulos_2_clicked();

 //4   void on_btnAgenda_clicked();


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
    void handle_tipostarifa();
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *e);
    void blockMe(bool state);

    //Barra de mantenimiento
    ToolBarButton * btn_clientes;
    ToolBarButton * btn_almacen;
    ToolBarButton * btn_proovedores;
    ToolBarButton * btn_agenda;
    void crear_barraMantenimiento();
    //Barra de ventas
    ToolBarButton * btn_presupuestos;
    ToolBarButton * btn_pedidos;
    ToolBarButton * btn_albaranes;
    ToolBarButton * btn_factura_mult;
    ToolBarButton * btn_facturas;
    ToolBarButton * btn_tpv;
    ToolBarButton * btn_gestionCobros;

    void crear_barraVentas();
    // Barra de Compras
    ToolBarButton * btn_pedidos_pro;
    ToolBarButton * btn_albaranes_pro;
    ToolBarButton * btn_facturas_pro;
    void crear_barraCompras();

    // Barra de Almacen
    ToolBarButton * btn_recepcionPedidos;
    ToolBarButton * btn_gestionDevoluciones;
    ToolBarButton * btn_inventario;
    ToolBarButton * btn_traspasoAlmacen;
    ToolBarButton * btn_gestionPagos;
    ToolBarButton * btn_articulos_2;
    void crear_barraAlmacen();

    //WIDGETS
    frmClientes *frmClientes1;
    frmFacturas *frmFacturas1;
    FrmArticulos *frmArticulos1;
    frmProveedores *frmProveedores1;
    FrmAlbaran *frmAlbaran1;
    FrmPedidos *frmPedidos1;
    FrmPresupuestosCli *frmPresupcli;
    FrmCajaMinuta *frmCajaMinuta;
    FrmPedidosProveedor *FrmPedidos_pro;
    FrmAlbaranProveedor *FrmAlbaran_pro;
    FrmFacturasProveedor * frmFacturas_pro;
    FrmFacturarAlabaranes *frmFactura_multiple;

    init_form * TerraForm;

    bool on_edit;
};

#endif // MAINWINDOW_H
