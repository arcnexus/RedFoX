#ifndef FRMALBARAN_H
#define FRMALBARAN_H

#include <QDialog>
#include <QHeaderView>
#include <QSqlQueryModel>
#include "frmbuscarcliente.h"

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
private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnadir_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnAnterior_clicked();

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

private:
    Ui::FrmAlbaran *ui;
    FrmBuscarCliente *BuscarClientes;
};

#endif // FRMALBARAN_H
