#ifndef FRMANADIRMEDICAMENTO_H
#define FRMANADIRMEDICAMENTO_H

#include <QWidget>
#include <QNetworkReply>
#include <QXmlStreamAttributes>

namespace Ui {
class FrmAnadirMedicamento;
}

class FrmAnadirMedicamento : public QWidget
{
    Q_OBJECT
    
public:
    explicit FrmAnadirMedicamento(QWidget *parent = 0);
    ~FrmAnadirMedicamento();
    
private slots:
    void on_pushButton_clicked();
    void finishedSlotMedicamento(QNetworkReply* reply);
    void finishedSlotBuscarMedicamentoPorPatologia(QNetworkReply* reply);
    void finishedSlotBuscarPatologia(QNetworkReply* reply);


    void on_btnBuscarMedicamentoporPatologia_clicked();

    void on_btnBuscarPatologia_clicked();

private:
    Ui::FrmAnadirMedicamento *ui;
    QXmlStreamAttributes attrib;
};

#endif // FRMANADIRMEDICAMENTO_H
