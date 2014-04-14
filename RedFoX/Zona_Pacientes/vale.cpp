#include "vale.h"

vale::vale(QObject *parent) :
    QObject(parent)
{
}

 int vale::crearvale(int id_cliente,QString cliente, QDate fecha_ini, QDate fecha_fin, double importe, int id_ticket)
{
    QString error;
    int numero = nuevo_numero_vale();
    QHash <QString, QVariant>  v;
    v["numero"] = numero;
    v["codigo"] = "" ; // TODO - Generar código barras distintivo vale.
    v["fecha"] = fecha_ini;
    v["fecha_vencimiento_vale"] = fecha_fin;
    v["id_usuario"] = Configuracion_global->id_usuario_activo;
    v["importe"] = importe;
    v["id_ticket"] = id_ticket;
    v["id_cliente"] = id_cliente;
    v["cliente"] = cliente;
    v["agotado"] = false;
    v["ejercicio"] = Configuracion_global->cEjercicio.toInt();

    int new_id = SqlCalls::SqlInsert(v,"vales",Configuracion_global->empresaDB,error);
    if(new_id == -1)
        QMessageBox::warning(qApp->activeWindow(),tr("Creación de vales"),tr("Ocurrió un error al insertar %1").arg(error));
    else
        TimedMessageBox::Box(qApp->activeWindow(),tr("Se ha creado un nuevo vale"));

    return new_id;

}

int vale::nuevo_numero_vale()
{
    QSqlQuery vales(Configuracion_global->empresaDB);
    if(vales.exec("select numero from vales order by numero desc limit 1"))
    {
        vales.next();
        int new_vale = vales.record().value("numero").toInt();
        new_vale++;
        return new_vale;
    }
    else
        return -1;
}
