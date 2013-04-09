#ifndef FRMRECEPCION_PEDIDOS_H
#define FRMRECEPCION_PEDIDOS_H

#include <QDialog>

namespace Ui {
class Frmrecepcion_pedidos;
}

class Frmrecepcion_pedidos : public QDialog
{
    Q_OBJECT
    
public:
    explicit Frmrecepcion_pedidos(QWidget *parent = 0);
    ~Frmrecepcion_pedidos();
    
private:
    Ui::Frmrecepcion_pedidos *ui;
};

#endif // FRMRECEPCION_PEDIDOS_H
