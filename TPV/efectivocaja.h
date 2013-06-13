#ifndef EFECTIVOCAJA_H
#define EFECTIVOCAJA_H

#include <QDialog>

namespace Ui {
class EfectivoCaja;
}

class EfectivoCaja : public QDialog
{
    Q_OBJECT
    
public:
    explicit EfectivoCaja(QWidget *parent = 0);
    ~EfectivoCaja();
    
private:
    Ui::EfectivoCaja *ui;
};

#endif // EFECTIVOCAJA_H
