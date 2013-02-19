#ifndef EDITEVENTFORM_H
#define EDITEVENTFORM_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
namespace Ui {
class EditEventForm;
}

class EditEventForm : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditEventForm(QWidget *parent = 0);
    ~EditEventForm();
    void setButtonColor();
    QColor event_color;
    QString asunto;
    QTime start;
    QTime end;

    bool isCita;

    int id_cliente;
    int id_spec;
    int id_depart;
private slots:
    void on_btn_getColor_clicked();

    void on_btn_cancelar_clicked();

    void on_btn_guardar_clicked();

    void on_pushButton_clicked();

private:
    Ui::EditEventForm *ui;

};

#endif // EDITEVENTFORM_H
