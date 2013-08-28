#ifndef FRMPEDidOS_H
#define FRMPEDidOS_H

#include "../Auxiliares/Globlal_Include.h"

#include "pedidos.h"
#include "../Zona_Pacientes/cliente.h"
#include "../Auxiliares/table_helper.h"
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
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
signals:

private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnadir_clicked();

    void on_btnAnterior_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnDeshacer_clicked();

    void on_btn_borrar_clicked();

    void totalChanged(double base , double dto ,double subtotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
    void lineaReady(lineaDetalle*);
    void lineaDeleted(lineaDetalle*);

    void convertir_ealbaran();
    void convertir_enFactura();
    void on_tabWidget_2_currentChanged(int index);


    void on_txtcodigo_cliente_editingFinished();

    void on_btnImprimir_clicked();

    void on_radBusqueda_toggled(bool checked);

    void on_txtBuscar_textEdited(const QString &arg1);

    void on_tabla_clicked(const QModelIndex &index);

    void on_tabla_doubleClicked(const QModelIndex &index);

    void on_btnBuscar_clicked();

    void on_btnLimpiar_clicked();

    void on_cboModo_currentIndexChanged(const QString &arg1);

    void on_cboOrden_currentIndexChanged(const QString &arg1);

    void on_btnAnadir_2_clicked();

    void on_btnEditar_2_clicked();

    void on_btnImprimir_2_clicked();

    void on_btnBorrar_2_clicked();

    void on_spin_porc_dto_pp_editingFinished();

    void on_spin_porc_dto_especial_editingFinished();

    void cboporc_iva_gasto1_currentIndexChanged(int index);

    void cboporc_iva_gasto2_currentIndexChanged(int index);

    void cboporc_iva_gasto3_currentIndexChanged(int index);

    void SpinGastoDist1_valueChanged(double arg1);
    void SpinGastoDist2_valueChanged(double arg1);
    void SpinGastoDist3_valueChanged(double arg1);
private:
    Ui::frmPedidos *ui;

    void LLenarCampos();
    void LLenarCamposCliente();
    void VaciarCampos();
    void BloquearCampos(bool state);
    void LLenarPedido();
    void formato_tabla();
    void filter_table();
    void calcular_iva_gastos();
    void buscar_transportista();

    Table_Helper helper;
    QAction * aAlbaran_action;
    QAction * aFactura_action;
    QMenu * convertir_menu;

    bool editando;
    QString _moneda;
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton* push;
    QSqlQueryModel *m;
    TimedMessageBox *t;
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // FRMPEDidOS_H
