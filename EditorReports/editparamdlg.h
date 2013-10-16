#ifndef EDITPARAMDLG_H
#define EDITPARAMDLG_H

#include <QDialog>
#include "customlabel.h"
#include "reportparama.h"
namespace Ui {
class editParamDlg;
}

class editParamDlg : public QDialog
{
    Q_OBJECT
    
public:
    editParamDlg(reportParama * p ,QWidget *parent = 0);
    ~editParamDlg();
    
private slots:
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

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::editParamDlg *ui;
    QGraphicsScene scene;
    CustomLabel* previewLabel;
    reportParama * param;
};

#endif // EDITPARAMDLG_H
