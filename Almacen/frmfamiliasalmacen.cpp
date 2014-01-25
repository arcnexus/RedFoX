#include "frmfamiliasalmacen.h"
#include "ui_frmfamiliasalmacen.h"

frmFamiliasAlmacen::frmFamiliasAlmacen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmFamiliasAlmacen)
{
    ui->setupUi(this);
    model_seccion = new QSqlQueryModel(this);
    model_seccion->setQuery("select id,codigo,seccion from secciones order by seccion", Configuracion_global->groupDB);
    ui->tabla_secciones->setModel(model_seccion);
    QStringList headers;
    headers << "id" << tr("código") << tr("Sección");
    QVariantList sizes;
    sizes << 0 << 120 <<200;
    for(int i;i < headers.size();i++)
    {
        ui->tabla_secciones->setColumnWidth(i,sizes.at(i).toInt());
        model_seccion->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }

    model_familias = new QSqlQueryModel(this);
    model_familias->setQuery("select id, codigo, familia from familias where id = 0 order by familia",
                              Configuracion_global->groupDB);
    ui->tabla_familias->setModel(model_familias);
    headers.clear();
    headers << "id" << tr("código") << tr("Familia");
    sizes.clear();
    sizes << 0 << 120 <<200;
    for(int i;i < headers.size();i++)
    {
        ui->tabla_familias->setColumnWidth(i,sizes.at(i).toInt());
        model_familias->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
    model_subfamilias = new QSqlQueryModel(this);
    model_subfamilias->setQuery("select codigo,sub_familia from subfamilias where id_familia = 0 order by subfamilia",
                                Configuracion_global->groupDB);
    ui->tabla_subfamilias->setModel(model_subfamilias);
    headers.clear();
    headers << tr("código") << tr("Subfamilia");
    sizes.clear();
    sizes  << 120 <<200;
    for(int i;i < headers.size();i++)
    {
        ui->tabla_subfamilias->setColumnWidth(i,sizes.at(i).toInt());
        model_subfamilias->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }

}

frmFamiliasAlmacen::~frmFamiliasAlmacen()
{
    delete ui;
}

void frmFamiliasAlmacen::on_btnAnadir_clicked()
{
    this->id_familia = 0;

    ui->txtCodigo->clear();
    ui->txtFamilia->clear();
    ui->imagen->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
    ui->txtCodigo->setFocus();
    setStatus(true);

}

void frmFamiliasAlmacen::on_btnEditar_clicked()
{
    setStatus(true);
    ui->txtCodigo->setFocus();


}

void frmFamiliasAlmacen::cargarImagen(int id_sec)
{
    ui->imagen->setPixmap(QPixmap(":/Icons/PNG/paquete.png"));
    QSqlQuery queryFamilia(Configuracion_global->groupDB);
    queryFamilia.prepare("Select image from familias where id = :id");
    queryFamilia.bindValue(":id",id_sec);
    if (queryFamilia.exec()) {
        if (queryFamilia.next()){
            //--------
            // imagen1
            //--------
            QSqlRecord registro =  queryFamilia.record();
            QByteArray ba1 = registro.field("image").value().toByteArray();
            ba1 = registro.field("image").value().toByteArray();
            QPixmap pm11;
            pm11.loadFromData(ba1);
            if(!registro.field("image").value().isNull())
                ui->imagen->setPixmap(pm11);
        }
    }
}


void frmFamiliasAlmacen::setStatus(bool success)
{
    ui->txtCodigo->setEnabled(success);
    ui->txtFamilia->setEnabled(success);
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


void frmFamiliasAlmacen::on_tabla_secciones_clicked(const QModelIndex &index)
{
    this->id_seccion= ui->tabla_secciones->model()->data(ui->tabla_secciones->model()->index(index.row(),0)).toInt();
    model_familias->setQuery(QString("select id,codigo,familia from familias where id_seccion = %1").arg(this->id_seccion),
                             Configuracion_global->groupDB);

}

void frmFamiliasAlmacen::on_btnCambiarImagen_clicked()
{
    if(this->id_familia == 0)
    {
        // Insertar
        QHash <QString,QVariant> h;
        QString error;
        h["codigo"] = ui->txtCodigo->text();
        h["familia"] = ui->txtFamilia->text();

        this->id_familia = SqlCalls::SqlInsert(h,"familias",Configuracion_global->groupDB,error);
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
        QSqlQuery queryFamilia(Configuracion_global->groupDB);

        queryFamilia.prepare("update familias set image =:imagen where id = :nid");
        queryFamilia.bindValue(":imagen",ba);
        queryFamilia.bindValue(":nid",this->id_familia);
        if (!queryFamilia.exec())
        {
            QMessageBox::warning(qApp->activeWindow(),tr("Guardar Imagen"),tr("No se ha podido guardar la imagen en la base de datos"),tr("Ok"));
            qDebug() << queryFamilia.lastError();
        }

    }
}

void frmFamiliasAlmacen::on_btnGuardar_clicked()
{
    QHash <QString,QVariant> h;
    QString error;
    h["codigo"] = ui->txtCodigo->text();
    h["id_seccion"] =this->id_seccion;
    h["familia"] = ui->txtFamilia->text();
    if(this->id_familia ==0)
    {
        // Insertar


        this->id_familia = SqlCalls::SqlInsert(h,"familias",Configuracion_global->groupDB,error);
        if(this->id_familia = -1)
        {
            this->id_familia = 0;
            QMessageBox::warning(this,tr("familias"),tr("No se pudo añadir la familia: %1").arg(error));
        }

    } else
    {
        // Guardar
        bool success = SqlCalls::SqlUpdate(h,"familias",Configuracion_global->groupDB,QString("id=%1").arg(this->id_familia),error);
        if(!success)
            QMessageBox::warning(this,tr("Familias"),tr("No se pudo guardar la Familia: %1").arg(error));
    }
    setStatus(false);
    model_seccion->setQuery("select id, codigo, seccion from secciones order by seccion",
                            Configuracion_global->groupDB);
    model_familias->setQuery(QString("select id,codigo,familia from familias where id_seccion = %1").arg(this->id_seccion),
                             Configuracion_global->groupDB);
}

void frmFamiliasAlmacen::on_tabla_familias_clicked(const QModelIndex &index)
{
    this->id_familia= ui->tabla_familias->model()->data(ui->tabla_familias->model()->index(index.row(),0)).toInt();
    if(this->id_familia>0)
    {
        QMap <int,QSqlRecord> sec;
        QString error;
        sec = SqlCalls::SelectRecord("familias",QString("id=%1").arg(this->id_familia),Configuracion_global->groupDB,error);
        ui->txtCodigo->setText(sec.value(this->id_familia).value("codigo").toString());
        ui->txtFamilia->setText(sec.value(this->id_familia).value("familia").toString());
        cargarImagen(this->id_familia);
        model_subfamilias->setQuery(QString("select codigo,sub_familia from subfamilias where id_familia = %1").arg(this->id_familia),
                                    Configuracion_global->groupDB);
    }
}
