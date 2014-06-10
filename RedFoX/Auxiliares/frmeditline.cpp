#include "frmeditline.h"
#include "ui_frmeditline.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Almacen/frmselectlotes.h"

void frmEditLine::init()
{    
    this->editando = false;
    this->reserva_unidades = false;
    add_pendientes = false;
    use_re = false;
    ui->lblpromocionado->setVisible(false);
    ui->chk3_2->setVisible(false);
    ui->chkdto->setVisible(false);
    ui->chkprecio->setVisible(false);
    ui->chk_dtoweb->setVisible(false);

    //IVA desde config
    ui->cboIva->setModel(Configuracion_global->iva_model);
    ui->cboIva->setModelColumn(4);
    int index = ui->cboIva->findText("21");
    ui->cboIva->setCurrentIndex(index);

    //-------------
    // Eventos
    //-------------
    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;

    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);
}

frmEditLine::frmEditLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmEditLine)
{
    ui->setupUi(this);    
    _tipoDoc = Presupuesto;
}

frmEditLine::~frmEditLine()
{
    delete ui;
}

void frmEditLine::setTipoDoc(frmEditLine::tipoDoc doc)
{
    _tipoDoc = doc;
}

bool frmEditLine::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if(keyEvent->key() == Qt::Key_B &&  keyEvent->modifiers() == Qt::ControlModifier)
        {
            ui->txtCodigo->blockSignals(true);
            buscar_art();
            ui->txtCodigo->blockSignals(false);
            return true;
        }
        if(keyEvent->key() == Qt::Key_Plus)
        {
            on_btnAnadir_mas_nueva_clicked();
            return true;
        }
        if(keyEvent->key() == Qt::Key_End)
        {
            on_txtCantidad_editingFinished();
            on_btnAceptar_clicked();
            return true;
        }
        if(keyEvent->key() == Qt::Key_Escape)
            return true;
    }
    return QDialog::eventFilter(obj,event);
}

void frmEditLine::buscar_art()
{
    db_consulta_view consulta(qApp->activeWindow());
    QStringList campos;
    campos << "descripcion" <<"codigo" <<"codigo_barras" << "codigo_fabricante"  << "coste";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("articulos");
    consulta.set_db("Maya");
    consulta.setId_tarifa_cliente(this->id_tarifa);
    if(is_venta)
    {
        consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion_reducida,coste,pvp, pvp_cliente as pvp_recom,stock_fisico_almacen"
                         " from vistaart_tarifa");
        QStringList cabecera;
        QVariantList tamanos;
        QVariantList moneda;
        cabecera  <<tr("id")<< tr("Código") << tr("Código Barras") << tr("Referencia") << tr("Descripción") << tr("Coste") <<tr("pvp");
        cabecera <<tr("P.RECOM.") <<tr("S.Alm.");
        tamanos <<"0" << "80" << "80" << "80" << "250" <<"100" <<"100" <<"100" <<"80";
        moneda <<"5" <<"6" <<"7" <<"8";
        consulta.set_headers(cabecera);
        consulta.set_tamano_columnas(tamanos);
        consulta.set_delegate_monetary(moneda);
        consulta.set_filtro("");
    }
    else
    {
        QString sql = QString("select id_articulo,codigo,codigo_barras,codigo_fabricante,descripcion_reducida,coste,"
                              "stock_fisico_almacen from vista_art_prov where id_proveedor = %1").arg(id_prov);
        consulta.set_SQL(sql);
        QStringList cabecera;
        QVariantList tamanos;
        QVariantList moneda;
        cabecera  <<tr("id")<< tr("Código") << tr("Código Barras") << tr("Referencia") << tr("Descripción") << tr("Coste") <<tr("Stock");
        tamanos <<"0" << "120" << "120" << "120" << "380" <<"80" <<"100";
        moneda <<"5" <<"6" <<"7" <<"8";
        consulta.set_headers(cabecera);
        consulta.set_tamano_columnas(tamanos);
        consulta.set_delegate_monetary(moneda);
    }

    consulta.set_titulo("Busqueda de Artículos");    
    if(consulta.exec() == QDialog::Accepted)
    {
        this->id_articulo = consulta.get_id();
        if(id_articulo>0)
        {
            cargar_articulo(this->id_articulo,this->id_tarifa);
            ui->txtCantidad->setFocus();
        }
    }
}

QHash<QString, QVariant> frmEditLine::get_datos()
{
    QHash <QString, QVariant> lin;
    if(this->is_venta)
    {
        lin["cantidad"] = Configuracion_global->MonedatoDouble(ui->txtCantidad->text());
        lin["codigo"] = ui->txtCodigo->text();
        lin["descripcion"] = ui->txtDescripcion->text();
        lin["dto"] = Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text()) - Configuracion_global->MonedatoDouble(ui->txtPVP->text());
        lin["id_articulo"]  =this->id_articulo;
        lin["id_cab"] = this->id_cab;
        lin["porc_dto"] = Configuracion_global->MonedatoDouble(ui->txtPorc_dto->text());
        lin["porc_iva"] = ui->cboIva->currentText().toFloat();
        lin["cantidad"] = Configuracion_global->MonedatoDouble(ui->txtCantidad->text());
        lin["precio"] = Configuracion_global->MonedatoDouble(ui->txtPVP->text());
        lin["precio_recom"] = Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text());
        lin["promocion"] = ui->lblpromocionado->isVisible();
        lin["subtotal"] = Configuracion_global->MonedatoDouble(ui->txtCantidad->text()) * Configuracion_global->MonedatoDouble(ui->txtPVP->text());
        lin["total"] = Configuracion_global->MonedatoDouble(ui->txt_total_linea->text());
        lin["id_lote"] = this->id_lote;
        if(use_re)
        {
            lin["porc_rec"] = Configuracion_global->iva_model->record(ui->cboIva->currentIndex()).value("recargo_equivalencia");
            lin["rec"] = lin.value("total").toDouble() * (lin.value("porc_rec").toDouble()/100.0);
        }
    }
    else
    {
        lin["id_articulo"] = this->id_articulo;
        lin["id_cab"] = this->id_cab;
        lin["cantidad"] = Configuracion_global->MonedatoDouble(ui->txtCantidad->text());
        lin["codigo"] = ui->txtCodigo->text();
        lin["descripcion"] = ui->txtDescripcion->text();
        lin["porc_dto"] = Configuracion_global->MonedatoDouble(ui->txtPorc_dto->text());
        lin["precio"] = Configuracion_global->MonedatoDouble(ui->txtPVP->text());
        lin["coste_real_unidad"] = Configuracion_global->MonedatoDouble(ui->txtPVP->text());
        lin["subtotal"] = Configuracion_global->MonedatoDouble(ui->txtCantidad->text()) * Configuracion_global->MonedatoDouble(ui->txtPVP->text());
        lin["total"] =  Configuracion_global->MonedatoDouble(ui->txt_total_linea->text());
        lin["porc_iva"] = ui->cboIva->currentText().toFloat();
        lin["iva"] = lin.value("total").toDouble() * (lin.value("porc_iva").toDouble()/100);
        lin["coste_real_unidad"] = Configuracion_global->MonedatoDouble(ui->txtPVP->text());
        if(use_re)
        {
            lin["porc_rec"] = Configuracion_global->iva_model->record(ui->cboIva->currentIndex()).value("recargo_equivalencia");
            lin["rec"] = lin.value("total").toDouble() * (lin.value("porc_rec").toDouble()/100);
        }
        if(add_pendientes)
        {
            lin["cantidad_pendiente"] = lin["cantidad"];
        }
    }
    return lin;
}

void frmEditLine::_insert_nueva_linea()
{
    QHash<QString, QVariant> lin = get_datos();
    QString error;

    double cantidad = Configuracion_global->MonedatoDouble(ui->txtCantidad->text());

    this->id = SqlCalls::SqlInsert(lin,this->tabla,Configuracion_global->empresaDB,error);
    if(id < 0)
    {
        QMessageBox::critical(this,tr("Error al insertar linea"),error);
        return;
    }    
    do_stocks(cantidad);
}

void frmEditLine::do_stocks(double cantidad)
{
    if(this->is_venta)
    {
        switch (_tipoDoc)
        {
        case frmEditLine::Presupuesto:
            return;
            break;
        case frmEditLine::Pedido:
            Articulo::reservar(this->id_articulo,cantidad);
            break;
        case frmEditLine::Albaran:
        case frmEditLine::Factura:
            Articulo::agregar_stock_fisico(this->id_articulo, -1.0 * cantidad);
            if(this->id_lote > 0)
            {
                QString idlote = QString::number(this->id_lote);
                QSqlQuery lote(Configuracion_global->groupDB);
                if(!lote.exec(QString("update articulos_lotes set stock = stock - %1 where id = %2").arg(cantidad).arg(idlote)))
                {
                    QMessageBox::warning(this,tr("No se pudo actualizar el stock del lote"),lote.lastError().text());
                    close();
                    return;
                }
            }
            break;
        }
    }
    else //es compra
    {
        switch (_tipoDoc)
        {
        case frmEditLine::Presupuesto:
            break;
        case frmEditLine::Pedido:
            Articulo::set_pendiente_recibir(this->id_articulo,cantidad);
            break;
        case frmEditLine::Albaran:
        case frmEditLine::Factura:
            Articulo::agregar_stock_fisico(id_articulo,cantidad);
            break;
        }
    }
}

bool frmEditLine::getUse_re() const
{
    return use_re;
}

void frmEditLine::setUse_re(bool value)
{
    use_re = value;
}

int frmEditLine::get_id()
{
    return this->id;
}

void frmEditLine::set_linea(int id,QString fichero)
{
    this->id = id;
    //-----------------------------------------------
    // Recuperar línea de ventas del fichero **fichero
    //-----------------------------------------------
    QMap <int,QSqlRecord> m;
    QString error;
    m = SqlCalls::SelectRecord(fichero,QString("id=%1").arg(id),Configuracion_global->empresaDB,error);
    if(error.isEmpty())
    {
        this->id_cab = m.value(id).value("id_cab").toInt();
        this->id_articulo = m.value(id).value("id_articulo").toInt();
        this->id_lote = m.value(id).value("id_lote").toInt();
        ui->txtCodigo->setText(m.value(id).value("codigo").toString());
        this->codigo_articulo = ui->txtCodigo->text();
        ui->txtDescripcion->setText(m.value(id).value("descripcion").toString());
        ui->txtCantidad->setText(Configuracion_global->toFormatoMoneda( m.value(id).value("cantidad").toString()));
        float cant = m.value(id).value("cantidad").toFloat();
        anterior["cantidad"] = cant;
        ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(QString::number(m.value(id).value("precio").toDouble(),
                                                                                  'f',Configuracion_global->decimales)));
        ui->txtPorc_dto->setText(Configuracion_global->toFormatoMoneda(QString::number(m.value(id).value("porc_dto").toFloat(),
                                                                                       'f',Configuracion_global->decimales)));
        ui->txtPvp_recomendado->setText(Configuracion_global->toFormatoMoneda(
                                            QString::number(m.value(id).value("precio_recom").toDouble(),'f',
                                                            Configuracion_global->decimales)));
        ui->txt_total_linea->setText(Configuracion_global->toFormatoMoneda(QString::number(m.value(id).value("total").toDouble(),
                                                                                           'f',Configuracion_global->decimales)));
        anterior["total"] = Configuracion_global->MonedatoDouble(ui->txt_total_linea->text());
        int index = ui->cboIva->findText(m.value(id).value("porc_iva").toString());
        ui->cboIva->setCurrentIndex(index);

        QMap <int,QSqlRecord> a;
        QStringList condiciones;
        condiciones << QString("id = %1").arg(this->id_articulo);
        condiciones << QString("tarifa = %1").arg(this->id_tarifa);
        a = SqlCalls::SelectRecord("vistaart_tarifa",condiciones,Configuracion_global->groupDB,error);
        // STOCKS
        ui->txtStockAlmacen->setText(a.value(this->id_articulo).value("stock_fisico_almacen").toString());
        ui->txtStock_real->setText(a.value(this->id_articulo).value("stock_real").toString());
        ui->txtPendientes_recibir->setText(a.value(this->id_articulo).value("unidades_pendientes_recibir").toString());
        ui->txtUnidades_reservadas->setText(a.value(this->id_articulo).value("unidades_reservadas").toString());
        ui->dateFecha_recepcion->setDate(a.value(this->id_articulo).value("fecha_prevista_recepcion").toDate());
        ui->txtPvp_conIva->setText(Configuracion_global->toFormatoMoneda(
                                       QString::number( a.value(this->id_articulo).value("pvp_con_iva").toDouble(),'f',
                                                        Configuracion_global->decimales)));
    } else
    {
        QMessageBox::warning(this,tr("Edición de líneas"),
                             tr("Ocurrió un error al recupear la línea :%1").arg(error), tr("Aceptar"));
        close();
    }
}

void frmEditLine::set_id_cliente(int id)
{
    this->id_cliente = id;
}

void frmEditLine::set_id_tarifa(int id)
{
    this->id_tarifa = id;
    QString error;
    ui->txtTarifa->setText(SqlCalls::SelectOneField("codigotarifa","descripcion",QString("id=%1").arg(this->id_tarifa),
                                                    Configuracion_global->groupDB,error).toString());
    if(!error.isEmpty())
        QMessageBox::warning(this,tr("Edición de líneas"),tr("Ocurrió un error al localizar la tarifa: %1").arg(error));
}

void frmEditLine::set_venta(bool tipo)
{
    this->is_venta = tipo;
    if(tipo)
        ui->lbl_pvp_cliente->setText(tr("PVP\nCliente"));
    else
    {
        ui->lbl_pvp_cliente->setText(tr("Coste"));
        ui->label_tarifa->hide();
        ui->txtTarifa->hide();
    }
}

void frmEditLine::set_id_cab(int id_cabecera)
{
    this->id_cab = id_cabecera;
}

void frmEditLine::set_tabla(QString t)
{
    this->tabla = t;
}

void frmEditLine::set_editando()
{
    ui->txtCodigo->setEnabled(false);
    ui->btnAnadir_mas_nueva->setVisible(false);
    this->editando = true;
}

void frmEditLine::set_acumula(bool acum)
{
    this->realiza_acumulados = acum;
}

void frmEditLine::set_reserva(bool reserv)
{
    this->reserva_unidades = reserv;
}

void frmEditLine::on_txtCodigo_editingFinished()
{
    ui->txtCodigo->blockSignals(true);
    if(ui->txtCodigo->text() != this->codigo_articulo )
    {
        if(!ui->txtCodigo->text().isEmpty())
        {
            QString error;
            this->id_articulo = SqlCalls::SelectOneField("articulos","id",QString("codigo = '%1' or codigo_fabricante = '%1' or codigo_barras = '%1'").arg(ui->txtCodigo->text()),
                                                   Configuracion_global->groupDB,error).toInt();

            if(this->id_articulo <1)
            {
                QMessageBox::warning(this,tr("Edición de líneas"),tr("No se pudo recuperar el artículo: %1").arg(error),
                                     tr("Acceptar"));
                vaciar_campos();
                ui->txtCodigo->setFocus();
            }
            else
            {
                cargar_articulo(this->id_articulo,this->id_tarifa);
            }
        }          
    }
    ui->txtCodigo->blockSignals(false);
}

void frmEditLine::cargar_articulo(int id_art, int tarifa)
{
    ui->txtCodigo->blockSignals(true);
    // BUSCAR ARTICULO
    if(is_venta)
    {
        QString error;
        QStringList condiciones;
        condiciones << QString("id = '%1'").arg(id_art);
        condiciones << QString("tarifa = %1").arg(tarifa);

        QMap <int,QSqlRecord> m = SqlCalls::SelectRecord("vistaart_tarifa",condiciones,Configuracion_global->groupDB,error);
        if(!error.isEmpty())
        {
            QMessageBox::warning(this,tr("Edición de líneas"),tr("Ocurrió un error al recuperar: %1").arg(error));
            return;
        }
        QStringList filtro;
        filtro << QString("id_articulo = %1").arg(id_art);
        filtro << QString("id_tarifa = %1").arg(id_tarifa);
        filtro << QString("fecha_inicio <= %1 and fecha_fin >= %1").arg(QDate::currentDate().toString("yyyy-MM-dd"));
        QMap <int,QSqlRecord> ofertas = SqlCalls::SelectRecord("articulo_ofertas",filtro,Configuracion_global->empresaDB,error);

        if(!m.isEmpty())
        {
            QSqlRecord r = m.first();
            this->id_articulo = r.value("id").toInt();

            ui->txtCodigo->setText(r.value("codigo").toString());
            ui->txtDescripcion->setText(r.value("descripcion_reducida").toString());

            ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(QString::number(       r.value("pvp_cliente").toDouble(),'f',Configuracion_global->decimales_campos_totales)));
            ui->txtPvp_conIva->setText(Configuracion_global->toFormatoMoneda(QString::number(r.value("pvp_con_iva").toDouble(),'f',Configuracion_global->decimales_campos_totales)));
            ui->txtPvp_recomendado->setText(Configuracion_global->toFormatoMoneda(QString::number(   r.value("pvp").toDouble(),'f',Configuracion_global->decimales)));


            this->id_lote = 0;
            if (r.value("lotes").toBool())
            {
                frmSelectLotes frmlotes(this);
                frmlotes.set_id_articulo(id_art);
                if(frmlotes.exec() == QDialog::Accepted)
                {
                    QString lote = frmlotes.get_lote();
                    this->id_lote = frmlotes.get_id();
                    ui->txtDescripcion->setText(ui->txtDescripcion->text().append(lote));
                }
            }
            // Calculamos %dto tarifa
            float dto = r.value("pvp").toDouble() - r.value("pvp_cliente").toDouble();
            if(r.value("pvp_cliente").toDouble() > 0)
            {
                if(ui->txtPorc_dto->text() =="0,00" || ui->txtPorc_dto->text() =="0")
                {
                    float porc_dto = (dto * 100) / r.value("pvp").toDouble();
                    ui->txtPorc_dto->setText(Configuracion_global->toFormatoMoneda(QString::number(porc_dto,'f',Configuracion_global->decimales_campos_totales)));
                }
            }
            else //PVP_CLIENTE == 0
            {
                ui->txtPorc_dto->setText(Configuracion_global->toFormatoMoneda(QString::number(100.0,'f',Configuracion_global->decimales_campos_totales)));
            }

            ui->txtCantidad->setText("1");
            ui->cboIva->setCurrentIndex(ui->cboIva->findText(r.value("tipo_iva").toString()));

            // STOCKS
            ui->txtStockAlmacen->setText(r.value("stock_fisico_almacen").toString());
            ui->txtStock_real->setText(r.value("stock_real").toString());
            ui->txtPendientes_recibir->setText(r.value("unidades_pendientes_recibir").toString());
            ui->txtUnidades_reservadas->setText(r.value("unidades_reservadas").toString());
            ui->dateFecha_recepcion->setDate(r.value("fecha_prevista_recepcion").toDate());

            // PROMOCIONES ARTÍCULO
            if(r.value("articulo_promocionado").toBool())
            {
                ui->lblpromocionado->setVisible(true);
                ui->chk3_2->setVisible(true);
                ui->chkdto->setVisible(true);
                ui->chkprecio->setVisible(true);
                ui->chk_dtoweb->setVisible(true);

                QStringList filter;
                filter << QString("id=%1").arg(r.value("id").toInt());
                filter << "activa = 1";

                QMap <int,QSqlRecord> p = SqlCalls::SelectRecord("articulos_ofertas",filter, Configuracion_global->empresaDB,error);
                if(error.isEmpty() && !p.isEmpty())
                {
                    // Oferta 3*2
                    if(p.value(r.value("id").toInt()).value("oferta32").toBool())
                    {
                        ui->chk3_2->setChecked(true);
                        float por_cada = p.value(r.value("id").toInt()).value("unidades").toFloat();
                        float regalo = p.value(r.value("id").toInt()).value("regalo").toFloat();
                        int cant = cant / por_cada;
                        this->unidades_regalo = cant * regalo;
                    }

                    // Oferta web
                    if(p.value(r.value("id").toInt()).value("oferta_web").toBool())
                    {
                        ui->chk_dtoweb->setChecked(true);
                        float dto_web = p.value(r.value("id").toInt()).value("dto_web").toFloat();
                        if(dto_web > dto)
                            ui->txtPorc_dto->setText(QString::number(dto_web,'f',2));
                    }

                    // Oferta dto
                    if(p.value(r.value("id").toInt()).value("oferta_dto").toBool())
                    {
                        ui->chkdto->setChecked(true);
                        float dto_local = p.value(r.value("id").toInt()).value("dto_local").toFloat();
                        if(dto_local > dto)
                            ui->txtPorc_dto->setText(QString::number(dto_local,'f',2));
                    }

                    // Oferta precio final
                    if(p.value(r.value("id").toInt()).value("oferta_precio_final").toBool())
                    {
                        ui->chkprecio->setChecked(true);
                        if(p.value(r.value("id").toInt()).value("precio_final").toDouble()<r.value("precio").toDouble())
                            ui->txtPVP->setText(QString::number(p.value(r.value("id").toInt()).value("precio_final").toDouble(),'f',2));
                    }
                }
            }
            // TODO Control de Execpciones
        }
    }
    else // tipo == false -> COMPRAS
    {
        QString error;
        QMap <int,QSqlRecord> m = SqlCalls::SelectRecord("vista_art_prov",QString("id_articulo = '%1' and id_proveedor=%2")
                                                         .arg(id_art).arg(id_prov)
                                                         ,Configuracion_global->groupDB,error);
        if(!error.isEmpty())
        {
            QMessageBox::warning(this,tr("Edición de líneas"),tr("Ocurrió un error al recuperar: %1").arg(error));
            return;
        }

        if(!m.isEmpty())
        {
            QSqlRecord r = m.first();

            ui->txtCodigo->setText(r.value("codigo_proveedor").toString());

            ui->txtDescripcion->setText(r.value("descripcion_reducida").toString());

            ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(QString::number(            r.value("coste").toDouble(),'f',Configuracion_global->decimales_campos_totales)));
            ui->txtPvp_recomendado->setText(Configuracion_global->toFormatoMoneda(QString::number(r.value("coste").toDouble(),'f',Configuracion_global->decimales)));
            ui->txtPvp_conIva->setText("0,00");

            ui->txtCantidad->setText("1");
            int id_iva = r.value("id_tipos_iva").toInt();
            for(auto i= 0; i< Configuracion_global->iva_model->rowCount(); ++i)
            {
                if(Configuracion_global->iva_model->record(i).value("id").toInt() == id_iva)
                {
                    ui->cboIva->setCurrentIndex(i);
                    break;
                }
            }

            // STOCKS
            ui->txtStockAlmacen->setText(r.value("stock_fisico_almacen").toString());
            ui->txtStock_real->setText(r.value("stock_real").toString());
            ui->txtPendientes_recibir->setText(r.value("cantidad_pendiente_recibir").toString());
            ui->txtUnidades_reservadas->setText(r.value("unidades_reservadas").toString());
            ui->dateFecha_recepcion->setDate(r.value("fecha_prevista_recepcion").toDate());
        }
    }
    calcular();
    this->codigo_articulo = ui->txtCodigo->text();
    ui->txtCodigo->blockSignals(false);
}

void frmEditLine::vaciar_campos()
{
    ui->txtCantidad->setText("1");
    ui->txtCodigo->clear();
    ui->txtDescripcion->clear();
    ui->txtImporte_iva->setText("0,00");
    ui->txtPendientes_recibir->setText("0,00");
    ui->txtPorc_dto->setText("0");
    ui->txtPVP->setText("0,00");
    ui->txtPvp_conIva->setText("0,00");
    ui->txtPvp_recomendado->setText("0,00");
    ui->txtStockAlmacen->setText("0,00");
    ui->txtStock_real->setText("0,00");
    ui->txtTotal_con_iva->setText("0,00");
    ui->txtUnidades_reservadas->setText("0,00");
    ui->txt_total_linea->setText("0,00");
    this->id = 0;
    ui->txtCodigo->setFocus();
}

void frmEditLine::calcular()
{
    double pvp_rec,porc_dto,total;
    pvp_rec = Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text());
    porc_dto = Configuracion_global->MonedatoDouble(ui->txtPorc_dto->text());

    if(Configuracion_global->MonedatoDouble(ui->txtPVP->text()) <0 && ui->txtPvp_recomendado->text() =="0,00")
    {
        ui->txtPvp_recomendado->setText(ui->txtPVP->text());
        ui->txtPorc_dto->setText("0,00");
    }
    double dto;
    if(ui->txtPorc_dto->text() =="0,00")
    {
        if(ui->txtPVP->text() != ui->txtPvp_recomendado->text())
        {
            double pvp_recom;
            dto = Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text()) -
                    Configuracion_global->MonedatoDouble(ui->txtPVP->text());
            if(ui->txtPvp_recomendado->text() == "0,00")
                pvp_recom = 1;
            else
                pvp_recom = Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text());
            if(ui->txtPorc_dto->text() =="0,00" || ui->txtPorc_dto->text() =="0")
            {
                float porc_dto = (dto * 100) / pvp_recom;
                ui->txtPorc_dto->setText(Configuracion_global->toFormatoMoneda(QString::number(porc_dto,'f',
                                                                                               Configuracion_global->decimales_campos_totales)));
            }
        }
    }

    double subtotal,base,precio_cliente,total_con_iva;
    subtotal = Configuracion_global->MonedatoDouble(ui->txtCantidad->text()) * Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text());
    if(subtotal == -0)
        subtotal = 0;
    dto = subtotal * (Configuracion_global->MonedatoDouble(ui->txtPorc_dto->text()))/100;
    base = subtotal -dto;
    if(pvp_rec ==0 && porc_dto ==0)
    {
        ui->txtPvp_recomendado->setText(ui->txtPVP->text());
        ui->txtPorc_dto->setText("0,00");
    }

    if(Configuracion_global->MonedatoDouble(ui->txtPVP->text()) <0)
        precio_cliente =  Configuracion_global->MonedatoDouble(ui->txtPVP->text());
     else
        precio_cliente = Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text())-(
                    Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text())*
                    Configuracion_global->MonedatoDouble(ui->txtPorc_dto->text())/100);

    ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(QString::number(precio_cliente,'f',Configuracion_global->decimales)));
    total = Configuracion_global->MonedatoDouble(Configuracion_global->toFormatoMoneda(QString::number(base)));
    ui->txt_total_linea->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',
                                                                     Configuracion_global->decimales_campos_totales)));
    total_con_iva = base + (base * ui->cboIva->currentText().toFloat()/100);
    ui->txtImporte_iva->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                   base * ui->cboIva->currentText().toFloat()/100,'f',
                                                                   Configuracion_global->decimales_campos_totales)));
    ui->txtTotal_con_iva->setText(Configuracion_global->toFormatoMoneda(
                QString::number(total_con_iva,'f',
                                Configuracion_global->decimales_campos_totales)));


}

bool frmEditLine::getAdd_pendientes() const
{
    return add_pendientes;
}

void frmEditLine::setAdd_pendientes(bool value)
{
    add_pendientes = value;
}

void frmEditLine::on_txtCantidad_editingFinished()
{
    if(Configuracion_global->MonedatoDouble(ui->txtStockAlmacen->text()) -
            Configuracion_global->MonedatoDouble(ui->txtCantidad->text()) <0  && this->is_venta)
        QMessageBox::warning(this,tr("Ventas"),tr("No tiene suficientes unidades en stock para la venta"),tr("Aceptar"));
    calcular();
}

void frmEditLine::on_txtPVP_editingFinished()
{
    ui->txtPVP->blockSignals(true);
    ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(ui->txtPVP->text()));
    calcular();
    ui->txtPVP->blockSignals(false);
}

void frmEditLine::on_txtPvp_recomendado_editingFinished()
{
    ui->txtPvp_recomendado->blockSignals(true);
    ui->txtPvp_recomendado->setText(Configuracion_global->toFormatoMoneda(ui->txtPvp_recomendado->text()));
    ui->txtPvp_recomendado->blockSignals(false);
}

void frmEditLine::on_txtPorc_dto_editingFinished()
{
    ui->txtPorc_dto->blockSignals(true);
    ui->txtPorc_dto->setText(Configuracion_global->toFormatoMoneda(ui->txtPorc_dto->text()));
    calcular();
    ui->txtPorc_dto->blockSignals(false);
}

void frmEditLine::on_cboIva_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    calcular();
}

void frmEditLine::on_btnAceptar_clicked()
{
    if(ui->txtDescripcion->text().isEmpty())
    {
        this->accept();
        return;
    }

    QHash<QString, QVariant> lin = get_datos();

    QString error;    

    if(this->editando)
    {
        // edición línea
        double nueva_cantidad = Configuracion_global->MonedatoDouble(ui->txtCantidad->text()) - anterior.value("cantidad").toDouble();
        //double nuevo_valor =Configuracion_global->MonedatoDouble(ui->txt_total_linea->text()) - anterior.value("total").toDouble();
        if (!SqlCalls::SqlUpdate(lin,this->tabla,Configuracion_global->empresaDB,QString("id=%1").arg(this->id),error))
        {
            QMessageBox::critical(this,tr("Error al actualizar linea"),error);
            close();
            return;
        }
        do_stocks(nueva_cantidad);
    }
    else // Nueva línea
    {        
        _insert_nueva_linea();
    }
    emit refrescar_lineas();
    accept();
}

void frmEditLine::on_btnAnadir_mas_nueva_clicked()
{
    if(ui->txtDescripcion->text().isEmpty())
        return;

    _insert_nueva_linea();
   vaciar_campos();
   emit refrescar_lineas();
}
int frmEditLine::getId_prov() const
{
    return id_prov;
}

void frmEditLine::setId_prov(int value)
{
    id_prov = value;
}

