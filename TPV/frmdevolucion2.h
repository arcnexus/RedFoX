#ifndef FRMDEVOLUCION2_H
#define FRMDEVOLUCION2_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class frmdevolucion2;
}

class frmdevolucion2 : public QDialog
{
    Q_OBJECT

public:
    explicit frmdevolucion2(QWidget *parent = 0);
    ~frmdevolucion2();
    void set_values(int id,QString codigo,QString descripcion,float cantidad,float dev_act);

private slots:


    void on_btnAceptar_clicked();

private:
    Ui::frmdevolucion2 *ui;
    int id;
};

#endif // FRMDEVOLUCION2_H
