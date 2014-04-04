#ifndef FRMFACTURAS_H
#define FRMFACTURAS_H

#include "../Auxiliares/Globlal_Include.h"

#include "factura.h"
#include "../Zona_Pacientes/cliente.h"

#include "../mayamodule.h"
#include "../Almacen/articulo.h"
namespace Ui {
class frmFacturas;
}

class frmFacturas : public MayaModule
{
    Q_OBJECT
    
public slots:
    void LLenarCampos();
    void LLenarCamposCliente();
    void LLenarFactura();
    void VaciarCampos();
    void BloquearCampos(bool state);



signals:

public:
    explicit frmFacturas(QWidget *parent = 0);
    ~frmFacturas();
    Factura *oFactura;
    Cliente *oCliente1;
    module_zone module_zone(){return Ventas;}
    QString module_name(){return "Facturas Ventas";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}

    void formato_tabla_lineas();
    void init_querys();

private slots:
    void init();

    void on_btnSiguiente_clicked();

    void on_btnAnadir_clicked();

    void on_btnAnterior_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnBuscar_clicked();

    void on_btnImprimir_clicked();
    void mostrar_factura();
    void refrescar_modelo();

    void on_btnEditar_clicked();
    void desbloquear_factura();

    bool crear_asiento();
    
    void on_btndeshacer_clicked();

    void on_txtcodigo_cliente_editingFinished();

    void on_anadirEntrega_clicked();

    void on_btnAsignarTransportista_clicked();

    void on_tabla_facturas_doubleClicked(const QModelIndex &index);

    void on_btnArticulos_clicked();

    void on_cboseries_currentIndexChanged(const QString &arg1);

    void on_btnBorrar_clicked();

    void on_spinPorc_dto_editingFinished();

    void on_spinPorc_dto_pp_editingFinished();

    void on_btnCobrar_clicked();

    void on_btnGuardar_clicked();

    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);

    void on_SpinGastoDist1_editingFinished();


    void on_spin_porc_iva_gasto1_editingFinished();

    void on_SpinGastoDist2_editingFinished();

    void on_spin_porc_iva_gasto2_editingFinished();

    void on_SpinGastoDist3_editingFinished();

    void on_spin_porc_iva_gasto3_editingFinished();

    void on_btnAnadirLinea_clicked();

    void on_Lineas_doubleClicked(const QModelIndex &index);

    void on_btn_borrarLinea_clicked();

    void on_cboDireccionesEntrega_currentIndexChanged(int index);

    void listados();
    void on_chkrecargo_equivalencia_toggled(bool checked);

private:
    Ui::frmFacturas *ui;
    QAction * actionGuardaBorrador;
    QAction * actionGuardaFactura;
    QMenu* menu_guardar;
    bool Altas;
    bool in_edit;

    QAction menuButton;
    QPushButton *push;
    QString moneda;
    QHash<QString,QString> h_Buscar;
    QSqlQueryModel *m_facturas;
    QSqlQueryModel *modelLineas;
    void formato_tabla_facturas();

    bool eventFilter(QObject *obj, QEvent *event);
    void calcular_factura();

    BarraBusqueda* m_busqueda;
    Articulo *oArticulo;
    void setUpBusqueda();
    QComboBox* cboSeries;
    QStringList series2_l;
    QString texto;
    QString orden;
    QString modo;
    QString serie;
    QString ejercicio;
    QHash<QString,QVariant> acumulados;

    QSqlQueryModel * model_series;
    QSqlQueryModel * model_dir_entrega;

    bool __init;
};

#endif // FRMFACTURAS_H
