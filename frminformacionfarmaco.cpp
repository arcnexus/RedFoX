#include "frminformacionfarmaco.h"
#include "ui_frminformacionfarmaco.h"
#include <QDebug>
#include <QDomDocument>
#include <QSettings>

FrmInformacionFarmaco::FrmInformacionFarmaco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmInformacionFarmaco)
{
    ui->setupUi(this);
    connect(ui->btnAceptar,SIGNAL(clicked()),this,SLOT(close()));
}

FrmInformacionFarmaco::~FrmInformacionFarmaco()
{
    delete ui;
}

void FrmInformacionFarmaco::capturarid(QString ccodigonacional)
{
     QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_drug/SearchByCodeFull?value="+
     ccodigonacional;
     // Recupero valores conexión Vademecum
     QSettings settings("infint", "terra");
     QString cClave1 = settings.value("Clave1").toString();
     QString cClave2 = settings.value("Clave2").toString();

     cUrl = cUrl + "&id_ent=" + cClave1;


         QNetworkAccessManager *manager = new QNetworkAccessManager(this);
         connect(manager, SIGNAL(finished(QNetworkReply*)),
                 this, SLOT(finishedSlotBuscarMedicamento(QNetworkReply*)));

        manager->get(QNetworkRequest(QUrl(cUrl)));

 }

 void FrmInformacionFarmaco::finishedSlotBuscarMedicamento(QNetworkReply* reply)
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
      while (!n.isNull()) {
          if (n.isElement()) {
              QDomNodeList attributes = n.childNodes();

              for (int i = 0; i < attributes.count(); i ++) {
                  QDomElement e = attributes.at(i).toElement();

                  if (e.tagName() == "name_speciality") {
                        ui->txtNombre->setText(e.text());
                  }
                  if (e.tagName() == "dosage_form") {
                        ui->txtDosificacion->setText(e.text());
                  }
                  if (e.tagName() == "national_code") {
                        ui->txtCodigoNacional->setText(e.text());
                  }
                  if (e.tagName() == "name_laboratory") {
                        ui->txtLaboratorio->setText(e.text());
                  }
                  if (e.tagName() == "name_atc") {
                        ui->txtNombreATC->setText(e.text());
                  }
                  if (e.tagName() == "drug_type") {
                        ui->txtTipoMedicamento->setText(e.text());
                  }
                  if (e.tagName() == "package") {
                        ui->txtCaja->setText(e.text());
                  }
                  if (e.tagName() == "price") {
                        ui->txtPVP->setText(e.text());
                  }
                  if (e.tagName() == "laboratory_price") {
                        ui->txtPVC->setText(e.text());
                  }
                  if (e.tagName() == "TLD") {
                        ui->txtTLD->setText(e.text());
                  }

              }

          }
          n = n.nextSibling();
      }
 }
