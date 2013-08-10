#include "edittextdlg.h"
#include "ui_edittextdlg.h"
#include <QColorDialog>
EditTextDlg::EditTextDlg(CustomLabel *lbl, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditTextDlg)
{
    label = lbl;
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);
    previewLabel = new CustomLabel(0);

    ui->btnBold->setChecked(label->fontWeigth()==QFont::Bold);
    ui->btnItalic->setChecked(label->italicFont());
    ui->btnUnder->setChecked(label->underlined());
    ui->fontComboBox->setCurrentText(label->fontFamily());
    ui->sizeSpin->setValue(label->fontSize());

    previewLabel->setText(label->Text());
    previewLabel->setfontWeigth(label->fontWeigth());
    previewLabel->setFont(label->Font());
    previewLabel->setitalicFont(label->italicFont());
    previewLabel->setunderlined(label->underlined());
    previewLabel->setfontColor(label->fontColor());
    previewLabel->setfontFamily(label->fontFamily());
    previewLabel->setfontSize(label->fontSize());
    previewLabel->setOrientacion(label->Orientacion());
    previewLabel->setAlineacion(label->Alineacion());
    previewLabel->setSize(label->rect().width(),label->rect().height());

    previewLabel->setFlag(QGraphicsItem::ItemIsSelectable,false);

    scene.addItem(previewLabel);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(reject()));
}

EditTextDlg::~EditTextDlg()
{
    delete ui;
}

void EditTextDlg::on_plainTextEdit_textChanged()
{
    previewLabel->setText(ui->plainTextEdit->toPlainText());
    scene.update();
}

void EditTextDlg::on_fontComboBox_currentIndexChanged(const QString &arg1)
{
    previewLabel->setfontFamily(arg1);
    scene.update();
}

void EditTextDlg::on_sizeSpin_valueChanged(int arg1)
{
    previewLabel->setfontSize(arg1);
    scene.update();
}

void EditTextDlg::on_btnBold_toggled(bool checked)
{
    if(checked)
        previewLabel->setfontWeigth(QFont::Bold);
    else
        previewLabel->setfontWeigth(QFont::Normal);
    scene.update();
}

void EditTextDlg::on_btnItalic_toggled(bool checked)
{
    previewLabel->setitalicFont(checked);
    scene.update();
}

void EditTextDlg::on_btnUnder_toggled(bool checked)
{
    previewLabel->setunderlined(checked);
    scene.update();
}

void EditTextDlg::on_btnColor_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        previewLabel->setfontColor(d.selectedColor());
        scene.update();
    }
}

void EditTextDlg::on_btnHorizontal_clicked()
{
    previewLabel->setOrientacion(Container::Horizontal);
    scene.update();
}

void EditTextDlg::on_btnVertical_clicked()
{
    previewLabel->setOrientacion(Container::Vertical);
    scene.update();
}

void EditTextDlg::on_btnLeft_clicked()
{
    previewLabel->setAlineacion(CustomLabel::Left);
    scene.update();
}

void EditTextDlg::on_btnCenter_clicked()
{
    previewLabel->setAlineacion(CustomLabel::Center);
    scene.update();
}

void EditTextDlg::on_btnRigth_clicked()
{
    previewLabel->setAlineacion(CustomLabel::Rigth);
    scene.update();
}

void EditTextDlg::on_btnAceptar_clicked()
{
    label->setText(previewLabel->Text());
    label->setfontWeigth(previewLabel->fontWeigth());
    label->setFont(previewLabel->Font());
    label->setitalicFont(previewLabel->italicFont());
    label->setunderlined(previewLabel->underlined());
    label->setfontColor(previewLabel->fontColor());
    label->setfontFamily(previewLabel->fontFamily());
    label->setfontSize(previewLabel->fontSize());
    label->setOrientacion(previewLabel->Orientacion());
    label->setAlineacion(previewLabel->Alineacion());
    this->accept();
}
