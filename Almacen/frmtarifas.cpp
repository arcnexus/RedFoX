#include "frmtarifas.h"
#include "ui_frmtarifas.h"

FrmTarifas::FrmTarifas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmTarifas)
{
    ui->setupUi(this);
    QSqlQueryModel *modelo = new QSqlQueryModel(this);
    modelo->setQuery("Select descripcion from codigotarifa",QSqlDatabase::database("Maya"));
    ui->listaTarifa->setModel(modelo);


    //-----------
    // CONEXIONES
    //-----------
    connect(ui->listaTarifa,SIGNAL(clicked(QModelIndex)),this,SLOT(cargarDatosTarifa(QModelIndex)));
    connect(ui->txtPVPDivisa,SIGNAL(editingFinished()),Configuracion_global,SLOT(format_text()));
    connect(ui->txtPVPDivisa,SIGNAL(editingFinished()),this,SLOT(cambiar_precio_editingfinished()));
    connect(ui->btnAceptar,SIGNAL(clicked()),this,SLOT(aceptar()));
    connect(ui->spinMargen,SIGNAL(valueChanged(double)),this,SLOT(calcular_precio(double)));


    //-------------------------------
    // CAMBIO DIVISA
    //-------------------------------
    connect(Configuracion_global,SIGNAL(cambioReady(float)),this,SLOT(asignarcambiodivisa(float)));
    //   Configuracion_global->getCambio("EUR","USD",1);

    ui->spinMargen->setValue(Configuracion_global->margen);
    ui->spinMargenMinimo->setValue(Configuracion_global->margen_minimo);


}

FrmTarifas::~FrmTarifas()
{
    delete ui;
}

void FrmTarifas::capturar_coste(float Coste)
{
    this->coste = Coste;
    ui->txtCosteLocal->setText(Configuracion_global->toFormatoMoneda(QString::number(Coste)));
}

void FrmTarifas::capturar_datos(int id, QString costeLocal){
    ui->listaTarifa->hide();
    QSqlQuery queryTarifas(QSqlDatabase::database("Maya"));
    if(!queryTarifas.exec("select * from tarifas where id="+QString::number(id))){
        QMessageBox::warning(this,tr("Editar"),
                             tr("Ocurrió un error al recuperar los datos: %1").arg(queryTarifas.lastError().text()));
    }else {
        queryTarifas.next();
        QSqlQuery queryGrupotarifa(QSqlDatabase::database("Maya"));
        if(queryGrupotarifa.exec("select * from codigotarifa where id="+QString::number(
                                      queryTarifas.record().field("id_codigotarifa").value().toInt()))){
            queryGrupotarifa.next();
            ui->txtCodTarifa->setText(queryGrupotarifa.record().field("codigo_tarifa").value().toString());
            ui->txtDescTarifa->setText(queryGrupotarifa.record().field("descripcion").value().toString());
        }
        ui->txtPais->setText(Configuracion_global->Devolver_pais(queryTarifas.record().field("id_pais").value().toInt()));
        ui->txtMoneda->setText(Configuracion_global->Devolver_moneda(queryTarifas.record().field("id_monedas").value().toInt()));
        ui->txtPVPDivisa->setText(Configuracion_global->toFormatoMoneda(queryTarifas.record().field("pvp").value().toString()));
        ui->txtCosteLocal->setText(costeLocal);
        if(queryTarifas.record().value("margen").toDouble() == 0)
            ui->spinMargen->setValue(Configuracion_global->margen);
        else
            ui->spinMargen->setValue(queryTarifas.record().field("margen").value().toDouble());
        if (queryTarifas.record().value("margenminimo") == 0)
            ui->spinMargenMinimo->setValue(Configuracion_global->margen_minimo);
        else
            ui->spinMargenMinimo->setValue(queryTarifas.record().field("margenminimo").value().toDouble());

        this->id_tarifa = queryTarifas.record().field("id").value().toInt();
        this->id_pais = queryTarifas.record().field("id_pais").value().toInt();
        this->id_moneda = queryTarifas.record().field("id_monedas").value().toInt();
        this->codigoTarifa = queryTarifas.record().field("codigo_tarifa").value().toString();
        this->margen = ui->spinMargen->value();
        this->margen_min = ui->spinMargenMinimo->value();
        this->coste = costeLocal.toDouble();
        this->pvpDivisa =ui->txtPVPDivisa->text().toDouble();
        this->cod_divisa = Configuracion_global->Devolver_codDivisa(queryTarifas.record().field("id_monedas").value().toInt());
        // Cargamos valor divisa desde la web
        if (Configuracion_global->DivisaLocal != ui->txtMoneda->text())
            Configuracion_global->getCambio(Configuracion_global->codDivisaLocal,this->cod_divisa);
        else
            asignarcambiodivisa(1);


   }
}

void FrmTarifas::cargarDatosTarifa(QModelIndex indice)
{
    QSqlQueryModel* modelo = (QSqlQueryModel*)ui->listaTarifa->model();
    QString tarifa = modelo->record(indice.row()).value("descripcion").toString();
    QSqlQuery qTarifa(QSqlDatabase::database("Maya"));
    qTarifa.prepare("select * from codigotarifa where descripcion ='"+tarifa+"'");

    if(qTarifa.exec()){
        qTarifa.next();

        ui->txtCodTarifa->setText(qTarifa.record().field("codigo_tarifa").value().toString());
        ui->txtDescTarifa->setText(qTarifa.record().field("descripcion").value().toString());
        ui->txtPais->setText(Configuracion_global->Devolver_pais(qTarifa.record().field("id_pais").value().toInt()));
        ui->txtMoneda->setText(Configuracion_global->Devolver_moneda(qTarifa.record().field("id_monedas").value().toInt()));
        this->id_tarifa = qTarifa.record().field("id").value().toInt();
        this->id_pais = qTarifa.record().field("id_pais").value().toInt();
        this->id_moneda = qTarifa.record().field("id_monedas").value().toInt();
        this->cod_divisa = Configuracion_global->Devolver_codDivisa(this->id_moneda);
        this->codigoTarifa = qTarifa.record().field("codigo_tarifa").value().toString();
        this->margen = ui->spinMargen->value();
        this->pvpDivisa = qTarifa.record().field("pvp").value().toDouble();
        this->margen_min = ui->spinMargenMinimo->value();
        if (Configuracion_global->DivisaLocal != ui->txtMoneda->text())
            Configuracion_global->getCambio(Configuracion_global->codDivisaLocal,this->cod_divisa);
        else
            asignarcambiodivisa(1);
    }
    calcular_precio(ui->spinMargen->value());
}

void FrmTarifas::asignarcambiodivisa(float valor)
{
    ui->txtValorDivisa->setText(QString::number(valor,'f',3));
    double valorLocal;
    valorLocal = (1/valor);
    ui->txtvalorLocal->setText(QString::number(valorLocal,'f',3));
    int pvpd;
    pvpd = this->pvpDivisa * ui->txtValorDivisa->text().toDouble();
    ui->txtPVPLocal->setText( QString::number(pvpd,'f',3));
    calcular_precio(ui->spinMargen->value());

}



void FrmTarifas::calcular_precio(double margen)
{

        blockSignals(true);
        //NOTE 70% /0.30 - 40 /0.60
        double pvp = (ui->txtCosteLocal->text().replace(",",".").toDouble()*100)/(100-margen);
        QString cPvp = Configuracion_global->toFormatoMoneda(QString::number(pvp,'f',2));
        this->pvpDivisa = cPvp.toDouble();
        ui->txtPVPLocal->setText(cPvp);
        double valordivisa = ui->txtPVPLocal->text().replace(",",".").toDouble() * ui->txtValorDivisa->text().replace(",",".").toDouble();
        ui->txtPVPDivisa->setText(Configuracion_global->toFormatoMoneda(QString::number(valordivisa,'f',2)));
        this->pvpDivisa = valordivisa;
        blockSignals(false);
}



void FrmTarifas::aceptar()
{
    this->margen = ui->spinMargen->value();
    this->margen_min = ui->spinMargenMinimo->value();
    this->accept();
}


void FrmTarifas::on_txtPVPLocal_editingFinished()
{
    blockSignals(true);
    double Margen=100-((ui->txtCosteLocal->text().replace(",",".").toDouble()*100)/ui->txtPVPLocal->text().replace(",",".").toDouble());
    ui->spinMargen->setValue(Margen);
    calcular_precio(Margen);
    blockSignals(false);
}

void FrmTarifas::on_btnDesbloquearDivisa_clicked(bool checked)
{
    if (checked)
    {
        ui->txtValorDivisa->setEnabled(true);
        ui->txtvalorLocal->setEnabled(true);
        ui->txtValorDivisa->setFocus();
        if(Configuracion_global->activar_seguimiento) {
            // TODO Seguimiento cambio divisas manual
        }

    } else
    {
        ui->txtValorDivisa->setEnabled(false);
        ui->txtvalorLocal->setEnabled(false);
    }

}

void FrmTarifas::on_spinMargen_editingFinished()
{
    calcular_precio(ui->spinMargen->value());
}
