#ifndef MAYAMODULE_H
#define MAYAMODULE_H

#include <QDialog>
#include "Auxiliares/Globlal_Include.h"
#include "Auxiliares/toolbarbutton.h"
class MayaModule : public QDialog
{
    Q_OBJECT
public:
    enum moduleZone{
        AdminZone = 0 , Mantenimiento , Compras , Ventas , Archivos , Almacen , Utilidades ,Contabilidad
        , SecretariaMedica,  InformacionMedica, NoZone
    };
    enum accessLevel{
        SinAcceso = 1,
        Lectura_parcial,
        Lectura_total,
        Escritura_parcial_editar,
        Escritura_parcial_add,
        Escritural_total,
        Administrador
    };

    explicit MayaModule(moduleZone zone , QString name , QWidget *parent = 0);
    ~MayaModule();
    virtual moduleZone ModuleZone(){return NoZone;}
    virtual QString ModuleName(){return "";}
    virtual ToolBarButton * ModuleToolBarButton() = 0;
    virtual QAction * ModuleMenuBarButton() = 0;
    virtual QString ModuleMenuPath() = 0; // Use '|' to subPaths

    bool userHaveAcces(int idUser);
    accessLevel userLevelInModule(){return _user_level;}

    virtual void hideButton() =0; //Used for showing the Dialog as stand alone window
signals:
    void block();
    void unblock();
public slots:

private:
    void tryRegisterModule(moduleZone zone , QString name);
    void RegisterModule(moduleZone zone , QString name);    
    moduleZone _zone;
    QString _name;
    int _id_modulo;
    accessLevel _user_level;
};

#endif // MAYAMODULE_H
