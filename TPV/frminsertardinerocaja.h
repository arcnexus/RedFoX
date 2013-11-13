#ifndef FRMINSERTARDINEROCAJA_H
#define FRMINSERTARDINEROCAJA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
namespace Ui {
class FrmInsertarDineroCaja;
}

class FrmInsertarDineroCaja : public QDialog
{
    Q_OBJECT

public:
    explicit FrmInsertarDineroCaja(QWidget *parent = 0);
    ~FrmInsertarDineroCaja();

private slots:
    void on_txtimporte_editingFinished();

    void on_btnAceptar_clicked();

private:
    Ui::FrmInsertarDineroCaja *ui;
};

#endif // FRMINSERTARDINEROCAJA_H
