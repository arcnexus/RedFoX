#ifndef FRMTRASPASOALBARAN_H
#define FRMTRASPASOALBARAN_H

#include <QDialog>
#include "pedidoproveedor.h"
namespace Ui {
class frmTraspasoAlbaran;
}

class frmTraspasoAlbaran : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmTraspasoAlbaran(QWidget *parent = 0);
    ~frmTraspasoAlbaran();
    
private:
    Ui::frmTraspasoAlbaran *ui;

    PedidoProveedor *oPedidoProveedor;
private slots:
    void traspasar();
};

#endif // FRMTRASPASOALBARAN_H
