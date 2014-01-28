#include "frmeditline.h"
#include "ui_frmeditline.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Almacen/frmselectlotes.h"

frmEditLine::frmEditLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmEditLine)
{
    ui->setupUi(this);

    this->editando = false;
    this->dto_tarifa =0;
    ui->lblpromocionado->setVisible(false);
    ui->chk3_2->setVisible(false);
    ui->chkdto->setVisible(false);
    ui->chkprecio->setVisible(false);
    ui->chk_dtoweb->setVisible(false);
    oArticulo = new Articulo(this);
    //-------------------
    //IVA desde config
    //-------------------
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

frmEditLine::~frmEditLine()
{
    delete ui;
}

bool frmEditLine::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

       //&&  keyEvent->modifiers() == Qt::ControlModifier

        if(keyEvent->key() == Qt::Key_Plus)
        {
            on_btnAnadir_mas_nueva_clicked();
            return true;
        }
        if(keyEvent->key() == Qt::Key_End)
        {
            on_btnAceptar_clicked();
            return true;
        }
        if(keyEvent->key() == Qt::Key_Escape)
            return true;
    }
    return QDialog::eventFilter(obj,event);
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

void frmEditLine::set_tipo(QString tipo)
{
    this->tipo = tipo;
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



void frmEditLine::on_txtCodigo_editingFinished()
{
    ui->txtCodigo->blockSignals(true);
    if(ui->txtCodigo->text() != this->codigo_articulo )
    {
        if(ui->txtCodigo->text() == "*")
        {
            db_consulta_view consulta(qApp->activeWindow());
            QStringList campos;
            campos << "descripcion" <<"codigo" <<"codigo_barras" << "codigo_fabricante"  << "coste";
            consulta.set_campoBusqueda(campos);
            consulta.set_texto_tabla("articulos");
            consulta.set_db("Maya");
            consulta.setId_tarifa_cliente(this->id_tarifa);


            if(Configuracion_global->importado_sp)
            {

                //--------------------------------
                // precio cliente segun descuento
                //--------------------------------
                if(this->dto_tarifa ==1)
                    consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                                 "porc_dto1/100))) as pvp,pvp as pvp_recom from vistaart_tarifa");
                else if(this->dto_tarifa ==2)
                    consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                                 "porc_dto2/100))) as pvp, pvp as pvp_recom from vistaart_tarifa");
                else if(this->dto_tarifa ==3)
                    consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                                 "porc_dto3/100))) as pvp,pvp as pvp_recom from vistaart_tarifa");
                else if(this->dto_tarifa ==4)
                    consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                                 "porc_dto4/100))) as pvp,pvp as pvp_recom from vistaart_tarifa");
                else if(this->dto_tarifa ==5)
                    consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                                 "porc_dto5/100))) as pvp,pvp as pvp_recom from vistaart_tarifa");
                else if(this->dto_tarifa ==6)
                    consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                                 "porc_dto6/100))) as pvp,pvp as pvp_recom from vistaart_tarifa");
                else if(this->dto_tarifa == 0)
                    consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,pvp, pvp as pvp_recom"
                                     " from vistaart_tarifa");
            }
            else
            {
                consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,pvp, pvp as pvp_recom"
                                 " from vistaart_tarifa");
            }
            QStringList cabecera;
            QVariantList tamanos;
            QVariantList moneda;
            cabecera  << tr("Código") << tr("Código Barras") << tr("Referencia") << tr("Descripción") << tr("Coste") <<tr("pvp");
            cabecera <<tr("P.RECOM.");
            tamanos <<"0" << "100" << "100" << "100" << "320" <<"130" <<"130" <<"130";
            moneda <<"5" <<"6" <<"7";
            consulta.set_headers(cabecera);
            consulta.set_tamano_columnas(tamanos);
            consulta.set_delegate_monetary(moneda);
            consulta.set_titulo("Busqueda de Artículos");
            consulta.set_filtro("");
            if(consulta.exec())
            {
                this->id_articulo = consulta.get_id();

           }
        } else if(ui->txtCodigo->text().isEmpty())
        {
            QMessageBox::information(this,tr("Edición de líneas de detalle"),
                                     tr("Ha dejado el código en blanco, si es correcto deberá introducir el resto de datos necesarios a mano,"
                                        "\nDescripción, \nPVP, \n%IVA"),tr("Aceptar"));

        } else
        {
            QString error;
               this->id_articulo = SqlCalls::SelectOneField("articulos","id",QString("codigo = '%1'").arg(ui->txtCodigo->text()),
                                                   Configuracion_global->groupDB,error).toInt();
            if(!error.isEmpty()){
                QMessageBox::warning(this,tr("Edición de líneas"),tr("No se pudo recuperar el artículo: %1").arg(error),
                                     tr("Acceptar"));
                vaciar_campos();
                ui->txtCodigo->setFocus();
            }
        }
        if(this->id_articulo >0)
            cargar_articulo(this->id_articulo,this->id_tarifa,this->dto_tarifa);
    }
    ui->txtCodigo->blockSignals(false);
}

void frmEditLine::cargar_articulo(int id_art,int tarifa,int tipo_dto)
{
    //---------------------
    // BUSCAR ARTICULO
    //---------------------
    QMap <int,QSqlRecord> m;
    QString error;
    float dto;
    this->dto_tarifa = tipo_dto;
    QStringList condiciones;
    condiciones << QString("id = '%1'").arg(id_art);
    condiciones << QString("tarifa = %1").arg(tarifa);

    m = SqlCalls::SelectRecord("vistaart_tarifa",condiciones,Configuracion_global->groupDB,
                               error);
    if(!error.isEmpty())
        QMessageBox::warning(this,tr("Edición de líneas"),tr("Ocurrió un error al recuperar: %1").arg(error));

    QMapIterator <int,QSqlRecord> i(m);
//    QHash <QString, QVariant> h;
//    h["found"] = false;
    while(i.hasNext())
    {
        i.next();
        this->id_articulo = i.value().value("id").toInt();
        if(this->tipo == "V")
        {
            ui->txtCodigo->setText(i.value().value("codigo").toString());
            if(!Configuracion_global->descripcion_resumida_lineas)
                ui->txtDescripcion->setText(i.value().value("descripcion").toString());
            else
                ui->txtDescripcion->setText(i.value().value("descripcion_reducida").toString());
        } else
        {
            if (Configuracion_global->referencia_fabricante_lineas_compras)
                ui->txtCodigo->setText(i.value().value("codigo_fabricante").toString());
            else
                ui->txtCodigo->setText(i.value().value("codigo").toString());
        }
        if (i.value().value("lotes").toBool())
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
        ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(QString::number(i.value().value("pvp_cliente").toDouble(),'f',
                                                                                  Configuracion_global->decimales_campos_totales)));

        ui->txtPvp_conIva->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                             i.value().value("pvp_con_iva").toDouble(),'f',
                                                                             Configuracion_global->decimales_campos_totales)));
        ui->txtPvp_recomendado->setText(Configuracion_global->toFormatoMoneda(QString::number(i.value().value("pvp").toDouble(),'f',
                                                                                              Configuracion_global->decimales)));
        //-------------------------
        // Calculamos %dto tarifa
        //-------------------------
        float pvp_recom;
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
        //---------------------------- Fin calculo & dto tarifa

        ui->txtCantidad->setText("1");
        int index = ui->cboIva->findText(i.value().value("tipo_iva").toString());
        ui->cboIva->setCurrentIndex(index);

    }
//    double pvp = (i.value().value("pvp").toDouble()-(i.value().value("pvp").toDouble()*(ui->txtPorc_dto->text().toFloat()/100)));
//    ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(QString::number(
//                                                                  pvp,'f', Configuracion_global->decimales)));
    // STOCKS
    ui->txtStockAlmacen->setText(i.value().value("stock_fisico_almacen").toString());
    ui->txtStock_real->setText(i.value().value("stock_real").toString());
    ui->txtPendientes_recibir->setText(i.value().value("unidades_pendientes_recibir").toString());
    ui->txtUnidades_reservadas->setText(i.value().value("unidades_reservadas").toString());
    ui->dateFecha_recepcion->setDate(i.value().value("fecha_prevista_recepcion").toDate());

    // PROMOCIONES ARTÍCULO
    bool art_promo = SqlCalls::SelectOneField("articulos","articulo_promocionado",
                                              QString("id=%1").arg(this->id),Configuracion_global->groupDB,
                               error).toBool();
    if(art_promo)
    {
        ui->lblpromocionado->setVisible(true);
        ui->chk3_2->setVisible(true);
        ui->chkdto->setVisible(true);
        ui->chkprecio->setVisible(true);
        ui->chk_dtoweb->setVisible(true);

        QStringList filter;
        filter << QString("id=%1").arg(i.value().value("id").toInt());
        filter << "activa = 1";

        QMap <int,QSqlRecord> p;
        p=SqlCalls::SelectRecord("articulos_ofertas",filter, Configuracion_global->empresaDB,error);
        if(error.isEmpty())
        {
            //-------------
            // Oferta 3*2
            //-------------
            if(p.value(i.value().value("id").toInt()).value("oferta32").toBool())
            {
                ui->chk3_2->setChecked(true);
                float por_cada = p.value(i.value().value("id").toInt()).value("unidades").toFloat();
                float regalo = p.value(i.value().value("id").toInt()).value("regalo").toFloat();
                int cant = cant / por_cada;
                this->unidades_regalo = cant * regalo;
            }
            //--------------
            // Oferta web
            //--------------
            if(p.value(i.value().value("id").toInt()).value("oferta_web").toBool())
            {
                ui->chk_dtoweb->setChecked(true);
                float dto_web = p.value(i.value().value("id").toInt()).value("dto_web").toFloat();
                if(dto_web > dto){
                    ui->txtPorc_dto->setText(QString::number(dto_web,'f',2));
                }
            }
            //--------------
            // Oferta dto
            //--------------
            if(p.value(i.value().value("id").toInt()).value("oferta_dto").toBool())
            {
                ui->chkdto->setChecked(true);
                float dto_local = p.value(i.value().value("id").toInt()).value("dto_local").toFloat();
                if(dto_local > dto){
                    ui->txtPorc_dto->setText(QString::number(dto_local,'f',2));
                }
            }
            //----------------------
            // Oferta precio final
            //----------------------
            if(p.value(i.value().value("id").toInt()).value("oferta_precio_final").toBool())
            {
                ui->chkprecio->setChecked(true);
                if(p.value(i.value().value("id").toInt()).value("precio_final").toDouble()<i.value().value("precio").toDouble()) {
                    ui->txtPVP->setText(QString::number(p.value(i.value().value("id").toInt()).value("precio_final").toDouble(),'f',2));
                }
            }

        }

    }
        //--------------------------
        // Control de Execpciones
        //--------------------------
    // TODO
                   calcular();

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


void frmEditLine::on_txtCantidad_editingFinished()
{
    calcular();
}

void frmEditLine::on_txtPVP_editingFinished()
{
    ui->txtPVP->blockSignals(true);
    ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(ui->txtPVP->text()));
    //ui->txtPorc_dto->setText("0,00");
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
        on_txtCodigo_editingFinished();
    QHash <QString, QVariant> lin;
    lin["cantidad"] = ui->txtCantidad->text().toFloat();
    lin["codigo"] = ui->txtCodigo->text();
    lin["descripcion"] = ui->txtDescripcion->text();
    lin["dto"] = Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text()) - Configuracion_global->MonedatoDouble(
                ui->txtPVP->text());
    //lin["id"] = this->id;
    lin["id_articulo"]  =this->id_articulo;
    lin["id_cab"] = this->id_cab;
    lin["porc_dto"] = Configuracion_global->MonedatoDouble(ui->txtPorc_dto->text());
    lin["porc_iva"] = ui->cboIva->currentText().toFloat();
    lin["cantidad"] = Configuracion_global->MonedatoDouble(ui->txtCantidad->text());
    lin["precio"] = Configuracion_global->MonedatoDouble(ui->txtPVP->text());
    lin["precio_recom"] = Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text());
    lin["promocion"] = ui->lblpromocionado->isVisible();
    lin["subtotal"] = ui->txtCantidad->text().toFloat() * Configuracion_global->MonedatoDouble(ui->txtPVP->text());
    lin["total"] = Configuracion_global->MonedatoDouble(ui->txt_total_linea->text());
    lin["id_lote"] = this->id_lote;
    QString error;
    int cant = anterior.value("cantidad").toFloat();
    if(this->editando)
    {
        //----------------
        // edición línea
        //----------------
        bool success = SqlCalls::SqlUpdate(lin,this->tabla,Configuracion_global->empresaDB,QString("id=%1").arg(this->id),
                                         error);
        if ( success)
        {
            if(this->realiza_acumulados)
            {
                oArticulo->acumulado_ventas(this->id_articulo,Configuracion_global->MonedatoDouble(ui->txtCantidad->text())-cant,
                                                Configuracion_global->MonedatoDouble(ui->txt_total_linea->text())-
                                                         anterior.value("total").toDouble(),QDate::currentDate(),"V");
                // ----------------------------
                // Lotes
                //-----------------------------
                if(this->id_lote>0);
                {
                    QSqlQuery lote(Configuracion_global->groupDB);
                    lote.exec(QString("update articulos_lote set stock = stock - %1 where id = %2").arg(
                                  Configuracion_global->MonedatoDouble(ui->txtCantidad->text())-cant,
                                  this->id_lote));

                }
            } else
            {
                // -------------------------------------------
                // Si no acumula añade a unidades reservadas
                //--------------------------------------------
                QSqlQuery queryart(Configuracion_global->groupDB);
                queryart.exec(QString("update articulos set unidades_reservadas = unidades_reservadas + %1 where id = %2").arg(
                                  Configuracion_global->MonedatoDouble(ui->txtCantidad->text())-anterior.value("cantidad").toFloat(),
                                  this->id_articulo));
                emit refrescar_lineas();
            }



            emit refrescar_lineas();
            accept();
        } else {
            QMessageBox::warning(this,tr("Edición lineas detalle"),
                             tr("Falló al guardar la linea de detalle en la BD: %1").arg(error),
                             tr("Aceptar"));

        }
    } else
    {
        //----------------------------------------
        // Nueva línea
        //----------------------------------------
        this->id = SqlCalls::SqlInsert(lin,this->tabla,Configuracion_global->empresaDB,error);
        if(this->id >0 && oArticulo->acumulado_ventas(this->id_articulo,Configuracion_global->MonedatoDouble(ui->txtCantidad->text())-
                                                                                        anterior.value("cantidad").toFloat(),
                                                      Configuracion_global->MonedatoDouble(ui->txt_total_linea->text())-
                                                                                           anterior.value("total").toDouble(),
                                                      QDate::currentDate(),"V"))
        {
            // ----------------------------
            // Lotes
            //-----------------------------
            if(this->id_lote>0);
            {
                QString idlote = QString::number(this->id_lote);
                QSqlQuery lote(Configuracion_global->groupDB);
                if(!lote.exec(QString("update articulos_lotes set stock = stock - %1 where id = %2").arg(
                              QString::number(Configuracion_global->MonedatoDouble(ui->txtCantidad->text()),'f',2),
                                  idlote)))
                    QMessageBox::warning(this,tr("Edición de líneas"),
                                         tr("No se pudo actualizar el stock del lote: %1").arg(lote.lastError().text()));

            }
            emit refrescar_lineas();
            accept();
        }
        else
        {
            QMessageBox::warning(this,tr("Edición lineas detalle"),
                                 tr("Falló al añadir la linea de detalle en la BD: %1").arg(error),
                                 tr("Aceptar"));
            close();
        }

    }

}

void frmEditLine::on_btnAnadir_mas_nueva_clicked()
{
    QHash <QString, QVariant> lin;
    lin["cantidad"] = ui->txtCantidad->text().toFloat();
    lin["codigo"] = ui->txtCodigo->text();
    lin["descripcion"] = ui->txtDescripcion->text();
    lin["dto"] = Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text()) - Configuracion_global->MonedatoDouble(
                ui->txtPVP->text());
    //lin["id"] = this->id;
    lin["id_articulo"]  =this->id_articulo;
    lin["id_cab"] = this->id_cab;
    lin["porc_dto"] = Configuracion_global->MonedatoDouble(ui->txtPorc_dto->text());
    lin["porc_iva"] = ui->cboIva->currentText().toFloat();
    lin["cantidad"] = Configuracion_global->MonedatoDouble(ui->txtCantidad->text());
    lin["precio"] = Configuracion_global->MonedatoDouble(ui->txtPVP->text());
    lin["precio_recom"] = Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text());
    lin["promocion"] = ui->lblpromocionado->isVisible();
    lin["subtotal"] = ui->txtCantidad->text().toFloat() * Configuracion_global->MonedatoDouble(ui->txtPVP->text());
    lin["total"] = Configuracion_global->MonedatoDouble(ui->txt_total_linea->text());
    QString error;
    this->id = SqlCalls::SqlInsert(lin,this->tabla,Configuracion_global->empresaDB,error);
    if(this->realiza_acumulados)
    {
        if(this->id >0 && oArticulo->acumulado_ventas(this->id_articulo,Configuracion_global->MonedatoDouble(ui->txtCantidad->text())-
                                                                                  anterior.value("cantidad").toFloat(),
                                                      Configuracion_global->MonedatoDouble(ui->txt_total_linea->text())-
                                                                                           anterior.value("total").toDouble(),
                                                      QDate::currentDate(),"V"))
        {
            emit refrescar_lineas();
            // ----------------------------
            // Lotes
            //-----------------------------
            if(this->id_lote>0);
            {
                QSqlQuery lote(Configuracion_global->groupDB);
                lote.exec(QString("update articulos_lote set stock = stock - %1 where id = %2").arg(
                              Configuracion_global->MonedatoDouble(ui->txtCantidad->text())-anterior.value("cantidad").toFloat(),
                              this->id_lote));
            }
        } else

        {
            QMessageBox::warning(this,tr("Edición lineas detalle"),
                                 tr("Falló al añadir la linea de detalle en la BD: %1").arg(error),
                                 tr("Aceptar"));

        }
    } else
    {
        // -------------------------------------------
        // Si no acumula añade a unidades reservadas
        //--------------------------------------------
        QSqlQuery queryart(Configuracion_global->groupDB);
        queryart.exec(QString("update articulos set unidades_reservadas = unidades_reservadas + %1 where id = %2").arg(
                          Configuracion_global->MonedatoDouble(ui->txtCantidad->text())-anterior.value("cantidad").toFloat(),
                          this->id_articulo));
        emit refrescar_lineas();
    }
   vaciar_campos();

}
