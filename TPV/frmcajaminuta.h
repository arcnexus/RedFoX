#ifndef FRMCAJAMINUTA_H
#define FRMCAJAMINUTA_H

#include "../Auxiliares/Globlal_Include.h"
#include "../db_table_view.h"
#include "ticket.h"
#include "../mayamodule.h"
namespace Ui {
class FrmCajaMinuta;
}

class FrmCajaMinuta : public MayaModule
{
    Q_OBJECT
signals:

public:
    explicit FrmCajaMinuta(QWidget *parent = 0);
    ~FrmCajaMinuta();
    
    moduleZone ModuleZone(){return Ventas;}
    QString ModuleName(){return "FormTPV";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
private slots:
    void on_btnBuscarArt_clicked();
    void linea_itemSelectionChanged();
    void on_btn_abrirCerrarCaja_clicked();

private:
    Ui::FrmCajaMinuta *ui;
    void bloquearCaja(bool state);

    void focusInEvent(QFocusEvent * e);
    bool eventFilter(QObject *target, QEvent *event);
    bool keys_onCodigo(int key);
    bool keys_lineas(int key);

    bool rellenarArticulo(QString cCodigo);
    Ticket ticket;
    int linea_row;
    int linea_column;
    bool caja_abierta;

    int _currentTarifa;
    ToolBarButton toolButton;
    QAction menuButton;
};

#endif // FRMCAJAMINUTA_H
