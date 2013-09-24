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
    
    module_zone module_zone(){return Ventas;}
    QString module_name(){return "Facturar Albaranes Ventas";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    QPushButton* wantShortCut(bool& ok){ok = false; return 0;}
private:
    Ui::FrmFacturarAlabaranes *ui;

    QAction menuButton;
};

#endif // FRMFACTURARALABARANES_H
