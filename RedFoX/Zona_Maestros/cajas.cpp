#include "cajas.h"

cajas::cajas(QObject *parent) :
    QObject(parent)
{
    vaciar();
}

bool cajas::anadir()
{
return false;
}

bool cajas::guardar(QString id, QString descripcion, bool anadiendo)
{
    QSqlQuery consulta(Configuracion_global->empresaDB);

    if(anadiendo){
        consulta.exec("INSERT INTO cajas (id , desc_caja) VALUES ('"+id+"' , '"+descripcion+"')");
        qDebug() << consulta.lastError().text();
        return true;
    }else{
        consulta.exec("UPDATE cajas SET desc_caja = '"+descripcion+"' WHERE id = "+id);
        return true;
    }
return false;
}

bool cajas::recuperar(QString codigo)
{
    QSqlQuery consulta(Configuracion_global->empresaDB);
    if(consulta.exec("SELECT * FROM cajas WHERE id = "+codigo)){
        consulta.first();
        QSqlRecord registro = consulta.record();
        id = registro.field("id").value().toString();
        descripcion = registro.field("desc_caja").value().toString();
        return true;
    }

return false;
}

bool cajas::borrar(QString id)
{
    QSqlQuery consulta(Configuracion_global->empresaDB);
    if (consulta.exec("DELETE FROM cajas WHERE id = "+id)) {
        return true;
    }
return false;
}

void cajas::vaciar()
{
    id = "";
    descripcion = "";
}

