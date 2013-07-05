#include "articulo.h"
#include "../Almacen/frmtarifas.h"

Articulo::Articulo(QObject *parent) : QObject(parent)
{

}

void Articulo::Anadir()
{
    QSqlQuery query(QSqlDatabase::database("Maya"));
    query.prepare("INSERT INTO articulos (codigo)"
                       " VALUES (:codigo)");
    if(Configuracion_global->auto_codigoart ==false)
         query.bindValue(":codigo","");
    else

        query.bindValue(":codigo","auto_codigo");


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
                                   "id_articulo,"
                                   "id_pais,"
                                   "id_monedas,"
                                   "margen,"
                                   "margen_minimo,"
                                   "id_codigo_tarifa) VALUES ("
                                   ":id_articulo,"
                                   ":id_pais,"
                                   ":id_monedas,"
                                   ":margen,"
                                   ":margen_minimo,"
                                   ":id_codigo_tarifa);");

                 anadirTarifa.bindValue(":id_articulo",this->id);
                 anadirTarifa.bindValue(":id_pais",queryTarifas.record().field("id_pais").value().toInt());
                 anadirTarifa.bindValue(":id_monedas",queryTarifas.record().field("id_monedas").value().toInt());
                 anadirTarifa.bindValue(":id_codigo_tarifa",queryTarifas.record().value("id").toInt());
                 anadirTarifa.bindValue(":margen",Configuracion_global->margen);
                 anadirTarifa.bindValue(":margen_minimo",Configuracion_global->margen_minimo);
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
               this->codigo = registro.field("codigo").value().toString();
               this->codigo_barras = registro.field("codigo_barras").value().toString();
               this->codigo_fabricante = registro.field("codigo_fabricante").value().toString();
               this->descripcion = registro.field("descripcion").value().toString();
               this->descripcion_reducida = registro.field("descripcion_reducida").value().toString();
               this->id_proveedor = registro.field("id_proveedor").value().toInt();
               this->id_familia = registro.field("id_familia").value().toInt();
              // this->familia = registro.field("familia").value().toString();
               this->id_seccion = registro.field("id_seccion").value().toInt();
              // this->seccion = registro.field("seccion").value().toString();
               this->id_subfamilia = registro.field("id_subfamilia").value().toInt();
               //this->subfamilia = registro.field("sub_familia").value().toString();
               this->tipo_iva =registro.field("tipo_iva").value().toDouble();
               this->codigo_iva = Configuracion_global->Devolver_descripcion_tipo_iva(this->tipo_iva);
               this->coste = registro.field("coste").value().toDouble();

               this->dto = registro.field("dto").value().toDouble();
               this->ultima_compra = registro.field("ultima_compra").value().toDate();
               this->ultima_venta = registro.field("ultima_venta").value().toDate();

               this->unidades_compradas = registro.field("unidades_compradas").value().toInt();
               this->unidades_vendidas = registro.field("unidades_vendidas").value().toInt();
               this->acumulado_compras = registro.field("acumulado_compras").value().toDouble();
               this->acumulado_ventas = registro.field("acumulado_ventas").value().toDouble();
               this->comentario = registro.field("comentario").value().toString();
               this->stock_maximo = registro.field("stock_maximo").value().toInt();
               this->stock_minimo = registro.field("stock_minimo").value().toInt();
               this->stock_real = registro.field("stock_real").value().toInt();
               this->nstock_fisico_almacen = registro.field("stock_fisico_almacen").value().toInt();
               this->tipo_unidad = registro.field("tipo_unidad").value().toString();
               this->controlar_stock = registro.field("controlar_stock").value().toInt();
               this->modelo = registro.field("modelo").value().toString();
               this->talla = registro.field("talla").value().toString();
               this->color = registro.field("color").value().toString();
               this->composicion = registro.field("composicion").value().toString();
               this->pvp_incluye_iva = registro.field("pvp_incluye_iva").value().toInt();
               this->fecha_prevista_recepcion = registro.field("fecha_prevista_recepcion").value().toDate();
               this->cantidad_pendiente_recibir = registro.field("cantidad_pendiente_recibir").value().toInt();
               this->reservados = registro.field("reservados").value().toInt();
               this->mostrar_web = registro.field("mostrar_web").value().toInt();
               this->etiquetas = registro.field("etiquetas").value().toInt();
               this->paquetes = registro.field("paquetes").value().toInt();
               this->localizacion_en_almacen = registro.field("localizacion_en_almacen").value().toString();
               this->id_tipos_iva = registro.field("id_tipos_iva").value().toInt();
               this->id_subsub_familia = registro.field("id_subsub_familia").value().toInt();
               this->id_grupo_art = registro.field("id_grupo_art").value().toInt();
               this->id_web = registro.field("id_web").value().toInt();
               this->articulo_promocionado = registro.field("articulo_promocionado").value().toBool();
               this->descripcion_promocion = registro.field("descripcion_promocion").value().toString();
               this->tipo_oferta = registro.field("tipo_oferta").value().toInt();
               this->por_cada = registro.field("por_cada").value().toInt();
               this->regalo_de= registro.field("regalo_de").value().toInt();
               this->porc_dto_web = registro.field("porc_dto_web").value().toDouble();
               this->oferta_pvp_fijo = registro.field("oferta_pvp_fijo").value().toDouble();
               this->comentario_oferta =registro.field("comentario_oferta").value().toString();
               this->margen = registro.field("margen").value().toDouble();
               this->margen_min = registro.field("margen_min").value().toDouble();
               this->coste_real = registro.field("coste_real").value().toDouble();

               // Recupero proveedor
               QSqlQuery *qryProveedor = new QSqlQuery(QSqlDatabase::database("Maya"));
               qryProveedor->prepare("select id,codigo,proveedor from proveedores where id = :id");
               qryProveedor->bindValue(":id",this->id_proveedor);
               if (!qryProveedor->exec()) {
                   QMessageBox::warning(qApp->activeWindow(),tr("Error Datos"),tr("No se encuentra el proveedor asociado \n Deberá comprovar ficha producto"),tr("OK"));

               } else {
                   qryProveedor->next();
                   QSqlRecord record = qryProveedor->record();
                   this->proveedor = record.field("proveedor").value().toString();
                   this->cCodProveedor = record.field("codigo").value().toString();
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
               this->codigo = registro.field("codigo").value().toString();
               this->codigo_barras = registro.field("codigo_barras").value().toString();
               this->codigo_fabricante = registro.field("codigo_fabricante").value().toString();
               this->descripcion = registro.field("descripcion").value().toString();
               this->descripcion_reducida = registro.field("descripcion_reducida").value().toString();
               this->id_proveedor = registro.field("id_proveedor").value().toInt();
               this->id_familia = registro.field("id_familia").value().toInt();
               this->familia = registro.field("familia").value().toString();
               this->id_seccion = registro.field("id_seccion").value().toInt();
               this->seccion = registro.field("seccion").value().toString();
               this->id_subfamilia = registro.field("id_subfamilia").value().toInt();
               this->subfamilia = registro.field("subfamilia").value().toString();
               this->tipo_iva =registro.field("tipo_iva").value().toDouble();
               this->codigo_iva = Configuracion_global->Devolver_descripcion_tipo_iva(this->tipo_iva);
               this->coste = registro.field("coste").value().toDouble();
               this->dto = registro.field("dto").value().toDouble();
               this->ultima_compra = registro.field("ultima_compra").value().toDate();
               this->ultima_venta = registro.field("ultima_venta").value().toDate();
               this->unidades_compradas = registro.field("unidades_compradas").value().toInt();
               this->unidades_vendidas = registro.field("unidades_vendidas").value().toInt();
               this->acumulado_compras = registro.field("acumulado_compras").value().toDouble();
               this->acumulado_ventas = registro.field("acumulado_ventas").value().toDouble();
               this->comentario = registro.field("comentario").value().toString();
               this->stock_maximo = registro.field("stock_maximo").value().toInt();
               this->stock_minimo = registro.field("stock_minimo").value().toInt();
               this->stock_real = registro.field("stock_real").value().toInt();
               this->nstock_fisico_almacen = registro.field("stock_fisico_almacen").value().toInt();
               this->tipo_unidad = registro.field("tipo_unidad").value().toString();
               this->controlar_stock = registro.field("controlar_stock").value().toInt();
               this->modelo = registro.field("modelo").value().toString();
               this->talla = registro.field("talla").value().toString();
               this->color = registro.field("color").value().toString();
               this->composicion = registro.field("composicion").value().toString();
               this->pvp_incluye_iva = registro.field("pvp_incluye_iva").value().toInt();
               this->fecha_prevista_recepcion = registro.field("fecha_prevista_recepcion").value().toDate();
               this->cantidad_pendiente_recibir = registro.field("cantidad_pendiente_recibir").value().toInt();
               this->reservados = registro.field("reservados").value().toInt();
               this->mostrar_web = registro.field("mostrar_web").value().toInt();
               this->etiquetas = registro.field("etiquetas").value().toInt();
               this->paquetes = registro.field("paquetes").value().toInt();
               this->localizacion_en_almacen = registro.field("localizacion_en_almacen").value().toString();
               this->id_tipos_iva = registro.field("id_tipos_iva").value().toInt();
               this->id_subsub_familia = registro.field("id_subsub_familia").value().toInt();
               this->id_grupo_art = registro.field("id_grupo_art").value().toInt();
               this->id_web = registro.field("id_web").value().toInt();
               this->articulo_promocionado = registro.field("articulo_promocionado").value().toBool();
               this->descripcion_promocion = registro.field("descripcion_promocion").value().toString();
               this->tipo_oferta = registro.field("tipo_oferta").value().toInt();
               this->por_cada = registro.field("por_cada").value().toInt();
               this->regalo_de= registro.field("regalo_de").value().toInt();
               this->porc_dto_web = registro.field("porc_dto_web").value().toDouble();
               this->oferta_pvp_fijo = registro.field("oferta_pvp_fijo").value().toDouble();
               this->comentario_oferta =registro.field("comentario_oferta").value().toString();
               this->margen = registro.field("margen").value().toDouble();
               this->margen_min = registro.field("margen_min").value().toDouble();
               this->coste_real = registro.field("coste_real").value().toDouble();

               // Recupero proveedor
               QSqlQuery *qryProveedor = new QSqlQuery(QSqlDatabase::database("Maya"));
               qryProveedor->prepare("select id,codigo,proveedor from proveedores where id = :id");
               qryProveedor->bindValue(":id",this->id_proveedor);
               if (!qryProveedor->exec()) {
                   QMessageBox::warning(qApp->activeWindow(),tr("Error Datos"),tr("No se encuentra el proveedor asociado \n Deberá comprovar ficha producto"),tr("OK"));

               } else {
                   qryProveedor->next();
                   QSqlRecord record = qryProveedor->record();
                   this->proveedor = record.field("proveedor").value().toString();
                   this->cCodProveedor = record.field("codigo").value().toString();
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

    if (this->codigo == "auto_codigo"){
        QString nuevo_codigo;
        QSqlQuery querycodigo(QSqlDatabase::database("Maya"));

        // seccion
        querycodigo.prepare("select codigo from secciones where id = :id");
        querycodigo.bindValue(":id",this->id_seccion);
        querycodigo.exec();
        querycodigo.next();
        nuevo_codigo.append(querycodigo.record().value("codigo").toString().trimmed());

        // familia
        querycodigo.prepare("select codigo from familias where id = :id");
        querycodigo.bindValue(":id",this->id_familia);
        querycodigo.exec();
        if(querycodigo.next())
            nuevo_codigo.append(querycodigo.record().value("codigo").toString().trimmed());
        else
            nuevo_codigo.append("000");
        //subfamilia
        querycodigo.prepare("select codigo from subfamilias where id = :id");
        querycodigo.bindValue(":id",this->id_subfamilia);
        querycodigo.exec();
        if(querycodigo.next())
            nuevo_codigo.append(querycodigo.record().value("codigo").toString().trimmed());
        else
            nuevo_codigo.append("000");

        //buscamos ultimo número de la serie
        QString cSQL = "select codigo from articulos where codigo like '";
                cSQL.append(nuevo_codigo);
                cSQL.append("%' order by  codigo desc limit 0,1");
        if(querycodigo.exec(cSQL)){
            if (querycodigo.next())
            {
                QString ultcod;
                int tamano =  Configuracion_global->tamano_codigo;
                ultcod = querycodigo.record().value("codigo").toString();
                int aum = ultcod.right(4).toInt();
                aum++;

                QString nuev,cnum;
                cnum = QString::number(aum);
                nuev = cnum.fill('0',(4-cnum.trimmed().size())).append(QString::number(aum));
                this->codigo = ultcod.left(9).append(nuev);

            } else{
                this->codigo = nuevo_codigo+"0001";
            }

        } else
        {
            this->codigo = nuevo_codigo +"0001";
        }

    }
    QSqlQuery query(QSqlDatabase::database("Maya"));
    query.prepare( "UPDATE articulos SET "
                   "`codigo`=:codigo,"
                   "`codigo_barras` =:codigo_barras,"
                   "`codigo_fabricante` =:codigo_fabricante,"
                   "`descripcion` =:descripcion,"
                   "`descripcion_reducida` =:descripcion_reducida,"
                   "`id_proveedor` =:id_proveedor,"
                   "`id_familia` =:id_familia,"
                   "`id_seccion` =:id_seccion,"
                   "`id_subfamilia` =:id_subfamilia,"
                   "`tipo_iva` =:tipo_iva,"
                   "`coste` =:coste,"
                   "`dto` =:dto,"
                   "`dtoProveedor` =:dtoProveedor,"
                   "`ultima_compra` =:ultima_compra,"
                   "`ultima_venta` =:ultima_venta,"
                   "`unidades_compradas` =:unidades_compradas,"
                   "`unidades_vendidas` =:unidades_vendidas,"
                   "`acumulado_compras` =:acumulado_compras,"
                   "`acumulado_ventas` =:acumulado_ventas,"
                   "`comentario` =:comentario,"
                   "`stock_maximo` =:stock_maximo,"
                   "`stock_minimo` =:stock_minimo,"
                   "`stock_real` =:stock_real,"
                   "`stock_fisico_almacen` = :stock_fisico_almacen,"
                   "`tipo_unidad` =:tipo_unidad,"
                   "`controlar_stock` =:controlar_stock,"
                   "`pvp_incluye_iva` =:pvp_incluye_iva,"
                   "`fecha_prevista_recepcion` =:fecha_prevista_recepcion,"
                   "`cantidad_pendiente_recibir` =:cantidad_pendiente_recibir,"
                   "`reservados` =:reservados,"
                   "`mostrar_web` =:mostrar_web,"
                   "`etiquetas` =:etiquetas,"
                   "`paquetes` =:paquetes,"
                   "`localizacion_en_almacen` =:localizacion_en_almacen,"
                   "`id_tipos_iva` =:id_tipos_iva,"
                   "`id_subsub_familia` =:id_subsub_familia,"
                   "`id_grupo_art` =:id_grupo_art,"
                   "`id_web` =:id_web,"
                   "`stock_fisico_almacen` =:stock_fisico_almacen,"
                   "`articulo_promocionado` =:articulo_promocionado,"
                   "`descripcion_promocion` =:descripcion_promocion,"
                   "`tipo_oferta` =:tipo_oferta,"
                   "`por_cada` =:por_cada,"
                   "`regalo_de` =:regalo_de,"
                   "`porc_dto_web` =:porc_dto_web,"
                   "`oferta_pvp_fijo` =:oferta_pvp_fijo,"
                   "`margen` =:margen,"
                   "`margen_min` =:margen_min,"
                   "`coste_real` =:coste_real,"
                   "`comentario_oferta` =:comentario_oferta"
                   " WHERE id =:id");


    query.bindValue(":codigo",this->codigo);
    query.bindValue(":codigo_barras",this->codigo_barras);
    query.bindValue(":codigo_fabricante",this->codigo_fabricante);
    query.bindValue(":descripcion",this->descripcion);
    query.bindValue(":descripcion_reducida",this->descripcion_reducida);
    query.bindValue(":id_proveedor",this->id_proveedor);
    query.bindValue(":id_familia",this->id_familia);
    query.bindValue(":familia",this->familia);
    query.bindValue(":id_seccion",this->id_seccion);
    query.bindValue(":seccion",this->seccion);
    query.bindValue(":id_subfamilia",this->id_subfamilia);
    query.bindValue(":subfamilia",this->subfamilia);
    query.bindValue(":tipo_iva",this->tipo_iva);
    query.bindValue(":dto",this->dto);
    query.bindValue(":ultima_compra",this->ultima_compra);
    query.bindValue(":ultima_venta",this->ultima_venta);
    query.bindValue(":unidades_compradas",this->unidades_compradas);
    query.bindValue(":unidades_vendidas",this->unidades_vendidas);
    query.bindValue(":acumulado_compras",this->acumulado_compras);
    query.bindValue(":acumulado_ventas",this->acumulado_ventas);
    query.bindValue(":comentario",this->comentario);
    query.bindValue(":stock_maximo",this->stock_maximo);
    query.bindValue(":stock_minimo",this->stock_minimo);
    query.bindValue(":stock_real",this->stock_real);
    query.bindValue(":stock_fisico_almacen",this->nstock_fisico_almacen);
    query.bindValue(":controlar_stock",this->controlar_stock);
    query.bindValue(":composicion",this->composicion);
    query.bindValue(":pvp_incluye_iva",this->pvp_incluye_iva);
    query.bindValue(":fecha_prevista_recepcion",fecha_prevista_recepcion);
    query.bindValue(":cantidad_pendiente_recibir",this->cantidad_pendiente_recibir);
    query.bindValue(":reservados",this->reservados);
    query.bindValue(":mostrar_web",this->mostrar_web);
    query.bindValue(":etiquetas",this->etiquetas);
    query.bindValue(":localizacion_en_almacen",this->localizacion_en_almacen);
    query.bindValue(":id",this->id);
    query.bindValue(":id_tipos_iva",this->id_tipos_iva);
    query.bindValue(":id_subsub_familia",this->id_subsub_familia);
    query.bindValue(":id_grupo_art",this->id_grupo_art);
    query.bindValue(":id_web",this->id_web);
    query.bindValue(":stock_fisico_almacen",this->nstock_fisico_almacen);
    query.bindValue(":coste",this->coste);
    query.bindValue(":articulo_promocionado",this->articulo_promocionado);
    query.bindValue(":descripcion_promocion",this->descripcion_promocion);
    query.bindValue(":tipo_oferta",this->tipo_oferta);
    query.bindValue(":por_cada",this->por_cada);
    query.bindValue(":regalo_de",this->regalo_de);
    query.bindValue(":porc_dto_web",this->porc_dto_web);
    query.bindValue(":oferta_pvp_fijo",this->oferta_pvp_fijo);
    query.bindValue(":comentario_oferta",this->comentario_oferta);
    query.bindValue(":margen",this->margen);
    query.bindValue(":margen_min",this->margen_min);
    query.bindValue(":coste_real",this->coste_real);

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
    this->codigo = "";
    this->codigo_barras="";
    this->codigo_fabricante = "";
    this->descripcion = "";
    this->descripcion_reducida = "";
    this->id_proveedor = 0;
    this->id_familia = 0;
    this->familia = "";
    this->id_seccion = 0;
    this->seccion = "";
    this->id_subfamilia =0;
    this->subfamilia = "";
    this->tipo_iva = 0;
    this->coste = 0;
    this->dto = 0;
    this->ultima_compra = QDate::currentDate();
    this->ultima_venta = QDate::currentDate();
    this->unidades_compradas = 0;
    this->unidades_vendidas = 0;
    this->acumulado_compras = 0;
    this->acumulado_ventas = 0;
    //this->bImagen = registro.field(36).value().to....
    this->comentario = "";
    this->stock_maximo = 0;
    this->stock_minimo = 0;
    this->stock_real = 0;
    this->controlar_stock = 0;
    this->modelo = "";
    this->talla = "";
    this->color = "";
    this->composicion = "";
    this->pvp_incluye_iva = 0;
    this->fecha_prevista_recepcion = QDate::currentDate();
    this->cantidad_pendiente_recibir = 0;
    this->reservados = 0;
    this->mostrar_web = 0;
    this->etiquetas = 0;
    this->localizacion_en_almacen = "";
    this->articulo_promocionado = false;
    this->descripcion_promocion = "";
    this->tipo_oferta = 0;
    this->por_cada = 0;
    this->regalo_de = 0;
    this->porc_dto_web = 0;
    this->oferta_pvp_fijo = 0;
    this->comentario_oferta = "";
    this->margen = 0;
    this->margen_min = 0;
    this->nstock_fisico_almacen =0;
    this->coste_real = 0;

}

void Articulo::Borrar(int nid)
{
    if(QMessageBox::question(qApp->activeWindow(),qApp->tr("Borrar Artículo"),
                             qApp->tr("¿Desea realmente Borrar este artículo?\nEsta opción no se puede deshacer"),
                             qApp->tr("No"),qApp->tr("Si")) == QMessageBox::Accepted)
    {
        QSqlQuery qryArticulo(QSqlDatabase::database("Maya"));
        qryArticulo.prepare("Delete from articulos where id = :nid");
        // TODO - Borrar estadisticas y tarifas

        qryArticulo.bindValue(":id",nid);
        if(!qryArticulo.exec())
        {
            QMessageBox::critical(qApp->activeWindow(),QObject::tr("Borrar Artíclo"),QObject::tr("Falló el borrado del Artículo"),QObject::tr("&Aceptar"));
        }
        else
        {
            // Busco el id más proximo
            qryArticulo.prepare("select * from articulos where id <:nid");
            qryArticulo.bindValue(":nid",this->id);
            qryArticulo.exec();
            QSqlRecord registro = qryArticulo.record();
            this->id = registro.field("id").value().toInt();
        }
    }
}

void Articulo::Vender(int id, int cantidad, double pvp)
{
}

void Articulo::Devolucion(int id, int cantidad, double importe, QString cMotivo, QString fechaDevolucion)
{
}

void Articulo::CargarImagen(QLabel *label, QLabel *label2, QLabel *label3, QLabel *label4)
{
    QSqlQuery qryArticulo(QSqlDatabase::database("Maya"));
    qryArticulo.prepare("Select bImagen,bImagen2,bImagen3,bImagen4 from articulos where id = :id");
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

int Articulo::getidSeccion(QString seccion_)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select id from secciones where seccion = :cValor ");
    Query.bindValue(":cValor",seccion_);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Seccion"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("id").value().toInt();
}

int Articulo::getidFamilia(QString familia_)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select id from familias where familia = :cValor ");
    Query.bindValue(":cValor",familia_);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Familia"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("id").value().toInt();
}

int Articulo::getidSubFamilia(QString subfamilia_)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select id from subfamilias where sub_familia = :cValor ");
    Query.bindValue(":cValor",subfamilia_);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Sub-Familia"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("id").value().toInt();
}

int Articulo::getidSubSufFamilia(QString cSubSubFamilia_)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select id from subsubfamilias where subsub_familia = :cValor ");
    Query.bindValue(":cValor",cSubSubFamilia_);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Sub-Sub-Familia"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("id").value().toInt();
}

int Articulo::getidGrupo(QString cGrupo_)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select id from gruposart where grupo_art = :cValor ");
    Query.bindValue(":cValor",cGrupo_);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Grupo Artículo"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("id").value().toInt();
}

QString Articulo::getseccion(int nid)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select seccion from secciones where id = :cValor ");
    Query.bindValue(":cValor",nid);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Secciones"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("seccion").value().toString();
}
QString Articulo::getfamilia(int nid)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select familia from familias where id = :cValor ");
    Query.bindValue(":cValor",nid);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Familias"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("familia").value().toString();
}



QString Articulo::getsubfamilia(int nid)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select sub_familia from subfamilias where id = :cValor ");
    Query.bindValue(":cValor",nid);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Sub-Familias"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("subfamilia").value().toString();

}

QString Articulo::getcSubSubFamilia(int nid)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select subsub_familia from subsubfamilias where id = :cValor ");
    Query.bindValue(":cValor",nid);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Sub-sub-Familias"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("subsubfamilia").value().toString();
}

QString Articulo::getcGrupo(int nid)
{
    QSqlQuery Query(QSqlDatabase::database("Maya"));
    Query.prepare("select grupo_art from gruposart where id = :cValor ");
    Query.bindValue(":cValor",nid);
    if(!Query.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Buscar Grupo Artículo"),
                             tr("Ocurrió un error al localizar: %1").arg(Query.lastError().text()));
    } else
      Query.next();
    return Query.record().field("grupoart").value().toString();
}

bool Articulo::agregar_proveedor_alternativo(int id_art, int id_proveedor, QString codigo, double pvd, QString descoferta, QString oferta,
                                             double pvd_real,int id_divisa)
{
    QSqlQuery query_proveedor_alternativo(QSqlDatabase::database("Maya"));

    query_proveedor_alternativo.prepare("INSERT INTO articulos_prov_frec "
                                        "(id_articulo,id_proveedor,pvd,oferta,codigo,descoferta,pvd_real,id_divisa)"
                                        " VALUES (:id_articulo,:id_proveedor,:pvd, :oferta,:codigo,:descoferta,:pvd_real,:id_divisa)");
    query_proveedor_alternativo.bindValue(":id_articulo",id_art);
    query_proveedor_alternativo.bindValue(":id_proveedor",id_proveedor);
    query_proveedor_alternativo.bindValue(":codigo",codigo);
    query_proveedor_alternativo.bindValue(":pvd",pvd);
    query_proveedor_alternativo.bindValue(":oferta",oferta);
    query_proveedor_alternativo.bindValue(":descoferta",descoferta);
    query_proveedor_alternativo.bindValue(":pvd_real",pvd_real);
    query_proveedor_alternativo.bindValue(":id_divisa",id_divisa);
    if(!query_proveedor_alternativo.exec()) {
        QMessageBox::warning(qApp->activeWindow(),tr("Insertar proveedor frecuente"),
                             tr("Falló la inserción de un nuevo proveedor: %1").arg(query_proveedor_alternativo.lastError().text()));
        return false;


    } else
        return true;

}

bool Articulo::guardarProveedorAlternativo(int id, QString codigo, double pvd, QString descoferta, QString oferta,
                                           double pvd_real, int id_divisa)
{
    QSqlQuery query_proveedor_alternativo(QSqlDatabase::database("Maya"));

    query_proveedor_alternativo.prepare("UPDATE articulos_prov_frec  set pvd = :pvd,oferta = :oferta, codigo = :codigo, "
                                        "descoferta = :descoferta, pvd_real = :pvd_real, id_divisa =:id_divisa "
                                        "where id = :id");
    query_proveedor_alternativo.bindValue(":codigo",codigo);
    query_proveedor_alternativo.bindValue(":pvd",pvd);
    query_proveedor_alternativo.bindValue(":oferta",oferta);
    query_proveedor_alternativo.bindValue(":descoferta",descoferta);
    query_proveedor_alternativo.bindValue(":pvd_real",pvd_real);
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
    this->id_proveedor = id_proveedor;
    return true;


}

bool Articulo::cambiar_pvp()
{
    QSqlQuery tarifas(QSqlDatabase::database("Maya"));
    tarifas.prepare("select * from tarifas where id_articulo = :id");
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
            editTarifa.capturar_datos(tarifas.record().value("id").toInt(),QString::number(this->coste,'f',2));
//            if (Configuracion_global->divisa_local != cMoneda)
//                Configuracion_global->getCambio(Configuracion_global->cod_divisa_local,editTarifa->cod_divisa);
//            else
//                editTarifa. asignarcambiodivisa(1);

            editTarifa.calcular_precio(tarifas.record().value("margen").toDouble());
            QSqlQuery queryTarifas(QSqlDatabase::database("Maya"));
            queryTarifas.prepare(
            "UPDATE tarifas SET "
            "margen = :margen,"
            "margen_minimo = :margen_minimo,"
            "pvp = :pvp "
            " WHERE id = :id");
            queryTarifas.bindValue(":margen",editTarifa.margen);
            queryTarifas.bindValue(":margen_minimo",editTarifa.margen_min);
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

QString Articulo::auto_codigo()
{
    QString codigoIni;
    int tamano_codigoIni;
    codigoIni = this->seccion+this->familia+this->subfamilia+this->cSubSubFamilia+this->cGrupoArt;
    tamano_codigoIni = codigoIni.length();
    QSqlQuery queryArt("select codigo from articulos where codigo like '"+codigoIni+"%' order by codigo desc limit 1",
                       QSqlDatabase::database("Maya"));
    if(queryArt.exec()){
        queryArt.next();
        QString lastcode = queryArt.record().value("codigo").toString();
        int Realsize = lastcode.length();
        QString code = lastcode.mid(tamano_codigoIni,(Realsize-tamano_codigoIni));
        int icode = code.toInt();
        icode++;
        code = QString::number(icode);
        while (code.length()< ( Realsize - tamano_codigoIni) )
        {
            code.prepend("0");
        }
        return codigoIni + code;
    }

}

