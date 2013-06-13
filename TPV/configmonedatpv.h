#ifndef CONFIGMONEDATPV_H
#define CONFIGMONEDATPV_H

#include <QDialog>

namespace Ui {
class configMonedaTpv;
}

class configMonedaTpv : public QDialog
{
    Q_OBJECT
    
public:
    explicit configMonedaTpv(QWidget *parent = 0);
    ~configMonedaTpv();
    
private:
    Ui::configMonedaTpv *ui;
};

#endif // CONFIGMONEDATPV_H
