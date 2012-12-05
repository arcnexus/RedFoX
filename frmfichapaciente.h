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
#include "episodio.h"

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
    void cargarEpisodio(int control);
    void guardarDatosPaciente();
public slots:
    void AnadirDatosMedicamento(int id, QString nombre,QString codigo_nacional);
    void cargarDatosMedicamento(int, int);

private slots:
    void finishedSlot(QNetworkReply* reply);
    void on_btnAgenda_clicked();
    void on_btnAnadirEpisodio_clicked();
    void on_btnBuscarCIEEpisodio_clicked();
    void on_btnEditarPaciente_clicked();
    void on_btnGuardarPaciente_clicked();
    void on_btnGuardarEpisodio_clicked();
    void on_listaEpisodios_clicked(const QModelIndex &index);
    void on_btnEditarEpisodio_clicked();
    void on_btnDeshacerEpisodio_clicked();
    void on_btnAnadirFarma_clicked();
    void BuscarCIE();
    void RecuperarCIE(int,QString,QString);
    void ActivarControlesFarmacologia();
    void GuardarDatosFarmacologia();
    void llenartablahistorialfarmacologiaepisodio();
    void BorrarDatosMedicamento();
    void MostrarFichaMedicamento();
    void AnadirImagenDiagnostico();
signals:
    void pasaid(int);
    void pasaCodigoNacional(QString);

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
    void LLenarEpisodio();
};

#endif // FRMFICHAPACIENTE_H
