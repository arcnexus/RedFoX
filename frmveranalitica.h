#ifndef FRMVERANALITICA_H
#define FRMVERANALITICA_H

#include <QDialog>

namespace Ui {
class FrmVerAnalitica;
}

class FrmVerAnalitica : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmVerAnalitica(QWidget *parent = 0);
    ~FrmVerAnalitica();
    
private:
    Ui::FrmVerAnalitica *ui;
};

#endif // FRMVERANALITICA_H
