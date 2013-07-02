#ifndef FRMCUADRO_CUENTAS_H
#define FRMCUADRO_CUENTAS_H

#include <QDialog>
#include"../Zona_Contabilidad/cuentas_contables.h"
#include "../mayamodule.h"
namespace Ui {
class frmCuadro_cuentas;
}

class frmCuadro_cuentas : public MayaModule
{
    Q_OBJECT
    
public:
    explicit frmCuadro_cuentas(QWidget *parent = 0);
    ~frmCuadro_cuentas();
    Cuentas_contables *cuentas;
    
    module_zone module_zone(){return Contabilidad;}
    QString module_name(){return "Cuadro de cuentas";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok) {ok = true; return push;}
private slots:
    void on_btnAnadir_clicked();
    void llenar_campos();
    void llenar_objeto();
    void activar_controles(bool state);

    void on_btnguardar_clicked();
    void llenar_tabla();

private:
    Ui::frmCuadro_cuentas *ui;
    bool anadir;
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton * push;
};

#endif // FRMCUADRO_CUENTAS_H
