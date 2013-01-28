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
    connect(ui->pushButton__Anadir,SIGNAL(clicked()),oImagenesDiagnostico,SLOT(anadirDatosDB()));
    connect(oImagenesDiagnostico,SIGNAL(cerrarventana()),this,SLOT(close()));

    QSqlQueryModel *qTipos = new QSqlQueryModel(this);
    qTipos->setQuery("Select descripcion from tiposimagen",QSqlDatabase::database("dbmedica"));
    ui->comboBox_TipoImagen->setModel(qTipos);
}

FrmAnadirImagen::~FrmAnadirImagen()
{
    delete ui;
}

void FrmAnadirImagen::RecuperarId(int cIDEpisodio)
{
    oImagenesDiagnostico->setIdEpisodio(cIDEpisodio);
}

void FrmAnadirImagen::AnadirImagen()
{
    QString ficheroImagen;
    ficheroImagen = QFileDialog::getOpenFileName(this,tr("Abrir fichero de imagen"),"",tr("Imágenes (*.bmp *.png *.xpm *.jpg)"));
    if (!ficheroImagen.isEmpty()) {
        QImage imagen(ficheroImagen);
        oImagenesDiagnostico->setLocalizacionImagen(ficheroImagen);
        QPixmap qpmImagen = QPixmap::fromImage(imagen);
        ui->label_Imagen->setPixmap(qpmImagen.scaled(ui->label_Imagen->size(),Qt::KeepAspectRatio));

    }
}

void FrmAnadirImagen::GuardarDatosEnObjetoImagen()
{
    oImagenesDiagnostico->setComentarios(ui->textEdit_ComentariosImagen->toPlainText());
    oImagenesDiagnostico->setDescripcion(ui->txtDescripcion->text());
    if (ui->checkBox_ImagenEvaluada->isChecked())
        oImagenesDiagnostico->setEvaluada(true);
   else
        oImagenesDiagnostico->setEvaluada(true);
    oImagenesDiagnostico->setFechaImagen(ui->dateEdit_FechaImagen->date());
    oImagenesDiagnostico->setTipoImagen(ui->comboBox_TipoImagen->currentText());

}
