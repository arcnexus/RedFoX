#ifndef FRMCAJAS_H
#define FRMCAJAS_H

#include <QDialog>
#include "../mayamodule.h"
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmCajas;
}

class FrmCajas : public MayaModule
{
    Q_OBJECT

public:
    explicit FrmCajas(QWidget *parent = 0);
    ~FrmCajas();
    module_zone module_zone(){return Mantenimiento;}
    QString module_name(){return "Cajas";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}

    QPushButton* wantShortCut(bool& ok) {ok = false; return 0;}

private:
    Ui::FrmCajas *ui;
    QAction menuButton;
    QSqlQuery *model;

    BarraBusqueda *m_busqueda;
};

#endif // FRMCAJAS_H
