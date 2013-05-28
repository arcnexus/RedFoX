#ifndef FRMENTRADA_APUNTES_H
#define FRMENTRADA_APUNTES_H

#include <QDialog>

namespace Ui {
class FrmEntrada_apuntes;
}

class FrmEntrada_apuntes : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmEntrada_apuntes(QWidget *parent = 0);
    ~FrmEntrada_apuntes();
    
private:
    Ui::FrmEntrada_apuntes *ui;
};

#endif // FRMENTRADA_APUNTES_H
