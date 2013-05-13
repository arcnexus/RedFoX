#ifndef FRMCOBRARDEUDA_H
#define FRMCOBRARDEUDA_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
#include "cliente.h"
namespace Ui {
class frmCobrarDeuda;
}

class frmCobrarDeuda : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmCobrarDeuda(QWidget *parent = 0);
    ~frmCobrarDeuda();
    int id;
    
private:
    Ui::frmCobrarDeuda *ui;
    Cliente oCliente;

private slots:
    void calcular();
    void cobrar();
};

#endif // FRMCOBRARDEUDA_H
