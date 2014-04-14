#ifndef FRMCAUSADEVOLUCION_H
#define FRMCAUSADEVOLUCION_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
namespace Ui {
class frmCausaDevolucion;
}

class frmCausaDevolucion : public QDialog
{
    Q_OBJECT

public:
    explicit frmCausaDevolucion(QWidget *parent = 0);
    ~frmCausaDevolucion();
    void set_id_ticket(int id);

private slots:
    void on_btnAceptar_clicked();

private:
    Ui::frmCausaDevolucion *ui;
    int id;
    int id_art;
};

#endif // FRMCAUSADEVOLUCION_H
