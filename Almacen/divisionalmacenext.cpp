#include "divisionalmacenext.h"
#include "../db_table_view.h"
#include "../Almacen/frmseccionesalmacen.h"
#include "../Almacen/frmfamiliasalmacen.h"
DivisionAlmacenExt::DivisionAlmacenExt(QObject *parent) :
    ModuleExtension(parent),
    Seccion("Secciones",this),
    familias("Familias",this),
    subFam("SubFamilias",this)
{
    int id = -1;
    tryRegisterModule(MayaModule::Almacen,"Secciones",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &Seccion;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::Almacen,"Familias",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &familias;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::Almacen,"SubFamilias",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &subFam;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    connect(&Seccion,SIGNAL(triggered()),this,SLOT(handle_actions()));
    connect(&familias,SIGNAL(triggered()),this,SLOT(handle_actions()));
    connect(&subFam,SIGNAL(triggered()),this,SLOT(handle_actions()));
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

void DivisionAlmacenExt::handle_actions()
{
    if(sender()== &Seccion)
      {
        frmSeccionesAlmacen form(qApp->activeWindow());
          form.exec();
      }
      else if (sender() == &familias)
      {
          frmFamiliasAlmacen form(qApp->activeWindow());
          form.exec();
      }
      else if (sender() == &subFam)
      {
          Db_table_View form(qApp->activeWindow());
          form.set_db("Maya");
          form.set_table("subfamilias");

          form.setWindowTitle(tr("Subfamilias"));

          QStringList headers;
          headers << tr("SubFamilia") << tr("Pertenece a");
          form.set_table_headers(headers);
                                   //tabla a buscar, columna relacionada y lo que quiero mostar
          form.set_relation(2,QSqlRelation("familias","id","familia"));

          form.set_columnHide(0);
          form.exec();
      }
}
