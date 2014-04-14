#ifndef FRMANALITICA2_H
#define FRMANALITICA2_H

#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class Frmanalitica2;
}

class Frmanalitica2 : public QDialog
{
    Q_OBJECT
    
public:
    explicit Frmanalitica2(QWidget *parent = 0);
    ~Frmanalitica2();
    
private:
    Ui::Frmanalitica2 *ui;
    int id_analitica;
    QString tipo;
public slots:


private slots:
    void cargarDatos(QString tipo,int id_analitica);
    void aceptar();

signals:
};

#endif // FRMANALITICA2_H
