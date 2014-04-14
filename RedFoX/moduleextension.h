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

    virtual MayaModule::module_zone module_zone()=0;

protected:
    void tryRegisterModule(MayaModule::module_zone zone , QString name , int& id);//Register each QAction as module
    MayaModule::accessLevel getUserLvl(int _id_modulo);
    bool userHaveAcess(int _id_modulo, int id_user);
signals:
    
public slots:

private:
    void RegisterModule(MayaModule::module_zone zone , QString name , int& _id_modulo);
    QHash<int,MayaModule::accessLevel> _lvls;
};

#endif // MODULEEXTENSION_H
