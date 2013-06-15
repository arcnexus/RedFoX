#ifndef EFECTIVOCAJA_H
#define EFECTIVOCAJA_H

#include <QDialog>
#include <QGridLayout>
#include "../Auxiliares/Globlal_Include.h"
#include "monedaview.h"
namespace Ui {
class EfectivoCaja;
}

class EfectivoCaja : public QDialog
{
    Q_OBJECT
    
public:
    explicit EfectivoCaja(QWidget *parent = 0);
    ~EfectivoCaja();
    
private slots:
    void on_btnAceptar_clicked();
    void valueChanged(double d);
    void on_checkBox_toggled(bool checked);

private:
    Ui::EfectivoCaja *ui;
    QGridLayout _container;
    QVector<MonedaView*> _monedas;

    void fillMonedas();
    void recalc();
};

#endif // EFECTIVOCAJA_H
