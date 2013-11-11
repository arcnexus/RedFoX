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

private:
    Ui::FrmCierreCaja *ui;
    bool facturas_en_cierre;
    double importe_fijo_cierre;
    void cargarmoneda();

};

#endif // FRMCIERRECAJA_H
