#ifndef FRMADDENTREGASCUENTA_H
#define FRMADDENTREGASCUENTA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class frmAddEntregasCuenta;
}

class frmAddEntregasCuenta : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmAddEntregasCuenta(QWidget *parent = 0);
    ~frmAddEntregasCuenta();
    double importe;
    QDate fecha;
    QString concepto;
    int id_cliente;
    void set_concepto(QString concepto);
    double get_importe_entrega();
    void set_id_cliente(int id);
    
private slots:
    void on_txtImporte_editingFinished();

    void on_txtFecha_editingFinished();

    void on_txtConcepto_cursorPositionChanged(int arg1, int arg2);

    void validar();
private:
    Ui::frmAddEntregasCuenta *ui;
};

#endif // FRMADDENTREGASCUENTA_H
