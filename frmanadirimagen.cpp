#include "frmanadirimagen.h"
#include "ui_frmanadirimagen.h"
#include <QDebug>
#include "imagenesdiagnostico.h"

ImagenesDiagnostico *oImagenesDiagnostico = new ImagenesDiagnostico();

FrmAnadirImagen::FrmAnadirImagen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmAnadirImagen)
{
    ui->setupUi(this);
    connect(ui->pushButton_Cancelar,SIGNAL(clicked()),this,SLOT(close()));
}

FrmAnadirImagen::~FrmAnadirImagen()
{
    delete ui;
}

void FrmAnadirImagen::RecuperarId(QString cID)
{
    qDebug() << cID;
}
