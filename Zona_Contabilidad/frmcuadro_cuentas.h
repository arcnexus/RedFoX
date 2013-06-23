#ifndef FRMCUADRO_CUENTAS_H
#define FRMCUADRO_CUENTAS_H

#include <QDialog>

namespace Ui {
class frmCuadro_cuentas;
}

class frmCuadro_cuentas : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmCuadro_cuentas(QWidget *parent = 0);
    ~frmCuadro_cuentas();
    
private:
    Ui::frmCuadro_cuentas *ui;
};

#endif // FRMCUADRO_CUENTAS_H
