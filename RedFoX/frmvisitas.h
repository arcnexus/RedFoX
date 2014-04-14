#ifndef FRMVISITAS_H
#define FRMVISITAS_H

#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class frmVisitas;
}

class frmVisitas : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmVisitas(QWidget *parent = 0);
    ~frmVisitas();
    
private:
    Ui::frmVisitas *ui;
};

#endif // FRMVISITAS_H
