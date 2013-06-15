#ifndef FRMFACTURARALABARANES_H
#define FRMFACTURARALABARANES_H

#include <QDialog>
#include "../mayamodule.h"
namespace Ui {
class FrmFacturarAlabaranes;
}

class FrmFacturarAlabaranes : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmFacturarAlabaranes(QWidget *parent = 0);
    ~FrmFacturarAlabaranes();
    
    moduleZone ModuleZone(){return Ventas;}
    QString ModuleName(){return "FormFacturarAlbaranesVentas";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
private:
    Ui::FrmFacturarAlabaranes *ui;

    ToolBarButton toolButton;
    QAction menuButton;
};

#endif // FRMFACTURARALABARANES_H
