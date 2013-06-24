#ifndef MODULEEXTENSION_H
#define MODULEEXTENSION_H

#include <QObject>
#include <QAction>

#include "mayamodule.h"

class ModuleExtension : public QObject
{
    Q_OBJECT
public:
    explicit ModuleExtension(QObject *parent = 0);
    
    virtual QList<QAction *> Extensions() = 0;
    virtual QString ExtensionPath() = 0;// Use '|' to subPaths

    virtual MayaModule::moduleZone ModuleZone()=0;

protected:
    void tryRegisterModule(MayaModule::moduleZone zone , QString name , int& id);//Register each QAction as module
    MayaModule::accessLevel getUserLvl(int _id_modulo);
    bool userHaveAcess(int _id_modulo, int idUser);
signals:
    
public slots:

private:
    void RegisterModule(MayaModule::moduleZone zone , QString name , int& _id_modulo);
    QHash<int,MayaModule::accessLevel> _lvls;
};

#endif // MODULEEXTENSION_H
