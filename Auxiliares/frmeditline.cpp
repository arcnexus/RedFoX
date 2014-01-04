#include "frmeditline.h"
#include "ui_frmeditline.h"
#include "Busquedas/db_consulta_view.h"

frmEditLine::frmEditLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmEditLine)
{
    ui->setupUi(this);
    this->dto_tarifa =0;
    ui->lblpromocionado->setVisible(false);
    ui->chk3_2->setVisible(false);
    ui->chkdto->setVisible(false);
    ui->chkprecio->setVisible(false);
    ui->chk_dtoweb->setVisible(false);
    oArticulo = new Articulo(this);
    //IVA desde config
    ui->cboIva->setModel(Configuracion_global->iva_model);
    ui->cboIva->setModelColumn(Configuracion_global->iva_model->fieldIndex("iva"));
}

frmEditLine::~frmEditLine()
{
    delete ui;
}

int frmEditLine::get_id()
{
    return this->id;
}

void frmEditLine::set_linea(int id,QString fichero)
{
    //-----------------------------------------------
    // Recuperar línea de ventas del fichero fichero
    //-----------------------------------------------
    QMap <int,QSqlRecord> m;
    QString error;
    m = SqlCalls::SelectRecord(fichero,QString("id=%1").arg(id),Configuracion_global->empresaDB,error);
    if(error.isEmpty())
    {
        // TODO - Cargar las líneas desde el fichero
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
}

void frmEditLine::set_tipo(QString tipo)
{
    this->tipo = tipo;
}

void frmEditLine::set_dto_tarifa(float dto)
{
    this->dto_tarifa = dto;
}



void frmEditLine::on_txtCodigo_editingFinished()
{
    ui->txtCodigo->blockSignals(true);
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
        cabecera  << tr("Código") << tr("Código Barras") << tr("Referencia") << tr("Descripción") << tr("Coste") <<tr("pvp") <<(tr("P.RECOM."));
        tamanos <<"0" << "100" << "100" << "100" << "320" <<"130" <<"130" <<"130";
        moneda <<"5" <<"6" <<"7";
        consulta.set_headers(cabecera);
        consulta.set_tamano_columnas(tamanos);
        consulta.set_delegate_monetary(moneda);
        consulta.set_titulo("Busqueda de Artículos");
        consulta.set_filtro("");
        if(consulta.exec())
        {
            this->id = consulta.get_id();

       }
    } else
    {
        QString error;
           this->id = SqlCalls::SelectOneField("articulos","id",QString("codigo = '%1'").arg(ui->txtCodigo->text()),
                                               Configuracion_global->groupDB,error).toInt();
        if(!error.isEmpty()){
            QMessageBox::warning(this,tr("Edición de líneas"),tr("No se pudo recuperar el artículo: %1").arg(error),
                                 tr("Acceptar"));
            close();
        }
    }
    cargar_articulo(this->id,this->id_tarifa,this->dto_tarifa);

    ui->txtCodigo->blockSignals(false);
}

void frmEditLine::cargar_articulo(int id,int tarifa,int tipo_dto)
{
    //---------------------
    // BUSCAR ARTICULO
    //---------------------
    QMap <int,QSqlRecord> m;
    QString error;
    float dto;
    this->dto_tarifa = tipo_dto;
    QStringList condiciones;
    condiciones << QString("id = '%1'").arg(id);
    condiciones << QString("tarifa = %1").arg(tarifa);

    m = SqlCalls::SelectRecord("vistaart_tarifa",condiciones,Configuracion_global->groupDB,
                               error);
    QMapIterator <int,QSqlRecord> i(m);
    QHash <QString, QVariant> h;
    h["found"] = false;
    while(i.hasNext())
    {
        i.next();
        this->id = i.value().value("id").toInt();
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

        ui->txtPvp_conIva->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                             i.value().value("pvp_con_iva").toDouble(),'f',
                                                                             Configuracion_global->decimales)));
        ui->txtPvp_recomendado->setText(Configuracion_global->toFormatoMoneda(QString::number(i.value().value("pvp").toDouble(),'f',
                                                                                              Configuracion_global->decimales)));

        ui->txtCantidad->setText("1");
        int index = ui->cboIva->findText(i.value().value("tipo_iva").toString());
        ui->cboIva->setCurrentIndex(index);

        switch (this->dto_tarifa) {
        case 1:
            ui->txtPorc_dto->setText(i.value().value("porc_dto1").toString());
            break;
        case 2:
            ui->txtPorc_dto->setText(i.value().value("porc_dto2").toString());
            break;
        case 3:
            ui->txtPorc_dto->setText(i.value().value("porc_dto3").toString());
            break;
        case 4:
            ui->txtPorc_dto->setText(i.value().value("porc_dto4").toString());
            break;
        case 5:
            ui->txtPorc_dto->setText(i.value().value("porc_dto5").toString());
            break;
        case 6:
            ui->txtPorc_dto->setText(i.value().value("porc_dto6").toString());
            break;
        default:
            ui->txtPorc_dto->setText("0");
            break;


        }
        double pvp = (i.value().value("pvp").toDouble()-(i.value().value("pvp").toDouble()*(ui->txtPorc_dto->text().toFloat()/100)));
        ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                      pvp,'f', Configuracion_global->decimales)));
        // STOCKS
        ui->txtStockAlmacen->setText(i.value().value("stock_fisico_almacen").toString());
        ui->txtStock_real->setText(i.value().value("stock_real").toString());
        ui->txtPendientes_recibir->setText(i.value().value("unidades_pendientes_recibir").toString());

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
                    if(p.value(i.value().value("id").toInt()).value("precio_final").toDouble()<h.value("precio").toDouble()) {
                        ui->txtPVP->setText(QString::number(p.value(i.value().value("id").toInt()).value("precio_final").toDouble(),'f',2));
                    }
                }

            }

        }
        //--------------------------
        // Control de Execpciones
        //--------------------------
    // TODO

    }
    calcular();
}

void frmEditLine::calcular()
{
    double subtotal, dto, base;
    subtotal = Configuracion_global->MonedatoDouble(ui->txtCantidad->text()) * Configuracion_global->MonedatoDouble(ui->txtPvp_recomendado->text());
    dto = subtotal * (Configuracion_global->MonedatoDouble(ui->txtPorc_dto->text()))/100;
    base = subtotal -dto;
    ui->txt_tota_linea->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',
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
