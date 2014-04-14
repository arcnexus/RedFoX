#ifndef FRMBUSCARCIE_H
#define FRMBUSCARCIE_H

#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmBuscarCIE;
}

class FrmBuscarCIE : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmBuscarCIE(QWidget *parent = 0);
    ~FrmBuscarCIE();
    
private:
    Ui::FrmBuscarCIE *ui;
private slots:
    void sltBuscarCie();
    void sltDevolverValores();
    void finishedSlotCIE(QNetworkReply*);
signals:
    void emitirCIE(int,QString,QString );
};

#endif // FRMBUSCARCIE_H
