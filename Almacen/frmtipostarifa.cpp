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
    ui->cboPais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));
    llenar_lista();
    QSqlQueryModel *monedas = new QSqlQueryModel(this);
    monedas->setQuery("Select moneda from monedas",QSqlDatabase::database("Maya"));
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
    //ui->txtImpDto->setText(Configuracion_global->toFormatoMoneda( QString::number(oTipostarifa->importe_dto,'f',2)));
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
            QSqlQuery queryArticulo(QSqlDatabase::database("Maya"));
            while (end == false)
            {
                if (queryArticulo.exec("select * from articulos where id >" + QString::number(id)))
                    if(queryArticulo.next()){
                        id = queryArticulo.record().value("id").toInt();
                        // Comprovamos que no exista ya la tarifa
                        QSqlQuery querytarifa(QSqlDatabase::database("Maya"));
                        querytarifa.prepare("Select id from tarifas where id_codigo_tarifa = :id_codigo_tarifa and id_articulo =:id_art");
                        querytarifa.bindValue(":id_codigo_tarifa",oTipostarifa->id);
                        querytarifa.bindValue(":id_art",queryArticulo.record().value("id").toInt());
                        if(querytarifa.exec())
                        {
                            if(!querytarifa.next())
                            {
                                ui->lblDescArt->setText(queryArticulo.record().value("descripcion").toString());
                                querytarifa.prepare("INSERT INTO tarifas (id_articulo,id_pais,id_monedas,margen,"
                                                    "margen_minimo,pvp,id_codigo_tarifa) VALUES (:id_articulo,:id_pais,"
                                                    ":id_monedas,:margen,:margen_minimo,:pvp,:id_codigo_tarifa);");
                                querytarifa.bindValue(":id_articulo",id);
                                querytarifa.bindValue(":id_pais",oTipostarifa->id_pais);
                                querytarifa.bindValue(":id_monedas",oTipostarifa->id_monedas);
                                querytarifa.bindValue(":margen",oTipostarifa->margen);
                                querytarifa.bindValue(":margen_minimo",oTipostarifa->margen_min);
                                double pvp = (queryArticulo.record().value("coste").toDouble()*100)/(100-oTipostarifa->margen);
                                querytarifa.bindValue(":pvp",pvp);
                                querytarifa.bindValue(":id_codigo_tarifa",oTipostarifa->id);
                                if(!querytarifa.exec())
                                    QMessageBox::warning(this,tr("Gestión tipos de tarifa"),
                                                         tr("Ocurrió un fallo al crear la tarifa: %1").arg(querytarifa.lastError().text()));
                            }
                        }



                    }else
                    {
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
    mTarifas->setQuery("select id,descripcion from codigotarifa",QSqlDatabase::database("Maya"));
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
}
