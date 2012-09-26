#ifndef FRMMODIFICARLIN_ALB_H
#define FRMMODIFICARLIN_ALB_H

#include <QDialog>

namespace Ui {
class FrmModificarLin_alb;
}

class FrmModificarLin_alb : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmModificarLin_alb(QWidget *parent = 0);
    ~FrmModificarLin_alb();
    void PonerCampos(int ID_Lin);
private slots:
    void on_txtCodigo_editingFinished();

    void on_pushButton_clicked();

    void on_txtCantidad_editingFinished();

    void on_txtPVP_editingFinished();

    void on_txtPorcDto_editingFinished();

private:
    Ui::FrmModificarLin_alb *ui;
    void CalcularTotal();
    int nId;
};

#endif // FRMMODIFICARLIN_ALB_H
