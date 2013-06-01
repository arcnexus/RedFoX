#ifndef FRMTARIFAS_H
#define FRMTARIFAS_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
namespace Ui {
class FrmTarifas;
}

class FrmTarifas : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmTarifas(QWidget *parent = 0);
    ~FrmTarifas();
    int id_tarifa;
    double margen;
    double margen_min;
    double pvpDivisa;
    int id_pais;
    int id_moneda;
    QString codigoTarifa;
    QString cod_divisa;
    float coste;
    void capturar_coste(float);
    void capturar_datos(int id,QString coste);

private:
    Ui::FrmTarifas *ui;
private slots:
    void cargarDatosTarifa(QModelIndex);
    void asignarcambiodivisa(float);
    void calcular_precio(double margen);
    void aceptar();


    void on_txtPVPLocal_editingFinished();
};

#endif // FRMTARIFAS_H
