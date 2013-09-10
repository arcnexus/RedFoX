#ifndef FRMABRIRCAJA_H
#define FRMABRIRCAJA_H

#include <QDialog>

namespace Ui {
class FrmAbrirCaja;
}

class FrmAbrirCaja : public QDialog
{
    Q_OBJECT

public:
    explicit FrmAbrirCaja(QWidget *parent = 0);
    ~FrmAbrirCaja();

private slots:
    void on_pushButton_clicked();

private:
    Ui::FrmAbrirCaja *ui;
};

#endif // FRMABRIRCAJA_H
