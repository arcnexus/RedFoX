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
    h_transportista["id_proveedor"] =0;

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
            h_transportista["id_proveedor"] = i.value().value("id_proveedor").toInt();
            if(h_transportista.value("id_proveedor").toInt() >-1){
                //--------------------------
                // Recupero datos proveedor
                //--------------------------
                oProveedor.Recuperar(h_transportista.value("id_proveedor").toInt());
            }


        }
        return true;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Transportistas"),
                         tr("No se pudo recuperar el transportista: ")+error,
                         tr("Aceptar"));
        return false;
    }
}

