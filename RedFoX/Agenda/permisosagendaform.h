#ifndef PERMISOSAGENDAFORM_H
#define PERMISOSAGENDAFORM_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
namespace Ui {
class PermisosAgendaForm;
}

class PermisosAgendaForm : public QDialog
{
    Q_OBJECT
    
public:
    explicit PermisosAgendaForm(QWidget *parent = 0);
    ~PermisosAgendaForm();
    
private slots:
    void on_combo_from_currentIndexChanged(int index);

    void on_btn_add_clicked();

    void on_btn_remove_clicked();

private:
    Ui::PermisosAgendaForm *ui;
    QSqlRelationalTableModel model;
};

#endif // PERMISOSAGENDAFORM_H
