#include "cliente.h"

//


Cliente::Cliente(QObject *parent) :
    QObject(parent)
{
}
void Cliente::Guardar() {
    QSqlQuery query(QSqlDatabase::database("terra"));
    query.prepare( "UPDATE clientes set "
                   "cCodigoCliente = :cCodigoCliente,"
                   "cApellido1 = :cApellido1,"
                   "cApellido2 = :cApellido2,"
                   "cNombre= :cNombre,"
                   "cNombreFiscal= :cNombreFiscal,"
                   "cNombreComercial= :cNombreComercial,"
                   "cPersonaContacto= :cPersonaContacto,"
                   "cCifNif =:cCifNif,"
                   "cDireccion1= :cDireccion1,"
                   "cDireccion2= :cDireccion2,"
                   "cCP= :cCP,"
                   "cPoblacion= :cPoblacion,"
                   "cProvincia= :cProvincia,"
                   "idPais= :idPais,"
                   "cTelefono1 = :cTelefono1,"
                   "cTelefono2 = :cTelefono2,"
                   "cFax=:cFax,"
                   "cMovil=:cMovil,"
                   "cEmail=:cEmail,"
                   "cWeb=:cWeb,"
                   "cDireccionFactura1=:cDireccionFactura1,"
                   "cDireccionFactura2=:cDireccionFactura2,"
                   "cCPFactura=:cCPFactura,"
                   "cPoblacionFactura=:cPoblacionFactura,"
                   "cProvinciaFactura=:cProvinciaFactura,"
                   "idPaisFactura=:idPaisFactura,"
                   "cDireccionAlmacen=:cDireccionAlmacen1,"
                   "cDireccionAlmacen2=:cDireccionAlmacen2,"
                   "cCPAlmacen=:cCPAlmacen,"
                   "cPoblacionAlmacen=:cPoblacionAlmacen,"
                   "cProvinciaAlmacen=:cProvinciaAlmacen,"
                   "idPaisAlmacen=:idPaisAlmacen,"
                   "dFechaAlta=:dFechaAlta,"
                   "dFechaUltimaCompra=:dFechaUltimaCompra,"
                   "rAcumuladoVentas=:rAcumuladoVentas,"
                   "rVentasEjercicio=:rVentasEjercicio,"
                   "rRiesgoMaximo=:rRiesgoMaximo,"
                   "rDeudaActual=:rDeudaActual,"
                   "tComentarios=:tComentarios,"
                   "lBloqueado=:lBloqueado,"
                   "tComentarioBloqueo =:tComentarioBloqueo,"
                   "nPorcDtoCliente=:nPorcDtoCliente,"
                   "lRecargoEquivalencia=:lRecargoEquivalencia,"
                   "cCuentaContable=:cCuentaContable,"
                   "cCuentaIvaRepercutido=:cCuentaIvaRepercutido,"
                   "cCuentaDeudas=:cCuentaDeudas,"
                   "cCuentaCobros=:cCuentaCobros,"
                   "nDiaPago1=:nDiaPago1,"
                   "nDiaPago2=:nDiaPago2,"
                   "nTarifaCliente=:nTarifaCliente,"
                   "rImporteACuenta=:rImporteaCuenta,"
                   "rVales=:rVales,"
                   "cCentidadBancaria=:cEntidadBancaria,"
                   "cOficinaBancaria=:cOficinaBancaria,"
                   "cDC=:cDC,"
                   "cCuentaCorriente=:cCuentaCorriente,"
                   "dFechaNacimiento=:dFechaNacimiento,"
                   "rImportePendiente=:rImportePendiente,"
                   "cAccesoWeb =:cAccesoWeb,"
                   "cPasswordWeb=:cPasswordWeb,"
                   "nIRPF =:nIRPF"
                   " WHERE id =:id" );

    query.bindValue(":cCodigoCliente", this->cCodigoCliente);
    query.bindValue(":cApellido1", this->cApellido1);
    query.bindValue(":cApellido2",this->cApellido2);
    query.bindValue(":cNombre",this->cNombre);
    query.bindValue(":cNombreFiscal",this->cNombreFiscal);
    query.bindValue(":cNombreComercial",this->cNombreComercial);
    query.bindValue(":cPersonaContacto",this->cPersonaContacto);
    query.bindValue(":cCifNif",this->cCifNif);
    query.bindValue(":cDireccion1",this->cDireccion1);
    query.bindValue(":cDireccion2",this->cDireccion2);
    query.bindValue(":cCP",this->cCp);
    query.bindValue(":cPoblacion",this->cPoblacion);
    query.bindValue(":cProvincia", this->cProvincia);
    query.bindValue(":idPais",this->idPais);
    query.bindValue(":cTelefono1", this->cTelefono1);
    query.bindValue(":cTelefono2",this->cTelefono2);
    query.bindValue(":cFax",this->cFax);
    query.bindValue(":cMovil",this->cMovil);
    query.bindValue(":cEmail",this->cEmail);
    query.bindValue(":cWeb",this->cWeb);
    query.bindValue(":cDireccionFactura1",this->cDireccionFactura1);
    query.bindValue(":cDireccionFactura2",this->cDireccionFactura2);
    query.bindValue(":cCPFactura",this->cCPFactura);
    query.bindValue(":cPoblacionFactura",this->cPoblacionFactura);
    query.bindValue(":cProvinciaFactura", this->cProvinciaFactura);
    query.bindValue(":idPaisFactura", this->idPaisFactura);
    query.bindValue(":cDireccionAlmacen1",this->cDireccionAlmacen);
    query.bindValue(":cDireccionAlmacen2",this->cDireccionAlmacen2);
    query.bindValue(":cCPAlmacen", this->cCPAlmacen);
    query.bindValue(":cPoblacionAlmacen",this->cPoblacionAlmacen);
    query.bindValue(":cProvinciaAlmacen",this->cProvinciaAlmacen);
    query.bindValue(":idPaisAlmacen",this->idPaisAlmacen);
    query.bindValue(":dFechaAlta",this->dFechaalta);
    query.bindValue(":dFechaUltimaCompra",this->dFechaCompra);
    query.bindValue(":rAcumuladoVentas",this->rAcumuladoVentas);
    query.bindValue(":rVentasEjercicio",this->rVentasEjercicio);
    query.bindValue(":rRiesgoMaximo",this->rRiesgoMaximo);
    query.bindValue(":rDeudaActual",this->rDeudaActual);
    query.bindValue(":tComentarios",this->tComentarios);
    query.bindValue(":lBloqueado",this->lBloqueado);
    query.bindValue(":tComentarioBloqueo",this->tComentarioBloqueo);
    query.bindValue(":nPorcDtoCliente", this->nPorcDtoCliente);
    query.bindValue(":lRecargoEquivalencia",this->lRecargoEquivalencia);
    query.bindValue(":cCuentaContable",this->cCuentaContable);
    query.bindValue(":cCuentaIvaRepercutido",this->cCuentaIvaRepercutido);
    query.bindValue(":cCuentaDeudas", this->cCuentaDeudas);
    query.bindValue(":cCuentaCobros", this->cCuentaCobros);
    query.bindValue(":nDiaPago1",this->nDiaPago1);
    query.bindValue(":nDiaPago2",this->nDiaPago2);
    query.bindValue(":nTarifaCliente",this->nTarifaCliente);
    QString rImporteACuenta;
    rImporteACuenta = QString::number(this->rImporteACuenta);
    rImporteACuenta = rImporteACuenta.replace(".","");
    this->rImporteACuenta = rImporteACuenta.toDouble();
    query.bindValue(":rImporteACuenta",this->rImporteACuenta);
    query.bindValue(":rVales",this->rVales);
    query.bindValue(":cEntidadBancaria", this->cEntidadBancaria);
    query.bindValue(":cOficinaBancaria",this->cOficinaBancaria);
    query.bindValue(":cDC",this->cDc);
    query.bindValue(":cCuentaCorriente",this->cCuentaCorriente);
    query.bindValue(":dFechaNacimiento",this->dFechaNacimiento);
    query.bindValue(":rImportePendiente",this->rImportePendiente);
    query.bindValue(":cAccesoWeb",this->cAccesoWeb);
    query.bindValue(":cPasswordWeb",this->cPasswordWeb);
    if (this->lIRPF)
        query.bindValue(":nIRPF",1);
    else
        query.bindValue(":nIRPF",0);
    query.bindValue(":id",this->id);


    if(!query.exec()){
        QMessageBox::critical(qApp->activeWindow(),"error al guardar datos cliente. Descripción Error: ", query.lastError().text());
    } else {
        QMessageBox::information(qApp->activeWindow(),"Guardar datos","Los datos se han guardado correctamente:","Ok");
    }
        ;

}
void Cliente::Anadir() {
    QSqlQuery query(QSqlDatabase::database("terra"));
         query.prepare("INSERT INTO clientes (cCodigoCliente, cApellido1, cApellido2,cNombre,cNombreFiscal,cNombreComercial,cPersonaContacto,"
                       " cCifNif,cDireccion1,cDireccion2,cCP,cPoblacion,cProvincia,idpais,cTelefono1,cTelefono2,cFax,"
                       "cMovil,cEmail,cWeb,cDireccionFactura1,cDireccionFactura2,cCPFactura,cPoblacionFactura,cProvinciaFactura,"
                       "idPaisFactura,cDireccionAlmacen,cDireccionAlmacen2,cCPAlmacen,cPoblacionAlmacen,cProvinciaAlmacen,"
                       "idPaisAlmacen,dFechaAlta,dFechaUltimaCompra,rAcumuladoVentas,rVentasEjercicio,rRiesgoMaximo,rDeudaActual,"
                       "tComentarios,lBloqueado,tComentarioBloqueo,nPorcDtoCliente,lRecargoEquivalencia,cCuentaContable,cCuentaIvaRepercutido,"
                       "cCuentaDeudas,cCuentaCobros,idFormaPago,nDiapago1,nDiaPago2,nTarifaCliente,rImporteACuenta,rVales,"
                       "cCentidadBancaria,cOficinaBancaria,cDC,cCuentaCorriente,dFechaNacimiento,rImportePendiente,cTipoCliente,cAccesoWeb,"
                       "cPasswordWeb,nIRPF) "
                       "VALUES (:cCodigoCliente, :cApellido1, :cApellido2,:cNombre,:cNombreFiscal,:cNombreComercial,:cPersonaContacto,"
                       ":cCifNif,:cDireccion1,:cDireccion2,:cCP,:cPoblacion,:cProvincia,:idPais,:cTelefono1,:cTelefono2,:cFax,"
                       ":cMovil,:cEmail,:cWeb,:cDireccionFactura1,:cDireccionFactura2,:cCPFactura,:cPoblacionFactura,:cProvinciaFactura,"
                       ":idPaisFactura,:cDireccionAlmacen,:cDireccionAlmacen2,:cCPAlmacen,:cPoblacionAlmacen,:cProvinciaAlmacen,"
                       ":idPaisAlmacen,:dFechaAlta,:dFechaUltimaCompra,:rAcumuladoVentas,:rVentasEjercicio,:rRiesgoMaximo,:rDeudaActual,"
                       ":tComentarios,:lBloqueado,:tComentarioBloqueo,:nPorcDtoCliente,:lRecargoEquivalencia,:cCuentaContable,:cCuentaIvaRepercutido,"
                       ":cCuentaDeudas,:cCuentaCobros,:cFormaPago,:nDiapago1,:nDiaPago2,:nTarifaCliente,:rImporteACuenta,:rVales,"
                       ":cCentidadBancaria,:cOficinaBancaria,:cDC,:cCuentaCorriente,:dFechaNacimiento,:rImportePendiente,"
                       ":cTipoCliente,:cAccesoWeb,:cPasswordWeb,:nIRPF)");
         query.bindValue(":cCodigoCliente", this->cCodigoCliente);
         query.bindValue(":cApellido1", this->cApellido1);
         query.bindValue(":cApellido2",this->cApellido2);
         query.bindValue(":cNombre",this->cNombre);
         query.bindValue(":cNombreFiscal",this->cNombreFiscal);
         query.bindValue(":cNombreComercial",this->cNombreComercial);
         query.bindValue(":cPersonaContacto",this->cPersonaContacto);
         query.bindValue(":cCifNif",this->cCifNif);
         query.bindValue(":cDireccion1",this->cDireccion1);
         query.bindValue(":cDireccion2",this->cDireccion2);
         query.bindValue(":cCP",this->cCp);
         query.bindValue(":cPoblacion",this->cPoblacion);
         query.bindValue(":cProvincia", this->cProvincia);
         query.bindValue(":idpais",this->idPais);
         query.bindValue(":cTelefono1", this->cTelefono1);
         query.bindValue(":cTelefono2",this->cTelefono2);
         query.bindValue(":cFax",this->cFax);
         query.bindValue(":cMovil",this->cMovil);
         query.bindValue(":cEmail",this->cEmail);
         query.bindValue(":cWeb",this->cWeb);
         query.bindValue(":cDireccionFactura1",this->cDireccionFactura1);
         query.bindValue(":cDireccionFactura2",this->cDireccionFactura2);
         query.bindValue(":cCPFactura",this->cCPFactura);
         query.bindValue(":cPoblacionFactura",this->cPoblacionFactura);
         query.bindValue(":cProvinciaFactura", this->cProvinciaFactura);
         query.bindValue(":idPaisFactura", this->idPaisFactura);
         query.bindValue(":cDireccionAlmacen1",this->cDireccionAlmacen);
         query.bindValue(":cDireccionAlmacen2",this->cDireccionAlmacen2);
         query.bindValue(":cCPAlmacen", this->cCPAlmacen);
         query.bindValue(":cPoblacionAlmacen",this->cPoblacionAlmacen);
         query.bindValue(":cProvinciaAlmacen",this->cProvinciaAlmacen);
         query.bindValue(":idPaisAlmacen",this->idPaisAlmacen);
         query.bindValue(":dFechaAlta",this->dFechaalta);
         query.bindValue(":dFechaUltimaCompra",this->dFechaCompra);
         query.bindValue(":rAcumuladoVentas",this->rAcumuladoVentas);
         query.bindValue(":rVentasEjercicio",this->rVentasEjercicio);
         query.bindValue(":rRiesgoMaximo",this->rRiesgoMaximo);
         query.bindValue(":rDeudaActual",this->rDeudaActual);
         query.bindValue(":tComentarios",this->tComentarios);
         query.bindValue(":lBloqueado",this->lBloqueado);
         query.bindValue(":tComentarioBloqueo",this->tComentarioBloqueo);
         query.bindValue(":nPorcDtoCliente", this->nPorcDtoCliente);
         query.bindValue(":lRecargoEquivalencia",this->lRecargoEquivalencia);
         query.bindValue(":cCuentaContable",this->cCuentaContable);
         query.bindValue(":cCuentaIvaRepercutido",this->cCuentaIvaRepercutido);
         query.bindValue(":cCuentaDeudas", this->cCuentaDeudas);
         query.bindValue(":cCuentaCobros", this->cCuentaCobros);
         query.bindValue(":cFormaPago",this->cFormaPago);
         query.bindValue(":nDiaPago1",this->nDiaPago1);
         query.bindValue(":nDiaPago2",this->nDiaPago2);
         query.bindValue(":nTarifaCliente",this->nTarifaCliente);
         query.bindValue(":rImporteACuenta",this->rImporteACuenta);
         query.bindValue(":rVales",this->rVales);
         query.bindValue(":cEntidadBancaria", this->cEntidadBancaria);
         query.bindValue(":cOficinaBancaria",this->cOficinaBancaria);
         query.bindValue(":cDC",this->cDc);
         query.bindValue(":cCuentaCorriente",this->cCuentaCorriente);
         query.bindValue(":dFechaNacimiento",this->dFechaNacimiento);
         query.bindValue(":rImportePendiente",this->rImportePendiente);
         query.bindValue(":cTipoCliente","");//FIXME cTipoCliente
         query.bindValue(":cAccesoWeb",this->cAccesoWeb);
         query.bindValue(":cPasswordWeb",this->cPasswordWeb);
         if (this->lIRPF)
             query.bindValue(":nIRPF",1);
         else
             query.bindValue(":nIRPF",0);

         if(!query.exec()){
             QMessageBox::critical(qApp->activeWindow(),"error al insertar:", query.lastError().text());
         } else{
             QMessageBox::information(qApp->activeWindow(),"NuevoCliente","Cliente insertado Correctamente");
             int nId = query.lastInsertId().toInt();
             this->id = nId;
         }
}
void Cliente::Recuperar(QString cSQL) {
    qryCliente = new QSqlQuery(QSqlDatabase::database("terra"));
    qryCliente->prepare(cSQL);
    if( !qryCliente->exec() ) {
        QMessageBox::critical(qApp->activeWindow(), "error:", qryCliente->lastError().text());
    } else {
        if (qryCliente->next()) {
            QSqlRecord registro = qryCliente->record();
            this->id = registro.field("id").value().toInt();
            this->cCodigoCliente= registro.field("cCodigoCliente").value().toString();
            this->cApellido1 = registro.field("cApellido1").value().toString();
            this->cApellido2 = registro.field("cApellido2").value().toString();
            this->cNombre = registro.field("cNombre").value().toString();
            this->cNombreFiscal = registro.field("cNombreFiscal").value().toString();
            this->cNombreComercial = registro.field("cNombreComercial").value().toString();
            this->cPersonaContacto = registro.field("cPersonaContacto").value().toString();
            this->cCifNif = registro.field("cCifNif").value().toString();
            this->cDireccion1 = registro.field("cDireccion1").value().toString();
            this->cDireccion2 = registro.field("cDireccion2").value().toString();
            this->cCp = registro.field("cCP").value().toString();
            this->cPoblacion = registro.field("cPoblacion").value().toString();
            this->cProvincia = registro.field("cProvincia").value().toString();
            this->cPais = registro.field("cPais").value().toString();
            this->cTelefono1 = registro.field("cTelefono1").value().toString();
            this->cTelefono2 = registro.field("cTelefono2").value().toString();
            this->cFax =registro.field("cFax").value().toString();
            this->cMovil = registro.field("cMovil").value().toString();
            this->cEmail = registro.field("cEmail").value().toString();
            this->cWeb = registro.field("cWeb").value().toString();
            this->cDireccionFactura1 = registro.field("cDireccionFactura1").value().toString();
            this->cDireccionFactura2 = registro.field("cDireccionFactura2").value().toString();
            this->cCPFactura = registro.field("cCPFactura").value().toString();
            this->cPoblacionFactura = registro.field("cPoblacionFactura").value().toString();
            this->cProvinciaFactura = registro.field("cProvinciaFactura").value().toString();
            this->cPaisFactura = registro.field("cPaisFactura").value().toString();
            this->cDireccionAlmacen = registro.field("cDireccionAlmacen").value().toString();
            this->cDireccionAlmacen2 = registro.field("cDireccionAlmacen").value().toString();
            this->cCPAlmacen = registro.field("cCPAlmacen").value().toString();
            this->cPoblacionAlmacen = registro.field("cPoblacionAlmacen").value().toString();
            this->cProvinciaAlmacen = registro.field("cProvinciaAlmacen").value().toString();
            this->cPaisAlmacen = registro.field("cPaisAlmacen").value().toString();
            this->dFechaalta = registro.field("dFechaAlta").value().toDate();
            this->dFechaCompra = registro.field("dFechaCompra").value().toDate();
            this->rAcumuladoVentas = registro.field("rAcumuladoVentas").value().toDouble();
            this->rVentasEjercicio = registro.field("rVentasEjercicio").value().toDouble();
            this->rRiesgoMaximo = registro.field("rRiesgoMaximo").value().toDouble();
            this->rDeudaActual = registro.field("rDeudaActual").value().toDouble();
            this->tComentarios = registro.field("tComentarios").value().toString();
            this->lBloqueado = registro.field("lBloqueado").value().toInt();
            this->tComentarioBloqueo = registro.field("tComentarioBloqueo").value().toString();
            this->nPorcDtoCliente = registro.field("nPorcDtoCliente").value().toDouble();
            this->lRecargoEquivalencia = registro.field("lRecargoEquivalencia").value().toInt();
            this->cCuentaContable = registro.field("cCuentaContable").value().toString();
            this->cCuentaIvaRepercutido = registro.field("cCuentaIvaRepercutido").value().toString();
            this->cCuentaDeudas = registro.field("cCuentaDeudas").value().toString();
            this->cCuentaCobros = registro.field("cCuentaCobros").value().toString();
            this->cFormaPago = registro.field("cFormaPago").value().toString();
            this->nDiaPago1 = registro.field("nDiaPago1").value().toInt();
            this->nDiaPago2 = registro.field("nDiaPago2").value().toInt();
            this->nTarifaCliente = registro.field("nTarifaCliente").value().toInt();
            this->rImporteACuenta = registro.field("rImporteACuenta").value().toDouble();
            this->rVales = registro.field("rVales").value().toDouble();
            this->cEntidadBancaria = registro.field("cCEntidadBancaria").value().toString();
            this->cOficinaBancaria = registro.field("cOficinaBancaria").value().toString();
            this->cDc = registro.field("cDC").value().toString();
            this->cCuentaCorriente = registro.field("cCuentaCorriente").value().toString();
            this->dFechaNacimiento = registro.field("dFechaNacimiento").value().toDate();
            this->rImportePendiente = registro.field("rImportePendiente").value().toDouble();
            this->cAccesoWeb = registro.field("cAccesoWeb").value().toString();
            this->cPasswordWeb = registro.field("cPasswordWeb").value().toString();
            int nIRPF =registro.field("nIRPF").value().toInt();
            if (nIRPF==1)
                this->lIRPF = true;
            else
                this->lIRPF = false;

            }

    delete qryCliente;

    }

}

void Cliente::AnadirDeuda(int id_cliente, QDate dFechaDeuda, QDate dFechaVto, QString cDocumento, int id_Tiquet,
                          int id_Factura, int nTipo, double rImporteDeuda, double rPagado = 0, double rPendienteCobro = 0,
                          QString cEntidad = "", QString cOficina = "", QString cDC="", QString cCuenta = "") {
    QSqlQuery qCliente(QSqlDatabase::database("terra"));
    double rImporte;
    rImporte = this->rDeudaActual + rImporteDeuda;
    qCliente.prepare("Update clientes set  rDeudaActual = :rImporte where id =:id_cliente");
    qCliente.bindValue(":rImporte",rImporte);
    qCliente.bindValue(":id", id_cliente);
    if(!qCliente.exec()) {
        QMessageBox::critical(qApp->activeWindow(),tr("Añadir deuda cliente"),tr("Ha fallado la inserción de la deuda en la ficha del paciente"),tr("&Aceptar"));
    }

    QSqlQuery qClientes_Deuda(QSqlDatabase::database("terra"));
    qClientes_Deuda.prepare("Insert into clientes_deuda (Id_Cliente,dFecha,dVencimiento,cDocumento,Id_Ticket,Id_Factura,nTipo,"
                            "rImporte,rPagado,rPendienteCobro,cEntidad,cOficina,cDC,cCuenta Values (:Id_Cliente,:dFecha,:dVencimiento,"
                            ":cDocumento,:Id_Ticket,:Id_Factura,:nTipo,:rImporte,:rPagado,:rPendienteCobro,:cEntidad,:cOficina,"
                            ":cDC,:cCuenta)" );
    qClientes_Deuda.bindValue(":Id_Cliente",id_cliente);
    qClientes_Deuda.bindValue(":dFecha",dFechaDeuda);
    qClientes_Deuda.bindValue(":dVencimiento",dFechaVto);
    qClientes_Deuda.bindValue(":cDocumento",cDocumento);
    qClientes_Deuda.bindValue(":Id_Tiquet",id_Tiquet);
    qClientes_Deuda.bindValue(":Id_Factura",id_Factura);
    qClientes_Deuda.bindValue(":nTipo",nTipo);
    qClientes_Deuda.bindValue(":rImporte",rImporteDeuda);
    qClientes_Deuda.bindValue(":rPagado",rPagado);
    qClientes_Deuda.bindValue(":rPendienteCobro",rPendienteCobro);
    qClientes_Deuda.bindValue(":cEntidad",cEntidad);
    qClientes_Deuda.bindValue(":cOficina",cOficina);
    qClientes_Deuda.bindValue(":cDC",cDC);
    qClientes_Deuda.bindValue(":cCuenta",cCuenta);
    if(!qClientes_Deuda.exec()) {
        QMessageBox::critical(qApp->activeWindow(),tr("Añadir deuda cliente"),tr("Falló la inserción en la tabla de deudas"),tr("&Aceptar"));
    }

}
void Cliente::DescontarDeuda(int id_deuda, double rPagado){
    QSqlQuery qClientes_deuda(QSqlDatabase::database("terra"));
    qClientes_deuda.prepare("Select * from clientes_deuda where id =:id_deuda");
    qClientes_deuda.bindValue(":id_deuda",id_deuda);
    if (!qClientes_deuda.exec()) {
        QMessageBox::critical(qApp->activeWindow(),tr("Modificar deuda Cliente"),tr("Falló la lectura de la deuda del cliente"),tr("&Aceptar"));
    }
    qClientes_deuda.prepare("update clientes_deuda ");
    qClientes_deuda.clear();

}
void Cliente::Borrar(int id_cliente)
{
    if(QMessageBox::question(qApp->activeWindow(),tr("Borrar Ficha"),
                          tr("Está apunto de borrar la ficha de un cliente\n¿Desea continuar?"),
                          tr("No"),tr("Si")) == QMessageBox::Accepted)
    {
        QSqlQuery qryCliente(QSqlDatabase::database("terra"));
        qryCliente.prepare("Delete from clientes where id = :id_Cliente");
        qryCliente.bindValue(":id",id_cliente);
        if(!qryCliente.exec())
            QMessageBox::critical(qApp->activeWindow(),tr("Borrar cliente"),
                                  tr("Falló el borrado de la deuda del cliente"),tr("&Aceptar"));
        else
            QMessageBox::information(qApp->activeWindow(),tr("Borrar cliente"),
                                     tr("Borrado correctamente"),tr("&Aceptar"));
    }
}

QString Cliente::NuevoCodigoCliente()
{
    QString cCodigo;
    QString cNum;
    int nCodigo;
    QSqlQuery *qClientes = new QSqlQuery(QSqlDatabase::database("terra"));
    if(qClientes->exec("select cCodigoCliente from clientes  order by cCodigoCliente desc limit 1")) {
        if (qClientes->next()) {
            QSqlRecord registro = qClientes->record();
            cCodigo = registro.field("cCodigoCliente").value().toString();
            nCodigo = cCodigo.toInt();
            nCodigo ++;
            cCodigo = QString::number(nCodigo);
        }
   }
   if (nCodigo == 0 || nCodigo == 1) {
        cNum = "1";
        while (cNum.length()< (Configuracion_global->nDigitosCuentasContables - Configuracion_global->cCuentaClientes.length()) ) {
            cNum.prepend("0");
        }
        cCodigo = Configuracion_global->cCuentaClientes + cNum;

}

   return cCodigo;
}

int Cliente::BuscaridPais(QString Pais)
{
    QSqlQuery qPais(QSqlDatabase::database("terra"));
    qPais.prepare("select id from paises where pais =:pais");
    qPais.bindValue(":pais",Pais);
    if(qPais.exec()) {
        qPais.next();
        QSqlRecord rPais = qPais.record();
        return rPais.field("id").value().toInt();
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Pacientes"),
                             tr("No se pudo enconontrar el identificador del pais: %1 ").arg(Pais),tr("Aceptar"));
    }
    return 0;
}

QString Cliente::RecuperarPais(int nid)
{
    QSqlQuery qPais(QSqlDatabase::database("terra"));
    qPais.prepare("select pais from paises where id =:id");
    qPais.bindValue(":id",nid);
    if(qPais.exec()) {
        qPais.next();
        QSqlRecord rPais = qPais.record();
        return rPais.field("pais").value().toString();
    } else {
        QMessageBox::warning(qApp->activeWindow(),tr("Gestión de Pacientes"),
                             tr("No se pudo enconontrar el Pais: %1 ").arg(QString::number(nid)),tr("Aceptar"));
    }
    return "";
}



