#ifndef FRMBUSCARFACTURA_H
#define FRMBUSCARFACTURA_H

#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmBuscarFactura;
}

class FrmBuscarFactura : public QDialog
{
    Q_OBJECT

signals:
    void enviar_id_Factura(int Id_Factura);

public:
    explicit FrmBuscarFactura(QWidget *parent = 0);
    ~FrmBuscarFactura();
    int DevolverID();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::FrmBuscarFactura *ui;
    QSqlQueryModel *ModelFacturas;
    QHeaderView *Cabecera;
};

#endif // FRMBUSCARFACTURA_H
