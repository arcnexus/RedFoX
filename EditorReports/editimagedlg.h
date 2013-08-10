#ifndef EDITIMAGEDLG_H
#define EDITIMAGEDLG_H

#include <QDialog>
#include "reportimage.h"
namespace Ui {
class EditImageDlg;
}

class EditImageDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditImageDlg(ReportImage* img ,QWidget *parent = 0);
    ~EditImageDlg();
    
private slots:
    void on_btnRuta_clicked();

    void on_btnAceptar_clicked();

private:
    Ui::EditImageDlg *ui;
    QGraphicsScene scene;
    ReportImage* pre;
    ReportImage* image;
};

#endif // EDITIMAGEDLG_H
