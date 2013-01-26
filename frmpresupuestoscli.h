#ifndef FRMPRESUPUESTOSCLI_H
#define FRMPRESUPUESTOSCLI_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QSqlQueryModel>
#include "presupuesto.h"
#include "Zona_Pacientes/cliente.h"

namespace Ui {
class FrmPresupuestosCli;
}

class FrmPresupuestosCli : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmPresupuestosCli(QWidget *parent = 0);
    ~FrmPresupuestosCli();
    void LLenarCampos();
    void LLenarCamposCliente();
    void LLenarPresupuesto();
    void VaciarCampos();
    void BloquearCampos();
    void DesbloquearCampos();
    void CalcularTotalLinea();
    void RellenarDespuesCalculo();
    void LineasVentas();
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

    void on_txtcCodigoArticulo_editingFinished();

    void on_btnAnadirLinea_clicked();

    void on_botEditarLinea_clicked();

    void on_botBorrarLinea_clicked();

private:
    Ui::FrmPresupuestosCli *ui;
    QSqlQueryModel *model;
    QSortFilterProxyModel *proxyModel;
    QTableView *tableView;
    QSqlQueryModel *ModelLin_pre;
};

#endif // FRMPRESUPUESTOSCLI_H
