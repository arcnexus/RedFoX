#ifndef FRMCLIENTES_H
#define FRMCLIENTES_H


#include "Auxiliares/Globlal_Include.h"

//
#include "frmfichapaciente.h"
#include "cliente.h"

#include "../mayamodule.h"
#include <QCompleter>
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

    void init_querys();
    void bloquear_direccion_alt(bool state);
signals:

public slots:
    void init();
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

    void txtApellido_Nombre_editingFinished();

    void txtprovincia_editingFinished();

    void txtcif_nif_editingFinished();

    void on_btnEditar_clicked();
    void bloquearCampos(bool state);

    void on_btnDeshacer_clicked();

    void on_btnBorrar_clicked();

    void on_btnBuscar_clicked();


    void txtrRiesgoPermitido_editingFinished();

    void on_btnFichaPaciente_clicked();


    void BorrardireccionAlternativa();
    void EditardireccionAlternativa();
    void CargardireccionAlternativa(QModelIndex);
    void ValidarCC();
    void validarNifIntrac();
    void Contactos();
    void menu_deudas(QPoint);

    void ver_asiento();
    void refrescar_grafica();
    void set_blink();

    void on_tabla_busquedas_doubleClicked(const QModelIndex &index);

    void on_btnGuardardireccionAlternativa_clicked();

    void on_btnAnadirdireccion_clicked();

    void btnExcepciones_clicked();

    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);

    void on_btnCobroTotal_clicked();

    void on_radPendientes_toggled(bool checked);

    void on_TablaDeudas_clicked(const QModelIndex &index);

    void on_txtcp_editingFinished();

    void on_txtdireccion2_editingFinished();

    void on_btnEdita_tipoCliente_clicked();

    void on_txtcpPoblacionAlternativa_editingFinished();

    void on_txtdireccion1Alternativa2_editingFinished();

    void on_btnDeshacerdireccionAlternativa_clicked();

    void on_chklBloqueoCliente_toggled(bool checked);

private:
    Ui::frmClientes *ui;

    Cliente* oCliente;

    bool Anadirdireccion /*= false*/;
    int iddireccionAlternativa;
    void formato_tabla_busquedas();

    QAction menuButton;
    QPushButton * push;
    QHash<QString,QString> h_Buscar;

    BarraBusqueda* m_busqueda;
    void setUpBusqueda();

    bool eventFilter(QObject *obj, QEvent *event);

    //SQL MODELS
    QSqlQueryModel *qModeldireccion;
    QSqlQueryModel *Pedidos;
    QSqlQueryModel *Albaranes;
    QSqlQueryModel *Facturas;
    QSqlQueryModel *Presupuestos;
    QSqlQueryModel *Vales;
    QSqlQueryModel *modelAsientos;
    QSqlQueryModel *queryTransportistas;
    QSqlQueryModel *queryAgentes;
    QSqlQueryModel *qmidiomas;
    QSqlQueryModel *modelPoblaciones;
    QSqlQueryModel *deudas ;
    QSqlQueryModel *modelHistorial;
    QSqlQueryModel *m_clientes;

    QCompleter * pob_completer;
    QSqlTableModel * pob_completer_model;

    QCompleter * calle_completer;
    QSqlTableModel * calle_completer_model;

    void llenar_tipoCliente();
    bool __init;
};
#endif
