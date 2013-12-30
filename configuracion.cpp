#include "../Auxiliares/Globlal_Include.h"

#include "../EditorReports/reportrenderer.h"

//#include "SOAP/soapcheckVatBindingProxy.h"
//#include "SOAP/checkVatBinding.nsmap"

#include <QLineEdit>
#include <QtNetwork/QSslConfiguration>
#include <QSslError>
#include <QPrintDialog>

byte Configuracion::key[ CryptoPP::AES::DEFAULT_KEYLENGTH ];
byte Configuracion::iv[ CryptoPP::AES::BLOCKSIZE ];

Configuracion::Configuracion(QObject* parent) :
    QObject(parent)
{

    iva_model = 0;
    paises_model = new QSqlQueryModel(this);
    client_model = 0;
    usuarios_model = 0;
    validator_cantidad = new QDoubleValidator(-99999999999999999.00,99999999999999999.00,2,this);
    validator_porciento = new QDoubleValidator(0,100,2,this);
    grupo_iva << tr("Nacional") << tr("Europeo") << tr("canarias") << tr("Internacional");

    //AES keys
    //TODO generar una ¿mejor?
    key[0] = 0x10;
    key[1] = 0x10;
    key[2] = 0x10;
    key[3] = 0x10;
    key[4] = 0x10;
    key[5] = 0x10;
    key[6] = 0x10;
    key[7] = 0x10;
    key[8] = 0x10;
    key[9] = 0x10;
    key[10] = 0x10;
    key[11] = 0x10;
    key[12] = 0x10;
    key[13] = 0x10;
    key[14] = 0x10;
    key[15] = 0x10;

    iv[0] = 0x10;
    iv[1] = 0x10;
    iv[2] = 0x10;
    iv[3] = 0x10;
    iv[4] = 0x10;
    iv[5] = 0x10;
    iv[6] = 0x10;
    iv[7] = 0x10;
    iv[8] = 0x10;
    iv[9] = 0x10;
    iv[10] = 0x10;
    iv[11] = 0x10;
    iv[12] = 0x10;
    iv[13] = 0x10;
    iv[14] = 0x10;
    iv[15] = 0x10;
}


QString Configuracion::
toFormatoMoneda(QString cTexto)
{
    if(cTexto.isEmpty())
        return "0,00";
    else
    {
        QString aux = cTexto;
        int decimals = Configuracion_global->decimales;
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
    if(moneda.isEmpty() || moneda == "0,00")
        return 0.00;
    moneda = moneda.replace(".","").replace(",",".");
    double dblMoneda;
    int d;
    d= Configuracion_global->decimales_campos_totales;
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

    if(iva_model == 0)
    {
        iva_model = new QSqlTableModel(this,Configuracion_global->groupDB);
        iva_model->setTable("tiposiva");
    }
    iva_model->select();

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
    QSqlQuery qIVA(Configuracion_global->groupDB);
    qIVA.prepare("select id from tiposiva where tipo = :cIva");
    qIVA.bindValue(":cIva",cIva);
    if(!qIVA.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("RECUPERAR id IVA"),
                             tr("Falló la recuperación del identificador del IVA : %1").arg(qIVA.lastError().text()));
        return 0;
    } else {
        qIVA.next();
        return qIVA.record().field("id").value().toDouble();
    }

}

QString Configuracion::setTipoIva(int idIva)
{
    QSqlQuery qIVA(Configuracion_global->groupDB);
    qIVA.prepare("select tipo from tiposiva where id = :id");
    qIVA.bindValue(":id",idIva);
    if(!qIVA.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("RECUPERAR id IVA"),
                             tr("Falló la recuperación del identificador del IVA : %1").arg(qIVA.lastError().text()));
        return 0;
    } else {
        qIVA.next();
        return qIVA.record().field("tipo").value().toString();
    }
}

void Configuracion::Cargar_paises()
{

    paises_model->setQuery("select pais from paises order by pais",Configuracion_global->groupDB);
}

int Configuracion::Devolver_id_pais(QString pais)
{
    QSqlQuery qPais(Configuracion_global->groupDB);
    qPais.prepare("select id from paises where pais = :pais");
    qPais.bindValue(":pais",pais);
    if(qPais.exec())
    {
        qPais.next();
        int id = qPais.record().field("id").value().toInt();
        return id;
    }
    return 0;
}

QString Configuracion::Devolver_pais(int id)
{
    QSqlQuery qPais(Configuracion_global->groupDB);
    qPais.prepare("select pais from paises where id = :nid");
    qPais.bindValue(":nid",id);
    if(qPais.exec())
    {
        qPais.next();
        return qPais.record().field("pais").value().toString();
    }
    return "";
}

QString Configuracion::Devolver_moneda(int id)
{
    QSqlQuery qMoneda(Configuracion_global->groupDB);
    qMoneda.prepare("select moneda from monedas where id = :nid");
    qMoneda.bindValue(":nid",id);
    if(qMoneda.exec())
    {
        qMoneda.next();
        return qMoneda.record().field("moneda").value().toString();
    }
    return "";
}

QString Configuracion::Devolver_codDivisa(int id)
{
    QSqlQuery qMoneda(Configuracion_global->groupDB);
    qMoneda.prepare("select nombre_corto from monedas where id = :nid");
    qMoneda.bindValue(":nid",id);
    if(qMoneda.exec())
    {
        qMoneda.next();
        return qMoneda.record().field("nombre_corto").value().toString();
    }
    return "";
}

int Configuracion::Devolver_id_moneda(QString cDivisa)
{
    QSqlQuery queryMoneda(Configuracion_global->groupDB);
    queryMoneda.prepare("select id from monedas where moneda = :cDivisa");
    queryMoneda.bindValue(":cDivisa",cDivisa);
    if(queryMoneda.exec())
    {
        queryMoneda.next();
        return queryMoneda.record().value("id").toInt();
    }
}

QString Configuracion::Devolver_idioma(int id)
{
    QSqlQuery qidioma(Configuracion_global->groupDB);
    if(qidioma.exec("select idioma from idiomas where id = "+QString::number(id))) {
        qidioma.next();
        return qidioma.record().value("idioma").toString();
    }
    else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar idioma"),
                             tr("Fallo la recuperación del idioma: ")+qidioma.lastError().text(),
                             tr("Aceptar"));
    }
    return "";
}

int Configuracion::Devolver_id_idioma(QString idioma)
{
    QSqlQuery qidioma(Configuracion_global->groupDB);
    if(qidioma.exec("select id from idiomas where idioma = '"+idioma+"'")){
        qidioma.next();
        return qidioma.record().value("id").toInt();
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar idioma"),
                             tr("Fallo la recuperación del idioma: ")+qidioma.lastError().text(),
                             tr("Aceptar"));
    }
    return 0;

}

int Configuracion::Devolver_id_forma_pago(QString forma_pago)
{
    QSqlQuery queryFP(Configuracion_global->groupDB);
    if(queryFP.exec("select id from formpago where forma_pago = '"+forma_pago+"'")){
        queryFP.next();
        int id = queryFP.record().value("id").toInt();
        return id;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Formas de pago"),
                             tr("Fallo la recuperación del identificador de la forma de pago: ")+queryFP.lastError().text(),
                             tr("Aceptar"));
    }
    return 0;
}

int Configuracion::Devolver_id_codigo_forma_pago(QString codigo)
{
    QSqlQuery queryFP(Configuracion_global->groupDB);
    if(queryFP.exec("select id from formpago where codigo = '"+codigo+"'")){
        queryFP.next();
        int id = queryFP.record().value("id").toInt();
        return id;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Formas de pago"),
                             tr("Fallo la recuperación del identificador de la forma de pago: ")+queryFP.lastError().text(),
                             tr("Aceptar"));
    }
    return 0;
}

QString Configuracion::Devolver_forma_pago(int id)
{
    QSqlQuery queryFP(Configuracion_global->groupDB);
    if(queryFP.exec("select forma_pago from formpago where id = "+QString::number(id))){
        queryFP.next();
        QString cFP = queryFP.record().value("forma_pago").toString();
        return cFP;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Formas de pago"),
                             tr("Fallo la recuperación de la forma de pago: ")+queryFP.lastError().text(),
                             tr("Aceptar"));
    }
    return "";
}

QString Configuracion::Devolver_codigo_forma_pago(int id)
{
    QSqlQuery queryFP(Configuracion_global->groupDB);
    if(queryFP.exec("select codigo from formpago where id = "+QString::number(id))){
        queryFP.next();
        QString cFP = queryFP.record().value("codigo").toString();
        return cFP;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Formas de pago"),
                             tr("Fallo la recuperación de la forma de pago: ")+queryFP.lastError().text(),
                             tr("Aceptar"));
    }
    return "";

}

int Configuracion::Devolver_id_tarifa(QString cTarifa)
{
    QSqlQuery queryTarifa(Configuracion_global->groupDB);
    if(queryTarifa.exec("select id from codigotarifa where descripcion = '"+cTarifa+"'")){
        queryTarifa.next();
        int id = queryTarifa.record().value("id").toInt();
        return id;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Tarifas"),
                             tr("Fallo la recuperación del identificador de la tarifa: ")+queryTarifa.lastError().text(),
                             tr("Aceptar"));
    }
    return 0;

}

QString Configuracion::Devolver_tarifa(int id_tarifa)
{
    QSqlQuery queryTarifa(Configuracion_global->groupDB);
    if(queryTarifa.exec("select descripcion from codigotarifa where id = "+QString::number(id_tarifa))){
        queryTarifa.next();
        QString cTarifa = queryTarifa.record().value("descripcion").toString();
        return cTarifa;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Tarifas"),
                             tr("Fallo la recuperación de la tarifa: ")+queryTarifa.lastError().text(),
                             tr("Aceptar"));
    }
    return "";
}

QString Configuracion::Devolver_tipo_gasto(int id_gasto)
{
    QSqlQuery queryGasto(Configuracion_global->groupDB);
    if(queryGasto.exec("select descripcion from grupos_gasto where id = "+QString::number(id_gasto))){
        queryGasto.next();
        QString cGasto = queryGasto.record().value("descripcion").toString();
        return cGasto;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Tarifas"),
                             tr("Fallo la recuperación del tipo de gasto: ")+queryGasto.lastError().text(),
                             tr("Aceptar"));
    }
    return "";
}

int Configuracion::Devolver_id_tipo_gasto(QString desc)
{

    QSqlQuery queryGasto(Configuracion_global->groupDB);
    if(queryGasto.exec("select id from grupos_gasto where descripcion = '"+desc+"'")){
        queryGasto.next();
        int id = queryGasto.record().value("id").toInt();
        return id;
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Tipos de Gasto"),
                             tr("Fallo la recuperación del identificador del gasto: ")+queryGasto.lastError().text(),
                             tr("Aceptar"));
    }
    return 0;
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
}

QString Configuracion::Devolver_descripcion_tipo_iva(double tipo)
{
    QSqlQuery query_iva(Configuracion_global->groupDB);
    if(query_iva.exec("select tipo from tiposiva where iva = "+QString::number(tipo)))
    {
        query_iva.next();
        return query_iva.record().value("tipo").toString();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar tipo de iva"),
                             tr("Fallo la recuperación del tipo de iva: ")+query_iva.lastError().text(),
                             tr("Aceptar"));
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
}

QString Configuracion::devolver_codigo_articulo(int id)
{
    QSqlQuery query_art(Configuracion_global->groupDB);
    if(query_art.exec("select codigo from articulos where id = "+QString::number(id)))
    {
        query_art.next();
        return query_art.record().value("codigo").toString();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar artículo"),
                             tr("Fallo al recuperar el código de artículo: ")+query_art.lastError().text(),
                             tr("Aceptar"));

}

QString Configuracion::devolver_codigo_barras(int id)
{
    QSqlQuery query_art(Configuracion_global->groupDB);
    if(query_art.exec("select codigo_barras from articulos where id = "+QString::number(id)))
    {
        query_art.next();
        return query_art.record().value("codigo_barras").toString();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar artículo"),
                             tr("Fallo al recuperar el código de barras del artículo: ")+query_art.lastError().text(),
                             tr("Aceptar"));
}

QString Configuracion::devolver_referencia_articulo(int id)
{
    QSqlQuery query_art(Configuracion_global->groupDB);
    if(query_art.exec("select codigo_fabricante from articulos where id = "+QString::number(id)))
    {
        query_art.next();
        return query_art.record().value("codigo_fabricante").toString();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar artículo"),
                             tr("Fallo al recuperar la referencia de artículo: ")+query_art.lastError().text(),
                             tr("Aceptar"));
}

float Configuracion::Devolver_iva(int id)
{
    QSqlQuery query(Configuracion_global->groupDB);
    if(query.exec("select iva from tiposiva where id = "+QString::number(id)))
    {
        query.next();
        return query.record().value("iva").toFloat();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar IVA"),
                             tr("Fallo al recuperar el IVA")+query.lastError().text(),
                             tr("Aceptar"));
}

float Configuracion::devolver_rec_iva(float porc_iva)
{
    QSqlQuery query(Configuracion_global->groupDB);
    if(query.exec("select recargo_equivalencia from tiposiva where iva = "+QString::number(porc_iva)))
    {
        query.next();
        return query.record().value("recargo_equivalencia").toFloat();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar R.E."),
                             tr("Fallo al recuperar el R.E")+query.lastError().text(),
                             tr("Aceptar"));
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
            dblNumber += 0,0001;
        default:
            break;
        }
         StrNumber = QString::number(dblNumber,'f',decimals);
    }

    return toFormatoMoneda(StrNumber);
}

QString Configuracion::devolver_agente(int id)
{
    QSqlQuery query(Configuracion_global->groupDB);
    if(query.exec("select nombre from agentes where id = "+QString::number(id)))
    {
        query.next();
        return query.record().value("nombre").toString();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar agente"),
                             tr("Fallo al recuperar el agente")+query.lastError().text(),
                             tr("Aceptar"));
}

int Configuracion::devolver_id_agente(QString agente)
{
    QSqlQuery query(Configuracion_global->groupDB);
    if(query.exec("select id from agentes where nombre = '"+agente+"'"))
    {
        query.next();
        return query.record().value("id").toInt();

    } else
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar identificador agente"),
                             tr("Fallo la recuperación del identificador: ")+query.lastError().text(),
                             tr("Aceptar"));

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
        usuarios_model = new QSqlRelationalTableModel(this,Configuracion_global->groupDB);
    usuarios_model->setTable("usuarios");
    usuarios_model->select();
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
  //  qDebug() << global_user;
    this->global_pass = DeCrypt(settings.value("cPasswordBDMaya").toString());
   // qDebug() << global_pass;
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
        this->globalDB.setDatabaseName("mayaglobal");
        this->globalDB.setHostName(Configuracion_global->global_host);
        this->globalDB.open(Configuracion_global->global_user,Configuracion_global->global_pass);
        //this->globalDB.open("root","meganizado"); ?? q collons!?
    }

    if (this->globalDB.lastError().isValid())
    {
        QMessageBox::critical(0, "error:", this->globalDB.lastError().text());
    }
    return !this->globalDB.lastError().isValid();
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

void Configuracion::CerrarDbWeb()
{
    dbWeb.close();
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

void Configuracion::CerrarBDMediTec()
{
    db_meditec.close();
}
void Configuracion::CargarDatos(int id)
{
    QSqlQuery qEmpresa(Configuracion_global->groupDB);
    qEmpresa.prepare("Select * from empresas where id =:id");
    qEmpresa.bindValue(":id",id);
    if (qEmpresa.exec()) {
        qEmpresa.next();
        this->pais = qEmpresa.record().field("pais").value().toString();
        this->cEjercicio = qEmpresa.record().field("ejercicio").value().toString();
        this->ndigitos_factura = qEmpresa.record().field("digitos_factura").value().toInt();

        if(qEmpresa.record().field("lProfesional").value().toInt()==1)
            this->lProfesional = true;
        else
            this->lProfesional = false;
        this->irpf = qEmpresa.record().field("IIRPF").value().toInt();
        this->serie = qEmpresa.record().field("serie").value().toString();
        this->digitos_cuentas_contables = qEmpresa.record()./*field("digitos_cuenta").*/value("digitos_cuenta").toInt();
        this->cuenta_clientes = qEmpresa.record().field("codigo_cuenta_clientes").value().toString();
        this->cuenta_acreedores = qEmpresa.record().field("codigo_cuenta_acreedores").value().toString();
        this->cuenta_proveedores = qEmpresa.record().field("codigo_cuenta_proveedores").value().toString();
        this->auto_codigoart = qEmpresa.record().value("auto_codigo").toBool();
        this->tamano_codigo = qEmpresa.record().value("tamano_codigo").toInt();
        this->cuenta_cobrosClientes = qEmpresa.record().value("cuenta_cobros").toString();
        this->cuenta_pagosProveedor = qEmpresa.record().value("cuenta_pagos").toString();
        this->cuenta_venta_mercaderias = qEmpresa.record().value("cuenta_ventas_mercaderias").toString();
        this->cuenta_venta_servicios = qEmpresa.record().value("cuenta_ventas_servicios").toString();
        this->cuenta_iva_repercutido1 = qEmpresa.record().value("cuenta_iva_repercutido1").toString();
        this->cuenta_iva_repercutido2 = qEmpresa.record().value("cuenta_iva_repercutido2").toString();
        this->cuenta_iva_repercutido3 = qEmpresa.record().value("cuenta_iva_repercutido3").toString();
        this->cuenta_iva_repercutido4 = qEmpresa.record().value("cuenta_iva_repercutido4").toString();
        this->cuenta_iva_soportado1 = qEmpresa.record().value("cuenta_iva_soportado1").toString();
        this->cuenta_iva_soportado2 = qEmpresa.record().value("cuenta_iva_soportado2").toString();
        this->cuenta_iva_soportado3 = qEmpresa.record().value("cuenta_iva_soportado3").toString();
        this->cuenta_iva_soportado4 = qEmpresa.record().value("cuenta_iva_soportado4").toString();
        this->cuenta_iva_repercutidoRe1 = qEmpresa.record().value("cuenta_iva_repercutido1_re").toString();
        this->cuenta_iva_repercutidoRe2 = qEmpresa.record().value("cuenta_iva_repercutido2_re").toString();
        this->cuenta_iva_repercutidoRe3 = qEmpresa.record().value("cuenta_iva_repercutido3_re").toString();
        this->cuenta_iva_repercutidoRe4 = qEmpresa.record().value("cuenta_iva_repercutido4_re").toString();
        this->cuenta_iva_soportado_re1 = qEmpresa.record().value("cuenta_iva_soportado1_re").toString();
        this->cuenta_iva_soportado_re2 = qEmpresa.record().value("cuenta_iva_soportado2_re").toString();
        this->cuenta_iva_soportado_re3 = qEmpresa.record().value("cuenta_iva_soportado3_re").toString();
        this->cuenta_iva_soportado_re4 = qEmpresa.record().value("cuenta_iva_soportado4_re").toString();
        this->medic = qEmpresa.record().value("medica").toBool();
        this->internacional = qEmpresa.record().value("internacional").toBool();
        this->margen = qEmpresa.record().value("margen").toDouble();
        this->margen_minimo = qEmpresa.record().value("margen_minimo").toDouble();
        this->activar_seguimiento = qEmpresa.record().value("seguimiento").toBool();
        this->irpf = qEmpresa.record().value("usar_irpf").toBool();
        this->caducidad_vales = qEmpresa.record().value("caducidad_vales").toInt();


    } else {
        qDebug() << qEmpresa.lastError().text();
        QMessageBox::warning(qApp->activeWindow(),tr("EMPRESA"),tr("Falló la carga de datos de empresa"),tr("Aceptar"));
    }
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

void Configuracion::setUpKeys()
{
    key[0] = 0x10;
    key[1] = 0x14;
    key[2] = 0x12;
    key[3] = 0x8;
    key[4] = 0x10;
    key[5] = 0x19;
    key[6] = 0x10;
    key[7] = 0x3;
    key[8] = 0x10;
    key[9] = 0x10;
    key[10] = 0x7;
    key[11] = 0x55;
    key[12] = 0x10;
    key[13] = 0x43;
    key[14] = 0x10;
    key[15] = 0x17;

    iv[0] = 0x10;
    iv[1] = 0x23;
    iv[2] = 0x10;
    iv[3] = 0x10;
    iv[4] = 0x67;
    iv[5] = 0xee;
    iv[6] = 0x10;
    iv[7] = 0xf5;
    iv[8] = 0x10;
    iv[9] = 0x89;
    iv[10] = 0x10;
    iv[11] = 0xdd;
    iv[12] = 0x10;
    iv[13] = 0x67;
    iv[14] = 0x10;
    iv[15] = 0xa3;
}
QString Configuracion::Crypt(QString input)
{
    setUpKeys();
    std::string plaintext = input.toStdString();
    std::string ciphertext;
    std::string decryptedtext;

    //
    // Create Cipher Text
    //
    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );

    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ) );
    stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plaintext.c_str() ), plaintext.length() + 1 );
    stfEncryptor.MessageEnd();

    //
    // Dump Cipher Text
    //

    QString output;
    for( int i = 0; i < ciphertext.size(); i++ )
    {
        QString aux;
        aux.sprintf("%02X",static_cast<byte>(ciphertext[i]));
        output.append(aux);
    }

    return output;
}

QString Configuracion::DeCrypt(QString input)
{
    setUpKeys();
    if(input.isEmpty())
        return input;
    int c = 0;
        char* dd = new char[input.size()/2];

        for(int a = 0; a< input.size() ; a+=2)
        {
            bool ok;
            int z =  input.mid(a,2).toInt(&ok,16);
            dd[c] = QChar(z).toLatin1();
            c++;
        }

        std::string ciphertext(dd);
        int x = ciphertext.size();
        std::string decryptedtext;
        if(x<c)
            return "";
        ciphertext.erase(c);

        //
        // Decrypt
        //
        CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );

        CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ) );
        stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
        stfDecryptor.MessageEnd();

        QString ss = QString::fromStdString(decryptedtext);
        ss.remove(QRegExp("\\000"));
        delete [] dd;
        return ss;
}

QString Configuracion::SHA256HashString(QString input)
{
    std::string digest;
    std::string aString = input.toStdString();
    CryptoPP::SHA256 hash;

    CryptoPP::StringSource foo(aString, true,
    new CryptoPP::HashFilter(hash,
      new CryptoPP::Base64Encoder (
         new CryptoPP::StringSink(digest))));

    return QString::fromStdString(digest).trimmed();
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
        r.setPrinter(&printer);
        QString errorStr;
        int errorLine;
        int errorColumn;
        QDomDocument doc;
        if (!doc.setContent(rep, false, &errorStr, &errorLine,
                            &errorColumn))
        {
            error = QString("Error: Parse error at line %1, column %2 : %3").arg(errorLine).arg(errorColumn).arg(errorStr);
            TimedMessageBox* t = new TimedMessageBox(qApp->activeWindow(),error,TimedMessageBox::Critical);
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
            TimedMessageBox* t = new TimedMessageBox(qApp->activeWindow(),error,TimedMessageBox::Critical);
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
            TimedMessageBox* t = new TimedMessageBox(qApp->activeWindow(),error,TimedMessageBox::Critical);
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





