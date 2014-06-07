#ifndef FRMCOBRARFACTURA_H
#define FRMCOBRARFACTURA_H

#include <QDialog>

class QSqlQueryModel;

namespace Ui {
class FrmCobrarFactura;
}

class FrmCobrarFactura : public QDialog
{
    Q_OBJECT

public:
    explicit FrmCobrarFactura(QWidget *parent = 0);
    ~FrmCobrarFactura();

    void setFactura(int id);
private slots:
    void on_btnCobrar_clicked();

    void on_tabla_vencimientos_clicked(const QModelIndex &index);

private:
    Ui::FrmCobrarFactura *ui;
    int _idFactura;

    QSqlQueryModel * vencimientos;
};

#endif // FRMCOBRARFACTURA_H
