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
    void setImporte(double value, int id_);


    int getId_factura() const;
    void setId_factura(int value);

    int getId_ticket() const;
    void setId_ticket(int value);

private slots:
    void on_btnAceptar_clicked();

    void on_txtEfectivo_editingFinished();

    void on_txtTransferencia_editingFinished();

    void on_txtTarjeta_editingFinished();

    void on_txtCheque_editingFinished();

    void on_txtInternet_editingFinished();

    void calcular();

    void on_txtDomiciliacion_editingFinished();

private:
    Ui::FrmGestionCobros2 *ui;
    double importe;
    int id;
    int id_factura;
    int id_ticket;
    struct valores
    {
        double efectivo;
        double transferencia;
        double domiciliacion;
        double tarjeta;
        double cheque;
        double internet;
        double pendiente;
        double vale;
    };
    QString entidad,oficina,dc,cuenta;




};

#endif // FRMGESTIONCOBROS2_H
