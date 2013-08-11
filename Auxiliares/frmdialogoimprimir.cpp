#include "frmdialogoimprimir.h"
#include "ui_frmdialogoimprimir.h"

FrmDialogoImprimir::FrmDialogoImprimir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmDialogoImprimir)
{
    ui->setupUi(this);
    ui->rad_Impresora->setChecked(true);
    ui->frame_email->setVisible(false);
    //ui->frame_pdf->setVisible(false);
}

FrmDialogoImprimir::~FrmDialogoImprimir()
{
    delete ui;
}

void FrmDialogoImprimir::set_email(QString email)
{
    ui->txtEmail->setText(email);
}

void FrmDialogoImprimir::set_ruta(QString ruta)
{
    //ui->txtRutaPDF->setText(ruta);
}

void FrmDialogoImprimir::set_preview(bool preview)
{
    ui->rad_preview->setChecked(preview);
}

QString FrmDialogoImprimir::get_ruta()
{
    //return ui->txtRutaPDF->text();
}

QString FrmDialogoImprimir::get_email()
{
    return ui->txtEmail->text();
}

int FrmDialogoImprimir::get_option()
{
    if (ui->rad_Impresora->isChecked())
        return 1;
    else if(ui->radEmail->isChecked())
        return 2;
    else if(ui->rad_PDF->isChecked())
        return 3;
    else if(ui->rad_preview->isChecked())
        return 4;

}

bool FrmDialogoImprimir::to_pdf()
{
    return ui->rad_PDF->isChecked();
}



