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
    void buscar_deuda(int id_cli);
    void buscar_deuda(int id_cli, int id_doc);
    void titulo(QString titulo);
    int getId_factura() const;
    void setId_factura(int value);

    int getId_ticket() const;
    void setId_ticket(int value);

private:
    Ui::frmGestionCobros *ui;
    ToolBarButton toolButton;
    QAction menuButton;
    QSqlQueryModel *deudas;


    int id_cliente;
    int id_factura;
    int id_ticket;


private slots:
    void on_txtbuscar_cliente_textChanged(const QString &arg1);
    void on_btncobro_total_clicked();
    void on_btnCerrar_clicked();
    void seleccionar_factura();
    void seleccionar_ticket();
    void seleccionar_varios();
    void on_txtfecha_fin_editingFinished();
    void on_chkCliente_clicked(bool checked);
    void on_txtfecha_ini_editingFinished();
};

#endif // FRMGESTIONCOBROS_H
