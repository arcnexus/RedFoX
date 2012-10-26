#ifndef FRMFORMPAGO_H
#define FRMFORMPAGO_H

#include <QDialog>

namespace Ui {
class FrmFormPago;
}

class FrmFormPago : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmFormPago(QWidget *parent = 0);
    ~FrmFormPago();
    
private:
    Ui::FrmFormPago *ui;
};

#endif // FRMFORMPAGO_H
