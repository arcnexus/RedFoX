#ifndef FRMGESTIONCOBROS_H
#define FRMGESTIONCOBROS_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
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
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    QPushButton* wantShortCut(bool& ok){ok = false; return 0;}

    void format_tables();
private slots:

    void on_radPendientes_toggled(bool checked);

    void on_radPagados_toggled(bool checked);

    void on_radTodos_toggled(bool checked);

    void on_txtbuscar_cliente_editingFinished();

    void on_txtfecha_ini_dateChanged(const QDate &date);

    void on_txtfecha_fin_dateChanged(const QDate &date);

    void on_tabla_clientes_clicked(const QModelIndex &index);

    void on_btncobro_total_clicked();

private:
    Ui::frmGestionCobros *ui;
    bool eventFilter(QObject *obj, QEvent *event);

    QAction menuButton;
    QSqlQueryModel *deudas;
    QSqlQueryModel *modelo_clientes;
    void init_querys();
    void init();
    bool __init;
};

#endif // FRMGESTIONCOBROS_H
