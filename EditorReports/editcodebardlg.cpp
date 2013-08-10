#include "editcodebardlg.h"
#include "ui_editcodebardlg.h"

EditCodeBarDlg::EditCodeBarDlg(CodeBar *code, QWidget *parent) :
    EditDinamycItemDlg(parent),
    ui(new Ui::EditCodeBarDlg)
{
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

    pre = new CodeBar(0);
    pre->setcode("*123456789*");
    pre->setvisibleCode(code->visibleCode());
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
    this->accept();
}

void EditCodeBarDlg::on_checkBox_toggled(bool checked)
{
    pre->setvisibleCode(checked);
    scene.update();
}
