#ifndef FRMFACTURAS_H
#define FRMFACTURAS_H

#include "../Auxiliares/Globlal_Include.h"

#include "factura.h"
#include "../Zona_Pacientes/cliente.h"
#include "../Auxiliares/table_helper.h"
namespace Ui {
class frmFacturas;
}

class frmFacturas : public QDialog
{
    Q_OBJECT
    
public slots:
    void LLenarCampos();
    void LLenarCamposCliente();
    void LLenarFactura();
    void VaciarCampos();
    void BloquearCampos(bool state);
signals:
    void block();
    void unblock();
public:
    explicit frmFacturas(QWidget *parent = 0);
    ~frmFacturas();
    Factura *oFactura;
    Cliente *oCliente1;
    
private slots:
    void on_btnSiguiente_clicked();

    void Guardar_factura();

    void on_btnAnadir_clicked();

    void on_btnDeshacer_clicked();

    void on_btnAnterior_clicked();

    void on_botBuscarCliente_clicked();

    void on_btnBuscar_clicked();

    void on_btnImprimir_clicked();

    void totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
    void on_btnEditar_clicked();
    void lineaReady(lineaDetalle* ld);
private:
    Ui::frmFacturas *ui;
    QAction * actionGuardaBorrador;
    QAction * actionGuardaFactura;
    QMenu* menu_guardar;
    bool Altas;
    Table_Helper helper;
    bool in_edit;
};

#endif // FRMFACTURAS_H
