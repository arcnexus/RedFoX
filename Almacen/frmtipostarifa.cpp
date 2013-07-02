#include "frmtipostarifa.h"
#include "ui_frmtipostarifa.h"



FrmTiposTarifa::FrmTiposTarifa(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmTiposTarifa)
{
    ui->setupUi(this);
    ui->lblDescArt->setVisible(false);
    oTipostarifa = new TiposTarifa(this);
    QSqlQueryModel *mTarifas = new QSqlQueryModel(this);
    mTarifas->setQuery("select id,descripcion from codigotarifa",QSqlDatabase::database("Maya"));
    ui->listatarifas->setModel(mTarifas);
    ui->listatarifas->setColumnHidden(0,true);
    mTarifas->setHeaderData(1,Qt::Horizontal,tr("Tarifa"));
    ui->cboPais->setModel(Configuracion_global->paises_model);
    ui->cboPais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    QSqlQueryModel *monedas = new QSqlQueryModel(this);
    monedas->setQuery("Select moneda from monedas",QSqlDatabase::database("Maya"));
    ui->cboMoneda->setModel(monedas);

}

FrmTiposTarifa::~FrmTiposTarifa()
{
    delete ui;
}

void FrmTiposTarifa::cargarDatos()
{
    ui->txtCodigo->setText(oTipostarifa->codigo_tarifa);
    ui->txtDescripcion->setText(oTipostarifa->descripcion);
    ui->txtMargen->setValue(oTipostarifa->margen);
    ui->txtMargen_min->setValue(oTipostarifa->margen_min);
    //ui->txtImpDto->setText(Configuracion_global->toFormatoMoneda( QString::number(oTipostarifa->importe_dto,'f',2)));
    //ui->txtPromocion->setText(oTipostarifa->desc_promo);
}

void FrmTiposTarifa::on_btnAgregarTarifa_clicked()
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
                            querytarifa.bindValue(":id_codigo_tarifa",oTipostarifa->codigo_tarifa);
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

void FrmTiposTarifa::on_listatarifas_clicked(const QModelIndex &index)
{
    int nid =ui->listatarifas->model()->data(ui->listatarifas->model()->index(index.row(),0),Qt::EditRole).toInt();
    oTipostarifa->recuperar(nid);
    cargarDatos();

}
