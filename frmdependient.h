#ifndef FRMDEPENDIENT_H
#define FRMDEPENDIENT_H

#include <QDialog>

namespace Ui {
class FrmDependient;
}

class FrmDependient : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmDependient(QWidget *parent = 0);
    ~FrmDependient();
    
private:
    Ui::FrmDependient *ui;
};

#endif // FRMDEPENDIENT_H
