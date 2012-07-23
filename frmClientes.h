#ifndef FRMCLIENTES_H
#define FRMCLIENTES_H


#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtSql>
#include "frmbuscarpoblacion.h"


namespace Ui {
class frmClientes;
}

class frmClientes : public QDialog
{
    Q_OBJECT

public:
    explicit frmClientes(QWidget *parent = 0);
    ~frmClientes();
    bool Altas;

public slots:
    void LLenarCampos();
    void LLenarCliente();
    void VaciarCampos();
   // void SetId_Cliente(int id_Cliente);


private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

    void on_btnGuardar_clicked();

    void on_btnAnadir_clicked();

    void on_txtPrimerApellido_lostFocus();

    void on_txtSegundoApellido_lostFocus();

    void on_txtcNombre_lostFocus();

    void on_txtcPoblacion_lostFocus();

    void on_txtcProvincia_lostFocus();

    void on_txtcCifNif_lostFocus();

    void on_btnEditar_clicked();
    void bloquearCampos();
    void desbloquearCampos();

    void on_btnDeshacer_clicked();

    void on_btnBorrar_clicked();

    void on_btnBuscar_clicked();

    void on_txtcCp_lostFocus();

private:
    Ui::frmClientes *ui;
    QSqlQueryModel *modelFP;
    QSqlQueryModel *modelFacturas;
    QSqlQueryModel *modelPoblaciones;
    QSqlDatabase dbCliente;
    FrmBuscarPoblacion BuscarPoblacion;

};
#endif
