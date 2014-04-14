#ifndef FRMTIPOCLIENTE_H
#define FRMTIPOCLIENTE_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class frmTipoCliente;
}

class frmTipoCliente : public QDialog
{
    Q_OBJECT

public:
    explicit frmTipoCliente(QWidget *parent = 0);
    ~frmTipoCliente();

private slots:
    void on_btnAddTipo_clicked();

    void on_btnAddSubTipo_clicked();

    void on_lista_tipos_clicked(const QModelIndex &index);

    void on_lista_subtipos_clicked(const QModelIndex &index);

    void on_btnEditarTipo_clicked();

    void on_btnBorrarTipo_clicked();

    void on_btnEditarSubTipo_clicked();

    void on_btnBorrarSubTipo_clicked();

private:
    Ui::frmTipoCliente *ui;

    QSqlTableModel* model_tipos;
    QSqlTableModel* model_Subtipos;

    bool edit_tipo;
};

#endif // FRMTIPOCLIENTE_H
