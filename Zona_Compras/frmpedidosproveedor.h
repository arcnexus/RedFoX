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


protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void lineaReady(lineaDetalle*);
    void lineaDeleted(lineaDetalle *ld);
    void totalChanged(double base , double dto ,double subtotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);

    void bloquearcampos(bool state);
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
    void refrescar_modelo();
    void guardar_campos_en_objeto();
    void clear();
    void resizeTable(int x);
    void on_btnAnadirEntregas_clicked();
    void cargar_tabla_entregas();
    void on_btnImprimir_clicked();

    void on_tabla_doubleClicked(const QModelIndex &index);

    void on_tabla_clicked(const QModelIndex &index);

    void on_btnBuscar_clicked();

    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);
    void on_btnAnadirLinea_clicked();

    void on_Lineas_doubleClicked(const QModelIndex &index);
    void calcular_pedido();

    void on_btnAnadir_costes_clicked();

    void on_SpinGastoDist1_valueChanged(double arg1);

    void on_SpinGastoDist2_valueChanged(double arg1);

    void on_SpinGastoDist3_valueChanged(double arg1);

    void on_cboporc_iva_gasto1_currentIndexChanged(int index);

    void on_cboporc_iva_gasto2_currentIndexChanged(int index);

    void on_cboporc_iva_gasto3_currentIndexChanged(int index);
    void calcular_coste_real_linea(int id_linea);
    void calcular_coste_real_todas_lineas();

    void on_btnRefrescar__clicked();

private:

    void formatotabla();

    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();



    BarraBusqueda* m_busqueda;
    void setUpBusqueda();

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
    QSqlQueryModel *modelLineas;
    QSqlQueryModel *iva;
    QSqlQueryModel *modelgastos;
};

#endif // FRMPEDidOSPROVEEDOR_H
