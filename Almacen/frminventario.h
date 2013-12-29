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
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}

    QPushButton* wantShortCut(bool& ok){ok = true; return shortCut;}
private slots:
    void on_txtBuscar_textEdited(const QString &arg1);

    void on_btnSincronizar_clicked();

    void on_btnBuscar_clicked();

private:
    Ui::frmInventario *ui;
    QAction menuButton;
    QPushButton* shortCut;
    void formato_tabla(QSqlTableModel *modelo);
    QSqlTableModel * m;
};

#endif // FRMINVENTARIO_H
