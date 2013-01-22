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
#include "frmagendavisitas.h"
#include "sqlcalls.h"
#include "paciente.h"
#include "episodio.h"
#include <QMessageBox>
#include "frmanadirmedicamento.h"
#include "vademecum.h"
#include "frmbuscarcie.h"
#include "farmacologia.h"
#include "frminformacionfarmaco.h"
#include "frmanadirimagen.h"
#include <QDesktopWidget>
#include "frmvisitas.h"
#include "interconsulta.h"
#include "frmanalitica.h"
#include "analitica.h"
#include "frmveranalitica.h"
#include <QInputDialog>

//TODO integrar http://doc.ginkgo-cadx.com/ginkgo-integration/ginkgo-cadx-integration-input-xml-integration/

FrmFichaPaciente::FrmFichaPaciente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmFichaPaciente)
{
    oPaciente = new Paciente();
    oEpisodio = new Episodio();
    oImagenes = new ImagenesDiagnostico();

    ui->setupUi(this);
    // Rellenar combos
    SqlCalls llamada;
    QStringList listaEstudios = llamada.queryList("Select nivel from nivelestudios","dbmedica");
    QStringList listaDoctores = llamada.queryList("Select nombre from doctores ","dbmedica");

    ui->cboNivelEstudios->addItems(listaEstudios);
    ui->cboDoctorEpisodio->addItems(listaDoctores);

    QSqlQueryModel *qTipos = new QSqlQueryModel(this);
    qTipos->setQuery("Select descripcion from tiposimagen",QSqlDatabase::database("dbmedica"));
    ui->comboBox_tipoImagen->setModel(qTipos);
    ui->txtHistorialEpisodio->setPlainText("");

    // Conexiones

    connect(ui->btnCerrar, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnBuscarCIEEpisodio,SIGNAL(clicked()),this,SLOT(BuscarCIE()));
    connect(ui->btnEditarFarmacologia,SIGNAL(clicked()), this, SLOT(ActivarControlesFarmacologia()));
    connect(ui->btnGuardarFarmacologia,SIGNAL(clicked()),this,SLOT(GuardarDatosFarmacologia()));
    connect(ui->listaTratamientosFarma,SIGNAL(cellClicked(int ,int )),this, SLOT(cargarDatosMedicamento(int ,int)));
    connect(ui->radvertodosfarmacologia,SIGNAL(clicked()),this,SLOT(llenartablahistorialfarmacologiaepisodio()));
    connect(ui->radversoloactivosfarmacologia,SIGNAL(clicked()),this,SLOT(llenartablahistorialfarmacologiaepisodio()));
    connect(ui->btnBorrarFarma,SIGNAL(clicked()),this,SLOT(BorrarDatosMedicamento()));
    connect(ui->btnVerFarma,SIGNAL(clicked()),this,SLOT(MostrarFichaMedicamento()));
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


    // Ocultar Iconos imagenes
    ui->itemFarma->setVisible(false);
    ui->itemAnalitica->setVisible(false);
    ui->itemImagenes->setVisible(false);
    ui->itemInterConsulta->setVisible(false);
}

FrmFichaPaciente::~FrmFichaPaciente()
{
    delete ui;
    delete oPaciente;
    delete oEpisodio ;
    delete oImagenes;
}

void FrmFichaPaciente::cargarDatos(int idcliente)
{
    oPaciente->RecuperarPaciente(idcliente);

    ui->txtNumHistoriaClinica->setText(QString::number(oPaciente->getnumHistoria()));
    ui->chkAlcohol->setChecked(oPaciente->getalcoholbool());
    ui->txtAlergiasConocidas->setPlainText(oPaciente->getalergiasConocidas());
    ui->txtAntecedentesFamiliares->setPlainText(oPaciente->getantecedentesFamiliares());
    ui->txtCirugiasPrevias->setPlainText(oPaciente->getcirugiasPrevias());
    ui->txtDiastole->setText(QString::number(oPaciente->getdiastole(),'f',2));
    ui->txtSistole->setText(QString::number(oPaciente->getsistole(),'f',2));
    ui->txtEnfermedadesConocidas->setPlainText(oPaciente->getenfermedadesConocidas());
    ui->txtFamiliaNuclear->setPlainText(oPaciente->getfamilia());
    ui->txtFechaAlta->setDate(oPaciente->getfechaAlta());
    ui->txtFiliacion->setText(oPaciente->getfiliacion());

    ui->txtHijos->setValue(oPaciente->gethijos());
    ui->txtHistorialEpisodio->setPlainText(oPaciente->gethistorial());
    ui->txtIMC->setText(QString::number(oPaciente->getIMC()));
    ui->txtNacimiento->setDateTime(oPaciente->getnacimiento());

    int nNivel = ui->cboNivelEstudios->findText(oPaciente->getnivelEstudios());
    if (nNivel>-1)
        ui->cboNivelEstudios->setCurrentIndex(nNivel);

    ui->txtNumeroSS->setText(oPaciente->getnumSS());
    ui->txtOtrasDrogas->setPlainText(oPaciente->gethabitosDrogas());
    ui->txtPerimetroCraneal->setText(QString::number(oPaciente->getperimetroCraneal(),'f',2));
    ui->txtPeso->setText(QString::number(oPaciente->getpeso(),'f',2));
    ui->txtProfesion->setText(oPaciente->getprofesion());

    ui->radHombre->setChecked(oPaciente->getsexo()=="H");
    ui->radMujer->setChecked(!(oPaciente->getsexo()=="H"));

    ui->chkTabaco->setChecked(oPaciente->gettabacobool());
    ui->chkOtrasDrogas->setChecked(oPaciente->getotrasDrogasbool());
    ui->txtTalla->setText(QString::number(oPaciente->gettalla(),'f',2));

    ui->radTrabaja->setChecked(oPaciente->gettrabajabool());
    ui->radNoTrabaja->setChecked(!oPaciente->gettrabajabool());

    ui->txtNumHistoriaClinica->setText(QString::number(oPaciente->getnumHistoria()));
    // cargar datos Episodios
    QSqlQuery Episodios(QSqlDatabase::database("dbmedica"));

    Episodios.prepare("Select id, descripcion from episodios where idpaciente = :nId");
    Episodios.bindValue(":nId",idcliente);
    if(Episodios.exec()) {
        ui->listaEpisodios->setColumnCount(1);
        //--------------------------------
        while (Episodios.next()) {
            QSqlRecord registro = Episodios.record();
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->listaEpisodios);

            item->setText(0,registro.field("descripcion").value().toString());
            ui->listaEpisodios->addTopLevelItem(item);
            // TODO - Leer base de datos visitas para añadir como hijo de Episodios.
            QSqlQuery qVisitas(QSqlDatabase::database("dbmedica"));
            qVisitas.prepare("select id,fechahora,medico from visitas where id =:nId");
            qVisitas.bindValue(":nId",registro.field("id").value().toString());
            if(qVisitas.exec()) {
                while(qVisitas.next()){
                    QSqlRecord recVis = qVisitas.record();
                    QTreeWidgetItem *child = new QTreeWidgetItem(item);
                    QString Valor = recVis.field("fechahora").value().toDateTime().toString() +" "+
                            recVis.field("medico").value().toString();
                    child->setText(0,Valor);
                    item->addChild(child);
                }
            }
        }
    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Error"),tr("pepito"),tr("aceptar"));
    //ui->listaEpisodios->setModel(EpisodiosModelo);

}

void FrmFichaPaciente::cargarEpisodio(int control)
{
    if(control==1)
        BloquearCamposEpisodio(false);

    // Cargo valores en pantalla
    ui->txtIdentificadorEpisodio->setText(QString::number(oEpisodio->getid()));
    ui->txtDescripcionEpisodio->setText(oEpisodio->getdescripcion());
    ui->txtDescrpicionEpisodio_2->setText(oEpisodio->getdescripcion());

    ui->chkEpisodioprivado->setChecked(oEpisodio->getprivado() == 1);

    int nIndex = ui->cboDoctorEpisodio->findText(oEpisodio->getdoctor());
    if (nIndex !=-1)
            ui->cboDoctorEpisodio->setCurrentIndex(nIndex);

    ui->txtFechaEpisodio->setDate(oEpisodio->getfecha());
    ui->txtHistorialEpisodio->setPlainText(oEpisodio->gethistorial());
    ui->txtCIEEpisiodio->setText(oEpisodio->getCIE());

    llenartablahistorialfarmacologiaepisodio();
    llenartablahistorialimagenesepisodio();
    llenartablahistorialanalisisepisodio();
}

void FrmFichaPaciente::guardarDatosPaciente()
{
    oPaciente->setnumHistoria(ui->txtNumHistoriaClinica->text().toInt());

    ui->chkAlcohol->isChecked() ? oPaciente->setalcohol(1) : oPaciente->setalcohol(0);

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
    oPaciente->setperimetroCraneal(ui->txtPerimetroCraneal->text().toDouble());
    oPaciente->setpeso(ui->txtPeso->text().toDouble());
    oPaciente->setprofesion(ui->txtProfesion->text());

    ui->radHombre->isChecked() ? oPaciente->setsexo("H") : oPaciente->setsexo("M");

    ui->chkTabaco->isChecked() ? oPaciente->settabaco(1) : oPaciente->settabaco(0);

    oPaciente->settalla(ui->txtTalla->text().toDouble());

    ui->radTrabaja->isChecked() ? oPaciente->settrabaja(1) : oPaciente->settrabaja(0);

    ui->chkOtrasDrogas->isChecked() ? oPaciente->setotrasDrogas(1) : oPaciente->setotrasDrogas(0);
}

void FrmFichaPaciente::AnadirDatosMedicamento(int id, QString nombre, QString codigo_nacional)
{
    QSqlQuery qTratamientos(QSqlDatabase::database("dbmedica"));
    //QSqlQueryModel *qTratamientosM = new QSqlQueryModel();

    qTratamientos.prepare("insert into histofarma (idmedicamento,medicamento,idepisodio,iniciotratamiento,codigonacional)"
                           " values (:idmedicamento,:medicamento,:idepisodio,:iniciotratamiento,:codigonacional)");
    qTratamientos.bindValue(":idmedicamento",id);
    qTratamientos.bindValue(":medicamento",nombre);
    qTratamientos.bindValue(":idepisodio",oEpisodio->getid());
    qTratamientos.bindValue(":iniciotratamiento",QDate::currentDate());
    qTratamientos.bindValue(":codigonacional",codigo_nacional);
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

void FrmFichaPaciente::on_btnAnadirEpisodio_clicked()
{
   int idEpisodio = oEpisodio->NuevoEpisodio(oPaciente->getid());
   oEpisodio->RecuperarEpisodio(idEpisodio);
   if (oEpisodio->getid() >0)
   {
       cargarEpisodio(1);
       ui->radEpisodioAbierto->setChecked(true);
   }
   else
   {
       QMessageBox::warning(this,tr("Nuevo episodio"),tr("No se ha podido recuperar el nuevo episodio"),tr("Aceptar"));
   }
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
    ui->chkOtrasDrogas->setEnabled(!state);
    ui->txtOtrasDrogas->setEnabled(!state);
    ui->txtNacimiento->setEnabled(!state);
    ui->txtFechaAlta->setEnabled(!state);
    ui->txtFamiliaNuclear->setEnabled(!state);
    ui->txtAlergiasConocidas->setEnabled(!state);
    ui->txtAntecedentesFamiliares->setEnabled(!state);
    ui->txtDiastole->setEnabled(!state);
    ui->txtEnfermedadesConocidas->setEnabled(!state);
    ui->txtFiliacion->setEnabled(!state);
    ui->txtHijos->setEnabled(!state);
    ui->txtIMC->setEnabled(!state);
    ui->txtCirugiasPrevias->setEnabled(!state);
    ui->txtMutua->setEnabled(!state);
    ui->txtNumeroSS->setEnabled(!state);
    ui->txtPerimetroCraneal->setEnabled(!state);
    ui->txtPeso->setEnabled(!state);
    ui->txtProfesion->setEnabled(!state);
    ui->txtSistole->setEnabled(!state);
    ui->txtTalla->setEnabled(!state);
    ui->cboNivelEstudios->setEnabled(!state);
    ui->radHombre->setEnabled(!state);
    ui->radMujer->setEnabled(!state);
    ui->radTrabaja->setEnabled(!state);
    ui->radNoTrabaja->setEnabled(!state);
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
     ui->btnAnadirEpisodio->setEnabled(!state);
     ui->btnEditarEpisodio->setEnabled(!state);
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
    oEpisodio->setidPaciente(oPaciente->getid());

    oEpisodio->setprivado(ui->chkEpisodioprivado->isChecked());
}

void FrmFichaPaciente::BloquearCamposImagen()
{
    ui->lineEdit_descripcionimagen->setEnabled(false);
    ui->txtComentariosImagen->setEnabled(false);
    ui->dateEdit_fechaimagen->setEnabled(false);
    ui->pushButton_GuardarImagen->setEnabled(false);
    ui->pushButton_editarimagen->setEnabled(true);
    ui->pushButton_DeshacerImagen->setEnabled(false);
    ui->comboBox_tipoImagen->setEnabled(false);
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

void FrmFichaPaciente::on_btnGuardarEpisodio_clicked()
{
    LLenarEpisodio();
    oEpisodio->GuardarEpisodio();
    // cargar datos Episodios
    //BUG ? ¿este Model para que?
//    QSqlQueryModel *EpisodiosModelo = new QSqlQueryModel(this);
//    EpisodiosModelo->setQuery("Select descripcion from episodios where idpaciente = "+QString::number(oPaciente->getid()),QSqlDatabase::database("dbmedica"));
//   // ui->listaEpisodios->setModel(EpisodiosModelo);
 BloquearCamposEpisodio(true);

}


void FrmFichaPaciente::on_listaEpisodios_clicked(const QModelIndex &index)
{
    QString cDescripcion = ui->listaEpisodios->model()->data(index).toString();
    QSqlQuery qEpisodio(QSqlDatabase::database("dbmedica"));
    qEpisodio.prepare("select id from episodios where descripcion =:descripcion and idpaciente = :idpaciente");
    qEpisodio.bindValue(":descripcion",cDescripcion);
    qEpisodio.bindValue(":idpaciente",oPaciente->getid());
    if (qEpisodio.exec())
    {
        qEpisodio.next();
        oEpisodio->RecuperarEpisodio(qEpisodio.value(0).toInt());
        cargarEpisodio(0);
        ui->btnEditarEpisodio->setEnabled(true);
    }
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
    if(ui->listaTratamientosFarma->currentRow()>0) {
        ui->btnEditarFarmacologia->setEnabled(false);
        ui->btnGuardarFarmacologia->setEnabled(true);
        ui->btnDeshacerFarmacologia->setEnabled(false);
        ui->txtInicioTratamientoFarma->setEnabled(true);
        ui->txtPosologiaFarma->setEnabled(true);
        ui->txtComentariosFarma->setEnabled(true);
        ui->chkactivo->setEnabled(true);
        ui->txtInicioTratamientoFarma->setFocus();
    } else
        QMessageBox::warning(this,tr("Editar historial Farmacología"),tr("Antes de editar debe seleccionar una ficha de imagen"),
                             tr("Aceptar"));
}

void FrmFichaPaciente::GuardarDatosFarmacologia()
{
    ui->btnEditarFarmacologia->setEnabled(true);
    ui->btnGuardarFarmacologia->setEnabled(false);
    ui->btnDeshacerFarmacologia->setEnabled(false);
    ui->txtInicioTratamientoFarma->setEnabled(false);
    ui->txtPosologiaFarma->setEnabled(false);
    ui->txtComentariosFarma->setEnabled(false);
    ui->chkactivo->setEnabled(false);

    Farmacologia oFarmacologia;

    oFarmacologia.setidepisodio(oEpisodio->getid());
    oFarmacologia.setactivo(ui->chkactivo->isChecked());
    oFarmacologia.setcomentarios(ui->txtComentariosFarma->toPlainText());
    oFarmacologia.setindicacionposologia(ui->txtPosologiaFarma->toPlainText());
    oFarmacologia.setiniciotratamiento(ui->txtInicioTratamientoFarma->date());
    oFarmacologia.setmedicamento(ui->listaTratamientosFarma->item(ui->listaTratamientosFarma->currentRow(),0)->text());
    oFarmacologia.modificarFarmaco(ui->listaTratamientosFarma->item(ui->listaTratamientosFarma->currentRow(),1)->text().toInt());

    llenartablahistorialfarmacologiaepisodio();
}

void FrmFichaPaciente::llenartablahistorialfarmacologiaepisodio()
{
    // Cargar farmacología episodio
    QStringList list;
    list <<"NOMBRE"<<"ID";
    QSqlQuery qFarma(QSqlDatabase::database("dbmedica"));
    QString cSQL = " Select medicamento, id,idmedicamento,activo from histofarma where idepisodio = :id ";
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
                QTableWidgetItem *newItem2 = new QTableWidgetItem(QString::number(reg.field("idmedicamento").value().toInt()));
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
    list <<QObject::tr("DESCRIPCIÓN")<< QObject::tr("FECHA") <<QObject::tr("TIPO") <<"ID";
    QSqlQuery qImagenes(QSqlDatabase::database("dbmedica"));
    QString cSQL = " Select descripcion, id,fechaimagen, tipoimagen,evaluada from imagenes where idepisodio = :id ";
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
    if (qImagenes.exec()) {
        while (qImagenes.next()) {
            // Nombre Medicamento
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
                cFecha = reg.field("fechaimagen").value().toDateTime().toString("dd/MM/yyyy : hh:mm");
                QTableWidgetItem *newItem1 = new QTableWidgetItem(cFecha);
                // para que los elementos no sean editables
                newItem1->setFlags(newItem1->flags() & (~Qt::ItemIsEditable));
                newItem1->setTextColor(Qt::blue); // color de los items
                ui->listaImagenes->setItem(pos,1,newItem1);

                // tipo
                QTableWidgetItem *newItem2 = new QTableWidgetItem(reg.field("tipoimagen").value().toString());
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
    }
}

void FrmFichaPaciente::llenartablahistorialanalisisepisodio()
{
    // Cargar imagenes episodio
    QStringList list;
    list <<QObject::tr("ANALISIS")<< QObject::tr("FECHA") <<"ID" <<tr("COMENTARIOS");
    QSqlQuery qAnalisis(QSqlDatabase::database("dbmedica"));
    QString cSQL = " Select analisis, id,fechaanalisis, comentarios from Analitica where idepisodio = :id ";
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
            cFecha = reg.field("fechaanalisis").value().toDateTime().toString("dd/MM/yyyy : hh:mm");
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
        QString cSQL = "select codigonacional from histofarma where id =:id";
        qFarma.prepare(cSQL);
        qFarma.bindValue(":id",ui->listaTratamientosFarma->item(ui->listaTratamientosFarma->currentRow(),1)->text());
        if(qFarma.exec()) {
            qFarma.next();
            QString cCodigoNacional = qFarma.value(0).toString();
            FrmInformacionFarmaco frmFarmaco(this);
            frmFarmaco.capturarid(cCodigoNacional);
            frmFarmaco.setModal(true);
            frmFarmaco.setWindowModality(Qt::WindowModal);
            frmFarmaco.exec();
        }
    }
}

void FrmFichaPaciente::AnadirImagenDiagnostico()
{
    if (oEpisodio->getid() >0)
    {
        FrmAnadirImagen imagen;
        imagen.RecuperarId(oEpisodio->getid());
        imagen.adjustSize();
        imagen.move(QApplication::desktop()->screen()->rect().center() - imagen.rect().center());
        imagen.exec();
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
        ui->comboBox_tipoImagen->setEnabled(true);
        ui->dateEdit_fechaimagen->setEnabled(true);
        ui->txtComentariosImagen->setEnabled(true);
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
   // ui->txtNumHistoriaClinica->setText(QString::number(historia));
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
        oAnalitica.setIdEpisodio(oEpisodio->getid());
        oAnalitica.AnadirAnalitica();
        frmAnalitica.capturaID(oAnalitica.getId());
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
        int nId = ui->listaAnaliticas->item(ui->listaAnaliticas->currentRow(),2)->text().toInt();
        oAnalitica.EliminarAnalitica(nId);
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

        oAnalitica.setIdEpisodio(oEpisodio->getid());
        oAnalitica.setId(ui->listaAnaliticas->item(ui->listaAnaliticas->currentRow(),2)->text().toInt());

        frmAnalitica.capturaId(oAnalitica.getId());
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
    ui->txtInicioTratamientoFarma->setDate(oFarmacologia.getiniciotratamiento());
    ui->chkactivo->setChecked(oFarmacologia.getactivo()==1);
    ui->txtComentariosFarma->setPlainText(oFarmacologia.getcomentarios());
    ui->txtPosologiaFarma->setPlainText(oFarmacologia.getindicacionposologia());
}

void FrmFichaPaciente::cargarDatosImagenes(int crow, int ccol)
{
    QString id = ui->listaImagenes->item(crow,3)->text();

    oImagenes->llenarObjetoconDatosDB(id.toInt());
    ui->txtComentariosImagen->setPlainText(oImagenes->getComentarios());
    ui->lineEdit_descripcionimagen->setText(oImagenes->getDescripcion());
    ui->dateEdit_fechaimagen->setDate(oImagenes->getFechaImagen());

    int nindex = ui->comboBox_tipoImagen->findText(oImagenes->getTipoImagen());
    if (nindex >-1)
        ui->comboBox_tipoImagen->setCurrentIndex(nindex);
    else
        ui->comboBox_tipoImagen->clear();

    if (!oImagenes->getLocalizacionImagen().isEmpty())
    {
        QImage imagen(oImagenes->getLocalizacionImagen());
        QPixmap qpmImagen = QPixmap::fromImage(imagen);
        ui->lblImagenDiagnostico->setPixmap(qpmImagen.scaled(ui->lblImagenDiagnostico->size(), Qt::KeepAspectRatio));
    }    
    ui->checkBox_evaluada->setChecked(oImagenes->getEvaluada()==1);
}

void FrmFichaPaciente::guardarDatosImagenes()
{
    QString cId = ui->listaImagenes->item(ui->listaImagenes->currentRow(),3)->text();
    oImagenes->setId(cId.toInt());
    oImagenes->setComentarios(ui->txtComentariosImagen->toPlainText());
    oImagenes->setDescripcion(ui->lineEdit_descripcionimagen->text());
    oImagenes->setFechaImagen(ui->dateEdit_fechaimagen->date());

    oImagenes->setEvaluada(ui->checkBox_evaluada->isChecked());

    oImagenes->setTipoImagen(ui->comboBox_tipoImagen->currentText());
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
    frmVisitas frmvisitas;
    frmvisitas.exec();
}
