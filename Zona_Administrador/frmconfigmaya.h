#ifndef FRMCONFIGMAYA_H
#define FRMCONFIGMAYA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
namespace Ui {
class frmConfigmaya;
}

class frmConfigmaya : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmConfigmaya(QWidget *parent = 0);
    ~frmConfigmaya();
    
private:
    Ui::frmConfigmaya *ui;
private slots:
    void configurar();
};

#endif // FRMCONFIGMAYA_H
