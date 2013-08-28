#ifndef FRMPROVEEDORES_H
#define FRMPROVEEDORES_H

#include "../Auxiliares/Globlal_Include.h"
#include "../mayamodule.h"
namespace Ui {
class frmProveedores;
}

class frmProveedores : public MayaModule
{
    Q_OBJECT
    
public:
    explicit frmProveedores(QWidget *parent = 0);
    int id_contacto;
    ~frmProveedores();
    module_zone module_zone(){return Mantenimiento;}
    QString module_name(){return "Proveedores";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}
public slots:
    void BloquearCampos(bool state);
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

    void on_txtpoblacion_editingFinished();

    void on_txtcp_editingFinished();

    void on_btnBorrar_clicked();

    void on_btnDeshacer_clicked();

    void on_txtentidad_bancaria_proveedor_editingFinished();

    void on_txtoficina_bancaria_proveedor_editingFinished();

    void on_txtdc_proveedor_editingFinished();

    void on_txtcc_proveedor_editingFinished();

    void on_txtentidad_pago_proveedor_editingFinished();

    void on_txtoficina_pago_proveedor_editingFinished();

    void on_txtdc_pago_proveedor_editingFinished();

    void on_txtcuentaPagoProveedor_editingFinished();

    void on_txtcp_almacen_editingFinished();

    void on_txtpais_currentIndexChanged(const QString &arg1);



    void on_txtcodigoFormaPago_currentIndexChanged(const QString &arg1);


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

    void on_btnAnadirEntrega_clicked();


    void on_txtcif_editingFinished();

    void on_radModo_busqueda_toggled(bool checked);

    void on_tabla_clicked(const QModelIndex &index);

    void on_tabla_doubleClicked(const QModelIndex &index);


    void on_btnLimpiar_clicked();

    void on_cboOrden_currentIndexChanged(const QString &arg1);

    void on_cboModo_currentIndexChanged(const QString &arg1);

    void on_txtBuscar_textEdited(const QString &arg1);

    void on_tablaContactos_doubleClicked(const QModelIndex &index);

private:
    Ui::frmProveedores *ui;
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton *push;
    QSqlQueryModel *model;
    void formato_tabla(QSqlQueryModel *modelo);
    void filter_table();
};

#endif // FRMPROVEEDORES_H
