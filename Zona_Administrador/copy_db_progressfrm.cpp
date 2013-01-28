#include "copy_db_progressfrm.h"
#include "ui_copy_db_progressfrm.h"

copy_db_progressFrm::copy_db_progressFrm(QWidget *parent, int barras) :
    QDialog(parent),
    ui(new Ui::copy_db_progressFrm)
{
    ui->setupUi(this);
    if(barras == 2)
    {
        ui->lbl_3->hide();
        ui->progressBar_3->hide();
    }
    this->resize(width(),1);
}

void copy_db_progressFrm::setProgess_1(QString text, int progress)
{
    ui->lbl_1->setText(text);
    ui->progressBar_1->setValue(progress);
    QApplication::processEvents();
}

void copy_db_progressFrm::set_Max_1(int max)
{
    ui->progressBar_1->setMaximum(max);
}

void copy_db_progressFrm::setProgess_2(QString text, int progress)
{
    ui->lbl_2->setText(text);
    ui->progressBar_2->setValue(progress);
    QApplication::processEvents();
}

void copy_db_progressFrm::set_Max_2(int max)
{
    ui->progressBar_2->setMaximum(max);
}

void copy_db_progressFrm::setProgess_3(QString text, int progress)
{
    ui->lbl_3->setText(text);
    ui->progressBar_3->setValue(progress);
    QApplication::processEvents();
}

void copy_db_progressFrm::set_Max_3(int max)
{
    ui->progressBar_3->setMaximum(max);
}
copy_db_progressFrm::~copy_db_progressFrm()
{
    delete ui;
}
