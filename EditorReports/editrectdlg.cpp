#include "editrectdlg.h"
#include "ui_editrectdlg.h"
#include <QColorDialog>
EditRectDlg::EditRectDlg(RoundedRect *r, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditRectDlg)
{
    ui->setupUi(this);
    rect = r;

    pre = new RoundedRect(0);
    pre->setSize(200,200);
    scene.addItem(pre);
    ui->graphicsView->setScene(&scene);

    connect(ui->rdbSolid,SIGNAL(toggled(bool)),this,SLOT(fillPattern(bool)));
    connect(ui->rdbDeg,SIGNAL(toggled(bool)),this,SLOT(fillPattern(bool)));

    connect(ui->rdbH,SIGNAL(toggled(bool)),this,SLOT(patterDirection(bool)));
    connect(ui->rdbV,SIGNAL(toggled(bool)),this,SLOT(patterDirection(bool)));

    connect(ui->btnCancelar,SIGNAL(clicked()),this,SLOT(reject()));

    pre->setcolor1(r->color1());
    pre->setcolor2(r->color2());
    pre->setpenColor(r->penColor());

    ui->rdbSolid->setChecked(!r->useGradient());
    ui->rdbDeg->setChecked(r->useGradient());

    ui->rdbV->setChecked(r->GradientDirection() == Container::Vertical);
    ui->rdbH->setChecked(r->GradientDirection() == Container::Horizontal);

    ui->spinGrueso->setValue(r->penWidth());
    ui->spinRadioX->setValue(r->RadiousX());
    ui->spinRadioY->setValue(r->RadiousY());

    pre->setRadiousX(r->RadiousX());
    pre->setRadiousY(r->RadiousY());
}

EditRectDlg::~EditRectDlg()
{
    delete ui;
}

void EditRectDlg::fillPattern(bool b)
{
    if(b)
    {
        if(sender() == ui->rdbSolid)
        {
            pre->setuseGradient(false);
            ui->rdbH->setEnabled(false);
            ui->rdbV->setEnabled(false);
            ui->btnColor2->setEnabled(false);
        }
        else
        {
            pre->setuseGradient(true);
            ui->rdbH->setEnabled(true);
            ui->rdbV->setEnabled(true);
            ui->btnColor2->setEnabled(true);
        }
        scene.update();
    }
}

void EditRectDlg::patterDirection(bool b)
{
    if(b)
    {
        if(sender() == ui->rdbV)
            pre->setGradientDirection(Container::Vertical);
        else
            pre->setGradientDirection(Container::Horizontal);
        scene.update();
    }
}

void EditRectDlg::on_spinGrueso_valueChanged(int arg1)
{
    pre->setpenWidth(arg1);
    scene.update();
}

void EditRectDlg::on_spinRadioX_valueChanged(double arg1)
{
    pre->setRadiousX(arg1);
    scene.update();
}

void EditRectDlg::on_spinRadioY_valueChanged(double arg1)
{
     pre->setRadiousY(arg1);
     scene.update();
}

void EditRectDlg::on_btnColor1_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        pre->setcolor1(d.selectedColor());
        scene.update();
    }
}

void EditRectDlg::on_btnColor2_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        pre->setcolor2(d.selectedColor());
        scene.update();
    }
}

void EditRectDlg::on_pushButton_3_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        pre->setpenColor(d.selectedColor());
        scene.update();
    }
}

void EditRectDlg::on_btnAceptar_clicked()
{
    rect->setpenWidth(pre->penWidth());
    rect->setpenColor(pre->penColor());
    rect->setRadiousX(pre->RadiousX());
    rect->setRadiousY(pre->RadiousY());
    rect->setuseGradient(pre->useGradient());
    rect->setGradientDirection(pre->GradientDirection());
    rect->setcolor1(pre->color1());
    rect->setcolor2(pre->color2());
    this->accept();
}
