#include "vademecum.h"
#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QUrl>
#include <QDebug>


QString cXML;
vademecum::vademecum(QObject *parent) :
    QObject(parent)
{
}

QString vademecum::recuperar(QString cUrl)
{
    // Recupero valores conexión Vademecum
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(finishedSlot(QNetworkReply*)));
    cUrl = cUrl + "&id_ent=" + cClave1;
        manager->get(QNetworkRequest(QUrl(cUrl)));
    QString dato = cXML;
    return dato;

}

QStringList vademecum::recuperarLista(QString cUrl)
{
        // Recupero valores conexión Vademecum
        QSettings settings("infint", "terra");
        QString cClave1 = settings.value("Clave1").toString();
        QString cClave2 = settings.value("Clave2").toString();

    //    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    //        connect(manager, SIGNAL(finished(QNetworkReply*)),
    //                this, SLOT(finishedSlot(QNetworkReply*)));
        cUrl = cUrl + "&id_ent=" + cClave1;
    //        manager->get(QNetworkRequest(QUrl(cUrl)));


            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            connect(manager, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(finishedSlot(QNetworkReply*)));
           manager->get(QNetworkRequest(QUrl(cUrl)));
    // procesar cXML
           qDebug() << cXML;
    QStringList cDatos;
    // Devolver Resultado

    return cDatos;


}

void vademecum::finishedSlot(QNetworkReply* reply)
{
    //qDebug()<<reply->readAll();
     QString data=(QString)reply->readAll();
     cXML = data;
}
