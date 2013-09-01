#include "apuntes.h"

apuntes::apuntes(QObject *parent) :
    QObject(parent)
{
}

int apuntes::nuevo_numero_apunte()
{
    QSqlQuery queryApuntes(Configuracion_global->contaDB);
    if(queryApuntes.exec("select asiento from diario order by asiento desc limit 0,1"))
    {
        queryApuntes.next();
        int asiento = queryApuntes.record().value("asiento").toInt();
        return asiento+1;
    } else
    {
        return -1;
    }

}

bool apuntes::nuevalinea()
{
    QSqlQuery query_apunte(Configuracion_global->contaDB);
    query_apunte.prepare("INSERT INTO diario ( id_documento,cuenta_d,descripcion_d,"
                         "cuenta_h,descripcion_h,importe_d,importe_h,asiento,id_cuenta_d,id_cuenta_h,"
                         "fecha_asiento,pos_en_asiento,comentario_d,comentario_h,cuenta_relacion) "
                         "VALUES (:id_documento,:cuenta_d,:descripcion_d,:cuenta_h,"
                         ":descripcion_h,:importe_d,:importe_h,:asiento,:id_cuenta_d,:id_cuenta_h,"
                         ":fecha_asiento,:pos_en_asiento,:comentario_d,:comentario_h,:cuenta_relacion);");
    query_apunte.bindValue(":id_documento",this->id_documento);
    query_apunte.bindValue(":cuenta_d",this->cuenta_d);
    query_apunte.bindValue(":descripcion_d",this->descripcion_d);
    query_apunte.bindValue(":cuenta_h",this->cuenta_h);
    query_apunte.bindValue(":descripcion_h",this->descripcion_h);
    query_apunte.bindValue(":importe_d",this->importe_d);
    query_apunte.bindValue(":importe_h",this->importe_h);
    query_apunte.bindValue(":asiento",this->asiento);
    query_apunte.bindValue(":id_cuenta_d",this->id_cuenta_d);
    query_apunte.bindValue(":id_cuenta_h",this->id_cuenta_h);
    query_apunte.bindValue(":fecha_asiento",this->fecha_asiento);
    query_apunte.bindValue(":pos_en_asiento",this->pos_en_asiento);
    query_apunte.bindValue(":comentario_d",this->comentario_d);
    query_apunte.bindValue(":comentario_h",this->comentario_h);
    query_apunte.bindValue(":cuenta_relacion",this->cuenta_relacion);

    if(!query_apunte.exec())
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de apuntes"),
                             tr("Ocurrió un error al insertar %1").arg(query_apunte.lastError().text()),tr("Aceptar"));

        return false;
    } else
    {
        return true;
    }
}

void apuntes::clear()
{

    id = 0;
    id_cuenta = 0;
    id_documento = 0;
    DH = "";
    cuenta_d = "";
    descripcion_d = "";
    cuenta_h = "";
    descripcion_h = "";
    importe_d = 0;
    importe_h = 0;
    asiento = 0;
    id_cuenta_d = 0;
    id_cuenta_h = 0;
    fecha_asiento = QDate::currentDate();
    pos_en_asiento = 0;
    cta_principal = "";
    comentario_d = "";
    comentario_h = "";
}
