#ifndef EDITACUM_H
#define EDITACUM_H

#include <QDialog>
#include "reportacumulator.h"

namespace Ui {
class EditAcum;
}

class EditAcum : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditAcum(ReportAcumulator* rep, QWidget *parent = 0);
    ~EditAcum();
    
private slots:
    void on_btnAceptar_clicked();

    void on_fontComboBox_currentIndexChanged(const QString &arg1);

    void on_sizeSpin_valueChanged(int arg1);

    void on_btnBold_toggled(bool checked);

    void on_btnItalic_toggled(bool checked);

    void on_btnUnder_toggled(bool checked);

    void on_btnColor_clicked();

    void on_btnLeft_clicked();

    void on_btnCenter_clicked();

    void on_btnRigth_clicked();

    void on_checkBox_2_toggled(bool checked);

private:
    Ui::EditAcum *ui;
    ReportAcumulator* out;
    ReportAcumulator* pre;
    QGraphicsScene scene;
};

#endif // EDITACUM_H
