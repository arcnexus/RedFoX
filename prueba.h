#ifndef PRUEBA_H
#define PRUEBA_H

#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class Prueba;
}

class Prueba : public QWidget
{
    Q_OBJECT
    
public:
    explicit Prueba(QWidget *parent = 0);
    ~Prueba();
    
private:
    Ui::Prueba *ui;
};

#endif // PRUEBA_H
