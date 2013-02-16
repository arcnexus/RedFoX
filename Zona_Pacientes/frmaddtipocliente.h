#ifndef FRMADDTIPOCLIENTE_H
#define FRMADDTIPOCLIENTE_H

#include <QDialog>

namespace Ui {
class FrmAddTipoCliente;
}

class FrmAddTipoCliente : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAddTipoCliente(QWidget *parent = 0);
    ~FrmAddTipoCliente();
    
private:
    Ui::FrmAddTipoCliente *ui;
    void LLenarTablaSubfamilias(QString cFamilia);
};

#endif // FRMADDTIPOCLIENTE_H
