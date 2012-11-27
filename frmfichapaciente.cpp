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
#include "paciente.h"
#include "episodio.h"
# include <QMessageBox>
#include "frmanadirmedicamento.h"
#include "vademecum.h"
#include "frmbuscarcie.h"

Paciente *oPaciente = new Paciente();
Episodio *oEpisodio = new Episodio();

FrmFichaPaciente::FrmFichaPaciente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmFichaPaciente)
{
    ui->setupUi(this);
    // Rellenar combos
    SqlCalls *llamada = new SqlCalls();
    QStringList listaEstudios = llamada->queryList("Select nivel from nivelestudios","dbmedica");
    QStringList listaDoctores = llamada->queryList("Select nombre from doctores ","dbmedica");
    delete llamada;
    ui->cboNivelEstudios->addItems(listaEstudios);
    ui->cboDoctorEpisodio->addItems(listaDoctores);
    connect(ui->btnCerrar, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnBuscarCIEEpisodio,SIGNAL(clicked()),this,SLOT(BuscarCIE()));


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
    ui->txtHistorialEpisodio->setPlainText(oPaciente->gethistorial());
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
    // cargar datos Episodios
    QSqlQueryModel *EpisodiosModelo = new QSqlQueryModel(this);
    EpisodiosModelo->setQuery("Select descripcion from episodios where idpaciente = "+QString::number(oPaciente->getid()),QSqlDatabase::database("dbmedica"));
    ui->listaEpisodios->setModel(EpisodiosModelo);

}

void FrmFichaPaciente::cargarEpisodio(int control)
{
    if(control==1) {
        DesbloquearCamposEpisodio();
    }
    // Cargo valores en pantalla
    ui->txtIdentificadorEpisodio->setText(QString::number(oEpisodio->getid()));
    ui->txtDescripcionEpisodio->setText(oEpisodio->getdescripcion());
    ui->txtDescrpicionEpisodio_2->setText(oEpisodio->getdescripcion());
    if(oEpisodio->getprivado() == 1)
        ui->chkEpisodioprivado->setChecked(true);
    else
        ui->chkEpisodioprivado->setChecked(false);
    int nIndex = ui->cboDoctorEpisodio->findText(oEpisodio->getdoctor());
    if (nIndex !=-1)
            ui->cboDoctorEpisodio->setCurrentIndex(nIndex);
    ui->txtFechaEpisodio->setDate(oEpisodio->getfecha());
    ui->txtHistorialEpisodio->setPlainText(oEpisodio->gethistorial());
    ui->txtCIEEpisiodio->setText(oEpisodio->getCIE());

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
    oPaciente->sethistorial(ui->txtHistorialEpisodio->toPlainText());
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

void FrmFichaPaciente::leerDatosMedicamento(int id, QString nombre)
{
    qDebug()  <<id;
    qDebug() <<nombre;
    QSqlQuery *qTratamientos = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QSqlQueryModel *qTratamientosM = new QSqlQueryModel();

    qTratamientos->prepare("insert into histofarma (idmedicamento,medicamento,idepisodio) values (:idmedicamento,:medicamento,:idepisodio)");
    qTratamientos->bindValue(":idmedicamento",id);
    qTratamientos->bindValue(":medicamento",nombre);
    qTratamientos->bindValue(":idepisodios",oEpisodio->getid());
    if (qTratamientos->exec()) {

        QString cSQL = "Select * from histofarma where id ="+QString::number(oEpisodio->getid());
        qTratamientosM->setQuery(cSQL,QSqlDatabase::database("dbmedica"));
    }
    delete qTratamientos;
    ui->listaTratamientosFarma->setModel(qTratamientosM);

}


void FrmFichaPaciente::finishedSlot(QNetworkReply* reply)
{
    //qDebug()<<reply->readAll();
     QString data=(QString)reply->readAll();
     ui->txtHistorialEpisodio->setPlainText(data);

}


void FrmFichaPaciente::on_btnAgenda_clicked()
{
    ui->btnAgenda->setEnabled(false);
    frmAgendaVisitas *Agenda = new frmAgendaVisitas(this);
    Agenda->setWindowState(Qt::WindowMaximized);
    Agenda->show();
    ui->btnAgenda->setEnabled(true);

}


void FrmFichaPaciente::on_btnAnadirEpisodio_clicked()
{
   int idEpisodio = oEpisodio->NuevoEpisodio(oPaciente->getid());
   oEpisodio->RecuperarEpisodio(idEpisodio);
   if (oEpisodio->getid() >0) {

        cargarEpisodio(1);
        ui->radEpisodioAbierto->setChecked(true);

   } else {
       QMessageBox::warning(NULL,tr("Nuevo episodio"),tr("No se ha podido recuperar el nuevo episodio"),tr("Aceptar"));

   }

}

void FrmFichaPaciente::on_btnBuscarCIEEpisodio_clicked()
{
//    vademecum *recuperar = new vademecum(this);
//    QStringList cDatos;
//    cDatos = recuperar->recuperarLista("http://wslatam.vademecum.es/MX/vweb/xml/ws_indication/SearchByName?value=ane");
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
    // Desactivo campos
     ui->chkEpisodioprivado->setEnabled(false);
     ui->cboDoctorEpisodio->setEnabled(false);
     ui->txtFechaEpisodio->setEnabled(false);
     ui->txtDescripcionEpisodio->setEnabled(false);
     ui->txtHistorialEpisodio->setEnabled(false);
     ui->txtCIEEpisiodio->setEnabled(false);
     ui->btnBuscarCIEEpisodio->setEnabled(false);
     ui->btnAnadirEpisodio->setEnabled(true);
     ui->btnEditarEpisodio->setEnabled(true);
     ui->btnGuardarEpisodio->setEnabled(false);
     ui->btnDeshacerEpisodio->setEnabled(false);
     ui->btnEpisodioAbierto->setEnabled(false);
     ui->btnEpisodioCerrado->setEnabled(false);
}

void FrmFichaPaciente::DesbloquearCamposEpisodio()
{
    // Activo campos
     ui->chkEpisodioprivado->setEnabled(true);
     ui->cboDoctorEpisodio->setEnabled(true);
     ui->txtFechaEpisodio->setEnabled(true);
     ui->txtDescripcionEpisodio->setEnabled(true);
     ui->txtHistorialEpisodio->setEnabled(true);
     ui->txtCIEEpisiodio->setEnabled(true);
     ui->btnBuscarCIEEpisodio->setEnabled(true);
     ui->btnAnadirEpisodio->setEnabled(false);
     ui->btnEditarEpisodio->setEnabled(false);
     ui->btnGuardarEpisodio->setEnabled(true);
     ui->btnDeshacerEpisodio->setEnabled(true);
     ui->btnEpisodioAbierto->setEnabled(true);
     ui->btnEpisodioCerrado->setEnabled(true);
}

void FrmFichaPaciente::LLenarEpisodio()
{
    if (ui->radEpisodioAbierto->isChecked())
        oEpisodio->setcerrado(0);
    else
        oEpisodio->setcerrado(1);
    oEpisodio->setCIE(ui->txtCIEEpisiodio->text());
    oEpisodio->setdescripcion(ui->txtDescripcionEpisodio->text());
    oEpisodio->setdoctor(ui->cboDoctorEpisodio->currentText());
    oEpisodio->setfecha(ui->txtFechaEpisodio->date());
    oEpisodio->sethistorial(ui->txtHistorialEpisodio->toPlainText());
    oEpisodio->setidPaciente(oPaciente->getid());
    if (ui->chkEpisodioprivado->isChecked())
        oEpisodio->setprivado(1);
    else
        oEpisodio->setprivado(0);
    //DesbloquearCamposEpisodio();
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

void FrmFichaPaciente::on_btnGuardarEpisodio_clicked()
{
    LLenarEpisodio();
    oEpisodio->GuardarEpisodio();
    // cargar datos Episodios
    QSqlQueryModel *EpisodiosModelo = new QSqlQueryModel(this);
    EpisodiosModelo->setQuery("Select descripcion from episodios where idpaciente = "+QString::number(oPaciente->getid()),QSqlDatabase::database("dbmedica"));
    ui->listaEpisodios->setModel(EpisodiosModelo);
    BloquearCamposEpisodio();

}


void FrmFichaPaciente::on_listaEpisodios_clicked(const QModelIndex &index)
{
    QString cDescripcion = ui->listaEpisodios->model()->data(index).toString();
    QSqlQuery *qEpisodio = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qEpisodio->prepare("select id from episodios where descripcion =:descripcion and idpaciente = :idpaciente");
    qEpisodio->bindValue(":descripcion",cDescripcion);
    qEpisodio->bindValue(":idpaciente",oPaciente->getid());
    if (qEpisodio->exec()) {
        qEpisodio->next();
        oEpisodio->RecuperarEpisodio(qEpisodio->value(0).toInt());
        cargarEpisodio(0);
        ui->btnEditarEpisodio->setEnabled(true);
    }
}



void FrmFichaPaciente::on_btnEditarEpisodio_clicked()
{
    DesbloquearCamposEpisodio();
    ui->txtDescripcionEpisodio->setFocus();
}

void FrmFichaPaciente::on_btnDeshacerEpisodio_clicked()
{
    cargarEpisodio(0);
    BloquearCamposEpisodio();
}

void FrmFichaPaciente::on_btnAnadirFarma_clicked()
{
    FrmAnadirMedicamento *nuevomed = new FrmAnadirMedicamento();
    nuevomed->setWindowState(Qt::WindowMaximized);
    connect(nuevomed, SIGNAL(datos(int, QString)), this, SLOT(leerDatosMedicamento(int, QString)));
    nuevomed->show();


}

void FrmFichaPaciente::BuscarCIE()
{
    FrmBuscarCIE *BuscarCIE = new FrmBuscarCIE();
    BuscarCIE->show();

}
