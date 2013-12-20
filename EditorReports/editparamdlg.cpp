#include "editparamdlg.h"
#include "ui_editparamdlg.h"
#include <QColorDialog>
editParamDlg::editParamDlg(reportParama * p, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editParamDlg)
{
    ui->setupUi(this);

    param = p;

    ui->graphicsView->setScene(&scene);
    previewLabel = new CustomLabel(0);
    ui->lineEdit->setText(p->arg());

    ui->btnBold->setChecked(p->fontWeigth()==QFont::Bold);
    ui->btnItalic->setChecked(p->italicFont());
    ui->btnUnder->setChecked(p->underlined());
    ui->fontComboBox->setCurrentText(p->fontFamily());
    ui->sizeSpin->setValue(p->fontSize());

    previewLabel->setText(p->arg());
    previewLabel->setfontWeigth(p->fontWeigth());
    previewLabel->setFont(p->Font());
    previewLabel->setitalicFont(p->italicFont());
    previewLabel->setunderlined(p->underlined());
    previewLabel->setfontColor(p->fontColor());
    previewLabel->setfontFamily(p->fontFamily());
    previewLabel->setfontSize(p->fontSize());
    previewLabel->setOrientacion(p->Orientacion());
    previewLabel->setAlineacion(p->Alineacion());
    previewLabel->setSize(p->rect().width(),p->rect().height());

    ui->txtNombre->setText(p->name());
    previewLabel->setFlag(QGraphicsItem::ItemIsSelectable,false);

    scene.addItem(previewLabel);
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(reject()));
}

editParamDlg::~editParamDlg()
{
    delete ui;
}

void editParamDlg::on_fontComboBox_currentIndexChanged(const QString &arg1)
{
    previewLabel->setfontFamily(arg1);
    scene.update();
}

void editParamDlg::on_sizeSpin_valueChanged(int arg1)
{
    previewLabel->setfontSize(arg1);
    scene.update();
}

void editParamDlg::on_btnBold_toggled(bool checked)
{
    if(checked)
        previewLabel->setfontWeigth(QFont::Bold);
    else
        previewLabel->setfontWeigth(QFont::Normal);
    scene.update();
}

void editParamDlg::on_btnItalic_toggled(bool checked)
{
    previewLabel->setitalicFont(checked);
    scene.update();
}

void editParamDlg::on_btnUnder_toggled(bool checked)
{
    previewLabel->setunderlined(checked);
    scene.update();
}

void editParamDlg::on_btnColor_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        previewLabel->setfontColor(d.selectedColor());
        scene.update();
    }
}

void editParamDlg::on_btnHorizontal_clicked()
{
    previewLabel->setOrientacion(Container::Horizontal);
    scene.update();
}

void editParamDlg::on_btnVertical_clicked()
{
    previewLabel->setOrientacion(Container::Vertical);
    scene.update();
}

void editParamDlg::on_btnLeft_clicked()
{
    previewLabel->setAlineacion(CustomLabel::Left);
    scene.update();
}

void editParamDlg::on_btnCenter_clicked()
{
    previewLabel->setAlineacion(CustomLabel::Center);
    scene.update();
}

void editParamDlg::on_btnRigth_clicked()
{
    previewLabel->setAlineacion(CustomLabel::Rigth);
    scene.update();
}

void editParamDlg::on_btnAceptar_clicked()
{
    param->setText(ui->txtNombre->text());
    param->setText(previewLabel->Text());
    param->setarg(previewLabel->Text());
    param->setfontWeigth(previewLabel->fontWeigth());
    param->setFont(previewLabel->Font());
    param->setitalicFont(previewLabel->italicFont());
    param->setunderlined(previewLabel->underlined());
    param->setfontColor(previewLabel->fontColor());
    param->setfontFamily(previewLabel->fontFamily());
    param->setfontSize(previewLabel->fontSize());
    param->setOrientacion(previewLabel->Orientacion());
    param->setAlineacion(previewLabel->Alineacion());
    this->accept();
}

void editParamDlg::on_lineEdit_textChanged(const QString &arg1)
{
    previewLabel->setText(arg1);
    scene.update();
}
