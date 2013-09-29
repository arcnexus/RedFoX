#include "refresca_ofertas.h"

refresca_ofertas::refresca_ofertas(QObject *parent) :
    QThread(parent)
{

}

void refresca_ofertas::run()
{
    QSqlQueryModel * model = new QSqlQueryModel(this);
    model->setQuery("select id,descripcion from articulos where mostrar_en_cuadro = 1",Configuracion_global->groupDB);
}
