#ifndef FRMGESTIONCOBROS_H
#define FRMGESTIONCOBROS_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../mayamodule.h"
namespace Ui {
class frmGestionCobros;
}

class frmGestionCobros : public MayaModule
{
    Q_OBJECT
    
public:
    explicit frmGestionCobros(QWidget *parent = 0);
    ~frmGestionCobros();
    module_zone module_zone(){return Ventas;}
    QString module_name(){return "Gestion Cobros";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}QPushButton* wantShortCut(bool& ok){ok = false; return 0;}
private:
    Ui::frmGestionCobros *ui;
    ToolBarButton toolButton;
    QAction menuButton;
private slots:
    void on_txtbuscar_cliente_textChanged(const QString &arg1);
    void on_btncobro_total_clicked();
};

#endif // FRMGESTIONCOBROS_H
