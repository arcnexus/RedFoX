#include "frmselectlotes.h"
#include "ui_frmselectlotes.h"
#include "Auxiliares/datedelegate.h"
#include "Auxiliares/monetarydelegate.h"

frmSelectLotes::frmSelectLotes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSelectLotes)
{
    ui->setupUi(this);
    modelo = new QSqlQueryModel(this);
    modelo->setQuery("select * from articulos_lotes where id = 0",Configuracion_global->groupDB);
    ui->tablaLotes->setModel(modelo);
    QStringList headers;
    QVariantList sizes;
    headers << tr("id") << tr("Lote") << tr("Caducidad") <<tr("Stock");
    sizes << 0 << 260 << 100 <<100;
    for (int i = 0; i < headers.size();i++)
    {
        ui->tablaLotes->setColumnWidth(i,sizes.at(i).toInt());
        modelo->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }

}

frmSelectLotes::~frmSelectLotes()
{
    delete ui;
}

void frmSelectLotes::set_id_articulo(int id_art)
{
    modelo->setQuery(QString("select id,lote,caducidad, stock from articulos_lotes where id_articulo = %1").arg(id_art),
                     Configuracion_global->groupDB);
}

QString frmSelectLotes::get_lote()
{
    return this->lote;
}

int frmSelectLotes::get_id()
{
    return this->id;
}

void frmSelectLotes::on_tablaLotes_doubleClicked(const QModelIndex &index)
{
    int id = ui->tablaLotes->model()->data(ui->tablaLotes->model()->index(index.row(),0)).toInt();
    this->id = id;
    QString error;
    if(id>0)
    {
        QMap <int,QSqlRecord> art;
        art = SqlCalls::SelectRecord("articulos_lotes",QString("id = %1").arg(id),Configuracion_global->groupDB,error);

        this->lote = tr(" Lote: %1").arg(art.value(id).value("lote").toString());
        this->lote.append(tr(" Cad:%1").arg(art.value(id).value("caducidad").toDate().toString("dd/MM/yyyy")));
        accept();
    }
    else{
        QMessageBox::warning(qApp->activeWindow(),tr("Lotes"),tr("Debe seleccionar un lote para poder vender este producto."),
                             tr("Aceptar"));
        if(QMessageBox::question(qApp->activeWindow(),tr("Lotes"),tr("¿Desea forzar la venta del producto sin especificar lote?"),
                                 tr("No"),tr("Sí")) == QMessageBox::Accepted)
        {
            this->lote = "Lote:";
            accept();
        } else
            close();
    }

}
