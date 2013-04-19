#ifndef FRMANADIRDIAGNOSTICO_H
#define FRMANADIRDIAGNOSTICO_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmAnadirDiagnostico;
}

class FrmAnadirDiagnostico : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAnadirDiagnostico(QWidget *parent = 0);
    ~FrmAnadirDiagnostico();
    int id;
    
private:
    Ui::FrmAnadirDiagnostico *ui;
private slots:
    void BtnBuscar_Clicked();
    void SeleccionLista(QModelIndex index);
};

#endif // FRMANADIRDIAGNOSTICO_H
