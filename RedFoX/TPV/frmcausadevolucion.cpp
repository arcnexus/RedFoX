#include "frmcausadevolucion.h"
#include "ui_frmcausadevolucion.h"
#include "../Almacen/articulo.h"

frmCausaDevolucion::frmCausaDevolucion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmCausaDevolucion)
{
    ui->setupUi(this);
    ui->cmbUsuario->setModel(Configuracion_global->usuarios_model);
    ui->dateDevolucion->setDateTime(QDateTime::currentDateTime());
}

frmCausaDevolucion::~frmCausaDevolucion()
{
    delete ui;
}

void frmCausaDevolucion::set_id_ticket(int id)
{
    this->id = id;
}

void frmCausaDevolucion::on_btnAceptar_clicked()
{

    QMap <int,QSqlRecord> ltic;
    QMap <int,QSqlRecord> tic;
    bool trans = true;
    QString error,lerror;
    tic = SqlCalls::SelectRecord("cab_tpv",QString("id =%1").arg(this->id),Configuracion_global->empresaDB,error);
    ltic = SqlCalls::SelectRecord("lin_tpv",QString("id_cab=%1").arg(this->id),Configuracion_global->empresaDB,lerror);
    QMapIterator <int,QSqlRecord> i(tic);
    Configuracion_global->empresaDB.transaction();
    Configuracion_global->groupDB.transaction();

    while (i.hasNext())
    {

        // AGREGAR LÍNEA DEVOLUCIÓN
        i.next();
        QHash <QString, QVariant> dev;
        dev["ticket"] = this->id;
        dev["fecha_devolucion"] = ui->dateDevolucion->dateTime();
        dev["atendido_por"] = ui->cmbUsuario->currentText();
        dev["id_cliente"]= tic.value(this->id).value("id_cliente").toInt();
        dev["cliente"]= tic.value(this->id).value("nombre_cliente").toString();
        dev["comentarios"] =ui->txtMotivo->toPlainText();
        dev["codigo_articulo"] = i.value().value("codigo").toString();
        dev["descripcion"] = i.value().value("descripcion").toString();
        dev["id_articulo"] = i.value().value("id_articulo").toInt();
        dev["ejercicio"] = tic.value(this->id).value("ejercicio").toInt() ;
        this->id_art = i.value().value("id_articulo").toInt();

        int new_id = SqlCalls::SqlInsert(dev,"devoluciones",Configuracion_global->empresaDB,error);
        if(new_id == -1){
            trans = false;
            QMessageBox::warning(this,tr("Gestiones TPV"),tr("No se pudo realizar la devolución"),tr("Aceptar"));
        }

    }

    if(trans)
    {
        Configuracion_global->groupDB.commit();
        Configuracion_global->empresaDB.commit();
        TimedMessageBox::Box(this,tr("Se ha creado una nota de devolución de producto"));
    }
    else
    {
        Configuracion_global->groupDB.rollback();
        Configuracion_global->empresaDB.rollback();

    }
    accept();

}
