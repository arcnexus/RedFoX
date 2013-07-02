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
#include "../sqlcalls.h"
#include "paciente.h"
#include "episodio.h"
#include "frmanadirmedicamento.h"
#include "../vademecum.h"
#include "../Busquedas/frmbuscarcie.h"
#include "farmacologia.h"
#include "frminformacionfarmaco.h"
#include "frmanadirimagen.h"
#include "../frmvisitas.h"
#include "../interconsulta.h"
#include "frmanalitica.h"
#include "analitica.h"
#include "frmveranalitica.h"
#include "frmmtcbase.h"
#include "visitas.h"
#include "frmanadirdiagnostico.h"
#include "frmanadirhierb.h"


//TODO integrar http://doc.ginkgo-cadx.com/ginkgo-integration/ginkgo-cadx-integration-input-xml-integration/

FrmFichaPaciente::FrmFichaPaciente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmFichaPaciente)
{
    oPaciente = new Paciente();
    oEpisodio = new Episodio(this);
    oImagenes = new ImagenesDiagnostico(this);
    oVisita = new Visitas(this);

    ui->setupUi(this);
    // Rellenar combos
    SqlCalls llamada;
    QStringList listaEstudios = llamada.queryList("Select nivel from nivel_estudios","dbmedica");
    QStringList listaDoctores = llamada.queryList("Select nombre from doctores ","dbmedica");

    ui->cbonivel_estudios->addItems(listaEstudios);
    ui->cboDoctorEpisodio->addItems(listaDoctores);
    ui->cboRealizadaPorDr->addItems(listaDoctores);

    QSqlQueryModel *qTipos = new QSqlQueryModel(this);
    qTipos->setQuery("Select descripcion from tiposimagen",QSqlDatabase::database("dbmedica"));
    ui->comboBox_tipo_imagen->setModel(qTipos);
    ui->txtHistorialEpisodio->setPlainText("");

    //------------------
    // Doctores
    //------------------
    QSqlQueryModel *qMDoctor = new QSqlQueryModel(this);
    qMDoctor->setQuery("Select nombre from doctores",QSqlDatabase::database("dbmedica"));
    ui->cboDoctorEpisodio->setModel(qMDoctor);
    //------------------
    // Historiales
    //------------------
    ui->cboFechainicio_tratamiento_pestana_H_Farmacologico->setDate(QDate::currentDate());


    //------------------
    // Conexiones
    //------------------
    connect(ui->btnCerrar, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnBuscarCIEEpisodio,SIGNAL(clicked()),this,SLOT(BuscarCIE()));
    connect(ui->btnEditarFarmacologia,SIGNAL(clicked()), this, SLOT(ActivarControlesFarmacologia()));
    connect(ui->btnGuardarFarmacologia,SIGNAL(clicked()),this,SLOT(GuardarDatosFarmacologia()));
    connect(ui->listaTratamientosFarma,SIGNAL(cellClicked(int ,int )),this, SLOT(cargarDatosMedicamento(int ,int)));
    connect(ui->radvertodosfarmacologia,SIGNAL(clicked()),this,SLOT(llenartablahistorialfarmacologiaepisodio()));
    connect(ui->radversoloactivosfarmacologia,SIGNAL(clicked()),this,SLOT(llenartablahistorialfarmacologiaepisodio()));
    connect(ui->btnBorrarFarma,SIGNAL(clicked()),this,SLOT(BorrarDatosMedicamento()));
    connect(ui->btnVerFarma,SIGNAL(clicked()),this,SLOT(MostrarFichaMedicamento()));
    connect(ui->btnVerFarmaco_PestanaHFarmacologia,SIGNAL(clicked()),this,SLOT(MostrarFichaMedicamento2()));
    connect(ui->btnAnadirImagen,SIGNAL(clicked()), this, SLOT(AnadirImagenDiagnostico()));
    connect(ui->radImagenEvaluada,SIGNAL(clicked()),this,SLOT(llenartablahistorialimagenesepisodio()));
    connect(ui->radImagenPendiente, SIGNAL(clicked()),this,SLOT(llenartablahistorialimagenesepisodio()));
    connect(ui->listaImagenes,SIGNAL(cellClicked(int,int)),this,SLOT(cargarDatosImagenes(int,int)));
    connect(ui->btnBorrarImagen,SIGNAL(clicked()),this,SLOT(BorrarImagenDiagnostico()));
    connect(ui->pushButton_editarimagen,SIGNAL(clicked()), this, SLOT(EditarImagenDiagnostico()));
    connect(ui->pushButton_GuardarImagen,SIGNAL(clicked()),this,SLOT(guardarDatosImagenes()));
    connect(oImagenes,SIGNAL(refrescarlista()),this,SLOT(llenartablahistorialimagenesepisodio()));
    connect(ui->pushButton_DeshacerImagen,SIGNAL(clicked()),this,SLOT(deshacerDatosImagenes()));
    connect(ui->btnAnadirInterconsulta,SIGNAL(clicked()),this,SLOT(AnadirInterconsulta()));
    connect(ui->btnAnadirAnalitica,SIGNAL(clicked()),this,SLOT(AnadirAnalitica()));
    connect(ui->btnVerAnalitica,SIGNAL(clicked()),this,SLOT(VerAnalitica()));
    connect(ui->btnBorrarAnalitica,SIGNAL(clicked()),this,SLOT(BorrarAnalitica()));
    connect(ui->btnAnadirVisita,SIGNAL(clicked()),this,SLOT(AnadirVisita()));
    connect(ui->listaEpisodios,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(listaEpisodios_currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->btnBdMed,SIGNAL(clicked()),this,SLOT(vademecums()));
    connect(ui->listaEpisodios,SIGNAL(clicked(QModelIndex)),this,SLOT(ListaEpisodio_clicked(QModelIndex)));
    connect(ui->listaVisitas,SIGNAL(clicked(QModelIndex)),this,SLOT(ListaVisita_Clicked(QModelIndex)));
    connect(ui->btnAnadirDiagnostico,SIGNAL(clicked()),this,SLOT(anadirDiagnostico()));
    connect(ui->btnEditarVisita,SIGNAL(clicked()),this,SLOT(bntEditarVisita_clicked()));
    connect(ui->btnGuardarVisita,SIGNAL(clicked()),this,SLOT(btnGuardarVisita_clicked()));
    connect(ui->listaHistorialFarmacologicoTotal,SIGNAL(clicked(QModelIndex)), this,SLOT(ListaHistorialFarmacologia_clicked(QModelIndex)));
    connect(ui->listaImagenes_pestana_dimagen,SIGNAL(clicked(QModelIndex)), this,SLOT(ListaHistorialImagenes_clicked(QModelIndex)));
    connect(ui->btnAnadirHierba,SIGNAL(clicked()),this,SLOT(on_btnAnadirHerb_clicked()));

    // Ocultar Iconos imagenes
    ui->itemFarma->setVisible(false);
    ui->itemAnalitica->setVisible(false);
    ui->itemImagenes->setVisible(false);
    ui->itemInterConsulta->setVisible(false);
    ui->itemAvisos->setVisible(false);
    this->setWindowState(Qt::WindowMaximized);
}

FrmFichaPaciente::~FrmFichaPaciente()
{
    delete ui;
    delete oPaciente;
    delete oEpisodio ;
    delete oImagenes;
}

void FrmFichaPaciente::cargarDatos(int id_cliente)
{
    oPaciente->RecuperarPaciente(id_cliente);

    ui->txtnum_historiaClinica->setText(QString::number(oPaciente->getnum_historia()));
    ui->chkAlcohol->setChecked(oPaciente->getalcoholbool());
    ui->txtalergias_conocidas->setPlainText(oPaciente->getalergias_conocidas());
    ui->txtantecedentes_familiares->setPlainText(oPaciente->getantecedentes_familiares());
    ui->txtcirugias_previas->setPlainText(oPaciente->getcirugias_previas());
    ui->txtDiastole->setText(QString::number(oPaciente->getdiastole(),'f',2));
    ui->txtSistole->setText(QString::number(oPaciente->getsistole(),'f',2));
    ui->txtEnfermedadesConocidas->setPlainText(oPaciente->getenfermedadesConocidas());
    ui->txtFamiliaNuclear->setPlainText(oPaciente->getfamilia());
    ui->txtfecha_alta->setDate(oPaciente->getfecha_alta());
    ui->txtFiliacion->setText(oPaciente->getfiliacion());

    ui->txtHijos->setValue(oPaciente->gethijos());
    ui->txtHistorialEpisodio->setPlainText(oPaciente->gethistorial());
    ui->txtimc->setText(QString::number(oPaciente->getimc()));
    ui->txtNacimiento->setDateTime(oPaciente->getnacimiento());

    int nNivel = ui->cbonivel_estudios->findText(oPaciente->getnivel_estudios());
    if (nNivel>-1)
        ui->cbonivel_estudios->setCurrentIndex(nNivel);

    ui->txtNumeroSS->setText(oPaciente->getnum_ss());
    ui->txtotras_drogas->setPlainText(oPaciente->gethabitos_drogas());
    ui->txtperimetro_craneal->setText(QString::number(oPaciente->getperimetro_craneal(),'f',2));
    ui->txtPeso->setText(QString::number(oPaciente->getpeso(),'f',2));
    ui->txtProfesion->setText(oPaciente->getprofesion());

    ui->radHombre->setChecked(oPaciente->getsexo()=="H");
    ui->radMujer->setChecked(!(oPaciente->getsexo()=="H"));

    ui->chkTabaco->setChecked(oPaciente->gettabacobool());
    ui->chkotras_drogas->setChecked(oPaciente->getotras_drogasbool());
    ui->txtTalla->setText(QString::number(oPaciente->gettalla(),'f',2));

    ui->radTrabaja->setChecked(oPaciente->gettrabajabool());
    ui->radNoTrabaja->setChecked(!oPaciente->gettrabajabool());

    ui->txtnum_historiaClinica->setText(QString::number(oPaciente->getnum_historia()));
    // cargar datos Episodios
    QSqlQueryModel *modelEpisodios = new QSqlQueryModel(this);
    modelEpisodios->setQuery("select descripcion,fecha,id from episodios where id_paciente ="+QString::number(oPaciente->id)+
                             " order by fecha desc",QSqlDatabase::database("dbmedica"));
    ui->listaEpisodios->setModel(modelEpisodios);
    ui->listaEpisodios->setColumnHidden(2,true);
    //-----------------------
    // Historial Farmacología
    //-----------------------
    QSqlQueryModel *qFarmacologia = new QSqlQueryModel(this);
    qFarmacologia->setQuery("select id,medicamento from histofarma where "
                            "id_paciente = "+QString::number(oPaciente->id),QSqlDatabase::database("dbmedica"));
    qFarmacologia->setHeaderData(1,Qt::Horizontal,tr("NOMBRE DEL MEDICAMENTO"));
    ui->listaHistorialFarmacologicoTotal->setModel(qFarmacologia);
    ui->listaHistorialFarmacologicoTotal->setColumnWidth(0,0);
    ui->listaHistorialFarmacologicoTotal->setColumnWidth(1,285);

    //-----------------------
    // Historial Imagenes
    //-----------------------
    QSqlQueryModel *queryImagenes = new QSqlQueryModel(this);
    queryImagenes->setQuery("select id,descripcion from imagenes where id_paciente ="+QString::number(oPaciente->id),
                            QSqlDatabase::database("dbmedica"));
    ui->listaImagenes_pestana_dimagen->setModel(queryImagenes);
    queryImagenes->setHeaderData(1,Qt::Horizontal,tr("DESCRIPCIÓN IMAGEN"));
    ui->listaImagenes_pestana_dimagen->setColumnWidth(0,0);
    ui->listaImagenes_pestana_dimagen->setColumnWidth(1,370);

}

void FrmFichaPaciente::cargarEpisodio(int control)
{
    // Control 1 = opción cambiar estado campos y botones Cntrl = 0, no cambiar
    if(control==1)
        BloquearCamposEpisodio(false);
    else
        BloquearCamposEpisodio(true);

    // Cargo valores en pantalla
    ui->txtidentificadorEpisodio->setText(QString::number(oEpisodio->getid()));
    ui->txtDescripcionEpisodio->setText(oEpisodio->getdescripcion());
    ui->txtDescrpicionEpisodio_2->setText(oEpisodio->getdescripcion());

    ui->chkEpisodioprivado->setChecked(oEpisodio->getprivado() == 1);

    int nIndex = ui->cboDoctorEpisodio->findText(oEpisodio->getdoctor());
    if (nIndex !=-1)
        ui->cboDoctorEpisodio->setCurrentIndex(nIndex);
    else
        ui->cboDoctorEpisodio->setCurrentIndex(-1);

    ui->txtFechaEpisodio->setDate(oEpisodio->getfecha());
    ui->txtHistorialEpisodio->setPlainText(oEpisodio->gethistorial());
    ui->txtCIEEpisiodio->setText(oEpisodio->getCIE());

    llenartablahistorialfarmacologiaepisodio();
    llenartablahistorialimagenesepisodio();
    llenartablahistorialanalisisepisodio();
    llenarhistorialvisitas();
}

void FrmFichaPaciente::guardarDatosPaciente()
{
    oPaciente->setnum_historia(ui->txtnum_historiaClinica->text().toInt());

    ui->chkAlcohol->isChecked() ? oPaciente->setalcohol(1) : oPaciente->setalcohol(0);

    oPaciente->setalergias_conocidas(ui->txtalergias_conocidas->toPlainText());
    oPaciente->setantecedentes_familiares(ui->txtantecedentes_familiares->toPlainText());
    oPaciente->setcirugias_previas(ui->txtcirugias_previas->toPlainText());
    oPaciente->setdiastole(ui->txtDiastole->text().toDouble());
    oPaciente->setsistole(ui->txtSistole->text().toDouble());
    oPaciente->setenfermedadesConocidas(ui->txtEnfermedadesConocidas->toPlainText());
    oPaciente->setfamilia(ui->txtFamiliaNuclear->toPlainText());
    oPaciente->setfecha_alta(ui->txtfecha_alta->date());
    oPaciente->setfiliacion(ui->txtFiliacion->text());
    oPaciente->sethabitos_drogas(ui->txtotras_drogas->toPlainText());
    oPaciente->sethijos(ui->txtHijos->text().toInt());
    oPaciente->sethistorial(ui->txtHistorialEpisodio->toPlainText());
    oPaciente->setimc(ui->txtimc->text().toDouble());
    oPaciente->setnacimiento(ui->txtNacimiento->dateTime());
    oPaciente->setnivel_estudios(ui->cbonivel_estudios->currentText());
    oPaciente->setnum_ss(ui->txtNumeroSS->text());
    oPaciente->setperimetro_craneal(ui->txtperimetro_craneal->text().toDouble());
    oPaciente->setpeso(ui->txtPeso->text().toDouble());
    oPaciente->setprofesion(ui->txtProfesion->text());

    ui->radHombre->isChecked() ? oPaciente->setsexo("H") : oPaciente->setsexo("M");

    ui->chkTabaco->isChecked() ? oPaciente->settabaco(1) : oPaciente->settabaco(0);

    oPaciente->settalla(ui->txtTalla->text().toDouble());

    ui->radTrabaja->isChecked() ? oPaciente->settrabaja(1) : oPaciente->settrabaja(0);

    ui->chkotras_drogas->isChecked() ? oPaciente->setotras_drogas(1) : oPaciente->setotras_drogas(0);
}

void FrmFichaPaciente::AnadirDatosMedicamento(int id, QString nombre, QString codigo_nacional)
{
    QSqlQuery qTratamientos(QSqlDatabase::database("dbmedica"));
    //QSqlQueryModel *qTratamientosM = new QSqlQueryModel();

    qTratamientos.prepare("insert into histofarma (id_medicamento,medicamento,id_episodio,inicio_tratamiento,codigo_nacional)"
                           " values (:id_medicamento,:medicamento,:id_episodio,:inicio_tratamiento,:codigo_nacional)");
    qTratamientos.bindValue(":id_medicamento",id);
    qTratamientos.bindValue(":medicamento",nombre);
    qTratamientos.bindValue(":id_episodio",oEpisodio->getid());
    qTratamientos.bindValue(":inicio_tratamiento",QDate::currentDate());
    qTratamientos.bindValue(":codigo_nacional",codigo_nacional);
    if (!qTratamientos.exec()) {
        QMessageBox::warning(qApp->activeWindow(),"ERROR",
                             "No se pudo crear el registro del historial de farmacología, Error:"+
                             qTratamientos.lastError().text()
                             ,tr("Aceptar"));
    } else {
        llenartablahistorialfarmacologiaepisodio();
    }
    ui->itemFarma->setVisible(true);
}

void FrmFichaPaciente::finishedSlot(QNetworkReply* reply)
{
    //qDebug()<<reply->readAll();
     QString data=(QString)reply->readAll();
     ui->txtHistorialEpisodio->setPlainText(data);
}

void FrmFichaPaciente::on_btnBuscarCIEEpisodio_clicked()
{
//    vademecum *recuperar = new vademecum(this);
//    QStringList cDatos;
//    cDatos = recuperar->recuperarLista("http://wslatam.vademecum.es/MX/vweb/xml/ws_indication/SearchByName?value=ane");
}

void FrmFichaPaciente::BloquearCamposPaciente(bool state)
{
    ui->BtnDeshacerPaciente->setEnabled(!state);
    ui->btnGuardarPaciente->setEnabled(!state);
    ui->btnEditarPaciente->setEnabled(!state);
    ui->chkAlcohol->setEnabled(!state);
    ui->chkTabaco->setEnabled(!state);
    ui->chkotras_drogas->setEnabled(!state);
    ui->txtotras_drogas->setEnabled(!state);
    ui->txtNacimiento->setEnabled(!state);
    ui->txtfecha_alta->setEnabled(!state);
    ui->txtFamiliaNuclear->setEnabled(!state);
    ui->txtalergias_conocidas->setEnabled(!state);
    ui->txtantecedentes_familiares->setEnabled(!state);
    ui->txtDiastole->setEnabled(!state);
    ui->txtEnfermedadesConocidas->setEnabled(!state);
    ui->txtFiliacion->setEnabled(!state);
    ui->txtHijos->setEnabled(!state);
    ui->txtimc->setEnabled(!state);
    ui->txtcirugias_previas->setEnabled(!state);
    ui->txtMutua->setEnabled(!state);
    ui->txtNumeroSS->setEnabled(!state);
    ui->txtperimetro_craneal->setEnabled(!state);
    ui->txtPeso->setEnabled(!state);
    ui->txtProfesion->setEnabled(!state);
    ui->txtSistole->setEnabled(!state);
    ui->txtTalla->setEnabled(!state);
    ui->cbonivel_estudios->setEnabled(!state);
    ui->radHombre->setEnabled(!state);
    ui->radMujer->setEnabled(!state);
    ui->radTrabaja->setEnabled(!state);
    ui->radNoTrabaja->setEnabled(!state);
}

void FrmFichaPaciente::on_btnAnadirEpisodio_clicked()
{
     VaciarCamposEpisodio();
     oEpisodio->setAltas(true);
     ui->tab_episodios->setCurrentIndex(1);
     ui->txtDescripcionEpisodio->setFocus();
}

void FrmFichaPaciente::ListaEpisodio_clicked(QModelIndex index)
{
    VaciarCamposHistorialCompleto();
    int id =ui->listaEpisodios->model()->data(ui->listaEpisodios->model()->index(index.row(),2),Qt::EditRole).toInt();
    oEpisodio->RecuperarEpisodio(id);
    cargarEpisodio(1);
    llenarhistorialvisitas();
    llenartablahistorialanalisisepisodio();
    llenartablahistorialfarmacologiaepisodio();
    llenartablahistorialimagenesepisodio();

}


void FrmFichaPaciente::on_btnGuardarEpisodio_clicked()
{
    if (!ui->txtDescripcionEpisodio->text().isEmpty() && !ui->cboDoctorEpisodio->currentText().isEmpty()) {
        LLenarEpisodio();
        if (oEpisodio->getAltas()==true)
            oEpisodio->NuevoEpisodio(oPaciente->getid());
        else
            oEpisodio->GuardarEpisodio();
        BloquearCamposEpisodio(true);
    } else {
        QMessageBox::warning(this,tr("Guardar Episodio"),tr("Los campos descripción de episodio y paciente no pueden quedar vacios"),
                             tr("Aceptar"));
    }


}


void FrmFichaPaciente::BloquearCamposEpisodio(bool state)
{
     ui->chkEpisodioprivado->setEnabled(!state);
     ui->cboDoctorEpisodio->setEnabled(!state);
     ui->txtFechaEpisodio->setEnabled(!state);
     ui->txtDescripcionEpisodio->setEnabled(!state);
     ui->txtHistorialEpisodio->setEnabled(!state);
     ui->txtCIEEpisiodio->setEnabled(!state);
     ui->btnBuscarCIEEpisodio->setEnabled(!state);
     ui->btnAnadirEpisodio->setEnabled(state);
     ui->btnEditarEpisodio->setEnabled(state);
     ui->btnGuardarEpisodio->setEnabled(!state);
     ui->btnDeshacerEpisodio->setEnabled(!state);
     ui->btnEpisodioAbierto->setEnabled(!state);
     ui->btnEpisodioCerrado->setEnabled(!state);
}

void FrmFichaPaciente::LLenarEpisodio()
{
    oEpisodio->setcerrado(!ui->radEpisodioAbierto->isChecked());

    oEpisodio->setCIE(ui->txtCIEEpisiodio->text());
    oEpisodio->setdescripcion(ui->txtDescripcionEpisodio->text());
    oEpisodio->setdoctor(ui->cboDoctorEpisodio->currentText());
    oEpisodio->setfecha(ui->txtFechaEpisodio->date());
    oEpisodio->sethistorial(ui->txtHistorialEpisodio->toPlainText());
    oEpisodio->setid_paciente(oPaciente->getid_cliente());

    oEpisodio->setprivado(ui->chkEpisodioprivado->isChecked());
}

void FrmFichaPaciente::VaciarCamposEpisodio()
{
    ui->txtCIEEpisiodio->setText("");
    ui->txtDescripcionEpisodio->setText("");
    ui->cboDoctorEpisodio->setCurrentIndex(-1);
    ui->txtFechaEpisodio->setDateTime(QDateTime::currentDateTime());
    ui->txtHistorialEpisodio->setPlainText("");
    BloquearCamposEpisodio(false);
    ui->txtDescripcionEpisodio->setFocus();

}

void FrmFichaPaciente::VaciarCamposHistorialCompleto()
{
    VaciarCamposEpisodio();
    //---------------------
    // Campos Visita
    //---------------------
    ui->txtfecha_horaVisita->setDateTime(QDateTime::currentDateTime());
    ui->cboRealizadaPorDr->setCurrentIndex(-1);
    ui->txtExploracion->clear();
    ui->txtLengua->clear();
    ui->txtPulso->clear();
    ui->txtDiagnosticoVisita->clear();
    ui->txtTratamiento->clear();

    //---------------------
    // Datos Farmacología
    //---------------------
    ui->txtinicio_tratamientoFarma->setDate(QDate::currentDate());
    ui->txtPosologiaFarma->clear();
    ui->chkactivo->setChecked(false);
    ui->txcomentariosFarma->clear();
    ui->itemFarma->setVisible(false);

    //----------------------
    // Analiticas
    //----------------------
    ui->listaAnaliticas->clear();
    ui->itemAnalitica->setVisible(false);

    //----------------------
    // Imagenes
    //----------------------
    ui->lineEdit_descripcionimagen->clear();
    ui->dateEdit_fecha_imagen->setDate(QDate::currentDate());
    ui->comboBox_tipo_imagen->setCurrentIndex(-1);
    ui->checkBox_evaluada->setChecked(false);
    ui->lblImagenDiagnostico->clear();
    ui->lblImagenDiagnostico->setText(tr("Sin imagen"));
    ui->txcomentariosImagen->clear();
    ui->itemImagenes->setVisible(false);
    //-----------------------
    // Referidos
    //-----------------------
    ui->txtReferidoDe->clear();
    ui->cboMotivoInterconsultas->setCurrentIndex(-1);
    ui->txtDeivaraInterconsulta->clear();
    ui->cboMotivoDeivacionInterconsulta->setCurrentIndex(-1);
    ui->txtPeticionInterconsulta->clear();
    ui->txtRespuestaInterconsulta->clear();
    ui->tablaInterconsultas->clear();
    ui->itemInterConsulta->setVisible(false);
    //------------------------
    // Avisos
    //------------------------
    ui->txtMotivoAviso->clear();
    ui->txtAvisar_desde->setDateTime(QDateTime::currentDateTime());
    ui->txtAvisar_hasta->setDateTime(QDateTime::currentDateTime());
    ui->txtTextoAviso->clear();
    ui->chkAvisoCerrado->setChecked(false);
    ui->listaAvisos->clear();
    ui->itemAvisos->setVisible(false);
}

void FrmFichaPaciente::BloquearCamposImagen()
{
    ui->lineEdit_descripcionimagen->setEnabled(false);
    ui->txcomentariosImagen->setEnabled(false);
    ui->dateEdit_fecha_imagen->setEnabled(false);
    ui->pushButton_GuardarImagen->setEnabled(false);
    ui->pushButton_editarimagen->setEnabled(true);
    ui->pushButton_DeshacerImagen->setEnabled(false);
    ui->comboBox_tipo_imagen->setEnabled(false);
    ui->checkBox_evaluada->setEnabled(false);
}

void FrmFichaPaciente::on_btnEditarPaciente_clicked()
{
    BloquearCamposPaciente(false);
    ui->txtFamiliaNuclear->setFocus();
}

void FrmFichaPaciente::on_btnGuardarPaciente_clicked()
{
    BloquearCamposPaciente(true);
    guardarDatosPaciente();
    oPaciente->GuardarPaciente();
}





void FrmFichaPaciente::on_btnEditarEpisodio_clicked()
{
    BloquearCamposEpisodio(false);
    ui->txtDescripcionEpisodio->setFocus();
}

void FrmFichaPaciente::on_btnDeshacerEpisodio_clicked()
{
    cargarEpisodio(0);
    BloquearCamposEpisodio(true);
}

void FrmFichaPaciente::on_btnAnadirFarma_clicked()
{
    FrmAnadirMedicamento nuevomed(this);
    nuevomed.setWindowState(Qt::WindowMaximized);
    connect(&nuevomed, SIGNAL(datos(int, QString,QString)), this, SLOT(AnadirDatosMedicamento(int, QString,QString)));
    nuevomed.exec();
}

void FrmFichaPaciente::on_btnAnadirHerb_clicked()
{
    frmAnadirHierb nuevahierba(this);
    nuevahierba.exec();
}

void FrmFichaPaciente::BuscarCIE()
{
    FrmBuscarCIE BuscarCIE(this);
    connect(&BuscarCIE,SIGNAL(emitirCIE(int,QString,QString )), this, SLOT(RecuperarCIE(int,QString,QString)));
    BuscarCIE.exec();
}

void FrmFichaPaciente::RecuperarCIE(int id, QString codigo, QString descripcion)
{
    oEpisodio->setidcie(id);
    oEpisodio->setcodigocie(codigo);
    oEpisodio->setCIE(descripcion);
    ui->txtCIEEpisiodio->setText(descripcion);
}

void FrmFichaPaciente::ActivarControlesFarmacologia()
{
    if(ui->listaTratamientosFarma->currentRow()>-1) {
        ui->btnEditarFarmacologia->setEnabled(false);
        ui->btnGuardarFarmacologia->setEnabled(true);
        ui->btnDeshacerFarmacologia->setEnabled(false);
        ui->txtinicio_tratamientoFarma->setEnabled(true);
        ui->txtPosologiaFarma->setEnabled(true);
        ui->txcomentariosFarma->setEnabled(true);
        ui->chkactivo->setEnabled(true);
        ui->txtinicio_tratamientoFarma->setFocus();
    } else
        QMessageBox::warning(this,tr("Editar historial Farmacología"),tr("Antes de editar debe seleccionar una ficha de Farmaco"),
                             tr("Aceptar"));
}

void FrmFichaPaciente::GuardarDatosFarmacologia()
{
    ui->btnEditarFarmacologia->setEnabled(true);
    ui->btnGuardarFarmacologia->setEnabled(false);
    ui->btnDeshacerFarmacologia->setEnabled(false);
    ui->txtinicio_tratamientoFarma->setEnabled(false);
    ui->txtPosologiaFarma->setEnabled(false);
    ui->txcomentariosFarma->setEnabled(false);
    ui->chkactivo->setEnabled(false);

    Farmacologia oFarmacologia;

    oFarmacologia.setid_episodio(oEpisodio->getid());
    oFarmacologia.setactivo(ui->chkactivo->isChecked());
    oFarmacologia.secomentarios(ui->txcomentariosFarma->toPlainText());
    oFarmacologia.setindicacion_posologia(ui->txtPosologiaFarma->toPlainText());
    oFarmacologia.setinicio_tratamiento(ui->txtinicio_tratamientoFarma->date());
    oFarmacologia.setmedicamento(ui->listaTratamientosFarma->item(ui->listaTratamientosFarma->currentRow(),0)->text());
    oFarmacologia.modificarFarmaco(ui->listaTratamientosFarma->item(ui->listaTratamientosFarma->currentRow(),1)->text().toInt());

    llenartablahistorialfarmacologiaepisodio();
}

void FrmFichaPaciente::llenartablahistorialfarmacologiaepisodio()
{
    // Cargar farmacología episodio
    QStringList list;
    list <<"NOMBRE"<<"id";
    QSqlQuery qFarma(QSqlDatabase::database("dbmedica"));
    QString cSQL = " Select medicamento, id,id_medicamento,activo from histofarma where id_episodio = :id ";
    qFarma.prepare(cSQL);
    qFarma.bindValue(":id",oEpisodio->getid());
    ui->listaTratamientosFarma->setRowCount(0);
    ui->listaTratamientosFarma->setColumnCount(3);
    ui->listaTratamientosFarma->setColumnWidth(0,220);
    ui->listaTratamientosFarma->setColumnWidth(1,0);
    ui->listaTratamientosFarma->setColumnWidth(2,0);
    ui->listaTratamientosFarma->setHorizontalHeaderLabels(list);
    int pos = 0;
    QSqlRecord reg ;
    // Relleno la tabla
    if (qFarma.exec())
    {
        while (qFarma.next())
        {
            // Nombre Medicamento
            reg = qFarma.record();
            if ((ui->radversoloactivosfarmacologia->isChecked()
                 && reg.field("activo").value().toInt()==1)
                 || ui->radvertodosfarmacologia->isChecked() )
            {
                ui->listaTratamientosFarma->setRowCount(pos+1);
                QTableWidgetItem *newItem = new QTableWidgetItem(reg.field("medicamento").value().toString());
                // para que los elementos no sean editables
                newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                if (reg.field("activo").value().toInt() ==1)
                    newItem->setTextColor(Qt::blue); // color de los Medicamentos activos
                else
                    newItem->setTextColor(Qt::darkGray); // color de los Medicamentos NO activos
                ui->listaTratamientosFarma->setItem(pos,0,newItem);

                // id HistorialMedicamento
                QTableWidgetItem *newItem1 = new QTableWidgetItem(QString::number(reg.field("id").value().toInt()));
                // para que los elementos no sean editables
                newItem1->setFlags(newItem1->flags() & (~Qt::ItemIsEditable));
                newItem1->setTextColor(Qt::blue); // color de los items
                ui->listaTratamientosFarma->setItem(pos,1,newItem1);


                // id Medicamento
                QTableWidgetItem *newItem2 = new QTableWidgetItem(QString::number(reg.field("id_medicamento").value().toInt()));
                // para que los elementos no sean editables
                newItem2->setFlags(newItem2->flags() & (~Qt::ItemIsEditable));
                newItem2->setTextColor(Qt::blue); // color de los items
                ui->listaTratamientosFarma->setItem(pos,2,newItem2);

                pos++;
            }
        }
    }
    ui->itemFarma->setVisible(pos >0);
    ui->btnEditarFarmacologia->setEnabled(pos >0);
}

void FrmFichaPaciente::llenartablahistorialimagenesepisodio()
{
    // Cargar imagenes episodio
    QStringList list;
    list <<QObject::tr("DESCRIPCIÓN")<< QObject::tr("FECHA") <<QObject::tr("TIPO") <<"id";
    QSqlQuery qImagenes(QSqlDatabase::database("dbmedica"));
    QString cSQL = " Select descripcion, id,fecha_imagen, evaluada from imagenes where id_episodio = :id ";
    qImagenes.prepare(cSQL);
    qImagenes.bindValue(":id",oEpisodio->getid());
    ui->listaImagenes->setRowCount(0);
    ui->listaImagenes->setColumnCount(4);
    ui->listaImagenes->setColumnWidth(0,250);
    ui->listaImagenes->setColumnWidth(1,170);
    ui->listaImagenes->setColumnWidth(2,120);
    ui->listaImagenes->setColumnWidth(3,0);
    ui->listaImagenes->setHorizontalHeaderLabels(list);
    int pos = 0;
    QSqlRecord reg ;
    // Relleno la tabla
     QString cFecha;
     bool MostrarIcono = false;
    if (qImagenes.exec()) {
        while (qImagenes.next()) {
            // Nombre Medicamento
            MostrarIcono = true;
            reg = qImagenes.record();
            if ((ui->radImagenEvaluada->isChecked() && reg.field("evaluada").value().toInt()==1)
                    || !ui->radImagenEvaluada->isChecked() ) {

                ui->listaImagenes->setRowCount(pos+1);
                QTableWidgetItem *newItem = new QTableWidgetItem(reg.field("descripcion").value().toString());
                // para que los elementos no sean editables
                newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
                if (reg.field("evaluada").value().toInt() ==1)
                    newItem->setTextColor(Qt::blue); // color de las imagenes evaluados
                else
                    newItem->setTextColor(Qt::darkGray); // color de las imagenes NO evaluadas
                ui->listaImagenes->setItem(pos,0,newItem);

                // fecha
                cFecha = reg.field("fecha_imagen").value().toDateTime().toString("dd/MM/yyyy : hh:mm");
                QTableWidgetItem *newItem1 = new QTableWidgetItem(cFecha);
                // para que los elementos no sean editables
                newItem1->setFlags(newItem1->flags() & (~Qt::ItemIsEditable));
                newItem1->setTextColor(Qt::blue); // color de los items
                ui->listaImagenes->setItem(pos,1,newItem1);

                // tipo
                QTableWidgetItem *newItem2 = new QTableWidgetItem(reg.field("tipo_imagen").value().toString());
                // para que los elementos no sean editables
                newItem2->setFlags(newItem2->flags() & (~Qt::ItemIsEditable));
                newItem2->setTextColor(Qt::blue); // color de los items
                ui->listaImagenes->setItem(pos,2,newItem2);

                // id
                QTableWidgetItem *newItem3 = new QTableWidgetItem(reg.field("id").value().toString());
                // para que los elementos no sean editables
                newItem3->setFlags(newItem3->flags() & (~Qt::ItemIsEditable));
                newItem3->setTextColor(Qt::blue); // color de los items
                ui->listaImagenes->setItem(pos,3,newItem3);

                pos++;
            }
        }
        if(MostrarIcono == true)
            ui->itemImagenes->setVisible(true);
        else
            ui->itemImagenes->setVisible(false);
    }
}

void FrmFichaPaciente::llenartablahistorialanalisisepisodio()
{
    // Cargar imagenes episodio
    QStringList list;
    list <<QObject::tr("ANALISIS")<< QObject::tr("FECHA") <<"id" <<tr("COMENTARIOS");
    QSqlQuery qAnalisis(QSqlDatabase::database("dbmedica"));
    QString cSQL = " Select analisis, id,fecha_analisis, comentarios from Analitica where id_episodio = :id ";
    qAnalisis.prepare(cSQL);
    qAnalisis.bindValue(":id",oEpisodio->getid());
    ui->listaAnaliticas->setRowCount(0);
    ui->listaAnaliticas->setColumnCount(4);
    ui->listaAnaliticas->setColumnWidth(0,230);
    ui->listaAnaliticas->setColumnWidth(1,170);
    ui->listaAnaliticas->setColumnWidth(2,0);
    ui->listaAnaliticas->setColumnWidth(3,150);
    ui->listaAnaliticas->setHorizontalHeaderLabels(list);
    int pos = 0;
    QSqlRecord reg ;
    // Relleno la tabla
     QString cFecha;
    if (qAnalisis.exec()) {
        while (qAnalisis.next()) {
            // Nombre Medicamento
            reg = qAnalisis.record();

            ui->listaAnaliticas->setRowCount(pos+1);
            QTableWidgetItem *newItem = new QTableWidgetItem(reg.field("analisis").value().toString());
            // para que los elementos no sean editables
            newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
            newItem->setTextColor(Qt::blue); // color de las imagenes evaluados
            ui->listaAnaliticas->setItem(pos,0,newItem);

            // fecha
            cFecha = reg.field("fecha_analisis").value().toDateTime().toString("dd/MM/yyyy : hh:mm");
            QTableWidgetItem *newItem1 = new QTableWidgetItem(cFecha);
            // para que los elementos no sean editables
            newItem1->setFlags(newItem1->flags() & (~Qt::ItemIsEditable));
            newItem1->setTextColor(Qt::blue); // color de los items
            ui->listaAnaliticas->setItem(pos,1,newItem1);

            // id
            QTableWidgetItem *newItem2 = new QTableWidgetItem(reg.field("id").value().toString());
            // para que los elementos no sean editables
            newItem2->setFlags(newItem2->flags() & (~Qt::ItemIsEditable));
            newItem2->setTextColor(Qt::blue); // color de los items
            ui->listaAnaliticas->setItem(pos,2,newItem2);

            // comentarios
            QTableWidgetItem *newItem3 = new QTableWidgetItem(reg.field("comentarios").value().toString());
            // para que los elementos no sean editables
            newItem3->setFlags(newItem3->flags() & (~Qt::ItemIsEditable));
            newItem3->setTextColor(Qt::blue); // color de los items
            ui->listaAnaliticas->setItem(pos,3,newItem3);

            pos++;
        }
    }
}




void FrmFichaPaciente::BorrarDatosMedicamento()
{
    if(!ui->listaTratamientosFarma->selectedItems().isEmpty())
    {
        if(QMessageBox::question(this,
                                 tr("Borrar medicamento historial"),
                                 tr("Va a proceder a borrar un medicamento del historial,")+
                                 tr("¿Desea continuar?"),
                                 tr("Cancelar"),
                                 tr("Borrar")) ==QMessageBox::Accepted)
        {
            Farmacologia oFarma;
            oFarma.borrarFarmaco(ui->listaTratamientosFarma->item(ui->listaTratamientosFarma->currentRow(),1)->text().toInt());
            llenartablahistorialfarmacologiaepisodio();
        }
    }
}

void FrmFichaPaciente::MostrarFichaMedicamento()
{
    if(!ui->listaTratamientosFarma->selectedItems().isEmpty())
    {
        QSqlQuery qFarma(QSqlDatabase::database("dbmedica"));
        QString cSQL = "select codigo_nacional from histofarma where id =:id";
        qFarma.prepare(cSQL);
        qFarma.bindValue(":id",ui->listaTratamientosFarma->item(ui->listaTratamientosFarma->currentRow(),1)->text());
        if(qFarma.exec()) {
            qFarma.next();
            QString codigo_nacional = qFarma.value(0).toString();
            FrmInformacionFarmaco frmFarmaco(this);
            frmFarmaco.capturarid(codigo_nacional);
            frmFarmaco.setModal(true);
            frmFarmaco.setWindowModality(Qt::WindowModal);
            frmFarmaco.exec();
        }
    }
}
void FrmFichaPaciente::MostrarFichaMedicamento2()
{

    QSqlQuery qFarma(QSqlDatabase::database("dbmedica"));
    QString cSQL = "select codigo_nacional from histofarma where id =:id";
    qFarma.prepare(cSQL);
    qFarma.bindValue(":id",this->id_farmaco);
    if(qFarma.exec()) {
        qFarma.next();
        QString codigo_nacional = qFarma.value(0).toString();
        FrmInformacionFarmaco frmFarmaco(this);
        frmFarmaco.capturarid(codigo_nacional);
        frmFarmaco.setModal(true);
        frmFarmaco.setWindowModality(Qt::WindowModal);
        frmFarmaco.exec();
    }

}
void FrmFichaPaciente::AnadirImagenDiagnostico()
{
    if (oEpisodio->getid() >0)
    {
        int id;
        QSqlQuery queryImagenes(QSqlDatabase::database("dbmedica"));
        queryImagenes.prepare("INSERT INTO `emp0999med`.`imagenes`"
                              "(`id_episodio`) VALUES (:id_episodio);");
        queryImagenes.bindValue(":id_episodio",oEpisodio->id);
        if (queryImagenes.exec()){
            id = queryImagenes.lastInsertId().toInt();
            FrmAnadirImagen imagen;
            imagen.Recuperarid(id);
            imagen.adjustSize();
            imagen.move(QApplication::desktop()->screen()->rect().center() - imagen.rect().center());
            imagen.exec();
        }
    }
    else
        QMessageBox::warning(this,tr("Añadir imagenes a episodio"),
                             tr("Debe seleccionar o crear un episodio antes de añadir una nueva imagen diagnostica"),
                             tr("Aceptar"));
   llenartablahistorialimagenesepisodio();
}

void FrmFichaPaciente::BorrarImagenDiagnostico()
{
    oImagenes->BorrarImagen(ui->listaImagenes->item(ui->listaImagenes->currentRow(),3)->text().toInt());
    llenartablahistorialimagenesepisodio();
}

void FrmFichaPaciente::EditarImagenDiagnostico()
{
    if(ui->listaImagenes->currentRow()>=0) {
        ui->lineEdit_descripcionimagen->setEnabled(true);
        ui->comboBox_tipo_imagen->setEnabled(true);
        ui->dateEdit_fecha_imagen->setEnabled(true);
        ui->txcomentariosImagen->setEnabled(true);
        ui->pushButton_editarimagen->setEnabled(false);
        ui->pushButton_GuardarImagen->setEnabled(true);
        ui->pushButton_DeshacerImagen->setEnabled(true);
        ui->checkBox_evaluada->setEnabled(true);
        ui->lineEdit_descripcionimagen->setFocus();
     } else
        QMessageBox::warning(this,tr("Editar imágenes"),tr("Debe seleccionar en la lista que imágen editar"),
                             tr("Aceptar"));
}

void FrmFichaPaciente::recibedatospaciente(int historia, QString Nombre)
{
   // ui->txtnum_historiaClinica->setText(QString::number(historia));
    ui->txtPaciente->setText(Nombre);
}

void FrmFichaPaciente::AnadirInterconsulta()
{
    Interconsulta oInterconsulta(this);
    oInterconsulta.AnadirInterconsulta(oEpisodio->getid(),oPaciente->getid());
}

void FrmFichaPaciente::AnadirAnalitica()
{
    if(!oEpisodio->getid()==0) {
        FrmAnalitica frmAnalitica(this);
        Analitica oAnalitica(this);
        oAnalitica.setid_episodio(oEpisodio->getid());
        oAnalitica.id_paciente=oEpisodio->id_paciente;
        oAnalitica.AnadirAnalitica();
        frmAnalitica.capturaid(oAnalitica.getid());
        frmAnalitica.capturaPaciente(ui->txtPaciente->text());
        frmAnalitica.exec();
    } else
        QMessageBox::warning(this,tr("Analitica"),tr("Debe seleccionar un episodio antes de poder añadir una antalítica"), tr("Aceptar"));
    llenartablahistorialanalisisepisodio();
}

void FrmFichaPaciente::BorrarAnalitica()
{    
    if(!ui->listaAnaliticas->selectedItems().isEmpty())
    {
        Analitica oAnalitica;
        int nid = ui->listaAnaliticas->item(ui->listaAnaliticas->currentRow(),2)->text().toInt();
        oAnalitica.EliminarAnalitica(nid);
        llenartablahistorialanalisisepisodio();
    }
}


void FrmFichaPaciente::VerAnalitica()
{
    if(!oEpisodio->getid()==0 &&
       !ui->listaAnaliticas->selectedItems().isEmpty())
    {
        FrmVerAnalitica frmAnalitica;
        Analitica oAnalitica(this);
        frmAnalitica.setWindowModality(Qt::WindowModal);

        oAnalitica.setid_episodio(oEpisodio->getid());
        oAnalitica.setid(ui->listaAnaliticas->item(ui->listaAnaliticas->currentRow(),2)->text().toInt());

        frmAnalitica.capturaid(oAnalitica.getid());
        frmAnalitica.capturaPaciente(ui->txtPaciente->text());
        frmAnalitica.exec();
        llenartablahistorialanalisisepisodio();
    } else
        QMessageBox::warning(this,tr("Analitica"),tr("Debe seleccionar un episodio antes de poder añadir una antalítica"), tr("Aceptar"));    
}


void FrmFichaPaciente::cargarDatosMedicamento(int crow, int ccol)
{
    Farmacologia oFarmacologia;
    QString cSQL;
    QString id = ui->listaTratamientosFarma->item(crow,1)->text();
    cSQL = "Select * from histofarma where id="+id;
    oFarmacologia.cargarDatos(cSQL);
    ui->txtinicio_tratamientoFarma->setDate(oFarmacologia.getinicio_tratamiento());
    ui->chkactivo->setChecked(oFarmacologia.getactivo()==1);
    ui->txcomentariosFarma->setPlainText(oFarmacologia.gecomentarios());
    ui->txtPosologiaFarma->setPlainText(oFarmacologia.getindicacion_posologia());
}

void FrmFichaPaciente::cargarDatosImagenes(int crow, int ccol)
{
    QString id = ui->listaImagenes->item(crow,3)->text();

    oImagenes->llenarObjetoconDatosDB(id.toInt());
    ui->txcomentariosImagen->setPlainText(oImagenes->Comentarios);
    ui->lineEdit_descripcionimagen->setText(oImagenes->Descripcion);
    ui->dateEdit_fecha_imagen->setDate(oImagenes->fecha_imagen);

    int nindex = ui->comboBox_tipo_imagen->findText(oImagenes->DevolverTexto_tipo_imagen( oImagenes->id_tipo_imagen));
    if (nindex >-1)
        ui->comboBox_tipo_imagen->setCurrentIndex(nindex);
    else
        ui->comboBox_tipo_imagen->clear();

    QSqlQuery qryImagenes(QSqlDatabase::database("dbmedica"));
    qryImagenes.prepare("Select imagen from imagenes where id = :id");
    qryImagenes.bindValue(":id",id);
    if (qryImagenes.exec()) {
        if (qryImagenes.next()){
            //------------------------------
            // Insertamos imagen en pantalla
            //------------------------------
            QByteArray ba1 = qryImagenes.record().field("imagen").value().toByteArray();
            QPixmap pm1;
            pm1.loadFromData(ba1);
            if(!qryImagenes.record().field("imagen").value().isNull()){
                ui->lblImagenDiagnostico->setPixmap(pm1);
                ui->lblImagenDiagnostico->setScaledContents(true);
            } else {
                ui->lblImagenDiagnostico->clear();
                ui->lblImagenDiagnostico->setText(tr("Sin imagen"));
            }
        } else {
            QMessageBox::warning(this,tr("ATENCIÓN"),
                                 tr("no se ha encontrado ningún registro de imagen"),
                                 tr("Aceptar"));
        }
    } else{
        QMessageBox::warning(this,tr("ATENCIÓN"),
        tr("Se ha producido un error durante la recuperación de la imagen: %1").arg(qryImagenes.lastError().text()),
                             tr("Aceptar"));

    }
    ui->checkBox_evaluada->setChecked(oImagenes->Evaluada==1);
}

void FrmFichaPaciente::guardarDatosImagenes()
{
    QString cid = ui->listaImagenes->item(ui->listaImagenes->currentRow(),3)->text();
    oImagenes->id = cid.toInt();
    oImagenes->Comentarios = ui->txcomentariosImagen->toPlainText();
    oImagenes->Descripcion = ui->lineEdit_descripcionimagen->text();
    oImagenes->fecha_imagen = ui->dateEdit_fecha_imagen->date();

    oImagenes->Evaluada = ui->checkBox_evaluada->isChecked();

    oImagenes->id_tipo_imagen = oImagenes->Devolverid_tipo_imagen(ui->comboBox_tipo_imagen->currentText());
    oImagenes->guardarDatosDB();
    BloquearCamposImagen();
}

void FrmFichaPaciente::deshacerDatosImagenes()
{
    cargarDatosImagenes(ui->listaImagenes->currentRow(),3);
    BloquearCamposImagen();
}

void FrmFichaPaciente::on_BtnDeshacerPaciente_clicked()
{
    //TODO FrmFichaPaciente::on_BtnDeshacerPaciente_clicked()
}

void FrmFichaPaciente::AnadirVisita()
{
//    frmVisitas frmvisitas;
//    frmvisitas.exec();
    if (!ui->cboRealizadaPorDr->currentText().isEmpty()){
        Visitas *oVisita = new Visitas(this);
        int nidVisita;
        nidVisita = oVisita->AnadirVisita(oEpisodio->id,ui->cboRealizadaPorDr->currentText());
        if (nidVisita >0){
            CargarVisita(nidVisita);
            ui->txtfecha_horaVisita->setEnabled(true);
            ui->txtfecha_horaVisita->setDateTime(QDateTime::currentDateTime());
            ui->txtExploracion->setEnabled(true);
            ui->txtLengua->setEnabled(true);
            ui->txtPulso->setEnabled(true);
            ui->txtTratamiento->setEnabled(true);
            ui->txtDiagnosticoVisita->setEnabled(true);
            ui->btnAnadirVisita->setEnabled(false);
            ui->btnGuardarVisita->setEnabled(true);
            ui->btnDeshacerVisita->setEnabled(true);
            ui->txtExploracion->setFocus();
            llenarhistorialvisitas();
        }
    }

}
void FrmFichaPaciente::llenarhistorialvisitas()
{

    QSqlQueryModel *modelVisitas = new QSqlQueryModel(this);
    modelVisitas->setQuery("Select fecha_hora,medico,id from visitas where id_episodio ="+QString::number(oEpisodio->getid())+" order by fecha_hora desc",
                           QSqlDatabase::database("dbmedica"));
    ui->listaVisitas->setModel(modelVisitas);

}


void FrmFichaPaciente::ListaVisita_Clicked(QModelIndex index)
{
    int id =ui->listaVisitas->model()->data(ui->listaVisitas->model()->index(index.row(),2),Qt::EditRole).toInt();
    CargarVisita(id);
}

void FrmFichaPaciente::CargarVisita(int nid)
{

    oVisita->RecuperarVisita(nid);
    ui->txtfecha_horaVisita->setDateTime(oVisita->fecha_hora);
    ui->txtExploracion->setText(oVisita->exploracion);
    ui->txtLengua->setText(oVisita->lengua);
    ui->txtPulso->setText(oVisita->pulso);
    ui->txtTratamiento->setText(oVisita->tratamiento);
    ui->txtDiagnosticoVisita->setText(oVisita->diagnostico);
    ui->cboRealizadaPorDr->setCurrentIndex(ui->cboRealizadaPorDr->findText(oVisita->medico));
    ui->btnEditarVisita->setEnabled(true);
    QSqlQueryModel *modelSindromes = new QSqlQueryModel(this);
    modelSindromes->setQuery("select id,sindrome from sindromes where id_visita ="+QString::number(oVisita->id),
                             QSqlDatabase::database("dbmedica"));
    ui->tablaSindromes->setModel(modelSindromes);
    ui->tablaSindromes->setColumnWidth(0,0);
    ui->tablaSindromes->setColumnWidth(1,250);
    modelSindromes->setHeaderData(1,Qt::Horizontal,tr("Sindrome"));

}

void FrmFichaPaciente::bntEditarVisita_clicked()
{
    //-------------------
    // CONTROLES
    //-------------------
    ui->btnEditarVisita->setEnabled(false);
    ui->btnAnadirVisita->setEnabled(false);
    ui->listaVisitas->setEnabled(false);
    ui->btnGuardarVisita->setEnabled(true);
    ui->btnDeshacerVisita->setEnabled(true);
    ui->txtfecha_horaVisita->setEnabled(true);
    ui->txtExploracion->setEnabled(true);
    ui->txtLengua->setEnabled(true);
    ui->txtPulso->setEnabled(true);
    ui->txtTratamiento->setEnabled(true);
    ui->txtDiagnosticoVisita->setEnabled(true);
    ui->txtExploracion->setFocus();
}

void FrmFichaPaciente::btnGuardarVisita_clicked()
{
    oVisita->id_episodio = oEpisodio->id;
    oVisita->fecha_hora = ui->txtfecha_horaVisita->dateTime();
    oVisita->medico = ui->cboRealizadaPorDr->currentText();
    oVisita->exploracion = ui->txtExploracion->toPlainText();
    oVisita->tratamiento = ui->txtTratamiento->toPlainText();
    oVisita->lengua = ui->txtLengua->toPlainText();
    oVisita->pulso = ui->txtPulso->toPlainText();
    oVisita->diagnostico =ui->txtDiagnosticoVisita->toPlainText();

    oVisita->GuardarDatos();
    //-------------------
    // CONTROLES
    //-------------------
    ui->btnEditarVisita->setEnabled(true);
    ui->btnAnadirVisita->setEnabled(true);
    ui->listaVisitas->setEnabled(true);
    ui->btnGuardarVisita->setEnabled(false);
    ui->btnDeshacerVisita->setEnabled(false);
    ui->txtfecha_horaVisita->setEnabled(false);
    ui->txtExploracion->setEnabled(false);
    ui->txtLengua->setEnabled(false);
    ui->txtPulso->setEnabled(false);
    ui->txtTratamiento->setEnabled(false);
    ui->txtDiagnosticoVisita->setEnabled(false);

}

void FrmFichaPaciente::btnDeshacerVisita_clicked()
{
    //-------------------
    // CONTROLES
    //-------------------
    ui->btnEditarVisita->setEnabled(true);
    ui->btnAnadirVisita->setEnabled(true);
    ui->listaVisitas->setEnabled(true);
    ui->btnGuardarVisita->setEnabled(false);
    ui->btnDeshacerVisita->setEnabled(false);
    ui->txtfecha_horaVisita->setEnabled(false);
    ui->txtExploracion->setEnabled(false);
    ui->txtLengua->setEnabled(false);
    ui->txtPulso->setEnabled(false);
    ui->txtTratamiento->setEnabled(false);
    ui->txtDiagnosticoVisita->setEnabled(false);
    oVisita->RecuperarVisita(oVisita->id);
}

void FrmFichaPaciente::anadirDiagnostico()
{
    FrmAnadirDiagnostico frmDiag(this);
    if( frmDiag.exec() == QDialog::Accepted){
        // Cargamos datos de la BD
        int id = frmDiag.id;
        Configuracion_global->AbridBDMediTec();
        QSqlQuery QueryMediTec(QSqlDatabase::database("db_meditec"));
        if(QueryMediTec.exec("select sindrome from sindromes where id ="+QString::number(id))){
            if(QueryMediTec.next()){
                QSqlQuery querySindromes(QSqlDatabase::database("dbmedica"));
                querySindromes.prepare("INSERT INTO sindromes (id_visita,idsindrome,sindrome)"
                                       " VALUES (:id_visita,:idsindrome,:sindrome);");
                querySindromes.bindValue(":id_visita",oVisita->id);
                querySindromes.bindValue(":idsindrome",QueryMediTec.record().value("id").toInt());
                querySindromes.bindValue(":sindrome",QueryMediTec.record().value("sindrome").toString());
                if(!querySindromes.exec())
                    QMessageBox::warning(this,tr("ATENCIÓN"),
                    tr("Ha fallado la inserción de registros de Sindromes: %1").arg(querySindromes.lastError().text()));
                else {
                    QSqlQueryModel *modelSindromes = new QSqlQueryModel(this);
                    modelSindromes->setQuery("select id,sindrome from sindromes where id_visita ="+QString::number(oVisita->id),
                                             QSqlDatabase::database("dbmedica"));
                    ui->tablaSindromes->setModel(modelSindromes);
                    ui->tablaSindromes->setRowHeight(0,0);
                    ui->tablaSindromes->setColumnWidth(0,0);
                    ui->tablaSindromes->setColumnWidth(1,250);
                    modelSindromes->setHeaderData(1,Qt::Horizontal,tr("Sindrome"));
                }

            }

        }
        Configuracion_global->CerrarBDMediTec();

    }

}

void FrmFichaPaciente::vademecums()
{
FrmMTCbase base;
base.cargarSindromes(oVisita->id);
base.setWindowState(Qt::WindowMaximized);
base.exec();
}

void FrmFichaPaciente::ListaHistorialFarmacologia_clicked(QModelIndex index)
{
    this->id_farmaco =ui->listaHistorialFarmacologicoTotal->model()->data(ui->listaHistorialFarmacologicoTotal->model()->index(index.row(),0),Qt::EditRole).toInt();
    QSqlQuery queryFarmacologia(QSqlDatabase::database("dbmedica"));
    queryFarmacologia.prepare("select * from histofarma where id = :id");
    queryFarmacologia.bindValue(":id",this->id_farmaco);
    if(queryFarmacologia.exec()){
        queryFarmacologia.next();
        ui->txcomentariosFarmaco_PestanaHFarmacologia->setPlainText(queryFarmacologia.record().value("comentarios").toString());
        ui->cboFechainicio_tratamiento_pestana_H_Farmacologico->setDate(
                    queryFarmacologia.record().value("inicio_tratamiento").toDate());
        ui->txtPosologiaIndicada_PestanaHFarmacologico->setText(
                    queryFarmacologia.record().value("indicacion_posologia").toString());
    } else {
        ui->txcomentariosFarmaco_PestanaHFarmacologia->clear();
        ui->cboFechainicio_tratamiento_pestana_H_Farmacologico->clear();
        ui->txtPosologiaIndicada_PestanaHFarmacologico->clear();
        QMessageBox::warning(this,tr("ATENCIÓN"),tr("No se ha encontrado el registro del medicamento"),tr("Aceptar"));

    }
}

void FrmFichaPaciente::ListaHistorialImagenes_clicked(QModelIndex index)
{
    int id =ui->listaImagenes_pestana_dimagen->model()->data(ui->listaImagenes_pestana_dimagen->model()->index(index.row(),0),Qt::EditRole).toInt();
    QSqlQuery queryImagen(QSqlDatabase::database("dbmedica"));
    queryImagen.prepare("select * from imagenes where id = :id");
    queryImagen.bindValue(":id",id);
    if(queryImagen.exec()){
        queryImagen.next();

        ui->txcomentariosImagen_PestanaDImagen->setPlainText(queryImagen.record().value("comentarios").toString());
        ui->txtFechatomaimagen_pestana_Dimagen->setDate(
                    queryImagen.record().value("fecha_imagen").toDate());

         //------------------------------
         // Insertamos imagen en pantalla
         //------------------------------
         QByteArray ba1 = queryImagen.record().field("imagen").value().toByteArray();
         QPixmap pm1;
         pm1.loadFromData(ba1);
         if(!queryImagen.record().field("imagen").value().isNull()){
             ui->lblimagengrande->setPixmap(pm1);
             ui->lblimagengrande->setScaledContents(true);
         } else {
             ui->lblimagengrande->clear();
             ui->lblimagengrande->setText(tr("Sin imagen"));
         }
    } else {
        ui->txcomentariosImagen_PestanaDImagen->clear();
        ui->txtFechatomaimagen_pestana_Dimagen->clear();
        QMessageBox::warning(this,tr("ATENCIÓN"),tr("No se ha encontrado el registro de la imágen"),tr("Aceptar"));

    }
}





