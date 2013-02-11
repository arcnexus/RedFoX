#ifndef FRMFACTURASPROVEEDOR_H
#define FRMFACTURASPROVEEDOR_H

#include <QDialog>

namespace Ui {
class FrmFacturasProveedor;
}

class FrmFacturasProveedor : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmFacturasProveedor(QWidget *parent = 0);
    ~FrmFacturasProveedor();
    
private:
    Ui::FrmFacturasProveedor *ui;
};

#endif // FRMFACTURASPROVEEDOR_H
