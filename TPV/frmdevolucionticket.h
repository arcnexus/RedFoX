#ifndef FRMDEVOLUCIONTICKET_H
#define FRMDEVOLUCIONTICKET_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
namespace Ui {
class FrmDevolucionTicket;
}

class FrmDevolucionTicket : public QDialog
{
    Q_OBJECT

public:
    explicit FrmDevolucionTicket(QWidget *parent = 0);
    ~FrmDevolucionTicket();

private:
    Ui::FrmDevolucionTicket *ui;
    void cargarlineas(int id_cab);

    QSqlQueryModel *tickets;
private slots:
    void refrescar_lineas(QModelIndex current, QModelIndex previous);

    void on_pushButton_clicked();
};

#endif // FRMDEVOLUCIONTICKET_H
