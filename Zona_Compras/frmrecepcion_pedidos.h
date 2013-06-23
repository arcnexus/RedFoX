#ifndef FRMRECEPCION_PEDIDOS_H
#define FRMRECEPCION_PEDIDOS_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class Frmrecepcion_pedidos;
}

class Frmrecepcion_pedidos : public QDialog
{
    Q_OBJECT
    
public:
    explicit Frmrecepcion_pedidos(QWidget *parent = 0);
    ~Frmrecepcion_pedidos();
    bool paso;
    
private slots:
    void on_btnBuscar_clicked();

    void on_chkForzarCierre_clicked();

    void on_tablaPedidos_doubleClicked(const QModelIndex &index);

    void validarcantidad(int, int);
    void reconectar();
    void abrir_albaran();

    void on_btnFactura_clicked();

    void on_BtnCancelar_clicked();

    void on_btncancelar_alb_clicked();

private:
    Ui::Frmrecepcion_pedidos *ui;
    bool albaran;
    int id_albaran;
    bool factura;
    int id_factura;
    int id_pedido;
signals:
    void block();
    void unblock();
};

#endif // FRMRECEPCION_PEDIDOS_H
