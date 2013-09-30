#ifndef BARRABUSQUEDA_H
#define BARRABUSQUEDA_H

#include <QWidget>

namespace Ui {
class BarraBusqueda;
}

class BarraBusqueda : public QWidget
{
    Q_OBJECT
    
public:
    explicit BarraBusqueda(QWidget *parent = 0);
    ~BarraBusqueda();
signals:
    void closeME();
private:
    bool eventFilter(QObject *, QEvent *);
    Ui::BarraBusqueda *ui;
};

#endif // BARRABUSQUEDA_H
