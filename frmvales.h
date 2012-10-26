#ifndef FRMVALES_H
#define FRMVALES_H

#include <QDialog>

namespace Ui {
class FrmVales;
}

class FrmVales : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmVales(QWidget *parent = 0);
    ~FrmVales();
    
private:
    Ui::FrmVales *ui;
};

#endif // FRMVALES_H
