#include "editlinedlg.h"
#include "ui_editlinedlg.h"
#include <QColorDialog>
EditLineDlg::EditLineDlg(ReportLine* li ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditLineDlg)
{
    ui->setupUi(this);
    line = li;
    ui->graphicsView->setScene(&scene);
    preLine = new ReportLine(0);
    preLine->setSize(300,300);

    connect(ui->radioButton_3,SIGNAL(toggled(bool)),this,SLOT(styleChanged(bool)));
    connect(ui->radioButton_4,SIGNAL(toggled(bool)),this,SLOT(styleChanged(bool)));
    connect(ui->radioButton_5,SIGNAL(toggled(bool)),this,SLOT(styleChanged(bool)));
    connect(ui->radioButton_6,SIGNAL(toggled(bool)),this,SLOT(styleChanged(bool)));
    connect(ui->radioButton_7,SIGNAL(toggled(bool)),this,SLOT(styleChanged(bool)));

    connect(ui->rdbH,SIGNAL(toggled(bool)),this,SLOT(orientacion(bool)));
    connect(ui->rdbV,SIGNAL(toggled(bool)),this,SLOT(orientacion(bool)));

    switch (li->penStyle())
    {
    case Qt::SolidLine:
        ui->radioButton_3->setChecked(true);
        break;
    case Qt::DashLine:
        ui->radioButton_4->setChecked(true);
        break;
    case Qt::DotLine:
        ui->radioButton_5->setChecked(true);
        break;
    case Qt::DashDotLine:
        ui->radioButton_6->setChecked(true);
        break;
    case Qt::DashDotDotLine:
        ui->radioButton_7->setChecked(true);
        break;
    }
    if(li->Orientacion() == Container::Horizontal)
        ui->rdbH->setChecked(true);
    else
        ui->rdbV->setChecked(true);

    ui->spinBox->setValue(li->penWidth());

    preLine->setpenColor(li->penColor());

    scene.addItem(preLine);
    connect(ui->btnCancelar,SIGNAL(clicked()),this,SLOT(reject()));
}

EditLineDlg::~EditLineDlg()
{
    delete ui;
}

void EditLineDlg::styleChanged(bool b)
{
    if(b)
    {
        QObject * s = sender();
        if (s == ui->radioButton_3)
            preLine->setpenStyle(Qt::SolidLine);
        else if(s == ui->radioButton_4)
            preLine->setpenStyle(Qt::DashLine);
        else if(s == ui->radioButton_5)
            preLine->setpenStyle(Qt::DotLine);
        else if(s == ui->radioButton_6)
            preLine->setpenStyle(Qt::DashDotLine);
        else if(s == ui->radioButton_7)
            preLine->setpenStyle(Qt::DashDotDotLine);
        scene.update();
    }
}

void EditLineDlg::orientacion(bool b)
{
    if(b)
    {
        QObject * s = sender();
        if (s == ui->rdbH)
            preLine->setOrientacion(Container::Horizontal);
        else
            preLine->setOrientacion(Container::Vertical);
        scene.update();
    }
}

void EditLineDlg::on_spinBox_valueChanged(int arg1)
{
    preLine->setpenWidth(arg1);
    scene.update();
}

void EditLineDlg::on_pushButton_3_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        preLine->setpenColor(d.selectedColor());
        scene.update();
    }
}

void EditLineDlg::on_btnAceptar_clicked()
{
    line->setpenWidth(preLine->penWidth());
    line->setpenColor(preLine->penColor());
    line->setpenStyle(preLine->penStyle());
    line->setOrientacion(preLine->Orientacion());
    this->accept();
}
