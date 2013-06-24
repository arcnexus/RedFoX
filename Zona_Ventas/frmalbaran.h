#ifndef FRMALBARAN_H
#define FRMALBARAN_H

#include "../Auxiliares/Globlal_Include.h"
#include "../Busquedas/frmbuscarcliente.h"
//
#include "../Auxiliares/table_helper.h"
#include "../mayamodule.h"
namespace Ui {
class FrmAlbaran;
}

class FrmAlbaran : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmAlbaran(QWidget *parent = 0);
    ~FrmAlbaran();
    void LLenarCampos();
    void LLenarCamposCliente();
    void VaciarCampos();
    void BloquearCampos(bool state);
    void DesbloquearCampos();
    void LLenarAlbaran();

    moduleZone ModuleZone(){return Ventas;}
    QString ModuleName(){return "Albaranes Ventas";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
signals:

private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnadir_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnAnterior_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_btn_borrar_clicked();

    void totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);

    void lineaReady(lineaDetalle*);
    void lineaDeleted(lineaDetalle*);
private:
    Ui::FrmAlbaran *ui;
    FrmBuscarCliente *BuscarClientes;
    QSqlQueryModel *ModelLin_alb;
    Table_Helper helper;
    bool in_edit;

    ToolBarButton toolButton;
    QAction menuButton;
};

#endif // FRMALBARAN_H
