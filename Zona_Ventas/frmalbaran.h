#ifndef FRMALBARAN_H
#define FRMALBARAN_H

#include "../Auxiliares/Globlal_Include.h"
#include "../Auxiliares/table_helper.h"
#include "../mayamodule.h"
#include "../Auxiliares/timedmessagebox.h"
#include "albaran.h"
#include "../Zona_Pacientes/cliente.h"

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
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
    void setUpBusqueda();
signals:

private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnadir_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnAnterior_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_btn_borrar_clicked();


    void on_btndeshacer_clicked();

    void on_btnImprimir_clicked();

    void on_table2_doubleClicked(const QModelIndex &index);

    void on_btnBuscar_clicked();

    void on_btnForzar_edicion_clicked();

    void on_btnFacturar_clicked();

    void mostrarBusqueda();
    void ocultarBusqueda();

    void on_cboseries_currentIndexChanged(const QString &arg1);

    void refrescar_modelo();
    void calcular_albaran();

    void on_btnAnadirLinea_clicked();

    void on_Lineas_doubleClicked(const QModelIndex &index);

    void on_btn_borrarLinea_clicked();

    void on_SpinGastoDist1_valueChanged(double arg1);

    void on_SpinGastoDist2_valueChanged(double arg1);

    void on_SpinGastoDist3_valueChanged(double arg1);

    void on_cboporc_iva_gasto1_currentIndexChanged(const QString &arg1);

    void on_cboporc_iva_gasto2_currentIndexChanged(int index);

    void on_cboporc_iva_gasto3_currentIndexChanged(int index);

    void filter_table(QString texto, QString orden, QString modo);
    void on_spinporc_Dto_valueChanged(double arg1);

    void on_spinPorc_dto_pp_valueChanged(double arg1);

    void on_spinporc_irpf_valueChanged(double arg1);

    void on_chkrecargo_equivalencia_toggled(bool checked);

    void listados();
private:
    Ui::FrmAlbaran *ui;
    QSqlQueryModel *ModelLin_alb;
    QSqlQueryModel *modelLineas;
    Table_Helper helper;
    bool in_edit;
    QString moneda;

    QAction menuButton;
    QPushButton* push;
    QSqlQueryModel *m;
    void formato_tabla();

    void calcular_iva_gastos();
    TimedMessageBox *t;
    QSqlQueryModel *series;
    bool eventFilter(QObject *obj, QEvent *event);

    BarraBusqueda* m_busqueda;
    QComboBox* cboseries;
    QString texto;
    QString orden;
    QString modo;
    QString serie;

    Albaran *oAlbaran ;
    Cliente *oCliente2;
};

#endif // FRMALBARAN_H
