#ifndef FRMFICHAPACIENTE_H
#define FRMFICHAPACIENTE_H

#include <QDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>
#include <QtSql>
#include "paciente.h"


namespace Ui {
class FrmFichaPaciente;
}

class FrmFichaPaciente : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmFichaPaciente(QWidget *parent = 0);
    ~FrmFichaPaciente();
    void cargarDatos(int idcliente);
    void guardarDatosPaciente();
    
private slots:
    void finishedSlot(QNetworkReply* reply);


    void on_btnAgenda_clicked();

    void on_btnAnadirEpisodio_clicked();

    void on_btnBuscarCIE_clicked();

    void on_btnEditarPaciente_clicked();


    void on_btnGuardarPaciente_clicked();

private:
    Ui::FrmFichaPaciente *ui;
    QNetworkAccessManager* nam;
    //QXmlStreamReader *xmlReader;
    QXmlStreamAttributes attrib;
    QSqlQuery paciente;
    void BloquearCamposPaciente();
    void DesbloquearCamposPaciente();
    void BloquearCamposEpisodio();
    void DesbloquearCamposEpisodio();
};

#endif // FRMFICHAPACIENTE_H
