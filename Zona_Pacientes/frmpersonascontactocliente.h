#ifndef FRMPERSONASCONTACTOCLIENTE_H
#define FRMPERSONASCONTACTOCLIENTE_H

#include <QDialog>

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

private:
    Ui::frmPersonasContactoCliente *ui;
   void  RefrescarTabla();


private slots:
    void Anadir();
    void Borrar();
};

#endif // FRMPERSONASCONTACTOCLIENTE_H
