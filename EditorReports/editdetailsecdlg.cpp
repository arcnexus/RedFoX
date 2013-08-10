#include "editdetailsecdlg.h"
#include "ui_editdetailsecdlg.h"

EditDetailSecDlg::EditDetailSecDlg(DetailSection* sec , QWidget *parent) :
    EditDinamycItemDlg(parent),
    ui(new Ui::EditDetailSecDlg)
{
    ui->setupUi(this);
    this->sec = sec;

    ui->comboZona->addItems(zonas());
    ui->comboZonaInterna->addItems(zonas());
    if(sec)
    {
        ui->cabChk->setChecked(sec->header());
        ui->footChk->setChecked(sec->foot());
        ui->lineEdit->setText(sec->SectionName());
        QStringList gsql = sec->SqlGlobal().split(".");
        if(gsql.size()==2)
        {
            ui->comboZona->setCurrentText(gsql.at(0));
            ui->comboTabla->setCurrentText(gsql.at(1));
        }
        QStringList isql = sec->SqlInterno().split(".");
        if(isql.size()==2)
        {
            ui->comboZonaInterna->setCurrentText(isql.at(0));
            ui->comboTablaInterna->setCurrentText(isql.at(1));
        }
        QStringList cla = sec->ClausulaInterna().split("=");
        if(cla.size()==2)
        {
            ui->comboFieldFrom->setCurrentText(cla.at(0));
            ui->comboFieldTo->setCurrentText(cla.at(1));
        }
        c1 = sec->color1();
        c2 = sec->color2();
        ui->chkColorear->setChecked(sec->colorear());
        ui->chkAlternative->setChecked(sec->use2Colors());
    }

    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

EditDetailSecDlg::~EditDetailSecDlg()
{
    delete ui;
}

void EditDetailSecDlg::on_btnAcept_clicked()
{
    if(sec)
    {
        sec->setHeader(ui->cabChk->isChecked());
        sec->setFoot(ui->footChk->isChecked());
        sec->setSectionName(ui->lineEdit->text());

        QString gSql = QString("%1.%2").arg(ui->comboZona->currentText()).arg(ui->comboTabla->currentText());
        QString iSql = QString("%1.%2").arg(ui->comboZonaInterna->currentText()).arg(ui->comboTablaInterna->currentText());
        QString cla = QString("%1=%2").arg(ui->comboFieldFrom->currentText()).arg(ui->comboFieldTo->currentText());

        sec->setSqlGlobal(gSql);
        sec->setSqlInterno(iSql);
        sec->setClausulaInterna(cla);

        sec->setcolor1(c1);
        sec->setcolor2(c2);
        sec->setcolorear(ui->chkColorear->isChecked());
        sec->setuse2Colors(ui->chkAlternative->isChecked());
    }
    this->accept();
}

void EditDetailSecDlg::on_comboZona_currentIndexChanged(const QString &arg1)
{
    ui->comboTabla->clear();
    ui->comboTabla->addItems(tablas(arg1));
}

void EditDetailSecDlg::on_comboZonaInterna_currentIndexChanged(const QString &arg1)
{
    ui->comboTablaInterna->clear();
    ui->comboTablaInterna->addItems(tablas(arg1));
}

void EditDetailSecDlg::on_comboTablaInterna_currentIndexChanged(const QString &arg1)
{
    ui->comboFieldFrom->clear();
    ui->comboFieldFrom->addItems(campos(arg1));
}

void EditDetailSecDlg::on_comboTabla_currentIndexChanged(const QString &arg1)
{
    ui->comboFieldTo->clear();
    ui->comboFieldTo->addItems(campos(arg1));
}

void EditDetailSecDlg::on_cabChk_toggled(bool checked)
{
    if(checked)
        ui->iGroup->setEnabled(true);
    else
        ui->iGroup->setEnabled(ui->footChk->isChecked());
}

void EditDetailSecDlg::on_footChk_toggled(bool checked)
{
    if(checked)
        ui->iGroup->setEnabled(true);
    else
        ui->iGroup->setEnabled(ui->cabChk->isChecked());
}

void EditDetailSecDlg::on_btnColor1_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        c1 = d.selectedColor();
    }
}

void EditDetailSecDlg::on_btnColor2_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        c2 = d.selectedColor();
    }
}
