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

     QStringList medicamentos;
     QStringList id;
     ui->tablamedicamentos->setColumnCount(4);
     ui->tablamedicamentos->setColumnWidth(0,400);
     ui->tablamedicamentos->setColumnWidth(1,0);
     ui->tablamedicamentos->setColumnWidth(2,450);
     ui->tablamedicamentos->setColumnWidth(3,200);



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






             }

             pos++;

             //data.append(s);
         }
         n = n.nextSibling();
     }

//     int ncol=2;
//     QStandardItemModel *model = new QStandardItemModel(nrow, ncol, this);
//     model->setHorizontalHeaderItem(0, new QStandardItem(QString("ID")));
//     model->setHorizontalHeaderItem(1, new QStandardItem(QString("Nombre")));
//     // Manteniendo el ancho de columna al tamaño de los contenidos y del TableView
//    model->sethorizontalHeader()->setResizeMode(QHeaderView::Stretch);
//    // Llenado de los valores del modelo para ID
//    QList<QString>::iterator i;
//        for (i = id.begin(); i != id.end(); ++i) {
//            model->setItem(i, 0, new QStandardItem(QString(id.at(i).toUtf8())));
//     }
//     // Asignando el modelo al TableView
//     ui->tablaMedicamentos->setModel(*model);



     // Muestro elementos
     ui->textEdit->setText((cXML));

}



