#ifndef EDITDETAILSECDLG_H
#define EDITDETAILSECDLG_H

#include <QDialog>
#include <QColorDialog>
#include "detailsection.h"
#include "editdinamycitemdlg.h"
namespace Ui {
class EditDetailSecDlg;
}

class EditDetailSecDlg : public EditDinamycItemDlg
{
    Q_OBJECT
    
public:
    explicit EditDetailSecDlg(DetailSection *sec, QWidget *parent = 0);
    ~EditDetailSecDlg();
    Ui::EditDetailSecDlg *ui;
    QColor c1;
    QColor c2;
private slots:

    void on_btnAcept_clicked();

    void on_comboZona_currentIndexChanged(const QString &arg1);

    void on_comboZonaInterna_currentIndexChanged(const QString &arg1);

    void on_comboTablaInterna_currentIndexChanged(const QString &arg1);

    void on_comboTabla_currentIndexChanged(const QString &arg1);

    void on_cabChk_toggled(bool checked);

    void on_footChk_toggled(bool checked);

    void on_btnColor1_clicked();

    void on_btnColor2_clicked();

private:

    DetailSection *sec;
};

#endif // EDITDETAILSECDLG_H
