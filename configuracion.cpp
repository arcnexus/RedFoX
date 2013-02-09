#include "configuracion.h"
#include <QLineEdit>
Configuracion::Configuracion(QObject* parent) :
    QObject(parent)
{
    iva_model = 0;
    paises_model = 0;
    validator_cantidad = new QDoubleValidator(-99999999999999999.00,99999999999999999.00,2,this);
    validator_porciento = new QDoubleValidator(0,100,2,this);
}


QString Configuracion::FormatoNumerico(QString cTexto)
{
    QString aux = cTexto;
    aux.remove(",");
    aux.remove(".");
    aux.remove("-");
    aux.remove("$");
    aux.remove("€");
    aux.remove("£");
    bool is_num;
    aux.toDouble(&is_num);
    if(is_num)
    {
        int last_dot = cTexto.lastIndexOf(".");
        int last_comma = cTexto.lastIndexOf(",");
        int pos = qMax(last_dot,last_comma);

        QString ret = "";

        int inicio = -1;
        if(cTexto.contains("-"))
            inicio++;

        if(pos!=-1)
        {
            ret.prepend(cTexto.mid(pos));
            pos--;
        }

        int b = 0;
        for (int i = pos;i>inicio;i--)
        {
            if(!((cTexto.at(i)!='.') && (cTexto.at(i)!= ',')))
                continue;

            ret.prepend(cTexto.at(i));

            if((((b+1)%3) == 0)&& (i!=inicio+1))
                ret.prepend(".");
            b++;
        }
        if(cTexto.contains("-"))
            ret.prepend("-");

        return ret;
    }
    else
        return "";
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

void Configuracion::Cargar_iva()
{
    ivas.clear();
    QSqlQuery query(QSqlDatabase::database("terra"));
    if(query.exec("SELECT * FROM tiposiva"))
    {
        while(query.next())
        {
            QString key = query.record().value("cTipo").toString();
            ivas.insert(key,query.record());
        }
    }
    if(iva_model == 0)
        iva_model = new QSqlTableModel(this,QSqlDatabase::database("terra"));
    iva_model->setTable("tiposiva");
    iva_model->select();
}

void Configuracion::Cargar_paises()
{
    paises.clear();
    QSqlQuery query(QSqlDatabase::database("terra"));
    if(query.exec("SELECT * FROM paises"))
    {
        while(query.next())
        {
            QString key = query.record().value("cTipo").toString();
            paises.insert(key,query.record());
        }
    }
    if(paises_model == 0)
        paises_model = new QSqlTableModel(this,QSqlDatabase::database("terra"));
    paises_model->setTable("paises");
    paises_model->select();
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
    if (primerdigito == 11)
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

void Configuracion::format_text()
{
    QLineEdit * lineEdit = qobject_cast<QLineEdit*>(sender());
    if(lineEdit)
    {
        lineEdit->blockSignals(true);        
        lineEdit->setText(FormatoNumerico(lineEdit->text()));
        QApplication::processEvents();
        lineEdit->blockSignals(false);
    }
}






