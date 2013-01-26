#include "frminformacionfarmaco.h"
#include "ui_frminformacionfarmaco.h"
#include <QDebug>
#include <QDomDocument>
#include <QSettings>
#include <QDate>
#include <QUrl>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWebKitWidgets/QWebView>
#else
#include <QWebView>
#endif

FrmInformacionFarmaco::FrmInformacionFarmaco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmInformacionFarmaco)
{
    ui->setupUi(this);
    ui->webimagen1->setVisible(false);
    ui->webimagen2->setVisible(false);
    ui->webimagen3->setVisible(false);
    ui->webLogoLaboratorio->setVisible(false);
    ui->txtFechaBaja->setVisible(false);
    ui->lblfechabaja->setVisible(false);

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


    //NOTE - Este pointer no se puede borrar aqui
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
    QDomNode n2 = n.firstChild();
    QDomNode n3 = n2.firstChild();
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
                if (e.tagName() == "RECETA") {
                    ui->txtReceta->setText(e.text());
                }
                if (e.tagName() == "FINAN") {
                    ui->txtFinanciado->setText(e.text());
                }
                if (e.tagName() == "fecha_alta") {
                    QDate alta;
                    int ano,mes,dia;
                    ano = e.text().left(4).toInt();
                    mes = e.text().mid(5,2).toInt();
                    dia = e.text().mid(8,2).toInt();
                    alta.setDate(ano,mes,dia);
                    ui->txtFechaAlta->setDate(alta);
                }
                if (e.tagName() == "fecha_baja") {
                    if (e.text()!="0" && !e.text().isEmpty()) {
                        QDate baja;
                        int ano,mes,dia;
                        ano = e.text().left(4).toInt();
                        mes = e.text().mid(5,2).toInt();
                        dia = e.text().mid(8,2).toInt();
                        baja.setDate(ano,mes,dia);
                        ui->txtFechaBaja->setVisible(true);
                        ui->lblfechabaja->setVisible(true);
                        ui->txtFechaBaja->setDate(baja);
                    } else {
                        ui->txtFechaBaja->setVisible(false);
                        ui->lblfechabaja->setVisible(false);
                    }
                }
                if (e.tagName() == "baja") {
                    if(e.text()=="1")
                        ui->lblDadodeBaja->setVisible(true);
                    else
                        ui->lblDadodeBaja->setVisible(false);
                }

                if (e.tagName() == "id_laboratory") {
                    QUrl  uUrl;
                    QString cUrl = "http://svadcf.es/documentos/image/fotos/laboratorio/"+e.text().trimmed()+".gif";
                    uUrl.setUrl(cUrl);
                    ui->webLogoLaboratorio->load(uUrl);
                }

                if (e.tagName() == "id_speciality") {
                    connect(ui->webimagen1,SIGNAL(loadFinished(bool)),this, SLOT(cargaFinalizada1(bool)));
                    connect(ui->webimagen2,SIGNAL(loadFinished(bool)),this, SLOT(cargaFinalizada2(bool)));
                    connect(ui->webimagen3,SIGNAL(loadFinished(bool)),this, SLOT(cargaFinalizada3(bool)));
                    connect(ui->webLogoLaboratorio,SIGNAL(loadFinished(bool)),this, SLOT(cargaFinalizadaLogo(bool)));
                    QUrl  uUrl;
                    QString cUrl = "http://svadcf.es/documentos/image/fotos/medicamento/"+e.text().trimmed()+"_1.jpg";
                    uUrl.setUrl(cUrl);
                    ui->webimagen1->load(uUrl);

                    cUrl = "http://svadcf.es/documentos/image/fotos/medicamento/"+e.text().trimmed()+"_2.jpg";
                    uUrl.setUrl(cUrl);
                    ui->webimagen2->load(uUrl);
                    cUrl = "http://svadcf.es/documentos/image/fotos/medicamento/"+e.text().trimmed()+"_3.jpg";
                    uUrl.setUrl(cUrl);
                    ui->webimagen3->load(uUrl);
                }

                //----------------------------
                // Llenar tabla indicaciones
                //----------------------------

                if (e.tagName() == "Indications_set") {
                    ui->listaIndicaciones->setColumnCount(2);
                    ui->listaIndicaciones->setColumnWidth(0,200);
                    ui->listaIndicaciones->setColumnWidth(1,0);

                    int nrow = 0;
                    int pos = 0;
                    while (!n2.isNull()) {
                        if (n2.isElement()) {
                            QDomNodeList attributes = n2.childNodes();

                            for (int i = 0; i < attributes.count(); i ++) {
                                QDomElement e2 = attributes.at(i).toElement();
                                if (e2.tagName() == "")
                                    while (!n3.isNull()) {
                                        if (n3.isElement()) {
                                            QDomNodeList attributes = n3.childNodes();

                                            for (int i = 0; i < attributes.count(); i ++) {
                                                QDomElement e3 = attributes.at(i).toElement();
                                                if (e3.tagName() == "ID_IND") {
                                                    ui->listaIndicaciones->setRowCount(pos+1);
                                                    QTableWidgetItem *newItem = new QTableWidgetItem(e3.text());
                                                    // para que los elementos no sean editables
                                                    newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                                                    newItem->setTextColor(Qt::blue); // color de los items

                                                    ui->listaIndicaciones->setItem(pos,1,newItem);
                                                }
                                                if (e3.tagName() == "TITINDMIN") {
                                                    ui->listaIndicaciones->setRowCount(pos+1);
                                                    QTableWidgetItem *newItem = new QTableWidgetItem(e3.text());
                                                    // para que los elementos no sean editables
                                                    newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                                                    newItem->setTextColor(Qt::blue); // color de los items

                                                    ui->listaIndicaciones->setItem(pos,0,newItem);
                                                }

                                            }

                                            pos++;

                                            //data.append(s);
                                        }
                                        n3 = n3.nextSibling();
                                    }

                            }
                            n2 = n2.nextSibling();
                        }
                    }

                }

            }
            n = n.nextSibling();
        }
    }
    //NOTE - pero se puede borrar aqui
    reply->manager()->deleteLater();
}

void FrmInformacionFarmaco::cargaFinalizada1(bool estado)
{

    if(ui->webimagen1->findText("Not Found"))
        ui->webimagen1->setVisible(false);
    else
        ui->webimagen1->setVisible(true);
}
void FrmInformacionFarmaco::cargaFinalizada2(bool estado)
{

    if(ui->webimagen2->findText("Not Found"))
        ui->webimagen2->setVisible(false);
    else
        ui->webimagen2->setVisible(true);
}
void FrmInformacionFarmaco::cargaFinalizada3(bool estado)
{

    if(ui->webimagen3->findText("Not Found"))
        ui->webimagen3->setVisible(false);
    else
        ui->webimagen3->setVisible(true);
}
void FrmInformacionFarmaco::cargaFinalizadaLogo(bool estado)
{

    if(ui->webLogoLaboratorio->findText("Not Found"))
        ui->webLogoLaboratorio->setVisible(false);
    else
        ui->webLogoLaboratorio->setVisible(true);
}
