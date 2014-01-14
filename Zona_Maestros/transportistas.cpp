#include "transportistas.h"

transportistas::transportistas(QObject *parent) :
    QObject(parent)
{
    //------------------------
    // Inicio transportista
    //------------------------
    h_transportista["id"] = 0;
    h_transportista["codigo"] = "";
    h_transportista["transportista"] = "";
    //h_transportista["id_proveedor"] =0;

}

int transportistas::anadir()
{
    h_transportista.remove("id");

    QString error;
    int id  = SqlCalls::SqlInsert(h_transportista,"transportista",Configuracion_global->groupDB,error);
    if(id > -1)
    {
        h_transportista.insert("id",id);
        return id;
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de transportistas"),tr("Ocurrió un error al insertar: ")+error,
                    tr("Aceptar"));
    }
}

bool transportistas::guardar()
{

    QString error;
    QString condicion;
    condicion = QString("id = %1").arg(h_transportista.value("id").toInt());
    bool succes = SqlCalls::SqlUpdate(h_transportista,"transportista",Configuracion_global->groupDB,condicion,error);
    if (succes)
    {
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"Los datos han sido actualizados");
        return true;
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de transportistas"),tr("Ocurrió un error al actualizar:")+error,
                             tr("Aceptar"));
        return false;
    }
}


bool transportistas::recuperar(QStringList filtro,QStringList extras)
{
    QString error;
    QMap <int,QSqlRecord> r;
    r = SqlCalls::SelectRecord("transportista",filtro,extras,Configuracion_global->groupDB,error);
    QMapIterator <int,QSqlRecord> i(r);
    if(error.isEmpty())
    {
        while(i.hasNext())
        {
            i.next();
            h_transportista["id"] = i.value().value("id").toInt();
            h_transportista["codigo"] = i.value().value("codigo").toString();
            h_transportista["transportista"] = i.value().value("transportista").toString();
            h_transportista["cif"] = i.value().value("cif").toString();
            h_transportista["fecha_alta"] = i.value().value("fecha_alta").toString();
            h_transportista["direccion1"] = i.value().value("direccion1").toString();
            h_transportista["direccion2"] = i.value().value("direccion2").toString();
            h_transportista["cp"] = i.value().value("cp").toString();
            h_transportista["poblacion"] = i.value().value("poblacion").toString();
            h_transportista["provincia"] =i.value().value("provincia").toString();
            h_transportista["pais"] = i.value().value("pais").toString();
            h_transportista["telefono1"] = i.value().value("telefono1").toString();
            h_transportista["telefono2"] = i.value().value("telefono2").toString();
            h_transportista["fax"] = i.value().value("fax").toString();
            h_transportista["movil"] = i.value().value("movil").toString();
            h_transportista["email"] = i.value().value("email").toString();
            h_transportista["web"] = i.value().value("web").toString();
            h_transportista["contacto"] = i.value().value("contacto").toString();


            //h_transportista["id_proveedor"] = i.value().value("id_proveedor").toInt();
//            if(h_transportista.value("id_proveedor").toInt() >-1){
//                //--------------------------
//                // Recupero datos proveedor
//                //--------------------------
//                oProveedor.Recuperar(QString("select * from proveedores where id = %1").arg(h_transportista.value("id_proveedor").toInt()));
//            }


        }
        return true;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Transportistas"),
                         tr("No se pudo recuperar el transportista: ")+error,
                         tr("Aceptar"));
        return false;
    }
}

QString transportistas::RecuperarPais(int nid)
{
    QSqlQuery qPais(Configuracion_global->groupDB);
    qPais.prepare("select pais from paises where id =:id");
    qPais.bindValue(":id",nid);
    if(qPais.exec()) {
        qPais.next();
        QSqlRecord rPais = qPais.record();
        return rPais.field("pais").value().toString();
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Pacientes"),
                             tr("No se pudo enconontrar el Pais: %1 ").arg(QString::number(nid)),tr("Aceptar"));
    }
    return "";
}

void transportistas::BorrarTransportista (int id)
{
    if(QMessageBox::question(qApp->activeWindow(),tr("Borrar Transportista"),
                          tr("Está apunto de borrar un transportista\n¿Desea continuar?"),
                          tr("No"),tr("Si")) == QMessageBox::Accepted)
    {
        QSqlQuery qTransportista(Configuracion_global->groupDB);
        qTransportista.prepare("DELETE FROM transportista WHERE id =:id");
        qTransportista.bindValue(":id",id);
        if (!qTransportista.exec()) {
            QMessageBox::warning(qApp->activeWindow(),tr("Borrar Cliente"),tr("Falló el borrado de tipos de cliente"),
                                 tr("Aceptar"));
        }
}
}
