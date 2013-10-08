#include "articulo.h"
#include "../Almacen/frmtarifas.h"

Articulo::Articulo(QObject *parent) : QObject(parent)
{

}

void Articulo::Anadir()
{
    QSqlQuery query(Configuracion_global->groupDB);
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
         QSqlQuery queryTarifas(Configuracion_global->groupDB);
         if(queryTarifas.exec("select * from codigotarifa")){
             while (queryTarifas.next()){
                 QSqlQuery anadirTarifa(Configuracion_global->groupDB);
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
    QSqlQuery qryArticulo(Configuracion_global->groupDB);
    qryArticulo.prepare(cSQL);
    if (qryArticulo.exec()) {
        if(qryArticulo.next()){
            QSqlRecord r = qryArticulo.record();
            Cargar(r);
            return true;
        }  else {
            TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"No se encuentra el artículo");
        }
    } else
    {
        QMessageBox::critical(qApp->activeWindow(),"error al leer datos artículo:", qryArticulo.lastError().text());
    }
    return false;
}

void Articulo::Recuperar(QString cSQL, int nProcede)
{
    QSqlQuery qryArticulo(Configuracion_global->groupDB);
    qryArticulo.prepare(cSQL);
    if (qryArticulo.exec()) {
        if(qryArticulo.next()){
            QSqlRecord r = qryArticulo.record();
            Cargar(r);

        }  else {
            if (nProcede ==1)
                TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"Se ha llegado al final del fichero");
            if (nProcede == 2)
                TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"Se ha llegado al inicio del fichero");
        }
    } else
    {
        QMessageBox::critical(qApp->activeWindow(),"error al leer datos artículo:", qryArticulo.lastError().text());
    }

}

void Articulo::Recuperar(int id)
{
    QSqlQuery qryArticulo(Configuracion_global->groupDB);
    qryArticulo.prepare("select * from articulos where id ="+QString::number(id));
    if (qryArticulo.exec()) {
        if(qryArticulo.next()){
            QSqlRecord r = qryArticulo.record();
            Cargar(r);
        }  else {
           TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"No se ha encontrado el artículo");

        }
    } else
    {
        QMessageBox::critical(qApp->activeWindow(),"error al leer datos artículo:", qryArticulo.lastError().text());
    }
}

void Articulo::Cargar(QSqlRecord registro)
{
    this->id = registro.field("id").value().toInt();
    this->kit = registro.value("kit").toBool();
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
    this->porc_dto = registro.field("porc_dto").value().toFloat();
    this->fecha_ultima_compra = registro.field("fecha_ultima_compra").value().toDate();
    this->fecha_ultima_venta = registro.field("fecha_ultima_venta").value().toDate();
    this->unidades_compradas = registro.field("unidades_compradas").value().toInt();
    this->unidades_vendidas = registro.field("unidades_vendidas").value().toInt();
    this->importe_acumulado_compras = registro.field("importe_acumulado_compras").value().toDouble();
    this->importe_acumulado_ventas = registro.field("importe_acumulado_ventas").value().toDouble();
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
    this->unidades_reservadas = registro.field("unidades_reservadas").value().toInt();
    this->mostrar_web = registro.field("mostrar_web").value().toInt();
    this->etiquetas = registro.field("etiquetas").value().toInt();
    this->paquetes = registro.field("paquetes").value().toInt();
    this->localizacion_en_almacen = registro.field("localizacion_en_almacen").value().toString();
    this->id_tipos_iva = registro.field("id_tipos_iva").value().toInt();
    this->id_subsub_familia = registro.field("id_subsub_familia").value().toInt();
    this->id_grupo_art = registro.field("id_grupo_art").value().toInt();
    this->id_web = registro.field("id_web").value().toInt();
    this->articulo_promocionado = registro.field("articulo_promocionado").value().toBool();
    this->margen = registro.field("margen").value().toDouble();
    this->margen_min = registro.field("margen_min").value().toDouble();
    this->coste_real = registro.field("coste_real").value().toDouble();

    // Recupero proveedor
    QSqlQuery *qryProveedor = new QSqlQuery(Configuracion_global->groupDB);
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

}

void Articulo::Guardar()
{
    // --------------------------------------------
    // Si está activada la autogeneración de código
    //---------------------------------------------

    if (this->codigo == "auto_codigo"){
        QString nuevo_codigo;
        QSqlQuery querycodigo(Configuracion_global->groupDB);

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
    QHash <QString, QVariant> articulo;
    QString error;

    articulo["codigo"] = this->codigo;
    articulo["codigo_barras"] = this->codigo_barras;
    articulo["codigo_fabricante"] = this->codigo_fabricante;
    articulo["descripcion"] = this->descripcion;
    articulo["descripcion_reducida"] = this->descripcion_reducida;
    articulo["id_proveedor"] = this->id_proveedor;
    articulo["id_familia"] = this->id_familia;
   // articulo["familia"] = this->familia;
    articulo["id_seccion"] = this->id_seccion;
   // articulo["seccion"] = this->seccion;
    articulo["id_subfamilia"] = this->id_subfamilia;
  //  articulo["subfamilia"] = this->subfamilia;
    articulo["tipo_iva"] = this->tipo_iva;
    articulo["porc_dto"] = this->porc_dto;
    articulo["fecha_ultima_compra"] = this->fecha_ultima_compra;
    articulo["fecha_ultima_venta"] = this->fecha_ultima_venta;
    articulo["unidades_compradas"] = this->unidades_compradas;
    articulo["unidades_vendidas"] = this->unidades_vendidas;
    articulo["importe_acumulado_compras"] = this->importe_acumulado_compras;
    articulo["importe_acumulado_ventas"] = this->importe_acumulado_ventas;
    articulo["comentario"] = this->comentario;
    articulo["stock_maximo"] = this->stock_maximo;
    articulo["stock_minimo"] = this->stock_minimo;
    articulo["stock_real"] = this->stock_real;
    articulo["stock_fisico_almacen"] = this->nstock_fisico_almacen;
    articulo["controlar_stock"] = this->controlar_stock;
   // articulo["composicion"] = this->composicion;
    articulo["pvp_incluye_iva"] = this->pvp_incluye_iva;
    articulo["fecha_prevista_recepcion"],fecha_prevista_recepcion;
    articulo["cantidad_pendiente_recibir"] = this->cantidad_pendiente_recibir;
    articulo["unidades_reservadas"] = this->unidades_reservadas;
    articulo["mostrar_web"] = this->mostrar_web;
    articulo["etiquetas"] = this->etiquetas;
    articulo["localizacion_en_almacen"] = this->localizacion_en_almacen;
    articulo["id"] = this->id;
    articulo["id_tipos_iva"] = this->id_tipos_iva;
    articulo["id_subsub_familia"] = this->id_subsub_familia;
    articulo["id_grupo_art"] = this->id_grupo_art;
    articulo["id_web"] = this->id_web;
    articulo["stock_fisico_almacen"] = this->nstock_fisico_almacen;
    articulo["coste"] = this->coste;
    articulo["articulo_promocionado"] = this->articulo_promocionado;
    articulo["mostrar_en_cuadro"] = this->mostrar_en_cuadro;
    articulo["margen"] = this->margen;
    articulo["margen_min"] = this->margen_min;
    articulo["coste_real"] = this->coste_real;

    bool success = SqlCalls::SqlUpdate(articulo,"articulos",Configuracion_global->groupDB,
                                       QString("id = %1").arg(this->id),error);
    if(!success) {
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Guardar Artículo"),
                             QObject::tr("No se puede guardar el artículo ERROR: %1 ").arg(error),
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
    this->porc_dto = 0;
    this->fecha_ultima_compra = QDate::currentDate();
    this->fecha_ultima_venta = QDate::currentDate();
    this->unidades_compradas = 0;
    this->unidades_vendidas = 0;
    this->importe_acumulado_compras = 0;
    this->importe_acumulado_ventas = 0;
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
    this->unidades_reservadas = 0;
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
    this->mostrar_en_cuadro = false;

}

void Articulo::Borrar(int nid)
{
    if(QMessageBox::question(qApp->activeWindow(),qApp->tr("Borrar Artículo"),
                             qApp->tr("¿Desea realmente Borrar este artículo?\nEsta opción no se puede deshacer"),
                             qApp->tr("No"),qApp->tr("Si")) == QMessageBox::Accepted)
    {
        QSqlQuery qryArticulo(Configuracion_global->groupDB);
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
    QSqlQuery qryArticulo(Configuracion_global->groupDB);
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
    QSqlQuery Query(Configuracion_global->groupDB);
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
    QSqlQuery Query(Configuracion_global->groupDB);
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
    QSqlQuery Query(Configuracion_global->groupDB);
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
    QSqlQuery Query(Configuracion_global->groupDB);
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
    QSqlQuery Query(Configuracion_global->groupDB);
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
    QSqlQuery Query(Configuracion_global->groupDB);
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
    QSqlQuery Query(Configuracion_global->groupDB);
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
    QSqlQuery Query(Configuracion_global->groupDB);
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
    QSqlQuery Query(Configuracion_global->groupDB);
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
    QSqlQuery Query(Configuracion_global->groupDB);
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
    QSqlQuery query_proveedor_alternativo(Configuracion_global->groupDB);

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
    QSqlQuery query_proveedor_alternativo(Configuracion_global->groupDB);

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
    QSqlQuery queryProveedor(Configuracion_global->groupDB);
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
    QSqlQuery tarifas(Configuracion_global->groupDB);
    tarifas.prepare("select * from tarifas where id_articulo = :id");
    tarifas.bindValue(":id",this->id);

    if (tarifas.exec()){
        FrmTarifas editTarifa(qApp->activeWindow());
        while (tarifas.next())
        {
//            QSqlQuery qTarifa(Configuracion_global->groupDB);
//            qTarifa.prepare("select * from codigotarifa where descripcion ='"+ this->ct+"'");

//            if(qTarifa.exec()){
//                qTarifa.next();

//                QString cMoneda = Configuracion_global->Devolver_moneda(qTarifa.record().field("id_monedas").value().toInt());
            editTarifa.capturar_datos(tarifas.record().value("id").toInt(),QString::number(this->coste,'f',Configuracion_global->decimales));
//            if (Configuracion_global->divisa_local != cMoneda)
//                Configuracion_global->getCambio(Configuracion_global->cod_divisa_local,editTarifa->cod_divisa);
//            else
//                editTarifa. asignarcambiodivisa(1);

            editTarifa.calcular_precio(tarifas.record().value("margen").toDouble());
            QSqlQuery queryTarifas(Configuracion_global->groupDB);
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
                       Configuracion_global->groupDB);
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

float Articulo::asigna_dto_linea(int id_art, int id_cliente, float dto_esp,float dto_lin)
{
    float dto = 0,dto_cliente = 0,dto_articulo =0,dto_cliente_articulo = 0,dto_cliente_familia =0;
    float dto_grupo_cli_art = 0,dto_grupocli_familia =0;
    int dto_tarifa =0,tarifa_cliente =0;
    //---------------------
    // Busco dto cliente
    //---------------------
    QMap <int,QSqlRecord> map;
    QString error;
    map = SqlCalls::SelectRecord("clientes",QString("id = %1").arg(id_cliente),Configuracion_global->groupDB,error);
    if(error.isEmpty())
    {
        QMapIterator <int,QSqlRecord> i(map);
        while(i.hasNext())
        {
            i.next();
            dto_cliente = i.value().value("porc_dto_cliente").toFloat();
            dto_tarifa = i.value().value("tipo_dto_tarifa").toInt();
            tarifa_cliente = i.value().value("tarifa_cliente").toInt();
        }
     } else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Asignación de descuentos"),
                             tr("No se pudo recuperar los valores de descuento en ficha cliente: Error %1 ").arg(error),
                             tr("Aceptar"));
    }
    QStringList conditions;

    // Desactivado porque el precio ya es devuelto con ese descuento aplicado por el helped_table
//    //----------------------
//    // Busco dto artículo
//    //----------------------

//    conditions <<QString("id = %1").arg(id_art) << QString("tarifa = %1").arg(tarifa_cliente);
//    map = SqlCalls::SelectRecord("vistaart_tarifa",conditions,Configuracion_global->groupDB,error);
//    if(error.isEmpty())
//    {
//        QMapIterator <int,QSqlRecord> i(map);
//        while(i.hasNext())
//        {
//            i.next();
//            switch (dto_tarifa) {
//            case 1:
//                dto_articulo = i.value().value("porc_dto1").toFloat();
//                break;
//            case 2:
//                dto_articulo = i.value().value("porc_dto2").toFloat();
//                break;
//            case 3:
//                dto_articulo = i.value().value("porc_dto3").toFloat();
//                break;
//            case 4:
//                dto_articulo = i.value().value("porc_dto4").toFloat();
//                break;
//            case 5:
//                dto_articulo = i.value().value("porc_dto5").toFloat();
//                break;
//            case 6:
//                dto_articulo = i.value().value("porc_dto6").toFloat();
//                break;

//            default:
//                break;
//            }
//        }
//     } else
//    {
//        QMessageBox::warning(qApp->activeWindow(),tr("Asignación de descuentos"),
//                             tr("No se pudo recuperar los valores de descuento en ficha cliente: Error %1 ").arg(error),
//                             tr("Aceptar"));
//    }

    //--------------------------------------------
    // Compruebo excepciones por cliente-articulo
    //--------------------------------------------
    conditions.clear();
    conditions <<QString("id_articulo = %1").arg(id_art) << QString("id_cliente = %1").arg(id_cliente);
    map = SqlCalls::SelectRecord("articulos_circumstancias",conditions,Configuracion_global->groupDB,error);
    float dto_fijo,dto_aumento_porc,dto_aumento_fijo;
    if(error.isEmpty())
    {
        QMapIterator <int,QSqlRecord> i(map);
        while(i.hasNext())
        {
            i.next();
            dto_fijo = i.value().value("dto_fijo").toFloat();
            dto_aumento_porc = i.value().value("dto_aumento_porc").toFloat();
            dto_aumento_fijo = i.value().value("dto_aumento_porc").toFloat();
        }
        if(dto_fijo >0)
            dto_cliente_articulo = dto_fijo;
        if(dto_aumento_porc >0)
        {
            if(dto_articulo > dto_cliente)
                dto_cliente_articulo = dto_articulo + dto_articulo *(dto_aumento_porc/100.0);
            else
                dto_cliente_articulo = dto_cliente + dto_cliente *(dto_aumento_porc/100.0);
        }
        if (dto_aumento_fijo >0)
        {
            if(dto_articulo > dto_cliente)
                dto_cliente_articulo += dto_aumento_fijo;
            else
                dto_cliente_articulo += dto_aumento_fijo;
        }

    }


    //----------------------------------------------------
    // Compruebo excepciones por cliente-familia_articulo
    //----------------------------------------------------
    int id_familia = SqlCalls::SelectOneField("articulos","id_familia",QString("id = %1").arg(id_art),
                                              Configuracion_global->groupDB,error).toInt();
    conditions.clear();
    conditions <<QString("id_familia = %1").arg(id_familia) << QString("id_cliente = %1").arg(id_cliente);
    map = SqlCalls::SelectRecord("articulos_circumstancias",conditions,Configuracion_global->groupDB,error);
    if(error.isEmpty())
    {
        QMapIterator <int,QSqlRecord> i(map);
        while(i.hasNext())
        {
            i.next();
            dto_fijo = i.value().value("dto_fijo").toFloat();
            dto_aumento_porc = i.value().value("dto_aumento_porc").toFloat();
            dto_aumento_fijo = i.value().value("dto_aumento_porc").toFloat();
        }
        if(dto_fijo >0)
            dto_cliente_articulo = dto_fijo;
        if(dto_aumento_porc >0)
        {
            if(dto_articulo > dto_cliente)
                dto_cliente_familia = dto_articulo + dto_articulo *(dto_aumento_porc/100.0);
            else
                dto_cliente_familia = dto_cliente + dto_cliente *(dto_aumento_porc/100.0);
        }
        if (dto_aumento_fijo >0)
        {
            if(dto_articulo > dto_cliente)
                dto_cliente_familia += dto_aumento_fijo;
            else
                dto_cliente_familia += dto_aumento_fijo;
        }

    }

    //----------------------------------------------------
    // Compruebo excepciones por grupo_cliente-articulo
    //----------------------------------------------------
    QMap <int,QSqlRecord> map_grupos;
    map_grupos = SqlCalls::SelectRecord("tipocliente",QString("id_cliente = %1").arg(id_cliente),
                                        Configuracion_global->groupDB,error);

    QMapIterator <int, QSqlRecord> i_grupos(map_grupos);
    int id_grupo,id_subgrupo;
    while (i_grupos.hasNext())
    {
        i_grupos.next();
        id_grupo = i_grupos.value().value("id_tipo_cliente").toInt();
        id_subgrupo = i_grupos.value().value("id_subtipo_cliente").toInt();

        conditions <<QString("id_articulo = %1").arg(id_art) << QString("id_familia_cliente = %1").arg(id_grupo);
        conditions << QString("id_subfamilia_cliente = %1").arg(id_subgrupo);

        map = SqlCalls::SelectRecord("articulos_circumstancias",conditions,Configuracion_global->groupDB,error);
        if(error.isEmpty())
        {
            QMapIterator <int,QSqlRecord> i(map);
            while(i.hasNext())
            {
                i.next();
                dto_fijo = i.value().value("dto_fijo").toFloat();
                dto_aumento_porc = i.value().value("dto_aumento_porc").toFloat();
                dto_aumento_fijo = i.value().value("dto_aumento_porc").toFloat();
            }
            if(dto_fijo >0)
                dto_grupo_cli_art = dto_fijo;
            if(dto_aumento_porc >0)
            {
                if(dto_articulo > dto_cliente)
                    dto_grupo_cli_art = dto_articulo + dto_articulo *(dto_aumento_porc/100.0);
                else
                    dto_grupo_cli_art = dto_cliente + dto_cliente *(dto_aumento_porc/100.0);
            }
            if (dto_aumento_fijo >0)
            {
                if(dto_articulo > dto_cliente)
                    dto_grupo_cli_art += dto_aumento_fijo;
                else
                    dto_grupo_cli_art += dto_aumento_fijo;
            }

        }
    }

    //----------------------------------------------------
    // Compruebo excepciones por grupo_cliente-familia_articulo
    //----------------------------------------------------
    map_grupos.clear();
    map_grupos = SqlCalls::SelectRecord("tipocliente",QString("id_cliente = %1").arg(id_cliente),
                                        Configuracion_global->groupDB,error);

    QMapIterator <int, QSqlRecord> i_grupos2(map_grupos);
    id_grupo = 0;
    id_subgrupo = 0;
    while (i_grupos2.hasNext())
    {
        i_grupos2.next();
        id_grupo = i_grupos2.value().value("id_tipo_cliente").toInt();
        id_subgrupo = i_grupos2.value().value("id_subtipo_cliente").toInt();

        conditions <<QString("id_familia = %1").arg(id_familia) << QString("id_familia_cliente = %1").arg(id_grupo);
        conditions << QString("id_subfamilia_cliente = %1").arg(id_subgrupo);

        map = SqlCalls::SelectRecord("articulos_circumstancias",conditions,Configuracion_global->groupDB,error);
        if(error.isEmpty())
        {
            QMapIterator <int,QSqlRecord> i(map);
            while(i.hasNext())
            {
                i.next();
                dto_fijo = i.value().value("dto_fijo").toFloat();
                dto_aumento_porc = i.value().value("dto_aumento_porc").toFloat();
                dto_aumento_fijo = i.value().value("dto_aumento_porc").toFloat();
            }
            if(dto_fijo >0)
                dto_grupocli_familia = dto_fijo;
            if(dto_aumento_porc >0)
            {
                if(dto_articulo > dto_cliente)
                    dto_grupocli_familia = dto_articulo + dto_articulo *(dto_aumento_porc/100.0);
                else
                    dto_grupocli_familia = dto_cliente + dto_cliente *(dto_aumento_porc/100.0);
            }
            if (dto_aumento_fijo >0)
            {
                if(dto_articulo > dto_cliente)
                    dto_grupocli_familia += dto_aumento_fijo;
                else
                    dto_grupocli_familia+= dto_aumento_fijo;
            }

        }
    }


    //--------------------------
    // Asigno Dto mayor posible
    //--------------------------
    dto = dto_esp;
    if(dto < dto_cliente)
        dto = dto_cliente;
    if(dto_esp < dto_articulo)
        dto = dto_articulo;
    if(dto < dto_cliente_articulo)
        dto = dto_cliente_articulo;
    if(dto < dto_cliente_familia)
        dto = dto_cliente_familia;
    if(dto < dto_grupo_cli_art)
        dto = dto_grupo_cli_art;
    if(dto < dto_grupocli_familia)
        dto = dto_grupocli_familia;
    if (dto_lin > dto)
        dto = dto_lin;
    return dto;
}
