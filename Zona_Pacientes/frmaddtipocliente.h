#ifndef FRMADDTIPOCLIENTE_H
#define FRMADDTIPOCLIENTE_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include <QListWidgetItem>

namespace Ui {
class FrmAddTipoCliente;
}

class FrmAddTipoCliente : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAddTipoCliente(QWidget *parent = 0 , int id_cliente = 0);
    ~FrmAddTipoCliente();

    void updateListas();
private slots:
    void on_btnAceptar_clicked();

    void on_list_tipo_clicked(const QModelIndex &index);

    void itemTipoChanged(QListWidgetItem*item);
    void itemSubTipoChanged(QListWidgetItem*item);

private:
    Ui::FrmAddTipoCliente *ui;
    int id_cliente;

    QSqlTableModel * model_tipos;
    QSqlTableModel * model_SubTipos;

    QSqlTableModel * model_cliente;
};

#endif // FRMADDTIPOCLIENTE_H
