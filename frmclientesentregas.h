#ifndef FRMCLIENTESENTREGAS_H
#define FRMCLIENTESENTREGAS_H

#include <QDialog>

namespace Ui {
class FrmClientesEntregas;
}

class FrmClientesEntregas : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmClientesEntregas(QWidget *parent = 0);
    ~FrmClientesEntregas();
    
private:
    Ui::FrmClientesEntregas *ui;
};

#endif // FRMCLIENTESENTREGAS_H
