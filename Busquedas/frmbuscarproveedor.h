#ifndef FRMBUSCARPROVEEDOR_H
#define FRMBUSCARPROVEEDOR_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class FrmBuscarProveedor;
}

class FrmBuscarProveedor : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmBuscarProveedor(QWidget *parent = 0);
    ~FrmBuscarProveedor();
    QString cProv;
    int nIdProv;

    
private:
    Ui::FrmBuscarProveedor *ui;
    QSqlQueryModel *modelo;
private slots:
    void on_btnBuscar_clicked();
    void Aceptar();
};

#endif // FRMBUSCARPROVEEDOR_H
