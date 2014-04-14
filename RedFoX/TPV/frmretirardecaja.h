#ifndef FRMRETIRARDECAJA_H
#define FRMRETIRARDECAJA_H

#include <QDialog>
#include <Auxiliares/Globlal_Include.h>
namespace Ui {
class FrmRetirardeCaja;
}

class FrmRetirardeCaja : public QDialog
{
    Q_OBJECT

public:
    explicit FrmRetirardeCaja(QWidget *parent = 0);
    ~FrmRetirardeCaja();

private slots:
    void on_txtImporte_editingFinished();

    void on_btnAceptar_clicked();

private:
    Ui::FrmRetirardeCaja *ui;
};

#endif // FRMRETIRARDECAJA_H
