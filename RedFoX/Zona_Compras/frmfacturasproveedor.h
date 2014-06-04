#ifndef FRMFACTURASPROVEEDOR_H
#define FRMFACTURASPROVEEDOR_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"

#include "proveedor.h"
#include "facturasproveedor.h"
#include "../mayamodule.h"
namespace Ui {
class FrmFacturasProveedor;
}

class FrmFacturasProveedor : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmFacturasProveedor(QWidget *parent = 0, bool showCerrar = false);
    ~FrmFacturasProveedor();
    void llenarProveedor(int id);

    module_zone module_zone(){return Compras;}
    QString module_name(){return "Facturas Proveedor";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
    void init_querys();
    void init();
private slots:
    void bloquearcampos(bool state);
    void on_btnAnadir_clicked();
    void llenar_campos();
    void llenar_fac_pro();
    void llenar_campos_iva();

    void on_btnGuardar_clicked();

    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnEditar_clicked();

    void on_btnDeshacer_clicked();

    void on_btnListados_clicked();

    void on_btnBuscar_clicked();

    void on_tabla_clicked(const QModelIndex &index);

    void on_tabla_doubleClicked(const QModelIndex &index);

    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);
    void borrarFactura();
    void on_btnAnadirLinea_clicked();

    void on_Lineas_doubleClicked(const QModelIndex &index);

    void on_btn_borrarLinea_clicked();

    void llenarLineas();
    void on_chklporc_rec_toggled(bool checked);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    Ui::FrmFacturasProveedor *ui;
    Proveedor prov;
    FacturasProveedor *oFacPro;
    QAction menuButton;
    QPushButton* push;

    QSqlQueryModel *model_busqueda;
    QSqlQueryModel *modeltipogasto;
    QSqlQueryModel *modelLineas;
    void formato_tabla();


    BarraBusqueda* m_busqueda;
    void setUpBusqueda();

    bool __init;
    bool _showCerrar;

    bool editando;
    double total_anterior;
    QDate fecha_anterior;
    QSqlQueryModel* lineas_anterior;
    void calcular_factura();
signals:

};

#endif // FRMFACTURASPROVEEDOR_H
