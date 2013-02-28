#ifndef FRMFACTURARALABARANES_H
#define FRMFACTURARALABARANES_H

#include <QDialog>

namespace Ui {
class FrmFacturarAlabaranes;
}

class FrmFacturarAlabaranes : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmFacturarAlabaranes(QWidget *parent = 0);
    ~FrmFacturarAlabaranes();
    
private:
    Ui::FrmFacturarAlabaranes *ui;
};

#endif // FRMFACTURARALABARANES_H
