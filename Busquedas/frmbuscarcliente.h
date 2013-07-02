#ifndef FRMBUSCARCLIENTE_H
#define FRMBUSCARCLIENTE_H

#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmBuscarCliente;
}

class FrmBuscarCliente : public QDialog
{
    Q_OBJECT

signals:
    void enviar_id_cliente(int id_cliente);

public:
    explicit FrmBuscarCliente(QWidget *parent = 0);
    ~FrmBuscarCliente();
    int Devolverid();
    
private slots:
    void on_pushButton_clicked();



private:
    Ui::FrmBuscarCliente *ui;
    QSqlQueryModel *ModelClientes;
    QHeaderView *Cabecera;
};

#endif // FRMBUSCARCLIENTE_H
