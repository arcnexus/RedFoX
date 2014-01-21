#include "cajas.h"

cajas::cajas(QObject *parent) :
    QObject(parent)
{
    h_cajas["id"] = 0;
    h_cajas["desc_caja"] = "";
}

bool cajas::anadir()
{

}

bool cajas::guardar(QString nuevo)
{

}

bool cajas::recuperar(QStringList filtro, QStringList extras)
{

}

bool cajas::borrar()
{

}

bool cajas::vaciar()
{
    h_cajas["id"] = 0;
    h_cajas["desc_caja"] = "";
}
