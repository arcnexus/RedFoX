#ifndef FRMGASTOS_PED_PRO_H
#define FRMGASTOS_PED_PRO_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class frmgastos_ped_pro;
}

class frmgastos_ped_pro : public QDialog
{
    Q_OBJECT

public:
    explicit frmgastos_ped_pro(QWidget *parent = 0);
    ~frmgastos_ped_pro();
    void setId_cabecera(int id_cab);

private slots:
    void on_btnAnadir_clicked();


private:
    Ui::frmgastos_ped_pro *ui;
    int id_cab;
};

#endif // FRMGASTOS_PED_PRO_H
