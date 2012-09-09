#ifndef FRMARTICULOS_H
#define FRMARTICULOS_H

#include <QDialog>
#include "configuracion.h"

namespace Ui {
class FrmArticulos;
}

class FrmArticulos : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmArticulos(Configuracion *o_config, QWidget *parent = 0);
    ~FrmArticulos();

    
private slots:
    void on_botSiguiente_clicked();

    void on_botAnterior_clicked();

    void on_botAnadir_clicked();

    void on_botEditar_clicked();

    void on_botBorrar_clicked();

    void on_botGuardar_clicked();

    void on_botCambiarImagen_clicked();

    void on_pushButton_clicked();

    void on_botDeshacer_clicked();

    void on_txtrTarifa1_lostFocus();

    void on_txtrCoste_lostFocus();

    void on_txtrTarifa1_2_lostFocus();

    void on_txtrTarifa2_lostFocus();

    void on_txtrTarifa3_lostFocus();

private:
    Ui::FrmArticulos *ui;
    bool Altas;
    void desbloquearCampos();
    void bloquearCampos();
    void LLenarCampos();
    void CargarCamposEnArticulo();
    void VaciarCampos();
};

#endif // FRMARTICULOS_H
