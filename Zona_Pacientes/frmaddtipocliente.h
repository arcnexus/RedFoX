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
    QString familiaRetorno;
    
private:
    Ui::FrmAddTipoCliente *ui;
private slots:
    void LLenarTablaSubfamilias(QModelIndex);
    void on_btnAceptar_clicked();
};

#endif // FRMADDTIPOCLIENTE_H
