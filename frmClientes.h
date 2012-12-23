#ifndef FRMCLIENTES_H
#define FRMCLIENTES_H


#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtSql>
#include "frmbuscarpoblacion.h"
#include "configuracion.h"


namespace Ui {
class frmClientes;
}

class frmClientes : public QDialog
{
    Q_OBJECT

public:
    explicit frmClientes(Configuracion *oConfiguracion, QWidget *parent = 0);
    ~frmClientes();
    bool Altas;

public slots:
    void LLenarCampos();
    void LLenarCliente();
    void VaciarCampos();
   // void SetId_Cliente(int id_Cliente);

signals:
    void enviahistoriaynombre(int,QString);

private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

    void on_btnGuardar_clicked();

    void on_btnAnadir_clicked();

    void on_txtPrimerApellido_editingFinished();

    void on_txtSegundoApellido_editingFinished();

    void on_txtcNombre_editingFinished();

    void on_txtcPoblacion_editingFinished();

    void on_txtcProvincia_editingFinished();

    void on_txtcCifNif_editingFinished();

    void on_btnEditar_clicked();
    void bloquearCampos();
    void desbloquearCampos();

    void on_btnDeshacer_clicked();

    void on_btnBorrar_clicked();

    void on_btnBuscar_clicked();

    void on_txtcCp_editingFinished();

    void on_txtcCPFactura_editingFinished();

    void on_txtcPoblacionFactura_editingFinished();

    void on_txtcCpPoblacionAlmacen_editingFinished();

    void on_txtcPoblacionAlmacen_editingFinished();

    void on_TablaDeudas_clicked(const QModelIndex &index);

    void on_txtrRiesgoPermitido_editingFinished();

    void on_btnFichaPaciente_clicked();

private:
    Ui::frmClientes *ui;
    QSqlQueryModel *modelFP;
    QSqlQueryModel *modelFacturas;
    QSqlQueryModel *modelPoblaciones;
    QSqlDatabase dbCliente;
    FrmBuscarPoblacion BuscarPoblacion;
    QSqlQuery tbpaciente;


};
#endif
