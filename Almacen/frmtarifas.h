#ifndef FRMTARIFAS_H
#define FRMTARIFAS_H

#include <QDialog>

namespace Ui {
class FrmTarifas;
}

class FrmTarifas : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmTarifas(QWidget *parent = 0);
    ~FrmTarifas();
    
private:
    Ui::FrmTarifas *ui;
};

#endif // FRMTARIFAS_H
