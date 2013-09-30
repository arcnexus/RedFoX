#include "refresca_ofertas.h"

refresca_ofertas::refresca_ofertas(QObject *parent) :
    QThread(parent)
{

}

void refresca_ofertas::run()
{
    while(true)
    {
        this->modelo->setQuery("select id,descripcion from articulos where mostrar_en_cuadro = 1",Configuracion_global->groupDB);
                sleep(1);
    }
}
