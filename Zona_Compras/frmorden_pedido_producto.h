#ifndef FRMORDEN_PEDIDO_PRODUCTO_H
#define FRMORDEN_PEDIDO_PRODUCTO_H

#include <QDialog>
#include "../mayamodule.h"
namespace Ui {
class FrmOrden_Pedido_Producto;
}

class FrmOrden_Pedido_Producto : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmOrden_Pedido_Producto(QWidget *parent = 0);
    ~FrmOrden_Pedido_Producto();
    moduleZone ModuleZone(){return Compras;}
    QString ModuleName(){return "Orden de pedido";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
private:
    Ui::FrmOrden_Pedido_Producto *ui;
    ToolBarButton toolButton;
    QAction menuButton;
};

#endif // FRMORDEN_PEDIDO_PRODUCTO_H
