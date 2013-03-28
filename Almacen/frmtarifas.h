#ifndef FRMTARIFAS_H
#define FRMTARIFAS_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
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
    double pvp;
    int id_pais;
    int id_moneda;
    QString codigoTarifa;
    float coste;
    void capturar_coste(float);

private:
    Ui::FrmTarifas *ui;
private slots:
    void cargarDatosTarifa(QModelIndex);
    void asignarcambiodivisa(float);
    void valorar_en_local();

};

#endif // FRMTARIFAS_H
