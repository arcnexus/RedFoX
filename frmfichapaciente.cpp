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
    QStringList lista = llamada->queryList("Select nivel from nivelestudios","dbmedica");
    delete llamada;
    ui->cboNivelEstudios->addItems(lista);

}

FrmFichaPaciente::~FrmFichaPaciente()
{
    delete ui;
}

void FrmFichaPaciente::cargarDatos(int idcliente)
{
    oPaciente->RecuperarPaciente(idcliente);
    ui->txtNumHistoriaClinica->setText(QString::number(oPaciente->getnumHistoria()));
    bool Alc = oPaciente->getalcoholbool();
    ui->chkAlcohol->setChecked(Alc);
    ui->txtAlergiasConocidas->setPlainText(oPaciente->getalergiasConocidas());
    ui->txtAlergiasConocidasBarra->setPlainText(oPaciente->getalergiasConocidas());
    ui->txtAntecedentesFamiliares->setPlainText(oPaciente->getantecedentesFamiliares());
    ui->txtAntecedentesFamiliaresBarra->setPlainText(oPaciente->getantecedentesFamiliares());
    ui->txtCirugiasPrevias->setPlainText(oPaciente->getcirugiasPrevias());
    ui->txtCirugiasPreviasBarra->setPlainText(oPaciente->getcirugiasPrevias());
    ui->txtDiastole->setText(QString::number(oPaciente->getdiastole(),'f',2));
    ui->txtSistole->setText(QString::number(oPaciente->getsistole(),'f',2));
    ui->txtEnfermedadesConocidas->setPlainText(oPaciente->getenfermedadesConocidas());
    ui->txtEnfermedadesConocidasBarra->setPlainText(oPaciente->getenfermedadesConocidas());
    ui->txtFamiliaNuclear->setPlainText(oPaciente->getfamilia());
    ui->txtFechaAlta->setDate(oPaciente->getfechaAlta());
    ui->txtFiliacion->setText(oPaciente->getfiliacion());
;
    ui->txtHijos->setValue(oPaciente->gethijos());
    ui->txtHistorial->setPlainText(oPaciente->gethistorial());
    ui->txtIMC->setText(QString::number(oPaciente->getIMC()));
    ui->txtNacimiento->setDateTime(oPaciente->getnacimiento());
    int nNivel = ui->cboNivelEstudios->findText(oPaciente->getnivelEstudios());
    if (nNivel>-1)
        ui->cboNivelEstudios->setCurrentIndex(nNivel);

    ui->txtNumeroSS->setText(oPaciente->getnumSS());
    ui->txtOtrasDrogas->setPlainText(oPaciente->gethabitosDrogas());
    ui->txtPermietroCraneal->setText(QString::number(oPaciente->getperimetroCraneal(),'f',2));
    ui->txtPeso->setText(QString::number(oPaciente->getpeso(),'f',2));
    ui->txtProfesion->setText(oPaciente->getprofesion());
    if(oPaciente->getsexo()=="H")
        ui->radHombre->setChecked(true);
    else
        ui->radMujer->setChecked(true);
    ui->chkTabaco->setChecked(oPaciente->gettabacobool());
    ui->chkOtrasDrogas->setChecked(oPaciente->getotrasDrogasbool());
    ui->txtTalla->setText(QString::number(oPaciente->gettalla(),'f',2));
    if (oPaciente->gettrabajabool())
        ui->radTrabaja->setChecked(true);
    else
        ui->radNoTrabaja->setChecked(true);
}

void FrmFichaPaciente::guardarDatosPaciente()
{
    oPaciente->setnumHistoria(ui->txtNumHistoriaClinica->text().toInt());
    if(ui->chkAlcohol->isChecked())
        oPaciente->setalcohol(1);
    else
        oPaciente->setalcohol(0);
    oPaciente->setalergiasConocidas(ui->txtAlergiasConocidas->toPlainText());
    oPaciente->setantecedentesFamiliares(ui->txtAntecedentesFamiliares->toPlainText());
    oPaciente->setcirugiasPrevias(ui->txtCirugiasPrevias->toPlainText());
    oPaciente->setdiastole(ui->txtDiastole->text().toDouble());
    oPaciente->setsistole(ui->txtSistole->text().toDouble());
    oPaciente->setenfermedadesConocidas(ui->txtEnfermedadesConocidas->toPlainText());
    oPaciente->setfamilia(ui->txtFamiliaNuclear->toPlainText());
    oPaciente->setfechaAlta(ui->txtFechaAlta->date());
    oPaciente->setfiliacion(ui->txtFiliacion->text());
    oPaciente->sethabitosDrogas(ui->txtOtrasDrogas->toPlainText());
    oPaciente->sethijos(ui->txtHijos->text().toInt());
    oPaciente->sethistorial(ui->txtHistorial->toPlainText());
    oPaciente->setIMC(ui->txtIMC->text().toDouble());
    oPaciente->setnacimiento(ui->txtNacimiento->dateTime());
    oPaciente->setnivelEstudios(ui->cboNivelEstudios->currentText());
    oPaciente->setnumSS(ui->txtNumeroSS->text());
    oPaciente->setperimetroCraneal(ui->txtPermietroCraneal->text().toDouble());
    oPaciente->setpeso(ui->txtPeso->text().toDouble());
    oPaciente->setprofesion(ui->txtProfesion->text());
    if(ui->radHombre->isChecked())
        oPaciente->setsexo("H");
    else
        oPaciente->setsexo("M");
    if(ui->chkTabaco->isChecked())
        oPaciente->settabaco(1);
    else
        oPaciente->settabaco(0);
    oPaciente->settalla(ui->txtTalla->text().toDouble());
    if (ui->radTrabaja->isChecked())
        oPaciente->settrabaja(1);
    else
        oPaciente->settrabaja(0);
    if (ui->chkOtrasDrogas->isChecked())
            oPaciente->setotrasDrogas(1);
    else
            oPaciente->setotrasDrogas(0);
}


void FrmFichaPaciente::finishedSlot(QNetworkReply* reply)
{
 //   qDebug()<<reply->readAll();
     QString data=(QString)reply->readAll();
//     ui->txtHistorial->setPlainText(data);
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
    paciente = bd->RecuperarPaciente(oPaciente->getid());
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

void FrmFichaPaciente::BloquearCamposPaciente()
{
    ui->BtnDeshacerPaciente->setEnabled(false);
    ui->btnGuardarPaciente->setEnabled(false);
    ui->btnEditarPaciente->setEnabled(true);
    ui->chkAlcohol->setEnabled(false);
    ui->chkTabaco->setEnabled(false);
    ui->chkOtrasDrogas->setEnabled(false);
    ui->txtOtrasDrogas->setEnabled(false);
    ui->txtNacimiento->setEnabled(false);
    ui->txtFechaAlta->setEnabled(false);
    ui->txtFamiliaNuclear->setEnabled(false);
    ui->txtAlergiasConocidas->setEnabled(false);
    ui->txtAntecedentesFamiliares->setEnabled(false);
    ui->txtCirugiasPrevias->setEnabled(false);
    ui->txtDiastole->setEnabled(false);
    ui->txtEnfermedadesConocidas->setEnabled(false);
    ui->txtFiliacion->setEnabled(false);
    ui->txtHijos->setEnabled(false);
    ui->txtIMC->setEnabled(false);
    ui->txtCirugiasPrevias->setEnabled(false);
    ui->txtMutua->setEnabled(false);
    ui->txtNumeroSS->setEnabled(false);
    ui->txtPermietroCraneal->setEnabled(false);
    ui->txtPeso->setEnabled(false);
    ui->txtProfesion->setEnabled(false);
    ui->txtSistole->setEnabled(false);
    ui->txtTalla->setEnabled(false);
    ui->cboNivelEstudios->setEnabled(false);
    ui->radHombre->setEnabled(false);
    ui->radMujer->setEnabled(false);
    ui->radTrabaja->setEnabled(false);
    ui->radNoTrabaja->setEnabled(false);



}


void FrmFichaPaciente::DesbloquearCamposPaciente()
{
    ui->BtnDeshacerPaciente->setEnabled(true);
    ui->btnGuardarPaciente->setEnabled(true);
    ui->btnEditarPaciente->setEnabled(false);
    ui->chkAlcohol->setEnabled(true);
    ui->chkTabaco->setEnabled(true);
    ui->chkOtrasDrogas->setEnabled(true);
    ui->txtOtrasDrogas->setEnabled(true);
    ui->txtNacimiento->setEnabled(true);
    ui->txtFechaAlta->setEnabled(true);
    ui->txtFamiliaNuclear->setEnabled(true);
    ui->txtAlergiasConocidas->setEnabled(true);
    ui->txtAntecedentesFamiliares->setEnabled(true);
    ui->txtCirugiasPrevias->setEnabled(true);
    ui->txtDiastole->setEnabled(true);
    ui->txtEnfermedadesConocidas->setEnabled(true);
    ui->txtFiliacion->setEnabled(true);
    ui->txtHijos->setEnabled(true);
    ui->txtIMC->setEnabled(true);
    ui->txtCirugiasPrevias->setEnabled(true);
    ui->txtMutua->setEnabled(true);
    ui->txtNumeroSS->setEnabled(true);
    ui->txtPermietroCraneal->setEnabled(true);
    ui->txtPeso->setEnabled(true);
    ui->txtProfesion->setEnabled(true);
    ui->txtSistole->setEnabled(true);
    ui->txtTalla->setEnabled(true);
    ui->cboNivelEstudios->setEnabled(true);
    ui->radHombre->setEnabled(true);
    ui->radMujer->setEnabled(true);
    ui->radTrabaja->setEnabled(true);
    ui->radNoTrabaja->setEnabled(true);
}


void FrmFichaPaciente::BloquearCamposEpisodio()
{
}

void FrmFichaPaciente::DesbloquearCamposEpisodio()
{
}

void FrmFichaPaciente::on_btnEditarPaciente_clicked()
{
    DesbloquearCamposPaciente();
    ui->txtFamiliaNuclear->setFocus();
}

void FrmFichaPaciente::on_btnGuardarPaciente_clicked()
{
    BloquearCamposPaciente();
    guardarDatosPaciente();
    oPaciente->GuardarPaciente();


}
