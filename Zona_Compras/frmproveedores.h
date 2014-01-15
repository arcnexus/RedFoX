#ifndef FRMPROVEEDORES_H
#define FRMPROVEEDORES_H

#include "../Auxiliares/Globlal_Include.h"
#include "../mayamodule.h"
#include "proveedor.h"

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
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}    
    QPushButton* wantShortCut(bool& ok){ok = true; return push;}


public slots:

private slots:
    void BloquearCampos(bool state);

    void LLenarCampos();
    void llenar_tabDatos();
    void llenar_tabFinanzas();
    void llenar_tabPagos();
    void llenar_tabAlmacen();
    void llenar_tabConta();
    void historiales();
    void acumulados();
    void grafica();
    void contactos();

    void CargarCamposEnProveedor();

    void editar_contacto();

    void borrar_contacto();

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

    void pagar_deuda();
    void pagar_fraccion();
    void ver_asiento();

    void menu_contactos(const QPoint&);
    void menu_deudas(const QPoint&);
    void nuevo_contacto();
    void guardar_contacto();



    void on_btnBuscar_clicked();

    void on_btnAnadirEntrega_clicked();


    void on_txtcif_editingFinished();


    void on_tabla_clicked(const QModelIndex &index);

    void on_tabla_doubleClicked(const QModelIndex &index);

    void on_tablaContactos_doubleClicked(const QModelIndex &index);


    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);    
    void on_txtpais_currentIndexChanged(int index);

    void on_txtcodigoFormaPago_currentIndexChanged(int index);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    Ui::frmProveedores *ui;
    QAction menuButton;
    QPushButton *push;
    QSqlQueryModel *model;
    void formato_tabla(QSqlQueryModel *modelo);

    Proveedor * oProveedor;
    BarraBusqueda* m_busqueda;
    void setUpBusqueda();

    bool editing;


    //////
    QSqlQueryModel *qmFormaPago;
    QSqlQueryModel * modelDivisas;
    QSqlQueryModel * modelPais;

    QSqlQueryModel *modelArticulo;
    QSqlQueryModel *modelAsientos;
    QSqlQueryModel *modeloDeudas;

    QSqlQueryModel *modelContactos;

    QSqlQueryModel *modelPedidos;
    QSqlQueryModel *modelFacturas;
    QSqlQueryModel *modelAlbaranes;
    QSqlQueryModel * modelEntregas;
};

#endif // FRMPROVEEDORES_H
