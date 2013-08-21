#ifndef EDITFIELDDLG_H
#define EDITFIELDDLG_H

#include <QDialog>
#include "reportfield.h"
#include "editdinamycitemdlg.h"
#include "customlabel.h"
namespace Ui {
class editFieldDlg;
}

class editFieldDlg : public EditDinamycItemDlg
{
    Q_OBJECT
    
public:
    explicit editFieldDlg(ReportField * fld ,QWidget *parent = 0);
    ~editFieldDlg();
    
private slots:


    void on_comboZona_currentIndexChanged(const QString &arg1);

    void on_comboTabla_currentIndexChanged(const QString &arg1);

    void on_fontComboBox_currentIndexChanged(const QString &arg1);

    void on_sizeSpin_valueChanged(int arg1);

    void on_btnBold_toggled(bool checked);

    void on_btnItalic_toggled(bool checked);

    void on_btnUnder_toggled(bool checked);

    void on_btnColor_clicked();

    void on_btnLeft_clicked();

    void on_btnCenter_clicked();

    void on_btnRigth_clicked();

    void on_btnAceptar_clicked();

    void on_checkBox_2_toggled(bool checked);

private:
    Ui::editFieldDlg *ui;
    QGraphicsScene scene;
    CustomLabel * pre;
    ReportField * field;
};

#endif // EDITFIELDDLG_H
