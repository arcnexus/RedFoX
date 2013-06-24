#ifndef FRMENTRADA_APUNTES_H
#define FRMENTRADA_APUNTES_H

#include <QDialog>
#include "../mayamodule.h"
namespace Ui {
class FrmEntrada_apuntes;
}

class FrmEntrada_apuntes : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmEntrada_apuntes(QWidget *parent = 0);
    ~FrmEntrada_apuntes();
    moduleZone ModuleZone(){return Contabilidad;}
    QString ModuleName(){return "Entrada Apuntes";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
private:
    Ui::FrmEntrada_apuntes *ui;
    ToolBarButton toolButton;
    QAction menuButton;
signals:

};

#endif // FRMENTRADA_APUNTES_H
