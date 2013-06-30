#ifndef FRMTIPOSTARIFA_H
#define FRMTIPOSTARIFA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmTiposTarifa;
}

class FrmTiposTarifa : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmTiposTarifa(QWidget *parent = 0);
    ~FrmTiposTarifa();
    
private slots:
    void on_btnAgregarTarifa_clicked();

private:
    Ui::FrmTiposTarifa *ui;
};

#endif // FRMTIPOSTARIFA_H
