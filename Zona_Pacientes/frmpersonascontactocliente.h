#ifndef FRMPERSONASCONTACTOCLIENTE_H
#define FRMPERSONASCONTACTOCLIENTE_H

#include <QDialog>
#include "../Zona_Pacientes/cliente.h"

namespace Ui {
class frmPersonasContactoCliente;
}

class frmPersonasContactoCliente : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmPersonasContactoCliente(QWidget *parent = 0);
    ~frmPersonasContactoCliente();
    int nIdCliente;
   void  RefrescarTabla();
    Cliente oCliente;

private:
    Ui::frmPersonasContactoCliente *ui;
    int id_cliente;
    int id;



private slots:
    void Anadir();

    void on_btnEliminar_clicked();
    void on_tabla_PersonasContacto_clicked(const QModelIndex &index);
    void on_btnGuardar_clicked();
};

#endif // FRMPERSONASCONTACTOCLIENTE_H
