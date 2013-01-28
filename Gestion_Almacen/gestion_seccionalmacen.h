#ifndef GESTION_SECCIONALMACEN_H
#define GESTION_SECCIONALMACEN_H

#include "Auxiliares/Globlal_Include.h"
namespace Ui {
class gestion_SeccionAlmacen;
}

enum tipo_gestion
{
    solo_ver , editar
};

class gestion_SeccionAlmacen : public QDialog
{
    Q_OBJECT
    
public:
    explicit gestion_SeccionAlmacen(QWidget *parent = 0 , QString tabla = "", int tipo = editar);
    ~gestion_SeccionAlmacen();
    QString value;
    void filter(QString filter);
private slots:
    void on_btn_add_clicked();

    void on_btn_remove_clicked();

    void on_btn_cancel_clicked();

    void on_btn_aceptar_clicked();

    void on_btn_save_clicked();

    void on_resultado_list_clicked(const QModelIndex &index);

private:
    Ui::gestion_SeccionAlmacen *ui;
    QSqlRelationalTableModel* model;
    QString tabla;
};

#endif // GESTION_SECCIONALMACEN_H
