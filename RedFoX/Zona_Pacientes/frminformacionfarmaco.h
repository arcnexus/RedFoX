#ifndef FRMINFORMACIONFARMACO_H
#define FRMINFORMACIONFARMACO_H

#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmInformacionFarmaco;
}

class FrmInformacionFarmaco : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmInformacionFarmaco(QWidget *parent = 0);
    ~FrmInformacionFarmaco();
    
public slots:
    void capturarid(QString);
private:
    Ui::FrmInformacionFarmaco *ui;
private slots:

    void finishedSlotBuscarMedicamento(QNetworkReply* reply);
    void cargaFinalizada1(bool estado);
    void cargaFinalizada2(bool estado);
    void cargaFinalizada3(bool estado);
    void cargaFinalizadaLogo(bool estado);
};

#endif // FRMINFORMACIONFARMACO_H
