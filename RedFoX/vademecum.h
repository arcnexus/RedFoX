#ifndef VADEMECUM_H
#define VADEMECUM_H

#include "Auxiliares/Globlal_Include.h"

class vademecum : public QObject
{
    Q_OBJECT
public:
    explicit vademecum(QObject *parent = 0);
   // QString recuperar(QString cUrl);
    void recuperar(QString cUrl);
    QStringList recuperarLista(QString cUrl);

private:
    QNetworkAccessManager *manager;
    
signals:
    void procesandoEvent(bool bWait,bool bError,QString sValueXml);
    
public slots:

private slots:
    void finishedSlot(QNetworkReply* reply);
    
};

#endif // VADEMECUM_H
