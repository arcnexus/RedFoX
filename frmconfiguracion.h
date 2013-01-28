#ifndef FRMCONFIGURACION_H
#define FRMCONFIGURACION_H

#include "Auxiliares/Globlal_Include.h"


namespace Ui {
class FrmConfiguracion;
}

class FrmConfiguracion : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmConfiguracion(QWidget *parent = 0);
    ~FrmConfiguracion();
    
private slots:
    void on_btnGuardar_clicked();

private:
    Ui::FrmConfiguracion *ui;
    QSettings settings;
};

#endif // FRMCONFIGURACION_H
