#ifndef FRMENTRADA_APUNTES_H
#define FRMENTRADA_APUNTES_H

#include <QDialog>
#include "apuntes.h"

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
    apuntes *oApunte;
signals:
    void block();
    void unblock();

private slots:
    void on_btnnuevo_asiento_clicked();
    void controles(bool stat);
    void on_btncerrar_asiento_clicked();
    void on_btnanadir_linea_clicked();
};

#endif // FRMENTRADA_APUNTES_H
