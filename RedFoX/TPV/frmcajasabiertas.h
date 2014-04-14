#ifndef FRMCAJASABIERTAS_H
#define FRMCAJASABIERTAS_H

#include <QDialog>
#include <Auxiliares/Globlal_Include.h>

namespace Ui {
class frmCajasAbiertas;
}

class frmCajasAbiertas : public QDialog
{
    Q_OBJECT

public:
    explicit frmCajasAbiertas(QWidget *parent = 0);
    ~frmCajasAbiertas();
    void cargar_datos(QString caja,QString ejercicio);
    int id;

private slots:
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::frmCajasAbiertas *ui;
};

#endif // FRMCAJASABIERTAS_H
