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
    m_arts->setQuery("select id,codigo,descripcion,coste_real from articulos",Configuracion_global->groupDB);
    ui->tabla_buscar->setModel(m_arts);
    m_arts->setHeaderData(1,Qt::Horizontal,tr("Código"));
    m_arts->setHeaderData(2,Qt::Horizontal,tr("Descripción"));
    m_arts->setHeaderData(3,Qt::Horizontal,tr("Coste"));

    ui->tabla_buscar->setColumnHidden(0,true);
    ui->tabla_buscar->setItemDelegateForColumn(3,new MonetaryDelegate);
    ui->tabla_buscar->setColumnWidth(1,100);
    ui->tabla_buscar->setColumnWidth(2,250);
    ui->tabla_buscar->setColumnWidth(3,100);

    //-----------------------
    // Tabla escandallo
    //-----------------------
    m_kits = new QSqlQueryModel(this);
    m_kits->setQuery(QString("select codigo_componente,descripcion,cantidad,coste_base,porc_dto,coste_final from kits limit 0"),
                     Configuracion_global->groupDB);
    ui->tabla->setModel(m_kits);

    connect(ui->spinCoste,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
    connect(ui->spinDto,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
}

FrmKit::~FrmKit()
{
    delete ui;
}

void FrmKit::set_articulo(QString codigo , QString Descripcion)
{
    ui->txtCodigo_kit->setText(codigo);
    ui->txtDesc_kit->setText(Descripcion);

    refrescar_tabla_escandallo(codigo);
}

double FrmKit::getCoste()
{
    double d = 0.0;
    for(auto i=0; i< m_kits->rowCount(); i++)
    {
        d += (m_kits->record(i).value(2).toDouble() * m_kits->record(i).value(5).toDouble());
    }
    return d;
}

void FrmKit::on_btnAnadir_clicked()
{
    if(!ui->tabla_buscar->currentIndex().isValid())
        return;

    QSqlRecord r = m_arts->record(ui->tabla_buscar->currentIndex().row());

    ui->txtCodigo->setText(r.value("codigo").toString());
    ui->txtDescripcion->setText(r.value("descripcion").toString());
    ui->spinCoste->setValue(r.value("coste_real").toDouble());
    this->id_componente = r.value("id").toInt();
}

void FrmKit::on_btnGuardar_clicked()
{
    if(ui->txtCodigo->text().isEmpty())
        return;

    if(ui->txtCodigo->text() == ui->txtCodigo_kit->text())
        return;

    for(auto i=0; i< m_kits->rowCount(); i++)
    {
        if(m_kits->record(i).value(0).toString() == ui->txtCodigo->text())
            return;
    }

    QHash <QString, QVariant> h;
    h["codigo_kit"] = ui->txtCodigo_kit->text();
    h["codigo_componente"] = ui->txtCodigo->text();
    h["descripcion"] = ui->txtDescripcion->text();
    h["cantidad"] = ui->spinCantidad->value();
    h["porc_dto"] = ui->spinDto->value();
    h["coste_base"] = ui->spinCoste->value();
    h["coste_final"] = ui->lblCoste->text().toDouble();
    h["id_componente"] = this->id_componente;

    QString error;
    int id = SqlCalls::SqlInsert(h,"kits",Configuracion_global->groupDB,error);
    if(id == -1)
        QMessageBox::warning(this,tr("Gestión de KITS"),tr("Ocurrió un fallo al insertar al kit: ")+error,tr("Aceptar"));
    else
        refrescar_tabla_escandallo(ui->txtCodigo_kit->text());
}

void FrmKit::refrescar_tabla_escandallo(QString codigo)
{
    //-----------------------
    // Tabla escandallo
    //-----------------------
    m_kits->setQuery(QString("select codigo_componente,descripcion,cantidad,coste_base,porc_dto,coste_final from kits where codigo_kit = '%1'").arg(codigo),
                     Configuracion_global->groupDB);
    ui->tabla->setModel(m_kits);
    QStringList headers;
    headers << tr("Código") <<tr("Descripción") << tr("Cantidad") << tr("Coste Base") << tr("DTO") << tr("Coste en Kit");
    for(int x= 0; x< headers.length(); x++)
    {
        ui->tabla->setColumnWidth(x,105);
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

    SQL = "select id,codigo,descripcion,coste_real from articulos where "+ord+
            " like '%"+arg1.trimmed()+"%' order by "+ord+" "+sentido;
    m_arts->setQuery(SQL,Configuracion_global->groupDB);

}

void FrmKit::on_btnAnadirKits_clicked()
{
    //--------------------------
    // Descontar stock artículos
    //--------------------------
    QMap <int,QSqlRecord> kit;
    QString error;
    QSqlQuery *articulo = new QSqlQuery(Configuracion_global->groupDB);
    QString SQL;
    kit = SqlCalls::SelectRecord("kits",QString("codigo_kit = '%1'").arg(ui->txtCodigo_kit->text()),Configuracion_global->groupDB,error);
    if(error.isEmpty())
    {
        QMapIterator <int,QSqlRecord> i(kit);
        while (i.hasNext())
        {
            i.next();
            int id_componente = i.value().value("id_componente").toInt();
            QMap <int,QSqlRecord> art;
            art = SqlCalls::SelectRecord("articulos",QString("codigo ='%1'").arg(i.value().value("codigo_componente").toString()),
                                         Configuracion_global->groupDB,error);
            if(art.value(id_componente).value("controlar_stock").toBool())
            {
                float cant = i.value().value("cantidad").toFloat();
                SQL = QString("update articulos set stock_real = stock_real - %1,"
                              "stock_fisico_almacen = stock_fisico_almacen -%1 where id = %2").arg((cant*ui->spinCant->value())).arg(
                            id_componente);
                if(!articulo->exec(SQL))
                    QMessageBox::warning(this,tr("Gestión de Kits"),tr("Ocurrió un error al actulizar stock: %1").arg(
                                             articulo->lastError().text()));
            }

        }
        TimedMessageBox *t = new TimedMessageBox(this,tr("Estock de artículos actualizado"));
    } else
        QMessageBox::warning(this,tr("Gestión de Kits"),tr("Ocurrió un error al recuperar el kit: %1").arg(error));
}

void FrmKit::on_btnQuitar_clicked()
{
    //TODO quitar elemnto kit
}

void FrmKit::valueChanged(double)
{
    double x = ui->spinCoste->value() * (1-(ui->spinDto->value()/100));
    ui->lblCoste->setText(QString::number(x,'f',2));
}
