#include "configuracion.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>


Configuracion::Configuracion()
{
}


QString Configuracion::FormatoNumerico(QString texto)
{
    // Cambio . por , o por signo elegido por los usuarios
    int tamano = texto.length();
    int posDec;
    posDec = tamano -3;
    if ((texto.midRef(posDec,1) !=".") && (texto.midRef(posDec,1) != ",")) {
        texto.append(",00") ;
    }

    if (texto.midRef(posDec,1)==".") {
        texto.replace(posDec,1,",");
   }
    //qDebug() << "Tamaño: " << tamano << "PosDec" <<posDec << texto.midRef(posDec,1);

    if (texto.length()==14) {
        texto.insert(2,".");
        texto.insert(6,".");
        texto.insert(10,".");
    }
    if (texto.length()==13) {
        texto.insert(1,".");
        texto.insert(5,".");
        texto.insert(9,".");
    }
    if (texto.length()==12) {
        texto.insert(3,".");
        texto.insert(7,".");
    }
    if (texto.length()==11) {
        texto.insert(2,".");
        texto.insert(6,".");
    }
    if (texto.length()== 10) {
        texto.insert(1,".");
        texto.insert(5,".");
    }
    if (texto.length() == 9 ) {

        texto.insert(3, ".");
    }
    if (texto.length() == 8 ) {

        texto.insert(2, ".");
    }
    if (texto.length() == 7 ) {

        texto.insert(1, ".");
    }
    return texto;
}

bool Configuracion::EsNumero(QString texto)
{
    //Para buscar un caracter en una posición determinada de una cadena:
    //str.indexOf(“caracter”,posicion);
       int tamano = texto.size();
       tamano --;
       QString cadena = "-0123456789,.";
       int i =0;
       do {
           if (!cadena.contains(texto.at(i), Qt::CaseInsensitive)) {
               return false;
           }
           i++;
       } while (i<=tamano);
       return true;
}






