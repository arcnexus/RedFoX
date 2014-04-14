#ifndef FRMTRANSPORTISTAS_H
#define FRMTRANSPORTISTAS_H

#include "Auxiliares/Globlal_Include.h"
#include "../Zona_Maestros/transportistas.h"
#include "../mayamodule.h"
#include <QCompleter>
namespace Ui {
class FrmTransportistas;
}

class FrmTransportistas  : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmTransportistas(QWidget *parent = 0);
    int id_contacto;
    ~FrmTransportistas();
    module_zone module_zone(){return Mantenimiento;}
    QString module_name(){return "Transportistas";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    
    QPushButton* wantShortCut(bool& ok){ok = false; return 0;}            
public slots:
    void init();
protected:

private slots:
    void on_btnAnadir_clicked();

    void on_btnGuardar_clicked();

    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

    void on_btnBuscar_clicked();

    void on_btnEditar_clicked();

    void on_tablaBusqueda_doubleClicked(const QModelIndex &index);

    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);

    void on_btnBorrar_clicked();

    void on_btnAnadirContacto_clicked();

    void on_btnEditarContacto_clicked();

    void on_btnBorrarContacto_clicked();



    void on_pushButtonGuardarContacto_clicked();

    void on_btnDeshacer_clicked();

    void on_listaContactos_clicked(const QModelIndex &index);

    void on_txtcp_editingFinished();

    void on_txtdireccion2_editingFinished();

private:
    Ui::FrmTransportistas *ui;
    transportistas oTransportista;
    QAction menuButton;

    BarraBusqueda* m_busqueda;
    QSqlQueryModel* model_busqueda;
    QSqlTableModel* model_contactos;

    void init_querys();
    void Bloquear_campos(bool state);
    void cargar_en_objeto();
    void llenar_campos();
    bool anadiendo , anadiendoContacto;
    bool eventFilter(QObject *obj, QEvent *event);
    void consultar_proveedor();

    QCompleter * pob_completer;
    QSqlTableModel * pob_completer_model;

    QCompleter * calle_completer;
    QSqlTableModel * calle_completer_model;

    void setUpBusqueda();
    void vaciarCampos();
    void llenar_campos_contactoTransportista(int id);
    void bloquearCamposContacto(bool state);
};

#endif // FRMTRANSPORTISTAS_H
