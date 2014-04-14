#include "frmseccionesalmacen.h"
#include "ui_frmseccionesalmacen.h"

frmSeccionesAlmacen::frmSeccionesAlmacen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmSeccionesAlmacen)
{
    ui->setupUi(this);
    model_secciones = new QSqlQueryModel(this);
    model_secciones->setQuery("select id, codigo, seccion from secciones order by seccion",
                              Configuracion_global->groupDB);
    ui->tabla_secciones->setModel(model_secciones);
    QStringList headers;
    headers << "id" << tr("código") << tr("Sección");
    QVariantList sizes;
    sizes << 0 << 120 <<200;
    for(int i;i < headers.size();i++)
    {
        ui->tabla_secciones->setColumnWidth(i,sizes.at(i).toInt());
        model_secciones->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
    model_familias = new QSqlQueryModel(this);
    model_familias->setQuery("select codigo,familia from familias where id_seccion = 0",
                             Configuracion_global->groupDB);
    ui->tabla_familias->setModel(model_familias);
    headers << tr("código") << tr("Sección");
    sizes  << 120 <<200;
    for(int i;i < headers.size();i++)
    {
        ui->tabla_familias->setColumnWidth(i,sizes.at(i).toInt());
        model_familias->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }

}

frmSeccionesAlmacen::~frmSeccionesAlmacen()
{
    delete ui;
}

void frmSeccionesAlmacen::on_btnAnadir_clicked()
{
    this->id = 0;

    ui->txtCodigo->clear();
    ui->txtSeccion->clear();
    ui->imagen->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
    ui->txtCodigo->setFocus();
    setStatus(true);

}

void frmSeccionesAlmacen::on_btnEditar_clicked()
{
    setStatus(true);
    ui->txtCodigo->setFocus();


}

void frmSeccionesAlmacen::cargarImagen(int id_sec)
{
    ui->imagen->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
    QSqlQuery querySeccion(Configuracion_global->groupDB);
    querySeccion.prepare("Select image from secciones where id = :id");
    querySeccion.bindValue(":id",id_sec);
    if (querySeccion.exec()) {
        if (querySeccion.next()){
            //--------
            // imagen1
            //--------
            QSqlRecord registro =  querySeccion.record();
            QByteArray ba1 = registro.field("image").value().toByteArray();
            ba1 = registro.field("image").value().toByteArray();
            QPixmap pm11;
            pm11.loadFromData(ba1);
            if(!registro.field("image").value().isNull())
                ui->imagen->setPixmap(pm11);
        }
    }
}


void frmSeccionesAlmacen::setStatus(bool success)
{
    ui->txtCodigo->setEnabled(success);
    ui->txtSeccion->setEnabled(success);
    ui->btnCambiarImagen->setEnabled(success);
    ui->btnEliminarImagen->setEnabled(success);
    ui->btnAnadir->setEnabled(!success);
    ui->btnEditar->setEnabled(!success);
    ui->btnSalir->setEnabled(!success);
    ui->btnGuardar->setEnabled(success);
    ui->btnDeshacer->setEnabled(success);
    ui->tabla_secciones->setEnabled(!success);
    ui->tabla_familias->setEnabled(!success);

}


void frmSeccionesAlmacen::on_tabla_secciones_clicked(const QModelIndex &index)
{
    int ids =  model_secciones->record(index.row()).value("id").toInt();
    this->id= ui->tabla_secciones->model()->data(ui->tabla_secciones->model()->index(index.row(),0)).toInt();
    if(this->id>0)
    {
        QMap <int,QSqlRecord> sec;
        QString error;
        sec = SqlCalls::SelectRecord("secciones",QString("id=%1").arg(this->id),Configuracion_global->groupDB,error);
        ui->txtCodigo->setText(sec.value(this->id).value("codigo").toString());
        ui->txtSeccion->setText(sec.value(this->id).value("seccion").toString());
        cargarImagen(this->id);
        model_familias->setQuery(QString("select codigo,familia from familias where id_seccion = %1").arg(this->id),
                                 Configuracion_global->groupDB);
    }
}

void frmSeccionesAlmacen::on_btnCambiarImagen_clicked()
{
    if(this->id == 0)
    {
        // Insertar
        QHash <QString,QVariant> h;
        QString error;
        h["codigo"] = ui->txtCodigo->text();
        h["seccion"] = ui->txtSeccion->text();

        this->id = SqlCalls::SqlInsert(h,"secciones",Configuracion_global->groupDB,error);
    }
    QString ficheroImagen;
    ficheroImagen = QFileDialog::getOpenFileName(this,tr("Abrir fichero de imagen"),"","Imagenes (*.bmp *.png *.xpm *.jpg)");
    if (!ficheroImagen.isEmpty()) {

        QImage imagen(ficheroImagen);

        ui->imagen->setPixmap(QPixmap::fromImage(imagen));
        ui->imagen->setScaledContents(true);
        QByteArray ba;
        QFile f(ficheroImagen);
        if(f.open(QIODevice::ReadOnly)) {
            ba = f.readAll();
            f.close();
        }
        QSqlQuery querySeccion(Configuracion_global->groupDB);

        querySeccion.prepare("update secciones set image =:imagen where id = :nid");
        querySeccion.bindValue(":imagen",ba);
        querySeccion.bindValue(":nid",this->id);
        if (!querySeccion.exec())
        {
            QMessageBox::warning(qApp->activeWindow(),tr("Guardar Imagen"),tr("No se ha podido guardar la imagen en la base de datos"),tr("Ok"));
            qDebug() << querySeccion.lastError();
        }

    }
}

void frmSeccionesAlmacen::on_btnGuardar_clicked()
{
    QHash <QString,QVariant> h;
    QString error;
    h["codigo"] = ui->txtCodigo->text();
    h["seccion"] = ui->txtSeccion->text();
    if(this->id ==0)
    {
        // Insertar


        this->id = SqlCalls::SqlInsert(h,"secciones",Configuracion_global->groupDB,error);
        if(this->id == -1)
        {
            this->id = 0;
            QMessageBox::warning(this,tr("Secciones"),tr("No se pudo añadir la sección: %1").arg(error));
        }

    } else
    {
        // Guardar
        bool success = SqlCalls::SqlUpdate(h,"secciones",Configuracion_global->groupDB,QString("id=%1").arg(this->id),error);
        if(!success)
            QMessageBox::warning(this,tr("Secciones"),tr("No se pudo guardar la sección: %1").arg(error));
    }
    setStatus(false);
    model_secciones->setQuery("select id, codigo, seccion from secciones order by seccion",
                              Configuracion_global->groupDB);
}
