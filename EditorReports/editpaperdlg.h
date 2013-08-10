#ifndef EDITPAPERDLG_H
#define EDITPAPERDLG_H

#include <QDialog>
#include "paper.h"
namespace Ui {
class EditPaperDlg;
}

class EditPaperDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditPaperDlg(Paper* paper ,QWidget *parent = 0);
    ~EditPaperDlg();
    
private slots:


    void on_pushButton_2_clicked();

private:
    Ui::EditPaperDlg *ui;
    Paper* paper;
};

#endif // EDITPAPERDLG_H
