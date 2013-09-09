#ifndef FRMALBARAN_H
#define FRMALBARAN_H

#include "../Auxiliares/Globlal_Include.h"
#include "../Busquedas/frmbuscarcliente.h"
//
#include "../Auxiliares/table_helper.h"
#include "../mayamodule.h"
#include "../Auxiliares/timedmessagebox.h"
namespace Ui {
class FrmAlbaran;
}

class FrmAlbaran : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmAlbaran(QWidget *parent = 0);
    ~FrmAlbaran();
    void LLenarCampos();
    void LLenarCamposCliente();
    void VaciarCampos();
    void BloquearCampos(bool state);
    void DesbloquearCampos();
    void LLenarAlbaran();

    module_zone module_zone(){return Ventas;}
    QString module_name(){return "Albaranes Ventas";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
signals:

private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnadir_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnAnterior_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_btn_borrar_clicked();

    void totalChanged(double base , double dto ,double subtotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);

    void lineaReady(lineaDetalle*);
    void lineaDeleted(lineaDetalle*);
    void on_btndeshacer_clicked();

    void on_tabWidget_2_currentChanged(int index);

    void on_btnImprimir_clicked();

    void on_radBusqueda_toggled(bool checked);

    void on_cboOrden_currentIndexChanged(const QString &arg1);

    void on_table2_clicked(const QModelIndex &index);

    void on_table2_doubleClicked(const QModelIndex &index);

    void on_btnBuscar_clicked();

    void on_txtBuscar_textEdited(const QString &arg1);

    void on_btnLimpiar_clicked();

    void on_cboModo_currentIndexChanged(const QString &arg1);

    void on_btnForzar_edicion_clicked();


    void on_btnAnadir_2_clicked();

    void on_btnFacturar_clicked();

    void on_btnImprimir_2_clicked();

    void on_btnborrar_2_clicked();

    void on_btnEditar_2_clicked();

    void on_spinporc_Dto_editingFinished();

    void on_spinPorc_dto_pp_editingFinished();

private:
    Ui::FrmAlbaran *ui;
    FrmBuscarCliente *BuscarClientes;
    QSqlQueryModel *ModelLin_alb;
    Table_Helper helper;
    bool in_edit;

    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton* push;
    QSqlQueryModel *m;
    void formato_tabla();
    void filter_table();
    void calcular_iva_gastos();
    TimedMessageBox *t;
    QSqlQueryModel *series;
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif // FRMALBARAN_H
