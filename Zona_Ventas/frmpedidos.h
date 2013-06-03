#ifndef FRMPEDIDOS_H
#define FRMPEDIDOS_H

#include "../Auxiliares/Globlal_Include.h"

#include "pedidos.h"
#include "../Zona_Pacientes/cliente.h"
#include "../Auxiliares/table_helper.h"
namespace Ui {
class frmPedidos;
}

class FrmPedidos : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmPedidos(QWidget *parent = 0);
    ~FrmPedidos();
    Pedidos *oPedido;
    Cliente *oCliente3;
signals:
    void block();
    void unblock();
private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnadir_clicked();

    void on_btnAnterior_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnDeshacer_clicked();

    void on_btn_borrar_clicked();

    void totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
    void lineaReady(lineaDetalle*);

    void convertir_enAlbaran();
    void convertir_enFactura();
private:
    Ui::frmPedidos *ui;

    void LLenarCampos();
    void LLenarCamposCliente();
    void VaciarCampos();
    void BloquearCampos(bool state);
    void LLenarPedido();

    Table_Helper helper;
    QAction * aAlbaran_action;
    QAction * aFactura_action;
    QMenu * convertir_menu;

    bool editando;
};

#endif // FRMPEDIDOS_H
