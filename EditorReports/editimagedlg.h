#ifndef EDITIMAGEDLG_H
#define EDITIMAGEDLG_H

#include <QDialog>
#include "reportimage.h"
#include "editdinamycitemdlg.h"

namespace Ui {
class EditImageDlg;
}

class EditImageDlg : public EditDinamycItemDlg
{
    Q_OBJECT
    
public:
    explicit EditImageDlg(ReportImage* img ,QWidget *parent = 0);
    ~EditImageDlg();
    
private slots:
    void on_btnRuta_clicked();

    void on_btnAceptar_clicked();

    void on_comboZona_currentIndexChanged(const QString &arg1);
    void on_comboTable_currentIndexChanged(const QString &arg1);
    void on_comboCampo_currentIndexChanged(const QString &arg1);
    void on_chkArchivo_toggled(bool checked);

private:
    Ui::EditImageDlg *ui;
    QGraphicsScene scene;
    ReportImage* pre;
    ReportImage* image;
};

#endif // EDITIMAGEDLG_H
