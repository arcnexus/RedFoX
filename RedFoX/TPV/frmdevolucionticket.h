#ifndef FRMDEVOLUCIONTICKET_H
#define FRMDEVOLUCIONTICKET_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
namespace Ui {
class FrmDevolucionTicket;
}

class FrmDevolucionTicket : public QDialog
{
    Q_OBJECT

public:
    explicit FrmDevolucionTicket(QWidget *parent = 0);
    ~FrmDevolucionTicket();
    int get_id_ticket();
    QString get_forma_devolucion();

private:
    Ui::FrmDevolucionTicket *ui;
    void cargarlineas(int id_cab, int id_cab_old);
    int id_ticket;

    QSqlQueryModel *tickets;
private slots:
    void refrescar_lineas(QModelIndex current, QModelIndex previous);

    void on_btnDevolucion_clicked();
    void on_btnParcial_clicked();
    void on_tablaLineas_tiquet_actual_doubleClicked(const QModelIndex &index);
};

#endif // FRMDEVOLUCIONTICKET_H
