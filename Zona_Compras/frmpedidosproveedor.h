#ifndef FRMPEDIDOSPROVEEDOR_H
#define FRMPEDIDOSPROVEEDOR_H

#include <QDialog>

namespace Ui {
class FrmPedidosProveedor;
}

class FrmPedidosProveedor : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmPedidosProveedor(QWidget *parent = 0);
    ~FrmPedidosProveedor();
    
private:
    Ui::FrmPedidosProveedor *ui;
};

#endif // FRMPEDIDOSPROVEEDOR_H
