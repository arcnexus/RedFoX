#include "cliente.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QErrorMessage>
#include <QSqlQuery>
#include <QMessageBox>
#include <frmdecision.h>


Cliente::Cliente(QObject *parent) :
    QObject(parent)
{
}
void Cliente::Guardar() {
    QSqlQuery query(QSqlDatabase::database("empresa"));
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
                   "cPais= :cPais,"
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
                   "cPaisFactura=:cPaisFactura,"
                   "cDireccionAlmacen=:cDireccionAlmacen1,"
                   "cDireccionAlmacen2=:cDireccionAlmacen2,"
                   "cCPAlmacen=:cCPAlmacen,"
                   "cPoblacionAlmacen=:cPoblacionAlmacen,"
                   "cProvinciaAlmacen=:cProvinciaAlmacen,"
                   "cPaisAlmacen=:cPaisAlmacen,"
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
                   "cFormaPago=:cFormaPago,"
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
                   "cPasswordWeb=:cPasswordWeb"

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
    query.bindValue(":cPais",this->cPais);
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
    query.bindValue(":cPaisFactura", this->cPaisFactura);
    query.bindValue(":cDireccionAlmacen1",this->cDireccionAlmacen);
    query.bindValue(":cDireccionAlmacen2",this->cDireccionAlmacen2);
    query.bindValue(":cCPAlmacen", this->cCPAlmacen);
    query.bindValue(":cPoblacionAlmacen",this->cPoblacionAlmacen);
    query.bindValue(":cProvinciaAlmacen",this->cProvinciaAlmacen);
    query.bindValue(":cPaisAlmacen",this->cPaisAlmacen);
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
    query.bindValue(":cAccesoWeb",this->cAccesoWeb);
    query.bindValue(":cPasswordWeb",this->cPasswordWeb);
       query.bindValue(":id",this->id);


    if(!query.exec()){
        QMessageBox::critical(NULL,"error al guardar datos cliente:", query.lastError().text());
    } else {
        QMessageBox::information(NULL,"Guardar datos","Los datos se han guardado correctamente:","Ok");
    }
        ;

}
void Cliente::Anadir() {
    QSqlQuery query(QSqlDatabase::database("empresa"));
         query.prepare("INSERT INTO clientes (cCodigoCliente, cApellido1, cApellido2,cNombre,cNombreFiscal,cNombreComercial,cPersonaContacto,"
                       " cCifNif,cDireccion1,cDireccion2,cCP,cPoblacion,cProvincia,cPais,cTelefono1,cTelefono2,cFax,"
                       "cMovil,cEmail,cWeb,cDireccionFactura1,cDireccionFactura2,cCPFactura,cPoblacionFactura,cProvinciaFactura,"
                       "cPaisFactura,cDireccionAlmacen,cDireccionAlmacen2,cCPAlmacen,cPoblacionAlmacen,cProvinciaAlmacen,"
                       "cPaisAlmacen,dFechaAlta,dFechaUltimaCompra,rAcumuladoVentas,rVentasEjercicio,rRiesgoMaximo,rDeudaActual,"
                       "tComentarios,lBloqueado,tComentarioBloqueo,nPorcDtoCliente,lRecargoEquivalencia,cCuentaContable,cCuentaIvaRepercutido,"
                       "cCuentaDeudas,cCuentaCobros,cFormaPago,nDiapago1,nDiaPago2,nTarifaCliente,rImporteACuenta,rVales,"
                       "cCentidadBancaria,cOficinaBancaria,cDC,cCuentaCorriente,dFechaNacimiento,rImportePendiente,cAccesoWeb,cPasswordWeb) "
                       "VALUES (:cCodigoCliente, :cApellido, :cApellido2,:cNombre,:cNombreFiscal,:cNombreComercial,:cPersonaContacto,"
                       ":cCifNif,:cDireccion1,:cDireccion2,:cCP,:cPoblacion,:cProvincia,:cPais,:cTelefono1,:cTelefono2,:cFax,"
                       ":cMovil,:cEmail,:cWeb,:cDireccionFactura1,:cDireccionFactura2,:cCPFactura,:cPoblacionFactura,:cProvinciaFactura,"
                       ":cPaisFactura,:cDireccionAlmacen,:cDireccionAlmacen2,:cCPAlmacen,:cPoblacionAlmacen,:cProvinciaAlmacen,"
                       ":cPaisAlmacen,:dFechaAlta,:dFechaUltimaCompra,:rAcumuladoVentas,:rVentasEjercicio,:rRiesgoMaximo,:rDeudaActual,"
                       ":tComentarios,:lBloqueado,:tComentarioBloqueo,:nPorcDtoCliente,:lRecargoEquivalencia,:cCuentaContable,:cCuentaIvaRepercutido,"
                       ":cCuentaDeudas,:cCuentaCobros,:cFormaPago,:nDiapago1,:nDiaPago2,:nTarifaCliente,:rImporteACuenta,:rVales,"
                       ":cCentidadBancaria,:cOficinaBancaria,:cDC,:cCuentaCorriente,:dFechaNacimiento,:rImportePendiente,"
                       ":cAccesoWeb,:cPasswordWeb)");
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
         query.bindValue(":cPais",this->cPais);
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
         query.bindValue(":cPaisFactura", this->cPaisFactura);
         query.bindValue(":cDireccionAlmacen1",this->cDireccionAlmacen);
         query.bindValue(":cDireccionAlmacen2",this->cDireccionAlmacen2);
         query.bindValue(":cCPAlmacen", this->cCPAlmacen);
         query.bindValue(":cPoblacionAlmacen",this->cPoblacionAlmacen);
         query.bindValue(":cProvinciaAlmacen",this->cProvinciaAlmacen);
         query.bindValue(":cPaisAlmacen",this->cPaisAlmacen);
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
         query.bindValue(":cAccesoWeb",this->cAccesoWeb);
         query.bindValue(":cPasswordWeb",this->cPasswordWeb);

         if(!query.exec()){
             QMessageBox::critical(NULL,"error al insertar:", query.lastError().text());
         } else{
             QMessageBox::information(NULL,"NuevoCliente","Cliente insertado Correctamente");
         }
             ;

}
void Cliente::Recuperar(QString cSQL) {
    qryCliente = new QSqlQuery(QSqlDatabase::database("empresa"));
    qryCliente->prepare(cSQL);
    if( !qryCliente->exec() ) {
        QMessageBox::critical(NULL, "error:", qryCliente->lastError().text());
    } else {
        if (qryCliente->next()) {
            this->id = qryCliente->value(0).toInt();
            this->cCodigoCliente= qryCliente->value(1).toString();
            this->cApellido1 = qryCliente->value(2).toString();
            this->cApellido2 = qryCliente->value(3).toString();
            this->cNombre = qryCliente->value(4).toString();
            this->cNombreFiscal = qryCliente->value(5).toString();
            this->cNombreComercial = qryCliente->value(6).toString();
            this->cPersonaContacto = qryCliente->value(7).toString();
            this->cCifNif = qryCliente->value(8).toString();
            this->cDireccion1 = qryCliente->value(9).toString();
            this->cDireccion2 = qryCliente->value(10).toString();
            this->cCp = qryCliente->value(11).toString();
            this->cPoblacion = qryCliente->value(12).toString();
            this->cProvincia = qryCliente->value(13).toString();
            this->cPais = qryCliente->value(14).toString();
            this->cTelefono1 = qryCliente->value(15).toString();
            this->cTelefono2 = qryCliente->value(16).toString();
            this->cFax =qryCliente->value(17).toString();
            this->cMovil = qryCliente->value(18).toString();
            this->cEmail = qryCliente->value(19).toString();
            this->cWeb = qryCliente->value(20).toString();
            this->cDireccionFactura1 = qryCliente->value(21).toString();
            this->cDireccionFactura2 = qryCliente->value(22).toString();
            this->cCPFactura = qryCliente->value(23).toString();
            this->cPoblacionFactura = qryCliente->value(24).toString();
            this->cProvinciaFactura = qryCliente->value(25).toString();
            this->cPaisFactura = qryCliente->value(26).toString();
            this->cDireccionAlmacen = qryCliente->value(27).toString();
            this->cDireccionAlmacen2 = qryCliente->value(28).toString();
            this->cCPAlmacen = qryCliente->value(29).toString();
            this->cPoblacionAlmacen = qryCliente->value(30).toString();
            this->cProvinciaAlmacen = qryCliente->value(31).toString();
            this->cPaisAlmacen = qryCliente->value(32).toString();
            this->dFechaalta = qryCliente->value(33).toDate();
            this->dFechaCompra = qryCliente->value(34).toDate();
            this->rAcumuladoVentas = qryCliente->value(35).toDouble();
            this->rVentasEjercicio = qryCliente->value(36).toDouble();
            this->rRiesgoMaximo = qryCliente->value(37).toDouble();
            this->rDeudaActual = qryCliente->value(38).toDouble();
            this->tComentarios = qryCliente->value(39).toString();
            this->lBloqueado = qryCliente->value(40).toInt();
            this->tComentarioBloqueo = qryCliente->value(41).toString();
            this->nPorcDtoCliente = qryCliente->value(42).toDouble();
            this->lRecargoEquivalencia = qryCliente->value(43).toInt();
            this->cCuentaContable = qryCliente->value(44).toString();
            this->cCuentaIvaRepercutido = qryCliente->value(45).toString();
            this->cCuentaDeudas = qryCliente->value(46).toString();
            this->cCuentaCobros = qryCliente->value(47).toString();
            this->cFormaPago = qryCliente->value(48).toString();
            this->nDiaPago1 = qryCliente->value(49).toInt();
            this->nDiaPago2 = qryCliente->value(50).toInt();
            this->nTarifaCliente = qryCliente->value(51).toInt();
            this->rImporteACuenta = qryCliente->value(52).toDouble();
            this->rVales = qryCliente->value(53).toDouble();
            this->cEntidadBancaria = qryCliente->value(54).toString();
            this->cOficinaBancaria = qryCliente->value(55).toString();
            this->cDc = qryCliente->value(56).toString();
            this->cCuentaCorriente = qryCliente->value(57).toString();
            this->dFechaNacimiento = qryCliente->value(58).toDate();
            this->rImportePendiente = qryCliente->value(59).toDouble();
            this->cAccesoWeb = qryCliente->value(60).toString();
            this->cPasswordWeb = qryCliente->value(61).toString();


            }

    qryCliente->clear();
    qryCliente->finish();

    }

}

void Cliente::AnadirDeuda(int id_cliente, QDate dFechaDeuda, QDate dFechaVto, QString cDocumento, int id_Tiquet,
                          int id_Factura, int nTipo, double rImporteDeuda, double rPagado = 0, double rPendienteCobro = 0,
                          QString cEntidad = "", QString cOficina = "", QString cDC="", QString cCuenta = "") {
    QSqlQuery qCliente(QSqlDatabase::database("empresa"));
    double rImporte;
    rImporte = this->rDeudaActual + rImporteDeuda;
    qCliente.prepare("Update clientes set  rDeudaActual = :rImporte where id =:id_cliente");
    qCliente.bindValue(":rImporte",rImporte);
    qCliente.bindValue(":id", id_cliente);
    if(!qCliente.exec()) {
        QMessageBox::critical(NULL,tr("Añadir deuda cliente"),tr("Ha fallado la inserción de la deuda en la ficha del paciente"),tr("&Aceptar"));
    }

    QSqlQuery qClientes_Deuda(QSqlDatabase::database("empresa"));
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
        QMessageBox::critical(NULL,tr("Añadir deuda cliente"),tr("Falló la inserción en la tabla de deudas"),tr("&Aceptar"));
    }

}
void Cliente::DescontarDeuda(int id_deuda, double rPagado){
    QSqlQuery qClientes_deuda(QSqlDatabase::database("empresa"));
    qClientes_deuda.prepare("Select * from clientes_deuda where id =:id_deuda");
    qClientes_deuda.bindValue(":id_deuda",id_deuda);
    if (!qClientes_deuda.exec()) {
        QMessageBox::critical(NULL,tr("Modificar deuda Cliente"),tr("Falló la lectura de la deuda del cliente"),tr("&Aceptar"));
    }
    qClientes_deuda.prepare("update clientes_deuda ");
    qClientes_deuda.clear();

}
void Cliente::Borrar(int id_cliente) {
    QSqlQuery qryCliente(QSqlDatabase::database("empresa"));
    frmDecision msgBox;
    msgBox.Inicializar("Borrar Ficha","Está apunto de borrar la ficha de un cliente","¿Desea continuar?","","Sí","No");
    int elegido = msgBox.exec();
   if(elegido == QMessageBox::Yes) {
        qryCliente.prepare("Delete from clientes where id = :id_Cliente");
        qryCliente.bindValue(":id",id_cliente);
        if(!qryCliente.exec()){
           QMessageBox::critical(NULL,tr("Borrar cliente"),tr("Falló el borrado de la deuda del cliente"),tr("&Aceptar"));
        }


     }
}

// Setters
 void Cliente::setcCodigoCliente(QString cCodigo) {
    this->cCodigoCliente = cCodigo;
}

 void Cliente::setId(int nId) {
    this->id = nId;
 }

void Cliente::setcApellido1(QString cApellido) {
    this->cApellido1 = cApellido;
}

void Cliente::setcApellido2(QString cApellido) {
    this->cApellido2 = cApellido;
}


void Cliente::setcNombre(QString cNombre) {
    this->cNombre = cNombre;
}

void Cliente::setcNombreFiscal(QString cNombreFiscal) {
    this->cNombreFiscal = cNombreFiscal;
}
void Cliente::setcNombreComercial(QString cNombreComercial) {
    this->cNombreComercial = cNombreComercial;
}

void Cliente::setcPersonaContacto(QString cPersonaContacto) {
    this->cPersonaContacto = cPersonaContacto;
}

void Cliente::setcCifNif(QString cCifNif) {
    this->cCifNif = cCifNif;
}

void Cliente::setcDireccion1(QString cDireccion1) {
    this->cDireccion1 = cDireccion1;
}

void Cliente::setcDireccion2(QString cDireccion2) {
    this->cDireccion2 = cDireccion2;
}

void Cliente::setcCp(QString cCp) {
    this->cCp = cCp;
}

void Cliente::setcPoblacion(QString cPoblacion) {
    this->cPoblacion = cPoblacion;
}

void Cliente::setcProvincia(QString cProvincia) {
    this->cProvincia = cProvincia;
}

void Cliente::setcPais(QString cPais) {
    this->cPais = cPais;

}

void Cliente::setcTelefono1(QString cTelefono1) {
    this->cTelefono1 = cTelefono1;
}

void Cliente::setcTelefono2(QString cTelefono2) {
    this->cTelefono2 = cTelefono2;
}

void Cliente::setcFax(QString cFax) {
    this->cFax = cFax;
}

void Cliente::setcMovil(QString cMovil) {
    this->cMovil = cMovil;
}

void Cliente::setcEmail(QString cEmail) {
    this->cEmail = cEmail;
}

void Cliente::setcWeb(QString cWeb) {
    this->cWeb = cWeb;
}

void Cliente::setcDireccionFactura1(QString cDireccionFactura1) {
  this->cDireccionFactura1 = cDireccionFactura1;
}


void Cliente::setcDireccionFactura2(QString cDireccionFactura2) {
    this->cDireccionFactura2 = cDireccionFactura2;
}

void Cliente::setcCPFactura(QString cCPFactura) {
    this->cCPFactura = cCPFactura;
}

void Cliente::setcPoblacionFactura(QString cPoblacionFactura) {
    this->cPoblacionFactura = cPoblacionFactura;
}

void Cliente::setcProvinciaFactura(QString cProvinciaFactura) {
    this->cProvinciaFactura = cProvinciaFactura;
}

void Cliente::setcPaisFactura(QString cPaisFactura) {
    this->cPaisFactura = cPaisFactura;
}

void Cliente::setcDireccionAlmacen1(QString cDireccionAlmacen) {
    this->cDireccionAlmacen = cDireccionAlmacen;
}

void Cliente::setcDireccionAlmacen2(QString cDireccionAlmacen2) {
    this->cDireccionAlmacen2 = cDireccionAlmacen2;
}

void Cliente::setcCPAlmacen(QString cCPAlmacen) {
    this->cCPAlmacen = cCPAlmacen;
}

void Cliente::setcPoblacionAlmacen(QString cPoblacionAlmacen) {
    this->cPoblacionAlmacen = cPoblacionAlmacen;
}

void Cliente::setcProvinciaAlmacen(QString cProvinciaAlmacen) {
    this->cProvinciaAlmacen = cProvinciaAlmacen;
}

void Cliente::setcPaisAlmacen(QString cPaisAlmacen) {
    this->cPaisAlmacen = cPaisAlmacen;
}

void Cliente::setdFechaAlta(QDate dFechaAlta) {
    this->dFechaalta = dFechaAlta;
}

void Cliente::setdFechaUltimaCompra(QDate dFechaUltimaCompra) {
    this->dFechaCompra = dFechaUltimaCompra;
}

void Cliente::setrAcumuladoVentas(double rAcumuladoVentas) {
    this->rAcumuladoVentas = rAcumuladoVentas;
}

void Cliente::setrVentasEjercicio(double rVentasEjercicio) {
    this->rVentasEjercicio = rVentasEjercicio;
}

void Cliente::setrRiesgoMaximo(double rRiesgoMaximo) {
    this->rRiesgoMaximo = rRiesgoMaximo;
}

void Cliente::setrDeudaActual(double rDeudaActual) {
    this->rDeudaActual  = rDeudaActual;
}

void Cliente::settComentarios(QString tComentarios) {
    this->tComentarios = tComentarios;
}

void Cliente::setisBloqueado(int lBloqueado) {
    this->lBloqueado  = lBloqueado;
}

void Cliente::settComentarioBloqueo(QString tComentarioBloqueo) {
    this->tComentarioBloqueo = tComentarioBloqueo;
}

void Cliente::setnPorcDtoCliente(double nPorcDtoCliente) {
    this->nPorcDtoCliente = nPorcDtoCliente;
}

void Cliente::setislRecargoEquivalencia(int lRecargoEquivalencia) {
    this->lRecargoEquivalencia = lRecargoEquivalencia;
}

void Cliente::setcCuentaContable(QString cCuentaContable) {
    this->cCuentaContable = cCuentaContable;
}

void Cliente::setcCuentaIvaRepercutido(QString cCuentaIvaRepercutido) {
    this->cCuentaIvaRepercutido = cCuentaIvaRepercutido;
}

void Cliente::setcCuentaDeudas(QString cCuentaDeudas) {
    this->cCuentaDeudas = cCuentaDeudas;
}

void Cliente::setcCuentaCobros(QString cCuentaCobros) {

    this->cCuentaCobros = cCuentaCobros;
}

void Cliente::setcFormaPago(QString cFormaPago) {
    this->cFormaPago = cFormaPago;
}

void Cliente::setnDiaPago1(int nDiaPago1) {
    this->nDiaPago1 = nDiaPago1;
}

void Cliente::setnDiaPago2(int nDiaPago2) {
    this->nDiaPago2 = nDiaPago2;
}

void Cliente::setnTarifaCliente(int nTarifaCliente) {
    this->nTarifaCliente = nTarifaCliente;
}

void Cliente::setrImporteaCuenta(double rImporteACuenta) {
    this->rImporteACuenta = rImporteACuenta;
}

void Cliente::setrVales(double rVales) {
    this->rVales = rVales;
}

void Cliente::setcEntidadBancaria(QString cEntidadBancaria) {
    this->cEntidadBancaria = cEntidadBancaria;
}

void Cliente::setcOficinaBancaria(QString cOficinaBancaria) {
    this->cOficinaBancaria = cOficinaBancaria;
}

void Cliente::setcDC(QString cDC) {
    this->cDc = cDC;
}

void Cliente::setcCuentaCorriente(QString cCuentaCorriente) {
    this->cCuentaCorriente = cCuentaCorriente;
}

void Cliente::setdFechaNacimiento(QDate dFechaNacimiento) {
    this->dFechaNacimiento = dFechaNacimiento;
}

void Cliente::setrImportePendiente(double rImportePendiente) {
    this->rImportePendiente = rImportePendiente;
}

void Cliente::setcAccesoWeb(QString cAccesoWeb) {
    this->cAccesoWeb = cAccesoWeb;
}

void Cliente::setcPasswordWeb(QString cPasswordWeb) {
    this->cPasswordWeb = cPasswordWeb;
}




 // Getters
int Cliente::getId() {
    return this->id;
}
QString Cliente::getcCodigoCliente() {
    return this->cCodigoCliente;
}
QString Cliente::getcApellido1() {
    return this->cApellido1;
}
QString Cliente::getcApellido2() {
    return this->cApellido2;
}

QString Cliente::getcNombre() {
    return this->cNombre;
}

QString Cliente::getcNombreFiscal() {
    return this->cNombreFiscal;
}

QString Cliente::getcNombreComercial() {
    return this->cNombreComercial;
}

QString Cliente::getcPersonaContacto() {
    return this->cPersonaContacto;
}

QString Cliente::getcCifNif() {
    return this->cCifNif;
}

QString Cliente::getcDireccion1() {
    return this->cDireccion1;
}

QString Cliente::getcDireccion2() {
    return this->cDireccion2;
}

QString Cliente::getcCP() {
    return this->cCp;
}

QString Cliente::getcPoblacion() {
    return this->cPoblacion;
}

QString Cliente::getcProvincia() {
    return this->cProvincia;
}

QString Cliente::getcPais() {
    return this->cPais;
}

QString Cliente::getcTelefono1() {
    return this->cTelefono1;
}

QString Cliente::getcTelefono2() {
    return this->cTelefono2;
}

QString Cliente::getcFax() {
    return this->cFax;
}

QString Cliente::getcMovil() {
    return this->cMovil;
}

QString Cliente::getcEmail() {
    return this->cEmail;
}

QString Cliente::getcWeb() {
    return this->cWeb;
}

QString Cliente::getcDireccionFactura1() {
    return this->cDireccionFactura1;
}

QString Cliente::getcDireccionFactura2() {
    return this->cDireccionFactura2;
}

QString Cliente::getcCPFactura() {
    return this->cCPFactura;
}

QString Cliente::getcPoblacionFactura() {
    return this->cPoblacionFactura;
}

QString Cliente::getcProvinciaFactura() {
    return this->cProvinciaFactura;
}

QString Cliente::getcPaisFactura() {
    return this->cPaisFactura;
}

QString Cliente::getcDireccionAlmacen() {
    return this->cDireccionAlmacen;
}

QString Cliente::getcDireccionAlmacen2() {
    return this->cDireccionAlmacen2;
}

QString Cliente::getcCPAlmacen() {
    return this->cCPAlmacen;
}

QString Cliente::getcPoblacionAlmacen() {
    return this->cPoblacionAlmacen;
}

QString Cliente::getcProvinciaAlmacen() {
    return this->cProvinciaAlmacen;
}

QString Cliente::getcPaisAlmacen() {
    return this->cPaisAlmacen;
}

QDate Cliente::getdFechaAlta() {
    return this->dFechaalta;
}

QDate Cliente::getdFechaUltimaCompra() {
    return this->dFechaCompra;
}

double Cliente::getrAcumuladoVentas() {
    return this->rAcumuladoVentas;
}

double Cliente::getrVentasEjercicio() {
    return this->rVentasEjercicio;
}

double Cliente::getrRiesgoMaximo() {
    return this->rRiesgoMaximo;
}
double Cliente::getrDeudaActual() {
    return this->rDeudaActual;
}

QString Cliente::gettComentarios() {
    return this->tComentarios;
}

int Cliente::islBloqueado() {
    return this->lBloqueado;
}

QString Cliente::gettComentarioBloqueo() {
    return this->tComentarioBloqueo;
}

double Cliente::getnPorcDtoCliente() {
    return this->nPorcDtoCliente;
}

int Cliente::islRecargoEquivalencia() {
   return this->lRecargoEquivalencia;
}

QString Cliente::getcCuentaContable() {
    return this->cCuentaContable;
}

QString Cliente::getcCuentaIvaRepercutido() {
    return this->cCuentaIvaRepercutido;
}

QString Cliente::getcCuentaDeudas() {
    return this->cCuentaDeudas;
}

QString Cliente::getcCuentaCobros() {
    return this->cCuentaCobros;
}

QString Cliente::getcFormaPago() {
    return this->cFormaPago;
}

int Cliente::getnDiaPago1() {
    return this->nDiaPago1;
}

int Cliente::getnDiaPago2(){
    return this->nDiaPago2;
}
int Cliente::getnTarifaCliente() {
    return this->nTarifaCliente;
}

double Cliente::getrImporteACuenta() {
    return this->rImporteACuenta;
}

double Cliente::getrVales(){
    return this->rVales;
}

QString Cliente::getcEntidadBancaria(){
    return this->cEntidadBancaria;
}

QString Cliente::getcOficinaBancaria(){
    return this->cOficinaBancaria;
}

QString Cliente::getcDC() {
    return this->cDc;
}

QString Cliente::getcCuentaCorriente() {
    return this->cCuentaCorriente;
}

QDate Cliente::getdFechaNacimiento() {
    return this->dFechaNacimiento;
}

double Cliente::getrImportePendiente() {
    return this->rImportePendiente;
}

QString Cliente::getcAccesoWeb() {
    return this->cAccesoWeb;
}

QString Cliente::getcPasswordweb() {
    return this->cPasswordWeb;

}


