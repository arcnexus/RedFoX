#include "sectioneditordlg.h"
#include "ui_sectioneditordlg.h"
#include "ui_editdetailsecdlg.h"
#include "editdetailsecdlg.h"
SectionEditorDlg::SectionEditorDlg(Paper *paper, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SectionEditorDlg)
{
    ui->setupUi(this);
    this->paper = paper;

    //QList<Section*> sections = paper->getSeccionPool();
    QListIterator<Section*> it(paper->getSeccionPool());
    while(it.hasNext())
    {
        Section * sec = it.next();
        Section::SectionType t = sec->sectionType();
        switch (t) {
        case Section::ReportHeader:
            ui->cabRepChk->setChecked(true);
            break;
        case Section::PageHeader:
            ui->cabPage->setChecked(true);
            ui->fistPageChk->setChecked(qgraphicsitem_cast<PageHeaderSection*>(sec)->onFisrtPage());
            connect(ui->fistPageChk,SIGNAL(toggled(bool)),
                    qgraphicsitem_cast<PageHeaderSection*>(sec),SLOT(setOnFisrtPage(bool)));
            break;
        case Section::Detail:
            _addToList(sec->SectionName());
            break;
        case Section::PageFooter:
            ui->footPageChk->setChecked(true);
            break;
        case Section::ReportFooter:
            ui->footRepChk->setChecked(true);
            break;
        }
    }

    connect(ui->cabRepChk,SIGNAL(toggled(bool)),this,SLOT(cabRepChk_toggled(bool)));
    connect(ui->footPageChk,SIGNAL(toggled(bool)),this,SLOT(footPageChk_toggled(bool)));
    connect(ui->footRepChk,SIGNAL(toggled(bool)),this,SLOT(footRepChk_toggled(bool)));
    connect(ui->cabPage,SIGNAL(toggled(bool)),this,SLOT(cabPage_toggled(bool)));


}

SectionEditorDlg::~SectionEditorDlg()
{
    delete ui;
}

void SectionEditorDlg::cabRepChk_toggled(bool checked)
{
    if(checked)
        paper->addSection("Cabecera de report",Section::ReportHeader);
    else
        paper->removeSection("Cabecera de report");
}

void SectionEditorDlg::footPageChk_toggled(bool checked)
{
    if(checked)
        paper->addSection("Pie de pagina",Section::PageFooter);
    else
        paper->removeSection("Pie de pagina");
}

void SectionEditorDlg::footRepChk_toggled(bool checked)
{
    if(checked)
        paper->addSection("Pie de report",Section::ReportFooter);
    else
        paper->removeSection("Pie de report");
}

void SectionEditorDlg::cabPage_toggled(bool checked)
{
    if(checked)
        paper->addSection("Cabecera de pagina",Section::PageHeader);
    else
        paper->removeSection("Cabecera de pagina");
    QListIterator<Section*> it(paper->getSeccionPool());
    while(it.hasNext())
    {
        Section * sec = it.next();
        Section::SectionType t = sec->sectionType();
        if (t == Section::PageHeader)
        {
            connect(ui->fistPageChk,SIGNAL(toggled(bool)),
            qgraphicsitem_cast<PageHeaderSection*>(sec),SLOT(setOnFisrtPage(bool)));
            break;
        }
    }
}

void SectionEditorDlg::_addToList(QString name)
{
    QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
    item->setText(name);
}

bool SectionEditorDlg::_checkName(QString name)
{
    int count = 0;
    QListIterator<Section*> it(paper->getSeccionPool());
    while(it.hasNext())
    {
        if(it.next()->SectionName() == name)
            count++;
    }
    return count==1;
}

void SectionEditorDlg::updateList()
{
    ui->listWidget->clear();
    QListIterator<Section*> it(paper->getSeccionPool());
    while(it.hasNext())
    {
        Section * sec = it.next();
        Section::SectionType t = sec->sectionType();
        if (t == Section::Detail)
            _addToList(sec->SectionName());
    }
}

void SectionEditorDlg::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString text = item->text();
    QListIterator<Section*> it(paper->getSeccionPool());
    while(it.hasNext())
    {
        DetailSection * sec = qgraphicsitem_cast<DetailSection *>(it.next());
        if(sec->SectionName() == text)
        {
            EditDetailSecDlg dlg(sec,qApp->activeWindow());
            if(dlg.exec() == QDialog::Accepted)
            {
                if(_checkName(sec->SectionName()))
                {
                    item->setText(sec->SectionName());
                    paper->reCalculateSeccion(sec);
                }
                else
                {
                    QMessageBox::warning(this,"Error","Cada seccion debe tener un nombre unico");
                    sec->setSectionName(text);
                    on_listWidget_itemDoubleClicked(item);
                }
            }
            break;
        }
    }
}

void SectionEditorDlg::on_btnEdit_clicked()
{
    QList<QListWidgetItem*> l = ui->listWidget->selectedItems();
    if(l.isEmpty())
        return;
    on_listWidget_itemDoubleClicked(l.at(0));
}

void SectionEditorDlg::on_btnAdd_clicked()
{
    EditDetailSecDlg dlg(0,qApp->activeWindow());
    bool valid = false;
    while(!valid)
    {
        if(dlg.exec() == QDialog::Accepted)
        {
            QString text = dlg.ui->lineEdit->text();
            int count = 0;
            QListIterator<Section*> it(paper->getSeccionPool());
            while(it.hasNext())
            {
                if(it.next()->SectionName() == text)
                    count++;
            }
            if(count == 0)
                valid = true;
            else
                QMessageBox::warning(this,"Error","Cada seccion debe tener un nombre unico");
        }
        else
            break;
    }
    if(valid)
    {
        DetailSection * sec = new DetailSection(paper)  ;
        sec->setType(Section::Detail);
        sec->setMargin(paper->margin());
        sec->setHeader(dlg.ui->cabChk->isChecked());
        sec->setFoot(dlg.ui->footChk->isChecked());
        sec->setSectionName(dlg.ui->lineEdit->text());
        QString gSql = QString("%1.%2").arg(dlg.ui->comboZona->currentText()).arg(dlg.ui->comboTabla->currentText());
        QString iSql = QString("%1.%2").arg(dlg.ui->comboZonaInterna->currentText()).arg(dlg.ui->comboTablaInterna->currentText());
        QString cla = QString("%1=%2").arg(dlg.ui->comboFieldFrom->currentText()).arg(dlg.ui->comboFieldTo->currentText());

        sec->setSqlGlobal(gSql);
        sec->setSqlInterno(iSql);
        sec->setClausulaInterna(cla);

        connect(sec,SIGNAL(resized(Section*)),paper,SLOT(reCalculateSeccion(Section*)));
        paper->insertSection(sec);
        paper->reCalculateSeccion(sec);

        QListWidgetItem * item = new QListWidgetItem(ui->listWidget);
        item->setText(sec->SectionName());
    }
}

void SectionEditorDlg::on_btnUp_clicked()
{
    QList<QListWidgetItem*> l = ui->listWidget->selectedItems();
    if(l.isEmpty())
        return;
    paper->subirSeccion(l.at(0)->text());
    updateList();
}

void SectionEditorDlg::on_btnDown_clicked()
{
    QList<QListWidgetItem*> l = ui->listWidget->selectedItems();
    if(l.isEmpty())
        return;
    paper->bajarSeccion(l.at(0)->text());
    updateList();
}

void SectionEditorDlg::on_btnBorrar_clicked()
{
    QList<QListWidgetItem*> l = ui->listWidget->selectedItems();
    if(l.isEmpty())
        return;
    paper->borrarSeccion(l.at(0)->text());
    updateList();
}
