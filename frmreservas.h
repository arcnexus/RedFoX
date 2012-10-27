#ifndef FRMRESERVAS_H
#define FRMRESERVAS_H

#include <QDialog>

namespace Ui {
class FrmReservas;
}

class FrmReservas : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmReservas(QWidget *parent = 0);
    ~FrmReservas();
    
private:
    Ui::FrmReservas *ui;
};

#endif // FRMRESERVAS_H
