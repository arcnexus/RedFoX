#ifndef FRMPROVEEDORES_H
#define FRMPROVEEDORES_H

#include <QDialog>

namespace Ui {
class frmProveedores;
}

class frmProveedores : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmProveedores(QWidget *parent = 0);
    ~frmProveedores();
public slots:
    void DesbloquerCampos();
    void BloquearCampos();
    void LLenarCampos();
    void CargarCamposEnProveedor();
    
private slots:
    void on_btnSiguiente_clicked();

    void on_btnGuardar_clicked();

    void on_btnEditar_clicked();

    void on_btnAnadir_clicked();

    void on_btnAnterior_clicked();

    void on_txtcPoblacion_lostFocus();

    void on_txtcCP_lostFocus();

    void on_btnBorrar_clicked();

    void on_btnDeshacer_clicked();

    void on_txtcEntidadBancariaProveedor_lostFocus();

    void on_txtcOficinaBancariaProveedor_lostFocus();

    void on_txtcDCProveedor_lostFocus();

    void on_txtcCCProveedor_lostFocus();

    void on_txtcEntidadPagoProveedor_lostFocus();

    void on_txtcOficinaPagoProveedor_lostFocus();

    void on_txtcDCPagoProveedor_lostFocus();

    void on_txtcCuentaPagoProveedor_lostFocus();

    void on_txtcCPAlmacen_editingFinished();

private:
    Ui::frmProveedores *ui;
};

#endif // FRMPROVEEDORES_H
