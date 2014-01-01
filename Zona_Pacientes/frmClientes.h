#ifndef FRMCLIENTES_H
#define FRMCLIENTES_H


#include "../Auxiliares/Globlal_Include.h"

//
#include "frmfichapaciente.h"
#include "cliente.h"
#include "../sqlcalls.h"
#include "../mayamodule.h"
namespace Ui {
class frmClientes;
}

class frmClientes : public MayaModule
{
    Q_OBJECT

public:
    explicit frmClientes(QWidget *parent = 0);
    ~frmClientes();
    bool Altas;
    module_zone module_zone(){return Mantenimiento;}
    QString module_name(){return "Clientes";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    
    QPushButton* wantShortCut(bool& ok) {ok = true; return push;}

signals:

public slots:
    void LLenarCampos();
    void LLenarCliente();
    void VaciarCampos();
   // void Setid_cliente(int id_cliente);

signals:
    void enviahistoriaynombre(int,QString);


private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

    void on_btnGuardar_clicked();

    void on_btnAnadir_clicked();

    void txtPrimerApellido_editingFinished();

    void txtSegundoApellido_editingFinished();

    void txtnombre_editingFinished();

    void txtpoblacion_editingFinished();

    void txtprovincia_editingFinished();

    void txtcif_nif_editingFinished();

    void on_btnEditar_clicked();
    void bloquearCampos(bool state);

    void on_btnDeshacer_clicked();

    void on_btnBorrar_clicked();

    void on_btnBuscar_clicked();

    void txtcp_editingFinished();

    void txtrRiesgoPermitido_editingFinished();

    void on_btnFichaPaciente_clicked();
    void AddCustomerType();
    void DelCustomerType();
    void DeshacerdireccionAlternativa();
    void BorrardireccionAlternativa();
    void EditardireccionAlternativa();
    void CargardireccionAlternativa(QModelIndex);
    void ValidarCC();
    void validarNifIntrac();
    void Contactos();
    void menu_deudas(QPoint);

    void cobrar_deuda();
    void cobrar_fraccion();
    void ver_asiento();
    void refrescar_grafica();
    void set_blink();



    void on_radBuscar_toggled(bool checked);

    void on_radEditar_toggled(bool checked);

    void on_tabla_busquedas_doubleClicked(const QModelIndex &index);

    void on_tabla_busquedas_clicked(const QModelIndex &index);

    void on_btnGuardardireccionAlternativa_clicked();

    void on_btnAnadirdireccion_clicked();

    void on_btnAdd_customer_clicked();

    void on_btnExcepciones_clicked();

    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);

    void on_btnCobroTotal_clicked();

    void on_radPendientes_toggled(bool checked);

    void on_TablaDeudas_clicked(const QModelIndex &index);

private:
    Ui::frmClientes *ui;
    QSqlQueryModel *modelFP;
    QSqlQueryModel *modelFacturas;
    QSqlQueryModel *modelPoblaciones;
    QSqlQueryModel *qModeldireccion;
    QSqlQueryModel *deudas ;
    QSqlQueryModel *modelHistorial;
    QSqlQueryModel * m_clientes;
    QSqlDatabase dbCliente;
    QSqlQuery tbpaciente;
    Cliente* oCliente;
    //SqlCalls *llamadasSQL;
    bool Anadirdireccion /*= false*/;
    int iddireccionAlternativa;
    void formato_tabla_busquedas();

    QAction menuButton;
    QPushButton * push;
    QHash<QString,QString> h_Buscar;

    BarraBusqueda* m_busqueda;
    void setUpBusqueda();


    bool eventFilter(QObject *obj, QEvent *event);
};
#endif
