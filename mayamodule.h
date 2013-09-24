#ifndef MAYAMODULE_H
#define MAYAMODULE_H

#include <QDialog>
#include <QPushButton>
#include "Auxiliares/Globlal_Include.h"

class MayaModule : public QDialog
{
    Q_OBJECT
public:
    enum module_zone {
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

    explicit MayaModule(module_zone zone , QString name , QWidget *parent = 0);
    ~MayaModule();
    virtual module_zone moduleZone(){return NoZone;}
    virtual QString module_name(){return "";}
    virtual QAction * ModuleMenuBarButton() = 0;
    virtual QString ModuleMenuPath() = 0; // Use '|' to subPaths
    virtual QPushButton* wantShortCut(bool& ok) = 0;

    bool userHaveAcces(int id_user);
    accessLevel userLevelInModule(){return _user_level;}

signals:
    void block();
    void unblock();
public slots:

private:
    void tryRegisterModule(module_zone zone , QString name);
    void RegisterModule(module_zone zone , QString name);
    module_zone _zone;
    QString _name;
    int _id_modulo;
    accessLevel _user_level;
};

#endif // MAYAMODULE_H
