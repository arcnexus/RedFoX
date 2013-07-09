#ifndef FRMTIPOSTARIFA_H
#define FRMTIPOSTARIFA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../Almacen/tipostarifa.h"

namespace Ui {
class FrmTiposTarifa;
}

class FrmTiposTarifa : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmTiposTarifa(QWidget *parent = 0);
    ~FrmTiposTarifa();
    void cargarDatos();
    
private slots:
    void on_btnAgregarTarifa_clicked();

    void on_listatarifas_clicked(const QModelIndex &index);

    void on_btnAnadir_clicked();

    void activar_controles(bool state);

    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();

    void on_btnDeshacer_clicked();

    void llenar_lista();

    void cargar_datos_en_objeto();
    void asignarcambiodivisa(float valor);

private:
    Ui::FrmTiposTarifa *ui;
    TiposTarifa *oTipostarifa;

};

#endif // FRMTIPOSTARIFA_H
