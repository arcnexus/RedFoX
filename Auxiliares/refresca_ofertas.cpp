#include "refresca_ofertas.h"

refresca_ofertas::refresca_ofertas(QObject *parent) :
    QThread(parent)
{

}

void refresca_ofertas::run()
{
    QSqlDatabase groupDB = QSqlDatabase::addDatabase(Configuracion_global->group_Driver , "Thread");

    groupDB.setHostName(Configuracion_global->group_host);
    groupDB.setUserName(Configuracion_global->group_user);
    groupDB.setPassword(Configuracion_global->group_pass);
    groupDB.setPort(Configuracion_global->group_port);

    if(groupDB.open())
    {
        while(true)
        {
            sleep(10);
            this->model->setQuery("select id,descripcion from articulos where mostrar_en_cuadro = 1",groupDB);
            this->model->setHeaderData(1,Qt::Horizontal,tr("Artículo"));
            tabla->setModel(model);
            tabla->setColumnHidden(0,true);
            //       sleep(28);
            qDebug() << "ascbhnjkl";

        }
    }
}
