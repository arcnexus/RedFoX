#ifndef FRMCREARVALE_H
#define FRMCREARVALE_H

#include <QDialog>

namespace Ui {
class frmCrearVale;
}

class frmCrearVale : public QDialog
{
    Q_OBJECT

public:
    explicit frmCrearVale(QWidget *parent = 0);
    ~frmCrearVale();
    void setImporte(double importe);
    void setId_ticket(int id_ticket);

private slots:
    void on_txtImporte_editingFinished();

    void on_txtcodigo_editingFinished();

    void on_btnCrear_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::frmCrearVale *ui;
    int id_ticket;
};

#endif // FRMCREARVALE_H
