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

private slots:
    void on_txtImporte_editingFinished();

    void on_txtcodigo_editingFinished();

private:
    Ui::frmCrearVale *ui;
};

#endif // FRMCREARVALE_H
