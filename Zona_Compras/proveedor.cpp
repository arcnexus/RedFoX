#include "proveedor.h"



Proveedor::Proveedor(QObject *parent) :
    QObject(parent)
{
    this->id = 0;
}

void Proveedor::cargaracumulados(int id_proveedor)
{
    QSqlQuery query_acumulados(QSqlDatabase::database("empresa"));
    if(query_acumulados.exec("select * from acum_proveedores where id_proveedor = "+QString::number(id_proveedor)))
    {
        query_acumulados.next();
        this->enero = query_acumulados.record().value("acum_enero").toDouble();
        this->febrero = query_acumulados.record().value("acum_febrero").toDouble();
        this->marzo = query_acumulados.record().value("acum_marzo").toDouble();
        this->abril = query_acumulados.record().value("acum_abril").toDouble();
        this->mayo = query_acumulados.record().value("acum_mayo").toDouble();
        this->junio = query_acumulados.record().value("acum_junio").toDouble();
        this->julio = query_acumulados.record().value("acum_julio").toDouble();
        this->agosto = query_acumulados.record().value("acum_agosto").toDouble();
        this->septiembre = query_acumulados.record().value("acum_septiembre").toDouble();
        this->octubre = query_acumulados.record().value("acum_octubre").toDouble();
        this->noviembre = query_acumulados.record().value("acum_noviembre").toDouble();
        this->diciembre = query_acumulados.record().value("acum_diciembre").toDouble();
    }


}

bool Proveedor::acumular(int id_proveedor, int mes, double importe)
{

}

void Proveedor::anadir_persona_contacto(int Id, QString Nombre, QString descTelefono1, QString Telefono1,
                                        QString descTelefono2, QString Telefono2, QString descTelefono3, QString Telefono3,
                                        QString descMovil1, QString Movil1, QString descMovil2, QString Movil2,QString cargo)
    {
        QSqlQuery qContactos(QSqlDatabase::database("Maya"));
        qContactos.prepare("INSERT INTO personascontactoproveedor "
                           "(nombre,"
                           "telefono1,"
                           "telefono2,"
                           "telefono3,"
                           "movil1,"
                           "movil2,"
                           "idproveedor,"
                           "desctelefono1,"
                           "desctelefono2,"
                           "desctelefono3,"
                           "descmovil1,"
                           "descmovil2,"
                           "cargo_empresa)"
                           " VALUES ("
                          ":nombre,"
                          ":telefono1,"
                          ":telefono2,"
                          ":telefono3,"
                          ":movil1,"
                          ":movil2,"
                          ":idproveedor,"
                          ":desctelefono1,"
                          ":desctelefono2,"
                          ":desctelefono3,"
                          ":descmovil1,"
                          ":descmovil2,"
                           ":cargo);");


                           qContactos.bindValue(":nombre",Nombre);
                           qContactos.bindValue(":telefono1",Telefono1);
                           qContactos.bindValue( ":telefono2",Telefono2);
                           qContactos.bindValue(":telefono3",Telefono3);
                           qContactos.bindValue(":movil",Movil1);
                           qContactos.bindValue(":movil2",Movil2);
                           qContactos.bindValue(":idproveedor",Id);
                           qContactos.bindValue(":desctelefono1",descTelefono1);
                           qContactos.bindValue(":desctelefono2",descTelefono2);
                           qContactos.bindValue(":desctelefono3",descTelefono3);
                           qContactos.bindValue(":descmovil1",descMovil1);
                           qContactos.bindValue(":descmovil2",descMovil2);
                           qContactos.bindValue(":cargo",cargo);
                           if(!qContactos.exec())
                               QMessageBox::warning(qApp->activeWindow(),tr("Añadir personas de contacto"),
                                                    tr("Falló el añadir una persona de contacto: %1").arg(qContactos.lastError().text()));
}

void Proveedor::guardar_persona_contacto(int Id, QString Nombre, QString descTelefono1, QString Telefono1, QString descTelefono2, QString Telefono2, QString descTelefono3, QString Telefono3, QString descMovil1, QString Movil1, QString descMovil2, QString Movil2, QString cargo)
{
    QSqlQuery qContactos(QSqlDatabase::database("Maya"));
    qContactos.prepare("UPDATE personascontactoproveedor SET "
    "nombre =:nombre,"
    "telefono1 =:telefono1,"
    "telefono2 =:telefono2,"
    "telefono3 =:telefono3,"
    "movil1 =:movil1,"
    "movil2 =:movil2,"
    "desctelefono1 =:desctelefono1,"
    "desctelefono2 =:desctelefono2,"
    "desctelefono3 =:desctelefono3,"
    "descmovil1 =:descmovil1,"
    "descmovil2 =:descmovil2,"
    "cargo_empresa =:cargo_empresa "
    " where id = :id");


    qContactos.bindValue(":nombre",Nombre);
    qContactos.bindValue(":telefono1",Telefono1);
    qContactos.bindValue( ":telefono2",Telefono2);
    qContactos.bindValue(":telefono3",Telefono3);
    qContactos.bindValue(":movil",Movil1);
    qContactos.bindValue(":movil2",Movil2);
    qContactos.bindValue(":idproveedor",Id);
    qContactos.bindValue(":desctelefono1",descTelefono1);
    qContactos.bindValue(":desctelefono2",descTelefono2);
    qContactos.bindValue(":desctelefono3",descTelefono3);
    qContactos.bindValue(":descmovil1",descMovil1);
    qContactos.bindValue(":descmovil2",descMovil2);
    qContactos.bindValue(":cargo",cargo);
    qContactos.bindValue(":id",Id);
    if(!qContactos.exec())
        QMessageBox::warning(qApp->activeWindow(),tr("Editar personas de contacto"),
                             tr("Falló el guardar una persona de contacto: %1").arg(qContactos.lastError().text()));
}

void Proveedor::Anadir()
{
    QScopedPointer<QSqlQuery>QProveedor(new QSqlQuery(QSqlDatabase::database("Maya")));
    //QSqlQuery *QProveedor = new QSqlQuery(QSqlDatabase::database("Maya"));
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
    QScopedPointer<QSqlQuery>qProveedor(new QSqlQuery(QSqlDatabase::database("Maya")));
   // QSqlQuery *qProveedor = new QSqlQuery(QSqlDatabase::database("Maya"));
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
            this->idpais = rProveedor.field("id_pais").value().toInt();
            this->cPais =  Configuracion_global->Devolver_pais(this->idpais);
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
            this->idPaisAlmacen = rProveedor.field("idPaisAlmacen").value().toInt();
            this->cPaisAlmacen = Configuracion_global->Devolver_pais(this->idPaisAlmacen);
            this->cTelefonoAlmacen = rProveedor.field("cTelefonoAlmacen").value().toString();
            this->cFaxAlmacen = rProveedor.field("cFaxAlmacen").value().toString();
            this->idFormadePago = rProveedor.field("idFormaPago").value().toInt();
            this->cCodigoFormaPago = Configuracion_global->Devolver_codigo_forma_pago(this->idFormadePago);
            this->dFechaUltimaCompra = rProveedor.field("dFechaUltimaCompra").value().toDate();
            this->rAcumuladoCompras = rProveedor.field("rAcumuladoCompras").value().toDouble();
            this->cEntidadBancariaProveedor = rProveedor.field("cEntidadBancariaProveedor").value().toString();
            this->cOficinaBancariaProveedor = rProveedor.field("cOficinaBancariaProveedor").value().toString();
            this->cDCProveedor = rProveedor.field("cDCProveedor").value().toString();
            this->cCCProveedor = rProveedor.field("cCCProveedor").value().toString();
            this->cEntidadPagoProveedor = rProveedor.field("cEntidadPagoProveedor").value().toString();
            this->cOficinaPagoProveedor = rProveedor.field("cOficinaPagoProveedor").value().toString();
            this->cDCPagoProveedor = rProveedor.field("cDCPagoProveedor").value().toString();
            this->rRetencionIRPF = rProveedor.field("rRetencionIRPF").value().toDouble();
            this->nTipoRetencion = rProveedor.field("nTipoRetencion").value().toInt();
            this->cCuentaAplicacion = rProveedor.field("cCuentaAplicacion").value().toString();
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
    cargaracumulados(this->id);
}

void Proveedor::Recuperar(QString cSQL, int nProcede)
{
    QScopedPointer<QSqlQuery>qProveedor(new QSqlQuery(QSqlDatabase::database("Maya")));
    //QSqlQuery *qProveedor = new QSqlQuery(QSqlDatabase::database("Maya"));
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
            this->idpais = rProveedor.field("id_pais").value().toInt();
            this->cPais =  Configuracion_global->Devolver_pais(this->idpais);
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
            this->idPaisAlmacen = rProveedor.field("idPaisAlmacen").value().toInt();
            this->cPaisAlmacen = Configuracion_global->Devolver_pais(this->idPaisAlmacen);
            this->cTelefonoAlmacen = rProveedor.field("cTelefonoAlmacen").value().toString();
            this->cFaxAlmacen = rProveedor.field("cFaxAlmacen").value().toString();
            this->idFormadePago = rProveedor.field("idFormaPago").value().toInt();
            this->cCodigoFormaPago = Configuracion_global->Devolver_codigo_forma_pago(this->idFormadePago);
            this->dFechaUltimaCompra = rProveedor.field("dFechaUltimaCompra").value().toDate();
            this->rAcumuladoCompras = rProveedor.field("rAcumuladoCompras").value().toDouble();
            this->cEntidadBancariaProveedor = rProveedor.field("cEntidadBancariaProveedor").value().toString();
            this->cOficinaBancariaProveedor = rProveedor.field("cOficinaBancariaProveedor").value().toString();
            this->cDCProveedor = rProveedor.field("cDCProveedor").value().toString();
            this->cCCProveedor = rProveedor.field("cCCProveedor").value().toString();
            this->cEntidadPagoProveedor = rProveedor.field("cEntidadPagoProveedor").value().toString();
            this->cOficinaPagoProveedor = rProveedor.field("cOficinaPagoProveedor").value().toString();
            this->cDCPagoProveedor = rProveedor.field("cDCPagoProveedor").value().toString();
            this->rRetencionIRPF = rProveedor.field("rRetencionIRPF").value().toDouble();
            this->nTipoRetencion = rProveedor.field("nTipoRetencion").value().toInt();
            this->cCuentaAplicacion = rProveedor.field("cCuentaAplicacion").value().toString();
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
    cargaracumulados(this->id);
}


void Proveedor::Guardar()
{
   // QScopedPointer<QSqlQuery>qProveedor(new QSqlQuery(QSqlDatabase::database("Maya")));
    QSqlQuery queryProveedor(QSqlDatabase::database("Maya"));

    queryProveedor.prepare("UPDATE proveedores SET "
                        " cCodigo =:cCodigo,"
                        "cProveedor =:cProveedor,"
                        "cCif =:cCif,"
                        "cDireccion1 =:cDireccion1,"
                        "cDireccion2 =:cDireccion2,"
                        "cCP =:cCP,"
                        "cPoblacion =:cPoblacion,"
                        "cProvincia =:cProvincia, "
                        "id_pais =:id_pais, "
                        "cTelefono1 =:cTelefono1,"
                        "cTelefono2 =:cTelefono2,"
                        "cTelefono3 =:cTelefono3,"
                        "cFax =:cFax,"
                        "cMovil =:cMovil,"
                        "cEMail =:cEMail,"
                        "cWeb =:cWeb,"
                        "cPersonaContacto =:cPersonaContacto,"
                        "nDiaCobro =:nDiaCobro,"
                        "cDireccionAlmacen =:cDireccionAlmacen,"
                        "cCPAlmacen =:cCPAlmacen,"
                        "cPoblacionAlmacen =:cPoblacionAlmacen,"
                        "cProvinciaAlmacen =:cProvinciaAlmacen,"
                        "idPaisAlmacen =:idPaisAlmacen,"
                        "cTelefonoAlmacen =:cTelefonoAlmacen,"
                        "cFaxAlmacen =:cFaxAlmacen,"
                        "idFormaPago =:idFormaPago,"
                        "dFechaUltimaCompra =:dFechaUltimaCompra,"
                        "rAcumuladoCompras =:rAcumuladoCompras,"
                        "cEntidadBancariaProveedor =:cEntidadBancariaProveedor,"
                        "cOficinaBancariaProveedor =:cOficinaBancariaProveedor,"
                        "cDCProveedor =:cDCProveedor,"
                        "cCCProveedor =:cCCProveedor,"
                        "cEntidadPagoProveedor =:cEntidadPagoProveedor,"
                        "cOficinaPagoProveedor =:cOficinaPagoProveedor,"
                        "cDCPagoProveedor =:cDCPagoProveedor,"
                        "rRetencionIRPF =:rRetencionIRPF,"
                        "nTipoRetencion =:nTipoRetencion,"
                        "cCuentaAplicacion =:cCuentaAplicacion,"
                        "tComentarios =:tComentarios,"
                        "nDto =:nDto,"
                        "dFechaAlta =:dFechaAlta,"
                        "rDeudaMaxima =:rDeudaMaxima,"
                        "rDeudaActual =:rDeudaActual,"
                        "lRecargoEquivalencia =:lRecargoEquivalencia,"
                        "tTextoparaPedidos =:tTextoparaPedidos,"
                        "rEntregadoaCuenta =:rEntregadoaCuenta "
                        " WHERE id = :id");


    queryProveedor.bindValue(":cCodigo",this->cCodigo);
    queryProveedor.bindValue(":cProveedor",this->cProveedor);
    queryProveedor.bindValue(":cCif",this->cCif);
    queryProveedor.bindValue(":cDireccion1",this->cDireccion1);
    queryProveedor.bindValue(":cDireccion2",this->cDireccion2);
    queryProveedor.bindValue(":cCP",this->cCP);
    queryProveedor.bindValue(":cPoblacion",this->cPoblacion);
    queryProveedor.bindValue(":cProvincia",this->cProvincia);
    queryProveedor.bindValue(":id_pais",this->idpais);
    queryProveedor.bindValue(":cTelefono1",this->cTelefono1);
    queryProveedor.bindValue(":cTelefono2",this->cTelefono2);
    queryProveedor.bindValue(":cTelefono3",this->cTelefono3);
    queryProveedor.bindValue(":cFax",this->cFax);
    queryProveedor.bindValue(":cMovil",this->cMovil);
    queryProveedor.bindValue(":cEMail",this->cEMail);
    queryProveedor.bindValue(":cWeb",this->cWeb);
    queryProveedor.bindValue(":cPersonaContacto",this->cPersonaContacto);
    queryProveedor.bindValue(":nDiaCobro",this->nDiaCobro);
    queryProveedor.bindValue(":cDireccionAlmacen",this->cDireccionAlmacen);
    queryProveedor.bindValue(":cCPAlmacen",this->cCPAlmacen);
    queryProveedor.bindValue(":cPoblacionAlmacen",this->cPoblacionAlmacen);
    queryProveedor.bindValue(":cProvinciaAlmacen",this->cProvinciaAlmacen);
    queryProveedor.bindValue(":idPaisAlmacen",this->idPaisAlmacen);
    queryProveedor.bindValue(":cTelefonoAlmacen",this->cTelefonoAlmacen);
    queryProveedor.bindValue(":cFaxAlmacen",this->cFaxAlmacen);
    queryProveedor.bindValue(":idFormaPago",this->idFormadePago);
    queryProveedor.bindValue(":dFechaUltimaCompra",this->dFechaUltimaCompra);
    queryProveedor.bindValue(":rAcumuladoCompras",this->rAcumuladoCompras);
    queryProveedor.bindValue(":cEntidadBancariaProveedor",this->cEntidadBancariaProveedor);
    queryProveedor.bindValue(":cOficinaBancariaProveedor",this->cOficinaBancariaProveedor);
    queryProveedor.bindValue(":cDCProveedor",this->cDCProveedor);
    queryProveedor.bindValue(":cCCProveedor",this->cCCProveedor);
    queryProveedor.bindValue(":cEntidadPagoProveedor",this->cEntidadPagoProveedor);
    queryProveedor.bindValue(":cOficinaPagoProveedor",this->cOficinaPagoProveedor);
    queryProveedor.bindValue(":cDCPagoProveedor",this->cDCPagoProveedor);
    queryProveedor.bindValue(":rRetencionIRPF",this->rRetencionIRPF);
    queryProveedor.bindValue(":nTipoRetencion",this->nTipoRetencion);
    queryProveedor.bindValue(":cCuentaAplicacion",this->cCuentaAplicacion);
    queryProveedor.bindValue(":tComentarios",this->tComentarios);
    queryProveedor.bindValue(":nDto",this->nDto);
    queryProveedor.bindValue(":dFechaAlta",this->dFechaAlta);
    queryProveedor.bindValue(":rDeudaMaxima",this->rDeudaMaxima);
    queryProveedor.bindValue(":rDeudaActual",this->rDeudaActual);
    queryProveedor.bindValue(":lRecargoEquivalencia",this->lRecargoEquivalencia);
    queryProveedor.bindValue(":tTextoparaPedidos",this->tTextoparaPedidos);
    queryProveedor.bindValue(":rEntregadoaCuenta",this->rEntregadoaCuenta);
    queryProveedor.bindValue(":id",this->id);

    if(!queryProveedor.exec()){
        qDebug() << queryProveedor.lastQuery();

        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Gestión proveedores/Acreedores"),
           QObject::tr("No se ha podido insertar el proveedor en la BD: %1").arg(queryProveedor.lastError().text(),
           QObject::tr("Ok")));
    } else {
        QMessageBox::information(qApp->activeWindow(),QObject::tr("Gestión proveedores/Acreeedores"),
                                 QObject::tr("Los datos se han guardado correctamente"),tr("Aceptar"));
    }
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
    this->rRetencionIRPF = 0;
    this->nTipoRetencion = 0;
    this->cCuentaAplicacion = "";
    this->tComentarios = "";
    this->nDto = 0;
    this->dFechaAlta = QDate::currentDate();
    this->rDeudaMaxima = 0;
    this->rDeudaActual = 0;
    this->lRecargoEquivalencia = 0;
    this->tTextoparaPedidos = "";
    this->rEntregadoaCuenta = 0;
    this->idFormadePago =0;
    this->idPaisAlmacen =0;
}

void Proveedor::Borrar(int nId)
{
    QScopedPointer<QSqlQuery>qProveedor(new QSqlQuery(QSqlDatabase::database("Maya")));
    //QSqlQuery *qProveedor = new QSqlQuery(QSqlDatabase::database("Maya"));
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
    QScopedPointer<QSqlQuery>qProveedores(new QSqlQuery(QSqlDatabase::database("Maya")));
    //QSqlQuery *qProveedores = new QSqlQuery(QSqlDatabase::database("Maya"));
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


