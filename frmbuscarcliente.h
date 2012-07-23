#ifndef FRMBUSCARCLIENTE_H
#define FRMBUSCARCLIENTE_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QHeaderView>

namespace Ui {
class FrmBuscarCliente;
}

class FrmBuscarCliente : public QDialog
{
    Q_OBJECT

signals:
    void enviar_id_cliente(int Id_Cliente);

public:
    explicit FrmBuscarCliente(QWidget *parent = 0);
    ~FrmBuscarCliente();
    int DevolverID();
    
private slots:
    void on_pushButton_clicked();



private:
    Ui::FrmBuscarCliente *ui;
    QSqlQueryModel *ModelClientes;
    QHeaderView *Cabecera;
};

#endif // FRMBUSCARCLIENTE_H
