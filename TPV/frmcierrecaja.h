#ifndef FRMCIERRECAJA_H
#define FRMCIERRECAJA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

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
    void on_btnAceptar_clicked();


    void on_btnIniciar_cierre_clicked();

private:
    Ui::FrmCierreCaja *ui;
    bool facturas_en_cierre;
    double importe_fijo_cierre;
};

#endif // FRMCIERRECAJA_H
