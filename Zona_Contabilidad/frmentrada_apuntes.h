#ifndef FRMENTRADA_APUNTES_H
#define FRMENTRADA_APUNTES_H

#include <QDialog>

#include "../mayamodule.h"

#include "apuntes.h"

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
    apuntes *oApunte;
    void llenar_objeto();
    void clear();
    void llenar_tabla_apunte(int asiento);
    void totales_asiento(int asiento);

private slots:
    void on_btnnuevo_asiento_clicked();
    void controles(bool stat);
    void on_btncerrar_asiento_clicked();
    void on_btnanadir_linea_clicked();
    void on_txtnum_asiento_valueChanged(int arg1);
    void on_txtcuenta_debe_editingFinished();

};

#endif // FRMENTRADA_APUNTES_H
