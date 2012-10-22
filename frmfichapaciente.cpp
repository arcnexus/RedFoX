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
#include <paciente.h>

Paciente *oPaciente = new Paciente();

FrmFichaPaciente::FrmFichaPaciente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmFichaPaciente)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    // Rellenar combos
    SqlCalls *llamada = new SqlCalls();
    ui->cboNivelEstudios->setModel(<QAbstractItemModel> llamada->queryModel("Select * from NivelEstudios ","dbmedica"));
    delete llamada;
}

FrmFichaPaciente::~FrmFichaPaciente()
{
    delete ui;
}

void FrmFichaPaciente::cargarDatos(Paciente oPaciente)
{
    ui->txtnHistoriaClinica->setText(QString::number(oPaciente.getnumHistoria()));
    bool Alc = oPaciente.getalcoholbool();
    ui->chkAlcohol->setChecked(Alc);
    ui->txtAlergiasConocidas->setPlainText(oPaciente.getalergiasConocidas());
    ui->txtAlergiasConocidasBarra->setPlainText(oPaciente.getalergiasConocidas());
    ui->txtAntecedentesFamiliares->setPlainText(oPaciente.getantecedentesFamiliares());
    ui->txtAntecedentesBarra->setPlainText(oPaciente.getantecedentesFamiliares());
    ui->txtCirugiasPrevias->setPlainText(oPaciente.getcirugiasPrevias());
    ui->txtCirugiasPreviasBarra->setPlainText(oPaciente.getcirugiasPrevias());
    ui->txtDiastole->setText(QString::number(oPaciente.getdiastole(),'f',2));
    ui->txtEnfermedadesConocidas->setPlainText(oPaciente.getenfermedadesConocidas());
    ui->txtEnfermedadesConocidasBarra->setPlainText(oPaciente.getenfermedadesConocidas());
    ui->txtFamilia->setPlainText(oPaciente.getfamilia());
    ui->txtFechaAlta->setDate(oPaciente.getfechaAlta());
    ui->txtNumFiliacion->setText(oPaciente.getfiliacion());
    ui->txtHabitosDrogas->setPlainText(oPaciente.gethabitosDrogas());
    ui->txthijos->setValue(oPaciente.gethijos());
    ui->txtHistorial->setPlainText(oPaciente.gethistorial());
    ui->txtIMC->setText(QString::number(oPaciente.getIMC()));
    ui->txtFechaNacimiento->setDateTime(oPaciente.getnacimiento());
    int nNivel = ui->cboNivelEstudios->findText(oPaciente.getnivelEstudios());
    if (!nNivel==-1)
        ui->cboNivelEstudios->setCurrentIndex(nNivel);

    ui->txtNumSS->setText(oPaciente.getnumSS());
    ui->txtHabitosDrogas->setPlainText(oPaciente.gethabitosDrogas());
    ui->TxtPerimetroCraneal->setText(QString::number(oPaciente.getperimetroCraneal(),'f',2));
    ui->txtPeso->setText(QString::number(oPaciente.getpeso(),'f',2));
    ui->txtProfesion->setText(oPaciente.getprofesion());
    if(oPaciente.getsexo()=="H")
        ui->radHombre->setChecked(true);
    else
        ui->radMujer->setChecked(true);
    ui->chkTabaco->setChecked(oPaciente.gettabacobool());
    ui->txtTalla->setText(QString::number(oPaciente.gettalla(),'f',2));
    if (oPaciente.gettrabajabool())
        ui->radSiTrabaja->setChecked(true);
    else
        ui->radNoTrabaja->setChecked(true);
}


void FrmFichaPaciente::finishedSlot(QNetworkReply* reply)
{
 //   qDebug()<<reply->readAll();
     QString data=(QString)reply->readAll();
     ui->txtHistorial->setPlainText(data);
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

void FrmFichaPaciente::on_btnAnadirEpisodio_clicked()
{
   oPaciente->nuevoEpisodio();
}

void FrmFichaPaciente::on_btnBuscarCIE_clicked()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finishedSlot(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://demo.vademecumdata.es/vweb/xml/ws_drug/SearchByName?value=frenadol")));
}
