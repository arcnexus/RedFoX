#ifndef FRMINVENTARIO_H
#define FRMINVENTARIO_H

#include "../mayamodule.h"

namespace Ui {
class frmInventario;
}

class frmInventario : public MayaModule
{
    Q_OBJECT
    
public:
    explicit frmInventario(QWidget *parent = 0);
    ~frmInventario();
    
    module_zone module_zone(){return Almacen;}
    QString module_name(){return "Articulos";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok){ok = true; return shortCut;}
private:
    Ui::frmInventario *ui;
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton* shortCut;
};

#endif // FRMINVENTARIO_H
