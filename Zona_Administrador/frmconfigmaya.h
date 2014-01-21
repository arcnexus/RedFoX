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
    module_zone module_zone(){return AdminZone;}
    QString module_name(){return "Configuracion general";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    QPushButton* wantShortCut(bool& ok){ok = false; return 0;}
private:
    Ui::frmConfigmaya *ui;
    QAction menuButton;
private slots:
    void configurar();
    void on_btnSiguiente_clicked();
    bool checkForMayaGlobal();
    void crear_MayaGlobal();
    void doMenuSelection();
    bool createSuperUser();
    void on_btnInicio_clicked();
};

#endif // FRMCONFIGMAYA_H
