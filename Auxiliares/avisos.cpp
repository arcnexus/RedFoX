#include "avisos.h"

Avisos::Avisos(QObject *parent) :
    QObject(parent)
{

}

int Avisos::add_notice(QHash<QString, QVariant> h)
{
    QString error;
    h["id_tipo_aviso"] = this->id_tipo_aviso;
    h["id_usuario_origen"] = this->id_usuario_origen;
    h["id_usuario_destino"] = this->id_usuario_destino;
    h["aviso"] = this->aviso;
    h["fecha_hora_aviso"] = this->fecha_hora_aviso;
    h["completado"] = this->completado;
    h["id_empresa"] = this->id_empresa;

    int new_id = SqlCalls::SqlInsert(h,"avisos",Configuracion_global->groupDB,error);
    if(new_id == -1)
        QMessageBox::warning(qApp->activeWindow(),tr("Avisos"),tr("Alerta: Ocurrió un error al insertar: %1").arg(error),
                             tr("Aceptar"));
    return new_id;
}

bool Avisos::update_notice(QHash<QString, QVariant> h)
{
    QString error;
    h["id_tipo_aviso"] = this->id_tipo_aviso;
    h["id_usuario_origen"] = this->id_usuario_origen;
    h["id_usuario_destino"] = this->id_usuario_destino;
    h["aviso"] = this->aviso;
    h["fecha_hora_aviso"] = this->fecha_hora_aviso;
    h["completado"] = this->completado;
    h["id_empresa"] = this->id_empresa;

    bool success = SqlCalls::SqlUpdate(h,"avisos",Configuracion_global->groupDB,QString("id=%1").arg(this->id),error);
    if(success == -1)
        QMessageBox::warning(qApp->activeWindow(),tr("Avisos"),tr("Alerta: Ocurrió un error al actualizar: %1").arg(error),
                             tr("Aceptar"));

    return success;
}

bool Avisos::delete_notice()
{
    QString error;
    if(QMessageBox::question(qApp->activeWindow(),tr("Avisos"),
                             tr("¿Seguro que quiere borrar este aviso?\nEsta acción no se puede deshacer"),
                             tr("No"),tr("Sí")) == QMessageBox::Accepted)
    {
        bool success = SqlCalls::SqlDelete("avisos",Configuracion_global->groupDB,QString("id=%1").arg(this->id),
                                           error);
        if(!success)
            QMessageBox::warning(qApp->activeWindow(),tr("Avisos"),
                                 tr("Atención: Ocurrió un error al borrar el aviso: %1").arg(error),
                                 tr("Aceptar"));
        return success;
    }

}
