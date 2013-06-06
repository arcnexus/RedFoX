#ifndef FRMALBARANPROVEEDOR_H
#define FRMALBARANPROVEEDOR_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../Auxiliares/table_helper.h"
#include "proveedor.h"
#include "albaranproveedor.h"

namespace Ui {
class FrmAlbaranProveedor;
}

class FrmAlbaranProveedor : public QDialog
{
    Q_OBJECT
    
public slots:
    void lineaReady(lineaDetalle *ld);
public:
    explicit FrmAlbaranProveedor(QWidget *parent = 0, bool showCerrar = false);
    ~FrmAlbaranProveedor();
    void llenarProveedor(int id);
    int id;



    void resizeTable(int x);
private slots:
    void totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();
    void buscar_proveeedor();
    void on_btnEditar_clicked();

    void on_btnGuardar_clicked();
    void guardar_campos_en_objeto();

    void on_btnBuscar_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnAnadir_clicked();

private:
    Ui::FrmAlbaranProveedor *ui;
    Table_Helper helper;
    Proveedor prov;
    AlbaranProveedor *oAlbPro;
    QString moneda;
    void llenar_campos();
    void bloquearcampos(bool estado);
signals:
    void block();
    void unblock();
};

#endif // FRMALBARANPROVEEDOR_H
