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
    int id_producto;
    double margen;
    double margen_min;
    double pvpDivisa;
    int id_pais;
    int id_moneda;
    QString codigoTarifa;
    QString cod_divisa;
    float coste;
    float porc_dto1;
    float porc_dto2;
    float porc_dto3;
    float porc_dto4;
    float porc_dto5;
    float porc_dto6;



    void capturar_coste(float);
    void capturar_datos(int id,QString coste,int id_prod);


private:
    Ui::FrmTarifas *ui;
    QMap <int, QSqlRecord> volumenes;
    int id_volumen;
    QSqlQueryModel *modelVolumenes;

private slots:
    void cargarDatosTarifa(QModelIndex);
    void asignarcambiodivisa(float);
    void aceptar();


    void on_txtPVPLocal_editingFinished();
    void on_btnDesbloquearDivisa_clicked(bool checked);
    void on_spinMargen_editingFinished();
    void controles_volumenes(bool state);
    void on_btnAnadir_volumen_clicked();


    void on_btnEditar_volumen_clicked();

    void on_btnGuardar_volumen_clicked();

    void on_txtPrecio_volumen_editingFinished();

public slots:
    void calcular_precio(double margen);

};

#endif // FRMTARIFAS_H
