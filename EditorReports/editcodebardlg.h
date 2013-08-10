#ifndef EDITCODEBARDLG_H
#define EDITCODEBARDLG_H

#include <QDialog>
#include "editdinamycitemdlg.h"
#include "codebar.h"
namespace Ui {
class EditCodeBarDlg;
}

class EditCodeBarDlg : public EditDinamycItemDlg
{
    Q_OBJECT
    
public:
    explicit EditCodeBarDlg(CodeBar* code , QWidget *parent = 0);
    ~EditCodeBarDlg();
    
private slots:
    void on_comboZona_currentIndexChanged(const QString &arg1);

    void on_comboTable_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::EditCodeBarDlg *ui;
    QGraphicsScene scene;
    CodeBar * pre;
    CodeBar * code;
};

#endif // EDITCODEBARDLG_H
