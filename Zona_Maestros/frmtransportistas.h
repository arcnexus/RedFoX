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
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}
    QPushButton* wantShortCut(bool& ok){ok = false; return push;}
    
private slots:
    void on_btnAnadir_clicked();

    void on_btnGuardar_clicked();

    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();

    void on_btnBuscar_clicked();

    void on_btnEditar_2_clicked();

    void on_tablaBusqueda_doubleClicked(const QModelIndex &index);

    void on_tablaContactos_clicked(const QModelIndex &index);

private:
    Ui::FrmTransportistas *ui;
    transportistas oTransportista;
    ToolBarButton toolButton;
    QAction menuButton;
    QPushButton *push;
    QSqlQueryModel *model;
    void Bloquear_campos(bool state);
    void cargar_en_objeto();
    void llenar_campos();
    void llenar_campos_proveedor();
    bool anadiendo;
    bool eventFilter(QObject *obj, QEvent *event);
    void consultar_proveedor();
    QSqlQueryModel *m;
    void llenar_contactos(int id_proveedor);
};

#endif // FRMTRANSPORTISTAS_H
