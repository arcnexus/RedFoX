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
#include <QStandardItemModel>
#include <QtWebKit>
#include <qwebview.h>

#include "qwebkitglobal.h"
#include "qwebpage.h"
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    #include <QtWidgets/qwidget.h>
#else
    #include <QtGui/qwidget.h>
#endif
#include <QtGui/qicon.h>
#include <QtGui/qpainter.h>
#include <QtCore/qurl.h>
#include <QtNetwork/qnetworkaccessmanager.h>

FrmAnadirMedicamento::FrmAnadirMedicamento(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmAnadirMedicamento)
{
    ui->setupUi(this);
    connect(ui->btnCerrar, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->tablamedicamentos,SIGNAL(cellClicked(int,int)),this, SLOT(BuscarProspectoMedicamento(int,int)));
    connect(ui->pushButton_Aceptarporpatologia,SIGNAL(clicked()),this, SLOT(Aceptarporpatologia()));
    connect(ui->pushButton_AceptarPorMolecula,SIGNAL(clicked()),this,SLOT(Aceptarpormolecula()));
    connect(ui->pushButton_AceptarPorPA,SIGNAL(clicked()),this,SLOT(AceptarporPA()));

}

FrmAnadirMedicamento::~FrmAnadirMedicamento()
{
    delete ui;
}

void FrmAnadirMedicamento::on_pushButton_clicked()
{
    QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_drug/SearchByName?value="+
    ui->txtBuscar->text().trimmed();


    // Recupero valores conexión Vademecum
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();

    cUrl = cUrl + "&id_ent=" + cClave1;



        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(finishedSlotMedicamento(QNetworkReply*)));

       manager->get(QNetworkRequest(QUrl(cUrl)));


}
void FrmAnadirMedicamento::finishedSlotMedicamento(QNetworkReply* reply)
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

       QStringList list;
       list <<"NOMBRE"<<"ID"<<"DOSIFICACIÓN"<<"EMPAQUETADO"<<"CODIGO NACIONAL";

     ui->tablamedicamentos->setColumnCount(5);
     ui->tablamedicamentos->setColumnWidth(0,350);
     ui->tablamedicamentos->setColumnWidth(1,0);
     ui->tablamedicamentos->setColumnWidth(2,400);
     ui->tablamedicamentos->setColumnWidth(3,200);
     ui->tablamedicamentos->setColumnWidth(4,100);
     ui->tablamedicamentos->setHorizontalHeaderLabels(list);



    int nrow = 0;
    int pos = 0;
     while (!n.isNull()) {
         if (n.isElement()) {
             QDomNodeList attributes = n.childNodes();

             for (int i = 0; i < attributes.count(); i ++) {
                 QDomElement e = attributes.at(i).toElement();

                 if (e.tagName() == "name_speciality") {
                     //medicamentos.append(e.text());
                     ui->tablamedicamentos->setRowCount(pos+1);
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::blue); // color de los items

                     ui->tablamedicamentos->setItem(pos,0,newItem);


                 }
                 if (e.tagName() == "id_speciality") {
                     //id.append(e.text());
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::red); // color de los items
                     ui->tablamedicamentos->setItem(pos,1,newItem);
                 }
                 if (e.tagName() == "dosage_form") {
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::red); // color de los items
                     ui->tablamedicamentos->setItem(pos,2,newItem);
                 }
                 if (e.tagName() == "package") {
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::green); // color de los items
                     ui->tablamedicamentos->setItem(pos,3,newItem);
                 }
                 if (e.tagName() == "national_code") {
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::green); // color de los items
                     ui->tablamedicamentos->setItem(pos,4,newItem);
                 }
             }

             pos++;

             //data.append(s);
         }
         n = n.nextSibling();
     }

}
// FIN BUSCAR MEDICAMENTO POR SU NOMBRE

// BUSCAR EL PROSPECTO
void FrmAnadirMedicamento::BuscarProspectoMedicamento(int irow, int icol)
{
    icol = 4;
    QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_prospecto/get_prospecto?national_code="+

            ui->tablamedicamentos->item(irow,icol)->text();


    // Recupero valores conexión Vademecum
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();

    cUrl = cUrl + "&id_ent=" + cClave1;


        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(finishedSlotBuscarProspectoMedicamento(QNetworkReply*)));

       manager->get(QNetworkRequest(QUrl(cUrl)));
}


void FrmAnadirMedicamento::finishedSlotBuscarProspectoMedicamento(QNetworkReply* reply)
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
     QDomNode n2 = n.firstChild();



    int nrow = 0;
    int pos = 0;
     while (!n.isNull()) {
         if (n.isElement()) {
             QDomNodeList attributes = n.childNodes();

             for (int i = 0; i < attributes.count(); i ++) {
                 QDomElement e = attributes.at(i).toElement();

                 if (e.tagName() == "prospecto") {

                     while (!n2.isNull()) {
                         if (n2.isElement()) {
                             QDomNodeList attributes = n2.childNodes();

                             for (int i = 0; i < attributes.count(); i ++) {
                                 QDomElement e2 = attributes.at(i).toElement();

                                 if (e2.tagName() == "body") {
                                     ui->webView_ProspectoBuscarPorMedicamento->setHtml(e.text().replace("/images_prospectos","http://svad.es/documentos/images_prospectos"));

                                 }
                             }

                             pos++;

                             //data.append(s);
                         }
                         n2 = n2.nextSibling();
                     }

                 }
             }

             pos++;

             //data.append(s);
         }
         n = n.nextSibling();
     }


     //ui->textProspecto->setText((cXML));

}
// FIN BUSCAR EL PROSPECTO





// BUSCAR MEDICAMENTO POR PATOLOGÍA

void FrmAnadirMedicamento::on_btnBuscarMedicamentoporPatologia_clicked()
{
   // ui->tablaPatologias->setCurrentCell(1,ui->tablaPatologias->currentRow(),NULL);
    QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_indication/DrugsList?id_indication="+
    ui->tablaPatologias->item(ui->tablaPatologias->currentRow(), 1)->text();


    // Recupero valores conexión Vademecum
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();

    cUrl = cUrl + "&id_ent=" + cClave1;


        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(finishedSlotBuscarMedicamentoPorPatologia(QNetworkReply*)));

       manager->get(QNetworkRequest(QUrl(cUrl)));

}

void FrmAnadirMedicamento::finishedSlotBuscarMedicamentoPorPatologia(QNetworkReply* reply)
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


     ui->tablamedicamentosPatologia->setColumnCount(4);
     ui->tablamedicamentosPatologia->setColumnWidth(0,400);
     ui->tablamedicamentosPatologia->setColumnWidth(1,0);
     ui->tablamedicamentosPatologia->setColumnWidth(2,450);
     ui->tablamedicamentosPatologia->setColumnWidth(3,200);
     ui->tablamedicamentosPatologia->setColumnWidth(3,0);

    int nrow = 0;
    int pos = 0;
     while (!n.isNull()) {
         if (n.isElement()) {
             QDomNodeList attributes = n.childNodes();

             for (int i = 0; i < attributes.count(); i ++) {
                 QDomElement e = attributes.at(i).toElement();

                 if (e.tagName() == "name_speciality") {
                     //medicamentos.append(e.text());
                     ui->tablamedicamentosPatologia->setRowCount(pos+1);
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::blue); // color de los items

                     ui->tablamedicamentosPatologia->setItem(pos,0,newItem);


                 }
                 if (e.tagName() == "id_speciality") {
                     //id.append(e.text());
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::red); // color de los items
                     ui->tablamedicamentosPatologia->setItem(pos,1,newItem);
                 }

                 if (e.tagName() == "package") {
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::green); // color de los items
                     ui->tablamedicamentosPatologia->setItem(pos,2,newItem);
                 }
                 if (e.tagName() == "national_code") {
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::green); // color de los items
                     ui->tablamedicamentosPatologia->setItem(pos,3,newItem);
                 }
             }

             pos++;

             //data.append(s);
         }
         n = n.nextSibling();
     }


    // ui->textProspecto->setText((cXML));

}
// FIN BUSCAR MEDICAMENTOS POR PATOLOGÍA
// BUSCAR PATOLOGIAS

void FrmAnadirMedicamento::on_btnBuscarPatologia_clicked()
{
    QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_indication/SearchByName?value="+
    ui->txtBuscarPatologia->text().trimmed();


    // Recupero valores conexión Vademecum
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();

    cUrl = cUrl + "&id_ent=" + cClave1;

    qDebug() << cUrl;
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(finishedSlotBuscarPatologia(QNetworkReply*)));

       manager->get(QNetworkRequest(QUrl(cUrl)));
}


void FrmAnadirMedicamento::finishedSlotBuscarPatologia(QNetworkReply* reply)
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


     ui->tablaPatologias->setColumnCount(2);
     ui->tablaPatologias->setColumnWidth(0,ui->tablaPatologias->width()-20);
     ui->tablaPatologias->setColumnWidth(1,0);


    int nrow = 0;
    int pos = 0;
     while (!n.isNull()) {
         if (n.isElement()) {
             QDomNodeList attributes = n.childNodes();

             for (int i = 0; i < attributes.count(); i ++) {
                 QDomElement e = attributes.at(i).toElement();

                 if (e.tagName() == "name_indication") {
                     //medicamentos.append(e.text());
                     ui->tablaPatologias->setRowCount(pos+1);
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::blue); // color de los items

                     ui->tablaPatologias->setItem(pos,0,newItem);


                 }
                 if (e.tagName() == "id_indication") {
                     //id.append(e.text());
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::blue); // color de los items
                     ui->tablaPatologias->setItem(pos,1,newItem);
                 }

             }

             pos++;

             //data.append(s);
         }
         n = n.nextSibling();
     }


     //ui->textProspecto->setText((cXML));

}
// FIN BUSCAR PATOLOGÍAS

void FrmAnadirMedicamento::on_btnVerProspecto_clicked()
{
    ui->tabBuscarporPatologia->setCurrentIndex(1);

}


// BUSCAR EL PROSPECTO
void FrmAnadirMedicamento::on_tablamedicamentosPatologia_clicked()
{
    QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_prospecto/get_prospecto?national_code="+
    ui->tablamedicamentosPatologia->item(ui->tablamedicamentosPatologia->currentRow(),3)->text();


    // Recupero valores conexión Vademecum
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();

    cUrl = cUrl + "&id_ent=" + cClave1;


        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(finishedSlotBuscarProspecto(QNetworkReply*)));

       manager->get(QNetworkRequest(QUrl(cUrl)));
}


void FrmAnadirMedicamento::finishedSlotBuscarProspecto(QNetworkReply* reply)
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
     QDomNode n2 = n.firstChild();



    int nrow = 0;
    int pos = 0;
     while (!n.isNull()) {
         if (n.isElement()) {
             QDomNodeList attributes = n.childNodes();

             for (int i = 0; i < attributes.count(); i ++) {
                 QDomElement e = attributes.at(i).toElement();

                 if (e.tagName() == "prospecto") {

                     while (!n2.isNull()) {
                         if (n2.isElement()) {
                             QDomNodeList attributes = n2.childNodes();

                             for (int i = 0; i < attributes.count(); i ++) {
                                 QDomElement e2 = attributes.at(i).toElement();

                                 if (e2.tagName() == "body") {
                                     ui->textProspectoPato->setHtml(e.text().replace("/images_prospectos","http://svad.es/documentos/images_prospectos"));
                                     ui->textProspecto->setHtml(e.text().replace("/images_prospectos","http://svad.es/documentos/images_prospectos"));


                                 }
                             }

                             pos++;

                             //data.append(s);
                         }
                         n2 = n2.nextSibling();
                     }

                 }
             }

             pos++;

             //data.append(s);
         }
         n = n.nextSibling();
     }


     //ui->textProspecto->setText((cXML));

}
// FIN BUSCAR EL PROSPECTO

void FrmAnadirMedicamento::on_tablaPatologias_doubleClicked()
{
    ui->btnBuscarMedicamentoporPatologia->click();
}
// BUSCAR UNA MOLÉCULA
void FrmAnadirMedicamento::on_btnBuscarMolcula_clicked()
{
    QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_substance/SearchByName?value="+
    ui->txtBuscarMolecula->text().trimmed();


    // Recupero valores conexión Vademecum
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();

    cUrl = cUrl + "&id_ent=" + cClave1;

    qDebug() << cUrl;
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(finishedSlotBuscarMolecula(QNetworkReply*)));

       manager->get(QNetworkRequest(QUrl(cUrl)));
}

void FrmAnadirMedicamento::finishedSlotBuscarMolecula(QNetworkReply* reply)
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


     ui->tablaMoleculas->setColumnCount(2);
     ui->tablaMoleculas->setColumnWidth(0,ui->tablaMoleculas->width()-20);
     ui->tablaMoleculas->setColumnWidth(1,0);


    int nrow = 0;
    int pos = 0;
     while (!n.isNull()) {
         if (n.isElement()) {
             QDomNodeList attributes = n.childNodes();

             for (int i = 0; i < attributes.count(); i ++) {
                 QDomElement e = attributes.at(i).toElement();

                 if (e.tagName() == "id_molecule") {
                     //medicamentos.append(e.text());
                     ui->tablaMoleculas->setRowCount(pos+1);
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::blue); // color de los items

                     ui->tablaMoleculas->setItem(pos,1,newItem);


                 }
                 if (e.tagName() == "name_molecule") {
                     //id.append(e.text());
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::blue); // color de los items
                     ui->tablaMoleculas->setItem(pos,0,newItem);
                 }

             }

             pos++;

             //data.append(s);
         }
         n = n.nextSibling();
     }


     //ui->textProspecto->setText((cXML));

}
// FIN BUSCAR MOLÉCULAS

// BUSCAR MEDICAMENTOS POR MOLÉCULA
void FrmAnadirMedicamento::on_btnBuscarMedicamentoporMolecula_clicked()
{
    // ui->tablaPatologias->setCurrentCell(1,ui->tablaPatologias->currentRow(),NULL);
     QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_substance/DrugsList?id_molecule="+
     ui->tablaMoleculas->item(ui->tablaMoleculas->currentRow(), 1)->text();

    ui->tablamedicamentosMolecula->clear();
     // Recupero valores conexión Vademecum
     QSettings settings("infint", "terra");
     QString cClave1 = settings.value("Clave1").toString();
     QString cClave2 = settings.value("Clave2").toString();

     cUrl = cUrl + "&id_ent=" + cClave1;


         QNetworkAccessManager *manager = new QNetworkAccessManager(this);
         connect(manager, SIGNAL(finished(QNetworkReply*)),
                 this, SLOT(finishedSlotBuscarMedicamentoPorMolecula(QNetworkReply*)));

        manager->get(QNetworkRequest(QUrl(cUrl)));

 }

 void FrmAnadirMedicamento::finishedSlotBuscarMedicamentoPorMolecula(QNetworkReply* reply)
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


      ui->tablamedicamentosMolecula->setColumnCount(4);
      ui->tablamedicamentosMolecula->setColumnWidth(0,400);
      ui->tablamedicamentosMolecula->setColumnWidth(1,0);
      ui->tablamedicamentosMolecula->setColumnWidth(2,450);
      ui->tablamedicamentosMolecula->setColumnWidth(3,200);
      ui->tablamedicamentosMolecula->setColumnWidth(3,0);

     int nrow = 0;
     int pos = 0;
      while (!n.isNull()) {
          if (n.isElement()) {
              QDomNodeList attributes = n.childNodes();

              for (int i = 0; i < attributes.count(); i ++) {
                  QDomElement e = attributes.at(i).toElement();

                  if (e.tagName() == "name_speciality") {
                      //medicamentos.append(e.text());
                      ui->tablamedicamentosMolecula->setRowCount(pos+1);
                      QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                      // para que los elementos no sean editables
                      newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                      newItem->setTextColor(Qt::blue); // color de los items

                      ui->tablamedicamentosMolecula->setItem(pos,0,newItem);


                  }
                  if (e.tagName() == "id_speciality") {
                      //id.append(e.text());
                      QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                      // para que los elementos no sean editables
                      newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                      newItem->setTextColor(Qt::red); // color de los items
                      ui->tablamedicamentosMolecula->setItem(pos,1,newItem);
                  }

                  if (e.tagName() == "package") {
                      QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                      // para que los elementos no sean editables
                      newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                      newItem->setTextColor(Qt::green); // color de los items
                      ui->tablamedicamentosMolecula->setItem(pos,2,newItem);
                  }
                  if (e.tagName() == "national_code") {
                      QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                      // para que los elementos no sean editables
                      newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                      newItem->setTextColor(Qt::green); // color de los items
                      ui->tablamedicamentosMolecula->setItem(pos,3,newItem);
                  }
              }

              pos++;

              //data.append(s);
          }
          n = n.nextSibling();
      }


     // ui->textProspecto->setText((cXML));

 }
 // FIN BUSCAR MEDICAMENTOS POR MOLÉCULA

void FrmAnadirMedicamento::on_tablamedicamentosMolecula_clicked()
{
    QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_prospecto/get_prospecto?national_code="+
    ui->tablamedicamentosMolecula->item(ui->tablamedicamentosMolecula->currentRow(),3)->text();
    ui->textProspectoMolecula->setHtml("");

    // Recupero valores conexión Vademecum
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();

    cUrl = cUrl + "&id_ent=" + cClave1;


        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(finishedSlotBuscarProspectoMolecula(QNetworkReply*)));

       manager->get(QNetworkRequest(QUrl(cUrl)));
}


void FrmAnadirMedicamento::finishedSlotBuscarProspectoMolecula(QNetworkReply* reply)
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
     QDomNode n2 = n.firstChild();




    int pos = 0;
     while (!n.isNull()) {
         if (n.isElement()) {
             QDomNodeList attributes = n.childNodes();

             for (int i = 0; i < attributes.count(); i ++) {
                 QDomElement e = attributes.at(i).toElement();

                 if (e.tagName() == "prospecto") {

                     while (!n2.isNull()) {
                         if (n2.isElement()) {
                             QDomNodeList attributes = n2.childNodes();

                             for (int i = 0; i < attributes.count(); i ++) {
                                 QDomElement e2 = attributes.at(i).toElement();

                                 if (e2.tagName() == "body") {
                                     ui->textProspectoMolecula->setHtml(e.text().replace("/images_prospectos","http://svad.es/documentos/images_prospectos"));
                                 }
                             }

                             pos++;

                             //data.append(s);
                         }
                         n2 = n2.nextSibling();
                     }

                 }
             }

             pos++;

             //data.append(s);
         }
         n = n.nextSibling();
     }


     //ui->textProspecto->setText((cXML));

}
// FIN BUSCAR EL PROSPECTO
// BUSCAR POR PRINCIPIOS ACTIVOS

void FrmAnadirMedicamento::on_btnBuscarPrincipiosActivos_clicked()
{
    QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_atc/SearchByName?value="+
    ui->txtBuscarPrincipioActivo->text().trimmed();


    // Recupero valores conexión Vademecum
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();

    cUrl = cUrl + "&id_ent=" + cClave1;

    qDebug() << cUrl;
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(finishedSlotBuscarPrincipioActivo(QNetworkReply*)));

       manager->get(QNetworkRequest(QUrl(cUrl)));
}
void FrmAnadirMedicamento::finishedSlotBuscarPrincipioActivo(QNetworkReply* reply)
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


     ui->tablaPrincipiosActivos->setColumnCount(2);
     ui->tablaPrincipiosActivos->setColumnWidth(0,ui->tablaPrincipiosActivos->width()-20);
     ui->tablaPrincipiosActivos->setColumnWidth(1,0);


    int nrow = 0;
    int pos = 0;
     while (!n.isNull()) {
         if (n.isElement()) {
             QDomNodeList attributes = n.childNodes();

             for (int i = 0; i < attributes.count(); i ++) {
                 QDomElement e = attributes.at(i).toElement();

                 if (e.tagName() == "code_atc") {
                     //medicamentos.append(e.text());
                     ui->tablaPrincipiosActivos->setRowCount(pos+1);
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::blue); // color de los items

                     ui->tablaPrincipiosActivos->setItem(pos,1,newItem);


                 }
                 if (e.tagName() == "name_atc") {
                     //id.append(e.text());
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::blue); // color de los items
                     ui->tablaPrincipiosActivos->setItem(pos,0,newItem);
                 }

             }

             pos++;

             //data.append(s);
         }
         n = n.nextSibling();
     }


     //ui->textProspecto->setText((cXML));

}
// FIN BUSCAR MOLÉCULAS
// BUSCAR MEDICAMENTO POR PRINCIPIO ACTIVO
void FrmAnadirMedicamento::on_btnBuscarMedicamentoporPA_clicked()
{
    // ui->tablaPatologias->setCurrentCell(1,ui->tablaPatologias->currentRow(),NULL);
     QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_atc/DrugsList?code_atc="+
     ui->tablaPrincipiosActivos->item(ui->tablaPrincipiosActivos->currentRow(), 1)->text();

    ui->tablamedicamentospactivos->clear();
     // Recupero valores conexión Vademecum
     QSettings settings("infint", "terra");
     QString cClave1 = settings.value("Clave1").toString();
     QString cClave2 = settings.value("Clave2").toString();

     cUrl = cUrl + "&id_ent=" + cClave1;


         QNetworkAccessManager *manager = new QNetworkAccessManager(this);
         connect(manager, SIGNAL(finished(QNetworkReply*)),
                 this, SLOT(finishedSlotBuscarMedicamentoPorPActivo(QNetworkReply*)));

        manager->get(QNetworkRequest(QUrl(cUrl)));

 }

 void FrmAnadirMedicamento::finishedSlotBuscarMedicamentoPorPActivo(QNetworkReply* reply)
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


      ui->tablamedicamentospactivos->setColumnCount(4);
      ui->tablamedicamentospactivos->setColumnWidth(0,400);
      ui->tablamedicamentospactivos->setColumnWidth(1,0);
      ui->tablamedicamentospactivos->setColumnWidth(2,450);
      ui->tablamedicamentospactivos->setColumnWidth(3,200);
      ui->tablamedicamentospactivos->setColumnWidth(3,0);

     int nrow = 0;
     int pos = 0;
      while (!n.isNull()) {
          if (n.isElement()) {
              QDomNodeList attributes = n.childNodes();

              for (int i = 0; i < attributes.count(); i ++) {
                  QDomElement e = attributes.at(i).toElement();

                  if (e.tagName() == "name_speciality") {
                      //medicamentos.append(e.text());
                      ui->tablamedicamentospactivos->setRowCount(pos+1);
                      QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                      // para que los elementos no sean editables
                      newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                      newItem->setTextColor(Qt::blue); // color de los items

                      ui->tablamedicamentospactivos->setItem(pos,0,newItem);


                  }
                  if (e.tagName() == "id_speciality") {
                      //id.append(e.text());
                      QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                      // para que los elementos no sean editables
                      newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                      newItem->setTextColor(Qt::red); // color de los items
                      ui->tablamedicamentospactivos->setItem(pos,1,newItem);
                  }

                  if (e.tagName() == "package") {
                      QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                      // para que los elementos no sean editables
                      newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                      newItem->setTextColor(Qt::green); // color de los items
                      ui->tablamedicamentospactivos->setItem(pos,2,newItem);
                  }
                  if (e.tagName() == "national_code") {
                      QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                      // para que los elementos no sean editables
                      newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                      newItem->setTextColor(Qt::green); // color de los items
                      ui->tablamedicamentospactivos->setItem(pos,3,newItem);
                  }
              }

              pos++;

              //data.append(s);
          }
          n = n.nextSibling();
      }


     // ui->textProspecto->setText((cXML));

 }
 // FIN BUSCAR MEDICAMENTOS POR MOLÉCULA

void FrmAnadirMedicamento::on_tablamedicamentospactivos_clicked()
{
    QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_prospecto/get_prospecto?national_code="+
    ui->tablamedicamentospactivos->item(ui->tablamedicamentospactivos->currentRow(),3)->text();
    ui->textProspectopa->setHtml("");

    // Recupero valores conexión Vademecum
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();

    cUrl = cUrl + "&id_ent=" + cClave1;


        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(finishedSlotBuscarProspectoPrincAct(QNetworkReply*)));

       manager->get(QNetworkRequest(QUrl(cUrl)));
}


void FrmAnadirMedicamento::finishedSlotBuscarProspectoPrincAct(QNetworkReply* reply)
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
     QDomNode n2 = n.firstChild();



    int nrow = 0;
    int pos = 0;
     while (!n.isNull()) {
         if (n.isElement()) {
             QDomNodeList attributes = n.childNodes();

             for (int i = 0; i < attributes.count(); i ++) {
                 QDomElement e = attributes.at(i).toElement();

                 if (e.tagName() == "prospecto") {

                     while (!n2.isNull()) {
                         if (n2.isElement()) {
                             QDomNodeList attributes = n2.childNodes();

                             for (int i = 0; i < attributes.count(); i ++) {
                                 QDomElement e2 = attributes.at(i).toElement();

                                 if (e2.tagName() == "body") {
                                     ui->textProspectopa->setHtml(e.text().replace("/images_prospectos","http://svad.es/documentos/images_prospectos"));
                                 }
                             }

                             pos++;

                             //data.append(s);
                         }
                         n2 = n2.nextSibling();
                     }

                 }
             }

             pos++;

             //data.append(s);
         }
         n = n.nextSibling();
     }


     //ui->textProspecto->setText((cXML));

}
// FIN BUSCAR EL PROSPECTO

void FrmAnadirMedicamento::on_btnAceptar1_clicked()
{
    emit datos(ui->tablamedicamentos->item(ui->tablamedicamentos->currentRow(),1)->text().toInt(),
               ui->tablamedicamentos->item(ui->tablamedicamentos->currentRow(),0)->text(),
               ui->tablamedicamentos->item(ui->tablamedicamentos->currentRow(),4)->text());
    close();

}
void FrmAnadirMedicamento::Aceptarporpatologia()
{
    emit datos(ui->tablamedicamentosPatologia->item(ui->tablamedicamentosPatologia->currentRow(),1)->text().toInt(),
               ui->tablamedicamentosPatologia->item(ui->tablamedicamentosPatologia->currentRow(),0)->text(),
               ui->tablamedicamentosPatologia->item(ui->tablamedicamentosPatologia->currentRow(),3)->text());
    close();

}

void FrmAnadirMedicamento::Aceptarpormolecula()
{
    emit datos(ui->tablamedicamentosMolecula->item(ui->tablamedicamentosMolecula->currentRow(),1)->text().toInt(),
               ui->tablamedicamentosMolecula->item(ui->tablamedicamentosMolecula->currentRow(),0)->text(),
               ui->tablamedicamentosMolecula->item(ui->tablamedicamentosMolecula->currentRow(),3)->text());
    close();

}
void FrmAnadirMedicamento::AceptarporPA()
{
    emit datos(ui->tablamedicamentospactivos->item(ui->tablamedicamentospactivos->currentRow(),1)->text().toInt(),
               ui->tablamedicamentospactivos->item(ui->tablamedicamentospactivos->currentRow(),0)->text(),
               ui->tablamedicamentospactivos->item(ui->tablamedicamentospactivos->currentRow(),3)->text());
    close();

}
