#ifndef FRMPRESUPUESTOSCLI_H
#define FRMPRESUPUESTOSCLI_H

#include "Auxiliares/Globlal_Include.h"
#include "presupuesto.h"
#include "Zona_Pacientes/cliente.h"

#include "Auxiliares/table_helper.h"
#include <QTimer>
namespace Ui {
class FrmPresupuestosCli;
}

class FrmPresupuestosCli : public QDialog
{
    Q_OBJECT

signals:
    void block();
    void unblock();
public:
    explicit FrmPresupuestosCli(QWidget *parent = 0);
    ~FrmPresupuestosCli();
    void LLenarCampos();
    void LLenarCamposCliente();
    void LLenarPresupuesto();
    void VaciarCampos();
    void BloquearCampos(bool state);
    Presupuesto *oPres;
    Cliente *oClientePres;
private slots:
    void on_chklAprovado_stateChanged(int arg1);

    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

    void on_btnAnadir_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_btnBuscar_clicked();

    void on_botBuscarCliente_clicked();

    void totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
    void on_btnDeshacer_clicked();

    void convertir_enPedido();
    void convertir_enAlbaran();
    void convertir_enFactura();
private:
    Ui::FrmPresupuestosCli *ui;
    QAction * aPedido_action;
    QAction * aAlbaran_action;
    QAction * aFactura_action;
    QMenu * convertir_menu;
    Table_Helper helper;
    bool editando;
};

#endif // FRMPRESUPUESTOSCLI_H
