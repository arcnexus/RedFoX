#ifndef FRMCONFIGMAYA_H
#define FRMCONFIGMAYA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../mayamodule.h"
namespace Ui {
class frmConfigmaya;
}

class frmConfigmaya : public MayaModule
{
    Q_OBJECT
    
public:
    explicit frmConfigmaya(QWidget *parent = 0);
    ~frmConfigmaya();
    moduleZone ModuleZone(){return AdminZone;}
    QString ModuleName(){return "Configuracion general";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
private:
    Ui::frmConfigmaya *ui;
    ToolBarButton toolButton;
    QAction menuButton;
private slots:
    void configurar();

};

#endif // FRMCONFIGMAYA_H
