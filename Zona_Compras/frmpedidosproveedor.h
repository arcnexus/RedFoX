#ifndef FRMPEDIDOSPROVEEDOR_H
#define FRMPEDIDOSPROVEEDOR_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../Auxiliares/table_helper.h"

#include "proveedor.h"
#include "pedidoproveedor.h"
#include "../mayamodule.h"
namespace Ui {
class FrmPedidosProveedor;
}

class FrmPedidosProveedor : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmPedidosProveedor(QWidget *parent = 0, bool showCerrar = false);
    ~FrmPedidosProveedor();
    // = new Proveedor(this);
    void llenarProveedor(int id,bool isNew = false);
    PedidoProveedor *oPedido_proveedor;// = new PedidoProveedor(this);
    int id;
    moduleZone ModuleZone(){return Compras;}
    QString ModuleName(){return "Pedidos provedor";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}QPushButton* wantShortCut(bool& ok){ok = false; return 0;}
signals:


private slots:
    void lineaReady(lineaDetalle*);
    void lineaDeleted(lineaDetalle *ld);
    void totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);

    void bloquearcampos(bool estado);
    void buscar_proveeedor();
    void anadir_pedido();
    void guardar_pedido();
    void editar_pedido();
    void deshacer();
    void siguiente();
    void anterior();
    void imprimir();
    void borrar_pedido();
    void llenar_campos();
    void guardar_campos_en_objeto();
    void clear();
    void resizeTable(int x);
    void on_btnAnadirEntregas_clicked();
    void cargar_tabla_entregas();
    void on_btnImprimir_clicked();

private:


    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

private:
    PedidoProveedor pedido;

    Ui::FrmPedidosProveedor *ui;
    Table_Helper helper;
    QAction * aAlbaran_action;
    QAction * aFactura_action;
    QMenu * convertir_menu;
    Proveedor prov;
    QString moneda;
    ToolBarButton toolButton;
    QAction menuButton;
};

#endif // FRMPEDIDOSPROVEEDOR_H
