#ifndef FRMSECCIONESALMACEN_H
#define FRMSECCIONESALMACEN_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"

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

    void on_btnEditar_clicked();

    void on_tabla_secciones_clicked(const QModelIndex &index);

    void on_btnCambiarImagen_clicked();

    void on_btnGuardar_clicked();

private:
    Ui::frmSeccionesAlmacen *ui;
    QSqlQueryModel *model_secciones;
    QSqlQueryModel *model_familias;
    void cargarImagen(int id);
    int id;
    void setStatus(bool success);

};

#endif // FRMSECCIONESALMACEN_H
