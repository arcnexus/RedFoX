#ifndef FRMRECIBOS_H
#define FRMRECIBOS_H

#include <QDialog>

namespace Ui {
class FrmRecibos;
}

class FrmRecibos : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmRecibos(QWidget *parent = 0);
    ~FrmRecibos();
    
private:
    Ui::FrmRecibos *ui;
};

#endif // FRMRECIBOS_H
