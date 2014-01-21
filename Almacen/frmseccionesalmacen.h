#ifndef FRMSECCIONESALMACEN_H
#define FRMSECCIONESALMACEN_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class frmSeccionesAlmacen;
}

class frmSeccionesAlmacen : public QDialog
{
    Q_OBJECT

public:
    explicit frmSeccionesAlmacen(QWidget *parent = 0);
    ~frmSeccionesAlmacen();

private slots:
    void on_btnAnadir_clicked();

private:
    Ui::frmSeccionesAlmacen *ui;
    QSqlQueryModel *model_secciones;
};

#endif // FRMSECCIONESALMACEN_H
