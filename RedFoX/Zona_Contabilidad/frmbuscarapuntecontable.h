#ifndef FRMBUSCARAPUNTECONTABLE_H
#define FRMBUSCARAPUNTECONTABLE_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class frmBuscarApunteContable;
}

class frmBuscarApunteContable : public QDialog
{
    Q_OBJECT

public:
    explicit frmBuscarApunteContable(QWidget *parent = 0);
    ~frmBuscarApunteContable();
    int get_id();

private slots:
    void on_btnAceptar_clicked();

    void on_btnBuscar_clicked();

    void on_txtImporte_ini_editingFinished();

    void on_txtImporte_fin_editingFinished();

private:
    Ui::frmBuscarApunteContable *ui;
    int id;
    QSqlQueryModel *model;
};

#endif // FRMBUSCARAPUNTECONTABLE_H
