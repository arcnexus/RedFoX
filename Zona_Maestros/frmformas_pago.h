#ifndef FRMFORMAS_PAGO_H
#define FRMFORMAS_PAGO_H

#include <QDialog>
#include "../mayamodule.h"
#include "../Auxiliares/Globlal_Include.h"
#include "vencimientos.h"

namespace Ui {
class FrmFormas_pago;
}

class FrmFormas_pago : public MayaModule
{
    Q_OBJECT

public:
    explicit FrmFormas_pago(QWidget *parent = 0);
    ~FrmFormas_pago();
    module_zone module_zone(){return Mantenimiento;}
    QString module_name(){return "Formas de pago";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok) {ok = true; return push;}

private slots:
    void on_tabla_buscar_doubleClicked(const QModelIndex &index);

    void on_tabla_buscar_clicked(const QModelIndex &index);

    void on_btnSiguiente_2_clicked();

    void on_btnAnterior_2_clicked();

    void on_btnBuscar_2_clicked();

    void on_btnAnadir_3_clicked();

    void on_btnEditar_3_clicked();

    void on_btnGuardar_2_clicked();

    void on_btndeshacer_2_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_btnAnadir_2_clicked();

    void on_btnEditar_2_clicked();

    void on_btnLimpiar_clicked();

    void on_cboOrden_currentIndexChanged(const QString &arg1);

    void on_cboModo_currentIndexChanged(const QString &arg1);

    void on_btn_borrar_clicked();

    void on_btnborrar_2_clicked();

    void on_txtcod_cuenta_contable_editingFinished();

private:
    Ui::FrmFormas_pago *ui;
    QSqlQueryModel *m;
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton * push;
    vencimientos * oVtos;
    bool anadiendo,buscando;
    void bloquear_campos(bool state);
    void llenar_campos();
    void llenar_objeto();
    void refrescar_tabla();
    bool eventFilter(QObject *obj, QEvent *event);
    void consultar_cuenta();
};

#endif // FRMFORMAS_PAGO_H
