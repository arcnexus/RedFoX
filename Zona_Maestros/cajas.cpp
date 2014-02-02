#include "cajas.h"

cajas::cajas(QObject *parent) :
    QObject(parent)
{
    h_cajas["id"] = 0;
    h_cajas["desc_caja"] = "";
}

bool cajas::anadir()
{
return false;
}

bool cajas::guardar(QString nuevo)
{
return false;
}

bool cajas::recuperar(QStringList filtro, QStringList extras)
{
return false;
}

bool cajas::borrar()
{
return false;
}

bool cajas::vaciar()
{
    h_cajas["id"] = 0;
    h_cajas["desc_caja"] = "";
    return false;
}
