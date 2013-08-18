#include "mayamodule.h"

MayaModule::MayaModule(module_zone zone , QString name , QWidget *parent) :
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

bool MayaModule::userHaveAcces(int id_user)
{
    QSqlQuery q(Configuracion_global->groupDB);
    q.prepare("SELECT * FROM accesousuarios where id_modulo = :id and id_user=:id_user");
    q.bindValue(":id",_id_modulo);
    q.bindValue(":id_user",id_user);
    if(q.exec())
    {
        if(q.next())
        {
            _user_level = static_cast<accessLevel>(q.record().value(3).toInt());
            Configuracion_global->nivel = _user_level;
        }
    }
    return (_user_level != SinAcceso);
}

void MayaModule::tryRegisterModule(module_zone zone, QString name)
{
    QSqlQuery q(Configuracion_global->groupDB);
    q.prepare("SELECT * FROM modulos where module_name = :name");
    q.bindValue(":name",name);
    if(q.exec())
    {
        if(!q.next())
            RegisterModule(zone,name);
        else
            _id_modulo = q.record().value(0).toInt();
    }
}

void MayaModule::RegisterModule(module_zone zone , QString name)
{
    QSqlQuery q(Configuracion_global->groupDB);
    QSqlQuery q2(Configuracion_global->groupDB);

    q.prepare("INSERT INTO modulos (module_zone, module_name) VALUES (:zone, :name);");
    q.bindValue(":zone",zone);
    q.bindValue(":name",name);
    q.exec();
    _id_modulo = q.lastInsertId().toInt();
    q.prepare("SELECT id FROM usuarios;");
    q.exec();
    while(q.next())
    {
        //insertamos "Sin acceso" para todos los usuarios por defecto
        q2.prepare("INSERT INTO accesousuarios (id_user, id_modulo, id_nivel_acceso) "
                   "VALUES (:id, :id_modulo, 7);");
        q2.bindValue(":id",q.record().value(0).toInt());
        q2.bindValue(":id_modulo",_id_modulo);
        q2.exec();
    }
}
