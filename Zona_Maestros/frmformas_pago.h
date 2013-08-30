#ifndef FRMFORMAS_PAGO_H
#define FRMFORMAS_PAGO_H

#include <QDialog>
#include "../mayamodule.h"
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmFormas_pago;
}

class FrmFormas_pago : public MayaModule
{
    Q_OBJECT

public:
    explicit FrmFormas_pago(QWidget *parent = 0);
    ~FrmFormas_pago();
    module_zone module_zone(){return Mantenimiento;}
    QString module_name(){return "Formas de pago";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok) {ok = true; return push;}

private:
    Ui::FrmFormas_pago *ui;
    QSqlQueryModel *m;
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton * push;
};

#endif // FRMFORMAS_PAGO_H
