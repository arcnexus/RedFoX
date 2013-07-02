#ifndef FRMBUSCARPOBLACION_H
#define FRMBUSCARPOBLACION_H

#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class FrmBuscarPoblacion;
}

class FrmBuscarPoblacion : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmBuscarPoblacion(QWidget *parent = 0);
    ~FrmBuscarPoblacion();
    int Devolverid();
    int id;

public slots:
    void  setpoblacion(QString poblacion,int nPoblacion);

    
private:
    Ui::FrmBuscarPoblacion *ui;
    QString poblacion;
    QString cSelect;
    QSqlQueryModel * ModelPoblaciones;
    QHeaderView *Cabecera;
    QSqlDatabase dbPobla;

};

#endif // FRMBUSCARPOBLACION_H
