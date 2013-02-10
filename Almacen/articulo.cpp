#include "articulo.h"

Articulo::Articulo(QObject *parent) : QObject(parent)
{

}

void Articulo::Anadir()
{
    QSqlQuery query(QSqlDatabase::database("terra"));
         query.prepare("INSERT INTO articulos (cCodigo,cCodigoBarras,cCodigoFabricante,cDescripcion,cDescripcionReducida,"
                       "id_Proveedor,id_Familia,id_Seccion,id_Subfamilia,nTipoIva,rCoste,"
                       "rTarifa1,rTarifa2,rTarifa3,rDto,nDtoProveedor,nDtoproveedor2,nDtoProveedor3,dUltimaCompra,"
                       "dUltimaVenta,nMargen1,nMargen2,nMargen3,rPrecioMedio,rPrecioMedio2,rPrecioMedio3,nUnidadesCompradas,"
                       "nUnidadesVendidas,rAcumuladoCompras,rAcumuladoVentas,tComentario,nStockMaximo,nStockMinimo,"
                       "nStockReal,lControlarStock,lPvpIncluyeIva,"
                       "dFechaPrevistaRecepcion,nCantidadPendienteRecibir,nReservados,lMostrarWeb,nEtiquetas,"
                       "cLocalizacion)"
                       " VALUES (:cCodigo,:cCodigoBarras,:cCodigoFabricante,:cDescripcion,:cDescripcionReducida,"
                       ":id_Proveedor,:id_Familia,:id_Seccion,:id_Subfamilia,:nTipoIva,:rCoste,"
                       ":rTarifa1,:rTarifa2,:rTarifa3,:rDto,:nDtoProveedor,:nDtoproveedor2,:nDtoProveedor3,:dUltimaCompra,"
                       ":dUltimaVenta,:nMargen1,:nMargen2,:nMargen3,:rPrecioMedio,:rPrecioMedio2,:rPrecioMedio3,:nUnidadesCompradas,"
                       ":nUnidadesVendidas,:rAcumuladoCompras,:rAcumuladoVentas,:tComentario,:nStockMaximo,:nStockMinimo,"
                       ":nStockReal,:lControlarStock,:lPvpIncluyeIva,"
                       ":dFechaPrevistaRecepcion,:nCantidadPendienteRecibir,:nReservados,:lMostrarWeb,:nEtiquetas,"
                       ":cLocalizacion)");

         query.bindValue(":cCodigo",this->cCodigo);
         query.bindValue(":cCodigoBarras",this->cCodigoBarras);
         query.bindValue(":cCodigoFabricante",this->cCodigoFabricante);
         query.bindValue(":cDescripcion",this->cDescripcion);
         query.bindValue(":cDescripcionReducida",this->cDescripcionReducida);
         query.bindValue(":id_Proveedor",this->id_Proveedor);
         if (!this->id_Familia ==0)
            query.bindValue(":id_Familia",this->id_Familia);
         query.bindValue(":id_Seccion",this->id_Seccion);
         if(!this->id_SubFamilia ==0)
             query.bindValue(":id_Subfamilia",this->id_SubFamilia);
         query.bindValue(":nTipoIva",this->nTipoIva);
         query.bindValue(":rCoste",this->rCoste);
         query.bindValue(":rTarifa1",this->rTarifa1);
         query.bindValue(":rTarifa2",this->rTarifa2);
         query.bindValue(":rTarifa3",this->rTarifa3);
         query.bindValue(":rDto",this->rDto);
         query.bindValue(":nDtoProveedor",this->nDtoProveedor);
         query.bindValue(":nDtoproveedor2",this->nDtoProveedor2);
         query.bindValue(":nDtoProveedor3",this->nDtoProveedor3);
         query.bindValue(":dUltimaCompra",this->dUltimaCompra);
         query.bindValue(":dUltimaVenta",this->dUltimaVenta);
         query.bindValue(":nMargen1",this->nMargen1);
         query.bindValue(":nMargen2",this->nMargen2);
         query.bindValue(":nMargen3",this->nMargen3);
         query.bindValue(":rPrecioMedio",this->rPrecioMedio);
         query.bindValue(":rPrecioMedio2",this->rPrecioMedio2);
         query.bindValue(":rPrecioMedio3",this->rPrecioMedio3);
         query.bindValue(":nUnidadesCompradas",this->nUnidadesCompradas);
         query.bindValue(":nUnidadesVendidas",this->nUnidadesVendidas);
         query.bindValue(":rAcumuladoCompras",this->rAcumuladoCompras);
         query.bindValue(":rAcumuladoVentas",this->rAcumuladoVentas);
         query.bindValue(":tComentario",this->tComentario);
         query.bindValue(":nStockMaximo",this->nStockMaximo);
         query.bindValue(":nStockMinimo",this->nStockMinimo);
         query.bindValue(":nStockReal",this->nStockReal);
         query.bindValue(":lControlarStock",this->lControlarStock);
         query.bindValue(":lPvpIncluyeIva",this->lPvpIncluyeIva);
         query.bindValue(":dFechaPrevistaRecepcion",this->dFechaPrevistaRecepcion);
         query.bindValue(":nCantidadPendienteRecibir",this->nCantidadPendienteRecibir);
         query.bindValue(":nReservados",this->nReservados);
         query.bindValue(":lMostrarWeb",this->lMostrarWeb);
         query.bindValue(":nEtiquetas",this->nEtiquetas);
         query.bindValue(":cLocalizacion",this->cLocalizacion);


         if(!query.exec()) {
             QMessageBox::warning(qApp->activeWindow(),tr("Añadir Artículo"),
                                  tr("Falló la inserción de un nuevo artículo : %1").arg(query.lastError().text()),
                                  QObject::tr("Ok"));
         } else {
             QString cID = query.lastInsertId().toString();
             QString cSQL = "Select * from articulos where id ="+cID;
             Recuperar(cSQL);
         }


}


bool Articulo::Recuperar(QString cSQL)
{
    QSqlQuery qryArticulo(QSqlDatabase::database("terra"));
    qryArticulo.prepare(cSQL);
    if (qryArticulo.exec())
    {
           if (qryArticulo.next())
           {
               QSqlRecord registro =  qryArticulo.record();
               this->id = registro.field("id").value().toInt();
               this->cCodigo = registro.field("cCodigo").value().toString();
               this->cCodigoBarras = registro.field("cCodigoBarras").value().toString();
               this->cCodigoFabricante = registro.field("cCodigoFabricante").value().toString();
               this->cDescripcion = registro.field("cDescripcion").value().toString();
               this->cDescripcionReducida = registro.field("cDescripcionReducida").value().toString();
               this->id_Proveedor = registro.field("id_Proveedor").value().toInt();
               this->id_Familia = registro.field("id_Familia").value().toInt();
               this->cFamilia = registro.field("cFamilia").value().toString();
               this->id_Seccion = registro.field("id_Seccion").value().toInt();
               this->cSeccion = registro.field("cSeccion").value().toString();
               this->id_SubFamilia = registro.field("id_Subfamilia").value().toInt();
               this->cSubfamilia = registro.field("cSubfamilia").value().toString();
               this->nTipoIva = registro.field("nTipoIva").value().toDouble();
               this->rCoste = registro.field("rCoste").value().toDouble();
               this->rTarifa1 = registro.field("rTarifa1").value().toDouble();
               this->rTarifa2 = registro.field("rTarifa2").value().toDouble();
               this->rTarifa3 = registro.field("rTarifa3").value().toDouble();
               this->rDto = registro.field("rDto").value().toDouble();
               this->nDtoProveedor = registro.field("nDtoProveedor").value().toDouble();
               this->nDtoProveedor2 = registro.field("nDtoProveedor2").value().toDouble();
               this->nDtoProveedor3 = registro.field("nDtoProveedor3").value().toDouble();
               this->dUltimaCompra = registro.field("dUltimaCompra").value().toDate();
               this->dUltimaVenta = registro.field("dUltimaVenta").value().toDate();
               this->nMargen1 = registro.field("nMargen1").value().toDouble();
               this->nMargen2 = registro.field("nMargen2").value().toDouble();
               this->nMargen3 = registro.field("nMargen3").value().toDouble();
               this->rPrecioMedio = registro.field("rPrecioMedio").value().toDouble();
               this->nUnidadesCompradas = registro.field("nUnidadesCompradas").value().toInt();
               this->rPrecioMedio2 = registro.field("rPrecioMedio2").value().toDouble();
               this->nUnidadesVendidas = registro.field("nUnidadesVendidas").value().toInt();
               this->rPrecioMedio3 = registro.field("rPrecioMedio3").value().toDouble();
               this->rAcumuladoCompras = registro.field("rAcumuladoCompras").value().toDouble();
               this->rAcumuladoVentas = registro.field("rAcumuladoVentas").value().toDouble();
               this->tComentario = registro.field("tComentario").value().toString();
               this->nStockMaximo = registro.field("nStockMaximo").value().toInt();
               this->nStockMinimo = registro.field("nStockMinimo").value().toInt();
               this->nStockReal = registro.field("nStockReal").value().toInt();
               this->cTipoUnidad = registro.field("cTipoUnidad").value().toString();
               this->lControlarStock = registro.field("lControlarStock").value().toInt();
               this->cModelo = registro.field("cModelo").value().toString();
               this->cTalla = registro.field("cTalla").value().toString();
               this->cColor = registro.field("cColor").value().toString();
               this->cComposicion = registro.field("cComposicion").value().toString();
               this->lPvpIncluyeIva = registro.field("lPvpIncluyeIva").value().toInt();
               this->dFechaPrevistaRecepcion = registro.field("dFechaPrevistaRecepcion").value().toDate();
               this->nCantidadPendienteRecibir = registro.field("nCantidadPendienteRecibir").value().toInt();
               this->nReservados = registro.field("nReservados").value().toInt();
               this->lMostrarWeb = registro.field("lMostrarWeb").value().toInt();
               this->nEtiquetas = registro.field("nEtiquetas").value().toInt();
               this->nPaquetes = registro.field("nPaquetes").value().toInt();
               this->cLocalizacion = registro.field("cLocalizacion").value().toString();
               // Recupero proveedor
               QSqlQuery *qryProveedor = new QSqlQuery(QSqlDatabase::database("terra"));
               qryProveedor->prepare("select id,cCodigo,cProveedor from proveedores where id = :id");
               qryProveedor->bindValue(":id",this->id_Proveedor);
               if (!qryProveedor->exec()) {
                   QMessageBox::warning(qApp->activeWindow(),tr("Error Datos"),tr("No se encuentra el proveedor asociado \n Deberá comprovar ficha producto"),tr("OK"));

               } else {
                   qryProveedor->next();
                   QSqlRecord record = qryProveedor->record();
                   this->cProveedor = record.field("cProveedor").value().toString();
                   this->cCodProveedor = record.field("cCodigo").value().toString();
               }
               delete qryProveedor;

               return true;
           }
           else
           {
               QMessageBox::critical(qApp->activeWindow(),"Búsqueda de artículos", "No se encuentra el artículo");
               return false;
           }
    }
    else
    {
        QMessageBox::critical(qApp->activeWindow(),"error al leer datos artículo:", qryArticulo.lastError().text());
        return false;
    }
}

void Articulo::Recuperar(QString cSQL, int nProcede)
{
    QSqlQuery qryArticulo(QSqlDatabase::database("terra"));
    qryArticulo.prepare(cSQL);
    if (qryArticulo.exec()) {
           if (qryArticulo.next()){
               QSqlRecord registro =  qryArticulo.record();
               this->id = registro.field("id").value().toInt();
               this->cCodigo = registro.field("cCodigo").value().toString();
               this->cCodigoBarras = registro.field("cCodigoBarras").value().toString();
               this->cCodigoFabricante = registro.field("cCodigoFabricante").value().toString();
               this->cDescripcion = registro.field("cDescripcion").value().toString();
               this->cDescripcionReducida = registro.field("cDescripcionReducida").value().toString();
               this->id_Proveedor = registro.field("id_Proveedor").value().toInt();
               this->id_Familia = registro.field("id_Familia").value().toInt();
               this->cFamilia = registro.field("cFamilia").value().toString();
               this->id_Seccion = registro.field("id_Seccion").value().toInt();
               this->cSeccion = registro.field("cSeccion").value().toString();
               this->id_SubFamilia = registro.field("id_Subfamilia").value().toInt();
               this->cSubfamilia = registro.field("cSubfamilia").value().toString();
               this->nTipoIva = registro.field("nTipoIva").value().toDouble();
               this->rCoste = registro.field("rCoste").value().toDouble();
               this->rTarifa1 = registro.field("rTarifa1").value().toDouble();
               this->rTarifa2 = registro.field("rTarifa2").value().toDouble();
               this->rTarifa3 = registro.field("rTarifa3").value().toDouble();
               this->rDto = registro.field("rDto").value().toDouble();
               this->nDtoProveedor = registro.field("nDtoProveedor").value().toDouble();
               this->nDtoProveedor2 = registro.field("nDtoProveedor2").value().toDouble();
               this->nDtoProveedor3 = registro.field("nDtoProveedor3").value().toDouble();
               this->dUltimaCompra = registro.field("dUltimaCompra").value().toDate();
               this->dUltimaVenta = registro.field("dUltimaVenta").value().toDate();
               this->nMargen1 = registro.field("nMargen1").value().toDouble();
               this->nMargen2 = registro.field("nMargen2").value().toDouble();
               this->nMargen3 = registro.field("nMargen3").value().toDouble();
               this->rPrecioMedio = registro.field("rPrecioMedio").value().toDouble();
               this->nUnidadesCompradas = registro.field("nUnidadesCompradas").value().toInt();
               this->rPrecioMedio2 = registro.field("rPrecioMedio2").value().toDouble();
               this->nUnidadesVendidas = registro.field("nUnidadesVendidas").value().toInt();
               this->rPrecioMedio3 = registro.field("rPrecioMedio3").value().toDouble();
               this->rAcumuladoCompras = registro.field("rAcumuladoCompras").value().toDouble();
               this->rAcumuladoVentas = registro.field("rAcumuladoVentas").value().toDouble();
               this->tComentario = registro.field("tComentario").value().toString();
               this->nStockMaximo = registro.field("nStockMaximo").value().toInt();
               this->nStockMinimo = registro.field("nStockMinimo").value().toInt();
               this->nStockReal = registro.field("nStockReal").value().toInt();
               this->cTipoUnidad = registro.field("cTipoUnidad").value().toString();
               this->lControlarStock = registro.field("lControlarStock").value().toInt();
               this->cModelo = registro.field("cModelo").value().toString();
               this->cTalla = registro.field("cTalla").value().toString();
               this->cColor = registro.field("cColor").value().toString();
               this->cComposicion = registro.field("cComposicion").value().toString();
               this->lPvpIncluyeIva = registro.field("lPvpIncluyeIva").value().toInt();
               this->dFechaPrevistaRecepcion = registro.field("dFechaPrevistaRecepcion").value().toDate();
               this->nCantidadPendienteRecibir = registro.field("nCantidadPendienteRecibir").value().toInt();
               this->nReservados = registro.field("nReservados").value().toInt();
               this->lMostrarWeb = registro.field("lMostrarWeb").value().toInt();
               this->nEtiquetas = registro.field("nEtiquetas").value().toInt();
               this->nPaquetes = registro.field("nPaquetes").value().toInt();
               this->cLocalizacion = registro.field("cLocalizacion").value().toString();
               // Recupero proveedor
               QSqlQuery *qryProveedor = new QSqlQuery(QSqlDatabase::database("terra"));
               qryProveedor->prepare("select id,cCodigo,cProveedor from proveedores where id = :id");
               qryProveedor->bindValue(":id",this->id_Proveedor);
               if (!qryProveedor->exec()) {
                   QMessageBox::warning(qApp->activeWindow(),tr("Error Datos"),tr("No se encuentra el proveedor asociado \n Deberá comprovar ficha producto"),tr("OK"));

               } else {
                   qryProveedor->next();
                   QSqlRecord record = qryProveedor->record();
                   this->cProveedor = record.field("cProveedor").value().toString();
                   this->cCodProveedor = record.field("cCodigo").value().toString();
               }
               delete qryProveedor;

           } else {
               if (nProcede ==1)
                   QMessageBox::critical(qApp->activeWindow(),"Búsqueda de artículos", "Se ha llegado al final del fichero");
               if (nProcede == 2)
                   QMessageBox::critical(qApp->activeWindow(),"Búsqueda de artículos", "Se ha llegado al inicio del fichero");
           }
    } else {
        QMessageBox::critical(qApp->activeWindow(),"error al leer datos artículo:", qryArticulo.lastError().text());
    }
}

void Articulo::Guardar()
{
    QSqlQuery query(QSqlDatabase::database("terra"));
    query.prepare( "UPDATE articulos set "
                   "cCodigo =:cCodigo,"
                   "cCodigoBarras=:cCodigoBarras,"
                   "cCodigoFabricante =:cCodigoFabricante,"
                   "cDescripcion=:cDescripcion,"
                   "cDescripcionReducida=:cDescripcionReducida,"
                   "id_Proveedor=:id_Proveedor,"
                   "id_Familia=:id_Familia,"
                   "cFamilia =:cFamilia,"
                   "id_Seccion=:id_Seccion,"
                   "cSeccion=:cSeccion,"
                   "id_Subfamilia=:id_Subfamilia,"
                   "cSubfamilia=:cSubfamilia,"
                   "nTipoIva =:nTipoIva,"
                   "rCoste=:rCoste,"
                   "rTarifa1=:rTarifa1,"
                   "rTarifa2=:rTarifa2,"
                   "rTarifa3=:rTarifa3,"
                   "rDto=:rDto,"
                   "nDtoProveedor=:nDtoProveedor,"
                   "nDtoproveedor2=:nDtoProveedor2,"
                   "nDtoProveedor3=:nDtoProveedor3,"
                   "dUltimaCompra=:dUltimaCompra,"
                   "dUltimaVenta=:dUltimaVenta,"
                   "nMargen1=:nMargen1,"
                   "nMargen2=:nMargen2,"
                   "nMargen3=:nMargen3,"
                   "rPrecioMedio=:rPrecioMedio,"
                   "rPrecioMedio2=:rPrecioMedio2,"
                   "rPrecioMedio3=:rPrecioMedio3,"
                   "nUnidadesCompradas=:nUnidadesCompradas,"
                   "nUnidadesVendidas=:nUnidadesVendidas,"
                   "rAcumuladoCompras=:rAcumuladoCompras,"
                   "rAcumuladoVentas=:rAcumuladoVentas,"
                   "tComentario=:tComentario,"
                   "nStockMaximo=:nStockMaximo,"
                   "nStockMinimo=:nStockMinimo,"
                   "nStockReal=:nStockReal,"
                   "lControlarStock=:lControlarStock,"
                   "cModelo=:cModelo,"
                   "cTalla=:cTalla,"
                   "cColor=:cColor,"
                   "cComposicion=:cComposicion,"
                   "lPvpIncluyeIva=:lPvpIncluyeIva,"
                   "dFechaPrevistaRecepcion=:dFechaPrevistaRecepcion,"
                   "nCantidadPendienteRecibir=:nCantidadPendienteRecibir,"
                   "nReservados=:nReservados,"
                   "lMostrarWeb=:lMostrarWeb,"
                   "nEtiquetas =:nEtiquetas,"
                   "cLocalizacion =:cLocalizacion"
                   " where id = :id");

    query.bindValue(":cCodigo",this->cCodigo);
    query.bindValue(":cCodigoBarras",this->cCodigoBarras);
    query.bindValue(":cCodigoFabricante",this->cCodigoFabricante);
    query.bindValue(":cDescripcion",this->cDescripcion);
    query.bindValue(":cDescripcionReducida",this->cDescripcionReducida);
    query.bindValue(":id_Proveedor",this->id_Proveedor);
    query.bindValue(":id_Familia",this->id_Familia);
    query.bindValue(":cFamilia",this->cFamilia);
    query.bindValue(":id_Seccion",this->id_Seccion);
    query.bindValue(":cSeccion",this->cSeccion);
    query.bindValue(":id_Subfamilia",this->id_SubFamilia);
    query.bindValue(":cSubfamilia",this->cSubfamilia);
    query.bindValue(":nTipoIva",this->nTipoIva);
    query.bindValue(":rCoste",this->rCoste);
    query.bindValue(":rTarifa1",this->rTarifa1);
    query.bindValue(":rTarifa2",this->rTarifa2);
    query.bindValue(":rTarifa3",this->rTarifa3);
    query.bindValue(":rDto",this->rDto);
    query.bindValue(":nDtoProveedor",this->nDtoProveedor);
    query.bindValue(":nDtoProveedor2",this->nDtoProveedor2);
    query.bindValue(":nDtoProveedor3",this->nDtoProveedor3);
    query.bindValue(":dUltimaCompra",this->dUltimaCompra);
    query.bindValue(":dUltimaVenta",this->dUltimaVenta);
    query.bindValue(":nMargen1",this->nMargen1);
    query.bindValue(":nMargen2",this->nMargen2);
    query.bindValue(":nMargen3",this->nMargen3);
    query.bindValue(":rPrecioMedio",this->rPrecioMedio);
    query.bindValue(":rPrecioMedio2",this->rPrecioMedio2);
    query.bindValue(":rPrecioMedio3",this->rPrecioMedio3);
    query.bindValue(":nUnidadesCompradas",this->nUnidadesCompradas);
    query.bindValue(":nUnidadesVendidas",this->nUnidadesVendidas);
    query.bindValue(":rAcumuladoCompras",this->rAcumuladoCompras);
    query.bindValue(":rAcumuladoVentas",this->rAcumuladoVentas);
    query.bindValue(":tComentario",this->tComentario);
    query.bindValue(":nStockMaximo",this->nStockMaximo);
    query.bindValue(":nStockMinimo",this->nStockMinimo);
    query.bindValue(":nStockReal",this->nStockReal);
    query.bindValue(":lControlarStock",this->lControlarStock);
    query.bindValue(":cModelo",this->cModelo);
    query.bindValue(":cTalla",this->cTalla);
    query.bindValue(":cColor",this->cColor);
    query.bindValue(":cComposicion",this->cComposicion);
    query.bindValue(":lPvpIncluyeIva",this->lPvpIncluyeIva);
    query.bindValue(":dFechaPrevistaRecepcion",dFechaPrevistaRecepcion);
    query.bindValue(":nCantidadPendienteRecibir",this->nCantidadPendienteRecibir);
    query.bindValue(":nReservados",this->nReservados);
    query.bindValue(":lMostrarWeb",this->lMostrarWeb);
    query.bindValue(":nEtiquetas",this->nEtiquetas);
    query.bindValue(":cLocalizacion",this->cLocalizacion);
    query.bindValue(":id",this->id);


    if(!query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Guardar Artículo"),
                             QObject::tr("No se puede guardar el artículo ERROR: &1 ").arg(query.lastError().text()),
                             QObject::tr("Ok"));

    } else {
        QMessageBox::information(qApp->activeWindow(),QObject::tr("Guardar Artículo"),QObject::tr("Se ha guardado el artículo en la base de datos"),
                                 QObject::tr("Ok"));
    }

}

void Articulo::Vaciar()
{
    this->id = 0;
    this->cCodigo = "";
    this->cCodigoBarras="";
    this->cCodigoFabricante = "";
    this->cDescripcion = "";
    this->cDescripcionReducida = "";
    this->id_Proveedor = 0;
    this->id_Familia = 0;
    this->cFamilia = "";
    this->id_Seccion = 0;
    this->cSeccion = "";
    this->id_SubFamilia =0;
    this->cSubfamilia = "";
    this->nTipoIva = 0;
    this->rCoste = 0;
    this->rTarifa1 = 0;
    this->rTarifa2 = 0;
    this->rTarifa3 = 0;
    this->rDto = 0;
    this->nDtoProveedor = 0;
    this->nDtoProveedor2 = 0;
    this->nDtoProveedor3 = 0;
    this->dUltimaCompra = QDate::currentDate();
    this->dUltimaVenta = QDate::currentDate();
    this->nMargen1 = 0;
    this->nMargen2 = 0;
    this->nMargen3 = 0;
    this->rPrecioMedio = 0;
    this->nUnidadesCompradas = 0;
    this->rPrecioMedio2 = 0;
    this->nUnidadesVendidas = 0;
    this->rPrecioMedio3 = 0;
    this->rAcumuladoCompras = 0;
    this->rAcumuladoVentas = 0;
    //this->bImagen = registro.field(36).value().to....
    this->tComentario = "";
    this->nStockMaximo = 0;
    this->nStockMinimo = 0;
    this->nStockReal = 0;
    this->lControlarStock = 0;
    this->cModelo = "";
    this->cTalla = "";
    this->cColor = "";
    this->cComposicion = "";
    this->lPvpIncluyeIva = 0;
    this->dFechaPrevistaRecepcion = QDate::currentDate();
    this->nCantidadPendienteRecibir = 0;
    this->nReservados = 0;
    this->lMostrarWeb = 0;
    this->nEtiquetas = 0;
    this->cLocalizacion = "";
}

void Articulo::Borrar(int nId)
{
    if(QMessageBox::question(qApp->activeWindow(),qApp->tr("Borrar Artículo"),
                             qApp->tr("¿Desea realmente Borrar este artículo?\nEsta opción no se puede deshacer"),
                             qApp->tr("No"),qApp->tr("Si")) == QMessageBox::Accepted)
    {
        QSqlQuery qryArticulo(QSqlDatabase::database("terra"));
        qryArticulo.prepare("Delete from articulos where id = :nId");
        qryArticulo.bindValue(":id",nId);
        if(!qryArticulo.exec())
        {
            QMessageBox::critical(qApp->activeWindow(),QObject::tr("Borrar Artíclo"),QObject::tr("Falló el borrado del Artículo"),QObject::tr("&Aceptar"));
        }
        else
        {
            // Busco el id más proximo
            qryArticulo.prepare("select * from articulos where id <:nId");
            qryArticulo.bindValue(":nId",this->id);
            qryArticulo.exec();
            QSqlRecord registro = qryArticulo.record();
            this->id = registro.field("Id").value().toInt();
        }
    }
}

void Articulo::Vender(int id, int cantidad, double rPVP)
{
}

void Articulo::Devolucion(int id, int cantidad, double rImporte, QString cMotivo, QString dFechaDevolucion)
{
}

void Articulo::CargarImagen(QLabel *label)
{
    QSqlQuery qryArticulo(QSqlDatabase::database("terra"));
    qryArticulo.prepare("Select bImagen from articulos where Id = :id");
    qryArticulo.bindValue(":id",this->id);
    if (qryArticulo.exec()) {
           if (qryArticulo.next()){
               QSqlRecord registro =  qryArticulo.record();



               QByteArray ba1 = registro.field("bImagen").value().toByteArray();
               QPixmap pm1;
               pm1.loadFromData(ba1);
               label->setPixmap(pm1);
          } else
               QMessageBox::warning(qApp->activeWindow(),QObject::tr("Error al recuperar"),
                                    QObject::tr("No se puede recuperar la imagen asociada al artículo"),QObject::tr("Ok"));
    }
}


















