#ifndef FRMFACTURAS_H
#define FRMFACTURAS_H

#include "../Auxiliares/Globlal_Include.h"

#include "factura.h"
#include "../Zona_Pacientes/cliente.h"
#include "../Auxiliares/table_helper.h"
#include "../mayamodule.h"
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

private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnadir_clicked();

    void on_btnAnterior_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnBuscar_clicked();

    void on_btnImprimir_clicked();

    void totalChanged(double base , double dto ,double subtotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
    void on_btnEditar_clicked();
    void lineaReady(lineaDetalle* ld);
    void lineaDeleted(lineaDetalle*);
                         void on_tabWidget_2_currentChanged(int index);

    bool crear_asiento();
    
    void on_btndeshacer_clicked();

    void on_txtcodigo_cliente_editingFinished();

    void on_anadirEntrega_clicked();


    void on_radBuscar_toggled(bool checked);

    void on_radEditar_toggled(bool checked);

    void on_cboVer_currentTextChanged(const QString &arg1);

    void on_btnAsignarTransportista_clicked();

    void on_tabla_facturas_doubleClicked(const QModelIndex &index);

    void on_tabla_facturas_clicked(const QModelIndex &index);


    void on_btnArticulos_clicked();

    void on_cboseries_currentIndexChanged(const QString &arg1);

    void on_cboBuscar_currentIndexChanged(const QString &arg1);

    void on_cboModo_currentIndexChanged(const QString &arg1);

    void on_btnBorrar_clicked();

    void on_spinPorc_dto_editingFinished();

    void on_spinPorc_dto_pp_editingFinished();

    void on_btnCobrar_clicked();

    void on_btnGuardar_clicked();

    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);
    void on_cboDireccionesEntrega_currentIndexChanged(const QString &arg1);
    void series_changed(QString text);


private:
    Ui::frmFacturas *ui;
    QAction * actionGuardaBorrador;
    QAction * actionGuardaFactura;
    QMenu* menu_guardar;
    bool Altas;
    Table_Helper helper;
    bool in_edit;

    QAction menuButton;
    QPushButton *push;
    QString moneda;
    QHash<QString,QString> h_Buscar;
    QSqlQueryModel *m_facturas;
    void formato_tabla_facturas(QSqlQueryModel &modelo);

    bool eventFilter(QObject *obj, QEvent *event);
    void buscar_poblacion(int tipo);

    BarraBusqueda* m_busqueda;
    void setUpBusqueda();
    QComboBox* cboSeries;
    QStringList series2_l;
};

#endif // FRMFACTURAS_H
