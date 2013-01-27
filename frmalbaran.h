#ifndef FRMALBARAN_H
#define FRMALBARAN_H

#include <QDialog>
#include <QHeaderView>
#include <QSqlQueryModel>
#include "Busquedas/frmbuscarcliente.h"
#include "configuracion.h"
#include "Auxiliares/table_helper.h"
namespace Ui {
class FrmAlbaran;
}

class FrmAlbaran : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAlbaran(QWidget *parent = 0);
    ~FrmAlbaran();
    void lineasVentas();
    void LLenarCampos();
    void LLenarCamposCliente();
    void VaciarCampos();
    void BloquearCampos();
    void DesbloquearCampos();
    void LLenarAlbaran();
    void calcularTotalLinea();
    void RellenarDespuesCalculo();
private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnadir_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnAnterior_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_txtcCodigoArticulo_editingFinished();

    void on_txtcCantidadArticulo_editingFinished();

    void on_txtPVPArticulo_editingFinished();

    void on_txtPorcDtoArticulo_editingFinished();


    void on_btnAnadirLinea_clicked();

    void on_btnDeshacer_clicked();

    void on_pushButton_clicked();

    void on_botEditarLinea_clicked();

    void on_botBorrarLinea_clicked();

    void totalChanged(QString total);
private:
    Ui::FrmAlbaran *ui;
    FrmBuscarCliente *BuscarClientes;
    Configuracion *o_configuracion;
    QSqlQueryModel *ModelLin_alb;
    Table_Helper helper;
};

#endif // FRMALBARAN_H
