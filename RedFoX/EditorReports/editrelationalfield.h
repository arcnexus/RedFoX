#ifndef EDITRELATIONALFIELD_H
#define EDITRELATIONALFIELD_H

#include <QDialog>
#include "editdinamycitemdlg.h"
#include "relationalfield.h"
#include "customlabel.h"
namespace Ui {
class editRelationalField;
}

class editRelationalField : public EditDinamycItemDlg
{
    Q_OBJECT
    
public:
    explicit editRelationalField(RelationalField*  fld ,QWidget *parent = 0);
    ~editRelationalField();
    
private slots:
    void on_comboZona_currentIndexChanged(const QString &arg1);

    void on_comboZona_2_currentIndexChanged(const QString &arg1);

    void on_comboTabla_currentIndexChanged(const QString &arg1);

    void on_comboTabla_2_currentIndexChanged(const QString &arg1);

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
    Ui::editRelationalField *ui;
    QGraphicsScene scene;
    CustomLabel * pre;
    RelationalField * field;
};

#endif // EDITRELATIONALFIELD_H
