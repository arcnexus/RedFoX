#ifndef FRMSELECTLOTES_H
#define FRMSELECTLOTES_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class frmSelectLotes;
}

class frmSelectLotes : public QDialog
{
    Q_OBJECT

public:
    explicit frmSelectLotes(QWidget *parent = 0);
    ~frmSelectLotes();
    void set_id_articulo(int id_art);
    QString get_lote();
    int get_id();

private slots:
    void on_tablaLotes_doubleClicked(const QModelIndex &index);

private:
    Ui::frmSelectLotes *ui;
    QString lote;
    QSqlQueryModel *modelo;
    int id;

};

#endif // FRMSELECTLOTES_H
