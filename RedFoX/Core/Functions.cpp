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
    {
        label->setPixmap(QPixmap());
        label->repaint();
        return;
    }
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
                label->setPixmap(QPixmap());
                label->repaint();
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
    return "";
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
    return QPixmap();
}


QString RedFoX::Core::Functions::slugifi(QString input)
{
    QMap<QString,QString> map;
    {
    map["À"] = "A";
    map["Á"] = "A";
    map["Â"] = "A";
    map["Ã"] = "A";
    map["Ä"] = "Ae";
    map["Å"] = "A";
    map["Æ"] = "A";
    map["Ā"] = "A";
    map["Ą"] = "A";
    map["Ă"] = "A";
    map["Ç"] = "C";
    map["Ć"] = "C";
    map["Č"] = "C";
    map["Ĉ"] = "C";
    map["Ċ"] = "C";
    map["Ď"] = "D";
    map["Đ"] = "D";
    map["È"] = "E";
    map["É"] = "E";
    map["Ê"] = "E";
    map["Ë"] = "E";
    map["Ē"] = "E";
    map["Ę"] = "E";
    map["Ě"] = "E";
    map["Ĕ"] = "E";
    map["Ė"] = "E";
    map["Ĝ"] = "G";
    map["Ğ"] = "G";
    map["Ġ"] = "G";
    map["Ģ"] = "G";
    map["Ĥ"] = "H";
    map["Ħ"] = "H";
    map["Ì"] = "I";
    map["Í"] = "I";
    map["Î"] = "I";
    map["Ï"] = "I";
    map["Ī"] = "I";
    map["Ĩ"] = "I";
    map["Ĭ"] = "I";
    map["Į"] = "I";
    map["İ"] = "I";
    map["Ĳ"] = "IJ";
    map["Ĵ"] = "J";
    map["Ķ"] = "K";
    map["Ľ"] = "K";
    map["Ĺ"] = "K";
    map["Ļ"] = "K";
    map["Ŀ"] = "K";
    map["Ł"] = "L";
    map["Ñ"] = "N";
    map["Ń"] = "N";
    map["Ň"] = "N";
    map["Ņ"] = "N";
    map["Ŋ"] = "N";
    map["Ò"] = "O";
    map["Ó"] = "O";
    map["Ô"] = "O";
    map["Õ"] = "O";
    map["Ö"] = "Oe";
    map["Ø"] = "O";
    map["Ō"] = "O";
    map["Ő"] = "O";
    map["Ŏ"] = "O";
    map["Œ"] = "OE";
    map["Ŕ"] = "R";
    map["Ř"] = "R";
    map["Ŗ"] = "R";
    map["Ś"] = "S";
    map["Ş"] = "S";
    map["Ŝ"] = "S";
    map["Ș"] = "S";
    map["Š"] = "S";
    map["Ť"] = "T";
    map["Ţ"] = "T";
    map["Ŧ"] = "T";
    map["Ț"] = "T";
    map["Ù"] = "U";
    map["Ú"] = "U";
    map["Û"] = "U";
    map["Ü"] = "Ue";
    map["Ū"] = "U";
    map["Ů"] = "U";
    map["Ű"] = "U";
    map["Ŭ"] = "U";
    map["Ũ"] = "U";
    map["Ų"] = "U";
    map["Ŵ"] = "W";
    map["Ŷ"] = "Y";
    map["Ÿ"] = "Y";
    map["Ý"] = "Y";
    map["Ź"] = "Z";
    map["Ż"] = "Z";
    map["Ž"] = "Z";
    map["à"] = "a";
    map["á"] = "a";
    map["â"] = "a";
    map["ã"] = "a";
    map["ä"] = "ae";
    map["ā"] = "a";
    map["ą"] = "a";
    map["ă"] = "a";
    map["å"] = "a";
    map["æ"] = "ae";
    map["ç"] = "c";
    map["ć"] = "c";
    map["č"] = "c";
    map["ĉ"] = "c";
    map["ċ"] = "c";
    map["ď"] = "d";
    map["đ"] = "d";
    map["è"] = "e";
    map["é"] = "e";
    map["ê"] = "e";
    map["ë"] = "e";
    map["ē"] = "e";
    map["ę"] = "e";
    map["ě"] = "e";
    map["ĕ"] = "e";
    map["ė"] = "e";
    map["ƒ"] = "f";
    map["ĝ"] = "g";
    map["ğ"] = "g";
    map["ġ"] = "g";
    map["ģ"] = "g";
    map["ĥ"] = "h";
    map["ħ"] = "h";
    map["ì"] = "i";
    map["í"] = "i";
    map["î"] = "i";
    map["ï"] = "i";
    map["ī"] = "i";
    map["ĩ"] = "i";
    map["ĭ"] = "i";
    map["į"] = "i";
    map["ı"] = "i";
    map["ĳ"] = "ij";
    map["ĵ"] = "j";
    map["ķ"] = "k";
    map["ĸ"] = "k";
    map["ł"] = "l";
    map["ľ"] = "l";
    map["ĺ"] = "l";
    map["ļ"] = "l";
    map["ŀ"] = "l";
    map["ñ"] = "n";
    map["ń"] = "n";
    map["ň"] = "n";
    map["ņ"] = "n";
    map["ŉ"] = "n";
    map["ŋ"] = "n";
    map["ò"] = "o";
    map["ó"] = "o";
    map["ô"] = "o";
    map["õ"] = "o";
    map["ö"] = "oe";
    map["ø"] = "o";
    map["ō"] = "o";
    map["ő"] = "o";
    map["ŏ"] = "o";
    map["œ"] = "oe";
    map["ŕ"] = "r";
    map["ř"] = "r";
    map["ŗ"] = "r";
    map["ś"] = "s";
    map["š"] = "s";
    map["ť"] = "t";
    map["ù"] = "u";
    map["ú"] = "u";
    map["û"] = "u";
    map["ü"] = "ue";
    map["ū"] = "u";
    map["ů"] = "u";
    map["ű"] = "u";
    map["ŭ"] = "u";
    map["ũ"] = "u";
    map["ų"] = "u";
    map["ŵ"] = "w";
    map["ÿ"] = "y";
    map["ý"] = "y";
    map["ŷ"] = "y";
    map["ż"] = "z";
    map["ź"] = "z";
    map["ž"] = "z";
    map["ß"] = "ss";
    map["ſ"] = "ss";
    map["Α"] = "A";
    map["Ά"] = "A";
    map["Ἀ"] = "A";
    map["Ἁ"] = "A";
    map["Ἂ"] = "A";
    map["Ἃ"] = "A";
    map["Ἄ"] = "A";
    map["Ἅ"] = "A";
    map["Ἆ"] = "A";
    map["Ἇ"] = "A";
    map["ᾈ"] = "A";
    map["ᾉ"] = "A";
    map["ᾊ"] = "A";
    map["ᾋ"] = "A";
    map["ᾌ"] = "A";
    map["ᾍ"] = "A";
    map["ᾎ"] = "A";
    map["ᾏ"] = "A";
    map["Ᾰ"] = "A";
    map["Ᾱ"] = "A";
    map["Ὰ"] = "A";
    map["Ά"] = "A";
    map["ᾼ"] = "A";
    map["Β"] = "B";
    map["Γ"] = "G";
    map["Δ"] = "D";
    map["Ε"] = "E";
    map["Έ"] = "E";
    map["Ἐ"] = "E";
    map["Ἑ"] = "E";
    map["Ἒ"] = "E";
    map["Ἓ"] = "E";
    map["Ἔ"] = "E";
    map["Ἕ"] = "E";
    map["Έ"] = "E";
    map["Ὲ"] = "E";
    map["Ζ"] = "Z";
    map["Η"] = "I";
    map["Ή"] = "I";
    map["Ἠ"] = "I";
    map["Ἡ"] = "I";
    map["Ἢ"] = "I";
    map["Ἣ"] = "I";
    map["Ἤ"] = "I";
    map["Ἥ"] = "I";
    map["Ἦ"] = "I";
    map["Ἧ"] = "I";
    map["ᾘ"] = "I";
    map["ᾙ"] = "I";
    map["ᾚ"] = "I";
    map["ᾛ"] = "I";
    map["ᾜ"] = "I";
    map["ᾝ"] = "I";
    map["ᾞ"] = "I";
    map["ᾟ"] = "I";
    map["Ὴ"] = "I";
    map["Ή"] = "I";
    map["ῌ"] = "I";
    map["Θ"] = "TH";
    map["Ι"] = "I";
    map["Ί"] = "I";
    map["Ϊ"] = "I";
    map["Ἰ"] = "I";
    map["Ἱ"] = "I";
    map["Ἲ"] = "I";
    map["Ἳ"] = "I";
    map["Ἴ"] = "I";
    map["Ἵ"] = "I";
    map["Ἶ"] = "I";
    map["Ἷ"] = "I";
    map["Ῐ"] = "I";
    map["Ῑ"] = "I";
    map["Ὶ"] = "I";
    map["Ί"] = "I";
    map["Κ"] = "K";
    map["Λ"] = "L";
    map["Μ"] = "M";
    map["Ν"] = "N";
    map["Ξ"] = "KS";
    map["Ο"] = "O";
    map["Ό"] = "O";
    map["Ὀ"] = "O";
    map["Ὁ"] = "O";
    map["Ὂ"] = "O";
    map["Ὃ"] = "O";
    map["Ὄ"] = "O";
    map["Ὅ"] = "O";
    map["Ὸ"] = "O";
    map["Ό"] = "O";
    map["Π"] = "P";
    map["Ρ"] = "R";
    map["Ῥ"] = "R";
    map["Σ"] = "S";
    map["Τ"] = "T";
    map["Υ"] = "Y";
    map["Ύ"] = "Y";
    map["Ϋ"] = "Y";
    map["Ὑ"] = "Y";
    map["Ὓ"] = "Y";
    map["Ὕ"] = "Y";
    map["Ὗ"] = "Y";
    map["Ῠ"] = "Y";
    map["Ῡ"] = "Y";
    map["Ὺ"] = "Y";
    map["Ύ"] = "Y";
    map["Φ"] = "F";
    map["Χ"] = "X";
    map["Ψ"] = "PS";
    map["Ω"] = "O";
    map["Ώ"] = "O";
    map["Ὠ"] = "O";
    map["Ὡ"] = "O";
    map["Ὢ"] = "O";
    map["Ὣ"] = "O";
    map["Ὤ"] = "O";
    map["Ὥ"] = "O";
    map["Ὦ"] = "O";
    map["Ὧ"] = "O";
    map["ᾨ"] = "O";
    map["ᾩ"] = "O";
    map["ᾪ"] = "O";
    map["ᾫ"] = "O";
    map["ᾬ"] = "O";
    map["ᾭ"] = "O";
    map["ᾮ"] = "O";
    map["ᾯ"] = "O";
    map["Ὼ"] = "O";
    map["Ώ"] = "O";
    map["ῼ"] = "O";
    map["α"] = "a";
    map["ά"] = "a";
    map["ἀ"] = "a";
    map["ἁ"] = "a";
    map["ἂ"] = "a";
    map["ἃ"] = "a";
    map["ἄ"] = "a";
    map["ἅ"] = "a";
    map["ἆ"] = "a";
    map["ἇ"] = "a";
    map["ᾀ"] = "a";
    map["ᾁ"] = "a";
    map["ᾂ"] = "a";
    map["ᾃ"] = "a";
    map["ᾄ"] = "a";
    map["ᾅ"] = "a";
    map["ᾆ"] = "a";
    map["ᾇ"] = "a";
    map["ὰ"] = "a";
    map["ά"] = "a";
    map["ᾰ"] = "a";
    map["ᾱ"] = "a";
    map["ᾲ"] = "a";
    map["ᾳ"] = "a";
    map["ᾴ"] = "a";
    map["ᾶ"] = "a";
    map["ᾷ"] = "a";
    map["β"] = "b";
    map["γ"] = "g";
    map["δ"] = "d";
    map["ε"] = "e";
    map["έ"] = "e";
    map["ἐ"] = "e";
    map["ἑ"] = "e";
    map["ἒ"] = "e";
    map["ἓ"] = "e";
    map["ἔ"] = "e";
    map["ἕ"] = "e";
    map["ὲ"] = "e";
    map["έ"] = "e";
    map["ζ"] = "z";
    map["η"] = "i";
    map["ή"] = "i";
    map["ἠ"] = "i";
    map["ἡ"] = "i";
    map["ἢ"] = "i";
    map["ἣ"] = "i";
    map["ἤ"] = "i";
    map["ἥ"] = "i";
    map["ἦ"] = "i";
    map["ἧ"] = "i";
    map["ᾐ"] = "i";
    map["ᾑ"] = "i";
    map["ᾒ"] = "i";
    map["ᾓ"] = "i";
    map["ᾔ"] = "i";
    map["ᾕ"] = "i";
    map["ᾖ"] = "i";
    map["ᾗ"] = "i";
    map["ὴ"] = "i";
    map["ή"] = "i";
    map["ῂ"] = "i";
    map["ῃ"] = "i";
    map["ῄ"] = "i";
    map["ῆ"] = "i";
    map["ῇ"] = "i";
    map["θ"] = "th";
    map["ι"] = "i";
    map["ί"] = "i";
    map["ϊ"] = "i";
    map["ΐ"] = "i";
    map["ἰ"] = "i";
    map["ἱ"] = "i";
    map["ἲ"] = "i";
    map["ἳ"] = "i";
    map["ἴ"] = "i";
    map["ἵ"] = "i";
    map["ἶ"] = "i";
    map["ἷ"] = "i";
    map["ὶ"] = "i";
    map["ί"] = "i";
    map["ῐ"] = "i";
    map["ῑ"] = "i";
    map["ῒ"] = "i";
    map["ΐ"] = "i";
    map["ῖ"] = "i";
    map["ῗ"] = "i";
    map["κ"] = "k";
    map["λ"] = "l";
    map["μ"] = "m";
    map["ν"] = "n";
    map["ξ"] = "ks";
    map["ο"] = "o";
    map["ό"] = "o";
    map["ὀ"] = "o";
    map["ὁ"] = "o";
    map["ὂ"] = "o";
    map["ὃ"] = "o";
    map["ὄ"] = "o";
    map["ὅ"] = "o";
    map["ὸ"] = "o";
    map["ό"] = "o";
    map["π"] = "p";
    map["ρ"] = "r";
    map["ῤ"] = "r";
    map["ῥ"] = "r";
    map["σ"] = "s";
    map["ς"] = "s";
    map["τ"] = "t";
    map["υ"] = "y";
    map["ύ"] = "y";
    map["ϋ"] = "y";
    map["ΰ"] = "y";
    map["ὐ"] = "y";
    map["ὑ"] = "y";
    map["ὒ"] = "y";
    map["ὓ"] = "y";
    map["ὔ"] = "y";
    map["ὕ"] = "y";
    map["ὖ"] = "y";
    map["ὗ"] = "y";
    map["ὺ"] = "y";
    map["ύ"] = "y";
    map["ῠ"] = "y";
    map["ῡ"] = "y";
    map["ῢ"] = "y";
    map["ΰ"] = "y";
    map["ῦ"] = "y";
    map["ῧ"] = "y";
    map["φ"] = "f";
    map["χ"] = "x";
    map["ψ"] = "ps";
    map["ω"] = "o";
    map["ώ"] = "o";
    map["ὠ"] = "o";
    map["ὡ"] = "o";
    map["ὢ"] = "o";
    map["ὣ"] = "o";
    map["ὤ"] = "o";
    map["ὥ"] = "o";
    map["ὦ"] = "o";
    map["ὧ"] = "o";
    map["ᾠ"] = "o";
    map["ᾡ"] = "o";
    map["ᾢ"] = "o";
    map["ᾣ"] = "o";
    map["ᾤ"] = "o";
    map["ᾥ"] = "o";
    map["ᾦ"] = "o";
    map["ᾧ"] = "o";
    map["ὼ"] = "o";
    map["ώ"] = "o";
    map["ῲ"] = "o";
    map["ῳ"] = "o";
    map["ῴ"] = "o";
    map["ῶ"] = "o";
    map["ῷ"] = "o";
//    map["¨"] = "";
//    map["΅"] = "";
//    map["᾿"] = "";
//    map["῾"] = "";
//    map["῍"] = "";
//    map["῝"] = "";
//    map["῎"] = "";
//    map["῞"] = "";
//    map["῏"] = "";
//    map["῟"] = "";
//    map["῀"] = "";
//    map["῁"] = "";
//    map["΄"] = "";
//    map["΅"] = "";
//    map["`"] = "";
//    map["῭"] = "";
//    map["ͺ"] = "";
//    map["᾽"] = "";
    map["А"] = "A";
    map["Б"] = "B";
    map["В"] = "V";
    map["Г"] = "G";
    map["Д"] = "D";
    map["Е"] = "E";
    map["Ё"] = "E";
    map["Ж"] = "ZH";
    map["З"] = "Z";
    map["И"] = "I";
    map["Й"] = "I";
    map["К"] = "K";
    map["Л"] = "L";
    map["М"] = "M";
    map["Н"] = "N";
    map["О"] = "O";
    map["П"] = "P";
    map["Р"] = "R";
    map["С"] = "S";
    map["Т"] = "T";
    map["У"] = "U";
    map["Ф"] = "F";
    map["Х"] = "KH";
    map["Ц"] = "TS";
    map["Ч"] = "CH";
    map["Ш"] = "SH";
    map["Щ"] = "SHCH";
    map["Ы"] = "Y";
    map["Э"] = "E";
    map["Ю"] = "YU";
    map["Я"] = "YA";
    map["а"] = "A";
    map["б"] = "B";
    map["в"] = "V";
    map["г"] = "G";
    map["д"] = "D";
    map["е"] = "E";
    map["ё"] = "E";
    map["ж"] = "ZH";
    map["з"] = "Z";
    map["и"] = "I";
    map["й"] = "I";
    map["к"] = "K";
    map["л"] = "L";
    map["м"] = "M";
    map["н"] = "N";
    map["о"] = "O";
    map["п"] = "P";
    map["р"] = "R";
    map["с"] = "S";
    map["т"] = "T";
    map["у"] = "U";
    map["ф"] = "F";
    map["х"] = "KH";
    map["ц"] = "TS";
    map["ч"] = "CH";
    map["ш"] = "SH";
    map["щ"] = "SHCH";
    map["ы"] = "Y";
    map["э"] = "E";
    map["ю"] = "YU";
    map["я"] = "YA";
//    map["Ъ"] = "";
//    map["ъ"] = "";
//    map["Ь"] = "";
//    map["ь"] = "";
    map["ð"] = "d";
    map["Ð"] = "D";
    map["þ"] = "th";
    map["Þ"] = "TH";
    map["ა"] = "a";
    map["ბ"] = "b";
    map["გ"] = "g";
    map["დ"] = "d";
    map["ე"] = "e";
    map["ვ"] = "v";
    map["ზ"] = "z";
    map["თ"] = "t";
    map["ი"] = "i";
    map["კ"] = "k";
    map["ლ"] = "l";
    map["მ"] = "m";
    map["ნ"] = "n";
    map["ო"] = "o";
    map["პ"] = "p";
    map["ჟ"] = "zh";
    map["რ"] = "r";
    map["ს"] = "s";
    map["ტ"] = "t";
    map["უ"] = "u";
    map["ფ"] = "p";
    map["ქ"] = "k";
    map["ღ"] = "gh";
    map["ყ"] = "q";
    map["შ"] = "sh";
    map["ჩ"] = "ch";
    map["ც"] = "ts";
    map["ძ"] = "dz";
    map["წ"] = "ts";
    map["ჭ"] = "ch";
    map["ხ"] = "kh";
    map["ჯ"] = "j";
    map["ჰ"] = "h";

    }
    QString out;
    QChar c;
    for(auto i=0; i< input.length(); ++i)
    {
        c = input.at(i);
        if(c == ' ')
        {
            out.append("-");
        }
        else if(map.contains(QString(c)))
        {
            out.append(map.value(QString(c)));
        }
        else
            out.append(c);
    }
    return out;
}
