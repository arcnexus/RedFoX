#ifndef FRMBUSCARCIE_H
#define FRMBUSCARCIE_H

#include <QWidget>

namespace Ui {
class FrmBuscarCIE;
}

class FrmBuscarCIE : public QWidget
{
    Q_OBJECT
    
public:
    explicit FrmBuscarCIE(QWidget *parent = 0);
    ~FrmBuscarCIE();
    
private:
    Ui::FrmBuscarCIE *ui;
};

#endif // FRMBUSCARCIE_H
