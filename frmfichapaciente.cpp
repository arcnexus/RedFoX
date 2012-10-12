#include "frmfichapaciente.h"
#include "ui_frmfichapaciente.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QImageReader>
#include <QByteArray>
#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QtXml/QtXml>
#include <frmagendavisitas.h>
#include <sqlcalls.h>

FrmFichaPaciente::FrmFichaPaciente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmFichaPaciente)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
}

FrmFichaPaciente::~FrmFichaPaciente()
{
    delete ui;
}

void FrmFichaPaciente::on_pushButton_3_clicked()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finishedSlot(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://demo.vademecumdata.es/vweb/xml/ws_drug/SearchByName?value=frenadol")));



}

void FrmFichaPaciente::finishedSlot(QNetworkReply* reply)
{
 //   qDebug()<<reply->readAll();
     QString data=(QString)reply->readAll();
     ui->txtEpisodio->setPlainText(data);
//     QDomDocument medicamento;
//     QXmlStreamReader xmlReader(data) = new QXmlStreamReader();
//     while(!xmlReader->atEnd() && !xmlReader->hasError()) {
//             // Read next element
//             QXmlStreamReader::TokenType token = xmlReader->readNext();
//             //If token is just StartDocument - go to next
//             if(token == QXmlStreamReader::StartDocument) {
//                     continue;
//             }
//             //If token is StartElement - read it
//             if(token == QXmlStreamReader::StartElement) {

//                     if(xmlReader->name() == "name") {
//                             continue;
//                     }

//                     if(xmlReader->name() == "dosage_form") {
//                         qDebug() << xmlReader->readElementText();
//                     }
//             }
//     }





/*
     QXmlStreamReader xmlStream(data);
         while(!xmlStream.atEnd())
         {
             xmlStream.readNext();
             if(xmlStream.isStartElement())
             {
                 // Read the tag name.
                 QString sec(xmlStream.name().toString());
                 // Check in settings map, whether there's already an entry. If not, insert.
                 if(sec=="drug") {
                     attrib=xmlStream.attributes();
                     QString nombre = xmlStream.attributes().value("drug","dosage_form").toString();
                     ui->txtCIE->setText(nombre);
                 }
             }
         }
*/


}


void FrmFichaPaciente::on_btnAgenda_clicked()
{
    ui->btnAgenda->setEnabled(false);
    frmAgendaVisitas *Agenda = new frmAgendaVisitas(this);
    Agenda->setWindowState(Qt::WindowMaximized);
    Agenda->show();
    ui->btnAgenda->setEnabled(true);

}



void FrmFichaPaciente::on_tablaEpisodios_clicked(const QModelIndex &index)
{
    SqlCalls *bd = new SqlCalls();
    paciente = bd->RecuperarPaciente(1);
}
