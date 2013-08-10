#ifndef EDITRECTDLG_H
#define EDITRECTDLG_H

#include <QDialog>
#include "roundedrect.h"
namespace Ui {
class EditRectDlg;
}

class EditRectDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditRectDlg(RoundedRect* r, QWidget *parent = 0);
    ~EditRectDlg();
private slots:
    void fillPattern(bool b)    ;
    void patterDirection(bool b);

    void on_spinGrueso_valueChanged(int arg1);


    void on_spinRadioX_valueChanged(double arg1);

    void on_spinRadioY_valueChanged(double arg1);

    void on_btnColor1_clicked();

    void on_btnColor2_clicked();

    void on_pushButton_3_clicked();

    void on_btnAceptar_clicked();

private:
    Ui::EditRectDlg *ui;
    QGraphicsScene scene;
    RoundedRect* pre;
    RoundedRect* rect;
};

#endif // EDITRECTDLG_H
