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

    QSqlQueryModel *tickets;
private slots:
    void refrescar_lineas(QModelIndex current, QModelIndex previous);

};

#endif // FRMDEVOLUCIONTICKET_H
