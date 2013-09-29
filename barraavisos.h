#ifndef BARRAAVISOS_H
#define BARRAAVISOS_H

#include <QWidget>
#include"../Auxiliares/Globlal_Include.h"

namespace Ui {
class BarraAvisos;
}

class BarraAvisos : public QWidget
{
    Q_OBJECT
    
public:
    explicit BarraAvisos(QWidget *parent = 0);
    ~BarraAvisos();
    void setPagina(int pagina);
    void set_tpv_desgloses(double subtotal1, double subtotal2, double subtotal3, double subtotal4, double dto1,
                                    double dto2, double dto3, double dto4, double base1, double base2, double base3,
                                    double base4, float porc_iva1, float porc_iva2, float porc_iva3, float porc_iva4,
                                    double iva1, double iva2, double iva3, double iva4, double total1, double total2,
                                    double total3, double total4);
signals:
    void closeMe();
protected:
    void paintEvent(QPaintEvent *pe);
private slots:
    void on_btnClose_clicked();

private:
    Ui::BarraAvisos *ui;
};

#endif // BARRAAVISOS_H
