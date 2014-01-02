#ifndef FRMCIERRECAJA_H
#define FRMCIERRECAJA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../TPV/frmcajasabiertas.h"
namespace Ui {
class FrmCierreCaja;
}

class FrmCierreCaja : public QDialog
{
    Q_OBJECT

public:
    explicit FrmCierreCaja(QWidget *parent = 0);
    ~FrmCierreCaja();
    void cargar_datos_caja(int id);
private slots:

    void on_btnIniciar_cierre_clicked();

    void on_btnCierreParcial_clicked();

    void on_spinmoneda1_valueChanged(int arg1);

    void on_spinmoneda2_valueChanged(int arg1);

    void on_spinmoneda3_valueChanged(int arg1);

    void on_spinmoneda4_valueChanged(int arg1);

    void on_spinmoneda5_valueChanged(int arg1);

    void on_spinmoneda6_valueChanged(int arg1);

    void on_spinmoneda7_valueChanged(int arg1);

    void on_spinmoneda8_valueChanged(int arg1);

    void on_spinmoneda9_valueChanged(int arg1);

    void on_spinmoneda10_valueChanged(int arg1);

    void on_spinmoneda11_valueChanged(int arg1);

    void on_spinmoneda12_valueChanged(int arg1);

    void on_spinmoneda13_valueChanged(int arg1);

    void on_spinmoneda14_valueChanged(int arg1);

    void on_spinmoneda15_valueChanged(int arg1);

    void on_btnResumentickets_clicked();

    void on_btnResumenEntradas_clicked();

    void on_btnResumenSalidas_clicked();

    void on_btnResumenExtras_clicked();

    void on_btnResmenFacturas_clicked();

private:
    Ui::FrmCierreCaja *ui;
    bool facturas_en_cierre;
    double importe_fijo_cierre;
    void cargarmoneda();
    void totalmonedas();

};

#endif // FRMCIERRECAJA_H
