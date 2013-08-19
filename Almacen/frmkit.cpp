#include "frmkit.h"
#include "ui_frmkit.h"
#include "../Auxiliares/datedelegate.h"
#include "../Auxiliares/monetarydelegate.h"
#include"../Almacen/articulo.h"

FrmKit::FrmKit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmKit)
{
    ui->setupUi(this);
    QStringList campos;
    campos << tr("Descripción") << tr("Código");
    ui->cboOrden->addItems(campos);
    QStringList sentido;
    sentido << tr("A-Z") <<tr("Z-A");
    ui->cboSentido->addItems(sentido);
    ui->txtBuscar->setFocus();
    //------------------------
    // Tabla Buscar Articulos
    //------------------------
    m_arts = new QSqlQueryModel(this);
    m_arts->setQuery("select id,codigo,descripcion,pvp_con_iva from vistaart_tarifa where tarifa = 1",Configuracion_global->groupDB);
    ui->tabla_buscar->setModel(m_arts);
    m_arts->setHeaderData(1,Qt::Horizontal,tr("Código"));
    m_arts->setHeaderData(2,Qt::Horizontal,tr("Descripción"));
    m_arts->setHeaderData(3,Qt::Horizontal,tr("PVP"));

    ui->tabla_buscar->setColumnHidden(0,true);
    ui->tabla_buscar->setItemDelegateForColumn(3,new MonetaryDelegate);
    ui->tabla_buscar->setColumnWidth(1,100);
    ui->tabla_buscar->setColumnWidth(2,250);
    ui->tabla_buscar->setColumnWidth(3,100);

    //-----------------------
    // Tabla escandallo
    //-----------------------
    m_kits = new QSqlQueryModel(this);
    m_kits->setQuery(QString("select codigo_componente,descripcion,cantidad,precio from kits limit 0"),
                     Configuracion_global->groupDB);
    ui->tabla->setModel(m_kits);

}

FrmKit::~FrmKit()
{
    delete ui;
}

void FrmKit::set_articulo(QString codigo)
{
    //-----------------------
    // Campos artículos
    //-----------------------
    QMap<int,QSqlRecord> rec;
    QString error;
    QStringList condiciones;
    condiciones << QString("codigo='%1'").arg(codigo) << "tarifa = 1";
    rec = SqlCalls::SelectRecord("vistaart_tarifa",condiciones,Configuracion_global->groupDB,error);

    QMapIterator <int,QSqlRecord> i(rec);
    while (i.hasNext())
    {
        i.next();
        ui->txtCodigo_kit->setText(i.value().value("codigo").toString());
        ui->txtDesc_kit->setText(i.value().value("descripcion").toString());
        ui->txtPvp_kit->setText(Configuracion_global->toFormatoMoneda(QString::number(i.value().value("pvp_con_iva").toDouble(),'f',
                                                                                      Configuracion_global->decimales)));
    }

    refrescar_tabla_escandallo(codigo);



}

void FrmKit::on_btnAnadir_clicked()
{
    QModelIndex index = ui->tabla_buscar->currentIndex();
    int id = ui->tabla_buscar->model()->data(ui->tabla_buscar->model()->index(index.row(),0),Qt::EditRole).toInt();

    QMap <int,QSqlRecord> r;
    QStringList clausulas;
    clausulas << QString("id = %1").arg(id) << "tarifa = 1";
    QString error;
    r = SqlCalls::SelectRecord("vistaart_tarifa",clausulas,Configuracion_global->groupDB,error);
    QMapIterator <int, QSqlRecord> i(r);
    while(i.hasNext())
    {
        i.next();
        ui->txtCodigo->setText(i.value().value("codigo").toString());
        ui->txtDescripcion->setText(i.value().value("descripcion").toString());
        ui->chkStock->setChecked(i.value().value("controlar_stock").toBool());
        ui->spinPVP->setValue(i.value().value("pvp").toDouble());


    }
    ui->btnGuardar->setEnabled(true);
    ui->btnAnadir->setEnabled(false);
    ui->btnQuitar->setEnabled(false);
}

void FrmKit::on_btnGuardar_clicked()
{
    QHash <QString, QVariant> h;
    h["codigo_kit"] = ui->txtCodigo_kit->text();
    h["codigo_componente"] = ui->txtCodigo->text();
    h["descripcion"] = ui->txtDescripcion->text();
    h["cantidad"] = ui->spinCantidad->value();
    h["porc_dto"] = ui->txtDto->text().toFloat();
    h["descontar_stock"] = ui->chkStock->isChecked();
    h["precio"] = ui->spinPVP->value();

    QString error;
    int id = SqlCalls::SqlInsert(h,"kits",Configuracion_global->groupDB,error);
    if(id == -1)
        QMessageBox::warning(this,tr("Gestión de KITS"),tr("Ocurrió un fallo al insertar al kit: ")+error,tr("Aceptar"));
    else
        refrescar_tabla_escandallo(ui->txtCodigo_kit->text());
    ui->btnAnadir->setEnabled(true);
    ui->btnDeshacer->setEnabled(false);
    ui->btnGuardar->setEnabled(false);
    ui->btnQuitar->setEnabled(true);


}

void FrmKit::refrescar_tabla_escandallo(QString codigo)
{
    //-----------------------
    // Tabla escandallo
    //-----------------------
    m_kits = new QSqlQueryModel(this);
    m_kits->setQuery(QString("select codigo_componente,descripcion,cantidad,precio from kits where codigo_kit = '%1'").arg(codigo),
                     Configuracion_global->groupDB);
    ui->tabla->setModel(m_kits);
    QStringList headers;
    headers << tr("Código") <<tr("Descripción") << tr("Cantidad") << tr("Precio");
    QVariantList sizes;
    sizes << 100 << 300 << 100 <<100;
    for(int x= 0; x< headers.length(); x++)
    {
        ui->tabla->setColumnWidth(x,sizes.at(x).toInt());
        m_kits->setHeaderData(x,Qt::Horizontal,headers.at(x));
    }
}

void FrmKit::on_txtBuscar_textEdited(const QString &arg1)
{
    QString SQL;
    QHash <QString,QString> orden;
    // << tr("Descripción") << tr("Código")
    orden["Descripción"] = "descripcion";
    orden["Código"] = "codigo";
    QString sentido = "";
    if(ui->cboSentido->currentText() == tr("Z-A"))
        sentido = "DESC";


    QString ord = orden.value(ui->cboOrden->currentText());

    SQL = "select id,codigo,descripcion,pvp_con_iva from vistaart_tarifa where "+ord+
            " like '%"+arg1.trimmed()+"%' and tarifa = 1 order by "+ord+" "+sentido;
    m_arts->setQuery(SQL,Configuracion_global->groupDB);

}
