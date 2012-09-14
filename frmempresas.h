#ifndef FRMEMPRESAS_H
#define FRMEMPRESAS_H

#include <QDialog>
#include "empresa.h"
namespace Ui {
class FrmEmpresas;
}

class FrmEmpresas : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmEmpresas(QWidget *parent = 0);
    ~FrmEmpresas();
    void LLenarCampos();
    void CargarCamposEnEmpresa();
private slots:
    void on_pushButton_clicked();

    void on_botSiguiente_clicked();

    void on_botAnterior_clicked();

    void on_botGuardar_clicked();

private:
    Ui::FrmEmpresas *ui;
    Empresa oEmpresa;


};

#endif // FRMEMPRESAS_H
