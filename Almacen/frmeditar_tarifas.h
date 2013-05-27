#ifndef FRMEDITAR_TARIFAS_H
#define FRMEDITAR_TARIFAS_H

#include <QDialog>
#include <../Auxiliares/Globlal_Include.h>

namespace Ui {
class frmEditar_tarifas;
}

class frmEditar_tarifas : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmEditar_tarifas(QWidget *parent = 0);
    ~frmEditar_tarifas();

    void setFiltro(int id);

private:
    Ui::frmEditar_tarifas *ui;
};

#endif // FRMEDITAR_TARIFAS_H
