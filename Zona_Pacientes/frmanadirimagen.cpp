#include "frmanadirimagen.h"
#include "ui_frmanadirimagen.h"

#include "imagenesdiagnostico.h"


ImagenesDiagnostico *oImagenesDiagnostico = new ImagenesDiagnostico(NULL);

FrmAnadirImagen::FrmAnadirImagen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAnadirImagen)
{
    ui->setupUi(this);
    connect(ui->pushButton_Cancelar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->pushButton_BuscarImagen,SIGNAL(clicked()),this,SLOT(AnadirImagen()));
    connect(oImagenesDiagnostico,SIGNAL(ui_ponerDatosEnObjetoImagen()),this,SLOT(GuardarDatosEnObjetoImagen()));
    connect(ui->pushButton__Anadir,SIGNAL(clicked()),this,SLOT(GuardarInfoImagen_en_BD()));
    connect(oImagenesDiagnostico,SIGNAL(cerrarventana()),this,SLOT(close()));
    connect(ui->comboBox_TipoImagen,SIGNAL(currentIndexChanged(QString)),this,SLOT(CambioTipoImagen(QString)));

    QSqlQueryModel *qTipos = new QSqlQueryModel(this);
    qTipos->setQuery("Select descripcion from tiposimagen",QSqlDatabase::database("dbmedica"));
    ui->comboBox_TipoImagen->setModel(qTipos);
    oImagenesDiagnostico->idTipoImagen = oImagenesDiagnostico->DevolverId_tipo_imagen(ui->comboBox_TipoImagen->currentText());
}

FrmAnadirImagen::~FrmAnadirImagen()
{
    delete ui;
}

void FrmAnadirImagen::RecuperarId(int cIDEpisodio)
{
    oImagenesDiagnostico->id= cIDEpisodio;
}

void FrmAnadirImagen::AnadirImagen()
{


    QString ficheroImagen;
    ficheroImagen = QFileDialog::getOpenFileName(this,tr("Abrir fichero de imagen"),"","Imagenes (*.bmp *.png *.xpm *.jpg)");
    if (!ficheroImagen.isEmpty()) {

        QImage imagen(ficheroImagen);

        ui->label_Imagen->setPixmap(QPixmap::fromImage(imagen));
        ui->label_Imagen->setScaledContents(true);
        QByteArray ba;
        QFile f(ficheroImagen);
        if(f.open(QIODevice::ReadOnly)) {
            ba = f.readAll();
            f.close();
        }
        QSqlQuery queryImagenes(QSqlDatabase::database("dbmedica"));
        queryImagenes.prepare("update imagenes set imagen =:imagen where Id = :nid");
        queryImagenes.bindValue(":imagen",ba);
        queryImagenes.bindValue(":nid",oImagenesDiagnostico->id);
        if (!queryImagenes.exec())
            QMessageBox::warning(qApp->activeWindow(),tr("Guardar Imagen"),
                                 tr("No se ha podido guardar la imagen en la base de datos"),tr("Ok"));



    }

}

void FrmAnadirImagen::GuardarInfoImagen_en_BD()
{
    GuardarDatosEnObjetoImagen();
    oImagenesDiagnostico->guardarDatosDB();
    this->accept();
}


void FrmAnadirImagen::GuardarDatosEnObjetoImagen()
{
    oImagenesDiagnostico->Comentarios = ui->textEdit_ComentariosImagen->toPlainText();
    oImagenesDiagnostico->Descripcion =ui->txtDescripcion->text();
    if (ui->checkBox_ImagenEvaluada->isChecked())
        oImagenesDiagnostico->Evaluada = true;
   else
        oImagenesDiagnostico->Evaluada = false;
    oImagenesDiagnostico->FechaImagen =ui->dateEdit_FechaImagen->date();
    oImagenesDiagnostico->idTipoImagen = oImagenesDiagnostico->DevolverId_tipo_imagen(ui->comboBox_TipoImagen->currentText());

}

void FrmAnadirImagen::CambioTipoImagen(QString texto)
{
    int id = oImagenesDiagnostico->DevolverId_tipo_imagen(texto);
    oImagenesDiagnostico->idTipoImagen = id;
}
