#ifndef FRMPRESUPUESTOSCLI_H
#define FRMPRESUPUESTOSCLI_H

#include "../Auxiliares/Globlal_Include.h"
#include "presupuesto.h"
#include "../Zona_Pacientes/cliente.h"

#include "../Auxiliares/table_helper.h"
#include <QTimer>
#include "../mayamodule.h"
namespace Ui {
class FrmPresupuestosCli;
}

class FrmPresupuestosCli : public MayaModule
{
    Q_OBJECT

signals:

public:
    explicit FrmPresupuestosCli(QWidget *parent = 0);
    ~FrmPresupuestosCli();
    void LLenarCampos();
    void LLenarCamposCliente();
    void LLenarPresupuesto();
    void VaciarCampos();
    void BloquearCampos(bool state);
    Presupuesto *oPres;
    Cliente *oClientePres;

    module_zone module_zone(){return Ventas;}
    QString module_name(){return "Presupestos Ventas";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
private slots:
    void on_chklAprovado_stateChanged(int arg1);

    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

    void on_btnAnadir_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_btnBuscar_clicked();

    void on_botBuscarCliente_clicked();

    void totalChanged(double base , double dto ,double subtotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
    void on_btnDeshacer_clicked();

    void convertir_epedido();
    void convertir_ealbaran();
    void convertir_enFactura();
    void on_btnBorrar_clicked();

    void lineaReady(lineaDetalle*);
    void lineaDeleted(lineaDetalle*);
    void on_tabWidget_currentChanged(int index);


    void on_btndeshacer_clicked();

    void on_txtcodigo_cliente_editingFinished();

    void on_btnImprimir_clicked();



    void on_radBusqueda_toggled(bool checked);

    void on_txtBuscar_textEdited(const QString &arg1);

    void on_tabla_clicked(const QModelIndex &index);

    void on_tabla_doubleClicked(const QModelIndex &index);

    void on_btn_convertir_clicked();

    void on_cboOrden_currentIndexChanged(const QString &arg1);

    void on_cboModo_currentIndexChanged(const QString &arg1);


    void on_btnDesbloquear_clicked();

    void on_btnLimpiar_clicked();

    void on_cboOrden_currentIndexChanged(int index);

    void on_cboModo_currentIndexChanged(int index);

    void on_btnAnadir_2_clicked();

    void on_btnEditar_2_clicked();

    void on_btnImprimir_2_clicked();

    void on_spin_porc_dto_editingFinished();

    void on_spin_porc_dto_pp_editingFinished();

private:
    Ui::FrmPresupuestosCli *ui;
    QAction * aPedido_action;
    QAction * aAlbaran_action;
    QAction * aFactura_action;
    QMenu * convertir_menu;
    Table_Helper helper;
    bool editando;
    QString moneda;

    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton* push;
    QSqlQueryModel *m;
    TimedMessageBox *t;
    bool eventFilter(QObject *obj, QEvent *event);

    void formato_tabla();
    void filter_table();
    void buscar_poblacion(int tipo);
};

#endif // FRMPRESUPUESTOSCLI_H
