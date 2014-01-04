#include "frmeditline.h"
#include "ui_frmeditline.h"
#include "Busquedas/db_consulta_view.h"

frmEditLine::frmEditLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmEditLine)
{
    ui->setupUi(this);
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

void frmEditLine::set_tipo(QString tipo)
{
    this->tipo = tipo;
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
        consulta.setId_tarifa_cliente(this->tarifa);
        consulta.setTipo_dto_tarifa(this->tipo_dto_tarifa);
        if(this->tipo_dto_tarifa ==1)
            consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                         "porc_dto1/100))) as pvp,pvp as pvp_recom from vistaart_tarifa");
        else if(this->tipo_dto_tarifa ==2)
            consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                         "porc_dto2/100))) as pvp, pvp as pvp_recom from vistaart_tarifa");
        else if(this->tipo_dto_tarifa ==3)
            consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                         "porc_dto3/100))) as pvp,pvp as pvp_recom from vistaart_tarifa");
        else if(this->tipo_dto_tarifa ==4)
            consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                         "porc_dto4/100))) as pvp,pvp as pvp_recom from vistaart_tarifa");
        else if(this->tipo_dto_tarifa ==5)
            consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                         "porc_dto5/100))) as pvp,pvp as pvp_recom from vistaart_tarifa");
        else if(this->tipo_dto_tarifa ==6)
            consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                         "porc_dto6/100))) as pvp,pvp as pvp_recom from vistaart_tarifa");
        else if(this->tipo_dto_tarifa == 0)
            consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,pvp, pvp as pvp_recom"
                             " from vistaart_tarifa");
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
           this->id = SqlCalls::SelectOneField("articulos","id",QString("codigo = '%1'").arg(ui->txtCodigo->text()),
                                               Configuracion_global->groupDB,error);
    }
    cargar_articulo(this->id);

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
        ui->txtPVP->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                      i.value().value("pvp").toDouble(),'f',
                                                                      Configuracion_global->decimales)));
        ui->txtCantidad ="1";
        int index = ui->cboIva->findText(i.value().value("tipo_iva").toString());
        ui->cboIva->setCurrentIndex(index);

        switch (tipo_dto) {
        case 1:
            ui->txtPorc_dto->setText(i.value().value("porc_dto1").toString());
            break;
        case 2:
            ui->txtPorc_dto->setText(i.value().value("porc_dto2").toFloat());
            break;
        case 3:
            ui->txtPorc_dto->setText(dto = i.value().value("porc_dto3").toFloat());
            break;
        case 4:
            ui->txtPorc_dto->setText(dto = i.value().value("porc_dto4").toFloat());
            break;
        case 5:
            ui->txtPorc_dto->setText(dto = i.value().value("porc_dto5").toFloat());
            break;
        case 6:
            ui->txtPorc_dto->setText(dto = i.value().value("porc_dto6").toFloat());
            break;
        default:
            ui->txtPorc_dto->setText("0");
            break;


        }
 // ---------------------------- Aqui me he quedado----------------
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
