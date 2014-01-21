#include "divisionalmacenext.h"
#include "../db_table_view.h"
#include "../Almacen/frmseccionesalmacen.h"
DivisionAlmacenExt::DivisionAlmacenExt(QObject *parent) :
    ModuleExtension(parent),
    Seccion("Secciones",this),
    familias("Familias",this),
    subFam("SubFamilias",this),
    subsubFam("SubSubFamilias",this),
    grupos("Grupos",this)
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

    tryRegisterModule(MayaModule::Almacen,"SubSubFamilias",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &subsubFam;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    tryRegisterModule(MayaModule::Almacen,"Grupos",id);
    if(userHaveAcess(id,Configuracion_global->id_usuario_activo))
    {
        QPair<QAction *, MayaModule::accessLevel> p;
        p.first = &grupos;
        p.second = getUserLvl(id);
        _actions.append(p);
    }

    connect(&Seccion,SIGNAL(triggered()),this,SLOT(handle_actions()));
    connect(&familias,SIGNAL(triggered()),this,SLOT(handle_actions()));
    connect(&subFam,SIGNAL(triggered()),this,SLOT(handle_actions()));
    connect(&subsubFam,SIGNAL(triggered()),this,SLOT(handle_actions()));
    connect(&grupos,SIGNAL(triggered()),this,SLOT(handle_actions()));
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
          Db_table_View form(qApp->activeWindow());
          form.set_db("Maya");
          form.set_table("familias");

          form.setWindowTitle(tr("Familias"));

          QStringList headers;
          headers << tr("Codigo") << tr("Familia") << tr("Pertenece a");
          form.set_table_headers(headers);

          form.set_relation(3,QSqlRelation("secciones","id","seccion"));

          form.set_columnHide(0);
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
      else if (sender() == &subsubFam)
      {
          Db_table_View form(qApp->activeWindow());
          form.set_db("Maya");
          form.set_table("subsubfamilias");

          form.setWindowTitle(tr("SubSubfamilias"));

          QStringList headers;
          headers << tr("SubSubFamilia") << tr("Pertenece a");
          form.set_table_headers(headers);

          form.set_relation(2,QSqlRelation("subfamilias","id","subfamilia"));

          form.set_columnHide(0);
          form.exec();
      }
      else if (sender() == &grupos)
      {
          Db_table_View form(qApp->activeWindow());
          form.set_db("Maya");
          form.set_table("grupoart");

          form.setWindowTitle(tr("Grupo de Articulos"));

          QStringList headers;
          headers << tr("Grupo") << tr("Pertenece a");
          form.set_table_headers(headers);

          form.set_relation(2,QSqlRelation("subsubfamilias","id","subsubfamilia"));

          form.set_columnHide(0);
          form.exec();
    }
}
