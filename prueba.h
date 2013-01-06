#ifndef PRUEBA_H
#define PRUEBA_H

#include <QWidget>

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
