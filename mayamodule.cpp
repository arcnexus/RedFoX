#include "mayamodule.h"

MayaModule::MayaModule(moduleZone zone , QString name ,QWidget *parent) :
    QDialog(parent)
{
    _zone = zone;
    _name = name;
    _user_level = SinAcceso;
    tryRegisterModule(zone,name);
}

MayaModule::~MayaModule()
{
}

bool MayaModule::userHasAcces(int idUser)
{
    QSqlQuery q(QSqlDatabase::database("Maya"));
    q.prepare("SELECT * FROM accesousuarios where idModulo = :id and idUser=:idUser");
    q.bindValue(":id",_id_modulo);
    q.bindValue(":idUser",idUser);
    if(q.exec())
    {
        if(q.next())
        {
            _user_level = static_cast<accessLevel>(q.record().value(3).toInt());
        }
    }
    return (_user_level != SinAcceso);
}

void MayaModule::tryRegisterModule(moduleZone zone, QString name)
{
    QSqlQuery q(QSqlDatabase::database("Maya"));
    qDebug() << name;
    q.prepare("SELECT * FROM modulos where ModuleName = :name");
    q.bindValue(":name",name);
    if(q.exec())
    {
        if(!q.next())
            RegisterModule(zone,name);
        else
            _id_modulo = q.record().value(0).toInt();
    }

}

void MayaModule::RegisterModule(moduleZone zone , QString name)
{
    QSqlQuery q(QSqlDatabase::database("Maya"));
    q.prepare("INSERT INTO modulos (ModuleZone, ModuleName) VALUES (:zone, :name);");
    q.bindValue(":zone",zone);
    q.bindValue(":name",name);
    q.exec();
    _id_modulo = q.lastInsertId();
}
