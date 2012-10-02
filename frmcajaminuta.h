#ifndef FRMCAJAMINUTA_H
#define FRMCAJAMINUTA_H

#include <QDialog>

namespace Ui {
class FrmCajaMinuta;
}

class FrmCajaMinuta : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmCajaMinuta(QWidget *parent = 0);
    ~FrmCajaMinuta();
    
private:
    Ui::FrmCajaMinuta *ui;
};

#endif // FRMCAJAMINUTA_H
