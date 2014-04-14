#ifndef FRMORDEN_PEDidO_PRODUCTO_H
#define FRMORDEN_PEDidO_PRODUCTO_H

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
    module_zone module_zone(){return Compras;}
    QString module_name(){return "Orden de pedido";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    QPushButton* wantShortCut(bool& ok){ok = false; return 0;}
private:
    Ui::FrmOrden_Pedido_Producto *ui;
    QAction menuButton;
};

#endif // FRMORDEN_PEDidO_PRODUCTO_H
