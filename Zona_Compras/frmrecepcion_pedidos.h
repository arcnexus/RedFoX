#ifndef FRMRECEPCION_PEDIDOS_H
#define FRMRECEPCION_PEDIDOS_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../mayamodule.h"
namespace Ui {
class Frmrecepcion_pedidos;
}

class Frmrecepcion_pedidos : public MayaModule
{
    Q_OBJECT
    
public:
    explicit Frmrecepcion_pedidos(QWidget *parent = 0);
    ~Frmrecepcion_pedidos();
    bool paso;
    moduleZone ModuleZone(){return Compras;}
    QString ModuleName(){return "Recepcion de pedidos";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
private slots:
    void on_btnBuscar_clicked();

    void on_chkForzarCierre_clicked();

    void on_tablaPedidos_doubleClicked(const QModelIndex &index);

    void validarcantidad(int, int);
    void reconectar();
    void abrir_albaran();

private:
    Ui::Frmrecepcion_pedidos *ui;
    bool albaran;
    int id_albaran;
    bool factura;
    int id_factura;
    int id_pedido;
    ToolBarButton toolButton;
    QAction menuButton;
};

#endif // FRMRECEPCION_PEDIDOS_H
