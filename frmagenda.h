#ifndef FRMAGENDA_H
#define FRMAGENDA_H

#include <QMainWindow>

namespace Ui {
class frmAgenda;
}

class frmAgenda : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit frmAgenda(QWidget *parent = 0);
    ~frmAgenda();
    
private:
    Ui::frmAgenda *ui;
};

#endif // FRMAGENDA_H
