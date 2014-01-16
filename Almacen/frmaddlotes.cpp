#include "frmaddlotes.h"
#include "ui_frmaddlotes.h"

frmaddLotes::frmaddLotes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmaddLotes)
{
    ui->setupUi(this);
}

frmaddLotes::~frmaddLotes()
{
    delete ui;
}

void frmaddLotes::cargar_articulo(int id)
{
    QMap <int,QSqlRecord> art;
    QString error;
    art = SqlCalls::SelectRecord("articulos",QString("id=%1").arg(id),Configuracion_global->groupDB,error);
    ui->lblcodigo->setText(art.value(id).value("codigo").toString());
    ui->lbldescripcion->setText(art.value(id).value("descripcion").toString());
    ui->dateCaducidad->setDate(QDate::currentDate());
    this->id_articulo = id;
}

void frmaddLotes::on_btnAnadir_clicked()
{
    QHash <QString,QVariant> lote;
    QString error;
    lote["id_articulo"] = this->id_articulo;
    lote["lote"] = ui->txtLote->text();
    lote["caducidad"] = ui->dateCaducidad->date();
    if(ui->txtLote->text().isEmpty())
        QMessageBox::warning(this,tr("Gestión de artículos"),tr("Debe especificar el lote"),tr("Aceptar"));
    else
    {
        int new_id = SqlCalls::SqlInsert(lote,"articulos_lotes",Configuracion_global->groupDB,error);
        if(new_id <1)
            QMessageBox::warning(this,tr("Gestión de articulos"),tr("No se pudo añadir el artíclo: %1").arg(error),
                                 tr("Aceptar"));
        else
            accept();

    }

}
