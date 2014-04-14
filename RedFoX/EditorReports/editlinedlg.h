#ifndef EDITLINEDLG_H
#define EDITLINEDLG_H

#include <QDialog>
#include "reportline.h"
namespace Ui {
class EditLineDlg;
}

class EditLineDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditLineDlg(ReportLine *li, QWidget *parent = 0);
    ~EditLineDlg();
private slots:
    void styleChanged(bool b);
    void orientacion(bool b);
    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_3_clicked();

    void on_btnAceptar_clicked();

private:
    Ui::EditLineDlg *ui;
    QGraphicsScene scene;
    ReportLine * preLine;
    ReportLine * line;
};

#endif // EDITLINEDLG_H
