#ifndef FRMPROVEEDORES_H
#define FRMPROVEEDORES_H

#include "Auxiliares/Globlal_Include.h"

namespace Ui {
class frmProveedores;
}

class frmProveedores : public QDialog
{
    Q_OBJECT
    
public:
    explicit frmProveedores(QWidget *parent = 0);
    int id_contacto;
    ~frmProveedores();
public slots:
    void DesbloquerCampos();
    void BloquearCampos();
    void LLenarCampos();
    void CargarCamposEnProveedor();
    void cargar_forma_pago(QString);
    void editar_contacto();
    void borrar_contacto();
    
private slots:
    void on_btnSiguiente_clicked();

    void on_btnGuardar_clicked();

    void on_btnEditar_clicked();

    void on_btnAnadir_clicked();

    void on_btnAnterior_clicked();

    void on_txtcPoblacion_editingFinished();

    void on_txtcCP_editingFinished();

    void on_btnBorrar_clicked();

    void on_btnDeshacer_clicked();

    void on_txtcEntidadBancariaProveedor_editingFinished();

    void on_txtcOficinaBancariaProveedor_editingFinished();

    void on_txtcDCProveedor_editingFinished();

    void on_txtcCCProveedor_editingFinished();

    void on_txtcEntidadPagoProveedor_editingFinished();

    void on_txtcOficinaPagoProveedor_editingFinished();

    void on_txtcDCPagoProveedor_editingFinished();

    void on_txtcCuentaPagoProveedor_editingFinished();

    void on_txtcCPAlmacen_editingFinished();

    void on_txtcPais_currentIndexChanged(const QString &arg1);



    void on_txtcCodigoFormaPago_currentIndexChanged(const QString &arg1);


    void on_btnNuevaFactura_clicked();

    void on_btnNuevoAlbaran_clicked();

    void on_btnNuevoPedido_clicked();
    void pagar_deuda();
    void pagar_fraccion();
    void ver_asiento();

    void historiales();
    void acumulados();
    void grafica();
    void contactos();
    void menu_contactos(const QPoint&);
    void menu_deudas(const QPoint&);
    void nuevo_contacto();
    void guardar_contacto();



    void on_btnBuscar_clicked();

private:
    Ui::frmProveedores *ui;
};

#endif // FRMPROVEEDORES_H
