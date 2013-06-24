#include "moduleextension.h"

ModuleExtension::ModuleExtension(QObject *parent) :
    QObject(parent)
{

}

void ModuleExtension::tryRegisterModule(MayaModule::moduleZone zone, QString name, int &id)
{
    QSqlQuery q(QSqlDatabase::database("Maya"));
    q.prepare("SELECT * FROM modulos where ModuleName = :name");
    q.bindValue(":name",name);
    if(q.exec())
    {
        if(!q.next())
            RegisterModule(zone,name,id);
        else
            id = q.record().value(0).toInt();
    }
}

MayaModule::accessLevel ModuleExtension::getUserLvl(int _id_modulo)
{
   return _lvls.value(_id_modulo);
}

void ModuleExtension::RegisterModule(MayaModule::moduleZone zone, QString name, int &_id_modulo)
{
    QSqlQuery q(QSqlDatabase::database("Maya"));
    QSqlQuery q2(QSqlDatabase::database("Maya"));

    q.prepare("INSERT INTO modulos (ModuleZone, ModuleName) VALUES (:zone, :name);");
    q.bindValue(":zone",zone);
    q.bindValue(":name",name);
    q.exec();
    _id_modulo = q.lastInsertId().toInt();
    q.prepare("SELECT id FROM usuarios;");
    q.exec();
    while(q.next())
    {
        //insertamos "Sin acceso" para todos los usuarios por defecto
        q2.prepare("INSERT INTO accesousuarios (idUser, idModulo, idNivelAcceso) "
                   "VALUES (:id, :idModulo, 1);");
        q2.bindValue(":id",q.record().value(0).toInt());
        q2.bindValue(":idModulo",_id_modulo);
        q2.exec();
    }
}


bool ModuleExtension::userHaveAcess(int _id_modulo, int idUser)
{
    QSqlQuery q(QSqlDatabase::database("Maya"));
    q.prepare("SELECT * FROM accesousuarios where idModulo = :id and idUser=:idUser");
    q.bindValue(":id",_id_modulo);
    q.bindValue(":idUser",idUser);
    MayaModule::accessLevel _user_level = MayaModule::SinAcceso;
    if(q.exec())
    {
        if(q.next())
        {
            _user_level = static_cast<MayaModule::accessLevel>(q.record().value(3).toInt());
        }
    }
    _lvls.insert(_id_modulo,_user_level);
    return (_user_level != MayaModule::SinAcceso);
}
