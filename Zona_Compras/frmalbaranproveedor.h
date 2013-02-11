#ifndef FRMALBARANPROVEEDOR_H
#define FRMALBARANPROVEEDOR_H

#include <QDialog>

namespace Ui {
class FrmAlbaranProveedor;
}

class FrmAlbaranProveedor : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAlbaranProveedor(QWidget *parent = 0);
    ~FrmAlbaranProveedor();
    
private:
    Ui::FrmAlbaranProveedor *ui;
};

#endif // FRMALBARANPROVEEDOR_H
