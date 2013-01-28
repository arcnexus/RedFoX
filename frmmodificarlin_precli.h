#ifndef FRMMODIFICARLIN_PRECLI_H
#define FRMMODIFICARLIN_PRECLI_H

#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmModificarLin_precli;
}

class FrmModificarLin_precli : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmModificarLin_precli(QWidget *parent = 0);
    ~FrmModificarLin_precli();
    void PonerCampos(int ID_Lin);
private slots:
    void on_pushButton_clicked();

    void on_txtCodigo_editingFinished();



    void CalcularTotal();

    void on_txtCantidad_editingFinished();

    void on_txtPVP_editingFinished();

    void on_txtPorcDto_editingFinished();

private:
    Ui::FrmModificarLin_precli *ui;
    int nId;
};

#endif // FRMMODIFICARLIN_PRECLI_H
