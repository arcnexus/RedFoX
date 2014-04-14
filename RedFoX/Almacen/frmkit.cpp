#include "frmkit.h"
#include "ui_frmkit.h"
#include "Auxiliares/datedelegate.h"
#include "Auxiliares/monetarydelegate.h"
#include"../Almacen/articulo.h"

void FrmKit::refreshCantidades()
{
    QMap<QString,double> cantidades;
    for(auto i = 0; i< m_kits->rowCount(); i++)
            cantidades.insert(m_kits->record(i).value("codigo_componente").toString(),m_kits->record(i).value("cantidad").toDouble());

    double cant = -1.0;
    for(auto i = 0; i< m_arts->rowCount(); i++)
    {
        if(cantidades.contains(m_arts->record(i).value("codigo").toString()))
        {
            if(cant == -1.0)
                cant = m_arts->record(i).value("stock_fisico_almacen").toDouble() / cantidades.value(m_arts->record(i).value("codigo").toString());
            else
            {
                double x = m_arts->record(i).value("stock_fisico_almacen").toDouble() / cantidades.value(m_arts->record(i).value("codigo").toString());
                cant = qMin(cant,x);
            }
        }
    }
    cant = qMax(0,qRound(cant - 0.5));
    ui->spinCant->setMaximum(cant);
}

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
    ui->tabla_buscar->setModel(m_arts);
    m_arts->setHeaderData(1,Qt::Horizontal,tr("Código"));
    m_arts->setHeaderData(2,Qt::Horizontal,tr("Descripción"));
    m_arts->setHeaderData(3,Qt::Horizontal,tr("Coste"));

    ui->tabla_buscar->setItemDelegateForColumn(3,new MonetaryDelegate);


    //-----------------------
    // Tabla escandallo
    //-----------------------
    m_kits = new QSqlQueryModel(this);
    m_kits->setQuery(QString("select id,codigo_componente,descripcion,cantidad,coste_base,porc_dto,coste_final from kits limit 0"),
                     Configuracion_global->groupDB);
    ui->tabla->setModel(m_kits);
    ui->tabla->setColumnHidden(0,true);

    connect(ui->spinCoste,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
    connect(ui->spinDto,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));    
}

FrmKit::~FrmKit()
{
    delete ui;
}

void FrmKit::set_articulo(QString codigo , QString Descripcion, int stock)
{
    ui->txtCodigo_kit->setText(codigo);
    ui->txtDesc_kit->setText(Descripcion);
    m_arts->setQuery("select id,codigo,descripcion,coste_real, stock_fisico_almacen from articulos where kit = 0",Configuracion_global->groupDB);
    ui->tabla_buscar->setColumnHidden(0,true);
    ui->tabla_buscar->setColumnHidden(4,true);
    ui->tabla_buscar->setColumnWidth(1,100);
    ui->tabla_buscar->setColumnWidth(2,250);
    ui->tabla_buscar->setColumnWidth(3,100);
    refrescar_tabla_escandallo(codigo);
    refreshCantidades();
    ui->spinCantRomper->setMaximum(stock);
    this->stock = stock;
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
        set_articulo(ui->txtCodigo_kit->text(),ui->txtDescripcion->text(),this->stock);
}

void FrmKit::refrescar_tabla_escandallo(QString codigo)
{
    //-----------------------
    // Tabla escandallo
    //-----------------------
    m_kits->setQuery(QString("select id,codigo_componente,descripcion,cantidad,coste_base,porc_dto,coste_final from kits where codigo_kit = '%1'").arg(codigo),
                     Configuracion_global->groupDB);
    ui->tabla->setModel(m_kits);
    QStringList headers;
    headers << "" << tr("Código") <<tr("Descripción") << tr("Cantidad") << tr("Coste Base") << tr("DTO") << tr("Coste en Kit");
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
    bool t = Configuracion_global->groupDB.transaction();
    QString error = "";
    // Descontar stock artículos
    QMap<QString,double> cantidades;
    for(auto i = 0; i< m_kits->rowCount(); i++)
        cantidades.insert(m_kits->record(i).value("codigo_componente").toString(),m_kits->record(i).value("cantidad").toDouble());
    QMapIterator<QString,double> _itCant(cantidades);
    while(_itCant.hasNext())
    {
        _itCant.next();
        QMap <int,QSqlRecord> art=
                SqlCalls::SelectRecord("articulos",QString("codigo ='%1'").arg(_itCant.key()),Configuracion_global->groupDB,error);

        if(!art.isEmpty() && art.first().value("controlar_stock").toBool())
        {
            float cant = cantidades.value(art.first().value("codigo").toString());
            QString SQL = QString("update articulos set stock_real = stock_real - %1,"
                          "stock_fisico_almacen = stock_fisico_almacen -%1 where id = %2").arg((cant*ui->spinCant->value())).arg(
                        art.first().value("id").toInt());
            QSqlQuery articulo(Configuracion_global->groupDB);
            if(!articulo.exec(SQL))
                QMessageBox::warning(this,tr("Gestión de Kits"),tr("Ocurrió un error al actulizar stock:\n%1").arg(
                                         articulo.lastError().text()));
        }
    }

    //Aumentar stock Kit
    QMap <int,QSqlRecord> kit = SqlCalls::SelectRecord("articulos",QString("codigo ='%1'").arg(ui->txtCodigo_kit->text()),
                                                       Configuracion_global->groupDB,error);
    int s;
    if(error.isEmpty() && !kit.isEmpty())
    {
        QSqlRecord r = kit.first();
        QHash <QString,QVariant> _data;
        s = r.value("stock_fisico_almacen").toDouble() + ui->spinCant->value();
        _data["stock_fisico_almacen"] = s;
        _data["stock_real"] = r.value("stock_real").toDouble() + ui->spinCant->value();
        if(SqlCalls::SqlUpdate(_data,"articulos",Configuracion_global->groupDB,QString("id = %1").arg(r.value("id").toInt()),error))
        {
            if(t)
                Configuracion_global->groupDB.commit();
            TimedMessageBox::Box(this,tr("Estock de artículos actualizado"));
        }
        else
        {
            if(t)
                Configuracion_global->groupDB.rollback();
        }
    }

    if(!error.isEmpty())
    {
        QMessageBox::warning(this,tr("Gestión de Kits"),tr("Ocurrió un error al recuperar el kit: %1").arg(error));
    }
    set_articulo(ui->txtCodigo_kit->text(),ui->txtDescripcion->text(),s);
}

void FrmKit::on_btnQuitar_clicked()
{
    if(!ui->tabla->currentIndex().isValid())
        return;
    bool t = Configuracion_global->groupDB.transaction();
    QString codigo = m_kits->record(ui->tabla->currentIndex().row()).value("codigo_componente").toString();
    float cant =  m_kits->record(ui->tabla->currentIndex().row()).value("cantidad").toFloat();
    int id = m_kits->record(ui->tabla->currentIndex().row()).value("id").toInt();
    QString error;

    if(QMessageBox::question(this,tr("Eliminar componente de kit"),tr("¿Desea devolver el stock al componente?"),
                          tr("No"),
                          tr("Si")) == QMessageBox::Accepted)
    {
        QMap <int,QSqlRecord> art=
                SqlCalls::SelectRecord("articulos",QString("codigo ='%1'").arg(codigo),Configuracion_global->groupDB,error);

        if(!art.isEmpty() && art.first().value("controlar_stock").toBool())
        {
            QString SQL = QString("update articulos set stock_real = stock_real + %1,"
                          "stock_fisico_almacen = stock_fisico_almacen +%1 where id = %2").arg((cant*ui->spinCantRomper->maximum())).arg(
                        art.first().value("id").toInt());
            QSqlQuery articulo(Configuracion_global->groupDB);
            if(!articulo.exec(SQL))
                QMessageBox::warning(this,tr("Gestión de Kits"),tr("Ocurrió un error al actulizar stock:\n%1").arg(
                                         articulo.lastError().text()));
            else
                TimedMessageBox::Box(this,tr("Estock de artículos actualizado"));
        }
        else if(!error.isEmpty())
        {
            if(t)
                Configuracion_global->groupDB.rollback();
            QMessageBox::critical(this,tr("Error al actualizar stock"),error);
            return;
        }
    }
    if(!SqlCalls::SqlDelete("kits",Configuracion_global->groupDB,QString("id = %1").arg(id),error))
    {
        if(t)
            Configuracion_global->groupDB.rollback();
        QMessageBox::critical(this,tr("Error al eliminar componente"),error);
        return;
    }
    if(t)
    {
        if(!Configuracion_global->groupDB.commit())
        {
            if(t)
                Configuracion_global->groupDB.rollback();
            QMessageBox::critical(this,tr("Error al guardar en base de datos"),error);
        }
        else
            TimedMessageBox::Box(this,tr("Componente eliminado con éxito"));
    }
    set_articulo(ui->txtCodigo_kit->text(),ui->txtDescripcion->text(),this->stock);
}

void FrmKit::valueChanged(double)
{
    double x = ui->spinCoste->value() * (1-(ui->spinDto->value()/100));
    ui->lblCoste->setText(QString::number(x,'f',2));
}

void FrmKit::on_btnRomperKit_clicked()
{
    bool t = Configuracion_global->groupDB.transaction();
    QString error = "";
    // Aumentar stock artículos
    QMap<QString,double> cantidades;
    for(auto i = 0; i< m_kits->rowCount(); i++)
        cantidades.insert(m_kits->record(i).value("codigo_componente").toString(),m_kits->record(i).value("cantidad").toDouble());
    QMapIterator<QString,double> _itCant(cantidades);
    while(_itCant.hasNext())
    {
        _itCant.next();
        QMap <int,QSqlRecord> art=
                SqlCalls::SelectRecord("articulos",QString("codigo ='%1'").arg(_itCant.key()),Configuracion_global->groupDB,error);

        if(!art.isEmpty() && art.first().value("controlar_stock").toBool())
        {
            float cant = cantidades.value(art.first().value("codigo").toString());
            QString SQL = QString("update articulos set stock_real = stock_real + %1,"
                          "stock_fisico_almacen = stock_fisico_almacen +%1 where id = %2").arg((cant*ui->spinCantRomper->value())).arg(
                        art.first().value("id").toInt());
            QSqlQuery articulo(Configuracion_global->groupDB);
            if(!articulo.exec(SQL))
                QMessageBox::warning(this,tr("Gestión de Kits"),tr("Ocurrió un error al actulizar stock:\n%1").arg(
                                         articulo.lastError().text()));
        }
    }

    //Aumentar stock Kit
    QMap <int,QSqlRecord> kit = SqlCalls::SelectRecord("articulos",QString("codigo ='%1'").arg(ui->txtCodigo_kit->text()),
                                                       Configuracion_global->groupDB,error);
    int s;
    if(error.isEmpty() && !kit.isEmpty())
    {
        QSqlRecord r = kit.first();
        QHash <QString,QVariant> _data;
        s = r.value("stock_fisico_almacen").toDouble() - ui->spinCantRomper->value();
        _data["stock_fisico_almacen"] = s;
        _data["stock_real"] = r.value("stock_real").toDouble() - ui->spinCantRomper->value();
        if(SqlCalls::SqlUpdate(_data,"articulos",Configuracion_global->groupDB,QString("id = %1").arg(r.value("id").toInt()),error))
        {
            if(t)
                Configuracion_global->groupDB.commit();
            TimedMessageBox::Box(this,tr("Estock de artículos actualizado"));
        }
        else
        {
            if(t)
                Configuracion_global->groupDB.rollback();
        }
    }

    if(!error.isEmpty())
    {
        QMessageBox::warning(this,tr("Gestión de Kits"),tr("Ocurrió un error al recuperar el kit:\n %1").arg(error));
    }
    set_articulo(ui->txtCodigo_kit->text(),ui->txtDescripcion->text(),s);
}
