#ifndef FRMANALITICA_H
#define FRMANALITICA_H

#include "Auxiliares/Globlal_Include.h"
#include "analitica.h"

namespace Ui {
class FrmAnalitica;
}

class FrmAnalitica : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAnalitica(QWidget *parent = 0);
    ~FrmAnalitica();
    
private:
    Ui::FrmAnalitica *ui;
    int id_analitica;
    Analitica oAnalitica;
public slots:
    void capturaid(int);
    void capturaPaciente(QString);
private slots:
    void AnadirCamposAnalitica();
    void AsignarAnalitica();
    void AsignarAnalitica(QString);
    void AsignarFecha();
    void AsignarFecha(QDate);
    void GuardarYCerrar();
    void llenartabla();
signals:
    void pasartipo(QString,int);
};

#endif // FRMANALITICA_H
