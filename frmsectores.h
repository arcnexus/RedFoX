#ifndef FRMSECTORES_H
#define FRMSECTORES_H

#include <QDialog>

namespace Ui {
class FrmSectores;
}

class FrmSectores : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmSectores(QWidget *parent = 0);
    ~FrmSectores();
    
private:
    Ui::FrmSectores *ui;
};

#endif // FRMSECTORES_H
