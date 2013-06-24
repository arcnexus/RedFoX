#ifndef FRMCUADRO_CUENTAS_H
#define FRMCUADRO_CUENTAS_H

#include <QDialog>
#include"../Zona_Contabilidad/cuentas_contables.h"

namespace Ui {
class frmCuadro_cuentas;
}

class frmCuadro_cuentas : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmCuadro_cuentas(QWidget *parent = 0);
    ~frmCuadro_cuentas();
    Cuentas_contables *cuentas;
    
private slots:
    void on_btnAnadir_clicked();
    void llenar_campos();
    void llenar_objeto();
    void activar_controles(bool state);

    void on_btnguardar_clicked();
    void llenar_tabla();

private:
    Ui::frmCuadro_cuentas *ui;
    bool anadir;
};

#endif // FRMCUADRO_CUENTAS_H
