#ifndef FRMCLIENTES_H
#define FRMCLIENTES_H


#include "../Auxiliares/Globlal_Include.h"

#include "../Busquedas/frmbuscarpoblacion.h"
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
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
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
    void bloquearCampos();
    void desbloquearCampos();

    void on_btnDeshacer_clicked();

    void on_btnBorrar_clicked();

    void on_btnBuscar_clicked();

    void txtcp_editingFinished();

    void txtcpAlternativa_editingFinished();

    void txtpoblacionAlternativa_editingFinished();

    void TablaDeudas_clicked(const QModelIndex &index);

    void txtrRiesgoPermitido_editingFinished();

    void on_btnFichaPaciente_clicked();
    void AddCustomerType();
    void AnadirdireccionAlternativa();
    void GuardardireccionAlternativa();
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

    void on_txtBuscar_textEdited(const QString &arg1);

    void on_tabla_busquedas_doubleClicked(const QModelIndex &index);

    void on_btnclear_clicked();

    void on_tabla_busquedas_clicked(const QModelIndex &index);

private:
    Ui::frmClientes *ui;
    QSqlQueryModel *modelFP;
    QSqlQueryModel *modelFacturas;
    QSqlQueryModel *modelPoblaciones;
    QSqlQueryModel *qModeldireccion;
    QSqlQueryModel * m_clientes;
    QSqlDatabase dbCliente;
    FrmBuscarPoblacion BuscarPoblacion;
    QSqlQuery tbpaciente;
    Cliente* oCliente;
    //SqlCalls *llamadasSQL;
    bool Anadirdireccion /*= false*/;
    int iddireccionAlternativa;
    void formato_tabla_busquedas();
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton * push;
    QHash<QString,QString> h_Buscar;
};
#endif
