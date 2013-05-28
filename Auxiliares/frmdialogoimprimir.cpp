#include "frmdialogoimprimir.h"
#include "ui_frmdialogoimprimir.h"

FrmDialogoImprimir::FrmDialogoImprimir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmDialogoImprimir)
{
    ui->setupUi(this);
    ui->rad_Impresora->setChecked(true);
    ui->frame_pdf->setVisible(false);
}

FrmDialogoImprimir::~FrmDialogoImprimir()
{
    delete ui;
}
