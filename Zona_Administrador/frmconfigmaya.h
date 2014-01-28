#ifndef FRMCONFIGMAYA_H
#define FRMCONFIGMAYA_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
#include "../mayamodule.h"

class AuxModule;
namespace Ui {
class frmConfigmaya;
}

class frmConfigmaya : public MayaModule
{
    Q_OBJECT
    
public:
    explicit frmConfigmaya(QWidget *parent = 0);
    ~frmConfigmaya();
    module_zone module_zone(){return AdminZone;}
    QString module_name(){return "Configuracion general";}
    QAction * ModuleMenuBarButton(){return &menuButton;}
    QString ModuleMenuPath(){return tr("");}
    QPushButton* wantShortCut(bool& ok){ok = false; return 0;}

private slots:
    void configurar();
    void on_btnSiguiente_clicked();
    bool checkForMayaGlobal();
    void crear_MayaGlobal();
    void doMenuSelection();
    bool createSuperUser();
    void on_btnInicio_clicked();
    void gotoEditPermisos();
    void on_cboGrupo_permiso_currentIndexChanged(int index);

    void on_cboEmpresa_permiso_currentIndexChanged(int index);

    void on_cboUser_permiso_currentIndexChanged(int index);
    
    void on_chkSuperUser_permiso_toggled(bool checked);

    void on_cbo_user_edit_currentIndexChanged(int index);

    void on_btnGuardarEdit_clicked();

private:
    Ui::frmConfigmaya *ui;
    QAction menuButton;

    QSqlQueryModel* model_grupos;
    QSqlQueryModel* model_empresa;

    void getModulesFromDB(QString bd_name);
    QVector<AuxModule*> _modulos;
    void addAdminModule(AuxModule *m);
    void addMantenModule(AuxModule *m);
    void addComprasModule(AuxModule *m);
    void addVentasModule(AuxModule *m);
    void addArchivosModule(AuxModule *m);
    void addAlmacenModule(AuxModule *m);
    void addUtilidadesModule(AuxModule *m);
    void addNoZoneModule(AuxModule *m);
    void addContaModule(AuxModule *m);
    void addSecMedicModule(AuxModule *m);
    void addInfoMedicModule(AuxModule *m);
    void llenarModulos(QString bd_name, int id_user);
};

#endif // FRMCONFIGMAYA_H
