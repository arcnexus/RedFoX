#include "avisos.h"

Avisos::Avisos(QObject *parent) :
    QObject(parent)
{

}

int Avisos::add_notice(QHash<QString, QVariant> h)
{
    QString error;
    int new_id = SqlCalls::SqlInsert(h,"avisos",Configuracion_global->groupDB,error);
    if(new_id == -1)
        QMessageBox::warning(qApp->activeWindow(),tr("Avisos"),tr("Alerta: Ocurrió un error al insertar: %1").arg(error),
                             tr("Aceptar"));
    return new_id;
}

bool Avisos::update_notice(QHash<QString, QVariant> h)
{
    QString error;
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
    return true;
}
