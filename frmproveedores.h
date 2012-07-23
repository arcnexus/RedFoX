#ifndef FRMPROVEEDORES_H
#define FRMPROVEEDORES_H

#include <QDialog>

namespace Ui {
class frmProveedores;
}

class frmProveedores : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmProveedores(QWidget *parent = 0);
    ~frmProveedores();
    
private:
    Ui::frmProveedores *ui;
};

#endif // FRMPROVEEDORES_H
