#ifndef FRMGESTIONCOBROS2_H
#define FRMGESTIONCOBROS2_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmGestionCobros2;
}

class FrmGestionCobros2 : public QDialog
{
    Q_OBJECT

public:
    explicit FrmGestionCobros2(QWidget *parent = 0);
    ~FrmGestionCobros2();

    double getImporte() const;
    void setImporte(double value);
    struct valores
    {
        double efectivo;
        double transferencia;
        double tarjeta;
        double cheque;
        double internet;
        double pendiente;
    };

private slots:
    void on_btnAceptar_clicked();

    void on_txtEfectivo_editingFinished();

    void on_txtTransferencia_editingFinished();

    void on_txtTarjeta_editingFinished();

    void on_txtCheque_editingFinished();

    void on_txtInternet_editingFinished();

    void calcular();

private:
    Ui::FrmGestionCobros2 *ui;
    double importe;


};

#endif // FRMGESTIONCOBROS2_H
