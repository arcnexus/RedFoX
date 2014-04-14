#ifndef EDITTEXTDLG_H
#define EDITTEXTDLG_H

#include <QDialog>
#include "customlabel.h"
namespace Ui {
class EditTextDlg;
}

class EditTextDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditTextDlg(CustomLabel* lbl ,QWidget *parent = 0);
    ~EditTextDlg();
    
private slots:
    void on_plainTextEdit_textChanged();

    void on_fontComboBox_currentIndexChanged(const QString &arg1);

    void on_sizeSpin_valueChanged(int arg1);


    void on_btnBold_toggled(bool checked);

    void on_btnItalic_toggled(bool checked);

    void on_btnUnder_toggled(bool checked);

    void on_btnColor_clicked();

    void on_btnHorizontal_clicked();

    void on_btnVertical_clicked();

    void on_btnLeft_clicked();

    void on_btnCenter_clicked();

    void on_btnRigth_clicked();

    void on_btnAceptar_clicked();

private:
    Ui::EditTextDlg *ui;
    QGraphicsScene scene;
    CustomLabel* previewLabel;
    CustomLabel* label;
};

#endif // EDITTEXTDLG_H
