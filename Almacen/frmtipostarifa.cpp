#include "frmtipostarifa.h"
#include "ui_frmtipostarifa.h"



FrmTiposTarifa::FrmTiposTarifa(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmTiposTarifa)
{
    ui->setupUi(this);
    ui->lblDescArt->setVisible(false);
    oTipostarifa = new TiposTarifa(this);
    mTarifas = new QSqlQueryModel(this);


    ui->cboPais->setModel(Configuracion_global->paises_model);
    ui->cboPais->setModelColumn(1);
    llenar_lista();
    QSqlQueryModel *monedas = new QSqlQueryModel(this);
    monedas->setQuery("Select moneda from monedas",Configuracion_global->groupDB);
    ui->cboMoneda->setModel(monedas);
    activar_controles(false);
}

FrmTiposTarifa::~FrmTiposTarifa()
{
    delete ui;
}

void FrmTiposTarifa::cargarDatos()
{
    ui->txtCodigo->setText(oTipostarifa->codigo_tarifa);
    ui->txtDescripcion->setText(oTipostarifa->descripcion);
    ui->spnMargen->setValue(oTipostarifa->margen);
    ui->spnMargen_min->setValue(oTipostarifa->margen_min);
    int nindex = ui->cboMoneda->findText(Configuracion_global->Devolver_moneda(oTipostarifa->id_monedas));
    ui->cboMoneda->setCurrentIndex(nindex);
    nindex = ui->cboPais->findText(Configuracion_global->Devolver_pais(oTipostarifa->id_pais));
    ui->cboPais->setCurrentIndex(nindex);
    //ui->txtImpDto->setText(Configuracion_global->toFormatoMoneda( QString::number(oTipostarifa->importe_dto,'f',Configuracion_global->decimales)));
    //ui->txtPromocion->setText(oTipostarifa->desc_promo);
}

void FrmTiposTarifa::on_btnAgregarTarifa_clicked()
{
    if(QMessageBox::question(this,tr("Agregar Tarifas"),tr("¿Desea añadir esta tarifa?"),
                             tr("NO"),tr("SI")) == QMessageBox::Accepted)
    {
        ui->lblDescArt->setVisible(true);
        ui->lblDescArt->setText("Calculando.....");

        int nid = mTarifas->record(ui->comboTarifa->currentIndex()).value("id").toInt();
        oTipostarifa->recuperar(nid);

        if(ui->radTodos->isChecked())
        {          
            // TODO PREPARAR PARA MULTIDIVISA

            QString cod_div_local = Configuracion_global->cod_divisa_local;
            QString cod_div_extra = Configuracion_global->Devolver_codDivisa(oTipostarifa->id_monedas);
            if (cod_div_local!= cod_div_extra)
                oTipostarifa->valor_divisa = Configuracion_global->getCambioBlock(cod_div_local,cod_div_extra);
            else
                oTipostarifa->valor_divisa = 1;

            QString error;
            QMap<int,QString> _art_con_tarifa = SqlCalls::SelectMap<int,QString>("tarifas","id_articulo","id_codigo_tarifa",
                                                                                 QString("id_codigo_tarifa = %1").arg(oTipostarifa->id),
                                                                                 Configuracion_global->groupDB,error
                    );

            QMap<int,QSqlRecord> _arts = SqlCalls::SelectRecord("articulos","id>0",Configuracion_global->groupDB,error);

            QMapIterator<int,QSqlRecord> _it(_arts);
            while(_it.hasNext())
            {
                _it.next();
                if(!_art_con_tarifa.contains(_it.key()))
                {
                    QSqlRecord r = _it.value();
                    ui->lblDescArt->setText(r.value("descripcion").toString());
                    QApplication::processEvents();

                    QHash<QString,QVariant> _data;
                    _data["id_articulo"]= _it.key();
                    _data["id_pais"]= oTipostarifa->id_pais;
                    _data["id_monedas"]= oTipostarifa->id_monedas;
                    _data["margen"]= oTipostarifa->margen;
                    _data["margen_minimo"]= oTipostarifa->margen_min;
                    _data["id_codigo_tarifa"]= oTipostarifa->id;

                    double coste = r.value("coste_real").toDouble();
                    float margen = oTipostarifa->margen;
                    double pvp, pvp_divisa;
                    pvp = (coste*100)/(100-margen);
                    pvp_divisa = pvp * oTipostarifa->valor_divisa;

                    if (oTipostarifa->valor_divisa == 1)
                        _data["pvp"]= pvp;
                    else
                        _data["pvp"]= pvp_divisa;

                    if(SqlCalls::SqlInsert(_data,"tarifas",Configuracion_global->groupDB,error) < 0 )
                    {
                        QMessageBox::warning(this,tr("Ocurrió un error al crear la tarifa:"),
                                             error,
                                             tr("Aceptar"));
                        break;
                    }
                }
            }
        }
        ui->lblDescArt->setText(tr("¡Proceso terminado!"));
    }
}

void FrmTiposTarifa::on_listatarifas_clicked(const QModelIndex &index)
{
    int nid =ui->listatarifas->model()->data(ui->listatarifas->model()->index(index.row(),0),Qt::EditRole).toInt();
    oTipostarifa->recuperar(nid);
    cargarDatos();
}

void FrmTiposTarifa::on_btnAnadir_clicked()
{
    oTipostarifa->clear();
    int nId = oTipostarifa->anadir();
    if(nId >0)
    {
        oTipostarifa->recuperar(nId);
        cargarDatos();
        llenar_lista();
        activar_controles(true);
        ui->txtCodigo->setFocus();
    }
}

void FrmTiposTarifa::activar_controles(bool state)
{
    ui->txtCodigo->setEnabled(state && !(ui->txtCodigo->text() == "PVP"));
    ui->txtDescripcion->setEnabled(state);
    ui->cboMoneda->setEnabled(state);
    ui->cboPais->setEnabled(state);
    ui->spnMargen->setEnabled(state);
    ui->spnMargen_min->setEnabled(state);
    ui->btnAnadir->setEnabled(!state);
    ui->btnBorrar->setEnabled(!state);
    ui->btnGuardar->setEnabled(state);
    ui->btnDeshacer->setEnabled(state);
    ui->btnCerrar->setEnabled(!state);
    ui->btnEditar->setEnabled(!state);
}


void FrmTiposTarifa::on_btnEditar_clicked()
{
    activar_controles(true);
    ui->txtCodigo->setFocus();
}

void FrmTiposTarifa::on_btnGuardar_clicked()
{
    cargar_datos_en_objeto();
    if(oTipostarifa->guardar())
    {
        if(Configuracion_global->id_tarifa_predeterminada==0)
        {
            if(QMessageBox::question(this,tr("¿Tarífa prederterminada?"),
                                  tr("¿Desea convertir esta tarifa en predeterminada?"),
                                     tr("No"),
                                     tr("Si")) == QMessageBox::Accepted)
            {
                QHash<QString,QVariant> _data;
                _data["id_tarifa_predeterminada"] = oTipostarifa->id;
                QString error;
                if(SqlCalls::SqlUpdate(_data,"empresas",Configuracion_global->groupDB,
                                       QString("id = %1").arg(Configuracion_global->idEmpresa),error))
                {
                    Configuracion_global->id_tarifa_predeterminada = oTipostarifa->id;
                    TimedMessageBox * t = new TimedMessageBox(this,tr("Tarifa predeterminada cambiada con éxito"));
                }
                else
                    QMessageBox::critical(this,tr("Error al asignar tarifa predeterminada"),error);
            }
        }
        activar_controles(false);
        llenar_lista();
    }
}

void FrmTiposTarifa::on_btnDeshacer_clicked()
{
    oTipostarifa->recuperar(oTipostarifa->id);
    activar_controles(false);
    llenar_lista();
}

void FrmTiposTarifa::llenar_lista()
{    
    mTarifas->setQuery("select id,descripcion from codigotarifa",Configuracion_global->groupDB);
    ui->listatarifas->setModel(mTarifas);
    ui->listatarifas->setColumnHidden(0,true);
    mTarifas->setHeaderData(1,Qt::Horizontal,tr("Tarifa"));

    ui->comboTarifa->setModel(mTarifas);
    ui->comboTarifa->setModelColumn(1);
}

void FrmTiposTarifa::cargar_datos_en_objeto()
{
    oTipostarifa->codigo_tarifa = ui->txtCodigo->text();
    oTipostarifa->descripcion = ui->txtDescripcion->text();
    oTipostarifa->margen = ui->spnMargen->value();
    oTipostarifa->margen_min = ui->spnMargen_min->value();
    oTipostarifa->id_monedas = Configuracion_global->Devolver_id_moneda(ui->cboMoneda->currentText());
    oTipostarifa->id_pais = Configuracion_global->Devolver_id_pais(ui->cboPais->currentText());
}

void FrmTiposTarifa::asignarcambiodivisa(float valor)
{
    oTipostarifa->valor_divisa = valor;
}

void FrmTiposTarifa::on_btnBorrar_clicked()
{
    if(oTipostarifa->id == 1 )
        return;
    QSqlQuery sql(Configuracion_global->groupDB);

    bool borrar = QMessageBox::question(qApp->activeWindow(),qApp->tr("Borrar Tarifa"),
                         qApp->tr("¿Desea realmente Borrar Tarifa\n%1\nEsta opción no se puede deshacer").arg(oTipostarifa->descripcion),
                         qApp->tr("No"),qApp->tr("Si")) == QMessageBox::Accepted;
    if(borrar){
    qDebug() << oTipostarifa->id;
    if(!sql.exec("DELETE FROM codigotarifa WHERE id = '"+QString::number(oTipostarifa->id)+"'")){

        QMessageBox::critical(qApp->activeWindow(),QObject::tr("Falló el borrado de la Tarifa"),"Error al borrar la tarifa",QObject::tr("&Aceptar"));
    }
    llenar_lista();
    }
}
