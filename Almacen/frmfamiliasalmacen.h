#ifndef FRMFAMILIASALMACEN_H
#define FRMFAMILIASALMACEN_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"

namespace Ui {
class frmFamiliasAlmacen;
}

class frmFamiliasAlmacen : public QDialog
{
    Q_OBJECT

public:
    explicit frmFamiliasAlmacen(QWidget *parent = 0);
    ~frmFamiliasAlmacen();

private slots:
    void on_btnAnadir_clicked();

    void on_btnEditar_clicked();

    void on_btnCambiarImagen_clicked();

    void on_btnGuardar_clicked();

    void on_tabla_familias_clicked(const QModelIndex &index);
    void on_tabla_secciones_clicked(const QModelIndex &index);

private:
    Ui::frmFamiliasAlmacen *ui;
    QSqlQueryModel *model_seccion;
    QSqlQueryModel *model_familias;
    QSqlQueryModel *model_subfamilias;
    void cargarImagen(int id);
    int id_seccion,id_familia;
    void setStatus(bool success);


};

#endif // FRMFAMILIASALMACEN_H
