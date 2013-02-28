#include "proveedor.h"



Proveedor::Proveedor(QObject *parent) :
    QObject(parent)
{
    this->id = 0;
}

void Proveedor::Anadir()
{
    QScopedPointer<QSqlQuery>QProveedor(new QSqlQuery(QSqlDatabase::database("empresa")));
    //QSqlQuery *QProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
    if (!QProveedor->exec("insert into proveedores (cCodigo) values('')"))
            QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión de proveedores"),
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
    QScopedPointer<QSqlQuery>qProveedor(new QSqlQuery(QSqlDatabase::database("empresa")));
   // QSqlQuery *qProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
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
            QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión Proveedores"),QObject::tr("No se encuentra el proveedor"),
                                         QObject::tr("Ok"));

            }

    } else
     QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión Proveedores"),QObject::tr("Problema en la BD no se puede recuperar el proveedor Error: ")+
                                 qProveedor->lastError().text() ,QObject::tr("Ok"));
}

void Proveedor::Recuperar(QString cSQL, int nProcede)
{
    QScopedPointer<QSqlQuery>qProveedor(new QSqlQuery(QSqlDatabase::database("empresa")));
    //QSqlQuery *qProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
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
                QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión Proveedores"),QObject::tr("Se ha llegado al último proveedor"),
                                         QObject::tr("Ok"));
            } else {
                QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión Proveedores"),QObject::tr("Se ha llegado al primer proveedor"),
                                         QObject::tr("Ok"));
            }
        }
    } else
     QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión Proveedores"),QObject::tr("Problema en la BD no se puede recuperar el proveedor Error: ")+
                                 qProveedor->lastError().text() ,QObject::tr("Ok"));
}


void Proveedor::Guardar()
{
    QScopedPointer<QSqlQuery>qProveedor(new QSqlQuery(QSqlDatabase::database("empresa")));
    //QSqlQuery *qProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
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
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión proveedores/Acreedores"),
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
    QScopedPointer<QSqlQuery>qProveedor(new QSqlQuery(QSqlDatabase::database("empresa")));
    //QSqlQuery *qProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
    qProveedor->prepare("delete from proveedores where Id = "+QString::number(nId));
    if(qProveedor->exec())
        QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión de Proveedores"),QObject::tr("Se ha borrado la ficha del proveedor"),
                                 QObject::tr("Aceptar"));
   else
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión de Proveedores"),QObject::tr("No Se ha borrado la ficha del proveedor ERROR: ")+
                             qProveedor->lastError().text(),QObject::tr("Aceptar"));
}

QString Proveedor::NuevoCodigoProveedor()
{
    QString cCodigo;
    QString cNum;
    int nCodigo;
    QScopedPointer<QSqlQuery>qProveedores(new QSqlQuery(QSqlDatabase::database("empresa")));
    //QSqlQuery *qProveedores = new QSqlQuery(QSqlDatabase::database("empresa"));
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
        while (cNum.length()< (Configuracion_global->nDigitosCuentasContables - Configuracion_global->cCuentaProveedores.length()) ) {
            cNum.prepend("0");
        }
        cCodigo = Configuracion_global->cCuentaProveedores + cNum;
    }
   return cCodigo;
}


