#ifndef FRMANADIRHIERB_H
#define FRMANADIRHIERB_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class frmAnadirHierb;
}

class frmAnadirHierb : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmAnadirHierb(QWidget *parent = 0);
    ~frmAnadirHierb();
    int id;
    
private:
    Ui::frmAnadirHierb *ui;
private slots:
    void llenar_tabla_producto_nombre();
    void ver_vademecum();
    void anadir_producto();
};

#endif // FRMANADIRHIERB_H
