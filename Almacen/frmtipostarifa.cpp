#include "frmtipostarifa.h"
#include "ui_frmtipostarifa.h"



FrmTiposTarifa::FrmTiposTarifa(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmTiposTarifa)
{
    ui->setupUi(this);
    ui->lblDescArt->setVisible(false);
    oTipostarifa = new TiposTarifa(this);

    ui->cboPais->setModel(Configuracion_global->paises_model);
    //ui->cboPais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));
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
    ui->txtporc_dto1->setText(QString::number(oTipostarifa->porc_dto1));
    ui->txtporc_dto2->setText(QString::number(oTipostarifa->porc_dto2));
    ui->txtporc_dto3->setText(QString::number(oTipostarifa->porc_dto3));
    ui->txtporc_dto4->setText(QString::number(oTipostarifa->porc_dto4));
    ui->txtporc_dto5->setText(QString::number(oTipostarifa->porc_dto5));
    ui->txtporc_dto6->setText(QString::number(oTipostarifa->porc_dto6));
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
        if(ui->radTodos->isChecked())
        {
            int id = 0;
            bool end = false;
            QSqlQuery queryArticulo(Configuracion_global->groupDB);
            // TODO PREPARAR PARA MULTIDIVISA
            QString cod_div_local = Configuracion_global->cod_divisa_local;
            QString cod_div_extra = Configuracion_global->Devolver_codDivisa(oTipostarifa->id_monedas);
            if (cod_div_local!= cod_div_extra)
                oTipostarifa->valor_divisa = Configuracion_global->getCambioBlock(cod_div_local,cod_div_extra);
                //Configuracion_global->getCambio(cod_div_local,cod_div_extra,1);
            else
            {
                oTipostarifa->valor_divisa = 1;
            }


            while (end == false)
            {
                if (queryArticulo.exec("select * from articulos where id >" + QString::number(id)))
                    if(queryArticulo.next()){
                        id = queryArticulo.record().value("id").toInt();
                        // Comprovamos que no exista ya la tarifa
                        QSqlQuery querytarifa(Configuracion_global->groupDB);
                        querytarifa.prepare("Select id from tarifas where id_codigo_tarifa = :id_codigo_tarifa and id_articulo =:id_art");
                        querytarifa.bindValue(":id_codigo_tarifa",oTipostarifa->id);
                        querytarifa.bindValue(":id_art",queryArticulo.record().value("id").toInt());
                        if(querytarifa.exec())
                        {
                            if(!querytarifa.next())
                            {
                                ui->lblDescArt->setText(queryArticulo.record().value("descripcion").toString());
                                QApplication::processEvents();
                                querytarifa.prepare("INSERT INTO tarifas (id_articulo,id_pais,id_monedas,margen,"
                                                    "margen_minimo,pvp,id_codigo_tarifa,porc_dto1,porc_dto2,"
                                                    "porc_dto3,porc_dto4,porc_dto5,porc-dto6) VALUES (:id_articulo,:id_pais,"
                                                    ":id_monedas,:margen,:margen_minimo,:pvp,:id_codigo_tarifa,"
                                                    ":porc_dto1,:porc_dto2,:porc_dto3,:porc_dto4,:porc_dto5,:porc_dto6);");
                                querytarifa.bindValue(":id_articulo",id);
                                querytarifa.bindValue(":id_pais",oTipostarifa->id_pais);
                                querytarifa.bindValue(":id_monedas",oTipostarifa->id_monedas);
                                querytarifa.bindValue(":margen",oTipostarifa->margen);
                                querytarifa.bindValue(":margen_minimo",oTipostarifa->margen_min);
                                querytarifa.bindValue(":id_codigo_tarifa",oTipostarifa->id);
                                querytarifa.bindValue(":porc_dto1", oTipostarifa->porc_dto1);
                                querytarifa.bindValue(":porc_dto2", oTipostarifa->porc_dto2);
                                querytarifa.bindValue(":porc_dto3", oTipostarifa->porc_dto3);
                                querytarifa.bindValue(":porc_dto4", oTipostarifa->porc_dto4);
                                querytarifa.bindValue(":porc_dto5", oTipostarifa->porc_dto5);
                                querytarifa.bindValue(":porc_dto6", oTipostarifa->porc_dto6);

                                double coste = queryArticulo.record().value("coste_real").toDouble();
                                float margen = oTipostarifa->margen;
                                double pvp, pvp_divisa;
                                pvp = (coste*100)/(100-margen);
                                pvp_divisa = pvp * oTipostarifa->valor_divisa;
                                if (oTipostarifa->valor_divisa == 1)
                                    querytarifa.bindValue(":pvp",pvp);
                                else
                                    querytarifa.bindValue(":pvp",pvp_divisa);
                                if(!querytarifa.exec())
                                {
                                    QMessageBox::warning(this,tr("Tipos de tarifas"),
                                                         tr("Ocurrió un error al crear la tarifa: %1 ").arg(querytarifa.lastError().text()),
                                                         tr("Aceptar"));
                                    break;
                                }
                            }
                        }
                     } else {
                        end = true;
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
    ui->txtCodigo->setEnabled(state);
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
    QSqlQueryModel *mTarifas = new QSqlQueryModel(this);
    mTarifas->setQuery("select id,descripcion from codigotarifa",Configuracion_global->groupDB);
    ui->listatarifas->setModel(mTarifas);
    ui->listatarifas->setColumnHidden(0,true);
    mTarifas->setHeaderData(1,Qt::Horizontal,tr("Tarifa"));
}

void FrmTiposTarifa::cargar_datos_en_objeto()
{
    oTipostarifa->codigo_tarifa = ui->txtCodigo->text();
    oTipostarifa->descripcion = ui->txtDescripcion->text();
    oTipostarifa->margen = ui->spnMargen->value();
    oTipostarifa->margen_min = ui->spnMargen_min->value();
    oTipostarifa->id_monedas = Configuracion_global->Devolver_id_moneda(ui->cboMoneda->currentText());
    oTipostarifa->id_pais = Configuracion_global->Devolver_id_pais(ui->cboPais->currentText());
    oTipostarifa->porc_dto1 = ui->txtporc_dto1->text().toFloat();
    oTipostarifa->porc_dto2 = ui->txtporc_dto2->text().toFloat();
    oTipostarifa->porc_dto3 = ui->txtporc_dto3->text().toFloat();
    oTipostarifa->porc_dto4 = ui->txtporc_dto4->text().toFloat();
    oTipostarifa->porc_dto5 = ui->txtporc_dto5->text().toFloat();
    oTipostarifa->porc_dto6 = ui->txtporc_dto6->text().toFloat();
}

void FrmTiposTarifa::asignarcambiodivisa(float valor)
{
    oTipostarifa->valor_divisa = valor;
}
