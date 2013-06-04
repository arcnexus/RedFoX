#include "articulo.h"
#include "../Almacen/frmtarifas.h"

Articulo::Articulo(QObject *parent) : QObject(parent)
{

}

void Articulo::Anadir()
{
    QSqlQuery query(QSqlDatabase::database("Maya"));
    query.prepare("INSERT INTO articulos (cCodigo)"
                       " VALUES (:cCodigo)");
    if(Configuracion_global->Autocodigoart ==false)
         query.bindValue(":cCodigo","");
    else

        query.bindValue(":cCodigo","autocodigo");


     if(!query.exec()) {
         QMessageBox::warning(qApp->activeWindow(),tr("Añadir Artículo"),
                              tr("Falló la inserción de un nuevo artículo : %1").arg(query.lastError().text()),
                              QObject::tr("Ok"));
     } else {
         this->id = query.lastInsertId().toInt();
         Recuperar("Select * from articulos where id = "+QString::number(this->id));
         //--------------------------
         // Añado tarifas a artículo
         //--------------------------
         QSqlQuery queryTarifas(QSqlDatabase::database("Maya"));
         if(queryTarifas.exec("select * from codigotarifa")){
             while (queryTarifas.next()){
                 QSqlQuery anadirTarifa(QSqlDatabase::database("Maya"));
                 anadirTarifa.prepare("INSERT INTO tarifas ("
                                   "id_Articulo,"
                                   "id_pais,"
                                   "id_monedas,"
                                   "margen,"
                                   "margenminimo,"
                                   "id_codigotarifa) VALUES ("
                                   ":id_Articulo,"
                                   ":id_pais,"
                                   ":id_monedas,"
                                   ":margen,"
                                   ":margenminimo,"
                                   ":id_codigotarifa);");

                 anadirTarifa.bindValue(":id_Articulo",this->id);
                 anadirTarifa.bindValue(":id_pais",queryTarifas.record().field("id_pais").value().toInt());
                 anadirTarifa.bindValue(":id_monedas",queryTarifas.record().field("id_monedas").value().toInt());
                 anadirTarifa.bindValue(":id_codigotarifa",queryTarifas.record().value("id").toInt());
                 anadirTarifa.bindValue(":margen",Configuracion_global->margen);
                 anadirTarifa.bindValue(":margenminimo",Configuracion_global->margen_minimo);
                 if(!anadirTarifa.exec())
                     QMessageBox::warning(qApp->activeWindow(),tr("Añadir tarifa"),
                                          tr("Falló la inserción de la tarifa: %1").arg(anadirTarifa.lastError().text()),
                                          tr("Aceptar"));
             }
         }
     }


}


bool Articulo::Recuperar(QString cSQL)
{
    QSqlQuery qryArticulo(QSqlDatabase::database("Maya"));
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

               this->rDto = registro.field("rDto").value().toDouble();
               this->dUltimaCompra = registro.field("dUltimaCompra").value().toDate();
               this->dUltimaVenta = registro.field("dUltimaVenta").value().toDate();

               this->nUnidadesCompradas = registro.field("nUnidadesCompradas").value().toInt();
               this->nUnidadesVendidas = registro.field("nUnidadesVendidas").value().toInt();
               this->rAcumuladoCompras = registro.field("rAcumuladoCompras").value().toDouble();
               this->rAcumuladoVentas = registro.field("rAcumuladoVentas").value().toDouble();
               this->tComentario = registro.field("tComentario").value().toString();
               this->nStockMaximo = registro.field("nStockMaximo").value().toInt();
               this->nStockMinimo = registro.field("nStockMinimo").value().toInt();
               this->nStockReal = registro.field("nStockReal").value().toInt();
               this->nStockFisico = registro.field("stockfisico").value().toInt();
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
               this->id_tiposiva = registro.field("id_tiposiva").value().toInt();
               this->idsubsubfamilia = registro.field("idsubsubfamilia").value().toInt();
               this->idgrupoart = registro.field("idgrupoart").value().toInt();
               this->idweb = registro.field("idweb").value().toInt();
               this->articulopromocionado = registro.field("articulopromocionado").value().toBool();
               this->descripcion_promocion = registro.field("descripcion_promocion").value().toString();
               this->tipo_oferta = registro.field("tipo_oferta").value().toInt();
               this->por_cada = registro.field("por_cada").value().toInt();
               this->regalode= registro.field("regalode").value().toInt();
               this->porc_dto_web = registro.field("porc_dto_web").value().toDouble();
               this->oferta_pvp_fijo = registro.field("oferta_pvp_fijo").value().toDouble();
               this->comentario_oferta =registro.field("comentario_oferta").value().toString();
               this->margen = registro.field("margen").value().toDouble();
               this->margen_min = registro.field("margen_min").value().toDouble();

               // Recupero proveedor
               QSqlQuery *qryProveedor = new QSqlQuery(QSqlDatabase::database("Maya"));
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
    QSqlQuery qryArticulo(QSqlDatabase::database("Maya"));
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
               this->rDto = registro.field("rDto").value().toDouble();
               this->dUltimaCompra = registro.field("dUltimaCompra").value().toDate();
               this->dUltimaVenta = registro.field("dUltimaVenta").value().toDate();
               this->nUnidadesCompradas = registro.field("nUnidadesCompradas").value().toInt();
               this->nUnidadesVendidas = registro.field("nUnidadesVendidas").value().toInt();
               this->rAcumuladoCompras = registro.field("rAcumuladoCompras").value().toDouble();
               this->rAcumuladoVentas = registro.field("rAcumuladoVentas").value().toDouble();
               this->tComentario = registro.field("tComentario").value().toString();
               this->nStockMaximo = registro.field("nStockMaximo").value().toInt();
               this->nStockMinimo = registro.field("nStockMinimo").value().toInt();
               this->nStockReal = registro.field("nStockReal").value().toInt();
               this->nStockFisico = registro.field("stockfisico").value().toInt();
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
               this->id_tiposiva = registro.field("id_tiposiva").value().toInt();
               this->idsubsubfamilia = registro.field("idsubsubfamilia").value().toInt();
               this->idgrupoart = registro.field("idgrupoart").value().toInt();
               this->idweb = registro.field("idweb").value().toInt();
               this->articulopromocionado = registro.field("articulopromocionado").value().toBool();
               this->descripcion_promocion = registro.field("descripcion_promocion").value().toString();
               this->tipo_oferta = registro.field("tipo_oferta").value().toInt();
               this->por_cada = registro.field("por_cada").value().toInt();
               this->regalode= registro.field("regalode").value().toInt();
               this->porc_dto_web = registro.field("porc_dto_web").value().toDouble();
               this->oferta_pvp_fijo = registro.field("oferta_pvp_fijo").value().toDouble();
               this->comentario_oferta =registro.field("comentario_oferta").value().toString();
               this->margen = registro.field("margen").value().toDouble();
               this->margen_min = registro.field("margen_min").value().toDouble();

               // Recupero proveedor
               QSqlQuery *qryProveedor = new QSqlQuery(QSqlDatabase::database("Maya"));
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
                   TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"Se ha llegado al final del fichero");
               if (nProcede == 2)
                   TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"Se ha llegado al inicio del fichero");
           }
    } else {
        QMessageBox::critical(qApp->activeWindow(),"error al leer datos artículo:", qryArticulo.lastError().text());
    }
}

void Articulo::Guardar()
{
    // --------------------------------------------
    // Si está activada la autogeneración de código
    //---------------------------------------------

    if (this->cCodigo == "autocodigo"){
        QString nuevo_codigo;
        QSqlQuery querycodigo(QSqlDatabase::database("Maya"));

        // seccion
        querycodigo.prepare("select cCodigo from secciones where id = :id");
        querycodigo.bindValue(":id",this->id_Seccion);
        querycodigo.exec();
        querycodigo.next();
        nuevo_codigo.append(querycodigo.record().value("cCodigo").toString().trimmed());

        // familia
        querycodigo.prepare("select cCodigo from familias where id = :id");
        querycodigo.bindValue(":id",this->id_Familia);
        querycodigo.exec();
        if(querycodigo.next())
            nuevo_codigo.append(querycodigo.record().value("cCodigo").toString().trimmed());
        else
            nuevo_codigo.append("000");
        //subfamilia
        querycodigo.prepare("select cCodigo from subfamilias where id = :id");
        querycodigo.bindValue(":id",this->id_SubFamilia);
        querycodigo.exec();
        if(querycodigo.next())
            nuevo_codigo.append(querycodigo.record().value("cCodigo").toString().trimmed());
        else
            nuevo_codigo.append("000");

        //buscamos ultimo número de la serie
        QString cSQL = "select cCodigo from articulos where cCodigo like '";
                cSQL.append(nuevo_codigo);
                cSQL.append("%' order by  cCodigo desc limit 0,1");
        if(querycodigo.exec(cSQL)){
            if (querycodigo.next())
            {
                QString ultcod;
                int tamano =  Configuracion_global->tamanocodigo;
                ultcod = querycodigo.record().value("cCodigo").toString();
                int aum = ultcod.right(4).toInt();
                aum++;

                QString nuev,cnum;
                cnum = QString::number(aum);
                nuev = cnum.fill('0',(4-cnum.trimmed().size())).append(QString::number(aum));
                this->cCodigo = ultcod.left(9).append(nuev);

            } else{
                this->cCodigo = nuevo_codigo+"0001";
            }

        } else
        {
            this->cCodigo = nuevo_codigo +"0001";
        }

    }
    QSqlQuery query(QSqlDatabase::database("Maya"));
    query.prepare( "UPDATE articulos SET "
                   "`cCodigo`=:cCodigo,"
                   "`cCodigoBarras` =:cCodigoBarras,"
                   "`cCodigoFabricante` =:cCodigoFabricante,"
                   "`cDescripcion` =:cDescripcion,"
                   "`cDescripcionReducida` =:cDescripcionReducida,"
                   "`id_Proveedor` =:id_Proveedor,"
                   "`id_Familia` =:id_Familia,"
                   "`id_Seccion` =:id_Seccion,"
                   "`id_Subfamilia` =:id_Subfamilia,"
                   "`nTipoIva` =:nTipoIva,"
                   "`rCoste` =:rCoste,"
                   "`rDto` =:rDto,"
                   "`nDtoProveedor` =:nDtoProveedor,"
                   "`dUltimaCompra` =:dUltimaCompra,"
                   "`dUltimaVenta` =:dUltimaVenta,"
                   "`nUnidadesCompradas` =:nUnidadesCompradas,"
                   "`nUnidadesVendidas` =:nUnidadesVendidas,"
                   "`rAcumuladoCompras` =:rAcumuladoCompras,"
                   "`rAcumuladoVentas` =:rAcumuladoVentas,"
                   "`tComentario` =:tComentario,"
                   "`nStockMaximo` =:nStockMaximo,"
                   "`nStockMinimo` =:nStockMinimo,"
                   "`nStockReal` =:nStockReal,"
                   "`stockfisico` = :stockfisico,"
                   "`cTipoUnidad` =:cTipoUnidad,"
                   "`lControlarStock` =:lControlarStock,"
                   "`lPvpIncluyeIva` =:lPvpIncluyeIva,"
                   "`dFechaPrevistaRecepcion` =:dFechaPrevistaRecepcion,"
                   "`nCantidadPendienteRecibir` =:nCantidadPendienteRecibir,"
                   "`nReservados` =:nReservados,"
                   "`lMostrarWeb` =:lMostrarWeb,"
                   "`nEtiquetas` =:nEtiquetas,"
                   "`nPaquetes` =:nPaquetes,"
                   "`cLocalizacion` =:cLocalizacion,"
                   "`id_tiposiva` =:id_tiposiva,"
                   "`idsubsubfamilia` =:idsubsubfamilia,"
                   "`idgrupoart` =:idgrupoart,"
                   "`idweb` =:idweb,"
                   "`stockfisico` =:stockfisico,"
                   "`articulopromocionado` =:articulopromocionado,"
                   "`descripcion_promocion` =:descripcion_promocion,"
                   "`tipo_oferta` =:tipo_oferta,"
                   "`por_cada` =:por_cada,"
                   "`regalode` =:regalode,"
                   "`porc_dto_web` =:porc_dto_web,"
                   "`oferta_pvp_fijo` =:oferta_pvp_fijo,"
                   "`margen` =:margen,"
                   "`margen_min` =:margen_min,"
                   "`comentario_oferta` =:comentario_oferta"
                   " WHERE id =:id");


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
    query.bindValue(":rDto",this->rDto);
    query.bindValue(":dUltimaCompra",this->dUltimaCompra);
    query.bindValue(":dUltimaVenta",this->dUltimaVenta);
    query.bindValue(":nUnidadesCompradas",this->nUnidadesCompradas);
    query.bindValue(":nUnidadesVendidas",this->nUnidadesVendidas);
    query.bindValue(":rAcumuladoCompras",this->rAcumuladoCompras);
    query.bindValue(":rAcumuladoVentas",this->rAcumuladoVentas);
    query.bindValue(":tComentario",this->tComentario);
    query.bindValue(":nStockMaximo",this->nStockMaximo);
    query.bindValue(":nStockMinimo",this->nStockMinimo);
    query.bindValue(":nStockReal",this->nStockReal);
    query.bindValue(":stockfisico",this->nStockFisico);
    query.bindValue(":lControlarStock",this->lControlarStock);
    query.bindValue(":cComposicion",this->cComposicion);
    query.bindValue(":lPvpIncluyeIva",this->lPvpIncluyeIva);
    query.bindValue(":dFechaPrevistaRecepcion",dFechaPrevistaRecepcion);
    query.bindValue(":nCantidadPendienteRecibir",this->nCantidadPendienteRecibir);
    query.bindValue(":nReservados",this->nReservados);
    query.bindValue(":lMostrarWeb",this->lMostrarWeb);
    query.bindValue(":nEtiquetas",this->nEtiquetas);
    query.bindValue(":cLocalizacion",this->cLocalizacion);
    query.bindValue(":id",this->id);
    query.bindValue(":id_tiposiva",this->id_tiposiva);
    query.bindValue(":idsubsubfamilia",this->idsubsubfamilia);
    query.bindValue(":idgrupoart",this->idgrupoart);
    query.bindValue(":idweb",this->idweb);
    query.bindValue(":stockfisico",this->nStockFisico);
    query.bindValue(":rCoste",this->rCoste);
    query.bindValue(":articulopromocionado",this->articulopromocionado);
    query.bindValue(":descripcion_promocion",this->descripcion_promocion);
    query.bindValue(":tipo_oferta",this->tipo_oferta);
    query.bindValue(":por_cada",this->por_cada);
    query.bindValue(":regalode",this->regalode);
    query.bindValue(":porc_dto_web",this->porc_dto_web);
    query.bindValue(":oferta_pvp_fijo",this->oferta_pvp_fijo);
    query.bindValue(":comentario_oferta",this->comentario_oferta);
    query.bindValue(":margen",this->margen);
    query.bindValue(":margen_min",this->margen_min);

    if(!query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Guardar Artículo"),
                             QObject::tr("No se puede guardar el artículo ERROR: %1 ").arg(query.lastError().text()),
                             QObject::tr("Ok"));

    } else {
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),
                                                  QObject::tr("Se ha guardado el artículo en la base de datos"));

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
    this->rDto = 0;
    this->dUltimaCompra = QDate::currentDate();
    this->dUltimaVenta = QDate::currentDate();
    this->nUnidadesCompradas = 0;
    this->nUnidadesVendidas = 0;
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
    this->articulopromocionado = false;
    this->descripcion_promocion = "";
    this->tipo_oferta = 0;
    this->por_cada = 0;
    this->regalode = 0;
    this->porc_dto_web = 0;
    this->oferta_pvp_fijo = 0;
    this->comentario_oferta = "";
    this->margen = 0;
    this->margen_min = 0;
    this->nStockFisico =0;

}

void Articulo::Borrar(int nId)
{
    if(QMessageBox::question(qApp->activeWindow(),qApp->tr("Borrar Artículo"),
                             qApp->tr("¿Desea realmente Borrar este artículo?\nEsta opción no se puede deshacer"),
                             qApp->tr("No"),qApp->tr("Si")) == QMessageBox::Accepted)
    {
        QSqlQuery qryArticulo(QSqlDatabase::database("Maya"));
        qryArticulo.prepare("Delete from articulos where id = :nId");
        // TODO - Borrar estadisticas y tarifas

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

void Articulo::CargarImagen(QLabel *label, QLabel *label2, QLabel *label3, QLabel *label4)
{
    QSqlQuery qryArticulo(QSqlDatabase::database("Maya"));
    qryArticulo.prepare("Select bImagen,bImagen2,bImagen3,bImagen4 from articulos where Id = :id");
    qryArticulo.bindValue(":id",this->id);
    if (qryArticulo.exec()) {
        if (qryArticulo.next()){
            //--------
            // imagen1
            //--------
            QSqlRecord registro =  qryArticulo.record();
            QByteArray ba1 = registro.field("bImagen").value().toByteArray();
            QPixmap pm1;
            pm1.loadFromData(ba1);
            if(!registro.field("bImagen").value().isNull())
                label->setPixmap(pm1);
            //--------
            // imagen2
            //--------
            ba1 = registro.field("bImagen2").value().toByteArray();
            QPixmap pm12;
            pm12.loadFromData(ba1);
            if(!registro.field("bImagen2").value().isNull())
                label2->setPixmap(pm12);
            //--------
            // imagen3
            //--------
            ba1 = registro.field("bImagen3").value().toByteArray();
            QPixmap pm13;
            pm13.loadFromData(ba1);
            if(!registro.field("bImagen3").value().isNull())
                label3->setPixmap(pm13);
            //--------
            // imagen4
            //--------
            ba1 = registro.field("bImagen4").value().toByteArray();
            QPixmap pm14;
            pm14.loadFromData(ba1);
            if(!registro.field("bImagen4").value().isNull())
                label4->setPixmap(pm14);
        } else
            QMessageBox::warning(qApp->activeWindow(),QObject::tr("Error al recuperar"),
                                    QObject::tr("No se pueden recuperar la imagenes asociadas al artículo"),
                                    QObject::tr("Ok"));
    }
}

int Articulo::getIdSeccion(QString cSeccion_)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select id from secciones where cSeccion = :cValor ");
    Query.bindValue(":cValor",cSeccion_);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Seccion"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("id").value().toInt();
}

int Articulo::getIdFamilia(QString cFamilia_)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select id from familias where cFamilia = :cValor ");
    Query.bindValue(":cValor",cFamilia_);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Familia"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("id").value().toInt();
}

int Articulo::getIdSubFamilia(QString cSubfamilia_)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select id from subfamilias where cSubfamilia = :cValor ");
    Query.bindValue(":cValor",cSubfamilia_);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Sub-Familia"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("id").value().toInt();
}

int Articulo::getIdSubSufFamilia(QString cSubSubFamilia_)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select id from subsubfamilias where subsubfamilia = :cValor ");
    Query.bindValue(":cValor",cSubSubFamilia_);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Sub-Sub-Familia"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("id").value().toInt();
}

int Articulo::getIdGrupo(QString cGrupo_)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select id from gruposart where grupoart = :cValor ");
    Query.bindValue(":cValor",cGrupo_);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Grupo Artículo"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("id").value().toInt();
}

QString Articulo::getcSeccion(int nId)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select cSeccion from secciones where id = :cValor ");
    Query.bindValue(":cValor",nId);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Secciones"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("cSeccion").value().toString();
}
QString Articulo::getcFamilia(int nId)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select cFamilia from familias where id = :cValor ");
    Query.bindValue(":cValor",nId);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Familias"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("cFamilia").value().toString();
}



QString Articulo::getcSubFamilia(int nId)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select cSubfamilia from subfamilias where id = :cValor ");
    Query.bindValue(":cValor",nId);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Sub-Familias"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("cSubfamilia").value().toString();

}

QString Articulo::getcSubSubFamilia(int nId)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select subsubfamilia from subsubfamilias where id = :cValor ");
    Query.bindValue(":cValor",nId);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Sub-sub-Familias"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("subsubfamilia").value().toString();
}

QString Articulo::getcGrupo(int nId)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select grupoart from gruposart where id = :cValor ");
    Query.bindValue(":cValor",nId);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Grupo Artículo"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("grupoart").value().toString();
}

bool Articulo::agregar_proveedor_alternativo(int id_art, int id_Proveedor, QString codigo, double pvd, QString descoferta, QString oferta,
                                             double pvdreal,int id_divisa)
{
    QSqlQuery query_proveedor_alternativo(QSqlDatabase::database("Maya"));

    query_proveedor_alternativo.prepare("INSERT INTO articulos_prov_frec "
                                        "(id_Articulo,id_Proveedor,pvd,oferta,codigo,descoferta,pvdreal,id_divisa)"
                                        " VALUES (:id_Articulo,:id_Proveedor,:pvd, :oferta,:codigo,:descoferta,:pvdreal,:id_divisa)");
    query_proveedor_alternativo.bindValue(":id_Articulo",id_art);
    query_proveedor_alternativo.bindValue(":id_Proveedor",id_Proveedor);
    query_proveedor_alternativo.bindValue(":codigo",codigo);
    query_proveedor_alternativo.bindValue(":pvd",pvd);
    query_proveedor_alternativo.bindValue(":oferta",oferta);
    query_proveedor_alternativo.bindValue(":descoferta",descoferta);
    query_proveedor_alternativo.bindValue(":pvdreal",pvdreal);
    query_proveedor_alternativo.bindValue(":id_divisa",id_divisa);
    if(!query_proveedor_alternativo.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Insertar proveedor frecuente"),
                             tr("Falló la inserción de un nuevo proveedor: %1").arg(query_proveedor_alternativo.lastError().text()));
        return false;


    } else
        return true;

}

bool Articulo::guardarProveedorAlternativo(int id, QString codigo, double pvd, QString descoferta, QString oferta, double pvdreal, int id_divisa)
{
    QSqlQuery query_proveedor_alternativo(QSqlDatabase::database("Maya"));

    query_proveedor_alternativo.prepare("UPDATE articulos_prov_frec  set pvd = :pvd,oferta = :oferta, codigo = :codigo, "
                                        "descoferta = :descoferta, pvdreal = :pvdreal, id_divisa =:id_divisa "
                                        "where id = :id");
    query_proveedor_alternativo.bindValue(":codigo",codigo);
    query_proveedor_alternativo.bindValue(":pvd",pvd);
    query_proveedor_alternativo.bindValue(":oferta",oferta);
    query_proveedor_alternativo.bindValue(":descoferta",descoferta);
    query_proveedor_alternativo.bindValue(":pvdreal",pvdreal);
    query_proveedor_alternativo.bindValue(":id_divisa",id_divisa);
    query_proveedor_alternativo.bindValue(":id",id);
    if(!query_proveedor_alternativo.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Actualizar proveedor frecuente"),
                             tr("Falló la modificación de un nuevo proveedor: %1").arg(query_proveedor_alternativo.lastError().text()));
        return false;


    } else
        return true;



}

bool Articulo::cambiarProveedorPrincipal(int id, int id_proveedor)
{
    QSqlQuery queryProveedor(QSqlDatabase::database("Maya"));
    queryProveedor.prepare("update articulos Set id_proveedor = :id_proveedor where id = :id");
    queryProveedor.bindValue(":id_proveedor",id_proveedor);
    queryProveedor.bindValue(":id",id);
    if (!queryProveedor.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Cambiar proveedor"),
                             tr("Falló el cambio de proveedor principal : %1").arg(queryProveedor.lastError().text()),
                             tr("Aceptar"));
        return false;
    }
    this->id_Proveedor = id_proveedor;
    return true;


}

bool Articulo::cambiar_pvp()
{
    QSqlQuery tarifas(QSqlDatabase::database("Maya"));
    tarifas.prepare("select * from tarifas where id_Articulo = :id");
    tarifas.bindValue(":id",this->id);

    if (tarifas.exec()){
        FrmTarifas editTarifa(qApp->activeWindow());
        while (tarifas.next())
        {
//            QSqlQuery qTarifa(QSqlDatabase::database("Maya"));
//            qTarifa.prepare("select * from codigotarifa where descripcion ='"+ this->ct+"'");

//            if(qTarifa.exec()){
//                qTarifa.next();

//                QString cMoneda = Configuracion_global->Devolver_moneda(qTarifa.record().field("id_monedas").value().toInt());
            editTarifa.capturar_datos(tarifas.record().value("id").toInt(),QString::number(this->rCoste,'f',2));
//            if (Configuracion_global->DivisaLocal != cMoneda)
//                Configuracion_global->getCambio(Configuracion_global->codDivisaLocal,editTarifa->cod_divisa);
//            else
//                editTarifa. asignarcambiodivisa(1);

            editTarifa.calcular_precio(tarifas.record().value("margen").toDouble());
            QSqlQuery queryTarifas(QSqlDatabase::database("Maya"));
            queryTarifas.prepare(
            "UPDATE tarifas SET "
            "margen = :margen,"
            "margenminimo = :margenminimo,"
            "pvp = :pvp "
            " WHERE id = :id");
            queryTarifas.bindValue(":margen",editTarifa.margen);
            queryTarifas.bindValue(":margenMinimo",editTarifa.margen_min);
            queryTarifas.bindValue(":pvp",editTarifa.pvpDivisa);
            queryTarifas.bindValue(":id",tarifas.record().value("id").toInt());
            if(!queryTarifas.exec()) {
                QMessageBox::warning(qApp->activeWindow(),tr("ATENCIÓN"),
                                     tr("Ocurrió un error al actualizar BD: %1").arg(queryTarifas.lastError().text()),
                                     tr("Aceptar"));
                return false;
            } else
                return true;

        }
    }

}

bool Articulo::agregarStock(int id, int value)
{
    // TODO controlar stock
    return true;
}

QString Articulo::autocodigo()
{
    QString codigoIni;
    int tamanocodigoIni;
    codigoIni = this->cSeccion+this->cFamilia+this->cSubfamilia+this->cSubSubFamilia+this->cGrupoArt;
    tamanocodigoIni = codigoIni.length();
    QSqlQuery queryArt("select codigo from articulos where codigo like '"+codigoIni+"%' order by codigo desc limit 1",
                       QSqlDatabase::database("Maya"));
    if(queryArt.exec()){
        queryArt.next();
        QString lastcode = queryArt.record().value("codigo").toString();
        int Realsize = lastcode.length();
        QString code = lastcode.mid(tamanocodigoIni,(Realsize-tamanocodigoIni));
        int icode = code.toInt();
        icode++;
        code = QString::number(icode);
        while (code.length()< ( Realsize - tamanocodigoIni) )
        {
            code.prepend("0");
        }
        return codigoIni + code;
    }

}

