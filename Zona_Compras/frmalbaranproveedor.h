#ifndef FRMALBARANPROVEEDOR_H
#define FRMALBARANPROVEEDOR_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../Auxiliares/table_helper.h"
#include "proveedor.h"
#include "albaranproveedor.h"
#include "../mayamodule.h"
namespace Ui {
class FrmAlbaranProveedor;
}

class FrmAlbaranProveedor : public MayaModule
{
    Q_OBJECT
    
public slots:
    void lineaReady(lineaDetalle *ld);
public:
    explicit FrmAlbaranProveedor(QWidget *parent = 0, bool showCerrar = false);
    ~FrmAlbaranProveedor();
    void llenarProveedor(int id);
    void llenar_tabla_entregas();
    int id;


    moduleZone ModuleZone(){return Compras;}
    QString ModuleName(){return "Albaranes Proveedor";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
    void resizeTable(int x);
private slots:
    void totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();
    void buscar_proveeedor();
    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();
    void guardar_campos_en_objeto();

    void on_btnBuscar_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnAnadir_clicked();

    void on_btnBorrar_clicked();

    void on_btnAnadirEntrega_clicked();

    void on_tabWidget_2_currentChanged(int index);

private:
    Ui::FrmAlbaranProveedor *ui;
    Table_Helper helper;
    Proveedor prov;
    AlbaranProveedor *oAlbPro;
    QString moneda;
    void llenar_campos();
    void bloquearcampos(bool estado);
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton* push;
signals:

};

#endif // FRMALBARANPROVEEDOR_H
