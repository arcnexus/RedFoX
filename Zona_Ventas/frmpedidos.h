#ifndef FRMPEDidOS_H
#define FRMPEDidOS_H

#include "../Auxiliares/Globlal_Include.h"

#include "pedidos.h"
#include "../Zona_Pacientes/cliente.h"

#include "../mayamodule.h"
namespace Ui {
class frmPedidos;
}

class FrmPedidos : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmPedidos(QWidget *parent = 0);
    ~FrmPedidos();
    Pedidos *oPedido;
    Cliente *oCliente3;

    module_zone module_zone(){return Ventas;}
    QString module_name(){return "Pedidos Ventas";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
    void init_querys();
    void formatLineas();

signals:

private slots:
    void init();

    void listados();

    void on_btnSiguiente_clicked();

    void on_btnAnadir_clicked();

    void on_btnAnterior_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnDeshacer_clicked();

    void on_btn_borrar_clicked();

    void refrescar_modelo();

    void convertir_en_albaran();
    void convertir_enFactura();


    void on_txtcodigo_cliente_editingFinished();

    void on_btnImprimir_clicked();

    void on_tabla_doubleClicked(const QModelIndex &index);

    void on_btnImprimir_2_clicked();

    void cboporc_iva_gasto1_currentIndexChanged(int index);

    void cboporc_iva_gasto2_currentIndexChanged(int index);

    void cboporc_iva_gasto3_currentIndexChanged(int index);

    void SpinGastoDist1_valueChanged(double arg1);
    void SpinGastoDist2_valueChanged(double arg1);
    void SpinGastoDist3_valueChanged(double arg1);

    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);

    void on_Lineas_doubleClicked(const QModelIndex &index);

    void on_btnAnadirLinea_clicked();

    void on_btn_borrarLinea_clicked();

    void on_chkrecargo_equivalencia_toggled(bool checked);

    void on_spinPorc_dto_pp_valueChanged(double arg1);

    void on_spin_porc_dto_especial_valueChanged(double arg1);

    void on_spinPorc_irpf_valueChanged(double);

private:
    Ui::frmPedidos *ui;

    void LLenarCampos();
    void LLenarCamposCliente();
    void VaciarCampos();
    void BloquearCampos(bool state);
    void LLenarPedido();
    void formato_tabla();

    void calcular_iva_gastos();
    void buscar_transportista();

    QAction * aAlbaran_action;
    QAction * aFactura_action;
    QMenu * convertir_menu;

    bool editando;
    QString _moneda;
    QAction menuButton;
    QPushButton* push;
    QSqlQueryModel *model_busqueda;
    TimedMessageBox *t;
    bool eventFilter(QObject *obj, QEvent *event);
    QSqlQueryModel *modelLineas;
    void calcular_pedido();

    BarraBusqueda* m_busqueda;
    void setUpBusqueda();
    QString moneda;
    QString texto;
    QString orden;
    QString modo;

    bool __init;
};

#endif // FRMPEDidOS_H
