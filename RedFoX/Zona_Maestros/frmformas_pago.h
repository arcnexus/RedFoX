#ifndef FRMFORMAS_PAGO_H
#define FRMFORMAS_PAGO_H

#include <QDialog>
#include "../mayamodule.h"
#include "Auxiliares/Globlal_Include.h"
#include "../vencimientos.h"

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
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    
    QPushButton* wantShortCut(bool& ok) {ok = false; return 0;}

private slots:
    void on_tabla_buscar_doubleClicked(const QModelIndex &index);

    void on_btnSiguiente_2_clicked();

    void on_btnAnterior_2_clicked();

    void on_btnBuscar_2_clicked();

    void on_btnAnadir_3_clicked();

    void on_btnEditar_3_clicked();

    void on_btnGuardar_2_clicked();

    void on_btndeshacer_2_clicked();

    void on_btnEditar_2_clicked();

    void on_btn_borrar_clicked();

    //void on_btnborrar_2_clicked();

    void on_txtcod_cuenta_contable_editingFinished();

    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);
private:
    Ui::FrmFormas_pago *ui;
    QSqlQueryModel *m;
    QAction menuButton;
    vencimientos * oVtos;
    bool anadiendo,buscando;
    void bloquear_campos(bool state);
    void llenar_campos();
    void llenar_objeto();

    bool eventFilter(QObject *obj, QEvent *event);
    void consultar_cuenta();


    BarraBusqueda* m_busqueda;
    void setUpBusqueda();
};

#endif // FRMFORMAS_PAGO_H
