#include "Auxiliares/Globlal_Include.h"
#include "EditorReports/reportrenderer.h"

//#include "SOAP/soapcheckVatBindingProxy.h"
//#include "SOAP/checkVatBinding.nsmap"

#include <QLineEdit>
#include <QtNetwork/QSslConfiguration>
#include <QSslError>
#include <QPrintDialog>

#include "Auxiliares/SMPT/SmtpMime"

#include "../ODSlib/ods.h"

#include "../Crypto/crypto.h"

Configuracion::Configuracion(QObject* parent) :
    QObject(parent)
{

    iva_model = new QSqlQueryModel(this);
    paises_model = new QSqlQueryModel(this);
    formapago_model = new QSqlQueryModel(this);
    divisas_model = new QSqlQueryModel(this);
    agentes_model = new QSqlQueryModel(this);
    idiomas_model = new QSqlQueryModel(this);
    tarifas_model = new QSqlQueryModel(this);
    grupos_gasto_model = new QSqlQueryModel(this);
    client_model = 0;
    usuarios_model = 0;
    validator_cantidad = new QDoubleValidator(-99999999999999999.00,99999999999999999.00,2,this);
    validator_porciento = new QDoubleValidator(0,100,2,this);
    grupo_iva << tr("Nacional") << tr("Europeo") << tr("canarias") << tr("Internacional");
}

void Configuracion::transaction()
{
    this->empresaDB.transaction();
    this->groupDB.transaction();
    if(this->contabilidad)
        this->contaDB.transaction();
}

void Configuracion::rollback()
{
    this->empresaDB.rollback();
    this->groupDB.rollback();
    if(this->contabilidad)
        this->contaDB.rollback();
}

void Configuracion::commit()
{
    this->empresaDB.commit();
    this->groupDB.commit();
    if(this->contabilidad)
        this->contaDB.commit();
}

QString Configuracion::Devolver_idioma(int id)
{
    return search_string(idiomas_model,id,"idioma");
}

QString Configuracion::toFormatoMoneda(QString cTexto)
{
    if(cTexto.isEmpty())
        return "0,00";
    else
    {
        QString aux = cTexto;
        int decimals = Configuracion_global->decimales;
        aux.remove(",");
        aux.remove(".");
        //aux.remove("-");
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
                if(ret.left(1)==".")
                    ret =ret.replace(".",",");
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

            if(!ret.contains(","))
                ret = cTexto+",00";
    //        if(ret.right(2)==",1" || ret.right(2)==",2" || ret.right(2)==",3" || ret.right(2)==",4" || ret.right(2)==",5"
    //                || ret.right(2)==",6" || ret.right(2)==",7" || ret.right(2)==",8" || ret.right(2)==",9")
    //            ret = ret+"0";
            if(ret.right(decimals)==",1" || ret.right(decimals)==",2" || ret.right(decimals)==",3" || ret.right(decimals)==",4"
                    || ret.right(decimals)==",5" || ret.right(decimals)==",6" || ret.right(decimals)==",7"
                    || ret.right(decimals)==",8" || ret.right(decimals)==",9")
                ret = ret+"0";
            if(!mostrar_siempre){
                if (ret.right(decimals-2) == "0" || ret.right(decimals-2) == "00" || ret.right(decimals-2) == "000")
                    ret = ret.left(ret.size()-(decimals -2));
                if (ret == "0,")
                    ret = "0,00";
                if (ret.right(1) == ",")
                    ret.append("00");
            }
            return ret;
        }
        else
            return "";
    }
}

double Configuracion::MonedatoDouble(QString moneda)
{
    bool negative;
    negative = false;
    if(!moneda.contains(",") && !moneda.contains("."))
        moneda.append(",00");
    if(moneda.isEmpty() || moneda == "0,00")
        return 0.00;

    moneda = moneda.replace(".","").replace(",",".");

    double dblMoneda;
    int d;
    d= Configuracion_global->decimales;
    if(d>2)
    {
        if (d==3)
        {
            if(moneda.right(1) =="5")
                moneda = moneda.left(moneda.size()-1).append("6");
        } else if(d==4)
        {
            if(moneda.right(2)=="50")
                moneda = moneda.left(moneda.size()-2).append("51");

        }
    }
    dblMoneda  = floor(moneda.toDouble() * pow(10., d) + .5) / pow(10., d);
    return dblMoneda;
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
    static int tries = 0;
    tries++;
    if(tries == 5)
    {
        //qFatal(tr("Fue imposible crear tipos de iva").toAscii().constData());
        tries=0;
        return;
    }
    ivas.clear();
    ivaList.clear();
    reList.clear();

    QSqlQuery query(Configuracion_global->groupDB);
    if(query.exec("SELECT * FROM tiposiva"))
    {
        while(query.next())
        {
            QString key = query.record().value("tipo").toString();
            ivas.insert(key,query.record());
        }
    }
    if(ivas.size()!=4)
    {
        query.prepare("INSERT INTO `tiposiva` (`id`, `nombre_interno`, `tipo`, `descripcion_tipo_iva`, `iva`, `recargo_equivalencia`) VALUES (1, 'base1', 'NORMAL', '21%', 21.00, 4.00), (2, 'base2', 'Reducido', '10%', 10.00, 2.00), (3, 'base3', 'Superreducido', '4%', 4.00, 1.00), (4, 'base4', 'Exento', 'Exento', 0.00, 0.00);");
        query.exec();
        qDebug() << query.lastError().text();
        return Cargar_iva();
    }

    iva_model->setQuery("SELECT * from `tiposiva`",Configuracion_global->groupDB);

    QString base1 , base2 , base3 ,base4;
    QString re1 , re2 , re3 , re4;
    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    for (int i = 0;i<keys.size();i++)
    {
        QString aux = Configuracion_global->ivas[keys.at(i)].value("nombre_interno").toString();
        if(aux == "base1")
        {
            base1 = Configuracion_global->ivas[keys.at(i)].value("iva").toString();
            re1 = Configuracion_global->ivas[keys.at(i)].value("recargo_equivalencia").toString();
        }
        else if(aux == "base2")
        {
            base2 = Configuracion_global->ivas[keys.at(i)].value("iva").toString();
            re2 = Configuracion_global->ivas[keys.at(i)].value("recargo_equivalencia").toString();
        }
        else if(aux == "base3")
        {
            base3 = Configuracion_global->ivas[keys.at(i)].value("iva").toString();
            re3 = Configuracion_global->ivas[keys.at(i)].value("recargo_equivalencia").toString();
        }
        else if(aux == "base4")
        {
            base4 = Configuracion_global->ivas[keys.at(i)].value("iva").toString();
            re4 = Configuracion_global->ivas[keys.at(i)].value("recargo_equivalencia").toString();
        }
    }
    ivaList << base1 << base2 << base3 << base4;
    reList << re1 << re2 << re3 << re4;
}

int Configuracion::getidIva(QString cIva)
{
    return _search(iva_model,cIva,"tipo","id").toInt();
}

QString Configuracion::setTipoIva(int idIva)
{
    return search_string(iva_model,idIva,"tipo");
}

void Configuracion::CargarDatosMaestros()
{
    Cargar_iva();
    Cargar_paises();
    Cargar_divisas();
    Cargar_formas_pago();
    Cargar_agentes();
    Cargar_idiomas();
    Cargar_tarifas();
    Cargar_gastos();
}

void Configuracion::Cargar_paises()
{
    paises_model->setQuery("select id,pais from paises order by pais",Configuracion_global->groupDB);
}

void Configuracion::Cargar_divisas()
{
    divisas_model->setQuery("select * from monedas order by moneda",Configuracion_global->groupDB);
}

void Configuracion::Cargar_formas_pago()
{
    formapago_model->setQuery("select * from formpago order by forma_pago",Configuracion_global->groupDB);
}

void Configuracion::Cargar_agentes()
{
    agentes_model->setQuery("SELECT * FROM agentes;",Configuracion_global->groupDB);
}

void Configuracion::Cargar_idiomas()
{
    idiomas_model->setQuery("select * from idiomas",Configuracion_global->groupDB);
}

void Configuracion::Cargar_tarifas()
{
    tarifas_model->setQuery("Select * from codigotarifa",Configuracion_global->groupDB);
}

void Configuracion::Cargar_gastos()
{
    grupos_gasto_model->setQuery("Select * from grupos_gasto",Configuracion_global->groupDB);
}

int Configuracion::Devolver_id_tarifa(QString nombre)
{
    return _search(tarifas_model,nombre,"descripcion","id").toInt();
}

int Configuracion::Devolver_id_moneda(QString nombre)
{
    return _search(divisas_model,nombre,"moneda","id").toInt();
}

QString Configuracion::Devolver_pais(int id)
{
    return search_string(paises_model,id,"pais");
}

QString Configuracion::Devolver_codigo_forma_pago(int id)
{
    return search_string(formapago_model,id,"codigo");
}

QString Configuracion::Devolver_moneda(int id)
{
    return search_string(divisas_model,id,"moneda");
}

QString Configuracion::Devolver_codDivisa(int id)
{
    return search_string(divisas_model,id,"nombre_corto");
}

QString Configuracion::Devolver_descripcion_tipo_iva(int id)
{
    return search_string(iva_model,id,"tipo");
}

int Configuracion::Devolver_id_pais(QString nombre)
{
    return _search(paises_model,nombre,"pais","id").toInt();
}

QString Configuracion::Devolver_tarifa(int id)
{
    return search_string(tarifas_model,id,"descripcion");
}

QString Configuracion::Devolver_tipo_gasto(int id_gasto)
{
    return search_string(grupos_gasto_model,id_gasto,"descripcion");
}

int Configuracion::Devolver_id_tipo_gasto(QString desc)
{
    return _search(grupos_gasto_model,desc,"descripcion","id").toInt();
}

int Configuracion::Devolver_id_cuenta_contable(QString codigo_cta)
{
    QSqlQuery queryCuenta(Configuracion_global->contaDB);
    if(queryCuenta.exec("select id from plan_general where codigo_cta = '"+codigo_cta+"'"))
    {
        queryCuenta.next();
        return queryCuenta.record().value("id").toInt();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar identificador cuenta contable"),
                             tr("Fallo la recuperación del identificador: ")+queryCuenta.lastError().text(),
                             tr("Aceptar"));
    return 0;
}


QString Configuracion::Devolver_descripcion_cuenta_contable(QString codigo_cta)
{
    QSqlQuery queryCuenta(Configuracion_global->contaDB);
    if(queryCuenta.exec("select descripcion from plan_general where codigo_cta = '"+codigo_cta+"'"))
    {
        queryCuenta.next();
        return queryCuenta.record().value("descripcion").toString();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar cuenta contable"),
                             tr("Fallo la recuperación de la descripción de la cuenta: ")+queryCuenta.lastError().text(),
                             tr("Aceptar"));
    return "";
}

QString Configuracion::Devolver_codigo_cta_contable(int id)
{
    QSqlQuery queryCuenta(Configuracion_global->contaDB);
    if(queryCuenta.exec("select codigo_cta from plan_general where id = "+QString::number(id)))
    {
        queryCuenta.next();
        return queryCuenta.record().value("codigo_cta").toString();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar cuenta contable"),
                             tr("Fallo al recuperar el código de la cuenta: ")+queryCuenta.lastError().text(),
                             tr("Aceptar"));
    return ""; //FIXME terminar Configuracion::Devolver_codigo_cta_contable(int id)
}

QString Configuracion::toRound(double dnumber, int decimals)
{
    QString StrNumber = QString::number(dnumber,'f',decimals);
    QString StrNumber_ = QString:: number(dnumber,'f',decimals+1);
    if(StrNumber_.right(1) == "5")
    {
        double dblNumber = StrNumber_.toDouble();
        switch (decimals) {
        case 1:
            dblNumber += 0.1;
            break;
        case 2:
            dblNumber += 0.01;
            break;
        case 3:
            dblNumber += 0.001;
            break;
        case 4:
            dblNumber += 0.0001;
        default:
            break;
        }
         StrNumber = QString::number(dblNumber,'f',decimals);
    }

    return toFormatoMoneda(StrNumber);
}


QString Configuracion::devolver_transportista(int id)
{
    QSqlQuery query(Configuracion_global->groupDB);
    if(query.exec("select transportista from transportista where id = "+QString::number(id)))
    {
        query.next();
        return query.record().value("transportista").toString();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar transportista"),
                             tr("Fallo al recuperar el transportista")+query.lastError().text(),
                             tr("Aceptar"));
    return ""; //FIXME terminar Configuracion::devolver_transportista(int id)

}

int Configuracion::devolver_id_transportista(QString transportista)
{
    QSqlQuery query(Configuracion_global->groupDB);
    if(query.exec("select id from transportista where transportista = '"+transportista+"'"))
    {
        query.next();
        return query.record().value("id").toInt();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar identificador transportista"),
                             tr("Fallo la recuperación del identificador: ")+query.lastError().text(),
                             tr("Aceptar"));
    return 0; //FIXME terminar devolver_id_transportista(QString transportista)
}

int Configuracion::devolver_id_tabla(QSqlQueryModel *model, QModelIndex index)
{
    int id = model->data(model->index(index.row(),0),Qt::EditRole).toInt();
    return id;
}

void Configuracion::CargarClientes()
{
    if(client_model == 0)
        client_model = new QSqlRelationalTableModel(this,Configuracion_global->groupDB);
    client_model->setTable("clientes");
    client_model->select();
}

void Configuracion::CargarUsuarios()
{
    if(usuarios_model == 0)
        usuarios_model = new QSqlQueryModel(this);
    usuarios_model->setQuery("SELECT * from redfoxglobal.usuarios",Configuracion_global->globalDB);
}

bool Configuracion::CargarDatosBD()
{
    QFile f(qApp->applicationDirPath()+"/MayaConfig.ini");
    if(!f.exists())
        return false;
    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
    this->global_driver = settings.value("cDriverBDMaya").toString();
    this->global_routeLite = settings.value("cRutaDBMaya").toString();
    this->global_host = settings.value("cHostBDMaya").toString();
    this->global_user  =   DeCrypt(settings.value("cUserBDMaya").toString());
    this->global_pass = DeCrypt(settings.value("cPasswordBDMaya").toString());
    this->global_port = settings.value("global_port").toInt();

    this->nombre_bdTiendaWeb = settings.value("nombre_bdTiendaWeb").toString();
    this->cHostBDTiendaWeb = settings.value("hostTiendaWeb").toString();
    this->cUsuarioTiendaWeb = DeCrypt(settings.value("usuarioTiendaWeb").toString());
    this->cPasswordTiendaWeb = DeCrypt(settings.value("Pass_web").toString());
    this->cPuertoTiendaWeb =settings.value("puertoTiendaWeb").toString();
    this->enlace_web = settings.value("enlace_web").toBool();


    this->HostDB_MediTec = settings.value("HostDB_MediTec").toString();
    this->NombreDB_MediTec = settings.value("NombreDB_MediTec").toString();
    this->UsuarioDB_MediTec = settings.value("UsuarioDB_MediTec").toString();
    this->PasswordDB_MediTec = settings.value("PasswordDB_MediTec").toString();
    this->PortDB_MediTec =  settings.value("PuertoDB_MediTec").toString();

    this->globalDB = QSqlDatabase::addDatabase(Configuracion_global->global_driver,"Global");

    if (this->global_driver == "QSQLITE")
    {
        this->globalDB.setDatabaseName(Configuracion_global->global_routeLite);
        this->globalDB.open();
    }
    else
    {
        this->globalDB.setHostName(Configuracion_global->global_host);
        this->globalDB.open(Configuracion_global->global_user,Configuracion_global->global_pass);
    }

    bool _mayaglobal = true;
    if(this->globalDB.isOpen())
    {
        QSqlQuery q(globalDB);
        if(q.exec("SHOW DATABASES like 'redfoxglobal'"))
            _mayaglobal = q.next();
    }

    Configuracion_global->CargarUsuarios();

    return (!this->globalDB.lastError().isValid()) && _mayaglobal;
}

void Configuracion::AbrirDbWeb()
{
    //--------------
    // abrir BD WEB
    //--------------
    QSqlDatabase dbWeb = QSqlDatabase::addDatabase("QMYSQL","dbweb");
    dbWeb.setDatabaseName(Configuracion_global->nombre_bdTiendaWeb);
        dbWeb.setDatabaseName(Configuracion_global->nombre_bdTiendaWeb);
        dbWeb.setHostName(Configuracion_global->cHostBDTiendaWeb);
        dbWeb.setPassword(Configuracion_global->cPasswordTiendaWeb);
        dbWeb.setUserName(Configuracion_global->cUsuarioTiendaWeb);
        if(!dbWeb.open())
            QMessageBox::warning(qApp->activeWindow(),tr("Conectar con servidor web"),
                                 tr("Imposible abrir la Base de Datos del servidor: %1").arg(dbWeb.lastError().text()));

}

void Configuracion::AbridBDMediTec()
{
    // Abro bdInformación técnica de medicina
    QSqlDatabase DB_MediTec = QSqlDatabase::addDatabase("QMYSQL","db_meditec");

        DB_MediTec.setDatabaseName(Configuracion_global->NombreDB_MediTec);
        DB_MediTec.setHostName(Configuracion_global->HostDB_MediTec);
        DB_MediTec.open(Configuracion_global->UsuarioDB_MediTec,Configuracion_global->PasswordDB_MediTec);

    if (DB_MediTec.lastError().isValid())
    {
        QMessageBox::critical(qApp->activeWindow(), "error:", DB_MediTec.lastError().text());
    }
}

void Configuracion::CargarDatos(QSqlRecord r)
{
    this->pais = r.field("pais").value().toString();
    this->cEjercicio = r.field("ejercicio").value().toString();
    this->ndigitos_factura = r.field("digitos_factura").value().toInt();

    this->lProfesional = r.value("usar_irpf").toBool();

    this->serie = r.field("serie").value().toString();
    this->digitos_cuentas_contables = r./*field("digitos_cuenta").*/value("digitos_cuenta").toInt();
    this->cuenta_clientes = r.field("codigo_cuenta_clientes").value().toString();
    this->cuenta_acreedores = r.field("codigo_cuenta_acreedores").value().toString();
    this->cuenta_proveedores = r.field("codigo_cuenta_proveedores").value().toString();
    this->auto_codigoart = r.value("auto_codigo").toBool();
    this->tamano_codigo = r.value("tamano_codigo").toInt();
    this->cuenta_cobrosClientes = r.value("cuenta_cobros").toString();
    this->cuenta_pagosProveedor = r.value("cuenta_pagos").toString();
    this->cuenta_venta_mercaderias = r.value("cuenta_ventas_mercaderias").toString();
    this->cuenta_venta_servicios = r.value("cuenta_ventas_servicios").toString();
    this->cuenta_iva_repercutido1 = r.value("cuenta_iva_repercutido1").toString();
    this->cuenta_iva_repercutido2 = r.value("cuenta_iva_repercutido2").toString();
    this->cuenta_iva_repercutido3 = r.value("cuenta_iva_repercutido3").toString();
    this->cuenta_iva_repercutido4 = r.value("cuenta_iva_repercutido4").toString();
    this->cuenta_iva_soportado1 = r.value("cuenta_iva_soportado1").toString();
    this->cuenta_iva_soportado2 = r.value("cuenta_iva_soportado2").toString();
    this->cuenta_iva_soportado3 = r.value("cuenta_iva_soportado3").toString();
    this->cuenta_iva_soportado4 = r.value("cuenta_iva_soportado4").toString();
    this->cuenta_iva_repercutidoRe1 = r.value("cuenta_iva_repercutido1_re").toString();
    this->cuenta_iva_repercutidoRe2 = r.value("cuenta_iva_repercutido2_re").toString();
    this->cuenta_iva_repercutidoRe3 = r.value("cuenta_iva_repercutido3_re").toString();
    this->cuenta_iva_repercutidoRe4 = r.value("cuenta_iva_repercutido4_re").toString();
    this->cuenta_iva_soportado_re1 = r.value("cuenta_iva_soportado1_re").toString();
    this->cuenta_iva_soportado_re2 = r.value("cuenta_iva_soportado2_re").toString();
    this->cuenta_iva_soportado_re3 = r.value("cuenta_iva_soportado3_re").toString();
    this->cuenta_iva_soportado_re4 = r.value("cuenta_iva_soportado4_re").toString();
    this->medic = r.value("medica").toBool();
    this->internacional = r.value("internacional").toBool();
    this->margen = r.value("margen").toDouble();
    this->margen_minimo = r.value("margen_minimo").toDouble();
    this->activar_seguimiento = r.value("seguimiento").toBool();
    this->irpf = r.value("usar_irpf").toBool();
    this->caducidad_vales = r.value("caducidad_vales").toInt();
}

QString Configuracion::ValidarCC(QString Entidad, QString Oficina, QString DC, QString CC)
{
    // Código Entidad & Código Oficina
    QString entidadOficina = Entidad + Oficina;
    int longitud = entidadOficina.length();
    if (longitud != 8)
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Verificación cuenta bancaria"),
                             QObject::tr("El Codigo  de Entidad más  el  Codigo  de Oficina debe tener 8 dígitos."),
                        QObject::tr("Aceptar"));
    int suma = 0;
    suma = (entidadOficina.mid(0, 1).toInt() * 4);
    suma = suma + (entidadOficina.mid(1, 1).toInt() * 8);
    suma = suma + (entidadOficina.mid(2, 1).toInt() * 5);
    suma = suma + (entidadOficina.mid(3, 1).toInt() * 10);
    suma = suma + (entidadOficina.mid(4, 1).toInt() * 9);
    suma = suma + (entidadOficina.mid(5, 1).toInt() * 7);
    suma = suma + (entidadOficina.mid(6, 1).toInt() * 3);
    suma = suma + (entidadOficina.mid(7, 1).toInt() * 6);
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
    QString dc = QString::number(primerdigito) + QString::number(segundodigito);
    if(dc == DC)
        return "1";
    else
        return "0";

}

QString Configuracion::ValidarCC(QString Entidad, QString Oficina, QString CC)
{
    // Código Entidad & Código Oficina
    QString entidadOficina = Entidad + Oficina;
    int longitud = entidadOficina.length();
    if (longitud != 8)
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Verificación cuenta bancaria"),
                             QObject::tr("El Codigo  de Entidad más  el  Codigo  de Oficina debe tener 8 dígitos."),
                        QObject::tr("Aceptar"));
    int suma;
    suma = entidadOficina.mid(1, 1).toInt() * 4 + entidadOficina.mid(2, 1).toInt() * 8 + entidadOficina.mid(3, 1).toInt() * 5 +
            entidadOficina.mid(4, 1).toInt() * 10;
    suma = suma + entidadOficina.mid(5, 1).toInt() * 9 + entidadOficina.mid(6, 1).toInt() * 7 + entidadOficina.mid(7, 1).toInt() * 3 +
            entidadOficina.mid(8, 1).toInt() * 6;
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
    QString dc = QString::number(primerdigito) + QString::number(segundodigito);
    return dc;
}


QString Configuracion::letraDNI(QString Nif)
{
    if(Nif.isEmpty())
    {
        QMessageBox::critical(qApp->activeWindow(),tr("Internal error"),tr("DNI vacio"));
        return "";
    }

    if(Nif.at(0).isLetter())
        return Nif;

    int nSuma, nSumaPar, nSumaNon;
    Nif = Nif.toUpper();

    if (Nif.isEmpty())
        return ("");

    if (Nif[0].isDigit() || Nif.mid(1,1).contains("[KLMXYZ]"))  // Persona física
    {
        if (Nif.mid(1,1).contains("[XYZ]"))
            //Nif = iif(Nif.SubString(1, 1) == "Z", "2", iif(Nif.SubString(1, 1) == "Y", "1", "0")) + Nif.SubString(2, Nif.Length());
            Nif = Nif.mid(1,1) == "Z" ? "2" : Nif.mid(1,1) == "Y" ? "1" : "0";
        else if (Nif.mid(1,1).contains("[KLM]"))
            Nif = Nif.mid(2);
        QString Strlast = Nif.right(1);
        if (Strlast[0].isLetter())
            return Nif;
        QString x1 = "TRWAGMYFPDXBNJZSQVHLCKE";
        return (Nif +x1.mid(Nif.trimmed().toInt() % 23,1));
    }
    else      // Persona jurídica
    {
        if (Nif.mid(1,1).contains("[ABCDEFGHJNPQRSUVW]"))
            return (Nif);
        if (Nif.right(1).contains("[ABCDEFGHJNPQRSUVW]"))
            return (Nif);
        nSumaPar = 0;
        nSumaNon = 0;
        for (int nLetra = 2; nLetra < 9; nLetra += 2)
        {
            if (nLetra < 8)
                //nSumaPar += StrToInt(Nif.SubString(nLetra + 1, 1));
                nSumaPar += Nif.mid(nLetra + 1, 1).toInt();

            nSumaNon += ((2 * Nif.mid(nLetra, 1).toInt() % 10) + ((2 * Nif.mid(nLetra, 1).toInt())) / 10);
        }

        nSuma = nSumaPar + nSumaNon;
        nSuma = 10 - (nSuma % 10);
        if (nSuma == 10)
            nSuma = 0;

        if (!Nif.mid(1,1).contains("[ABDEFGHJUV]"))
        {
            const char x[] = {'A','B','C','D','E','F','G','H','I','J'};
            return (QString(Nif +x[nSuma]));
        }
        else
            return /*(FormatFloat("0", nSuma))*/QString::number(nSuma);
    }
}

bool Configuracion::comprobarNIF(QString country_code, QString nif)
{
    return true;/*
    _ns2__checkVat ns2__checkVat;
    QByteArray   code_bytes;
    QByteArray  nif_bytes = nif.toLatin1();
    ns2__checkVat.vatNumber = nif_bytes.data();

    QSqlQuery q(Configuracion_global->groupDB);
    QString sql = QString("SELECT * FROM paises where pais = '%1'").arg(country_code);
    q.prepare(sql);
    if(q.exec())
    {
        if(q.next())
        {
            QString code = q.record().value("countryCode").toString();
            code_bytes = code.toLatin1();
            ns2__checkVat.countryCode = code_bytes.data();
        }
        else return false;
    }

    checkVatBindingProxy cheker;

    _ns2__checkVatResponse ns2__checkVatResponse;

    if(cheker.checkVat(&ns2__checkVat,&ns2__checkVatResponse) == SOAP_OK)
    {
        return ns2__checkVatResponse.valid;
    }
    return false;*/
}

QString Configuracion::search_string(QSqlQueryModel *model, int id, QString column)
{
    for(auto i=0; i<model->rowCount(); ++i)
    {
        if(model->record(i).value("id").toInt() == id)
            return model->record(i).value(column).toString();
    }
    return "";
}

QVariant Configuracion::_search(QSqlQueryModel *model, QVariant search, QString column, QString output_column)
{
    for (auto i=0; i< model->rowCount(); ++i)
    {
        if(model->record(i).value(column).toString() == search)
            return model->record(i).value(output_column);
    }
    return QVariant();
}

QString Configuracion::Crypt(QString plaintext)
{
    return Crypto::Crypt(plaintext);
}

QString Configuracion::DeCrypt(QString ciphertext)
{
    return Crypto::DeCrypt(ciphertext);
}

QString Configuracion::SHA256HashString(QString input)
{
    return Crypto::SHA256HashString(input);
}

void Configuracion::getCambio(QString from, QString to, float cuanty)
{
    QString url = QString("https://www.google.com/finance/converter?a=%1&from=%2&to=%3")
            .arg(cuanty)
            .arg(from)
            .arg(to);
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(cambioReply(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(url)));
}

float Configuracion::getCambioBlock(QString from, QString to, float cuanty)
{
    QString url = QString("https://www.google.com/finance/converter?a=%1&from=%2&to=%3")
            .arg(cuanty)
            .arg(from)
            .arg(to);
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(cambioReplyBlock(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(url)));
    _block = true;
    while(_block)
    {
        QApplication::processEvents();
        QThread::msleep(100);
    }
    return _cambio;
}

void Configuracion::updateTablaDivisas(QString current)
{
    connect(this,SIGNAL(cambioReady(float,QString)),SLOT(applyCambio(float,QString)));
    QSqlQuery q(Configuracion_global->groupDB);
    if(q.exec("SELECT * FROM monedas"))
    {
        while(q.next())
            getCambio(current,q.record().value("nombre_corto").toString());
    }
}

void Configuracion::ImprimirDirecto(QString report, QMap<QString, QString> queryClausules , QMap<QString, QString> params)
{
    QString c = QString("report_name = '%1' ORDER BY report_grade DESC").arg(report);
    QString error;
    QString rep = SqlCalls::SelectOneField("report","report_source",c,Configuracion_global->empresaDB,error).toString();
    if(error.isEmpty())
    {
        ReportRenderer r;
        QPrinter printer;
        QPrintDialog dlg(&printer);

        if(dlg.exec()==QDialog::Rejected)
            return;

        r.setPrinter(&printer);
        QString errorStr;
        int errorLine;
        int errorColumn;
        QDomDocument doc;
        if (!doc.setContent(rep, false, &errorStr, &errorLine,
                            &errorColumn))
        {
            error = QString("Error: Parse error at line %1, column %2 : %3").arg(errorLine).arg(errorColumn).arg(errorStr);
            TimedMessageBox::Box(qApp->activeWindow(),error,TimedMessageBox::Critical);
            return;
        }
        bool error;
        r.render(&printer,doc ,queryClausules,params,error);
        r.Print(&printer);
    }
}

void Configuracion::ImprimirPDF(QString report, QMap<QString, QString> queryClausules,QMap<QString, QString> params)
{
    QString c = QString("report_name = '%1' ORDER BY report_grade DESC").arg(report);
    QString error;
    QString rep = SqlCalls::SelectOneField("report","report_source",c,Configuracion_global->empresaDB,error).toString();
    if(error.isEmpty())
    {
        ReportRenderer r;

        QPrinter printer(QPrinter::ScreenResolution);
        printer.setResolution(300);

#ifdef Q_WS_MAC
        printer.setOutputFormat( QPrinter::NativeFormat );
#else
        printer.setOutputFormat( QPrinter::PdfFormat );
#endif
        QString output = QFileDialog::getSaveFileName(qApp->activeWindow(),QString(),QString(),"PDF (*.pdf)");
        if(output.isEmpty())
            return;
        if(!output.endsWith(".pdf"))
            output.append(".pdf");
        printer.setOutputFileName( output );

        r.setPrinter(&printer);
        QString errorStr;
        int errorLine;
        int errorColumn;
        QDomDocument doc;
        if (!doc.setContent(rep, false, &errorStr, &errorLine,
                            &errorColumn))
        {
            error = QString("Error: Parse error at line %1, column %2 : %3").arg(errorLine).arg(errorColumn).arg(errorStr);
            TimedMessageBox::Box(qApp->activeWindow(),error,TimedMessageBox::Critical);
            return;
        }
        bool error;
        r.render(&printer,doc ,queryClausules,params,error);
        r.Print(&printer);
    }
}

void Configuracion::ImprimirPreview(QString report, QMap<QString, QString> queryClausules, QMap<QString, QString> params)
{
    QString c = QString("report_name = '%1' ORDER BY report_grade DESC").arg(report);
    QString error;
    QString rep = SqlCalls::SelectOneField("report","report_source",c,Configuracion_global->empresaDB,error).toString();
    if(error.isEmpty())
    {
        ReportRenderer r;
        QPrinter printer;
       // QPrintDialog dlg(&printer);
        r.setPrinter(&printer);
        QString errorStr;
        int errorLine;
        int errorColumn;
        QDomDocument doc;
        if (!doc.setContent(rep, false, &errorStr, &errorLine,
                            &errorColumn))
        {
            error = QString("Error: Parse error at line %1, column %2 : %3").arg(errorLine).arg(errorColumn).arg(errorStr);
            TimedMessageBox::Box(qApp->activeWindow(),error,TimedMessageBox::Critical);
            return;
        }
        bool error;
        r.render(&printer,doc ,queryClausules,params,error);
        QPrintPreviewDialog predlg(&printer);
        predlg.setWindowState(Qt::WindowMaximized);
        connect(&predlg,SIGNAL(paintRequested(QPrinter*)),&r,SLOT(Print(QPrinter*)));
        predlg.exec();
    }
}

void Configuracion::EviarMail(QString report, QMap<QString, QString> queryClausules, QMap<QString, QString> params,QString pdfName, QString dest_mail, QString dest_name , QString asunto , QString texto)
{
    QString c = QString("report_name = '%1' ORDER BY report_grade DESC").arg(report);
    QString error;
    QString rep = SqlCalls::SelectOneField("report","report_source",c,Configuracion_global->empresaDB,error).toString();
    if(error.isEmpty())
    {
        ReportRenderer r;

        QPrinter printer(QPrinter::ScreenResolution);
        printer.setResolution(300);

#ifdef Q_WS_MAC
        printer.setOutputFormat( QPrinter::NativeFormat );
#else
        printer.setOutputFormat( QPrinter::PdfFormat );
#endif
        QString output = qApp->applicationDirPath() + "/" + pdfName;
        if(output.isEmpty())
            return;
        if(!output.endsWith(".pdf"))
            output.append(".pdf");
        printer.setOutputFileName( output );

        r.setPrinter(&printer);
        QString errorStr;
        int errorLine;
        int errorColumn;
        QDomDocument doc;
        if (!doc.setContent(rep, false, &errorStr, &errorLine,
                            &errorColumn))
        {
            error = QString("Error: Parse error at line %1, column %2 : %3").arg(errorLine).arg(errorColumn).arg(errorStr);
            TimedMessageBox::Box(qApp->activeWindow(),error,TimedMessageBox::Critical);
            return;
        }
        bool error;
        r.render(&printer,doc ,queryClausules,params,error);
        r.Print(&printer);

        SmtpClient smtp(Configuracion_global->user_mail_smpt, Configuracion_global->user_mail_port, SmtpClient::SslConnection);

        smtp.setUser(Configuracion_global->user_mail_acc);
        smtp.setPassword(Configuracion_global->user_mail_pass);

        // Create a MimeMessage

        MimeMessage message;

        message.setSender(new EmailAddress(Configuracion_global->user_mail_acc, Configuracion_global->user_long_name));
        message.addRecipient(new EmailAddress(dest_mail, dest_name));
        message.setSubject(asunto);

        // Add some text

        MimeText text;
        text.setText(texto);
        message.addPart(&text);

        // Now we create the attachment object
        QFile * f = new QFile(output);
        message.addPart(new MimeAttachment(f));

        // Now we can send the mail

        if(smtp.connectToHost())
        {
            if(smtp.login())
            {
            if(!smtp.sendMail(message))
                TimedMessageBox::Box(qApp->activeWindow(),tr("Error al enviar mail: envio"),TimedMessageBox::Critical);
            smtp.quit();
            }
            else
              TimedMessageBox::Box(qApp->activeWindow(),tr("Error al enviar mail: datos de acceso"),TimedMessageBox::Critical);
        }
        else
          TimedMessageBox::Box(qApp->activeWindow(),tr("Error al enviar mail: conexion con el servidor"),TimedMessageBox::Critical);

        f->remove();
        f->deleteLater();
    }
}

bool Configuracion::SqlToODS(QString fileName, QString query, QSqlDatabase db, QStringList headers, QString &error)
{
    bool ok = ODS::SqlToODS(fileName, query, db , headers);
    error = ODS::lastError;
    return ok;
}

void Configuracion::format_text()
{
    QLineEdit * lineEdit = qobject_cast<QLineEdit*>(sender());
    if(lineEdit)
    {
        lineEdit->blockSignals(true);        
        lineEdit->setText(toFormatoMoneda(lineEdit->text()));
        QApplication::processEvents();
        lineEdit->blockSignals(false);
    }
}

void Configuracion::cambioReply(QNetworkReply *reply)
{
    reply->deleteLater();
    if(reply->error() == QNetworkReply::NoError)
        {
            // Get the http status code
            int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (v >= 200 && v < 300) // Success
            {
                // Here we got the final reply
                //QString replyText = reply->readAll();
                readCambio(reply->readAll());
            }
            else if (v >= 300 && v < 400) // Redirection
            {

                // Get the redirection url
                QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
                // Because the redirection url can be relative,
                // we have to use the previous one to resolve it
                newUrl = reply->url().resolved(newUrl);

                QNetworkAccessManager *manager = reply->manager();
                connect(manager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(onIgnoreSSLErrors(QNetworkReply*,QList<QSslError>)));
                connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(cambioReply(QNetworkReply*)));
                QNetworkRequest redirection(newUrl);
                manager->get(redirection);
                return; // to keep the manager for the next request
            }
        }
    reply->manager()->deleteLater();
}

void Configuracion::cambioReplyBlock(QNetworkReply *reply)
{
    reply->deleteLater();
    if(reply->error() == QNetworkReply::NoError)
        {
            // Get the http status code
            int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (v >= 200 && v < 300) // Success
            {
                // Here we got the final reply
                //QString replyText = reply->readAll();
                _cambio = readCambioBlock(reply->readAll());
                _block = false;
            }
            else if (v >= 300 && v < 400) // Redirection
            {

                // Get the redirection url
                QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
                // Because the redirection url can be relative,
                // we have to use the previous one to resolve it
                newUrl = reply->url().resolved(newUrl);

                QNetworkAccessManager *manager = reply->manager();
                connect(manager,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),this,SLOT(onIgnoreSSLErrors(QNetworkReply*,QList<QSslError>)));
                connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(cambioReplyBlock(QNetworkReply*)));
                QNetworkRequest redirection(newUrl);
                manager->get(redirection);
                return; // to keep the manager for the next request
            }
        }
    reply->manager()->deleteLater();
}

void Configuracion::applyCambio(float f, QString target)
{
    static int i= 1;
 //   //qDebug() << i << target;
    i++;
    QSqlQuery q(Configuracion_global->groupDB);
    QString sql = QString("UPDATE monedas SET cambio = '%1' WHERE nombre_corto ='%2';").arg(f).arg(target);
    if(!q.exec(sql))
        qDebug() << q.lastError();
}


void Configuracion::readCambio(QString s)
{
    QString place = "<span class=bld>";
    int index = s.indexOf(place)+place.size();
    int end = s.indexOf(" ",index);
    int end2 = s.indexOf("</span>",end);
    QString sFloat = s.mid(index,end-index);
    QString to = s.mid(end+1,end2-end-1);
    bool ok;
    float f = sFloat.toFloat(&ok);
    if(ok)
    {
        emit cambioReady(f,to);//double, qString....
    }
//    else
//        qDebug()<<s;
}

float Configuracion::readCambioBlock(QString s)
{
    QString place = "<span class=bld>";
    int index = s.indexOf(place)+place.size();
    int end = s.indexOf(" ",index);
    int end2 = s.indexOf("</span>",end);
    QString sFloat = s.mid(index,end-index);
    QString to = s.mid(end+1,end2-end-1);
    bool ok;
    float f = sFloat.toFloat(&ok);

    if(ok)
        return f;
    else
        return 0.0;
}
/*
 *connect(Configuracion_global,SIGNAL(cambioReady(float)),this,SLOT(unSLOT(float);
 *Configuracion_global->getCambio("EUR","USD");
 *ó
 *Configuracion_global->getCambio("EUR","USD",100 (cantidad) );*/





