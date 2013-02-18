#ifndef FRMADDTIPOCLIENTE_H
#define FRMADDTIPOCLIENTE_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class FrmAddTipoCliente;
}

class FrmAddTipoCliente : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAddTipoCliente(QWidget *parent = 0);
    ~FrmAddTipoCliente();
    QString cFamiliaRetorno;
    
private:
    Ui::FrmAddTipoCliente *ui;
private slots:
    void LLenarTablaSubfamilias(QModelIndex);
    void validar();
};

#endif // FRMADDTIPOCLIENTE_H
