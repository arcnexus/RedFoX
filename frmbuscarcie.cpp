#include "frmbuscarcie.h"
#include "ui_frmbuscarcie.h"
#include <QSettings>
#include <QNetworkAccessManager>
# include <QDomDocument>

FrmBuscarCIE::FrmBuscarCIE(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmBuscarCIE)
{
    ui->setupUi(this);
    connect(ui->btnCancelar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->btnBuscar,SIGNAL(clicked()),this,SLOT(sltBuscarCie()));
}

FrmBuscarCIE::~FrmBuscarCIE()
{
    delete ui;
}

void FrmBuscarCIE::sltBuscarCie()
{
    QString cUrl = "http://demo.vademecumdata.es/vweb/xml/ws_cie9/SearchByName?value="+
    ui->txtCIE->text().trimmed();


    // Recupero valores conexión Vademecum
    QSettings settings("infint", "terra");
    QString cClave1 = settings.value("Clave1").toString();
    QString cClave2 = settings.value("Clave2").toString();

    cUrl = cUrl + "&id_ent=" + cClave1;

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(finishedSlotCIE(QNetworkReply*)));

       manager->get(QNetworkRequest(QUrl(cUrl)));


}
void FrmBuscarCIE::finishedSlotCIE(QNetworkReply* reply)
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
       list <<"ID"<<"CÓDIGO"<<"DENOMINACIÓN";

     ui->tablaCIE->setColumnCount(3);
     ui->tablaCIE->setColumnWidth(0,0);
     ui->tablaCIE->setColumnWidth(1,120);
     ui->tablaCIE->setColumnWidth(2,450);
     ui->tablaCIE->setHorizontalHeaderLabels(list);



    int nrow = 0;
    int pos = 0;
     while (!n.isNull()) {
         if (n.isElement()) {
             QDomNodeList attributes = n.childNodes();

             for (int i = 0; i < attributes.count(); i ++) {
                 QDomElement e = attributes.at(i).toElement();

                 if (e.tagName() == "id_classe_cie") {
                     //medicamentos.append(e.text());
                     ui->tablaCIE->setRowCount(pos+1);
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::blue); // color de los items

                     ui->tablaCIE->setItem(pos,0,newItem);


                 }
                 if (e.tagName() == "code_cie9") {
                     //id.append(e.text());
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::red); // color de los items
                     ui->tablaCIE->setItem(pos,1,newItem);
                 }
                 if (e.tagName() == "libelle_cie") {
                     QTableWidgetItem *newItem = new QTableWidgetItem(e.text());
                     // para que los elementos no sean editables
                     newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                     newItem->setTextColor(Qt::red); // color de los items
                     ui->tablaCIE->setItem(pos,2,newItem);
                 }

             }

             pos++;

             //data.append(s);
         }
         n = n.nextSibling();
     }

}
// FIN BUSCAR CIE

