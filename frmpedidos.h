#ifndef FRMPEDIDOS_H
#define FRMPEDIDOS_H

#include <QDialog>
#include "configuracion.h"
# include <QSqlQueryModel>
#include "pedidos.h"
#include "Zona_Pacientes/cliente.h"
#include "Auxiliares/table_helper.h"
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
private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnadir_clicked();

    void on_btnAnterior_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnDeshacer_clicked();

    void on_pushButton_clicked();

    void totalChanged(QString total);
private:
    Ui::frmPedidos *ui;
    Configuracion *o_configuracion;
    QSqlQueryModel *ModelLin_ped;

    void lineasVentas();
    void LLenarCampos();
    void LLenarCamposCliente();
    void VaciarCampos();
    void BloquearCampos();
    void DesbloquearCampos();
    void LLenarPedido();
    void CalcularTotalLinea();
    void RellenarDespuesCalculo();

    Table_Helper helper;
};

#endif // FRMPEDIDOS_H
