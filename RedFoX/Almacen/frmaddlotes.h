#ifndef FRMADDLOTES_H
#define FRMADDLOTES_H

#include <QDialog>
# include"Auxiliares/Globlal_Include.h"

namespace Ui {
class frmaddLotes;
}

class frmaddLotes : public QDialog
{
    Q_OBJECT

public:
    explicit frmaddLotes(QWidget *parent = 0);
    ~frmaddLotes();
    void cargar_articulo(int id);

private slots:
    void on_btnAnadir_clicked();


private:
    Ui::frmaddLotes *ui;
    int id_articulo;
};

#endif // FRMADDLOTES_H
