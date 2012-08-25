#ifndef FRMFACTURAS_H
#define FRMFACTURAS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QItemDelegate>
#include "configuracion.h"

namespace Ui {
class frmFacturas;
}

class frmFacturas : public QDialog
{
    Q_OBJECT
    
public slots:
    void LLenarCampos();
    void LLenarFactura();
    void VaciarCampos();
    void BloquearCampos();
    void DesbloquearCampos();

public:
    explicit frmFacturas(Configuracion *o_config, QWidget *parent = 0);
    ~frmFacturas();
    
private slots:
    void lineasVentas();

    void on_btnSiguiente_clicked();

    void on_btnGuardar_clicked();

    void on_btnAnadir_clicked();

    //void on_AnadirLinea_clicked();

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

private:
    Ui::frmFacturas *ui;
    QSqlDatabase dbEmp;
    QSqlDatabase dbTerra;
    QSqlQueryModel *modelFP;
    bool Altas;
    QSqlQueryModel *ModelLin_fac;
    QHeaderView *Cabecera;
    double importe;
    Configuracion *o_configuracion;
};

class ColumnaGrid : public QItemDelegate
{
    Q_OBJECT

public:

    ColumnaGrid(QObject *parent);

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};
#endif // FRMFACTURAS_H
