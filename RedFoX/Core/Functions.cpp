#include "Functions.h"
#include <QApplication>
#include <QLabel>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QThread>
#include <QMessageBox>
#include "../mainwindow.h"

void RedFoX::Core::Functions::loadImage(QLabel *label, QString url)
{
    Q_ASSERT(label);
    if(url.isEmpty())
        return;    
    QFile file(url);
    if(file.exists() && file.open(QFile::ReadOnly))
    {
        {
            QByteArray imgData = file.readAll();
            QPixmap pixmap;
            pixmap.loadFromData(imgData);
            label->setPixmap(pixmap);
            label->repaint();
        }
    }
    else if(QUrl(url).isValid())
    {
        QNetworkAccessManager * nam = new QNetworkAccessManager;

        QObject::connect(nam,&QNetworkAccessManager::finished, [nam,label](QNetworkReply * reply){

            if (reply->error() != QNetworkReply::NoError) {
                //TODO default image
                return;
            }

            QVariant attribute = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            if (attribute.isValid())
            {
                QUrl url = attribute.toUrl();
                nam->get(QNetworkRequest(url));
                return;
            }
            QByteArray imgData = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(imgData);
            label->setPixmap(pixmap);
            label->repaint();

            nam->deleteLater();
            reply->deleteLater();
        });
        nam->get(QNetworkRequest(QUrl(url)));
    }
}




QString RedFoX::Core::Functions::uploadImage(QString imgFile,QString folder, QString server, QString user, QString password,int port,
                                           RedFoX::Core::Functions::UploadMetod metod)
{
    QFile file(imgFile);
    if(file.exists() && file.open(QFile::ReadOnly))
    {
        QFileInfo info(file.fileName());
        QString filename = info.fileName();;

        if(metod == RedFoX::Core::Functions::LOCAL)
        {
            QFile dest(QString("%1/%2").arg(folder).arg(filename));
            if(dest.exists())
            {
                if(!dest.remove())
                {
                    QMessageBox::critical(qApp->activeWindow(),QObject::tr("Error copiando archivo"),dest.errorString());
                    return "";
                }
            }

            if(!file.copy(QString("%1/%2").arg(folder).arg(filename)))
            {
                QMessageBox::critical(qApp->activeWindow(),QObject::tr("Error copiando archivo"),file.errorString());
                return "";
            }
            else
            {
                return QString("%1/%2").arg(folder).arg(filename);
            }
        }
        else
        {
            QByteArray imgData = file.readAll();
            QNetworkAccessManager * nam = new QNetworkAccessManager;

            QObject::connect(nam,&QNetworkAccessManager::finished, [nam](QNetworkReply * reply){
                if(reply->error() != QNetworkReply::NoError)
                    QMessageBox::critical(qApp->activeWindow(),QObject::tr("Error copiando archivo"),reply->errorString());

                nam->deleteLater();
                reply->deleteLater();
            });

            QUrl url(QString("ftp://%1/%2%3").arg(server).arg(folder + "/").arg(filename));
            url.setUserName(user);
            url.setPassword(password);
            url.setPort(port);

            QNetworkReply * reply = nam->put(QNetworkRequest(url), imgData);

            QObject::connect(reply, &QNetworkReply::uploadProgress, [](qint64 sent, qint64 total){
                qDebug() << QString("%1 of %2").arg(sent).arg(total);
            });

            return QString("http://%1/static/%2/%3").arg(server).arg(folder).arg(filename);
        }
    }
}


QString RedFoX::Core::Functions::uploadImage(QString imgFile , QString target_folder)
{
    RedFoX::Core::Functions::UploadMetod metod =
            (Configuracion_global->groupIMGJson.value("metod").toString() == "FTP")
            ? RedFoX::Core::Functions::FTP : RedFoX::Core::Functions::LOCAL;

    if(metod == RedFoX::Core::Functions::LOCAL)
    {
        QString folder = Configuracion_global->groupIMGJson.value("path").toString();
        if(!target_folder.isEmpty())
            folder.append(QString("/%1").arg(target_folder));
        return uploadImage(imgFile, folder, "", "", "",0, metod);
    }
    else
    {
        QString host = Configuracion_global->groupIMGJson.value("host").toString();
        QString user = Configuracion_global->groupIMGJson.value("user").toString();
        QString pass = Configuracion_global->groupIMGJson.value("pass").toString();
        int port = Configuracion_global->groupIMGJson.value("port").toInt();
        QString folder = Configuracion_global->groupIMGJson.value("folder").toString();
        if(!target_folder.isEmpty())
            folder.append(QString("/%1").arg(target_folder));
        return uploadImage(imgFile, folder, host, user,pass,port, metod);
    }
}


QPixmap RedFoX::Core::Functions::getPixmap(QString url)
{
    if(url.isEmpty())
        return QPixmap();
    QFile file(url);
    if(file.exists() && file.open(QFile::ReadOnly))
    {
        {
            QByteArray imgData = file.readAll();
            QPixmap pixmap;
            pixmap.loadFromData(imgData);
            return pixmap;
        }
    }
    else if(QUrl(url).isValid())
    {
        QNetworkAccessManager * nam = new QNetworkAccessManager;
        bool wait = true;
        QPixmap pixmap;
        QObject::connect(nam,&QNetworkAccessManager::finished, [&pixmap,&wait,nam](QNetworkReply * reply){

            if (reply->error() != QNetworkReply::NoError) {
                wait = false;
                return;
            }

            QVariant attribute = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            if (attribute.isValid())
            {
                QUrl url = attribute.toUrl();
                nam->get(QNetworkRequest(url));
                return;
            }
            QByteArray imgData = reply->readAll();
            pixmap.loadFromData(imgData);

            nam->deleteLater();
            reply->deleteLater();
            wait = false;
        });
        nam->get(QNetworkRequest(QUrl(url)));
        while (wait)
        {
            qApp->processEvents();
            QThread::currentThread()->msleep(100);
        }
        return pixmap;
    }
}
