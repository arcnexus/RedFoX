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

    void on_chkRolloEtiq_toggled(bool checked);

    void on_spinPorFila_valueChanged(int arg1);

    void on_spinPorColum_valueChanged(int arg1);

    void on_rdbReport_toggled(bool checked);

    void on_rdbEtiqueta_toggled(bool checked);

    void on_rdbSobre_toggled(bool checked);

private:
    Ui::EditPaperDlg *ui;
    Paper* paper;
};

#endif // EDITPAPERDLG_H
