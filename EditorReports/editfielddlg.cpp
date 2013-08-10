#include "editfielddlg.h"
#include "ui_editfielddlg.h"
#include <QColorDialog>
editFieldDlg::editFieldDlg(ReportField *fld, QWidget *parent) :
    EditDinamycItemDlg(parent),
    ui(new Ui::editFieldDlg)
{
    ui->setupUi(this);
    ui->comboZona->addItems(zonas());
    field = fld;
    pre = new CustomLabel(0);

    ui->btnBold->setChecked(fld->fontWeigth()==QFont::Bold);
    ui->btnItalic->setChecked(fld->italicFont());
    ui->btnUnder->setChecked(fld->underlined());
    ui->fontComboBox->setCurrentText(fld->fontFamily());
    ui->sizeSpin->setValue(fld->fontSize());

    QStringList l = fld->sql().split(".");
    if(l.size()==3)
    {
        ui->comboZona->setCurrentText(l.at(0));
        ui->comboTabla->setCurrentText(l.at(1));
        ui->comboCampo->setCurrentText(l.at(2));
    }

    pre->setfontWeigth(fld->fontWeigth());
    pre->setFont(fld->Font());
    pre->setitalicFont(fld->italicFont());
    pre->setunderlined(fld->underlined());
    pre->setfontColor(fld->fontColor());
    pre->setfontFamily(fld->fontFamily());
    pre->setfontSize(fld->fontSize());
    pre->setAlineacion(fld->Alineacion());
 //   pre->setSize(fld->rect().width(),fld->rect().height());

    ui->graphicsView->setScene(&scene);
    scene.addItem(pre);
    pre->setText("1 2 3 4 5 6 7 8 9\na b c d e f g h i j k l m n ñ o p q r s t u v w x y z");
    pre->setSize(300,50);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

editFieldDlg::~editFieldDlg()
{
    delete ui;
}

void editFieldDlg::on_comboZona_currentIndexChanged(const QString &arg1)
{
    ui->comboTabla->clear();
    ui->comboTabla->addItems(tablas(arg1));
}

void editFieldDlg::on_comboTabla_currentIndexChanged(const QString &arg1)
{
    ui->comboCampo->clear();
    ui->comboCampo->addItems(campos(arg1));
}

void editFieldDlg::on_fontComboBox_currentIndexChanged(const QString &arg1)
{
    pre->setfontFamily(arg1);
    scene.update();
}

void editFieldDlg::on_sizeSpin_valueChanged(int arg1)
{
    pre->setfontSize(arg1);
    scene.update();
}

void editFieldDlg::on_btnBold_toggled(bool checked)
{
    if(checked)
        pre->setfontWeigth(QFont::Bold);
    else
        pre->setfontWeigth(QFont::Normal);
    scene.update();
}

void editFieldDlg::on_btnItalic_toggled(bool checked)
{
    pre->setitalicFont(checked);
    scene.update();
}

void editFieldDlg::on_btnUnder_toggled(bool checked)
{
    pre->setunderlined(checked);
    scene.update();
}

void editFieldDlg::on_btnColor_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        pre->setfontColor(d.selectedColor());
        scene.update();
    }
}

void editFieldDlg::on_btnLeft_clicked()
{
    pre->setAlineacion(CustomLabel::Left);
    scene.update();
}

void editFieldDlg::on_btnCenter_clicked()
{
    pre->setAlineacion(CustomLabel::Center);
    scene.update();
}

void editFieldDlg::on_btnRigth_clicked()
{
    pre->setAlineacion(CustomLabel::Rigth);
    scene.update();
}

void editFieldDlg::on_btnAceptar_clicked()
{
    QString sql = QString("%1.%2.%3").arg(ui->comboZona->currentText())
                                     .arg(ui->comboTabla->currentText())
                                     .arg(ui->comboCampo->currentText());
    field->setsql(sql);
    field->setexpandable(ui->checkBox->isChecked());
    field->setfontWeigth(pre->fontWeigth());
    field->setFont(pre->Font());
    field->setitalicFont(pre->italicFont());
    field->setunderlined(pre->underlined());
    field->setfontColor(pre->fontColor());
    field->setfontFamily(pre->fontFamily());
    field->setfontSize(pre->fontSize());
    field->setAlineacion(pre->Alineacion());
    this->accept();
}
