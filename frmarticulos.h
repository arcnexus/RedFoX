#ifndef FRMARTICULOS_H
#define FRMARTICULOS_H

#include <QDialog>

namespace Ui {
class FrmArticulos;
}

class FrmArticulos : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmArticulos(QWidget *parent = 0);
    ~FrmArticulos();
    
private:
    Ui::FrmArticulos *ui;
};

#endif // FRMARTICULOS_H
