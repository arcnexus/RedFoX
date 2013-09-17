#ifndef FRMTPV_H
#define FRMTPV_H

#include "../Auxiliares/Globlal_Include.h"
#include "../mayamodule.h"

namespace Ui {
class FrmTPV;
}

class FrmTPV : public MayaModule
{
    Q_OBJECT

public:
    explicit FrmTPV(QWidget *parent = 0);
    ~FrmTPV();

    module_zone module_zone(){return Ventas;}
    QString module_name(){return "FormTPV";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}

private slots:
    void on_btnClientes_clicked();
    void on_btnBuscar_clicked();



    void on_btnCambioModo_clicked();

    void on_txtCodigo_editingFinished();

private:
    Ui::FrmTPV *ui;
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton* push;
    QTableView *tablabuscar;
};

#endif // FRMTPV_H
