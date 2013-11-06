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

private slots:

private:
    Ui::FrmCierreCaja *ui;
    bool facturas_en_cierre;
    double importe_fijo_cierre;
    void cargar_datos_caja(int id);
};

#endif // FRMCIERRECAJA_H
