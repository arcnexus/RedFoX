#ifndef FRMCONFIGTERRA_H
#define FRMCONFIGTERRA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
namespace Ui {
class frmConfigTerra;
}

class frmConfigTerra : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmConfigTerra(QWidget *parent = 0);
    ~frmConfigTerra();
    
private:
    Ui::frmConfigTerra *ui;
private slots:
    void configurar();
};

#endif // FRMCONFIGTERRA_H
