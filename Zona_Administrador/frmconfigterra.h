#ifndef FRMCONFIGTERRA_H
#define FRMCONFIGTERRA_H

#include <QDialog>

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
};

#endif // FRMCONFIGTERRA_H
