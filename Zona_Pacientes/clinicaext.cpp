#include "clinicaext.h"
#include "../db_table_view.h"
ClinicaExt::ClinicaExt(QObject *parent) :
    ModuleExtension(parent),
    doctores("Doctores",this),
    tImagen("Tipos de imagen",this),
    tAnalitica("Tipos de analítica",this),
    cAnalitica("Campos de analítica",this),
    mInterCon("Motivos de interconsulta",this)
{
    int id = -1;
    tryRegisterModule(MayaModule::SecretariaMedica,"Doctores",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&doctores,SIGNAL(triggered()),this,SLOT(handle_doctores()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &doctores;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::SecretariaMedica,"Tipos de imagen",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&tImagen,SIGNAL(triggered()),this,SLOT(handle_tiposImagen()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &tImagen;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::SecretariaMedica,"Tipos de analitica",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&tAnalitica,SIGNAL(triggered()),this,SLOT(handle_tipoAnalitica()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &tAnalitica;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::SecretariaMedica,"Campos de analitica",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&cAnalitica,SIGNAL(triggered()),this,SLOT(handle_campoAnalitica()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &cAnalitica;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::SecretariaMedica,"Motivos de interconsulta",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&mInterCon,SIGNAL(triggered()),this,SLOT(handle_motivoInterConsulta()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &mInterCon;
        p.second = getUserLvl(id);
        _actions.append(p);
    }
}

QList<QAction *> ClinicaExt::Extensions()
{
    QList<QAction *> r;
    QVector<QPair<QAction *, MayaModule::accessLevel> >::Iterator i;
    for(i=_actions.begin();i!=_actions.end();++i)
    {
       r.append((*i).first);
    }
    return r;
}
void ClinicaExt::handle_doctores()
{
    Db_table_View form(qApp->activeWindow());
    form.set_db("dbmedica");
    form.set_table("doctores");

    form.setWindowTitle(tr("Doctores"));

    QStringList headers;
    headers << tr("Nombre") << tr("Nº Colegiado") << tr("Teléfono") << tr("Especialidad 1");
    headers << tr("Especialidad 2") << tr("Dirección") << tr("C.P.") << tr("Población");
    headers << tr("Provincia") << tr("Pais") << tr("Movil");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.set_columnHide(12);
    form.set_columnHide(13);
    form.set_columnHide(14);
    form.exec();
}
void ClinicaExt::handle_tiposImagen()
{
    Db_table_View form(qApp->activeWindow());
    form.set_db("dbmedica");
    form.set_table("tiposimagen");

    form.setWindowTitle(tr("Tipo de imagenes"));

    QStringList headers;
    headers << tr("Descripcion") << tr("Comentarios");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.exec();
}

void ClinicaExt::handle_tipoAnalitica()
{
    Db_table_View form(qApp->activeWindow());
    form.set_db("dbmedica");
    form.set_table("tiposanalitica");

    form.setWindowTitle(tr("Tipo de Analitica"));

    QStringList headers;
    headers << tr("Tipos de Analisis");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.exec();
}
void ClinicaExt::handle_campoAnalitica()
{
    Db_table_View form(qApp->activeWindow());
    form.set_db("dbmedica");
    form.set_table("tiposanalitica2");

    form.setWindowTitle(tr("Campos para analitica"));

    QStringList headers;
    headers << tr("Tipo") << tr("Descripcion") << tr("Valores de referencia") << tr("Pertenece a:");
    form.set_table_headers(headers);

    form.set_relation(4,QSqlRelation("tiposanalitica","id","tipoanalisis"));
    form.set_columnHide(0);
    form.exec();
}
void ClinicaExt::handle_motivoInterConsulta()
{
    Db_table_View form(qApp->activeWindow());
    form.set_db("dbmedica");
    form.set_table("motivosinterconsulta");

    form.setWindowTitle(tr("Motivo de inter consulta"));

    QStringList headers;
    headers << tr("Tipo") << tr("Descripcion") << tr("Comentarios");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.exec();
}
