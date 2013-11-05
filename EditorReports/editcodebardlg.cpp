#include "editcodebardlg.h"
#include "ui_editcodebardlg.h"

EditCodeBarDlg::EditCodeBarDlg(CodeBar *code, QWidget *parent) :
    EditDinamycItemDlg(parent),
    ui(new Ui::EditCodeBarDlg)
{
    pre = new CodeBar(0);
    ui->setupUi(this);
    ui->comboZona->addItems(zonas());
    QStringList l = code->sql().split(".");
    if(l.size()==3)
    {
        ui->comboZona->setCurrentText(l.at(0));
        ui->comboTable->setCurrentText(l.at(1));
        ui->comboCampo->setCurrentText(l.at(2));
    }
    ui->checkBox->setChecked(code->visibleCode());
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));

    this->code = code;
    ui->graphicsView->setScene(&scene);


    pre->setcode("*123456789*");
    pre->setvisibleCode(code->visibleCode());

    pre->setcodeSize(code->codeSize());
    pre->setbarSize(code->barSize());
    pre->setvertical(code->vertical());

    ui->checkVertical->setChecked(code->vertical());
    ui->spinBarSize->setValue(code->barSize());
    ui->spinFontSize->setValue(code->codeSize());

    scene.addItem(pre);
}

EditCodeBarDlg::~EditCodeBarDlg()
{
    delete ui;
}

void EditCodeBarDlg::on_comboZona_currentIndexChanged(const QString &arg1)
{
    ui->comboTable->clear();
    ui->comboCampo->clear();
    ui->comboTable->addItems(tablas(arg1));
}

void EditCodeBarDlg::on_comboTable_currentIndexChanged(const QString &arg1)
{
    ui->comboCampo->clear();
    ui->comboCampo->addItems(campos(arg1));
}

void EditCodeBarDlg::on_pushButton_clicked()
{
    QString zone = ui->comboZona->currentText();
    QString table= ui->comboTable->currentText();
    QString campo = ui->comboCampo->currentText();

    QString sql = QString("%1.%2.%3").arg(zone).arg(table).arg(campo);

    code->setsql(sql);
    code->setvisibleCode(pre->visibleCode());

    code->setcodeSize(pre->codeSize());
    code->setbarSize(pre->barSize());
    code->setvertical(pre->vertical());
    this->accept();
}

void EditCodeBarDlg::on_checkBox_toggled(bool checked)
{
    pre->setvisibleCode(checked);
    scene.update();
}

void EditCodeBarDlg::on_spinBarSize_valueChanged(int arg1)
{
    pre->setbarSize(arg1);
    scene.update();
}

void EditCodeBarDlg::on_spinFontSize_valueChanged(int arg1)
{
    pre->setcodeSize(arg1);
    scene.update();
}

void EditCodeBarDlg::on_checkVertical_toggled(bool checked)
{
    pre->setvertical(checked);
    scene.update();
}
