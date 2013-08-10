#ifndef SECTIONEDITORDLG_H
#define SECTIONEDITORDLG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include "paper.h"
namespace Ui {
class SectionEditorDlg;
}

class SectionEditorDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit SectionEditorDlg(Paper* paper, QWidget *parent = 0);
    ~SectionEditorDlg();
    
private slots:
    void cabRepChk_toggled(bool checked);
    void footPageChk_toggled(bool checked);
    void footRepChk_toggled(bool checked);
    void cabPage_toggled(bool checked);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_btnEdit_clicked();

    void on_btnAdd_clicked();

    void on_btnUp_clicked();

    void on_btnDown_clicked();

    void on_btnBorrar_clicked();

private:
    Ui::SectionEditorDlg *ui;
    Paper* paper;
    void _addToList(QString name);
    bool _checkName(QString name);
    void updateList();
};

#endif // SECTIONEDITORDLG_H
