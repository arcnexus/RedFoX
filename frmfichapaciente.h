#ifndef FRMFICHAPACIENTE_H
#define FRMFICHAPACIENTE_H

#include <QDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>


namespace Ui {
class FrmFichaPaciente;
}

class FrmFichaPaciente : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmFichaPaciente(QWidget *parent = 0);
    ~FrmFichaPaciente();
    
private slots:
    void on_pushButton_3_clicked();
    void finishedSlot(QNetworkReply* reply);

private:
    Ui::FrmFichaPaciente *ui;
    QNetworkAccessManager* nam;
    //QXmlStreamReader *xmlReader;
    QXmlStreamAttributes attrib;
};

#endif // FRMFICHAPACIENTE_H
