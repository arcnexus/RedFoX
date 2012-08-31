#ifndef FRMMODIFICARLIN_FAC_H
#define FRMMODIFICARLIN_FAC_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class frmModificarLin_fac;
}

class frmModificarLin_fac : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmModificarLin_fac(QWidget *parent = 0);
    ~frmModificarLin_fac();
    void ponerCampos(int ID_Lin);
    void CalcularTotal();
    
private slots:
    void on_txtCodigo_lostFocus();

    void on_txtCantidad_lostFocus();

    void on_txtPVP_lostFocus();

    void on_txtPorcDto_lostFocus();

    void on_pushButton_clicked();

private:
    Ui::frmModificarLin_fac *ui;
    int nId;
};

#endif // FRMMODIFICARLIN_FAC_H
