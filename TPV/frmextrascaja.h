#ifndef FRMEXTRASCAJA_H
#define FRMEXTRASCAJA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
namespace Ui {
class frmExtrasCaja;
}

class frmExtrasCaja : public QDialog
{
    Q_OBJECT

public:
    explicit frmExtrasCaja(QWidget *parent = 0);
    ~frmExtrasCaja();

private slots:
    void on_btnAceptar_clicked();

    void on_txtImporte_editingFinished();

private:
    Ui::frmExtrasCaja *ui;
};

#endif // FRMEXTRASCAJA_H
