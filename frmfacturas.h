#ifndef FRMFACTURAS_H
#define FRMFACTURAS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QItemDelegate>
#include "configuracion.h"
#include "factura.h"
#include "cliente.h"
namespace Ui {
class frmFacturas;
}

class frmFacturas : public QDialog
{
    Q_OBJECT
    
public slots:
    void LLenarCampos();
    void LLenarCamposCliente();
    void LLenarFactura();
    void VaciarCampos();
    void BloquearCampos();
    void DesbloquearCampos();

public:
    explicit frmFacturas(Configuracion *o_config, QWidget *parent = 0);
    ~frmFacturas();
    Factura *oFactura;
    Cliente *oCliente1;
    
private slots:
    void lineasVentas();

    void on_btnSiguiente_clicked();

    void on_btnGuardar_clicked();

    void on_btnAnadir_clicked();

    void on_btnEditar_clicked();

    void on_txtPVPArticulo_lostFocus();

    void on_txtcCodigoArticulo_lostFocus();

    void on_txtcCantidadArticulo_lostFocus();

    void calcularTotalLinea();

    void on_txtPorcDtoArticulo_lostFocus();

    void on_btnAnadirLinea_clicked();

    void on_btnDeshacer_clicked();

    void on_btnAnterior_clicked();

    void on_botRecalcular_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnBuscarArt_clicked();

    void on_tablaBuscaArt_doubleClicked(const QModelIndex &index);

    void on_chklRecargoEquivalencia_stateChanged(int arg1);

    void on_btnBuscar_clicked();


    void on_botEditarLinea_clicked();

    void on_txtcCodigoCliente_lostFocus();

    void on_btnImprimir_clicked();

    void RellenarDespuesCalculo();

    void on_botBorrarLinea_clicked();

    void on_botBorrador_clicked();

private:
    Ui::frmFacturas *ui;
    QSqlDatabase dbEmp;
    QSqlDatabase dbTerra;
    QSqlQueryModel *modelFP;
    QSqlQueryModel *modArt;
    bool Altas;
    QSqlQueryModel *ModelLin_fac;
    QHeaderView *Cabecera;
    double importe;
    Configuracion *o_configuracion;



public:
    QString TextoNumero(QString cTexto);

};

#endif // FRMFACTURAS_H
