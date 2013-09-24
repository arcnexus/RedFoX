#ifndef FRMPEDidOSPROVEEDOR_H
#define FRMPEDidOSPROVEEDOR_H

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
    module_zone module_zone(){return Compras;}
    QString module_name(){return "Pedidos provedor";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    
    QPushButton* wantShortCut(bool& ok){ok = true; return shortCut;}
signals:


private slots:
    void lineaReady(lineaDetalle*);
    void lineaDeleted(lineaDetalle *ld);
    void totalChanged(double base , double dto ,double subtotal , double iva, double re, double total, QString moneda);
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

    void on_txtBuscar_textEdited(const QString &arg1);

    void on_tabla_doubleClicked(const QModelIndex &index);

    void on_tabla_clicked(const QModelIndex &index);

    void on_btnBuscar_clicked();

    void on_cboModo_currentIndexChanged(const QString &arg1);

    void on_cboOrden_currentIndexChanged(const QString &arg1);

    void on_btnLimpiar_clicked();

private:

    void formatotabla();
    void filter_table();
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
    QAction menuButton;
    QPushButton* shortCut;
    QSqlQueryModel *model;
};

#endif // FRMPEDidOSPROVEEDOR_H
