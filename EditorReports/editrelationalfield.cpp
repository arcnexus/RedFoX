#include "editrelationalfield.h"
#include "ui_editrelationalfield.h"

editRelationalField::editRelationalField(RelationalField *fld, QWidget *parent) :
    EditDinamycItemDlg(parent),
    ui(new Ui::editRelationalField)
{
    ui->setupUi(this);
    ui->comboZona->addItems(zonas());
    ui->comboZona_2->addItems(zonas());

    field = fld;
    pre = new CustomLabel(0);

    ui->btnBold->setChecked(fld->fontWeigth()==QFont::Bold);
    ui->btnItalic->setChecked(fld->italicFont());
    ui->btnUnder->setChecked(fld->underlined());
    ui->fontComboBox->setCurrentText(fld->fontFamily());
    ui->sizeSpin->setValue(fld->fontSize());

    QStringList l1 = fld->sql().split("=");
    if(l1.size()==2)
    {
        QStringList l = l1.at(0).split(".");
        ui->comboZona->setCurrentText(l.at(0));
        ui->comboTabla->setCurrentText(l.at(1));
        ui->comboCampo->setCurrentText(l.at(2));

        QStringList l2 = l1.at(1).split("->");
        ui->comboZona_2->setCurrentText(l2.at(0).split(".").at(0));
        ui->comboTabla_2->setCurrentText(l2.at(0).split(".").at(1));
        ui->comboCampo_2->setCurrentText(l2.at(0).split(".").at(2));

        ui->comboCampo_3->setCurrentText(l2.at(1));
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

editRelationalField::~editRelationalField()
{
    delete ui;
}

void editRelationalField::on_comboZona_currentIndexChanged(const QString &arg1)
{
    ui->comboTabla->clear();
    ui->comboTabla->addItems(tablas(arg1));
}

void editRelationalField::on_comboZona_2_currentIndexChanged(const QString &arg1)
{
    ui->comboTabla_2->clear();
    ui->comboTabla_2->addItems(tablas(arg1));
}

void editRelationalField::on_comboTabla_currentIndexChanged(const QString &arg1)
{
    ui->comboCampo->clear();
    ui->comboCampo->addItems(campos(arg1));
}

void editRelationalField::on_comboTabla_2_currentIndexChanged(const QString &arg1)
{
    ui->comboCampo_2->clear();
    ui->comboCampo_2->addItems(campos(arg1));

    ui->comboCampo_3->clear();
    ui->comboCampo_3->addItems(campos(arg1));
}

void editRelationalField::on_btnAceptar_clicked()
{
    QString zone = ui->comboZona->currentText();
    QString table= ui->comboTabla->currentText();
    QString campo = ui->comboCampo->currentText();

    QString zone2 = ui->comboZona_2->currentText();
    QString table2= ui->comboTabla_2->currentText();
    QString campo2 = ui->comboCampo_2->currentText();
    QString campo3 = ui->comboCampo_3->currentText();

    QString sql = QString("%1.%2.%3=%4.%5.%6->%7").arg(zone).arg(table).arg(campo)
                                                  .arg(zone2).arg(table2).arg(campo2)
                                                  .arg(campo3);
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

void editRelationalField::on_fontComboBox_currentIndexChanged(const QString &arg1)
{
    pre->setfontFamily(arg1);
    scene.update();
}

void editRelationalField::on_sizeSpin_valueChanged(int arg1)
{
    pre->setfontSize(arg1);
    scene.update();
}

void editRelationalField::on_btnBold_toggled(bool checked)
{
    if(checked)
        pre->setfontWeigth(QFont::Bold);
    else
        pre->setfontWeigth(QFont::Normal);
    scene.update();
}

void editRelationalField::on_btnItalic_toggled(bool checked)
{
    pre->setitalicFont(checked);
    scene.update();
}

void editRelationalField::on_btnUnder_toggled(bool checked)
{
    pre->setunderlined(checked);
    scene.update();
}

void editRelationalField::on_btnColor_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        pre->setfontColor(d.selectedColor());
        scene.update();
    }
}

void editRelationalField::on_btnLeft_clicked()
{
    pre->setAlineacion(CustomLabel::Left);
    scene.update();
}

void editRelationalField::on_btnCenter_clicked()
{
    pre->setAlineacion(CustomLabel::Center);
    scene.update();
}

void editRelationalField::on_btnRigth_clicked()
{
    pre->setAlineacion(CustomLabel::Rigth);
    scene.update();
}
