#include "db_consulta_view.h"
#include "ui_db_consulta_view.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/datedelegate.h"

db_consulta_view::db_consulta_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::db_consulta_view)
{
    ui->setupUi(this);
    ui->resultado_list->installEventFilter(this);
    this->setWindowTitle(tr("Buscar...."));
    ui->lbltabla->setText(tr("tabla"));
    this->id = 0;
    this->cSQL = "";
}

db_consulta_view::~db_consulta_view()
{
    delete ui;
}

void db_consulta_view::set_texto_tabla(QString tabla)
{
    ui->lbltabla->setText(tabla);

}

void db_consulta_view::set_SQL(QString cSQL)
{
    this->cSQL = cSQL;
    modelo = new QSqlQueryModel(this);
    if (db =="Maya" || db == "Group")
        modelo->setQuery(cSQL,Configuracion_global->groupDB);
    else if(db=="empresa")
        modelo->setQuery(cSQL,Configuracion_global->empresaDB);
    else if(db=="conta")
        modelo->setQuery(cSQL,Configuracion_global->contaDB);
    else if(db=="medic")
        modelo->setQuery(cSQL,Configuracion_global->medicaDB);
    ui->resultado_list->setModel(modelo);

}

void db_consulta_view::set_filtro(QString filtro)
{
    cSQLFiltered = "";
    cSQLFiltered.append(cSQL);
    cSQLFiltered.append(" where ");
    cSQLFiltered.append(ui->cboCampoBusqueda->currentText().trimmed());
    cSQLFiltered.append(" like '%");
    cSQLFiltered.append(filtro);
    cSQLFiltered.append("%'");
    cSQLFiltered.append(" order by ");
    cSQLFiltered.append(ui->cboCampoBusqueda->currentText().trimmed());
    if (db =="Maya" || db == "Group")
        modelo->setQuery(cSQLFiltered,Configuracion_global->groupDB);
    else if(db=="empresa")
        modelo->setQuery(cSQLFiltered,Configuracion_global->empresaDB);
    else if(db=="conta")
        modelo->setQuery(cSQLFiltered,Configuracion_global->contaDB);
    else if(db=="medic")
        modelo->setQuery(cSQLFiltered,Configuracion_global->medicaDB);
}

void db_consulta_view::set_titulo(QString titulo)
{
    this->setWindowTitle(titulo);
}

void db_consulta_view::set_headers(QStringList cabecera)
{
    for (int i = 0; i< cabecera.size();i++)
        modelo->setHeaderData(i+1, Qt::Horizontal, cabecera.at(i));
        ui->resultado_list->setColumnHidden(0,true);

    this->headers = cabecera;
}

void db_consulta_view::set_tamano_columnas(QVariantList tamanos)
{
    for(int i = 0; i<tamanos.size();i++)
        ui->resultado_list->setColumnWidth(i,tamanos.at(i).toInt());
}

void db_consulta_view::set_delegate_monetary(QVariantList cols)
{
    for(int pos=0; pos <= cols.count()-1;pos++)
    {
        ui->resultado_list->setItemDelegateForColumn(cols.at(pos).toInt(),new MonetaryDelegate);
    }
}

void db_consulta_view::set_delegate_fecha(QVariantList cols)
{
    for(int pos=0; pos <= cols.count()-1;pos++)
    {
        ui->resultado_list->setItemDelegateForColumn(cols.at(pos).toInt(),new DateDelegate);
    }
}

void db_consulta_view::set_campoBusqueda(QStringList campos)
{
    campoBusqueda = campos;
    ui->cboCampoBusqueda->addItems(campoBusqueda);
}

void db_consulta_view::set_db(QString nombre_db)
{
    db = nombre_db;
}

bool db_consulta_view::eventFilter(QObject *target, QEvent *event)
{
    Q_UNUSED(target)
    if (event->type() == QEvent::KeyPress)
    {
        ui->resultado_list->blockSignals(true);
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Tab || keyEvent->key() == Qt::Key_Return)
        {
            int row = ui->resultado_list->currentIndex().row();
            id = ui->resultado_list->model()->data(ui->resultado_list->model()->index(row,0)).toInt();
            ui->btn_aceptar->setFocus();
        }
        ui->resultado_list->blockSignals(false);
    }


    return false;
}

void db_consulta_view::on_lineaTextoBuscar_textChanged(const QString &arg1)
{
    set_filtro(arg1);
}

void db_consulta_view::on_resultado_list_clicked(const QModelIndex &index)
{
    ui->resultado_list->blockSignals(true);
    int row = index.row();
    id = ui->resultado_list->model()->data(ui->resultado_list->model()->index(row,0)).toInt();
    ui->resultado_list->blockSignals(false);
}

void db_consulta_view::on_resultado_list_doubleClicked(const QModelIndex &index)
{
    ui->resultado_list->blockSignals(true);
    int row = index.row();
    id = ui->resultado_list->model()->data(ui->resultado_list->model()->index(row,0)).toInt();
    ui->resultado_list->blockSignals(false);
    ui->btn_aceptar->click();
}
