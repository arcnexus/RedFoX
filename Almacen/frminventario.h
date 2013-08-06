#ifndef FRMINVENTARIO_H
#define FRMINVENTARIO_H

#include "../mayamodule.h"
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class frmInventario;
}

class frmInventario : public MayaModule
{
    Q_OBJECT
    
public:
    explicit frmInventario(QWidget *parent = 0);
    ~frmInventario();
    
    module_zone module_zone(){return Almacen;}
    QString module_name(){return "Articulos";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok){ok = true; return shortCut;}
private slots:
    void on_txtBuscar_textEdited(const QString &arg1);

private:
    Ui::frmInventario *ui;
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton* shortCut;
    void formato_tabla(QSqlTableModel *modelo);
    QSqlTableModel * m;
};

#endif // FRMINVENTARIO_H
