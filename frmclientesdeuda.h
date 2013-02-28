#ifndef FRMCLIENTESDEUDA_H
#define FRMCLIENTESDEUDA_H

#include <QDialog>

namespace Ui {
class FrmClientesDeuda;
}

class FrmClientesDeuda : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmClientesDeuda(QWidget *parent = 0);
    ~FrmClientesDeuda();
    
private:
    Ui::FrmClientesDeuda *ui;
};

#endif // FRMCLIENTESDEUDA_H
