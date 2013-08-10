#include "editpaperdlg.h"
#include "ui_editpaperdlg.h"

EditPaperDlg::EditPaperDlg(Paper *paper, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditPaperDlg)
{
    ui->setupUi(this);
    this->paper = paper;
    connect(ui->btnCancelar,SIGNAL(clicked()),this,SLOT(reject()));
    QStringList items;
    items <<"A0"<<"A1"<<"A2"<<"A3"<<"A4"<<"A5"<<"A6"<<"A7"<<"A8"<<"A9"<<
            "B0"<<"B1"<<"B2"<<"B3"<<"B4"<<"B5"<<"B6"<<"B7"<<"B8"<<"B9"<<"B10"<<
            "C5E"<<"Comm10E"<<"DLE"<<"Executive"<<"Folio"<<"Ledger"<<"Legal"<<"Letter"<<"Tabloid";
    ui->comboBox->addItems(items);

    if(paper->StandartSize()!= Paper::Custom)
    {
        int siz = static_cast<int>(paper->StandartSize());
        ui->comboBox->setCurrentIndex(siz);
    }
    ui->spinUp->setValue(paper->margenSuperior());
    ui->spinDown->setValue(paper->margenInferior());
    ui->spinLeft->setValue(paper->margenInferior());
    ui->spinRigth->setValue(paper->margenDerecho());
}

EditPaperDlg::~EditPaperDlg()
{
    delete ui;
}

void EditPaperDlg::on_pushButton_2_clicked()
{
    Paper::_Sizes s = static_cast<Paper::_Sizes>(ui->comboBox->currentIndex());
    paper->setSize(s,0,0,ui->rdbV->isChecked() ? Paper::Retrato : Paper::Apaisado);
   // paper->setOrientacion(ui->rdbV->isChecked() ? Paper::Retrato : Paper::Apaisado);
    paper->setmargenSuperior(ui->spinUp->value());
    paper->setmargenInferior(ui->spinDown->value());
    paper->setmargenIzquierdo(ui->spinLeft->value());
    paper->setmargenDerecho(ui->spinRigth->value());
    paper->updatePaper();
    this->accept();
}
