#ifndef FRMANADIRMEDICAMENTO_H
#define FRMANADIRMEDICAMENTO_H

#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmAnadirMedicamento;
}

class FrmAnadirMedicamento : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAnadirMedicamento(QWidget *parent = 0);
    ~FrmAnadirMedicamento();
    int id_medicamento;
    QString cRetMedicamento;
    QStringList cMedicList;

signals:
    void datos(int, QString,QString);

private slots:
    void on_pushButton_clicked();
    void finishedSlotMedicamento(QNetworkReply* reply);
    void finishedSlotBuscarMedicamentoPorPatologia(QNetworkReply* reply);
    void finishedSlotBuscarPatologia(QNetworkReply* reply);
    void finishedSlotBuscarProspecto(QNetworkReply *reply);
    void finishedSlotBuscarMolecula(QNetworkReply *reply);
    void finishedSlotBuscarMedicamentoPorMolecula(QNetworkReply *reply);
    void finishedSlotBuscarProspectoMolecula(QNetworkReply *reply);
    void on_btnBuscarMedicamentoporPatologia_clicked();
    void finishedSlotBuscarPrincipioActivo(QNetworkReply *reply);
    void finishedSlotBuscarMedicamentoPorPActivo(QNetworkReply *reply);
    void finishedSlotBuscarProspectoPrincAct(QNetworkReply *reply);
    void on_btnBuscarPatologia_clicked();
    void finishedSlotBuscarProspectoMedicamento(QNetworkReply *reply);
    void on_btnVerProspecto_clicked();

    void on_tablamedicamentosPatologia_clicked();

    void on_tablaPatologias_doubleClicked();

    void on_btnBuscarMolcula_clicked();

    void on_btnBuscarMedicamentoporMolecula_clicked();

    void on_tablamedicamentosMolecula_clicked();

    void on_btnBuscarPrincipiosActivos_clicked();

    void on_btnBuscarMedicamentoporPA_clicked();

    void on_tablamedicamentospactivos_clicked();

    void BuscarProspectoMedicamento(int, int);

    void on_btnAceptar1_clicked();
    void Aceptarporpatologia();
    void Aceptarpormolecula();
    void AceptarporPA();

private:
    Ui::FrmAnadirMedicamento *ui;
    QXmlStreamAttributes attrib;
};

#endif // FRMANADIRMEDICAMENTO_H
