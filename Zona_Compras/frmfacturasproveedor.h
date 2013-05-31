#ifndef FRMFACTURASPROVEEDOR_H
#define FRMFACTURASPROVEEDOR_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../Auxiliares/table_helper.h"
#include "proveedor.h"
namespace Ui {
class FrmFacturasProveedor;
}

class FrmFacturasProveedor : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmFacturasProveedor(QWidget *parent = 0, bool showCerrar = false);
    ~FrmFacturasProveedor();
    void llenarProveedor(int id);
private slots:
    void totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
private:
    Ui::FrmFacturasProveedor *ui;
    Table_Helper helper;
    Proveedor prov;
};

#endif // FRMFACTURASPROVEEDOR_H
