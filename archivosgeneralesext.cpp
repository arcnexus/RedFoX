#include "archivosgeneralesext.h"
#include "db_table_view.h"
#include "../Almacen/frmtipostarifa.h"
ArchivosGeneralesExt::ArchivosGeneralesExt(QObject *parent) :
    ModuleExtension(parent),
    tarifa("Tipos de tarifa",this),
    moneda("Monedas",this),
    paises("Paises",this),
    fPago("Formas de pago",this),
    iva("Tipos de iva",this),
    bancos("Bancos",this),
    avisos("Avisos",this)
{
    int id = -1;
    tryRegisterModule(MayaModule::Mantenimiento,"Tipo de tarifa",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&tarifa,SIGNAL(triggered()),this,SLOT(tipostarifa()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &tarifa;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::Mantenimiento,"Monedas",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&moneda,SIGNAL(triggered()),this,SLOT(handle_monedas()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &moneda;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::Mantenimiento,"Paises",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&paises,SIGNAL(triggered()),this,SLOT(handle_paises()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &paises;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::Mantenimiento,"Formas de pago",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&fPago,SIGNAL(triggered()),this,SLOT(handle_fomasPago()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &fPago;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::Mantenimiento,"Tipos de iva",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&iva,SIGNAL(triggered()),this,SLOT(handle_tiposIVA()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &iva;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::Mantenimiento,"Bancos",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&bancos,SIGNAL(triggered()),this,SLOT(handle_bancos()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &bancos;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::Mantenimiento,"Avisos",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        connect(&avisos,SIGNAL(triggered()),this,SLOT(handle_avisos()));
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &avisos;
        p.second = getUserLvl(id);
        _actions.append(p);
    }
}

QList<QAction *> ArchivosGeneralesExt::Extensions()
{
    QList<QAction *> r;
    QVector<QPair<QAction *, MayaModule::accessLevel> >::Iterator i;
    for(i=_actions.begin();i!=_actions.end();++i)
    {
       r.append((*i).first);
    }
    return r;
}

void ArchivosGeneralesExt::tipostarifa()
{
    FrmTiposTarifa tipotar(qApp->activeWindow());
    tipotar.exec();
}
void ArchivosGeneralesExt::handle_monedas()
{
    Db_table_View form(qApp->activeWindow());
    form.set_db("Maya");
    form.set_table("monedas");

    form.setWindowTitle(tr("Monedas"));

    QStringList headers;
    headers << tr("Moneda");
    form.set_table_headers(headers);

    form.set_columnHide(400);
    form.exec();
}

void ArchivosGeneralesExt::handle_paises()
{
    Db_table_View form(qApp->activeWindow());
    form.set_db("Maya");
    form.set_table("paises");

    form.setWindowTitle(tr("Paises"));

    QStringList headers;
    headers << tr("Pais") << tr("Moneda");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.set_relation(2,QSqlRelation("monedas","id","moneda"));
    form.set_printFile("x");//TODO hacer directorio general de reports
    form.exec();
    Configuracion_global->Cargar_paises();
}

void ArchivosGeneralesExt::handle_fomasPago()
{
    Db_table_View form(qApp->activeWindow());
    form.set_db("Maya");
    form.set_table("FormPago");

    form.setWindowTitle(tr("Formas de pago"));

    QStringList headers;
    headers << tr("Codigo") << tr("Forma de pago") << tr("Dia de pago 1") << tr("Dia de pago 2");
    headers << tr("Dia de pago 3") << tr("Dia de pago 4") << tr("Dia 1") << tr("Dia 2") << tr("Dia 3")<< tr("Dia 4");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.exec();
}
void ArchivosGeneralesExt::handle_tiposIVA()
{
    Db_table_View form(qApp->activeWindow());
    form.set_db("Maya");
    form.set_table("tiposiva");

    form.setWindowTitle(tr("Tipos de I.V.A"));

    QStringList headers;
    headers << "" << tr("Tipo") << tr("Decripción") << tr("I.V.A") << tr("Recargo equivalencia");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.set_columnHide(1);
    form.set_noInsertDeleteRows();
    form.exec();
    Configuracion_global->Cargar_iva();
}
void ArchivosGeneralesExt::handle_bancos()
{
    Db_table_View form(qApp->activeWindow());
    form.set_db("Maya");
    form.set_table("bancos");

    form.setWindowTitle(tr("Bancos"));

    QStringList headers;
    headers << tr("Descripción") << tr("Entidad") << tr("Oficina") << tr("Dc");
    headers << tr("Cuenta") << tr("Saldo");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.exec();
}
void ArchivosGeneralesExt::handle_avisos()
{
    Db_table_View form(qApp->activeWindow());
    form.set_db("Maya");
    form.set_table("avisos");

    form.setWindowTitle(tr("Avisos"));

    QStringList headers;
    headers << tr("Motivo") << tr("Inicio") << tr("Final") << tr("Descripcion") << tr("Estado");
    headers << tr("Tipo de aviso") << tr("Avisar a");
    form.set_table_headers(headers);

    form.set_relation(6,QSqlRelation("tiposaviso","id","tipoaviso"));
    form.set_relation(7,QSqlRelation("usuarios","id","nombre"));
    form.set_columnHide(0);
    form.exec();
}
