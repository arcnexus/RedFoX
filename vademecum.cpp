#include "vademecum.h"
#include <QString>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QUrl>


QString cXML;
vademecum::vademecum(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);
        // Creo la conexión al slot.
        connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(finishedSlot(QNetworkReply*)));
}

void vademecum::recuperar(QString cUrl)
{
    emit procesandoEvent(true,false,"");
    // Recupero valores conexión (No los puedo liberar por no ser GNU la BD web que consulta)
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();
    cUrl = cUrl + "&id_ent=" + cClave1;
    manager->get(QNetworkRequest(QUrl(cUrl)));
}


void vademecum::finishedSlot(QNetworkReply* reply)
{
    //QString data=(QString)reply->readAll();error=(reply->error()!= QNetworkReply::NoError);
//    cXML = data;
//    QString replyString = QString(reply->readAll());
//    QString errorString=reply->errorString();
//    reply->deleteLater();
//    //int iMaxError=m_pref.maxError;
//    if (!error) {
//        emit procesandoEvent(false,false,data);
//    }else{
//        emit procesandoEvent(false,true,"");
 //   }
}
