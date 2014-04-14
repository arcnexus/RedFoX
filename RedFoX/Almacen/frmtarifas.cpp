#include "frmtarifas.h"
#include "ui_frmtarifas.h"
#include "Auxiliares/monetarydelegate.h"

FrmTarifas::FrmTarifas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmTarifas)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    QSqlQueryModel *modelo = new QSqlQueryModel(this);
    modelo->setQuery("Select descripcion from codigotarifa",Configuracion_global->groupDB);
    ui->listaTarifa->setModel(modelo);
    modelVolumenes = new QSqlQueryModel(this);
    ui->tabla_volumenes->setModel(modelVolumenes);
    this->id_volumen =0;
    this->id_tarifa =0;
    modelVolumenes->setQuery(QString("select id,desde,hasta,precio from articulos_volumen where id_tarifa = %1").arg(this->id_tarifa),
                             Configuracion_global->groupDB);
    QStringList headers;
    QVariantList sizes;
    headers << "id" <<tr("Desde") << tr("Hasta") << tr("Precio");
    sizes << 0 << 60 << 60 <<100;
    for(int i=0;i<headers.size();i++)
    {
        ui->tabla_volumenes->setColumnWidth(i,sizes.at(i).toInt());
        modelVolumenes->setHeaderData(i,Qt::Horizontal,headers.at(i));
        if(i>0)
            ui->tabla_volumenes->setItemDelegateForColumn(i,new MonetaryDelegate(this));
    }



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
    ui->spinmargen_minimo->setValue(Configuracion_global->margen_minimo);



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

void FrmTarifas::capturar_datos(int id, QString costeLocal, int id_prod){
    ui->listaTarifa->hide();
    QSqlQuery queryTarifas(Configuracion_global->groupDB);
    if(!queryTarifas.exec("select * from tarifas where id="+QString::number(id)))
    {
        QMessageBox::warning(this,tr("Editar"),
                             tr("Ocurrió un error al recuperar los datos: %1").arg(queryTarifas.lastError().text()));
    }
    else
    {
        if (queryTarifas.next())
        {
            QSqlQuery queryGrupotarifa(Configuracion_global->groupDB);
            int nId = queryTarifas.record().value("id_codigo_tarifa").toInt();
            int id_div = queryTarifas.record().value("id_monedas").toInt();

            QString cSQL1 = "select * from codigotarifa where id=";
            cSQL1.append(QString::number(nId));
            qDebug() << cSQL1;
            if(queryGrupotarifa.exec(cSQL1))
            {
                queryGrupotarifa.next();
                ui->txtCodTarifa->setText(queryGrupotarifa.record().field("codigo_tarifa").value().toString());
                ui->txtDescTarifa->setText(queryGrupotarifa.record().field("descripcion").value().toString());
            }
            this->id_producto = id_prod;
            ui->txtPais->setText(Configuracion_global->Devolver_pais(queryTarifas.record().field("id_pais").value().toInt()));
            ui->txtMoneda->setText(Configuracion_global->Devolver_moneda(queryTarifas.record().value("id_monedas").toInt()));
            ui->txtPVPDivisa->setText(Configuracion_global->toFormatoMoneda(queryTarifas.record().value("pvp").toString()));
            ui->txtCosteLocal->setText(costeLocal);
            if(queryTarifas.record().value("margen").toDouble() == 0)
                ui->spinMargen->setValue(Configuracion_global->margen);
            else
                ui->spinMargen->setValue(queryTarifas.record().field("margen").value().toDouble());
            if (queryTarifas.record().value("margen_minimo") == 0)
                ui->spinmargen_minimo->setValue(Configuracion_global->margen_minimo);
            else
                ui->spinmargen_minimo->setValue(queryTarifas.record().field("margen_minimo").value().toDouble());

            this->id_tarifa = queryTarifas.record().field("id").value().toInt();
            this->id_pais = queryTarifas.record().field("id_pais").value().toInt();
            this->id_moneda = queryTarifas.record().field("id_monedas").value().toInt();
            this->codigoTarifa = queryTarifas.record().field("codigo_tarifa").value().toString();
            this->margen = ui->spinMargen->value();
            this->margen_min = ui->spinmargen_minimo->value();
            this->coste = costeLocal.toDouble();
            this->pvpDivisa =ui->txtPVPDivisa->text().toDouble();
            this->cod_divisa = Configuracion_global->Devolver_codDivisa(queryTarifas.record().field("id_monedas").value().toInt());
            // Cargamos valor divisa desde la web
            if(id_div == Configuracion_global->id_divisa_local)
                asignarcambiodivisa(1);
            else
                Configuracion_global->getCambio(Configuracion_global->cod_divisa_local,this->cod_divisa);

            modelVolumenes->setQuery(QString("select id,desde,hasta,precio from articulos_volumen where id_tarifa = %1").arg(this->id_tarifa),
                                     Configuracion_global->groupDB);
        }
    }
}



void FrmTarifas::cargarDatosTarifa(QModelIndex indice)
{
    QSqlQueryModel* modelo = (QSqlQueryModel*)ui->listaTarifa->model();
    QString tarifa = modelo->record(indice.row()).value("descripcion").toString();
    QSqlQuery qTarifa(Configuracion_global->groupDB);
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
        this->margen_min = ui->spinmargen_minimo->value();
        if (Configuracion_global->divisa_local != ui->txtMoneda->text())
            Configuracion_global->getCambio(Configuracion_global->cod_divisa_local,this->cod_divisa);
        else
            asignarcambiodivisa(1);
    }
    calcular_precio(ui->spinMargen->value());

}

void FrmTarifas::asignarcambiodivisa(float valor)
{
    ui->txtValorDivisa->setText(Configuracion_global->toFormatoMoneda(QString::number(valor,'f',3)));
    double valorLocal;
    valorLocal = (1/valor);
    ui->txtvalorLocal->setText(Configuracion_global->toFormatoMoneda(QString::number(valorLocal,'f',3)));
    int pvpd;
    pvpd = this->pvpDivisa * ui->txtValorDivisa->text().toDouble();
    ui->txtPVPLocal->setText(Configuracion_global->toFormatoMoneda( QString::number(pvpd,'f',3)));
    calcular_precio(ui->spinMargen->value());
}



void FrmTarifas::calcular_precio(double margen)
{
    //TODO - CALCULO DIVISA LOCAL FALLA.
    blockSignals(true);
    //NOTE 70% /0.30 - 40 /0.60
    double pvp = Configuracion_global->MonedatoDouble(ui->txtCosteLocal->text())/(1-(margen/100));

    double cambio_divisa =  Configuracion_global->MonedatoDouble(ui->txtValorDivisa->text());
    double valordivisa =    pvp * cambio_divisa;

    ui->txtPVPLocal->setText(Configuracion_global->toFormatoMoneda(QString::number(pvp,'f',Configuracion_global->decimales)));
    ui->txtPVPDivisa->setText(Configuracion_global->toFormatoMoneda(QString::number(valordivisa,'f',Configuracion_global->decimales)));
    ui->txt_pvpiva->setText(Configuracion_global->toFormatoMoneda(QString::number(pvp * (1+iva/100.0),'f',Configuracion_global->decimales)));
    this->pvpDivisa = valordivisa;
    blockSignals(false);
}



void FrmTarifas::aceptar()
{
    this->margen = ui->spinMargen->value();
    this->margen_min = ui->spinmargen_minimo->value();
    this->accept();
}


void FrmTarifas::on_txtPVPLocal_editingFinished()
{
    blockSignals(true);
    double Margen = 100 - (Configuracion_global->MonedatoDouble(ui->txtCosteLocal->text()) * 100 / Configuracion_global->MonedatoDouble(ui->txtPVPLocal->text()));
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

void FrmTarifas::controles_volumenes(bool state)
{
    ui->spinDesde->setEnabled(state);
    ui->spinHasta->setEnabled(state);
    ui->txtPrecio_volumen->setEnabled(state);
    ui->txtPrecio_volumen->setReadOnly(!state);
    ui->btnAnadir_volumen->setEnabled(!state);
    ui->btnEditar_volumen->setEnabled(!state);
    ui->btnGuardar_volumen->setEnabled(state);
    ui->btnDeshacer_volumen->setEnabled(state);
    ui->btnBorrar_volumen->setEnabled(!state);

}

void FrmTarifas::on_btnAnadir_volumen_clicked()
{
ui->spinDesde->setValue(0);
ui->spinHasta->setValue(0);
ui->txtPrecio_volumen->setText("0,00");
controles_volumenes(true);
ui->spinDesde->setFocus();


}

void FrmTarifas::on_btnEditar_volumen_clicked()
{
   controles_volumenes(true);
   ui->spinDesde->setFocus();
}

void FrmTarifas::on_btnGuardar_volumen_clicked()
{
    QHash <QString,QVariant> hvol;
    QString error;
    hvol["desde"] = ui->spinDesde->value();
    hvol["hasta"] = ui->spinHasta->value();
    hvol["precio"] = Configuracion_global->MonedatoDouble(ui->txtPrecio_volumen->text());
    if(this->id_volumen ==0) // Añadir
    {
        hvol["id_producto"] =  this->id_producto;
        hvol["id_tarifa"] = this->id_tarifa;
        SqlCalls::SqlInsert(hvol,"articulos_volumen",Configuracion_global->groupDB,error);
    } else // Editando
    {
        SqlCalls::SqlUpdate(hvol,"articulos_volumen",Configuracion_global->groupDB,QString("id=%1").arg(this->id_volumen), error);
    }
    if(!error.isEmpty())
        QMessageBox::warning(this,tr("Artículos por volumen"),tr("Ocurrió un error al guardar los volumen en la BD: %1").arg(error),
                             tr("Aceptar"));
    else
        modelVolumenes->setQuery(QString("select id,desde,hasta,precio from articulos_volumen where id_tarifa = %1").arg(this->id_tarifa),
                                 Configuracion_global->groupDB);
    controles_volumenes(false);


}

void FrmTarifas::on_txtPrecio_volumen_editingFinished()
{
    ui->txtPrecio_volumen->blockSignals(true);
    ui->txtPrecio_volumen->setText(Configuracion_global->toFormatoMoneda(ui->txtPrecio_volumen->text()));
    ui->txtPrecio_volumen->blockSignals(false);
}

void FrmTarifas::on_txt_pvpiva_editingFinished()
{
    double pvp_iva = Configuracion_global->MonedatoDouble(ui->txt_pvpiva->text());
    double pvp = pvp_iva / (1+iva/100);
    ui->txtPVPLocal->setText(Configuracion_global->toFormatoMoneda(QString::number(pvp,'f',Configuracion_global->decimales)));
    on_txtPVPLocal_editingFinished();
}
