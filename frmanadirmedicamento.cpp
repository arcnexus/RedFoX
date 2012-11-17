#include "frmanadirmedicamento.h"
#include "ui_frmanadirmedicamento.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>
#include <QXmlStreamReader>
#include <QtSql>
#include "vademecum.h"
#include <QDomDocument>
#include <QDomElement>


FrmAnadirMedicamento::FrmAnadirMedicamento(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmAnadirMedicamento)
{
    ui->setupUi(this);
}

FrmAnadirMedicamento::~FrmAnadirMedicamento()
{
    delete ui;
}

void FrmAnadirMedicamento::on_pushButton_clicked()
{
//    vademecum *vConexionVademecum = new vademecum();
    QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_drug/SearchByName?value="+
    ui->txtBuscar->text().trimmed();
//    QString cResultado;
//    cResultado = vConexionVademecum->recuperar(cURL);

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

   //     manager->get(QNetworkRequest(QUrl("http://demo.vademecumdata.es/vweb/xml/ws_drug/SearchByName?value=frenadol")));
       manager->get(QNetworkRequest(QUrl(cUrl)));
        //ui->textEdit->setText(cResultado);
    //delete vConexionVademecum;





}
void FrmAnadirMedicamento::finishedSlot(QNetworkReply* reply)
{
    //qDebug()<<reply->readAll();
     QString data=(QString)reply->readAll();
     QString cXML = data;
     // Extract values from XML
     QDomDocument document("XmlDoc");
     document.setContent(cXML);

     QDomElement root = document.documentElement();
     if (root .tagName() != "object")
         qDebug("Bad root element.");

     QDomNode drugList = root.firstChild();

     QDomNode n = drugList.firstChild();

     QStringList forms;

     while (!n.isNull()) {
         if (n.isElement()) {
             QDomNodeList attributes = n.childNodes();

             for (int i = 0; i < attributes.count(); i ++) {
                 QDomElement e = attributes.at(i).toElement();

                 if (e.tagName() == "name_speciality")
                     forms.append(e.text());
             }
             //data.append(s);
         }
         n = n.nextSibling();
     }
     // Muestro elementos
     ui->textEdit->setText((cXML));
     ui->listamedicamentos->clear();
     ui->listamedicamentos->addItems(forms);
}



