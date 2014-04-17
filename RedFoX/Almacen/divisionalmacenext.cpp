#include "divisionalmacenext.h"
#include "../db_table_view.h"
#include "dlgdivisionesalmacen.h"

DivisionAlmacenExt::DivisionAlmacenExt(QObject *parent) :
    ModuleExtension(parent),
    divisiones("Divisiones Almacen",this)
{
    int id = -1;

    tryRegisterModule(MayaModule::Almacen,"Divisiones almacen",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &divisiones;
        p.second = getUserLvl(id);
        _actions.append(p);
    }
    connect(&divisiones,SIGNAL(triggered()),this,SLOT(do_divisiones()));
}

QList<QAction *> DivisionAlmacenExt::Extensions()
{
    QList<QAction *> r;
    QVector<QPair<QAction *, MayaModule::accessLevel> >::Iterator i;
    for(i=_actions.begin();i!=_actions.end();++i)
    {
       r.append((*i).first);
    }
    return r;
}

void DivisionAlmacenExt::do_divisiones()
{
    DlgDivisionesAlmacen dlg(qApp->activeWindow());
    dlg.exec();
}
