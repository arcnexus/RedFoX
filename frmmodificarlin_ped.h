#ifndef FRMMODIFICARLIN_PED_H
#define FRMMODIFICARLIN_PED_H

#include <QDialog>

namespace Ui {
class FrmModificarLin_ped;
}

class FrmModificarLin_ped : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmModificarLin_ped(QWidget *parent = 0);
    ~FrmModificarLin_ped();

     void PonerCampos(int ID_Lin);
    
private slots:
    void on_pushButton_clicked();

    void on_txtCodigo_editingFinished();

    void CalcularTotal();

    void on_txtCantidad_editingFinished();

    void on_txtPVP_editingFinished();

    void on_txtPorcDto_editingFinished();

private:
    Ui::FrmModificarLin_ped *ui;
    int nId;
};

#endif // FRMMODIFICARLIN_PED_H
