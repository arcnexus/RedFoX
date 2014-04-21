#include "importararticuloext.h"

#include "dlgimportods.h"

ImportarArticuloExt::ImportarArticuloExt(QObject *parent) :
    ModuleExtension(parent),
    fromOds(tr("Desde Hoja de cálculo (.ODS)"),this)
{
    int id = -1;

    tryRegisterModule(MayaModule::Almacen,"Importar Articulo - ODS",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &fromOds;
        p.second = getUserLvl(id);
        _actions.append(p);
    }
    connect(&fromOds,SIGNAL(triggered()),this,SLOT(do_ods()));
}

QList<QAction *> ImportarArticuloExt::Extensions()
{
    QList<QAction *> r;
    QVector<QPair<QAction *, MayaModule::accessLevel> >::Iterator i;
    for(i=_actions.begin();i!=_actions.end();++i)
    {
       r.append((*i).first);
    }
    return r;
}

void ImportarArticuloExt::do_ods()
{
    DlgImportODS d(0);
    d.exec();
}
