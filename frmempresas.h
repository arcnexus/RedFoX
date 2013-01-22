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
    //TODO - boton buscar
    //TODO - boton borrar
    //TODO - boton añadir

    void on_botSiguiente_clicked();

    void on_botAnterior_clicked();

    void on_botGuardar_clicked();

    void txtcPoblacion_editingFinished();

    void txtcCp_editingFinished();
    void on_botAnadir_clicked();

    void on_btn_ruta_db_clicked();

private:
    Ui::FrmEmpresas *ui;
    Empresa oEmpresa;
    void limpiar_campos();
   bool crear_empresa_sqlite();
   bool crear_medica_sqlite();
   bool crear_empresa_mysql();
   bool crear_medica_mysql();
};

#endif // FRMEMPRESAS_H
