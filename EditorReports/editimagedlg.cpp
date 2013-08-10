#include "editimagedlg.h"
#include "ui_editimagedlg.h"
#include <QFileDialog>
EditImageDlg::EditImageDlg(ReportImage* img ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditImageDlg)
{
    ui->setupUi(this);
    image = img;

    pre = new ReportImage(0);
    ui->graphicsView->setScene(&scene);
    scene.addItem(pre);

    pre->setSize(img->rect().width(),img->rect().width());
    pre->setruta(img->ruta());

    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

EditImageDlg::~EditImageDlg()
{
    delete ui;
}

void EditImageDlg::on_btnRuta_clicked()
{
    QString ruta = QFileDialog::getOpenFileName(this,"Imagen",QString(),"Imagenes (*.png *.jpg);");
    if(!ruta.isEmpty())
    {
        ui->txtRuta->setText(ruta);
        pre->setruta(ruta);
        scene.update();
    }

}

void EditImageDlg::on_btnAceptar_clicked()
{
    image->setruta(pre->ruta());
    this->accept();
}
