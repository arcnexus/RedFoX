#ifndef FRMGESTIONCOBROS_H
#define FRMGESTIONCOBROS_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class frmGestionCobros;
}

class frmGestionCobros : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmGestionCobros(QWidget *parent = 0);
    ~frmGestionCobros();
    
private:
    Ui::frmGestionCobros *ui;
private slots:
    void on_txtbuscar_cliente_textChanged(const QString &arg1);
};

#endif // FRMGESTIONCOBROS_H
