#ifndef DLGDIVISIONESALMACEN_H
#define DLGDIVISIONESALMACEN_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class DlgDivisionesAlmacen;
}

class DlgDivisionesAlmacen : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDivisionesAlmacen(QWidget *parent = 0);
    ~DlgDivisionesAlmacen();

private slots:
    void on_listSecciones_clicked(const QModelIndex &index);

    void on_listFamilias_clicked(const QModelIndex &index);

    void on_listSubfamilias_clicked(const QModelIndex &index);

    void on_btnEdit_clicked();

    void on_btnDeshacer_clicked();

    void on_btnGuardar_clicked();

    void on_btnAddSeccion_clicked();

    void on_btnAddFamily_clicked();

    void on_btnAddSub_clicked();

    void on_btnCambiarimagen_clicked();

    void on_btnBorrarSec_clicked();

    void on_btnBorrarFam_clicked();

    void on_btnBorrarSub_clicked();

private:
    Ui::DlgDivisionesAlmacen *ui;
    bool _img_changed;
    QString _img;
    void load_imagen(QSqlTableModel * model, int row);

    QByteArray _current_img;

    QSqlTableModel * model_secciones;
    QSqlTableModel * model_familias;
    QSqlTableModel * model_subfamilias;

    void bloquear_campos(bool state);
    void vaciar_campos();

    enum editState { Seccion = 0, Familia, SubFamilia };
    editState editstatus;
    bool editando;

    void set_treeLabel(editState e);
    void add_new(editState e);
    bool save_new_seccion();
    bool save_new_family();
    bool save_new_subfamily();

    bool save_edit_seccion();
    bool save_edit_family();
    bool save_edit_subfamily();

    bool save_seccion(int row);
    bool save_family(int row);
    bool save_subfamily(int row);

    bool borrar_subfamilias(int id_familia);
    bool borrar_familia(int id_seccion);

    void refresh();
};

#endif // DLGDIVISIONESALMACEN_H
