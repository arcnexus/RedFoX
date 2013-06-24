#ifndef FRMEMPRESAS_H
#define FRMEMPRESAS_H

#include "../Auxiliares/Globlal_Include.h"
#include "auxmodule.h"
#include "../empresa.h"
#include "../mayamodule.h"
#include "copy_db_progressfrm.h"



namespace Ui {
class FrmEmpresas;
}

class FrmEmpresas : public MayaModule
{
    Q_OBJECT
    
public:
    explicit FrmEmpresas(QWidget *parent = 0);
    ~FrmEmpresas();
    void LLenarCampos();
    void CargarCamposEnEmpresa();

    moduleZone ModuleZone(){return AdminZone;}
    QString ModuleName(){return "Control de empresas";}
    ToolBarButton * ModuleToolBarButton(){return &toolButton;}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    void hideButton(){toolButton.hide();}QPushButton* wantShortCut(bool& ok){ok = false; return 0;}
private slots:
    //TODO - boton buscar
    //TODO - boton borrar

    void on_botSiguiente_clicked();

    void on_botAnterior_clicked();

    void on_botGuardar_clicked();

    void txtcPoblacion_editingFinished();

    void txtcCp_editingFinished();

    void on_botAnadir_clicked();

    void on_btn_ruta_db_clicked();

    void on_txtcDriver_currentIndexChanged(int index);

    void on_btn_migrar_clicked();

    void on_botBorrar_clicked();

private:
    Ui::FrmEmpresas *ui;
    Empresa oEmpresa;

    void limpiar_campos();
    bool crear_empresa_sqlite(copy_db_progressFrm * form);
    bool crear_medica_sqlite(copy_db_progressFrm * form);
    bool crear_empresa_mysql(copy_db_progressFrm * form);
    bool crear_medica_mysql(copy_db_progressFrm * form);

    void borrar_mysql();
    void borrar_sqlite();

    ToolBarButton toolButton;
    QAction menuButton;
};

#endif // FRMEMPRESAS_H
