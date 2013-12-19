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
    ui->spinAltoSobre->setValue(paper->paperSize().height());
    ui->spinAnchoSobre->setValue(paper->paperSize().width());

    if(paper->tipoDoc() == Paper::_Report)
        ui->rdbReport->setChecked(true);
    else if(paper->tipoDoc() == Paper::_etiqueta)
        ui->rdbEtiqueta->setChecked(true);
    else
        ui->rdbSobre->setChecked(true);

    ui->spinAnchoEtq->setValue(paper->paperSize().width());
    ui->spinAltoEtq->setValue(paper->paperSize().height());
    ui->spinUp->setValue(paper->margenSuperior());
    ui->spinDown->setValue(paper->margenInferior());
    ui->spinLeft->setValue(paper->margenInferior());
    ui->spinRigth->setValue(paper->margenDerecho());

    ui->spinDistH->setValue(paper->lblDistH() );
    ui->spinDistV->setValue(paper->lblDistV() );
    ui->spinPorFila->setValue(paper->lblColumnCount() );
    ui->spinPorColum->setValue(paper->lblRowCount() );
    ui->chkRolloEtiq->setChecked(paper->lblPrinter() );

    ui->spinPaperH->setValue(paper->lblPaperH() );
    ui->spinPaperW->setValue(paper->lblPaperW() );
    ui->spinMlblUp->setValue(paper->lblPaperMarginS() );
    ui->spinMlblDwn->setValue(paper->lblPaperMarginInf() );
    ui->spinMlblI->setValue(paper->lblPaperMarginIzq() );
    ui->spinMlblD->setValue(paper->lblPaperMarginDer() );
}

EditPaperDlg::~EditPaperDlg()
{
    delete ui;
}

void EditPaperDlg::on_pushButton_2_clicked()
{
    if(ui->rdbReport->isChecked())
    {
        Paper::_Sizes s = static_cast<Paper::_Sizes>(ui->comboBox->currentIndex());        
        // paper->setOrientacion(ui->rdbV->isChecked() ? Paper::Retrato : Paper::Apaisado);
        paper->setmargenSuperior(ui->spinUp->value());
        paper->setmargenInferior(ui->spinDown->value());
        paper->setmargenIzquierdo(ui->spinLeft->value());
        paper->setmargenDerecho(ui->spinRigth->value());

        paper->setSize(s,0,0,ui->rdbV->isChecked() ? Paper::Retrato : Paper::Apaisado);
        paper->settipoDoc(Paper::_Report);
    }
    else if(ui->rdbEtiqueta->isChecked())
    {
        paper->setSize(Paper::Custom,
                       ui->spinAnchoEtq->value(),
                       ui->spinAltoEtq->value(),
                       Paper::Retrato);
        paper->setmargenSuperior (0);
        paper->setmargenInferior (0);
        paper->setmargenIzquierdo(0);
        paper->setmargenDerecho  (0);

        paper->setlblDistH(ui->spinDistH->value());
        paper->setlblDistV(ui->spinDistV->value());
        paper->setlblColumnCount(ui->spinPorFila->value());
        paper->setlblRowCount(ui->spinPorColum->value());
        paper->setlblPrinter(ui->chkRolloEtiq->isChecked());

        if(!ui->chkRolloEtiq->isChecked())
        {
            paper->setlblPaperH(ui->spinPaperH->value());
            paper->setlblPaperW(ui->spinPaperW->value());
            paper->setlblPaperMarginS(ui->spinMlblUp->value());
            paper->setlblPaperMarginInf(ui->spinMlblDwn->value());
        }

        paper->setlblPaperMarginIzq(ui->spinMlblI->value());
        paper->setlblPaperMarginDer(ui->spinMlblD->value());

        paper->settipoDoc(Paper::_etiqueta);
    }
    else if(ui->rdbSobre->isChecked())
    {
        paper->setmargenSuperior(ui->spinUp_2->value());
        paper->setmargenInferior(ui->spinDown_2->value());
        paper->setmargenIzquierdo(ui->spinLeft_2->value());
        paper->setmargenDerecho(ui->spinRigth_2->value());

        paper->setSize(Paper::Custom,ui->spinAnchoSobre->value(),ui->spinAltoSobre->value(),Paper::Retrato);

        paper->settipoDoc(Paper::_sobre);
    }
    paper->updatePaper();
    this->accept();
}

void EditPaperDlg::on_chkRolloEtiq_toggled(bool checked)
{
    ui->frameLamina->setEnabled(!checked);
    if(checked)
        ui->spinPorColum->setValue(1);
    ui->spinPorColum->setEnabled(!checked);
    ui->spinDistV->setEnabled(checked);
}

void EditPaperDlg::on_spinPorFila_valueChanged(int arg1)
{
    bool b = arg1 != 1;
    ui->spinDistH->setEnabled(b);
}

void EditPaperDlg::on_spinPorColum_valueChanged(int arg1)
{
    bool b = arg1 != 1;
    ui->spinDistV->setEnabled(b);
}

void EditPaperDlg::on_rdbReport_toggled(bool checked)
{
    if(checked)
        ui->stackedWidget->setCurrentIndex(0);
}

void EditPaperDlg::on_rdbEtiqueta_toggled(bool checked)
{
    if(checked)
        ui->stackedWidget->setCurrentIndex(1);
}

void EditPaperDlg::on_rdbSobre_toggled(bool checked)
{
    if(checked)
        ui->stackedWidget->setCurrentIndex(2);
}
