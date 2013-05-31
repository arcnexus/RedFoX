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
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(calcular_precio()));
    connect(ui->txtPVPDivisa,SIGNAL(editingFinished()),Configuracion_global,SLOT(format_text()));
    connect(ui->txtPVPDivisa,SIGNAL(editingFinished()),this,SLOT(cambiar_precio_editingfinished()));
    connect(ui->btnAceptar,SIGNAL(clicked()),this,SLOT(aceptar()));


    //-------------------------------
    // CAMBIO DIVISA
    //-------------------------------
    connect(Configuracion_global,SIGNAL(cambioReady(float)),this,SLOT(asignarcambiodivisa(float)));
 //   Configuracion_global->getCambio("EUR","USD",1);
}

FrmTarifas::~FrmTarifas()
{
    delete ui;
}

void FrmTarifas::capturar_coste(float Coste)
{
    this->coste = Coste;
    ui->txtCosteLocal->setText(Configuracion_global->FormatoNumerico(QString::number(Coste)));
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
        ui->txtPVPDivisa->setText(Configuracion_global->FormatoNumerico(queryTarifas.record().field("pvp").value().toString()));
        ui->txtCosteLocal->setText(costeLocal);
        ui->spinMargen->setValue(queryTarifas.record().field("margen").value().toDouble());
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

}

void FrmTarifas::cambiar_precio_editingfinished()
{
    double importe;
    importe = ui->txtPVPDivisa->text().toFloat()* ui->txtvalorLocal->text().toFloat();
    ui->txtPVPLocal->setText(Configuracion_global->FormatoNumerico(QString::number(importe,'f',2)));
    this->pvpDivisa = ui->txtPVPDivisa->text().toDouble();
    // recalcular margen comercial
    double Margen;
    Margen=((ui->txtPVPLocal->text().toDouble()*100)/this->coste)-100;
    ui->spinMargen->setValue(Margen);
}

void FrmTarifas::calcular_precio()
{
    double margen = (100-ui->spinMargen->value())/100;
    //NOTE 70% /0.30 - 40 /0.60

    double margen_moneda = (ui->txtCosteLocal->text().replace(",",".").toDouble()*margen);
    double pvp = margen_moneda + ui->txtCosteLocal->text().replace(",",".").toDouble();
    QString cPvp = Configuracion_global->FormatoNumerico(QString::number(pvp,'f',2));
    this->pvpDivisa = cPvp.toDouble();
    ui->txtPVPLocal->setText(cPvp);
    double valordivisa = ui->txtPVPLocal->text().replace(",",".").toDouble() * ui->txtValorDivisa->text().replace(",",".").toDouble();
    ui->txtPVPDivisa->setText(Configuracion_global->FormatoNumerico(QString::number(valordivisa,'f',2)));
    this->pvpDivisa = valordivisa;


}

void FrmTarifas::aceptar()
{
    this->margen = ui->spinMargen->value();
    this->margen_min = ui->spinMargenMinimo->value();
    this->accept();
}
