#include "articulo.h"
#include "../Almacen/frmtarifas.h"

Articulo::Articulo(QObject *parent) : QObject(parent)
{

}

void Articulo::Anadir()
{
    QString error;
    QHash<QString,QVariant>data;
    data["codigo"] = QString("_%1_").arg(rand());
    int id = SqlCalls::SqlInsert(data,"articulos",Configuracion_global->groupDB,error);

    if(id < 0) {
        QMessageBox::warning(qApp->activeWindow(),tr("Añadir Artículo"),
                             tr("Falló la inserción de un nuevo artículo :\n %1").arg(error),
                             QObject::tr("Ok"));
    }
    else
    {
        this->id = id;
        //Recuperar("Select * from articulos where id = "+QString::number(this->id));
        codigo_anterior = data.value("codigo").toString();
        cod_seccion = "";
        cod_familia = "";
        cod_subfamilia = "";
        cod_SubSubFamilia = "";
        cod_GrupoArt = "";

        //--------------------------
        // Añado tarifas a artículo
        //--------------------------
        QSqlQuery queryTarifas(Configuracion_global->groupDB);
        if(queryTarifas.exec("select * from codigotarifa"))
        {
            while (queryTarifas.next())
            {
                QSqlRecord r = queryTarifas.record();
                QHash<QString,QVariant> _tar;
                _tar["id_articulo"]= this->id;
                _tar["id_pais"]=  r.value("id_pais");
                _tar["id_monedas"]= r.value("id_monedas");
                _tar["margen"]= Configuracion_global->margen;
                _tar["margen_minimo"]= Configuracion_global->margen_minimo;
                _tar["id_codigo_tarifa"]= r.value("id");

                if(SqlCalls::SqlInsert(_tar,"tarifas",Configuracion_global->groupDB,error) < 0)
                {
                    QMessageBox::warning(qApp->activeWindow(),tr("Añadir tarifa"),
                                         tr("Falló la inserción de la tarifa: %1").arg(error),
                                         tr("Aceptar"));
                }
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

bool Articulo::Recuperar(int id)
{
    QString error;
    QMap<int,QSqlRecord> _map = SqlCalls::SelectRecord("articulos",QString("id = %1 limit 1").arg(id),Configuracion_global->groupDB, error);
    if(_map.contains(id))
    {
        Cargar(_map.value(id));
        return true;
    }
    else if(error.isEmpty())
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("No se ha encontrado el artículo"));
    else
        QMessageBox::critical(qApp->activeWindow(),tr("Error al leer datos artículo:"), error);

    return false;
}

bool Articulo::Next()
{
    QString error;
    QMap<int,QSqlRecord> _map = SqlCalls::SelectRecord("articulos",QString("id > %1 limit 1").arg(id),Configuracion_global->groupDB, error);
    if(!_map.isEmpty())
    {
        Cargar(_map.first());
        return true;
    }
    else if(error.isEmpty())
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Se ha llegado al final del fichero"));
    else
        QMessageBox::critical(qApp->activeWindow(),tr("Error al leer datos artículo:"), error);

    return false;
}

bool Articulo::Prev()
{
    QString error;
    QMap<int,QSqlRecord> _map = SqlCalls::SelectRecord("articulos",QString("id < %1 order by id desc limit 1").arg(id),Configuracion_global->groupDB, error);
    if(!_map.isEmpty())
    {
        Cargar(_map.first());
        return true;
    }
    else if(error.isEmpty())
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Se ha llegado al inicio del fichero"));
    else
        QMessageBox::critical(qApp->activeWindow(),tr("Error al leer datos artículo:"), error);

    return false;
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
    this->idProveedor = registro.field("id_proveedor").value().toInt();
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
    this->id_subSubFamilia = registro.field("id_subsub_familia").value().toInt();
    this->id_grupoart = registro.field("id_grupo_art").value().toInt();
    this->id_web = registro.field("id_web").value().toInt();
    this->articulo_promocionado = registro.field("articulo_promocionado").value().toBool();
    this->margen = registro.field("margen").value().toDouble();
    this->margen_min = registro.field("margen_min").value().toDouble();
    this->coste_real = registro.field("coste_real").value().toDouble();
    this->coste_real_anterior = this->coste_real;
    // Recupero proveedor
    QSqlQuery *qryProveedor = new QSqlQuery(Configuracion_global->groupDB);
    qryProveedor->prepare("select id,codigo,proveedor from proveedores where id = :id");
    qryProveedor->bindValue(":id",this->idProveedor);
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
    if(this->codigo != this->codigo_anterior)
    {
        if (this->codigo == "auto_codigo")
        {
            int tamano =  Configuracion_global->tamano_codigo;
            QString aux = cod_seccion + cod_familia + cod_subfamilia + cod_SubSubFamilia + cod_GrupoArt;
            if (aux.length() + 3 > tamano)
            {
                aux = cod_familia + cod_subfamilia + cod_SubSubFamilia + cod_GrupoArt;
            }
            if (aux.length() + 3 > tamano)
            {
                aux = cod_subfamilia + cod_SubSubFamilia + cod_GrupoArt;
            }
            if (aux.length() + 3 > tamano)
            {
                aux = cod_SubSubFamilia + cod_GrupoArt;
            }
            if (aux.length() + 3 > tamano)
            {
                aux = cod_GrupoArt;
            }
            if (aux.length() + 3 > tamano)
            {
                aux.chop(3);
            }
            QString error;
            QStringList cods = SqlCalls::SelectList("articulos","codigo",
                                                    QString("codigo like '%1%' order by codigo desc").arg(aux),
                                                    Configuracion_global->groupDB,error);
            QString codigo_nuevo;
            QString formato = QString("%1.0f").arg(tamano-aux.length());
            formato.prepend("%0");
            std::string _x = formato.toStdString();

            if(cods.isEmpty())
            {
                codigo_nuevo.sprintf(_x.c_str(),1.0);
            }
            else
            {
                foreach (QString s, cods) {
                    QString codigo = s;
                    codigo.remove(aux);
                    if(codigo.at(0).isNumber())
                    {
                        double d_cod = codigo.toDouble();
                        d_cod += 1;
                        codigo_nuevo.sprintf(_x.c_str(),d_cod);
                        break;
                    }
                }
            }
            this->codigo = aux+codigo_nuevo;
        }
    }
    QHash <QString, QVariant> articulo;
    QString error;

    articulo["codigo"] = this->codigo;
    articulo["codigo_barras"] = this->codigo_barras;
    articulo["codigo_fabricante"] = this->codigo_fabricante;
    articulo["descripcion"] = this->descripcion;
    articulo["descripcion_reducida"] = this->descripcion_reducida;
    articulo["id_proveedor"] = this->idProveedor;
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
    articulo["id_subsub_familia"] = this->id_subSubFamilia;
    articulo["id_grupo_art"] = this->id_grupoart;
    articulo["id_web"] = this->id_web;
    articulo["stock_fisico_almacen"] = this->nstock_fisico_almacen;
    articulo["coste"] = this->coste;
    articulo["articulo_promocionado"] = this->articulo_promocionado;
    articulo["mostrar_en_cuadro"] = this->mostrar_en_cuadro;
    articulo["margen"] = this->margen;
    articulo["margen_min"] = this->margen_min;
    articulo["coste_real"] = this->coste_real;
    articulo["articulo_promocionado"] = this->articulo_promocionado;

    bool success = SqlCalls::SqlUpdate(articulo,"articulos",Configuracion_global->groupDB,
                                       QString("id = %1").arg(this->id),error);
    if(!success) {
        QMessageBox::warning(qApp->activeWindow(),QObject::tr("Guardar Artículo"),
                             QObject::tr("No se puede guardar el artículo ERROR: %1 ").arg(error),
                             QObject::tr("Ok"));

    }
    else
    {
        if(coste_real_anterior != coste_real)
        {
            QString error = "";
            QMap<int,QSqlRecord> _kits = SqlCalls::SelectRecord("kits",QString("id_componente = %1").arg(this->id),Configuracion_global->groupDB,error);
            if(!_kits.isEmpty() || !error.isEmpty())
            {
                if(!_kits.isEmpty())
                {
                    QMapIterator<int,QSqlRecord> _it(_kits);
                    QStringList _affected_kits;
                    while (_it.hasNext())
                    {
                        _it.next();
                        QSqlRecord r = _it.value();
                        _affected_kits.append(r.value("codigo_kit").toString());
                        double coste_anterior = r.value("coste_base").toDouble();
                        if(coste_anterior != coste_real)
                        {
                            int row_id = r.value("id").toInt();
                            double coste_final_anterior = r.value("cantidad").toDouble() * r.value("coste_final").toDouble();
                            double coste_final = coste_real * (1-(r.value("porc_dto").toDouble()/100));
                            QHash<QString,QVariant> _data;
                            _data["coste_base"] = coste_real;
                            _data["coste_final"] = coste_final;

                            if(SqlCalls::SqlUpdate(_data,"kits",Configuracion_global->groupDB,QString("id = %1").arg(row_id),error))
                            {
                                QMap<int,QSqlRecord> _articulo_kit = SqlCalls::SelectRecord("articulos",QString("codigo = '%1'").arg(r.value("codigo_kit").toString()),Configuracion_global->groupDB,error);
                                if(!_articulo_kit.isEmpty())
                                {
                                    QSqlRecord art_r = _articulo_kit.first();
                                    int art_id = art_r.value("id").toInt();
                                    double art_coste_real = art_r.value("coste_real").toDouble();
                                    art_coste_real -= coste_final_anterior;
                                    art_coste_real += coste_final;

                                    QHash<QString, QVariant> _art_data;
                                    _art_data["coste_real"] = art_coste_real;
                                    _art_data["coste"] = art_coste_real;

                                    if(!SqlCalls::SqlUpdate(_art_data,"articulos",Configuracion_global->groupDB,QString("id = %1").arg(art_id),error))
                                        QMessageBox::warning(qApp->activeWindow(),tr("Error actualizando conste de kit afectado %1").arg(r.value("codigo_kit").toString()),error);
                                }
                            }
                        }
                    }

                    QMessageBox::information(qApp->activeWindow(),tr("Kits afectados"),tr("Los cambios en este artículo afectaron a los siguientes kits:\n")
                                             +_affected_kits.join("\n")+"\nNo olvide revisar las tarífas correspondientes.");
                }
                else
                {
                    QMessageBox::critical(qApp->activeWindow(),tr("Error"),error);
                    return;
                }
            }
            coste_real_anterior = coste_real;
        }
        TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),
                                                  QObject::tr("Se ha guardado el artículo en la base de datos"));
    }

}

void Articulo::Vaciar()
{
    id_tipos_iva = 0;
    id_web = 0;
    this->id = 0;
    this->codigo = "";
    this->codigo_barras="";
    this->codigo_fabricante = "";
    this->descripcion = "";
    this->descripcion_reducida = "";
    this->kit = false;
    proveedor= "";
    cCodProveedor= "";
    this->idProveedor = 0;
    this->id_familia = 0;
    this->familia = "";
    cod_familia = "";
    this->id_seccion = 0;
    this->seccion = "";
    this->cod_seccion = "";
    this->id_subfamilia =0;
    this->subfamilia = "";
    this->cod_subfamilia = "";
    id_subSubFamilia=0;
    cSubSubFamilia="";
    cod_SubSubFamilia="";
    id_grupoart=0;
    cGrupoArt="";
    cod_GrupoArt="";
    codigo_iva = "";
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
    this->margen = 0;
    this->margen_min = 0;
    this->nstock_fisico_almacen =0;
    this->coste_real = 0;
    this->mostrar_en_cuadro = false;
    tipo_unidad = "";
    paquetes = 0;
    pvp = 0;
}

void Articulo::Borrar(int nid , bool isKit, bool ask, QString codigo)
{
    bool borrar = !ask;
    if(ask)
        borrar = QMessageBox::question(qApp->activeWindow(),qApp->tr("Borrar Artículo"),
                             qApp->tr("¿Desea realmente Borrar este artículo?\nEsta opción no se puede deshacer"),
                             qApp->tr("No"),qApp->tr("Si")) == QMessageBox::Accepted;
    if(borrar)
    {
        QString error = "";
        QStringList _kits = SqlCalls::SelectList("kits","codigo_kit",QString("id_componente = %1").arg(nid),Configuracion_global->groupDB,error);
        if(!_kits.isEmpty() || !error.isEmpty())
        {
            if(!_kits.isEmpty())
                QMessageBox::critical(qApp->activeWindow(),tr("Articulo parte de kit"),tr("Borre primero los siguientes kits:\n")+_kits.join("\n"));
            else
                QMessageBox::critical(qApp->activeWindow(),tr("Error"),error);
            return;
        }
        bool allOk = true;
        bool t = Configuracion_global->groupDB.transaction();

        allOk = SqlCalls::SqlDelete("tarifas",Configuracion_global->groupDB,QString("id_articulo = %1").arg(nid),error);

        if(!allOk)
        {
            QMessageBox::critical(qApp->activeWindow(),QObject::tr("Falló el borrado del Artículo (Tarifas)"),error,QObject::tr("&Aceptar"));
            if(t)
                Configuracion_global->groupDB.rollback();
            return;
        }

        if(isKit)
        {
            allOk = SqlCalls::SqlDelete("kits",Configuracion_global->groupDB,QString("codigo_kit = '%1'").arg(codigo),error);
            if(!allOk)
            {
                QMessageBox::critical(qApp->activeWindow(),QObject::tr("Falló el borrado del Artículo (Componentes del kit)"),error,QObject::tr("&Aceptar"));
                if(t)
                    Configuracion_global->groupDB.rollback();
                return;
            }
        }
        allOk = SqlCalls::SqlDelete("articulos",Configuracion_global->groupDB,QString("id = %1").arg(nid),error);
        if(!allOk)
        {
            QMessageBox::critical(qApp->activeWindow(),QObject::tr("Falló el borrado del Artículo (Registro de Articulo)"),error,QObject::tr("&Aceptar"));
            if(t)
                Configuracion_global->groupDB.rollback();
            return;
        }
        else        
        {
            if(t)
                allOk = Configuracion_global->groupDB.commit();

            if(!allOk)
            {
                QMessageBox::critical(qApp->activeWindow(),QObject::tr("Falló el borrado del Artículo (Commit)"),error,QObject::tr("&Aceptar"));
                if(t)
                    Configuracion_global->groupDB.rollback();
                return;
            }
            else
            {
                TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),tr("Articulo borrado con éxito"));
                if(!Next())
                    Prev();
            }
        }
    }
}

QHash<QString, QVariant> Articulo::Vender(QString codigo, int cantidad,int tarifa, int tipo_dto_tarifa,int id_familia_cliente,
                                          int id_cliente)
{
    //---------------------
    // BUSCAR CÓDIGO
    //---------------------
    QMap <int,QSqlRecord> m;
    QString error;
    float dto;
    QStringList condiciones;
    condiciones << QString("codigo = '%1'").arg(codigo).toUpper();
    condiciones << QString("tarifa = %1").arg(tarifa);

    m = SqlCalls::SelectRecord("vistaart_tarifa",condiciones,Configuracion_global->groupDB,
                               error);
    QMapIterator <int,QSqlRecord> i(m);
    QHash <QString, QVariant> h;
    h["found"] = false;
    while(i.hasNext())
    {
        i.next();
        h["found"] = true;
        h["id"] = i.value().value("id").toInt();
        h["codigo"] = i.value().value("codigo").toString();
        h["descripcion"] = i.value().value("descripcion").toString();
        h["descripcion_reducida"] = i.value().value("descripcion_reducida").toString();
        h["codigo_barras"] = i.value().value("codigo_barras").toString();
        h["codigo_fabricante"] = i.value().value("codigo_fabricante").toString();
        h["pvp_con_iva"] = i.value().value("pvp_con_iva").toDouble();
        h["precio"] = i.value().value("pvp").toDouble();
        h["cantidad"] = cantidad;
        h["tipo_iva"] = i.value().value("tipo_iva").toFloat();

        switch (tipo_dto_tarifa) {
        case 1:
            dto = i.value().value("porc_dto1").toFloat();
            break;
        case 2:
            dto = i.value().value("porc_dto2").toFloat();
            break;
        case 3:
            dto = i.value().value("porc_dto3").toFloat();
            break;
        case 4:
            dto = i.value().value("porc_dto4").toFloat();
            break;
        case 5:
            dto = i.value().value("porc_dto5").toFloat();
            break;
        case 6:
            dto = i.value().value("porc_dto6").toFloat();
            break;
        default:
            dto =0;
            break;


        }
        h["dto"] = dto;
        // PROMOCIONES ARTÍCULO
        if(this->articulo_promocionado)
        {
            QStringList filter;
            filter << QString("id=%1").arg(i.value().value("id").toInt());
            filter << "activa = 0";

            QMap <int,QSqlRecord> p;
            p=SqlCalls::SelectRecord("articulos_ofertas",filter, Configuracion_global->empresaDB,error);
            if(error.isEmpty())
            {
                //-------------
                // Oferta 3*2
                //-------------
                if(p.value(i.value().value("id").toInt()).value("oferta32").toBool())
                {
                    float por_cada = p.value(i.value().value("id").toInt()).value("unidades").toFloat();
                    float regalo = p.value(i.value().value("id").toInt()).value("regalo").toFloat();
                    int cant = cantidad / por_cada;
                    h["regalo"] = cant * regalo;
                    h["promocionado"] = true;
                }
                //--------------
                // Oferta web
                //--------------
                if(p.value(i.value().value("id").toInt()).value("oferta_web").toBool())
                {
                    float dto_web = p.value(i.value().value("id").toInt()).value("dto_web").toFloat();
                    if(dto_web > dto){
                        dto = dto_web;
                        h["promocionado"] = true;
                    }
                }
                //--------------
                // Oferta dto
                //--------------
                if(p.value(i.value().value("id").toInt()).value("oferta_dto").toBool())
                {
                    float dto_local = p.value(i.value().value("id").toInt()).value("dto_local").toFloat();
                    if(dto_local > dto){
                        dto = dto_local;
                        h["promocionado"] = true;
                    }
                }
                //----------------------
                // Oferta precio final
                //----------------------
                if(p.value(i.value().value("id").toInt()).value("oferta_precio_final").toBool())
                {
                    if(p.value(i.value().value("id").toInt()).value("precio_final").toDouble()<h.value("precio").toDouble()) {
                        h["precio"] = p.value(i.value().value("id").toInt()).value("precio_final").toDouble();
                        h["promocionado"] = true;
                    }
                }

            }

        }
        //--------------------------
        // Control de Execpciones
        //--------------------------

        QMap <int,QSqlRecord> e;
        // Coincice id_articulo e id_cliente
        QStringList filter;
        filter << QString("id_articulo= %1").arg(h.value("id").toInt());
        filter << QString("id_cliente = %1").arg(id_cliente);

        e = SqlCalls::SelectRecord("articulos_exepciones",filter,Configuracion_global->empresaDB,error);
        QMapIterator<int,QSqlRecord> ex(e);
        if(ex.hasNext()) {
            while (ex.hasNext()) {
                ex.next();
                if(ex.value().value("importe_fijo").toDouble()>0)
                    h["precio"] = ex.value().value("importe_fijo").toDouble();
                if(ex.value().value("dto_fijo").toFloat()>0){
                    if(ex.value().value("dto_fijo").toFloat()> h.value("dto").toFloat())
                           h["dto"] = ex.value().value("dto_fijo");
                }
                if(ex.value().value("importe_moneda_aumento").toDouble()>0)
                    h["precio"] = h.value("precio").toDouble() + ex.value().value("importe_moneda_aumento").toDouble();
                if(ex.value().value("dto_aumento_fijo").toFloat()>0)
                    h["dto"] = h.value("dto").toFloat() + ex.value().value("dto_aumento_fijo").toFloat();
                if(ex.value().value("importe_porc_aumento").toDouble()>0)
                    h["precio"] = h.value("precio").toDouble() +(h.value("precio").toDouble() *
                                                                 (ex.value().value("importe_porc_aumento").toDouble()/100));
                if(ex.value().value("dto_aumento_porc").toFloat()>0)
                    h["dto"] = h.value("dto").toFloat() + (h.value("dto").toFloat()*(
                                                               ex.value().value("dto_aumento_porc").toFloat()/100));
            }
        } else
        {
            // coincide id_familia e id_cliente y id_cliente > 0
            if(id_cliente >0)
            {
                QStringList filter;
                filter << QString("id_familia=%1").arg(this->id_familia);
                filter << QString("id_cliente=%1").arg(id_cliente);
                e = SqlCalls::SelectRecord("articulos_excepciones",filter,Configuracion_global->empresaDB,error);
                if(e.size()>0)
                {
                    QMapIterator<int,QSqlRecord> ae(e);
                    while (ae.hasNext())
                    {
                        ae.next();
                        if(ae.value().value("importe_fijo").toDouble()>0)
                            h["precio"] = ae.value().value("importe_fijo").toDouble();
                        if(ae.value().value("dto_fijo").toFloat()>0){
                            if(ae.value().value("dto_fijo").toFloat() > h.value("dto").toFloat())
                                h["dto"] = ae.value().value("dto_fijo");
                        }
                        if(ae.value().value("importe_moneda_aumento").toDouble()>0)
                            h["precio"] = h.value("precio").toDouble() + ae.value().value("importe_moneda_aumento").toDouble();
                        if(ae.value().value("dto_aumento_fijo").toFloat()>0)
                            h["dto"] = h.value("dto").toFloat() + ae.value().value("dto_aumento_fijo").toFloat();
                        if(ae.value().value("importe_porc_aumento").toDouble()>0)
                            h["precio"] = h.value("precio").toDouble() +(h.value("precio").toDouble() *
                                                                         (ae.value().value("importe_porc_aumento").toDouble()/100));
                        if(ae.value().value("dto_aumento_porc").toFloat()>0)
                            h["dto"] = h.value("dto").toFloat() + (h.value("dto").toFloat()*(
                                                                       ae.value().value("dto_aumento_porc").toFloat()/100));

                    }
                } // TODO - EXCEPCIONES AGENTES Y OTRAS EXCEPCIONES
            }
        }

    }
    // ----------------
    // ACTUALIZO STOCK
    //-----------------
    QString cSQL;
    QString fecha = QDate::currentDate().toString("yyyyMMdd");
    double importe = h.value("precio").toDouble() * cantidad;

    cSQL = QString("update articulos set fecha_ultima_venta = '%1', unidades_vendidas = unidades_vendidas +%2, "
                   "importe_acumulado_ventas = importe_acumulado_ventas + %3,stock_real = stock_real - %2,"
                   "stock_fisico_almacen = stock_fisico_almacen - %2 where id = %4").arg(fecha,QString::number(cantidad),
                                                                                         QString::number(importe,'f',2),
                                                                                         QString::number(h.value("id").toInt()));
    QSqlQuery art(Configuracion_global->groupDB);
    if(!art.exec(cSQL))
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Ventas"),tr("Ocurrió un error al actualizar acumulados:%1").arg(
                                 art.lastError().text()),tr("Aceptar"));
        h["isOk"] = false;
    } else
        h["isOk"] = true;

    //-----------------------------
    // ACUMULADOS ARTICULO-EMPRESA
    //-----------------------------

    if(h.value("isOk").toBool())
    {
        int id= h.value("id").toInt();
        int id_acum;
        QMap <int,QSqlRecord> aae;
        aae = SqlCalls::SelectRecord("acum_articulos",QString("id_articulo=%1").arg(id),
                                     Configuracion_global->empresaDB,error);
        // si no existe registro acumulados lo creamos
        if(aae.count()<=0){
            QHash <QString,QVariant> art;
            art["id_articulo"] = id;
            id_acum = SqlCalls::SqlInsert(art,"acum_articulos",Configuracion_global->empresaDB,error);
        } else
            id_acum = id;

        // actualizamos datos

        QString mes,imp;
        if(QDate::currentDate().month() ==1)
            mes =QString("unid_vent_enero = unid_vent_enero + %1").arg(cantidad);
        if(QDate::currentDate().month() ==2)
            mes =QString("unid_vent_febrero = unid_vent_febrero + %1").arg(cantidad);
        if(QDate::currentDate().month() ==3)
            mes =QString("unid_vent_marzo = unid_vent_marzo + %1").arg(cantidad);
        if(QDate::currentDate().month() ==4)
            mes =QString("unid_vent_abril = unid_vent_abril + %1").arg(cantidad);
        if(QDate::currentDate().month() ==5)
            mes =QString("unid_vent_mayo = unid_vent_mayo + %1").arg(cantidad);
        if(QDate::currentDate().month() ==6)
            mes =QString("unid_vent_junio = unid_vent_junio + %1").arg(cantidad);
        if(QDate::currentDate().month() ==7)
            mes =QString("unid_vent_julio = unid_vent_julio + %1").arg(cantidad);
        if(QDate::currentDate().month() ==8)
            mes =QString("unid_vent_agosto = unid_vent_agosto + %1").arg(cantidad);
        if(QDate::currentDate().month() ==9)
            mes =QString("unid_vent_septiembre = unid_vent_septiembre + %1").arg(cantidad);
        if(QDate::currentDate().month() ==10)
            mes =QString("unid_vent_octubre = unid_vent_octubre + %1").arg(cantidad);
        if(QDate::currentDate().month() ==11)
            mes =QString("unid_vent_noviembre = unid_vent_noviembre + %1").arg(cantidad);
        if(QDate::currentDate().month() ==12)
            mes =QString("unid_vent_diciembre = unid_vent_diciembre + %1").arg(cantidad);


        if(QDate::currentDate().month() ==1)
            imp =QString("acum_vent_enero = acum_vent_enero + %1").arg(importe);
        if(QDate::currentDate().month() ==2)
            imp =QString("acum_vent_febrero = acum_vent_febrero + %1").arg(importe);
        if(QDate::currentDate().month() ==3)
            imp =QString("acum_vent_marzo = acum_vent_marzo + %1").arg(importe);
        if(QDate::currentDate().month() ==4)
            imp =QString("acum_vent_abril = acum_vent_abril + %1").arg(importe);
        if(QDate::currentDate().month() ==5)
            imp =QString("acum_vent_mayo = acum_vent_mayo + %1").arg(importe);
        if(QDate::currentDate().month() ==6)
            imp =QString("acum_vent_junio = acum_vent_junio + %1").arg(importe);
        if(QDate::currentDate().month() ==7)
            imp =QString("acum_vent_julio = acum_vent_julio + %1").arg(importe);
        if(QDate::currentDate().month() ==8)
            imp =QString("acum_vent_agosto = acum_vent_agosto + %1").arg(importe);
        if(QDate::currentDate().month() ==9)
            imp =QString("acum_vent_septiembre = acum_vent_septiembre + %1").arg(importe);
        if(QDate::currentDate().month() ==10)
            imp =QString("acum_vent_octubre = acum_vent_octubre + %1").arg(importe);
        if(QDate::currentDate().month() ==11)
            imp =QString("acum_vent_noviembre = acum_vent_noviembre + %1").arg(importe);
        if(QDate::currentDate().month() ==12)
            imp =QString("acum_vent_diciembre = acum_vent_diciembre + %1").arg(importe);
        QSqlQuery acum(Configuracion_global->empresaDB);
        QString cSQL;
        cSQL = QString("update acum_articulos set %1,%2 where id_articulo = %3").arg(mes,imp,QString::number(id_acum));
        if(acum.exec(cSQL)){
            h["isOk"] = true;
        }
        else
        {
            h["isOk"] = false;
            QMessageBox::warning(qApp->activeWindow(),tr("Ventas"),
                                 tr("Ocurrió un error al guardar acumulados empresa").arg(acum.lastError().text()),
                                 tr("Aceptar"));
        }
        if(id_cliente >0)
        {
            //-------------------------
            // Acumulados de clientes
            //-------------------------
            if(QDate::currentDate().month() == 1)
                imp = QString("acum_enero = acum_enero +%1").arg(importe);
            if(QDate::currentDate().month() == 2)
                imp = QString("acum_febrero = acum_febrero +%1").arg(importe);
            if(QDate::currentDate().month() == 3)
                imp = QString("acum_marzo = acum_marzo +%1").arg(importe);
            if(QDate::currentDate().month() == 4)
                imp = QString("acum_abril = acum_abril +%1").arg(importe);
            if(QDate::currentDate().month() == 5)
                imp = QString("acum_mayo = acum_mayo +%1").arg(importe);
            if(QDate::currentDate().month() == 6)
                imp = QString("acum_junio = acum_junio +%1").arg(importe);
            if(QDate::currentDate().month() == 7)
                imp = QString("acum_julio = acum_julio +%1").arg(importe);
            if(QDate::currentDate().month() == 8)
                imp = QString("acum_agosto = acum_agosto +%1").arg(importe);
            if(QDate::currentDate().month() == 9)
                imp = QString("acum_septiembre = acum_septiembre +%1").arg(importe);
            if(QDate::currentDate().month() == 10)
                imp = QString("acum_octubre = acum_octubre +%1").arg(importe);
            if(QDate::currentDate().month() == 11)
                imp = QString("acum_noviembre = acum_noviembre +%1").arg(importe);
            if(QDate::currentDate().month() == 12)
                imp = QString("acum_diciembre = acum_diciembre +%1").arg(importe);

            QString ac = QString("acum_ejercicio = acum_ejercicio +%1").arg(importe);
            // -----------------
            // Acumulado ventas
            //------------------
            QMap <int,QSqlRecord> ac_cl;
            ac_cl = SqlCalls::SelectRecord("acum_clientes",QString("id_cliente =%1").arg(id_cliente),
                                           Configuracion_global->empresaDB,error);
            if(ac_cl.count()==0)
            {
                QHash < QString, QVariant> h;
                h["id_cliente"] = id_cliente;
                SqlCalls::SqlInsert(h,"acum_clientes",Configuracion_global->empresaDB,error);
            }
            cSQL = QString("update acum_clientes set %1,%2 where id_cliente = %3").arg(imp,ac,QString::number(id_cliente));
            if(!acum.exec(cSQL)){
                h["isOk"] = false;
            }
            //----------------------
            // campos ficha cliente
            //----------------------
            QString fecha = QDate::currentDate().toString("yyyyMMdd");
            cSQL = QString("update clientes set acumulado_ventas =  acumulado_ventas + %1,"
                           "ventas_ejercicio = ventas_ejercicio + %1,"
                           "fecha_ultima_compra = %2 where id = %3").arg(QString::number(importe),
                                                                         fecha,
                                                                         QString::number(id_cliente));
            QSqlQuery cliente(Configuracion_global->groupDB);
            if(!cliente.exec(cSQL))
                    h["isOk"] =false;



        }

    }
    return h;
}

bool Articulo::Devolucion(int id, double cantidad, double pvp, int id_cliente)
{
    bool isOk = false;
    QHash <QString, QVariant> h;
    QString fecha = QDate::currentDate().toString("yyyyMMdd");
    // datos ficha artículo
    QSqlQuery art(Configuracion_global->groupDB);
    QString cSQL;
    cSQL = QString("update articulos set fecha_ultima_venta = '%1', unidades_vendidas = unidades_vendidas -%2, "
                   "importe_acumulado_ventas = importe_acumulado_ventas - %3,stock_real = stock_real - %2,"
                   "stock_fisico_almacen = stock_fisico_almacen + %2 where id = %4").arg(fecha,QString::number(cantidad),
                                                                                         QString::number(pvp,'f',2),
                                                                                         QString::number(id));
    if(art.exec(cSQL))
    {
        isOk = true;
    } else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Devolución producto"),
                             tr("Ocurrióun error al devolver: %1").arg(art.lastError().text()),
                             tr("Aceptar"));
        isOk = false;
    }
    if(isOk){
        // datos ficha acumulados empresa.
        QString mes,imp;

        // unidades
        if(QDate::currentDate().month() ==1)
            mes =QString("unid_vent_enero = unid_vent_enero - %1").arg(cantidad);
        if(QDate::currentDate().month() ==2)
            mes =QString("unid_vent_febrero = unid_vent_febrero - %1").arg(cantidad);
        if(QDate::currentDate().month() ==3)
            mes =QString("unid_vent_marzo = unid_vent_marzo - %1").arg(cantidad);
        if(QDate::currentDate().month() ==4)
            mes =QString("unid_vent_abril = unid_vent_abril - %1").arg(cantidad);
        if(QDate::currentDate().month() ==5)
            mes =QString("unid_vent_mayo = unid_vent_mayo - %1").arg(cantidad);
        if(QDate::currentDate().month() ==6)
            mes =QString("unid_vent_junio = unid_vent_junio - %1").arg(cantidad);
        if(QDate::currentDate().month() ==7)
            mes =QString("unid_vent_julio = unid_vent_julio - %1").arg(cantidad);
        if(QDate::currentDate().month() ==8)
            mes =QString("unid_vent_agosto = unid_vent_agosto - %1").arg(cantidad);
        if(QDate::currentDate().month() ==9)
            mes =QString("unid_vent_septiembre = unid_vent_septiembre - %1").arg(cantidad);
        if(QDate::currentDate().month() ==10)
            mes =QString("unid_vent_octubre = unid_vent_octubre - %1").arg(cantidad);
        if(QDate::currentDate().month() ==11)
            mes =QString("unid_vent_noviembre = unid_vent_noviembre - %1").arg(cantidad);
        if(QDate::currentDate().month() ==12)
            mes =QString("unid_vent_diciembre = unid_vent_diciembre - %1").arg(cantidad);

        // ventas pvp
        if(QDate::currentDate().month() ==1)
            imp =QString("acum_vent_enero = acum_vent_enero - %1").arg(pvp);
        if(QDate::currentDate().month() ==2)
            imp =QString("acum_vent_febrero = acum_vent_febrero - %1").arg(pvp);
        if(QDate::currentDate().month() ==3)
            imp =QString("acum_vent_marzo = acum_vent_marzo - %1").arg(pvp);
        if(QDate::currentDate().month() ==4)
            imp =QString("acum_vent_abril = acum_vent_abril - %1").arg(pvp);
        if(QDate::currentDate().month() ==5)
            imp =QString("acum_vent_mayo = acum_vent_mayo - %1").arg(pvp);
        if(QDate::currentDate().month() ==6)
            imp =QString("acum_vent_junio = acum_vent_junio - %1").arg(pvp);
        if(QDate::currentDate().month() ==7)
            imp =QString("acum_vent_julio = acum_vent_julio - %1").arg(pvp);
        if(QDate::currentDate().month() ==8)
            imp =QString("acum_vent_agosto = acum_vent_agosto - %1").arg(pvp);
        if(QDate::currentDate().month() ==9)
            imp =QString("acum_vent_septiembre = acum_vent_septiembre - %1").arg(pvp);
        if(QDate::currentDate().month() ==10)
            imp =QString("acum_vent_octubre = acum_vent_octubre - %1").arg(pvp);
        if(QDate::currentDate().month() ==11)
            imp =QString("acum_vent_noviembre = acum_vent_noviembre - %1").arg(pvp);
        if(QDate::currentDate().month() ==12)
            imp =QString("acum_vent_diciembre = acum_vent_diciembre - %1").arg(pvp);
        QSqlQuery acum(Configuracion_global->empresaDB);
        QString cSQL;
        cSQL = QString("update acum_articulos set %1,%2 where id_articulo = %3").arg(mes,imp,QString::number(id));
        if(acum.exec(cSQL)){
           isOk = true;
        }
        else
        {
            isOk = false;
            QMessageBox::warning(qApp->activeWindow(),tr("Ventas"),
                                 tr("Ocurrió un error al guardar acumulados empresa").arg(acum.lastError().text()),
                                 tr("Aceptar"));
        }

    }
    return isOk;

}


void Articulo::CargarImagen(QLabel *label, QLabel *label2, QLabel *label3, QLabel *label4)
{
    QSqlQuery qryArticulo(Configuracion_global->groupDB);
    qryArticulo.prepare("Select imagen,imagen2,imagen3,imagen4 from articulos where id = :id");
    qryArticulo.bindValue(":id",this->id);
    if (qryArticulo.exec()) {
        if (qryArticulo.next()){
            //--------
            // imagen1
            //--------
            QSqlRecord registro =  qryArticulo.record();
            QByteArray ba1 = registro.field("imagen").value().toByteArray();
            QPixmap pm1;
            pm1.loadFromData(QByteArray::fromBase64(ba1));
            if(!registro.field("imagen").value().isNull());
                label->setPixmap(pm1);
            //--------
            // imagen2
            //--------
            ba1 = registro.field("imagen2").value().toByteArray();
            QPixmap pm12;
            pm12.loadFromData(ba1);
            if(!registro.field("imagen2").value().isNull())
                label2->setPixmap(pm12);
            //--------
            // imagen3
            //--------
            ba1 = registro.field("imagen3").value().toByteArray();
            QPixmap pm13;
            pm13.loadFromData(ba1);
            if(!registro.field("imagen3").value().isNull())
                label3->setPixmap(pm13);
            //--------
            // imagen4
            //--------
            ba1 = registro.field("imagen4").value().toByteArray();
            QPixmap pm14;
            pm14.loadFromData(ba1);
            if(!registro.field("imagen4").value().isNull())
                label4->setPixmap(pm14);
        } else
            QMessageBox::warning(qApp->activeWindow(),QObject::tr("Error al recuperar"),
                                    QObject::tr("No se pueden recuperar la imagenes asociadas al artículo"),
                                    QObject::tr("Ok"));
    }
}

bool Articulo::acumulado_ventas(int id_articulo, float cantidad,double total, QDate fecha)
{
    //--------------------------------
    // ACUMULADOS ARTICULO Y STOCK
    //--------------------------------
    bool success;
    QSqlQuery art(Configuracion_global->groupDB);
    QString cSQL = QString("update articulos set fecha_ultima_venta = '%1',stock_real = stock_real -%2,").arg(fecha.toString("yyyyMMdd"),QString::number(cantidad,'f',2));
    cSQL.append(QString("unidades_vendidas = unidades_vendidas -%3 where id= %4").arg(QString::number(cantidad,'f',2),
                                                                              QString::number(id_articulo)));

    if(!art.exec(cSQL)){
       success = false;
        QMessageBox::warning(qApp->activeWindow(),tr("Artículos"),
                             tr("No se ha podido guardar los acumulados: %1").arg(art.lastError().text()),
                             tr("Aceptar"));
    }
    cSQL = QString("update acum_articulos set ");
    if(fecha.month() == 1){
         cSQL.append(QString("acum_vent_enero = acum_vent_enero + %1").arg(total));
         cSQL.append(QString(",unid_vent_enero = unit_vent_enero +%1").arg(cantidad));
    }
    if(fecha.month() == 2){
        cSQL.append(QString("acum_vent_febrero = acum_vent_febrero + %1").arg(total));
        cSQL.append(QString(",unid_vent_febrero = unit_vent_febrero +%1").arg(cantidad));
   }
    if(fecha.month() == 3){
        cSQL.append(QString("acum_vent_marzo = acum_vent_marzo + %1").arg(total));
        cSQL.append(QString(",unid_vent_marzo = unit_vent_marzo +%1").arg(cantidad));
   }
    if(fecha.month() == 4){
        cSQL.append(QString("acum_vent_abril = acum_vent_abril + %1").arg(total));
        cSQL.append(QString(",unid_vent_abril = unit_vent_abril +%1").arg(cantidad));
   }
    if(fecha.month() == 5){
        cSQL.append(QString("acum_vent_mayo = acum_vent_mayo + %1").arg(total));
        cSQL.append(QString(",unid_vent_mayo = unit_vent_mayo +%1").arg(cantidad));
   }
    if(fecha.month() == 6){
        cSQL.append(QString("acum_vent_junio = acum_vent_junio + %1").arg(total));
        cSQL.append(QString(",unid_vent_junio = unit_vent_junio +%1").arg(cantidad));
   }
    if(fecha.month() == 7){
        cSQL.append(QString("acum_vent_julio = acum_vent_julio + %1").arg(total));
        cSQL.append(QString(",unid_vent_julio = unit_vent_julio +%1").arg(cantidad));
   }
    if(fecha.month() == 8){
        cSQL.append(QString("acum_vent_agosto = acum_vent_agosto + %1").arg(total));
    cSQL.append(QString(",unid_vent_agosto = unit_vent_agosto +%1").arg(cantidad));
    }
    if(fecha.month() == 9){
        cSQL.append(QString("acum_vent_septiembre = acum_vent_septiembre + %1").arg(total));
        cSQL.append(QString(",unid_vent_septiembre = unit_vent_septiembre +%1").arg(cantidad));
   }
    if(fecha.month() == 10){
        cSQL.append(QString("acum_vent_octubre = acum_vent_octubre + %1").arg(total));
        cSQL.append(QString(",unid_vent_octubre = unit_vent_octubre +%1").arg(cantidad));
   }
    if(fecha.month() == 11){
        cSQL.append(QString("acum_vent_noviembre = acum_vent_noviembre + %1").arg(total));
        cSQL.append(QString(",unid_vent_noviembre = unit_vent_noviembre +%1").arg(cantidad));
   }
    if(fecha.month() == 12){
        cSQL.append(QString("acum_vent_diciembre = acum_vent_diciembre + %1").arg(total));
        cSQL.append(QString(",unid_vent_diciembre = unit_vent_diciembre +%1").arg(cantidad));
   }
    cSQL.append(QString(" where id_articulo = %1").arg(id_articulo));
    QSqlQuery art_ac(Configuracion_global->empresaDB);

    if(!art_ac.exec(cSQL)){
        QMessageBox::warning(qApp->activeWindow(),tr("Artículos"),
                             tr("No se ha podido guardar los acumulados: %1").arg(art_ac.lastError().text()),
                             tr("Aceptar"));
        return false;
    }
    if (!success)
        return false;
    return true;




}

void Articulo::acumulado_compras(int id_articulo, float cantidad, QDate fecha)
{

}

void Articulo::acumulado_devoluciones(int id_articulo, float cantidad, QDate fecha)
{

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
    QHash<QString,QVariant> _data;

    _data["id_articulo"]=id_art;
    _data["id_proveedor"]=id_proveedor;
    _data["pvd"]=pvd;
    _data["oferta"]=oferta;
    _data["codigo"]=codigo;
    _data["descoferta"]=descoferta;
    _data["pvd_real"]=pvd_real;
    _data["id_divisa"]=id_divisa;

    QString error;

    if(!SqlCalls::SqlInsert(_data,"articulos_prov_frec",Configuracion_global->groupDB,error))
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Falló la inserción de un nuevo proveedor"),error);
        return false;
    }
    else
        return true;
}

bool Articulo::guardarProveedorAlternativo(int id, QString codigo, double pvd, QString descoferta, QString oferta,
                                           double pvd_real, int id_divisa)
{
    QHash<QString,QVariant> _data;
    _data["pvd"]= pvd;
    _data["oferta"]= oferta;
    _data["codigo"]= codigo;
    _data["descoferta"]= descoferta;
    _data["pvd_real"]= pvd_real;
    _data["id_divisa"]= id_divisa;

    QString error;
    if(!SqlCalls::SqlUpdate(_data,"articulos_prov_frec",Configuracion_global->groupDB,QString("id = %1").arg(id),error))
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Falló la modificación de un nuevo proveedor"),error);
        return false;
    }
    else
        return true;
}

bool Articulo::cambiarProveedorPrincipal(int id, int id_proveedor)
{
    QHash<QString,QVariant> _data;
    _data["id_proveedor"] = id_proveedor;

    QString error;
    if(!SqlCalls::SqlUpdate(_data,"articulos",Configuracion_global->groupDB,QString("id = %1").arg(id),error))
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Falló el cambio de proveedor principal"),error,tr("Aceptar"));
        return false;
    }
    else
    {
        this->idProveedor = id_proveedor;
        return true;
    }
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

bool Articulo::agregarStock(char accion, int id, int cantidad, double importe,QDate fecha)
{
    QSqlQuery art(Configuracion_global->groupDB);
    QString cSQL;
    QString error;
    bool updated = true;
    // ACTUALIZAR STOCK Y ACUMULADOS FICHA ARTICULO
    if(accion == 'v')
        cSQL = "update articulos set unidades_vendidas = unidades_vendidas - "+QString::number(cantidad,'f',2)+" "+
           "importe_acumulado_ventas = importe_acumulado_ventas - "+QString::number(importe,
                                                                                   'f',Configuracion_global->decimales_campos_totales)+
           " where id = "+QString::number(id);
    else
        cSQL = "update articulos set unidades_compradas = unidades_compradas - "+QString::number(cantidad,'f',2)+" "+
                "importe_acumulado_compras = importe_acumulado_compras - "+QString::number(importe,
                                                                                        'f',Configuracion_global->decimales_campos_totales)+
                " where id = "+QString::number(id);

    if(!art.exec(cSQL)){
       updated = false;
       QMessageBox::warning(qApp->activeWindow(),tr("Actualizar stock"),tr("No se pueden actualizar los acumulados de artículo"),tr("Aceptar"));
    }
    bool cont_stock = SqlCalls::SelectOneField("articulos","controlar_stock",
                                               QString("id=%1").arg(id),
                                               Configuracion_global->groupDB,error).toBool();
    if(error.isEmpty())

    {
        if(cont_stock)
        {
            if(accion == 'v')
                cSQL = "update articulos set stock_real = stock_real + "+QString::number(cantidad,'f',2)+
                    " stock_fisico_almacen = stock_fisico_almacen + " +QString::number(cantidad,'f',2) +
                    " where id = " + QString::number(id);
            else

                cSQL = "update articulos set stock_real = stock_real - "+QString::number(cantidad,'f',2)+
                    " stock_fisico_almacen = stock_fisico_almacen - "  +QString::number(cantidad,'f',2) +
                    " where id = " + QString::number(id);

            if(!art.exec(cSQL)){
                updated = false;
                QMessageBox::warning(qApp->activeWindow(),tr("Actualizar stock"),tr("No se puede actualizar el stock"),tr("Aceptar"));
            }
        }
    } else
        updated = false;

    // ACUMULADOS MENSUALES.
    QSqlQuery acumulados(Configuracion_global->empresaDB);
    int mes = fecha.month();
    float cantidadm = cantidad;
    double total =importe;
    switch (mes) {
    case 1:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_enero = unid_vent_enero - " + QString::number(cantidadm) +
                    " acum_vent_enero = acum_vent_enero -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_enero = unid_comp_enero - " + QString::number(cantidadm) +
                    " acum_comp_enero = acum_comp_enero -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;
    case 2:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_febrero = unid_vent_febrero - " + QString::number(cantidadm) +
                    " acum_vent_febrero = acum_vent_febrero -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_febrero = unid_comp_febrero - " + QString::number(cantidadm) +
                    " acum_comp_febrero = acum_comp_febrero -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;

    case 3:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_marzo = unid_vent_marzo - " + QString::number(cantidadm) +
                    " acum_vent_marzo = acum_vent_marzo -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_marzo = unid_comp_marzo - " + QString::number(cantidadm) +
                    " acum_comp_marzo = acum_comp_marzo -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;
    case 4:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_abril = unid_vent_abril - " + QString::number(cantidadm) +
                    " acum_vent_abril = acum_vent_abril -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_abril = unid_comp_abril - " + QString::number(cantidadm) +
                    " acum_comp_abril = acum_comp_abril -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;
    case 5:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_mayo = unid_vent_mayo - " + QString::number(cantidadm) +
                    " acum_vent_mayo = acum_vent_mayo -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_mayo = unid_comp_mayo - " + QString::number(cantidadm) +
                    " acum_comp_mayo = acum_comp_mayo -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;
    case 6:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_junio = unid_vent_junio - " + QString::number(cantidadm) +
                    " acum_vent_junio = acum_vent_junio -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_junio = unid_comp_junio - " + QString::number(cantidadm) +
                    " acum_comp_junio = acum_comp_junio -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;

    case 7:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_julio = unid_vent_julio - " + QString::number(cantidadm) +
                    " acum_vent_julio = acum_vent_julio -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_julio = unid_comp_julio - " + QString::number(cantidadm) +
                    " acum_comp_julio = acum_comp_julio -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;
    case 8:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_agosto = unid_vent_agosto - " + QString::number(cantidadm) +
                    " acum_vent_agosto = acum_vent_agosto -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_agosto = unid_comp_agosto - " + QString::number(cantidadm) +
                    " acum_comp_agosto = acum_comp_agosto -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;

    case 9:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_septiembre = unid_vent_septiembre - " + QString::number(cantidadm) +
                    " acum_vent_septiembre = acum_vent_septiembre -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_septiembre = unid_comp_septiembre - " + QString::number(cantidadm) +
                    " acum_comp_septiembre = acum_comp_septiembre -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;
    case 10:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_octubre = unid_vent_octubre - " + QString::number(cantidadm) +
                    " acum_vent_octubre = acum_vent_octubre -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_octubre = unid_comp_octubre - " + QString::number(cantidadm) +
                    " acum_comp_octubre = acum_comp_octubre -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;
    case 11:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_noviembre = unid_vent_noviembre - " + QString::number(cantidadm) +
                    " acum_vent_noviembre = acum_vent_noviembre -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_noviembre = unid_comp_noviembre - " + QString::number(cantidadm) +
                    " acum_comp_noviembre = acum_comp_noviembre -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;
    case 12:
        if(accion == 'v')
            cSQL = "update acum_articulos set unid_vent_diciembre = unid_vent_diciembre - " + QString::number(cantidadm) +
                    " acum_vent_diciembre = acum_vent_diciembre -"+ QString::number(total) +
                    " unid_vent_acumulado = unid_vent_acumulado - "+QString::number(cantidadm) +
                    " acum_vent_ejercicio = acum_vent_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        else
            cSQL = "update acum_articulos set unid_comp_diciembre = unid_comp_diciembre - " + QString::number(cantidadm) +
                    " acum_comp_diciembre = acum_comp_diciembre -"+ QString::number(total) +
                    " unid_comp_acumulado = unid_comp_acumulado - "+QString::number(cantidadm) +
                    " acum_comp_ejercicio = acum_comp_ejercicio - " +QString::number(total) +
                    " where id_articulo = " + QString::number(id);
        break;


    default:
        break;
    }
    if(!acumulados.exec(cSQL))
    {
        updated = false;
        QMessageBox::warning(qApp->activeWindow(),tr("Actualizar stock"),tr("Ocurrió un error  al actualizar los acumulados de  articulo/empresa"),
                             tr("Aceptar"));
    }

 return updated;

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
