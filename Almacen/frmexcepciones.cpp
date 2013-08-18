#include "frmexcepciones.h"
#include "ui_frmexcepciones.h"

FrmExcepciones::FrmExcepciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmExcepciones)
{
    ui->setupUi(this);
}

FrmExcepciones::~FrmExcepciones()
{
    delete ui;
}

void FrmExcepciones::cargar_articulo(int id_art)
{
    QMap <int,QSqlRecord>  r;
    QString error;
    r = SqlCalls::SelectRecord("articulos",QString("id =%1").arg(id_art),Configuracion_global->groupDB,error);
    QMapIterator<int, QSqlRecord> i(r);
        while (i.hasNext())
        {
            i.next();
            ui->txt_codigo_articulo->setText(i.value().value("codigo").toString());
            ui->txt_descripcion_articulo->setText(i.value().value("descripcion").toString());

        }

}
