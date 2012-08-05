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

    void on_btnSiguiente_clicked();

    void on_btnGuardar_clicked();

    void on_btnAnadir_clicked();

    void on_AnadirLinea_clicked();

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
