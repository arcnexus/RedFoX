#include "refresca_ofertas.h"

refresca_ofertas::refresca_ofertas(QObject *parent) :
    QThread(parent)
{

}

void refresca_ofertas::run()
{
    while(true)
    {
        sleep(2);
        this->model->setQuery("select id,descripcion from articulos where mostrar_en_cuadro = 1",Configuracion_global->groupDB);
        this->model->setHeaderData(1,Qt::Horizontal,tr("Artículo"));
        tabla->setModel(model);
        tabla->setColumnHidden(0,true);
        sleep(28);

    }
}
