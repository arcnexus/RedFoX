#ifndef FRMINFORMACIONFARMACO_H
#define FRMINFORMACIONFARMACO_H

#include <QDialog>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace Ui {
class FrmInformacionFarmaco;
}

class FrmInformacionFarmaco : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmInformacionFarmaco(QWidget *parent = 0);
    ~FrmInformacionFarmaco();
    
private:
    Ui::FrmInformacionFarmaco *ui;
private slots:
    void capturarid(QString);
    void finishedSlotBuscarMedicamento(QNetworkReply* reply);
};

#endif // FRMINFORMACIONFARMACO_H
