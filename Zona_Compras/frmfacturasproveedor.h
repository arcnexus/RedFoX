#ifndef FRMFACTURASPROVEEDOR_H
#define FRMFACTURASPROVEEDOR_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../Auxiliares/table_helper.h"
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
    int id;

    moduleZone ModuleZone(){return Compras;}
    QString ModuleName(){return "Facturas Proveedor";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
private slots:
    void totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
    void lineaReady(lineaDetalle *ld);
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
    void resizeTable(int x);

private:
    Ui::FrmFacturasProveedor *ui;
    Table_Helper helper;
    Proveedor prov;
    FacturasProveedor *oFacPro;
    ToolBarButton toolButton;
    QAction menuButton;
signals:

};

#endif // FRMFACTURASPROVEEDOR_H
