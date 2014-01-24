#include "moduleextension.h"

ModuleExtension::ModuleExtension(QObject *parent) :
    QObject(parent)
{

}

void ModuleExtension::tryRegisterModule(MayaModule::module_zone zone, QString name, int &id)
{
    QSqlQuery q(Configuracion_global->empresaDB);
    q.prepare("SELECT * FROM modulos where module_name = :name");
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

void ModuleExtension::RegisterModule(MayaModule::module_zone zone, QString name, int &_id_modulo)
{
    QSqlQuery q(Configuracion_global->empresaDB);
    QSqlQuery q2(Configuracion_global->empresaDB);

    q.prepare("INSERT INTO modulos (module_zone, module_name) VALUES (:zone, :name);");
    q.bindValue(":zone",zone);
    q.bindValue(":name",name);
    q.exec();
    _id_modulo = q.lastInsertId().toInt();
    for(auto i=0; i<Configuracion_global->usuarios_model->rowCount();i++)
    {
        //insertamos "Admin" para todos los usuarios por defecto
        q2.prepare("INSERT INTO accesousuarios (id_user, id_modulo, id_nivel_acceso) "
                   "VALUES (:id, :id_modulo, 7);");
        q2.bindValue(":id",Configuracion_global->usuarios_model->record(i).value("id").toInt());
        q2.bindValue(":id_modulo",_id_modulo);
        q2.exec();
    }
}


bool ModuleExtension::userHaveAcess(int _id_modulo, int id_user)
{
    MayaModule::accessLevel _user_level = MayaModule::SinAcceso;
    if(Configuracion_global->super_user)
    {
        _user_level = MayaModule::Administrador;
    }
    else
    {
        QSqlQuery q(Configuracion_global->empresaDB);
        q.prepare("SELECT * FROM accesousuarios where id_modulo = :id and id_user=:id_user");
        q.bindValue(":id",_id_modulo);
        q.bindValue(":id_user",id_user);

        if(q.exec())
        {
            if(q.next())
            {
                _user_level = static_cast<MayaModule::accessLevel>(q.record().value(3).toInt());
            }
        }
    }
    _lvls.insert(_id_modulo,_user_level);
    return (_user_level != MayaModule::SinAcceso);
}
