#ifndef FRMDEVOLUCIONES_H
#define FRMDEVOLUCIONES_H

#include <QDialog>

namespace Ui {
class FrmDevoluciones;
}

class FrmDevoluciones : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmDevoluciones(QWidget *parent = 0);
    ~FrmDevoluciones();
    
private:
    Ui::FrmDevoluciones *ui;
};

#endif // FRMDEVOLUCIONES_H
