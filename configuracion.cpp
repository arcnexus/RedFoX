#include "Auxiliares/Globlal_Include.h"

#include "openrptLibs/include/orprerender.h"
#include "openrptLibs/include/parameteredit.h"
#include "openrptLibs/include/orprintrender.h"
#include "openrptLibs/include/previewdialog.h"
#include "openrptLibs/include/renderobjects.h"

#include "SOAP/soapcheckVatBindingProxy.h"
#include "SOAP/checkVatBinding.nsmap"

#include <QLineEdit>
#include <QtNetwork/QSslConfiguration>
#include <QSslError>

Configuracion::Configuracion(QObject* parent) :
    QObject(parent)
{

    iva_model = 0;
    paises_model = 0;
    client_model = 0;
    usuarios_model = 0;
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
        if(ret.right(2)==",1" || ret.right(2)==",2" || ret.right(2)==",3" || ret.right(2)==",4" || ret.right(2)==",5"
                || ret.right(2)==",6" || ret.right(2)==",7" || ret.right(2)==",8" || ret.right(2)==",9")
            ret = ret+"0";
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

    QSqlQuery query(QSqlDatabase::database("Maya"));
    if(query.exec("SELECT * FROM tiposiva"))
    {
        while(query.next())
        {
            QString key = query.record().value("cTipo").toString();
            ivas.insert(key,query.record());
        }
    }
    if(ivas.size()!=4)
    {
        query.prepare("INSERT INTO `tiposiva` (`id`, `nombre_interno`, `cTipo`, `cDescripcionTipoIVA`, `nIVA`, `nRegargoEquivalencia`) VALUES (1, 'base1', 'NORMAL', '21%', 21.00, 4.00), (2, 'base2', 'Reducido', '10%', 10.00, 2.00), (3, 'base3', 'Superreducido', '4%', 4.00, 1.00), (4, 'base4', 'Exento', 'Exento', 0.00, 0.00);");
        query.exec();
        return Cargar_iva();
    }

    if(iva_model == 0)
        iva_model = new QSqlTableModel(this,QSqlDatabase::database("Maya"));
    iva_model->setTable("tiposiva");
    iva_model->select();

    QString base1 , base2 , base3 ,base4;
    QString re1 , re2 , re3 , re4;
    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    for (int i = 0;i<keys.size();i++)
    {
        QString aux = Configuracion_global->ivas[keys.at(i)].value("nombre_interno").toString();
        if(aux == "base1")
        {
            base1 = Configuracion_global->ivas[keys.at(i)].value("nIVA").toString();
            re1 = Configuracion_global->ivas[keys.at(i)].value("nRegargoEquivalencia").toString();
        }
        else if(aux == "base2")
        {
            base2 = Configuracion_global->ivas[keys.at(i)].value("nIVA").toString();
            re2 = Configuracion_global->ivas[keys.at(i)].value("nRegargoEquivalencia").toString();
        }
        else if(aux == "base3")
        {
            base3 = Configuracion_global->ivas[keys.at(i)].value("nIVA").toString();
            re3 = Configuracion_global->ivas[keys.at(i)].value("nRegargoEquivalencia").toString();
        }
        else if(aux == "base4")
        {
            base4 = Configuracion_global->ivas[keys.at(i)].value("nIVA").toString();
            re4 = Configuracion_global->ivas[keys.at(i)].value("nRegargoEquivalencia").toString();
        }
    }
    ivaList << base1 << base2 << base3 << base4;
    reList << re1 << re2 << re3 << re4;
}

int Configuracion::getIdIva(QString cIva)
{
    QSqlQuery qIVA(QSqlDatabase::database("Maya"));
    qIVA.prepare("select id from tiposiva where cTipo = :cIva");
    qIVA.bindValue(":cIva",cIva);
    if(!qIVA.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("RECUPERAR ID IVA"),
                             tr("Falló la recuperación del identificador del IVA : %1").arg(qIVA.lastError().text()));
        return 0;
    } else {
        qIVA.next();
        return qIVA.record().field("id").value().toDouble();
    }

}

QString Configuracion::setTipoIva(int idIva)
{
    QSqlQuery qIVA(QSqlDatabase::database("Maya"));
    qIVA.prepare("select cTipo from tiposiva where id = :id");
    qIVA.bindValue(":id",idIva);
    if(!qIVA.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("RECUPERAR ID IVA"),
                             tr("Falló la recuperación del identificador del IVA : %1").arg(qIVA.lastError().text()));
        return 0;
    } else {
        qIVA.next();
        return qIVA.record().field("cTipo").value().toString();
    }
}

void Configuracion::Cargar_paises()
{
    paises.clear();
    QSqlQuery query(QSqlDatabase::database("Maya"));
    if(query.exec("SELECT * FROM paises"))
    {
        while(query.next())
        {
            QString key = query.record().value("cTipo").toString();
            paises.insert(key,query.record());
        }
    }
    if(paises_model == 0)
        paises_model = new QSqlTableModel(this,QSqlDatabase::database("Maya"));
    paises_model->setTable("paises");
    paises_model->select();
}

QString Configuracion::Devolver_pais(int id)
{
    QSqlQuery qPais(QSqlDatabase::database("Maya"));
    qPais.prepare("select pais from paises where id = :nId");
    qPais.bindValue(":nId",id);
    if(qPais.exec())
    {
        qPais.next();
        return qPais.record().field("pais").value().toString();
    }
    return "";
}

QString Configuracion::Devolver_moneda(int id)
{
    QSqlQuery qMoneda(QSqlDatabase::database("Maya"));
    qMoneda.prepare("select moneda from monedas where id = :nId");
    qMoneda.bindValue(":nId",id);
    if(qMoneda.exec())
    {
        qMoneda.next();
        return qMoneda.record().field("moneda").value().toString();
    }
    return "";
}

QString Configuracion::Devolver_codDivisa(int id)
{
    QSqlQuery qMoneda(QSqlDatabase::database("Maya"));
    qMoneda.prepare("select nombreCorto from monedas where id = :nId");
    qMoneda.bindValue(":nId",id);
    if(qMoneda.exec())
    {
        qMoneda.next();
        return qMoneda.record().field("nombreCorto").value().toString();
    }
    return "";
}

int Configuracion::Devolver_id_moneda(QString cDivisa)
{
    QSqlQuery queryMoneda(QSqlDatabase::database("Maya"));
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
    QSqlQuery qIdioma(QSqlDatabase::database("Maya"));
    if(qIdioma.exec("select idioma from idiomas where id = "+QString::number(id))) {
        qIdioma.next();
        return qIdioma.record().value("idioma").toString();
    }
    else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Idioma"),
                             tr("Fallo la recuperacion del idioma: ")+qIdioma.lastError().text(),
                             tr("Aceptar"));
    }
    return "";
}

int Configuracion::Devolver_id_idioma(QString idioma)
{
    QSqlQuery qIdioma(QSqlDatabase::database("Maya"));
    if(qIdioma.exec("select id from idiomas where idioma = '"+idioma+"'")){
        qIdioma.next();
        return qIdioma.record().value("id").toInt();
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Idioma"),
                             tr("Fallo la recuperacion del idioma: ")+qIdioma.lastError().text(),
                             tr("Aceptar"));
    }
    return 0;

}

void Configuracion::CargarClientes()
{
    if(client_model == 0)
        client_model = new QSqlRelationalTableModel(this,QSqlDatabase::database("Maya"));
    client_model->setTable("clientes");
    client_model->select();
}

void Configuracion::CargarUsuarios()
{
    if(usuarios_model == 0)
        usuarios_model = new QSqlRelationalTableModel(this,QSqlDatabase::database("Maya"));
    usuarios_model->setTable("usuarios");
    usuarios_model->select();
}

void Configuracion::CargarDatosBD()
{
    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
    this->cDriverBDMaya = settings.value("cDriverBDMaya").toString();
    this->cRutaBdMaya = settings.value("cRutaDBMaya").toString();
    this->cHostBDMaya = settings.value("cHostBDMaya").toString();
    this->cUsuarioBDMaya  =   settings.value("cUserBDMaya").toString();
    this->cPasswordBDMaya = settings.value("cPasswordBDMaya").toString();

    this->cNombreBDTiendaWeb = settings.value("nombreBDTiendaWeb").toString();
    this->cHostBDTiendaWeb = settings.value("hostTiendaWeb").toString();
    this->cUsuarioTiendaWeb =settings.value("usuarioTiendaWeb").toString();
    this->cPasswordTiendaWeb = settings.value("Pass_web").toString();
    this->cPuertoTiendaWeb =settings.value("puertoTiendaWeb").toString();
    this->EnlaceWeb = settings.value("EnlaceWeb").toBool();


    this->HostDB_MediTec = settings.value("HostDB_MediTec").toString();
    this->NombreDB_MediTec = settings.value("NombreDB_MediTec").toString();
    this->UsuarioDB_MediTec = settings.value("UsuarioDB_MediTec").toString();
    this->PasswordDB_MediTec = settings.value("PasswordDB_MediTec").toString();
    this->PortDB_MediTec =  settings.value("PuertoDB_MediTec").toString();


}

void Configuracion::AbrirDbWeb()
{
    //--------------
    // abrir BD WEB
    //--------------
    QSqlDatabase dbWeb = QSqlDatabase::addDatabase("QMYSQL","dbweb");
    dbWeb.setDatabaseName(Configuracion_global->cNombreBDTiendaWeb);
        dbWeb.setDatabaseName(Configuracion_global->cNombreBDTiendaWeb);
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
void Configuracion::CargarDatos()
{
    QSqlQuery qEmpresa(QSqlDatabase::database("Maya"));
    qEmpresa.prepare("Select * from empresas where id =:id");
    idEmpresa = 1;
    qEmpresa.bindValue(":id",idEmpresa);
    if (qEmpresa.exec()) {
        qEmpresa.next();
        this->cPais = qEmpresa.record().field("pais").value().toString();
        this->cEjercicio = qEmpresa.record().field("ejercicio").value().toString();
        this->nDigitosFactura = qEmpresa.record().field("digitosfactura").value().toInt();

        if(qEmpresa.record().field("lProfesional").value().toInt()==1)
            this->lProfesional = true;
        else
            this->lProfesional = false;
        this->nIRPF = qEmpresa.record().field("IIRPF").value().toInt();
        this->cSerie = qEmpresa.record().field("serie").value().toString();
        this->nDigitosCuentasContables = qEmpresa.record()./*field("ndigitoscuenta").*/value("ndigitoscuenta").toInt();
        this->cCuentaClientes = qEmpresa.record().field("codigocuentaclientes").value().toString();
        this->cCuentaAcreedores = qEmpresa.record().field("codigocuentaacreedores").value().toString();
        this->cCuentaProveedores = qEmpresa.record().field("codigocuentaproveedores").value().toString();
        this->Autocodigoart = qEmpresa.record().value("autocodigo").toBool();
        this->tamanocodigo = qEmpresa.record().value("tamanocodigo").toInt();
        this->cCuentaCobrosClientes = qEmpresa.record().value("cCuentaCobros").toString();
        this->cCuentaPagosProveedor = qEmpresa.record().value("cCuentaPagos").toString();
        this->medic = qEmpresa.record().value("medica").toBool();
        this->internacional = qEmpresa.record().value("internacional").toBool();


    } else {
        qDebug() << qEmpresa.lastError().text();
        QMessageBox::warning(qApp->activeWindow(),tr("EMPRESA"),tr("Falló la carga de datos de empresa"),tr("Aceptar"));
    }
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


QString Configuracion::letraDNI(QString Nif)
{
    int nSuma, nSumaPar, nSumaNon;

    if (Nif.isEmpty())
        return ("");

    if (Nif[0].isDigit() || Nif.mid(1,1).contains("[KLMXYZ]"))  // Persona física
    {
        if (Nif.mid(1,1).contains("[XYZ]"))
            //Nif = iif(Nif.SubString(1, 1) == "Z", "2", iif(Nif.SubString(1, 1) == "Y", "1", "0")) + Nif.SubString(2, Nif.Length());
            Nif = Nif.mid(1,1) == "Z" ? "2" : Nif.mid(1,1) == "Y" ? "1" : "0";
        else if (Nif.mid(1,1).contains("[KLM]"))
            Nif = Nif.mid(2);

        QString x1 = "TRWAGMYFPDXBNJZSQVHLCKE";
        return (Nif +x1.mid(Nif.trimmed().toInt() % 23,1));
    }
    else      // Persona jurídica
    {
        if (Nif.mid(1,1).contains("[ABCDEFGHJNPQRSUVW]"))
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

void Configuracion::imprimir(QString db, QString report, bool toPDF,bool preview, QWidget *parent)
{
    QFile f(report);
    if(f.open(QFile::ReadOnly))
    {
        QDomDocument ddoc;
        if(ddoc.setContent(f.readAll()))
        {
            ORPreRender pre;
            pre.setDom(ddoc);
            pre.setDatabase(QSqlDatabase::database(db));

            ParameterEdit paramEdit(0);
            QDialog *dlgEdit = ParameterEdit::ParameterEditDialog(&paramEdit, parent);
            if (paramEdit.setDocument(ddoc))
                if (dlgEdit->exec() != QDialog::Accepted)
                    return;

            pre.setParamList(paramEdit.getParameterList());
            ORODocument * doc = pre.generate();

            if(doc)
            {
                QPrinter printer(QPrinter::HighResolution);
                if(preview)
                {
                    PreviewDialog preview (doc, &printer, parent);
                    if (preview.exec() == QDialog::Rejected)
                        return;
                }
                ORPrintRender render;
                render.setupPrinter(doc, &printer);

                if(printer.printerName().isEmpty())
                {
                    QPrintDialog pd(&printer);
                    if(pd.exec() != QDialog::Accepted)
                    {
                        qDebug() << "no printer, can't preview";
                        return; // no printer, can't preview
                    }
                }

                if(toPDF)
                {
                    QString output = QFileDialog::getSaveFileName(parent);

                    if(output.isEmpty())
                        return;

                    if ( QFileInfo( output ).suffix().isEmpty() )
                      output.append(".pdf");

                    ORPrintRender::exportToPDF(doc, output );
                }
                else
                {
                    QPrintDialog pd(&printer);
                    pd.setMinMax(1, doc->pages());
                    if(pd.exec() == QDialog::Accepted)
                    {
                        render.setPrinter(&printer);
                        render.render(doc);
                    }
                }
                delete doc;
            }
        }
    }
}

bool Configuracion::comprobarNIF(QString country_code, QString nif)
{
    _ns2__checkVat ns2__checkVat;
    QByteArray   code_bytes;
    QByteArray  nif_bytes = nif.toAscii();
    ns2__checkVat.vatNumber = nif_bytes.data();

    QSqlQuery q(QSqlDatabase::database("Maya"));
    QString sql = QString("SELECT * FROM paises where pais = '%1'").arg(country_code);
    q.prepare(sql);
    if(q.exec())
    {
        if(q.next())
        {
            QString code = q.record().value("countryCode").toString();
            code_bytes = code.toAscii();
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
    return false;
}

QString Configuracion::Crypt(QString input)
{
    return "";
}

QString Configuracion::DeCrypt(QString input)
{
    return "";
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

void Configuracion::generarTablaDivisas()
{
    QFile f(":/Icons/divisas.txt");
    if(f.open((QIODevice::ReadOnly | QIODevice::Text)))
    {
        QSqlQuery q(QSqlDatabase::database("Maya"));
        QTextStream in(&f);
        in.setCodec("UTF-8");
        QString all = in.readAll();
        all.remove("\r");
        all.remove("\n");
        QStringList blocks = all.split("},",QString::SkipEmptyParts);
        foreach(const QString &moneda , blocks)
        {
            QStringList subBlocks = moneda.split("\",",QString::SkipEmptyParts);
            QString nombre_corto = subBlocks.at(0).mid(subBlocks.at(0).lastIndexOf("\"")+1);
            QString nombre = subBlocks.at(1).mid(subBlocks.at(1).lastIndexOf("\"")+1);
            QString simbol = subBlocks.at(4).mid(subBlocks.at(4).lastIndexOf("\"")+1);
            simbol.remove("}");
            simbol = simbol.trimmed();
            q.prepare("INSERT INTO monedas ("
                                  "moneda ,"
                                  "nombreCorto ,"
                                  "simbolo "
                                  ")"
                                  "VALUES ("
                                  ":moneda, :nombreCorto, :simbolo );");
            q.bindValue(":moneda",nombre);
            q.bindValue(":nombreCorto",nombre_corto);
            q.bindValue(":simbolo",simbol);
            if(!q.exec())
                    qDebug()<< q.lastError();
        }
    }
}

void Configuracion::updateTablaDivisas(QString current)
{
    connect(this,SIGNAL(cambioReady(float,QString)),SLOT(applyCambio(float,QString)));
    QSqlQuery q(QSqlDatabase::database("Maya"));
    if(q.exec("SELECT * FROM monedas"))
    {
        while(q.next())
            getCambio(current,q.record().value("nombreCorto").toString());
    }
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

void Configuracion::applyCambio(float f, QString target)
{
    static int i= 1;
    qDebug() << i << target;
    i++;
    QSqlQuery q(QSqlDatabase::database("Maya"));
    QString sql = QString("UPDATE monedas SET cambio = '%1' WHERE nombreCorto ='%2';").arg(f).arg(target);
    if(!q.exec(sql))
        qDebug() << q.lastError();
}


void Configuracion::readCambio(QString s)
{
    static int c = 0;
    qDebug()<< "readCambio"<< c;

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
        c++;
        emit cambioReady(f,to);
    }
    else
        qDebug()<<s;
}
/*
 *connect(Configuracion_global,SIGNAL(cambioReady(float)),this,SLOT(unSLOT(float);
 *Configuracion_global->getCambio("EUR","USD");
 *ó
 *Configuracion_global->getCambio("EUR","USD",100 (cantidad) );*/





