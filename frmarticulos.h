#ifndef FRMARTICULOS_H
#define FRMARTICULOS_H

#include <QDialog>
#include "configuracion.h"

namespace Ui {
class FrmArticulos;
}

class FrmArticulos : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmArticulos(Configuracion *o_config, QWidget *parent = 0);
    ~FrmArticulos();
    
private slots:
    void on_botSiguiente_clicked();

private:
    Ui::FrmArticulos *ui;
    void LLenarCampos();
};

#endif // FRMARTICULOS_H
