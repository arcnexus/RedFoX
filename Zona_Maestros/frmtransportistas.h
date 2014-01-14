#ifndef FRMTRANSPORTISTAS_H
#define FRMTRANSPORTISTAS_H

#include "../Auxiliares/Globlal_Include.h"
#include "../Zona_Maestros/transportistas.h"
#include "../mayamodule.h"

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
    
protected:
private slots:
    void on_btnAnadir_clicked();

    void on_btnGuardar_clicked();

    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

    void on_btnBuscar_clicked();

    void on_btnEditar_clicked();

    void on_tablaBusqueda_doubleClicked(const QModelIndex &index);

    void on_tablaContactos_clicked(const QModelIndex &index);

    void mostrarBusqueda();
    void ocultarBusqueda();
    void filter_table(QString texto, QString orden, QString modo);
    void on_txtCodigo_editingFinished();

    void on_btnBorrar_clicked();

private:
    Ui::FrmTransportistas *ui;
    transportistas oTransportista;
    QAction menuButton;
    QSqlQueryModel *model;
    void Bloquear_campos(bool state);
    void cargar_en_objeto();
    void llenar_campos();
    void llenar_campos_transportista();
    bool anadiendo;
    bool eventFilter(QObject *obj, QEvent *event);
    void consultar_proveedor();
    QSqlQueryModel *m;
    void llenar_contactos(int id_proveedor);

    BarraBusqueda* m_busqueda;
    void setUpBusqueda();
    void vaciarCampos();
};

#endif // FRMTRANSPORTISTAS_H
