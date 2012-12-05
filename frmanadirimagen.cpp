#include "frmanadirimagen.h"
#include "ui_frmanadirimagen.h"
#include <QDebug>
#include <QFileDialog>
#include "imagenesdiagnostico.h"

ImagenesDiagnostico *oImagenesDiagnostico = new ImagenesDiagnostico(NULL);

FrmAnadirImagen::FrmAnadirImagen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmAnadirImagen)
{
    ui->setupUi(this);
    connect(ui->pushButton_Cancelar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->pushButton_BuscarImagen,SIGNAL(clicked()),this,SLOT(AnadirImagen()));
    connect(oImagenesDiagnostico,SIGNAL(ui_ponerDatosEnObjetoImagen()),this,SLOT(GuardarDatosEnObjetoImagen()));
    connect(ui->pushButton__Anadir,SIGNAL(clicked()),oImagenesDiagnostico,SLOT(guardarDatosDB()));
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
    ficheroImagen = QFileDialog::getOpenFileName(this,tr("Abrir fichero de imagen"),"","Imagenes (*.bmp *.png *.xpm *.jpg)");
    if (!ficheroImagen.isEmpty()) {
        QImage imagen(ficheroImagen);
        ui->label_Imagen->setPixmap(QPixmap::fromImage(imagen));
        oImagenesDiagnostico->setLocalizacionImagen(ficheroImagen);

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
