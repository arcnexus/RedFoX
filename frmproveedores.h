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

private:
    Ui::frmProveedores *ui;
};

#endif // FRMPROVEEDORES_H
