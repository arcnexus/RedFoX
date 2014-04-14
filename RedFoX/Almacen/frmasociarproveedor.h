#ifndef FRMASOCIARPROVEEDOR_H
#define FRMASOCIARPROVEEDOR_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
namespace Ui {
class FrmAsociarProveedor;
}

class FrmAsociarProveedor : public QDialog
{
    Q_OBJECT
    
public:
    explicit FrmAsociarProveedor(QWidget *parent = 0);
    ~FrmAsociarProveedor();
    QString codigo;
    double pvd;
    double pvd_real;
    QString DescOferta;
    QString Oferta;
    int id_proveedor ;
    int id_divisa;
    void setAnadir();
    void seteditar(QString id);
    
private:
    Ui::FrmAsociarProveedor *ui;
    QSqlQueryModel *modelProveedor;
    Configuracion configuracion;
private slots:
    void filtrar_proveedor();
    void setcodigo();
    void setpvd();
    void setDescOferta();
    void seleccionarPro(QModelIndex indicde);
    void setpvd_real();
    void setidDivisa();
public slots:
    void Aceptar();
};

#endif // FRMASOCIARPROVEEDOR_H
