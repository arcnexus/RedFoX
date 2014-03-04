#include "transportistas.h"

transportistas::transportistas(QObject *parent) :
    QObject(parent)
{
    h_transportista["id"] = 0;
    h_transportista["codigo"] = "";
    h_transportista["transportista"] = "";
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
    if(SqlCalls::SqlUpdate(h_transportista,"transportista",Configuracion_global->groupDB,condicion,error))
    {
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"Los datos han sido actualizados");
        return true;
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de transportistas"),tr("Ocurrió un error al actualizar:\n")+error,
                             tr("Aceptar"));
        return false;
    }
}


bool transportistas::recuperar(QStringList filtro,QStringList extras)
{
    QString error;
    QMap <int,QSqlRecord> r = SqlCalls::SelectRecord("transportista",filtro,extras,Configuracion_global->groupDB,error);
    QMapIterator <int,QSqlRecord> i(r);
    if(error.isEmpty())
    {
        if(!r.isEmpty())
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
        }
        return true;
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Transportistas"),
                         tr("No se pudo recuperar el transportista: ")+error,
                         tr("Aceptar"));
        return false;
    }
}

void transportistas::BorrarTransportista (int id)
{
    if(QMessageBox::question(qApp->activeWindow(),tr("Borrar Transportista"),
                          tr("Está apunto de borrar un transportista\n¿Desea continuar?"),
                          tr("No"),tr("Si")) == QMessageBox::Accepted)
    {
        QString error;
        bool ok = SqlCalls::SqlDelete("transportista",Configuracion_global->groupDB,QString("id = %1").arg(id),error);
        ok &= SqlCalls::SqlDelete("personascontactotransportista",Configuracion_global->groupDB,QString("id_transportista = %1").arg(id),error);
        if (!ok)
        {
            QMessageBox::warning(qApp->activeWindow(),tr("Borrar Cliente"),tr("Falló el borrado de tipos de cliente"),
                                 tr("Aceptar"));
        }
    }
}

