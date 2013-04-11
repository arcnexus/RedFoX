#ifndef FRMLISTADOSARTICULO_H
#define FRMLISTADOSARTICULO_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class frmListadosArticulo;
}

class frmListadosArticulo : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmListadosArticulo(QWidget *parent = 0);
    ~frmListadosArticulo();

    
private:
    Ui::frmListadosArticulo *ui;
private slots:
    void btn_Preview_clicked();
};

#endif // FRMLISTADOSARTICULO_H
