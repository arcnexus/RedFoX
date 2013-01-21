#include "configuracion.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

Configuracion::Configuracion()
{
}


QString Configuracion::FormatoNumerico(QString cTexto)
{
    // Cambio . por , o por signo elegido por los usuarios
    int tamano = cTexto.length();
    int posDec;
    posDec = tamano -3;
    if ((cTexto.midRef(posDec,1) !=".") && (cTexto.midRef(posDec,1) != ",") &&  (cTexto.midRef((posDec+1),1) !=".")
            &&  (cTexto.midRef((posDec+1),1) !=".")) {

        cTexto.append(",00") ;
    } else {
        if ((cTexto.midRef((posDec+1),1) =="."))
            cTexto.append("0");
    }
    cTexto.replace(".",",");
    //qDebug() << "Tamaño: " << tamano << "PosDec" <<posDec << cTexto.midRef(posDec,1);

    if (cTexto.length()==14) {
        cTexto.insert(2,".");
        cTexto.insert(6,".");
        cTexto.insert(10,".");
    }
    if (cTexto.length()==13) {
        cTexto.insert(1,".");
        cTexto.insert(5,".");
        cTexto.insert(9,".");
    }
    if (cTexto.length()==12) {
        cTexto.insert(3,".");
        cTexto.insert(7,".");
    }
    if (cTexto.length()==11) {
        cTexto.insert(2,".");
        cTexto.insert(6,".");
    }
    if (cTexto.length()== 10) {
        cTexto.insert(1,".");
        cTexto.insert(5,".");
    }
    if (cTexto.length() == 9 ) {

        cTexto.insert(3, ".");
    }
    if (cTexto.length() == 8 ) {

        cTexto.insert(2, ".");
    }
    if (cTexto.length() == 7 ) {

        cTexto.insert(1, ".");
    }
    return cTexto;
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

void Configuracion::CargarDatos()
{
    QSettings settings("infint", "terra");
    this->cDriverBDTerra = settings.value("cDriverBDTerra").toString();
    this->cRutaBdTerra = settings.value("cRutaDBTerra").toString();
    this->cHostBDTerra = settings.value("cHostBDTerra").toString();
    this->cUsuarioBDTerra  =   settings.value("cUserBDTerra").toString();
    this->cPasswordBDTerra = settings.value("cPasswordBDTerra").toString();
    this->cPais = settings.value("cPais").toString();
    this->cEjercicio = settings.value("cEjercicioActivo").toString();
    this->nDigitosFactura = settings.value(("nDigitosFactura")).toInt();
    this->nIVA1 = settings.value("nIVA1").toDouble();
    this->nIVA2 = settings.value("nIVA2").toDouble();
    this->nIVA3 = settings.value("nIVA3").toDouble();
    this->nIVA4 = settings.value("nIVA4").toDouble();
    this->nRE1 = settings.value("nRE1").toDouble();
    this->nRE2 = settings.value("nRE2").toDouble();
    this->nRE3 = settings.value("nRE3").toDouble();
    this->nRE4 = settings.value("nRE4").toDouble();
    if(settings.value("lProfesional").toInt()==1)
        this->lProfesional = true;
    else
        this->lProfesional = false;
    this->nIRPF = settings.value("nIRPF").toInt();
    this->cSerie = settings.value("cSerie").toString();
    this->nDigitosCuentasContables = settings.value("nDigitosCuentas").toInt();
    this->cCuentaClientes = settings.value("cCuentaClientes").toString();
    this->cCuentaAcreedores = settings.value("cCuentaAcreedores").toString();
    this->cCuentaProveedores = settings.value("cCuentaProveedores").toString();

}

QString Configuracion::ValidarCC(QString Entidad, QString Oficina, QString DC, QString CC)
{
    // Código Entidad & Código Oficina
    QString cEntidadOficina = Entidad + Oficina;
    int longitud = cEntidadOficina.length();
    if (longitud != 8)
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Verificación cuenta bancaria"),
                             QObject::tr("El Codigo  de Entidad más  el  Codigo  de Oficina debe tener 8 dígitos."),
                        QObject::tr("Aceptar"));
    int suma = 0;
    suma = (cEntidadOficina.mid(0, 1).toInt() * 4);
    suma = suma + (cEntidadOficina.mid(1, 1).toInt() * 8);
    suma = suma + (cEntidadOficina.mid(2, 1).toInt() * 5);
    suma = suma + (cEntidadOficina.mid(3, 1).toInt() * 10);
    suma = suma + (cEntidadOficina.mid(4, 1).toInt() * 9);
    suma = suma + (cEntidadOficina.mid(5, 1).toInt() * 7);
    suma = suma + (cEntidadOficina.mid(6, 1).toInt() * 3);
    suma = suma + (cEntidadOficina.mid(7, 1).toInt() * 6);
    int resto = suma % 11;
    int primerdigito = 11 - resto;
    if (primerdigito == 10)
            primerdigito = 1;
    if (primerdigito == 11)
        primerdigito = 0;

    longitud = CC.length();
    if(longitud != 10)
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Verificación cuenta bancaria"),
                             QObject::tr("El  numero de cuenta debe tener 10 dígitos."),
                        QObject::tr("Aceptar"));
    suma = CC.mid( 0, 1).toInt() * 1 + CC.mid( 1, 1).toInt() * 2 + CC.mid( 2, 1).toInt() * 4 + CC.mid(3, 1).toInt() * 8;
    suma = suma + CC.mid(4, 1).toInt() * 5 + CC.mid(5, 1).toInt() * 10 + CC.mid(6, 1).toInt() * 9 + CC.mid( 7, 1).toInt() * 7;
    suma = suma + CC.mid(8, 1).toInt() * 3 + CC.mid(9, 1).toInt() * 6;
    resto = suma%11;
    int segundodigito = 11 - resto;
    if (segundodigito == 10)
        segundodigito = 1;
    if (segundodigito == 11)
        segundodigito = 0;

    //Digitos de Control
    QString cdc = QString::number(primerdigito) + QString::number(segundodigito);
    if(cdc == DC)
        return "1";
    else
        return "0";

}

QString Configuracion::ValidarCC(QString Entidad, QString Oficina, QString CC)
{
    // Código Entidad & Código Oficina
    QString cEntidadOficina = Entidad + Oficina;
    int longitud = cEntidadOficina.length();
    if (longitud != 8)
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Verificación cuenta bancaria"),
                             QObject::tr("El Codigo  de Entidad más  el  Codigo  de Oficina debe tener 8 dígitos."),
                        QObject::tr("Aceptar"));
    int suma;
    suma = cEntidadOficina.mid(1, 1).toInt() * 4 + cEntidadOficina.mid(2, 1).toInt() * 8 + cEntidadOficina.mid(3, 1).toInt() * 5 +
            cEntidadOficina.mid(4, 1).toInt() * 10;
    suma = suma + cEntidadOficina.mid(5, 1).toInt() * 9 + cEntidadOficina.mid(6, 1).toInt() * 7 + cEntidadOficina.mid(7, 1).toInt() * 3 +
            cEntidadOficina.mid(8, 1).toInt() * 6;
    int resto = suma % 11;
    int primerdigito = 11 - resto;
    if (primerdigito == 10)
            primerdigito = 1;
    if (primerdigito = 11)
        primerdigito = 0;

    longitud = CC.length();
    if(longitud != 10)
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Verificación cuenta bancaria"),
                             QObject::tr("El  numero de cuenta debe tener 10 dígitos."),
                        QObject::tr("Aceptar"));
    suma = CC.mid( 1, 1).toInt() * 1 + CC.mid( 2, 1).toInt() * 2 + CC.mid( 3, 1).toInt() * 4 + CC.mid(4, 1).toInt() * 8;
    suma = suma + CC.mid(5, 1).toInt() * 5 + CC.mid(6, 1).toInt() * 10 + CC.mid(7, 1).toInt() * 9 + CC.mid( 8, 1).toInt() * 7;
    suma = suma + CC.mid(9, 1).toInt() * 3 + CC.mid(10, 1).toInt() * 6;
    resto = suma%11;
    int segundodigito = 11 - resto;
    if (segundodigito == 10)
        segundodigito = 1;
    if (segundodigito == 11)
        segundodigito = 0;

    //Digitos de Control
    QString cdc = QString::number(primerdigito) + QString::number(segundodigito);
    return cdc;
}






