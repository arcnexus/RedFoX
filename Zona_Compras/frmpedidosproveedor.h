#ifndef FRMPEDidOSPROVEEDOR_H
#define FRMPEDidOSPROVEEDOR_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"


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
    void llenarProveedor(int id);
    PedidoProveedor *oPedido_proveedor;// = new PedidoProveedor(this);
    int id;
    module_zone module_zone(){return Compras;}
    QString module_name(){return "Pedidos provedor";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    
    QPushButton* wantShortCut(bool& ok){ok = true; return shortCut;}
    void init_querys();

public slots:
    void init();
signals:


protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void bloquearcampos(bool state);
    void buscar_proveeedor();
    void anadir_pedido();
    void guardar_pedido();
    void editar_pedido();
    void deshacer();
    void borrar_pedido();
    void llenar_campos();
    void refrescar_modelo();
    void guardar_campos_en_objeto();
    void clear();

    void on_btnImprimir_clicked();

    void on_tabla_doubleClicked(const QModelIndex &index);

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

    void on_spinPorc_dto_pp_valueChanged(double arg1);

    void on_chkrecargo_equivalencia_toggled(bool checked);

    void on_btnPrev_clicked();

    void on_btnNext_clicked();

private:

    void formatotabla();

    BarraBusqueda* m_busqueda;
    void setUpBusqueda();

    Ui::FrmPedidosProveedor *ui;
    QAction * aAlbaran_action;
    QAction * aFactura_action;
    QMenu * convertir_menu;
    Proveedor prov;
    QString moneda;
    QAction menuButton;
    QPushButton* shortCut;
    QSqlQueryModel *model_busqueda;
    QSqlQueryModel *modelLineas;

    QSqlQueryModel *modelgastos;
    QSqlQueryModel *modelEntregas;
};

#endif // FRMPEDidOSPROVEEDOR_H
