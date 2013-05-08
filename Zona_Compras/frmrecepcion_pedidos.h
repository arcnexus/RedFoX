#ifndef FRMRECEPCION_PEDIDOS_H
#define FRMRECEPCION_PEDIDOS_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class Frmrecepcion_pedidos;
}

class Frmrecepcion_pedidos : public QDialog
{
    Q_OBJECT
    
public:
    explicit Frmrecepcion_pedidos(QWidget *parent = 0);
    ~Frmrecepcion_pedidos();
    bool paso = false;
    
private slots:
    void on_btnBuscar_clicked();

    void on_chkForzarCierre_clicked();

    void on_tablaPedidos_doubleClicked(const QModelIndex &index);

    void validarcantidad(QTableWidgetItem*);
    void reconectar();
    void on_pushButton_3_clicked();

private:
    Ui::Frmrecepcion_pedidos *ui;
};

#endif // FRMRECEPCION_PEDIDOS_H
