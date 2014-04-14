#include "editimagedlg.h"
#include "ui_editimagedlg.h"
#include <QFileDialog>
EditImageDlg::EditImageDlg(ReportImage* img ,QWidget *parent) :
    EditDinamycItemDlg(parent),
    ui(new Ui::EditImageDlg)
{
    ui->setupUi(this);
    image = img;

    pre = new ReportImage(0);
    ui->graphicsView->setScene(&scene);
    scene.addItem(pre);

    ui->comboZona->addItems(zonas());
    ui->chkArchivo->setChecked(!img->fromDB());

    if(img->fromDB())
    {
        QStringList l = img->ruta().split(".");
        if(l.size()==3)
        {
            ui->comboZona->setCurrentText(l.at(0));
            ui->comboTable->setCurrentText(l.at(1));
            ui->comboCampo->setCurrentText(l.at(2));
        }
        ui->txtRuta->setEnabled(false);
        ui->btnRuta->setEnabled(false);
    }
    else
    {
        ui->txtRuta->setText(img->ruta());
    }

    pre->setSize(img->rect().width(),img->rect().width());
    pre->setruta(img->ruta());
    ui->txtNombre->setText(img->name());

    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

EditImageDlg::~EditImageDlg()
{
    delete ui;
}

void EditImageDlg::on_btnRuta_clicked()
{
    QString ruta = QFileDialog::getOpenFileName(this,"Imagen",QString(),"Images (*.png *.xpm *.jpg)");
    if(!ruta.isEmpty())
    {
        ui->txtRuta->setText(ruta);
        pre->setruta(ruta);
        scene.update();
    }

}

void EditImageDlg::on_btnAceptar_clicked()
{
    image->setName(ui->txtNombre->text());
    image->setfromDB(pre->fromDB());
    image->setruta(pre->ruta());    
    this->accept();
}

void EditImageDlg::on_chkArchivo_toggled(bool checked)
{
    ui->groupBox_2->setEnabled(!checked);
    ui->txtRuta->setEnabled(checked);
    ui->btnRuta->setEnabled(checked);
    pre->setfromDB(!checked);

    if(checked)
    {
        pre->setruta(ui->txtRuta->text());
    }
    else
    {
        QString s = QString("%1.%2.%3").arg(ui->comboZona->currentText())
                                       .arg(ui->comboTable->currentText())
                                       .arg(ui->comboCampo->currentText());
        pre->setruta(s);
    }
    scene.update();
}
void EditImageDlg::on_comboZona_currentIndexChanged(const QString &arg1)
{
    ui->comboTable->clear();
    ui->comboCampo->clear();
    ui->comboTable->addItems(tablas(arg1));
}

void EditImageDlg::on_comboTable_currentIndexChanged(const QString &arg1)
{
    ui->comboCampo->clear();
    ui->comboCampo->addItems(campos(arg1));
}
void EditImageDlg::on_comboCampo_currentIndexChanged(const QString &arg1)
{
    QString s = QString("%1.%2.%3").arg(ui->comboZona->currentText())
                                   .arg(ui->comboTable->currentText())
                                   .arg(arg1);
    pre->setruta(s);
    scene.update();
}

