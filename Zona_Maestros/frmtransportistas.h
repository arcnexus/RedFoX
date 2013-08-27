#ifndef FRMTRANSPORTISTAS_H
#define FRMTRANSPORTISTAS_H

#include "../Auxiliares/Globlal_Include.h"
#include "../mayamodule.h"

namespace Ui {
class FrmTransportistas;
}

class FrmTransportistas  : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmTransportistas(QWidget *parent = 0);
    int id_contacto;
    ~FrmTransportistas();
    module_zone module_zone(){return Mantenimiento;}
    QString module_name(){return "Transportistas";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
    
private:
    Ui::FrmTransportistas *ui;
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton *push;
    QSqlQueryModel *model;
};

#endif // FRMTRANSPORTISTAS_H
