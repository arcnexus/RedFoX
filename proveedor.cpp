#include "proveedor.h"
#include<QtSql>
#include<QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include "configuracion.h"

Proveedor::Proveedor(QObject *parent) :
    QObject(parent)
{
    this->id = 0;
}

void Proveedor::Anadir()
{
    QSqlQuery *QProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
    if (!QProveedor->exec("insert into proveedores (cCodigo) values('')"))
            QMessageBox::warning(NULL,QObject::tr("Gestión de proveedores"),
                                 QObject::tr("No se ha podido crear una nueva ficha de proveedor. Error: ")+
                                 QProveedor->lastError().text(),QObject::tr("Aceptar"));
    else {
        int nId  = QProveedor->lastInsertId().toInt();
        Recuperar( "Select * from proveedores where id = "+QString::number(nId));
        this->cCodigo = NuevoCodigoProveedor();
        this->cCuentaAplicacion = this->cCodigo;
    }
}

void Proveedor::Recuperar(QString cSQL)
{
    QSqlQuery *qProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
    if(qProveedor->exec(cSQL)) {
        if(qProveedor->next()) {
            QSqlRecord rProveedor = qProveedor->record();
            this->id = rProveedor.field("Id").value().toInt();
            this->cCodigo = rProveedor.field("cCodigo").value().toString();
            this->cProveedor = rProveedor.field("cProveedor").value().toString();
            this->cCif = rProveedor.field("cCif").value().toString();
            this->cDireccion1 = rProveedor.field("cDireccion1").value().toString();
            this->cDireccion2 = rProveedor.field("cDireccion2").value().toString();
            this->cCP = rProveedor.field("cCP").value().toString();
            this->cPoblacion = rProveedor.field("cPoblacion").value().toString();
            this->cProvincia = rProveedor.field("cProvincia").value().toString();
            this->cPais = rProveedor.field("cPais").value().toString();
            this->cTelefono1 = rProveedor.field("cTelefono1").value().toString();
            this->cTelefono2 = rProveedor.field("cTelefono2").value().toString();
            this->cTelefono3 = rProveedor.field("cTelefono3").value().toString();
            this->cFax = rProveedor.field("cFax").value().toString();
            this->cMovil = rProveedor.field("cMovil").value().toString();
            this->cEMail = rProveedor.field("cEMail").value().toString();
            this->cWeb = rProveedor.field("cWeb").value().toString();
            this->cPersonaContacto = rProveedor.field("cPersonaContacto").value().toString();
            this->nDiaCobro = rProveedor.field("nDiaCobro").value().toInt();
            this->cDireccionAlmacen = rProveedor.field("cDireccionAlmacen").value().toString();
            this->cCPAlmacen = rProveedor.field("cCPAlmacen").value().toString();
            this->cPoblacionAlmacen = rProveedor.field("cPoblacionAlmacen").value().toString();
            this->cProvinciaAlmacen = rProveedor.field("cProvinciaAlmacen").value().toString();
            this->cPaisAlmacen = rProveedor.field("cPaisAlmacen").value().toString();
            this->cTelefonoAlmacen = rProveedor.field("cTelefonoAlmacen").value().toString();
            this->cFaxAlmacen = rProveedor.field("cFaxAlmacen").value().toString();
            this->cCodigoFormaPago = rProveedor.field("cCodigoFormaPago").value().toString();
            this->dFechaUltimaCompra = rProveedor.field("dFechaUltimaCompra").value().toDate();
            this->rAcumuladoCompras = rProveedor.field("rAcumuladoCompras").value().toDouble();
            this->cEntidadBancariaProveedor = rProveedor.field("cEntidadBancariaProveedor").value().toString();
            this->cOficinaBancariaProveedor = rProveedor.field("cOficinaBancariaProveedor").value().toString();
            this->cDCProveedor = rProveedor.field("cDCProveedor").value().toString();
            this->cCCProveedor = rProveedor.field("cCCProveedor").value().toString();
            this->cEntidadPagoProveedor = rProveedor.field("cEntidadPagoProveedor").value().toString();
            this->cOficinaPagoProveedor = rProveedor.field("cOficinaPagoProveedor").value().toString();
            this->cDCPagoProveedor = rProveedor.field("cDCPagoProveedor").value().toString();
            this->cCuentaPagoProveedor = rProveedor.field("cCuentaPagoProveedor").value().toString();
            this->cCuentaIvaSoportado = rProveedor.field("cCuentaIvaSoportado").value().toString();
            this->rRetencionIRPF = rProveedor.field("rRetencionIRPF").value().toDouble();
            this->nTipoRetencion = rProveedor.field("nTipoRetencion").value().toInt();
            this->cCuentaAplicacion = rProveedor.field("cCuentaAplicacion").value().toString();
            this->cCuentaPagos = rProveedor.field("cCuentaPagos").value().toString();
            this->tComentarios = rProveedor.field("tComentarios").value().toString();
            this->nDto = rProveedor.field("nDto").value().toDouble();
            this->dFechaAlta = rProveedor.field("dFechaAlta").value().toDate();
            this->rDeudaMaxima = rProveedor.field("rDeudaMaxima").value().toDouble();
            this->rDeudaActual = rProveedor.field("rDeudaActual").value().toDouble();
            this->lRecargoEquivalencia = rProveedor.field("lRecargoEquivalencia").value().toInt();
            this->tTextoparaPedidos = rProveedor.field("tTextoparaPedidos").value().toString();
            this->rEntregadoaCuenta = rProveedor.field("rEntregadoaCuenta").value().toDouble();
        } else {
            QMessageBox::information(NULL,QObject::tr("Gestión Proveedores"),QObject::tr("No se encuentra el proveedor"),
                                         QObject::tr("Ok"));

            }

    } else
     QMessageBox::warning(NULL,QObject::tr("Gestión Proveedores"),QObject::tr("Problema en la BD no se puede recuperar el proveedor Error: ")+
                                 qProveedor->lastError().text() ,QObject::tr("Ok"));
}

void Proveedor::Recuperar(QString cSQL, int nProcede)
{
    QSqlQuery *qProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
    if(qProveedor->exec(cSQL)) {
        if(qProveedor->next()) {
            QSqlRecord rProveedor = qProveedor->record();
            this->id = rProveedor.field("Id").value().toInt();
            this->cCodigo = rProveedor.field("cCodigo").value().toString();
            this->cProveedor = rProveedor.field("cProveedor").value().toString();
            this->cCif = rProveedor.field("cCif").value().toString();
            this->cDireccion1 = rProveedor.field("cDireccion1").value().toString();
            this->cDireccion2 = rProveedor.field("cDireccion2").value().toString();
            this->cCP = rProveedor.field("cCP").value().toString();
            this->cPoblacion = rProveedor.field("cPoblacion").value().toString();
            this->cProvincia = rProveedor.field("cProvincia").value().toString();
            this->cPais = rProveedor.field("cPais").value().toString();
            this->cTelefono1 = rProveedor.field("cTelefono1").value().toString();
            this->cTelefono2 = rProveedor.field("cTelefono2").value().toString();
            this->cTelefono3 = rProveedor.field("cTelefono3").value().toString();
            this->cFax = rProveedor.field("cFax").value().toString();
            this->cMovil = rProveedor.field("cMovil").value().toString();
            this->cEMail = rProveedor.field("cEMail").value().toString();
            this->cWeb = rProveedor.field("cWeb").value().toString();
            this->cPersonaContacto = rProveedor.field("cPersonaContacto").value().toString();
            this->nDiaCobro = rProveedor.field("nDiaCobro").value().toInt();
            this->cDireccionAlmacen = rProveedor.field("cDireccionAlmacen").value().toString();
            this->cCPAlmacen = rProveedor.field("cCPAlmacen").value().toString();
            this->cPoblacionAlmacen = rProveedor.field("cPoblacionAlmacen").value().toString();
            this->cProvinciaAlmacen = rProveedor.field("cProvinciaAlmacen").value().toString();
            this->cPaisAlmacen = rProveedor.field("cPaisAlmacen").value().toString();
            this->cTelefonoAlmacen = rProveedor.field("cTelefonoAlmacen").value().toString();
            this->cFaxAlmacen = rProveedor.field("cFaxAlmacen").value().toString();
            this->cCodigoFormaPago = rProveedor.field("cCodigoFormaPago").value().toString();
            this->dFechaUltimaCompra = rProveedor.field("dFechaUltimaCompra").value().toDate();
            this->rAcumuladoCompras = rProveedor.field("rAcumuladoCompras").value().toDouble();
            this->cEntidadBancariaProveedor = rProveedor.field("cEntidadBancariaProveedor").value().toString();
            this->cOficinaBancariaProveedor = rProveedor.field("cOficinaBancariaProveedor").value().toString();
            this->cDCProveedor = rProveedor.field("cDCProveedor").value().toString();
            this->cCCProveedor = rProveedor.field("cCCProveedor").value().toString();
            this->cEntidadPagoProveedor = rProveedor.field("cEntidadPagoProveedor").value().toString();
            this->cOficinaPagoProveedor = rProveedor.field("cOficinaPagoProveedor").value().toString();
            this->cDCPagoProveedor = rProveedor.field("cDCPagoProveedor").value().toString();
            this->cCuentaPagoProveedor = rProveedor.field("cCuentaPagoProveedor").value().toString();
            this->cCuentaIvaSoportado = rProveedor.field("cCuentaIvaSoportado").value().toString();
            this->rRetencionIRPF = rProveedor.field("rRetencionIRPF").value().toDouble();
            this->nTipoRetencion = rProveedor.field("nTipoRetencion").value().toInt();
            this->cCuentaAplicacion = rProveedor.field("cCuentaAplicacion").value().toString();
            this->cCuentaPagos = rProveedor.field("cCuentaPagos").value().toString();
            this->tComentarios = rProveedor.field("tComentarios").value().toString();
            this->nDto = rProveedor.field("nDto").value().toDouble();
            this->dFechaAlta = rProveedor.field("dFechaAlta").value().toDate();
            this->rDeudaMaxima = rProveedor.field("rDeudaMaxima").value().toDouble();
            this->rDeudaActual = rProveedor.field("rDeudaActual").value().toDouble();
            this->lRecargoEquivalencia = rProveedor.field("lRecargoEquivalencia").value().toInt();
            this->tTextoparaPedidos = rProveedor.field("tTextoparaPedidos").value().toString();
            this->rEntregadoaCuenta = rProveedor.field("rEntregadoaCuenta").value().toDouble();
        } else {
            if (nProcede ==1) {
                QMessageBox::information(NULL,QObject::tr("Gestión Proveedores"),QObject::tr("Se ha llegado al último proveedor"),
                                         QObject::tr("Ok"));
            } else {
                QMessageBox::information(NULL,QObject::tr("Gestión Proveedores"),QObject::tr("Se ha llegado al primer proveedor"),
                                         QObject::tr("Ok"));
            }
        }
    } else
     QMessageBox::warning(NULL,QObject::tr("Gestión Proveedores"),QObject::tr("Problema en la BD no se puede recuperar el proveedor Error: ")+
                                 qProveedor->lastError().text() ,QObject::tr("Ok"));
}


void Proveedor::Guardar()
{
    QSqlQuery *qProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
    qProveedor->prepare("UPDATE proveedores SET "
                        "cCCProveedor = :cCCProveedor,"
                        "cCif = :cCif,"
                        "cCodigo = :cCodigo,"
                        "cCodigoFormaPago =:cCodigoFormaPago,"
                        "cCP =:cCP,"
                        "cCPAlmacen=:cCPAlmacen,"
                        "cCuentaAplicacion = :cCuentaAplicacion ,"
                        "cCuentaIvaSoportado =:cCuentaIvaSoportado,"
                        "cCuentaPagoProveedor =:cCuentaPagoProveedor,"
                        "cCuentaPagos =:cCuentaPagos,"
                        "cDCPagoProveedor =:cDCPagoProveedor,"
                        "cDCProveedor =:cDCProveedor,"
                        "cDireccion1 =:cDireccion1,"
                        "cDireccion2 =:cDireccion2,"
                        "cDireccionAlmacen =:cDireccionAlmacen,"
                        "cEMail =:cEMail,"
                        "cEntidadBancariaProveedor =:cEntidadBancariaProveedor,"
                        "cEntidadPagoProveedor =:cEntidadPagoProveedor,"
                        "cFax = :cFax,"
                        "cFaxAlmacen=:cFaxAlmacen,"
                        "cMovil = :cMovil,"
                        "cOficinaBancariaProveedor = :cOficinaBancariaProveedor,"
                        "cOficinaPagoProveedor =:cOficinaPagoProveedor,"
                        "cPais = :cPais,"
                        "cPaisAlmacen = :cPaisAlmacen,"
                        "cPersonaContacto =:cPersonaContacto,"
                        "cPoblacion =:cPoblacion,"
                        "cPoblacionAlmacen =:cPoblacionAlmacen,"
                        "cProveedor = :cProveedor,"
                        "cProvincia = :cProvincia,"
                        "cProvinciaAlmacen=:cProvinciaAlmacen,"
                        "cTelefono1 =:cTelefono1,"
                        "cTelefono2 =:cTelefono2,"
                        "cTelefono3 = :cTelefono3,"
                        "cTelefonoAlmacen =:cTelefonoAlmacen,"
                        "cWeb = :cWeb,"
                        "dFechaAlta =:dFechaAlta,"
                        "dFechaUltimaCompra =:dFechaUltimaCompra,"
                        "lRecargoEquivalencia = :lRecargoEquivalencia,"
                        "nDiaCobro = :nDiaCobro,"
                        "nDto = :nDto,"
                        "nTipoRetencion = :nTipoRetencion,"
                        "rAcumuladoCompras = :rAcumuladoCompras,"
                        "rDeudaActual = :rDeudaActual,"
                        "rDeudaMaxima = :rDeudaMaxima,"
                        "rEntregadoaCuenta = :rEntregadoaCuenta,"
                        "rRetencionIRPF = :rRetencionIRPF,"
                        "tComentarios= :tComentarios,"
                        "tTextoparaPedidos = :tTextoparaPedidos "
                        "WHERE  id = :nId");

    qProveedor->bindValue(":cCCProveedor",this->cCCProveedor);
    qProveedor->bindValue(":cCif",this->cCif);
    qProveedor->bindValue(":cCodigo",this->cCodigo);
    qProveedor->bindValue(":cCodigoFormaPago",this->cCodigoFormaPago);
    qProveedor->bindValue(":cCP",this->cCP);
    qProveedor->bindValue(":cCPAlmacen",this->cCPAlmacen);
    qProveedor->bindValue(":cCuentaAplicacion",this->cCuentaAplicacion);
      qProveedor->bindValue(":cCuentaIvaSoportado",this->cCuentaIvaSoportado);
      qProveedor->bindValue(":cCuentaPagoProveedor", this->cCuentaPagoProveedor);
      qProveedor->bindValue(":cCuentaPagos",this->cCuentaPagos);
      qProveedor->bindValue(":cDCPagoProveedor",this->cDCPagoProveedor);
      qProveedor->bindValue(":cDCProveedor", this->cDCProveedor);
      qProveedor->bindValue(":cDireccion1", this->cDireccion1);
      qProveedor->bindValue(":cDireccion2",this->cDireccion2);
      qProveedor->bindValue(":cDireccionAlmacen", this->cDireccionAlmacen);
      qProveedor->bindValue(":cEMail",this->cEMail);
      qProveedor->bindValue(":cEntidadBancariaProveedor",this->cEntidadBancariaProveedor);
      qProveedor->bindValue(":cEntidadPagoProveedor",this->cEntidadBancariaProveedor);
      qProveedor->bindValue(":cFax",this->cFax);
      qProveedor->bindValue(":cFaxAlmacen", this->cFaxAlmacen);
      qProveedor->bindValue(":cMovil", this->cFaxAlmacen);
      qProveedor->bindValue(":cOficinaBancariaProveedor", this->cOficinaBancariaProveedor);
      qProveedor->bindValue(":cOficinaPagoProveedor", this->cOficinaPagoProveedor);
      qProveedor->bindValue(":cPais", this->cPais);
      qProveedor->bindValue(":cPaisAlmacen",this->cPaisAlmacen);
      qProveedor->bindValue(":cPersonaContacto", this->cPersonaContacto);
      qProveedor->bindValue(":cPoblacion",this->cPoblacion);
      qProveedor->bindValue(":cPoblacionAlmacen", this->cPoblacionAlmacen);
      qProveedor->bindValue(":cProveedor", this->cProveedor);
      qProveedor->bindValue(":cProvincia", this->cProvincia);
      qProveedor->bindValue(":cProvinciaAlmacen", this->cProvinciaAlmacen);
      qProveedor->bindValue(":cTelefono1",this->cTelefono1);
      qProveedor->bindValue(":cTelefono2", this->cTelefono2);
      qProveedor->bindValue(":cTelefono3", this->cTelefono3);
      qProveedor->bindValue(":cTelefonoAlmacen", this->cTelefonoAlmacen);
      qProveedor->bindValue(":cWeb", this->cWeb);
      qProveedor->bindValue(":dFechaAlta", this->dFechaAlta);
      qProveedor->bindValue(":dFechaUltimaCompra", this->dFechaUltimaCompra);
      qProveedor->bindValue(":lRecargoEquivalencia",this->lRecargoEquivalencia);
      qProveedor->bindValue(":nDiaCobro",this->nDiaCobro);
      qProveedor->bindValue(":nDto",this->nDto);
      qProveedor->bindValue(":nTipoRetencion", this->nTipoRetencion);
      qProveedor->bindValue(":rAcumuladoCompras",this->rAcumuladoCompras);
      qProveedor->bindValue(":rDeudaActual", this->rDeudaActual);
      qProveedor->bindValue(":rDeudaMaxima", this->rDeudaMaxima);
      qProveedor->bindValue(":rEntregadoaCuenta", this->rEntregadoaCuenta);
      qProveedor->bindValue(":rRetencionIRPF", this->rRetencionIRPF);
      qProveedor->bindValue(":tComentarios", this->tComentarios);
      qProveedor->bindValue(":tTextoparaPedidos", this->tTextoparaPedidos);
      qProveedor->bindValue(":nId",this->id);
    if(!qProveedor->exec())
        QMessageBox::warning(NULL,QObject::tr("Gestión proveedores/Acreedores"),
                             QObject::tr("No se ha podido insertar el proveedor en la BD"),QObject::tr("Ok"));
    }

void Proveedor::Vaciar()
{
    this->id = 0;
    this->cCodigo = "";
    this->cProveedor = "";
    this->cCif = "";
    this->cDireccion1 = "";
    this->cDireccion2 = "";
    this->cCP = "";
    this->cPoblacion = "";
    this->cProvincia = "";
    this->cPais = "";
    this->cTelefono1 = "";
    this->cTelefono2 = "";
    this->cTelefono3 = "";
    this->cFax = "";
    this->cMovil = "";
    this->cEMail ="";
    this->cWeb = "";
    this->cPersonaContacto = "";
    this->nDiaCobro = 0;
    this->cDireccionAlmacen = "";
    this->cCPAlmacen = "";
    this->cPoblacionAlmacen = "";
    this->cProvinciaAlmacen = "";
    this->cPaisAlmacen = "";
    this->cTelefonoAlmacen = "";
    this->cFaxAlmacen = "";
    this->cCodigoFormaPago = "";
    this->dFechaUltimaCompra = QDate::currentDate();
    this->rAcumuladoCompras = 0;
    this->cEntidadBancariaProveedor ="";
    this->cOficinaBancariaProveedor = "";
    this->cDCProveedor = "";
    this->cCCProveedor = "";
    this->cEntidadPagoProveedor = "";
    this->cOficinaPagoProveedor = "";
    this->cDCPagoProveedor = "";
    this->cCuentaPagoProveedor = "";
    this->cCuentaIvaSoportado = "";
    this->rRetencionIRPF = 0;
    this->nTipoRetencion = 0;
    this->cCuentaAplicacion = "";
    this->cCuentaPagos = "";
    this->tComentarios = "";
    this->nDto = 0;
    this->dFechaAlta = QDate::currentDate();
    this->rDeudaMaxima = 0;
    this->rDeudaActual = 0;
    this->lRecargoEquivalencia = 0;
    this->tTextoparaPedidos = "";
    this->rEntregadoaCuenta = 0;
}

void Proveedor::Borrar(int nId)
{
    QSqlQuery *qProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
    qProveedor->prepare("delete from proveedores where Id = "+QString::number(nId));
    if(qProveedor->exec())
        QMessageBox::information(NULL,QObject::tr("Gestión de Proveedores"),QObject::tr("Se ha borrado la ficha del proveedor"),
                                 QObject::tr("Aceptar"));
   else
        QMessageBox::warning(NULL,QObject::tr("Gestión de Proveedores"),QObject::tr("No Se ha borrado la ficha del proveedor ERROR: ")+
                             qProveedor->lastError().text(),QObject::tr("Aceptar"));
    delete qProveedor;
}

QString Proveedor::NuevoCodigoProveedor()
{
    Configuracion *oConfig = new Configuracion();
    oConfig->CargarDatos();
    QString cCodigo;
    QString cNum;
    int nCodigo;
    QSqlQuery *qProveedores = new QSqlQuery(QSqlDatabase::database("empresa"));
    if(qProveedores->exec("select cCodigo from proveedores  order by cCodigo desc limit 1")) {
        if (qProveedores->next()) {
            QSqlRecord registro = qProveedores->record();
            cCodigo = registro.field("cCodigo").value().toString();
            nCodigo = cCodigo.toInt();
            nCodigo ++;
            cCodigo = QString::number(nCodigo);
        }
   }
   if (nCodigo == 0 || nCodigo == 1) {
        cNum = "1";
        while (cNum.length()< (oConfig->nDigitosCuentasContables - oConfig->cCuentaProveedores.length()) ) {
            cNum.prepend("0");
        }
        cCodigo = oConfig->cCuentaProveedores + cNum;
    }
   return cCodigo;
}

int Proveedor::getid()
{
    return this->id;
}

QString Proveedor::getcCodigo()
{
    return this->cCodigo;
}

QString Proveedor::getcProveedor()
{
    return this->cProveedor;
}

QString Proveedor::getcCif()
{
    return this->cCif;
}

QString Proveedor::getcDireccion1()
{
    return cDireccion1;
}

QString Proveedor::getcDireccion2()
{
    return cDireccion2;
}

QString Proveedor::getcCP()
{
    return this->cCP;
}

QString Proveedor::getcPoblacion()
{
    return this->cPoblacion;
}

QString Proveedor::getcProvincia()
{
    return this->cProvincia;
}

QString Proveedor::getcPais()
{
    return this->cPais;
}

QString Proveedor::getcTelefono1()
{
    return this->cTelefono1;
}

QString Proveedor::getcTelefono2()
{
    return this->cTelefono2;
}

QString Proveedor::getcTelefono3()
{
    return this->cTelefono3;
}

QString Proveedor::getcFax()
{
    return this->cFax;
}

QString Proveedor::getcMovil()
{
    return this->cMovil;
}

QString Proveedor::getcEMail()
{
    return this->cEMail;
}

QString Proveedor::getcWeb()
{
    return this->cWeb;
}

QString Proveedor::getcPersonaContacto()
{
    return this->cPersonaContacto;
}

int Proveedor::getnDiaCobro()
{
    return this->nDiaCobro;
}

QString Proveedor::getcDireccionAlmacen()
{
    return this->cDireccionAlmacen;
}

QString Proveedor::getcCPAlmacen()
{
    return this->cCPAlmacen;
}

QString Proveedor::getcPoblacionAlmacen()
{
    return this->cPoblacionAlmacen;

}

QString Proveedor::getcProvinciaAlmacen()
{
    return this->cProvinciaAlmacen;
}

QString Proveedor::getcPaisAlmacen()
{
    return this->cPaisAlmacen;
}

QString Proveedor::getcTelefonoAlmacen()
{
    return this->cTelefonoAlmacen;
}

QString Proveedor::getcFaxAlmacen()
{
    return this->cFaxAlmacen;
}

QString Proveedor::getcCodigoFormaPago()
{
    return this->cCodigoFormaPago;
}

QDate Proveedor::getdFechaUltimaCompra()
{
    return this->dFechaUltimaCompra;
}

double Proveedor::getrAcumuladoCompras()
{
    return this->rAcumuladoCompras;
}

QString Proveedor::getcEntidadBancariaProveedor()
{
    return this->cEntidadBancariaProveedor;
}

QString Proveedor::getcOficinaBancariaProveedor()
{
    return this->cOficinaBancariaProveedor;
}

QString Proveedor::getcDCProveedor()
{
    return this->cDCProveedor;
}

QString Proveedor::getcCCProveedor()
{
    return this->cCCProveedor;
}

QString Proveedor::getcEntidadPagoProveedor()
{
    return this->cEntidadPagoProveedor;
}

QString Proveedor::getcOficinaPagoProveedor()
{
    return this->cOficinaPagoProveedor;
}

QString Proveedor::getcDCPagoProveedor()
{
    return this->cDCPagoProveedor;
}

QString Proveedor::getcCuentaPagoProveedor()
{
    return this->cCuentaPagoProveedor;
}

QString Proveedor::getcCuentaIvaSoportado()
{
    return this->cCuentaIvaSoportado;
}

double Proveedor::getrRetencionIRPF()
{
    return this->rRetencionIRPF;
}

int Proveedor::getnTipoRetencion()
{
    return this->nTipoRetencion;
}

QString Proveedor::getcCuentaAplicacion()
{
    return this->cCuentaAplicacion;
}

QString Proveedor::getcCuentaPagos()
{
    return this->cCuentaPagos;

}

QString Proveedor::gettComentarios()
{
    return this->tComentarios;
}

double Proveedor::getnDto()
{
    return this->nDto;
}

QDate Proveedor::getdFechaAlta()
{
    return this->dFechaAlta;
}

double Proveedor::getrDeudaMaxima()
{
    return this->rDeudaMaxima;
}

double Proveedor::getrDeudaActual()
{
    return this->rDeudaActual;
}

int Proveedor::getlRecargoEquivalencia()
{
    return this->lRecargoEquivalencia;
}

QString Proveedor::gettTextoparaPedidos()
{
    return this->tTextoparaPedidos;
}

double Proveedor::getrEntregadoaCuenta()
{
    return this->rEntregadoaCuenta;
}


/***************************************************
// SETTERS
***************************************************/
void Proveedor::setId(int id)
{
    this->id = id;
}

void Proveedor::setcCodigo(QString cCodigo)
{
    this->cCodigo = cCodigo;
}

void Proveedor::setcProveedor(QString cProveedor)
{
    this->cProveedor = cProveedor;
}

void Proveedor::setcCif(QString cCif)
{
    this->cCif = cCif;
}

void Proveedor::setcDireccion1(QString cDireccion1)
{
    this->cDireccion1 = cDireccion1;
}

void Proveedor::setcDireccion2(QString cDireccion2)
{
    this->cDireccion2 = cDireccion2;
}

void Proveedor::setcCP(QString cCP)
{
    this->cCP = cCP;
}

void Proveedor::setcPoblacion(QString cPoblacion)
{
    this->cPoblacion = cPoblacion;
}

void Proveedor::setcProvincia(QString cProvincia)
{
    this->cProvincia = cProvincia;
}

void Proveedor::setcPais(QString cPais)
{
    this->cPais = cPais;
}

void Proveedor::setcTelefono1(QString cTelefono1)
{
    this->cTelefono1 = cTelefono1;
}

void Proveedor::setcTelefono2(QString cTelefono2)
{
    this->cTelefono2 = cTelefono2;
}

void Proveedor::setcTelefono3(QString cTelefono3)
{
    this->cTelefono3 = cTelefono3;
}

void Proveedor::setcFax(QString cFax)
{
    this->cFax = cFax;
}

void Proveedor::setcMovil(QString cMovil)
{
    this->cMovil = cMovil;
}

void Proveedor::setcEMail(QString cEMail)
{
    this->cEMail = cEMail;
}

void Proveedor::setcWeb(QString cWeb)
{
    this->cWeb = cWeb;
}

void Proveedor::setcPersonaContacto(QString cPersonaContacto)
{
    this->cPersonaContacto = cPersonaContacto;
}

void Proveedor::setnDiaCobro(int nDiaCobro)
{
    this->nDiaCobro = nDiaCobro;
}

void Proveedor::setcDireccionAlmacen(QString cDireccionAlmacen)
{
    this->cDireccionAlmacen = cDireccionAlmacen;
}

void Proveedor::setcCPAlmacen(QString cCPAlmacen)
{
    this->cCPAlmacen = cCPAlmacen;
}

void Proveedor::setcPoblacionAlmacen(QString cPoblacionAlmacen)
{
    this->cPoblacionAlmacen = cPoblacionAlmacen;
}

void Proveedor::setcProvinciaAlmacen(QString cProvinciaAlmacen)
{
    this->cProvinciaAlmacen = cProvinciaAlmacen;
}

void Proveedor::setcPaisAlmacen(QString cPaisAlmacen)
{
    this->cPaisAlmacen = cPaisAlmacen;
}

void Proveedor::setcTelefonoAlmacen(QString cTelefonoAlmacen)
{
    this->cTelefonoAlmacen = cTelefonoAlmacen;
}

void Proveedor::setcFaxAlmacen(QString cFaxAlmacen)
{
    this->cFaxAlmacen = cFaxAlmacen;

}

void Proveedor::setcCodigoFormaPago(QString cCodigoFormaPago)
{
    this->cCodigoFormaPago = cCodigoFormaPago;
}

void Proveedor::setdFechaUltimaCompra(QDate dFechaUltimaCompra)
{
    this->dFechaUltimaCompra = dFechaUltimaCompra;
}

void Proveedor::setrAcumuladoCompras(double rAcumuladoCompras)
{
    this->rAcumuladoCompras = rAcumuladoCompras;
}

void Proveedor::setcEntidadBancariaProveedor(QString cEntidadBancariaProveedor)
{
    this->cEntidadBancariaProveedor = cEntidadBancariaProveedor;
}

void Proveedor::setcOficinaBancariaProveedor(QString cOficinaBancariaProveedor)
{
    this->cOficinaBancariaProveedor = cOficinaBancariaProveedor;
}

void Proveedor::setcDCProveedor(QString cDCProveedor)
{
    this->cDCProveedor = cDCProveedor;
}

void Proveedor::setcCCProveedor(QString cCCProveedor)
{
    this->cCCProveedor = cCCProveedor;
}

void Proveedor::setcEntidadPagoPro(QString cEntidadPagoProveedor)
{
    this->cEntidadPagoProveedor = cEntidadPagoProveedor;
}

void Proveedor::setcOficinaPagoProveedor(QString cOficinaPagoProveedor)
{
    this->cOficinaPagoProveedor = cOficinaPagoProveedor;
}

void Proveedor::setcDCPagoProveedor(QString cDCPagoProveedor)
{
    this->cDCPagoProveedor = cDCPagoProveedor;
}

void Proveedor::setcCuentaPagoProveedor(QString cCuentaPagoProveedor)
{
    this->cCuentaPagoProveedor = cCuentaPagoProveedor;
}

void Proveedor::setcCuentaIvaSoportado(QString cCuentaIvaSoportado)
{
    this->cCuentaIvaSoportado = cCuentaIvaSoportado;
}

void Proveedor::setrRetencion(double rRetencionIRPF)
{
    this->rRetencionIRPF = rRetencionIRPF;
}

void Proveedor::setnTipoRetencion(int nTipoRetencion)
{
    this->nTipoRetencion = nTipoRetencion;
}

void Proveedor::setcCuentaAplicacion(QString cCuentaAplicacion)
{
    this->cCuentaAplicacion = cCuentaAplicacion;
}

void Proveedor::setcCuentaPagos(QString cCuentaPagos)
{
    this->cCuentaPagos = cCuentaPagos;
}

void Proveedor::settComentarios(QString tComentarios)
{
    this->tComentarios = tComentarios;
}

void Proveedor::setnDto(double nDto)
{
    this->nDto = nDto;
}

void Proveedor::setdFechaAlta(QDate dFechaAlta)
{
    this->dFechaAlta = dFechaAlta;
}

void Proveedor::setrDeudaMaxima(double rDeudaMaxima)
{
    this->rDeudaMaxima = rDeudaMaxima;
}

void Proveedor::setrDeudaActual(double rDeudaActual)
{
    this->rDeudaActual = rDeudaActual;
}

void Proveedor::setlRecargoEquivalencia(int lRecargoEquivalencia)
{
    this->lRecargoEquivalencia = this->lRecargoEquivalencia;
}

void Proveedor::settTextoparaPedidos(QString tTextoparaPedidos)
{
    this->tTextoparaPedidos = tTextoparaPedidos;
}

void Proveedor::setrEntregadoaCuenta(double rEntregadoaCuenta)
{
    this->rEntregadoaCuenta = rEntregadoaCuenta;
}


