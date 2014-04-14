#include "editacum.h"
#include "ui_editacum.h"
#include <QColorDialog>
EditAcum::EditAcum(ReportAcumulator *rep, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAcum)
{  
    out = rep;
    pre = new ReportAcumulator(0);
    ui->setupUi(this);

    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));

    ui->graphicsView->setScene(&scene);

    ui->btnBold->setChecked(rep->fontWeigth()==QFont::Bold);
    ui->btnItalic->setChecked(rep->italicFont());
    ui->btnUnder->setChecked(rep->underlined());
    ui->fontComboBox->setCurrentText(rep->fontFamily());
    ui->sizeSpin->setValue(rep->fontSize());

    if(rep->formato() > 0)
    {
        ui->checkBox_2->setChecked(true);
        ui->comobFormato->setCurrentIndex(rep->formato()-1);
    }

    pre->setfontWeigth(rep->fontWeigth());
    pre->setFont(rep->Font());
    pre->setitalicFont(rep->italicFont());
    pre->setunderlined(rep->underlined());
    pre->setfontColor(rep->fontColor());
    pre->setfontFamily(rep->fontFamily());
    pre->setfontSize(rep->fontSize());
    pre->setAlineacion(rep->Alineacion());
    pre->setTarget(rep->target());

    ui->comboBox->addItems(Container::acums());
    ui->txtNombre->setText(rep->name());
    scene.addItem(pre);
}

EditAcum::~EditAcum()
{
    delete ui;
}

void EditAcum::on_btnAceptar_clicked()
{
    out->setfontWeigth(pre->fontWeigth());
    out->setFont(pre->Font());
    out->setitalicFont(pre->italicFont());
    out->setunderlined(pre->underlined());
    out->setfontColor(pre->fontColor());
    out->setfontFamily(pre->fontFamily());
    out->setfontSize(pre->fontSize());
    out->setAlineacion(pre->Alineacion());
    if(ui->checkBox_2->isChecked())
    {
        int c = ui->comobFormato->currentIndex();
        out->setformato(c+1);
    }
    else
        out->setformato(0);
    out->setName(ui->txtNombre->text());
    out->setTarget(ui->comboBox->currentText());
    this->accept();
}

void EditAcum::on_fontComboBox_currentIndexChanged(const QString &arg1)
{
    pre->setfontFamily(arg1);
    scene.update();
}

void EditAcum::on_sizeSpin_valueChanged(int arg1)
{
    pre->setfontSize(arg1);
    scene.update();
}

void EditAcum::on_btnBold_toggled(bool checked)
{
    if(checked)
        pre->setfontWeigth(QFont::Bold);
    else
        pre->setfontWeigth(QFont::Normal);
    scene.update();
}

void EditAcum::on_btnItalic_toggled(bool checked)
{
    pre->setitalicFont(checked);
    scene.update();
}

void EditAcum::on_btnUnder_toggled(bool checked)
{
    pre->setunderlined(checked);
    scene.update();
}

void EditAcum::on_btnColor_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        pre->setfontColor(d.selectedColor());
        scene.update();
    }
}

void EditAcum::on_btnLeft_clicked()
{
    pre->setAlineacion(CustomLabel::Left);
    scene.update();
}

void EditAcum::on_btnCenter_clicked()
{
    pre->setAlineacion(CustomLabel::Center);
    scene.update();
}

void EditAcum::on_btnRigth_clicked()
{
    pre->setAlineacion(CustomLabel::Rigth);
    scene.update();
}

void EditAcum::on_checkBox_2_toggled(bool checked)
{
    ui->comobFormato->setEnabled(checked);
}
