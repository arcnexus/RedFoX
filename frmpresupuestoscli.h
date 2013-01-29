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
    
public:
    explicit FrmPresupuestosCli(QWidget *parent = 0);
    ~FrmPresupuestosCli();
    void LLenarCampos();
    void LLenarCamposCliente();
    void LLenarPresupuesto();
    void VaciarCampos();
    void BloquearCampos(bool state);
    void RellenarDespuesCalculo();
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

    void totalChanged(QString total);
    void on_btnDeshacer_clicked();

private:
    Ui::FrmPresupuestosCli *ui;
    QSqlQueryModel *model;
    QSortFilterProxyModel *proxyModel;
    QTableView *tableView;
    QSqlQueryModel *ModelLin_pre;

    Table_Helper helper;
};

#endif // FRMPRESUPUESTOSCLI_H
