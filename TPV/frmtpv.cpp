#include "frmtpv.h"
#include "ui_frmtpv.h"
#include <QGraphicsOpacityEffect>
#include <QColor>
#include "../Auxiliares/datedelegate.h"
#include "../Auxiliares/monetarydelegate_totals.h"
#include "../Auxiliares/refresca_ofertas.h"
#include "../TPV/frmcierrecaja.h"
#include "../TPV/frmextrascaja.h"
#include "../TPV/frmretirardecaja.h"
#include "../TPV/frminsertardinerocaja.h"
#include "../TPV/frmdevolucionticket.h"
#include "../TPV/frmcantidad.h"
#include "../Busquedas/db_consulta_view.h"


FrmTPV::FrmTPV(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmTPV),
    menuButton(QIcon(":/Icons/PNG/tpv.png"),tr("TPV"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/tpv.png"),"",this))
{
    ui->setupUi(this);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de Caja"));

    ui->btnPorc->setEnabled(false);
    ui->btnMultiplicar->setEnabled(false);
    ui->btnDividir->setEnabled(false);
    this->tipo_dto_tarifa = 0;

    oTpv = new tpv(this);
    oTpv->id_usuario = Configuracion_global->id_usuario_activo;
    oArticulo = new Articulo(this);
    oRefresca = new refresca_ofertas(this);
    ui->frame_ticket->setCurrentIndex(0);
    ui->frameTeclado->setCurrentIndex(0);
    ui->frmcontrol->setCurrentIndex(0);
    ui->lblCobrado->setVisible(false);
    ui->btnBuscarArt_ext->setVisible(false);


    QSqlQuery qtpv(Configuracion_global->empresaDB);
    QString cSQL = QString("select id,ticket from cab_tpv where caja = %1 and serie = '%2' order by ticket desc limit 1").arg(
                Configuracion_global->caja,Configuracion_global->serie);
    int last_id;
    if(qtpv.exec(cSQL))
        qtpv.next();
        last_id = qtpv.record().value("id").toInt();
    cargar_ticket(last_id);

    //-----------------
    // Lineas tickets
    //-----------------
    model_lista_tpv = new QSqlQueryModel(this);
    model_lista_tpv->setQuery(QString("select id,serie, ticket, caja,importe, fecha,hora,nombre_cliente "
                                      "from cab_tpv where ejercicio = %1 order by ticket desc").arg(
                           Configuracion_global->cEjercicio),Configuracion_global->empresaDB);
    ui->lista_tickets->setModel(model_lista_tpv);
    estructura_lista();


    //----------------
    // Ofertas
    //----------------
    model_ofertas = new QSqlQueryModel(this);
    oRefresca->model = model_ofertas; //Esto estaba al reves
    oRefresca->tabla = ui->tabla_ofertas;


//    oRefresca->start(); //no empieces el thread hasta que no hayas puesto las variables


    //---------------
    // Eventos
    //---------------
    this->installEventFilter(this);

    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;

    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);


    //--------------------
    // Carga de objetos
    //--------------------
    QSqlQueryModel * model_cajas = new QSqlQueryModel(this);
    model_cajas->setQuery("select desc_caja from cajas",Configuracion_global->empresaDB);
    ui->cboCajas->setModel(model_cajas);
    QSqlQueryModel * model_users = new QSqlQueryModel(this);
    model_users->setQuery("select nombre from usuarios", Configuracion_global->groupDB);
    ui->cboUsuarioAbertura->setModel(model_users);
    ui->cboCajas_filtro->setModel(model_cajas);
    ui->txtFecha_fin_filtro->setDate(QDate::currentDate());
    ui->txtFecha_ini_filtro->setDate(QDate::currentDate());
    QStringList tipobusqueda;
    tipobusqueda << tr("Descripción") << tr("Código");
    ui->cboBusqueda->addItems(tipobusqueda);
    model_articulos = new QSqlQueryModel(this);
    model_articulos->setQuery("select id, codigo,descripcion, pvp, pvp_con_iva from vistaart_tarifa where id = 0",
                              Configuracion_global->groupDB);
    ui->tabla_buscar_art->setModel(model_articulos);
    QStringList headers;
    headers <<tr("id") <<tr("Código") <<tr("Descripción") << tr("PVP") << tr("PVP + IVA");
    QVariantList sizes;
    sizes << 0 << 100 << 120 << 100 << 100;
    for(int i = 0; i < sizes.size(); i++)
    {
        model_articulos->setHeaderData(i,Qt::Horizontal,headers.at(i));
        ui->tabla_buscar_art->setColumnWidth(i,sizes.at(i).toInt());
    }
    ui->tabla_buscar_art->setColumnHidden(0,true);
    ui->tabla_buscar_art->setItemDelegateForColumn(3, new MonetaryDelegate_totals);
    ui->tabla_buscar_art->setItemDelegateForColumn(4, new MonetaryDelegate_totals);

    //---------------
    // CAJA ABIERTA
    //---------------
    ui->lblregistrando->setText(tr("Debe abrir una caja para poder operar"));
    QMap <int,QSqlRecord> c;
    QString error;

    QStringList condiciones;
    condiciones << QString("fecha_abertura = '%1'").arg(QDate::currentDate().toString("yyyyMMdd"));
    condiciones << QString("id_caja = %1").arg(Configuracion_global->caja);
    condiciones << QString("ejercicio = '%1'").arg(Configuracion_global->cEjercicio);
    c = SqlCalls::SelectRecord("cierrecaja",condiciones,Configuracion_global->empresaDB,error);
    if(c.size()>0)
    {
        QMapIterator<int,QSqlRecord> it(c);
        if(it.hasNext())
        {
            it.next();
            QString frase;
            QString caja = SqlCalls::SelectOneField("cajas","desc_caja",
                                                    QString("id=%1").arg(it.value().value("id_caja").toString()),
                                                    Configuracion_global->empresaDB,error).toString();
            frase = tr("Caja: %1, fecha abertura caja: %2").arg(caja,
                                                              it.value().value("fecha_abertura").toDate().toString("dd/MM/yyyy"));
            ui->lblregistrando->setText(frase);
        }
    }
}

FrmTPV::~FrmTPV()
{
    delete ui;
}

void FrmTPV::cargar_ticket(int id)
{
    QMap <int,QSqlRecord> tpv;
    QString error;
    oTpv->id = id;
    ui->lblSerie->setText(Configuracion_global->serie);

    tpv = SqlCalls::SelectRecord("cab_tpv",QString("id =%1").arg(id),Configuracion_global->empresaDB,error);
    ui->lblTicket->setText(QString::number(tpv.value(id).value("ticket").toInt()));
    if(!tpv.value(id).value("nombre_cliente").toString().isEmpty()){
        ui->btnAsignarCliente->setText(tr("Cliente: %1").arg(tpv.value(id).value("nombre_cliente").toString()));
        oTpv->id_cliente = tpv.value(id).value("id_cliente").toInt();
        this->tipo_dto_tarifa = SqlCalls::SelectOneField("clientes","tipo_dto_tarifa",QString("id=%1").arg(oTpv->id_cliente),
                                                         Configuracion_global->groupDB,error).toInt();
    } else {
        ui->btnAsignarCliente->setText(tr("click para asignar Cliente"));
        oTpv->id_cliente = 0;
        this->tipo_dto_tarifa = 0;
    }
    ui->lblFecha->setText(tpv.value(id).value("fecha").toString());
    oTpv->fecha = tpv.value(id).value("fecha").toDate();
    ui->lblHora->setText(tpv.value(id).value("hora").toString());
    this->id = id;
    QString usuario = SqlCalls::SelectOneField("usuarios","nombre",
                                              QString("id=%1").arg(tpv.value(id).value("id_dependiente").toInt()),
                                              Configuracion_global->globalDB,error).toString();
    ui->lblDependiente->setText(usuario);
    oTpv->porc_iva1 = tpv.value(id).value("porc_iva1").toFloat();
    oTpv->porc_iva2 = tpv.value(id).value("porc_iva2").toFloat();
    oTpv->porc_iva3 = tpv.value(id).value("porc_iva3").toFloat();
    oTpv->porc_iva4 = tpv.value(id).value("porc_iva4").toFloat();


    //--------------------------------
    // Datos cobro (si se da el caso)
    //--------------------------------
    ui->lblCobrado->setVisible(tpv.value(id).value("cobrado").toBool());
    if(tpv.value(id).value("cobrado").toBool())
    {
        ui->btnCobrar_->setEnabled(false);
        ui->btnCobrar_imprimir_nuevo->setEnabled(false);
        ui->btnCobrar_nuevo->setEnabled(false);
        ui->btnCobrar_imprimir->setEnabled(false);
        ui->txtEfectivo->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                           tpv.value(id).value("importe_efectivo").toDouble(),'f',
                                                                           Configuracion_global->decimales_campos_totales)));
        ui->txtTarjeta->setText(Configuracion_global->toFormatoMoneda(QString::number(
                                                                           tpv.value(id).value("importe_tarjeta").toDouble(),'f',
                                                                           Configuracion_global->decimales_campos_totales)));

    } else
    {

        ui->btnCobrar_->setEnabled(true);
        ui->btnCobrar_imprimir_nuevo->setEnabled(true);
        ui->btnCobrar_nuevo->setEnabled(true);
        ui->btnCobrar_imprimir->setEnabled(true);
        ui->txtEfectivo->setText("0,00");
        ui->txtTarjeta->setText("0,00");
    }


    cargar_lineas(id);


}

void FrmTPV::cargar_lineas(int id_cab)
{
    ui->tablaLineas_tiquet_actual->clear();
    ui->tablaLineas_tiquet_actual->setColumnCount(6);
    ui->tablaLineas_tiquet_actual->setRowCount(1);
    QMap <int,QSqlRecord> lineas;
    QMap <int,QSqlRecord> lineas_extra;
    QString error;
    lineas = SqlCalls::SelectRecord("lin_tpv",QString("id_cab=%1").arg(id_cab),Configuracion_global->empresaDB,error);
    QMapIterator <int, QSqlRecord> rec_lin(lineas);

    QColor  rowcolor;
    QString codigo,descripcion;
    int id;
    float cantidad;
    float porc_iva;
    double precio= 0;
    double importe = 0;
    double total_ticket = 0;
    double base_ticket = 0;
    double iva_ticket = 0;
    double subtotal1 = 0;
    double subtotal2 = 0;
    double subtotal3 = 0;
    double subtotal4 = 0;

    double dto1 = 0;
    double dto2 = 0;
    double dto3 = 0;
    double dto4 = 0;



    int pos = -1;
    int lin =0;
    //-----------------
    // formato tabla
    //-----------------
    QVariantList sizes;
    sizes << 0 << 200 <<50 <<80 <<80;
    QStringList headers;
    headers << "id" << tr("Descripción") << tr("Cant") << tr("PVP") << tr("TOTAL");
    ui->tablaLineas_tiquet_actual->setHorizontalHeaderLabels(headers);
    for(int i = 0;i<sizes.size();i++)
    {
        ui->tablaLineas_tiquet_actual->setColumnWidth(i,sizes.at(i).toInt());

    }
    ui->tablaLineas_tiquet_actual->setColumnHidden(5,true);
    while(rec_lin.hasNext()){
        rec_lin.next();
        ui->tablaLineas_tiquet_actual->setRowCount(ui->tablaLineas_tiquet_actual->rowCount()+1);
        id = rec_lin.value().value("id").toInt();
        codigo = rec_lin.value().value("codigo").toString();
        cantidad = rec_lin.value().value("cantidad").toFloat();
        descripcion = rec_lin.value().value("descripcion").toString();
        precio = rec_lin.value().value("precio").toDouble();
        importe = rec_lin.value().value("importe").toDouble();
        porc_iva = rec_lin.value().value("porc_iva").toFloat();
        //---------------------
        // totales
        //---------------------
        base_ticket += importe;

        //---------------------
        // Desglose
        //---------------------
        if(porc_iva == oTpv->porc_iva1)
        {
            subtotal1 += importe;
        }
        if(porc_iva == oTpv->porc_iva2)
        {
            subtotal2 += importe;

        }
        if(porc_iva == oTpv->porc_iva3)
        {
            subtotal3 += importe;

        }
        if(porc_iva == oTpv->porc_iva4)
        {
            subtotal4 += importe;

        }




        //--------------------------------
        // Creo línea base en tablewidget
        //--------------------------------

        pos++;
        ui->tablaLineas_tiquet_actual->setRowCount(pos+1);
        lin++;
        if(lin%2 == 0)
            rowcolor.setRgb(240,240,255);
        else
            rowcolor.setRgb(255,255,255);

        QTableWidgetItem *item_col0 = new QTableWidgetItem(QString::number(id));
        item_col0->setFlags(item_col0->flags() & (~Qt::ItemIsEditable));
        item_col0->setBackgroundColor(rowcolor);

        ui->tablaLineas_tiquet_actual->setItem(pos,0,item_col0);
        ui->tablaLineas_tiquet_actual->setColumnHidden(0,true);

        QTableWidgetItem *item_col1 = new QTableWidgetItem(descripcion);
        item_col1->setFlags(item_col1->flags() & (~Qt::ItemIsEditable));
        item_col1->setBackgroundColor(rowcolor);
        ui->tablaLineas_tiquet_actual->setItem(pos,1,item_col1);

        QTableWidgetItem *item_col2 = new QTableWidgetItem(QString::number(cantidad));
        item_col2->setFlags(item_col2->flags() & (~Qt::ItemIsEditable));
        item_col2->setBackgroundColor(rowcolor);
        item_col2->setTextAlignment(Qt::AlignRight);
        ui->tablaLineas_tiquet_actual->setItem(pos,2,item_col2);

        QTableWidgetItem *item_col3 = new QTableWidgetItem(Configuracion_global->toFormatoMoneda(QString::number(precio,'f',
                                                                                         Configuracion_global->decimales)));
        item_col3->setFlags(item_col3->flags() & (~Qt::ItemIsEditable));
        item_col3->setBackgroundColor(rowcolor);
        item_col3->setTextAlignment(Qt::AlignRight);
        ui->tablaLineas_tiquet_actual->setItem(pos,3,item_col3);

        QTableWidgetItem *item_col4 = new QTableWidgetItem(Configuracion_global->toFormatoMoneda(QString::number(importe,'f',
                                                                          Configuracion_global->decimales_campos_totales)));
        item_col4->setFlags(item_col4->flags() & (~Qt::ItemIsEditable));
        item_col4->setBackgroundColor(rowcolor);
        item_col4->setTextAlignment(Qt::AlignRight);
        ui->tablaLineas_tiquet_actual->setItem(pos,4,item_col4);

        QTableWidgetItem *item_col5 = new QTableWidgetItem("*");
        item_col5->setFlags(item_col5->flags() & (~Qt::ItemIsEditable));
        item_col5->setBackgroundColor(rowcolor);
        item_col5->setTextAlignment(Qt::AlignRight);
        ui->tablaLineas_tiquet_actual->setItem(pos,5,item_col5);
        //--------------------------------------
        // Comprovamos líneas extra (dtos, etc)
        //--------------------------------------
        lineas_extra = SqlCalls::SelectRecord("lin_tpv_2",QString("id_cab=%1").arg(id),Configuracion_global->empresaDB,error);
            QMapIterator <int,QSqlRecord> lineas_e(lineas_extra);
        if(error.isEmpty())
        {
            QString descripcion_2,tipo;
            double  valor_esp;
            int id_lin;

            while(lineas_e.hasNext())
            {
                lineas_e.next();
                id_lin = lineas_e.value().value("id").toInt();
                descripcion_2 = lineas_e.value().value("descripcion").toString();
                tipo = lineas_e.value().value("tipo").toString();
                valor_esp = lineas_e.value().value("valor").toDouble();

                pos++;
                ui->tablaLineas_tiquet_actual->setRowCount(pos+1);
                QTableWidgetItem *item_s_col0 = new QTableWidgetItem(QString::number(id_lin));
                item_s_col0->setFlags(item_col0->flags() & (~Qt::ItemIsEditable));
                item_s_col0->setBackgroundColor(rowcolor);

                ui->tablaLineas_tiquet_actual->setItem(pos,0,item_s_col0);
                if(tipo =="i"){

                    QTableWidgetItem *item_s_col1 =
                            new QTableWidgetItem(QString("     %1 %2 %3").arg(descripcion_2).arg(QString::number(valor_esp)).arg("€"));
                    item_s_col1->setFlags(item_col1->flags() & (~Qt::ItemIsEditable));
                    item_s_col1->setBackgroundColor(rowcolor);
                    ui->tablaLineas_tiquet_actual->setItem(pos,1,item_s_col1);
                } else
                {
                    QTableWidgetItem *item_s_col1 =
                            new QTableWidgetItem(QString("     %1 %2 %3").arg(descripcion_2).arg(QString::number(valor_esp)).arg("%"));
                    item_s_col1->setFlags(item_col1->flags() & (~Qt::ItemIsEditable));
                    item_s_col1->setBackgroundColor(rowcolor);
                    ui->tablaLineas_tiquet_actual->setItem(pos,1,item_s_col1);
                }


                QTableWidgetItem *item_s_col2 = new QTableWidgetItem("");
                item_s_col2->setFlags(item_col2->flags() & (~Qt::ItemIsEditable));
                item_s_col2->setBackgroundColor(rowcolor);
                item_s_col2->setTextAlignment(Qt::AlignRight);

                ui->tablaLineas_tiquet_actual->setItem(pos,2,item_s_col2);

                QTableWidgetItem *item_s_col3 = new QTableWidgetItem(QString(""));
                item_s_col3->setFlags(item_col3->flags() & (~Qt::ItemIsEditable));
                item_s_col3->setBackgroundColor(rowcolor);

                ui->tablaLineas_tiquet_actual->setItem(pos,3,item_s_col3);
                valor_esp = -valor_esp;
                if(tipo == "i"){
                    QTableWidgetItem *item_s_col4 = new QTableWidgetItem(QString::number(valor_esp,'f',2));
                    item_s_col4->setFlags(item_col4->flags() & (~Qt::ItemIsEditable));
                    item_s_col4->setBackgroundColor(rowcolor);
                    item_s_col4->setTextAlignment(Qt::AlignRight);
                    ui->tablaLineas_tiquet_actual->setItem(pos,4,item_s_col4);
                } else
                {
                    valor_esp = importe *(valor_esp/100);
                    QTableWidgetItem *item_s_col4 =
                            new QTableWidgetItem(QString::number(valor_esp,'f',Configuracion_global->decimales_campos_totales));
                    item_s_col4->setFlags(item_col4->flags() & (~Qt::ItemIsEditable));
                    item_s_col4->setBackgroundColor(rowcolor);
                    item_s_col4->setTextAlignment(Qt::AlignRight);
                    ui->tablaLineas_tiquet_actual->setItem(pos,4,item_s_col4);
                }
                QTableWidgetItem *item_s_col5 =
                        new QTableWidgetItem(tipo);
                item_s_col5->setFlags(item_col5->flags() & (~Qt::ItemIsEditable));
                item_s_col5->setBackgroundColor(rowcolor);
                item_s_col5->setTextAlignment(Qt::AlignRight);
                ui->tablaLineas_tiquet_actual->setItem(pos,5,item_s_col5);

                total_ticket += valor_esp;

                //-----------------------------
                // Desglose a TPV lineas extra
                //-----------------------------
                if(porc_iva == oTpv->porc_iva1)
                {
                    dto1 += valor_esp;
                }
                if(porc_iva == oTpv->porc_iva2)
                {
                    dto2 += valor_esp;

                }
                if(porc_iva == oTpv->porc_iva3)
                {
                    dto3 += valor_esp;
                }
                if(porc_iva == oTpv->porc_iva4)
                {
                    dto4 += valor_esp;
                }


            }

        }else
            QMessageBox::warning(this,tr("Gestión de tickets"),
                                 tr("Falló la recuperación de sub-lineas de ticket: %1").arg(error));

    }
    this->row_tabla = pos;
    ui->tablaLineas_tiquet_actual->selectRow(pos);
    ui->lbl_total_ticket->setText(Configuracion_global->toFormatoMoneda(QString::number(total_ticket,'f',
                                                        Configuracion_global->decimales_campos_totales)));

    //-----------------------------
    // Desglose a TPV
    //-----------------------------
    oTpv->subtotal1 = subtotal1;
    oTpv->subtotal2 = subtotal2;
    oTpv->subtotal3 = subtotal3;
    oTpv->subtotal4 = subtotal4;

    oTpv->dto1 = dto1;
    oTpv->dto2 = dto2;
    oTpv->dto3 = dto3;
    oTpv->dto4 = dto4;

    oTpv->base1 = subtotal1 + dto1;
    oTpv->base2 = subtotal2 + dto2;
    oTpv->base3 = subtotal3 + dto3;
    oTpv->base4 = subtotal4 + dto3;

    oTpv->iva1 = oTpv->base1 *(oTpv->porc_iva1 /100);
    oTpv->iva2 = oTpv->base2 * (oTpv->porc_iva2 /100);
    oTpv->iva3 = oTpv->base3 * (oTpv->porc_iva3 /100);
    oTpv->iva4 = oTpv->base4 * (oTpv->porc_iva4 /100);

    oTpv->total1 = oTpv->base1 + oTpv->iva1;
    oTpv->total2 = oTpv->base2 + oTpv->iva2;
    oTpv->total3 = oTpv->base3 + oTpv->iva3;
    oTpv->total4 = oTpv->base4 + oTpv->iva4;

    base_ticket = oTpv->base1+oTpv->base2+oTpv->base3+oTpv->base4;

    iva_ticket = oTpv->iva1 + oTpv->iva2 + oTpv->iva3 + oTpv->iva4;
    total_ticket  = base_ticket + iva_ticket;

    ui->lbl_base_ticket->setText(Configuracion_global->toFormatoMoneda(QString::number(base_ticket,'f',
                                                                      Configuracion_global->decimales_campos_totales)));
    ui->lbl_iva_ticket->setText(Configuracion_global->toFormatoMoneda(QString::number(iva_ticket,'f',
                                                                      Configuracion_global->decimales_campos_totales)));
    ui->lbl_total_ticket->setText(Configuracion_global->toFormatoMoneda(QString::number(total_ticket,'f',
                                                                      Configuracion_global->decimales_campos_totales)));
    //--------------------------------
    // LLENAR CAMPOS DESGLOSE
    //--------------------------------
    ui->txtSubtotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->subtotal1,'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
    ui->txtSubtotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->subtotal2,'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
    ui->txtSubtotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->subtotal3,'f',
                                                                                    Configuracion_global->decimales_campos_totales)));
    ui->txtSubtotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->subtotal4,'f',
                                                                                    Configuracion_global->decimales_campos_totales)));

    ui->txtBase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->base1,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtBase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->base2,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtBase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->base3,'f',
                                                                                Configuracion_global->decimales_campos_totales)));
    ui->txtBase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->base4,'f',
                                                                                Configuracion_global->decimales_campos_totales)));

    ui->txtIva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->iva1,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtIva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->iva2,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtIva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->iva3,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtIva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->iva4,'f',
                                                                               Configuracion_global->decimales_campos_totales)));

    ui->txtDto1->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->dto1,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtDto2->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->dto2,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtDto3->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->dto3,'f',
                                                                               Configuracion_global->decimales_campos_totales)));
    ui->txtDto4->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->dto4,'f',
                                                                               Configuracion_global->decimales_campos_totales)));

    ui->txtTotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->total1,'f',
                                                                                 Configuracion_global->decimales_campos_totales)));
    ui->txtTotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->total2,'f',
                                                                                 Configuracion_global->decimales_campos_totales)));
    ui->txtTotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->total3,'f',
                                                                                 Configuracion_global->decimales_campos_totales)));
    ui->txtTotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oTpv->total4,'f',
                                                                                 Configuracion_global->decimales_campos_totales)));

    ui->lblIva1->setText(tr("IVA: %1").arg(QString::number(oTpv->porc_iva1,'f',2)));
    ui->lblIva1_2->setText(tr("IVA: %1").arg(QString::number(oTpv->porc_iva2,'f',2)));
    ui->lblIva1_3->setText(tr("IVA: %1").arg(QString::number(oTpv->porc_iva3,'f',2)));
    ui->lblIva1_4->setText(tr("IVA: %1").arg(QString::number(oTpv->porc_iva4,'f',2)));

    ui->lbldesglose1->setText(tr("Desglose IVA %1").arg(QString::number(oTpv->porc_iva1,'f',2)));
    ui->lbldesglose2->setText(tr("Desglose IVA %1").arg(QString::number(oTpv->porc_iva2,'f',2)));
    ui->lbldesglose3->setText(tr("Desglose IVA %1").arg(QString::number(oTpv->porc_iva3,'f',2)));
    ui->lbldesglose4->setText(tr("Desglose IVA %1").arg(QString::number(oTpv->porc_iva4,'f',2)));

    //----------------
    // LLENAR CAB_TPV
    //----------------
    QHash <QString,QVariant> cab;
    cab["importe"] = total_ticket;
    cab["base1"] = oTpv->base1;
    cab["base2"] = oTpv->base2;
    cab["base3"] = oTpv->base3;
    cab["base4"] = oTpv->base4;
    cab["iva1"] = oTpv->iva1;
    cab["iva2"] = oTpv->iva2;
    cab["iva3"] = oTpv->iva3;
    cab["iva4"] = oTpv->iva4;
    cab["total1"] = oTpv->total1;
    cab["total2"] = oTpv->total2;
    cab["total3"] = oTpv->total3;
    cab["total4"] = oTpv->total4;
    bool updated = SqlCalls::SqlUpdate(cab,"cab_tpv",Configuracion_global->empresaDB,QString("id=%1").arg(oTpv->id),error);
    if(!updated)
        QMessageBox::warning(this,tr("Gestión de tickets"),tr("Atención: no se ha podido actualizar el ticket: %1").arg(error));
}

void FrmTPV::cargar_ofertas()
{
    model_ofertas->setQuery("select id,descripcion from articulos where mostrar_en_cuadro = 1",Configuracion_global->empresaDB);

}

//void FrmTPV::on_btnClientes_clicked()
//{

//}



//void FrmTPV::on_btnBuscar_clicked()
//{

//}


void FrmTPV::on_txtCodigo_editingFinished()
{
    ui->txtCodigo->blockSignals(true);
    if(!ui->txtCodigo->text().isEmpty())
    {
        bool procesar;
        if(oTpv->fecha == QDate::currentDate())
        {

            procesar = true;

        } else
        {
            if(QMessageBox::question(this,tr("Gestión de TPV"),tr("¿Desea alterar un ticket de otra fecha /caja cerrada?"),
                    tr("No"),tr("Sí")) == QMessageBox::Accepted)
                procesar = true;
            else

            {
                ui->txtCodigo->setText("");
                procesar = false;
            }

        }
        if(procesar)
        {
            //--------------------------------
            //--------------------------------
            // NUEVA LINEA DE VENTA ARTÍCULO
            //--------------------------------
            //................................
            if(ui->btnScanear->isChecked())
            {
                float cantidad;
                cantidad = 1;
                QString error;

                if(Configuracion_global->tpv_forzar_cantidad)
                {
                    frmcantidad cantid(this);
                    cantid.exec();
                    cantidad = cantid.getCantidad();
                }
                QHash <QString, QVariant> art;
                if(oTpv->id_cliente == 0)
                    art = oArticulo->Vender(ui->txtCodigo->text(),cantidad,Configuracion_global->id_tarifa_predeterminada,
                                            0,0,oTpv->id_cliente);
                else{
                    int tarifa = SqlCalls::SelectOneField("clientes","tarifa_cliente",QString("id= %1").arg(oTpv->id_cliente),
                                                          Configuracion_global->groupDB,error).toInt();
                    int grupo_dto = SqlCalls::SelectOneField("clientes","tipo_dto_tarifa",QString("id= %1").arg(oTpv->id_cliente),
                                                             Configuracion_global->groupDB,error).toInt();
                    art = oArticulo->Vender(ui->txtCodigo->text(),cantidad,tarifa,grupo_dto,0,oTpv->id_cliente);
                }
                if(art.value("found").toBool()){
                    QHash <QString,QVariant> lin;

                    lin["id_cab"] = oTpv->id;
                    lin["id_articulo"] = art.value("id").toInt();
                    lin["codigo"] = ui->txtCodigo->text();
                    lin["descripcion"] = art.value("descripcion_reducida").toString();
                    lin["precio"] = art.value("precio").toDouble();
                    lin["cantidad"] = art.value("cantidad").toFloat();
                    double imp = art.value("cantidad").toFloat() * art.value("precio").toDouble();
                    QString s_impu = Configuracion_global->toFormatoMoneda(QString::number(imp,'f',Configuracion_global->decimales_campos_totales));
                    lin["importe"] = Configuracion_global->MonedatoDouble(s_impu);
                    lin["porc_iva"] = art.value("tipo_iva").toFloat();
                    lin["iva"] = lin.value("importe").toDouble() * (lin.value("porc_iva").toFloat()/100);
                    lin["total"] = lin.value("importe").toDouble() + lin.value("iva").toFloat();
                    lin["fecha_linea"] = QDate::currentDate();
                    lin["promocion"] = art.value("promocionado").toBool();
                    int new_id = SqlCalls::SqlInsert(lin,"lin_tpv",Configuracion_global->empresaDB,error);
                    if(new_id <0)
                    {
                        QMessageBox::warning(this,tr("Gestión de tickets"),tr("Ocurrió un error al crear una nueva línea: %1").arg(error));
                    } else{
                        // Si el cliente o el artículo tiene, asignado un descuento creamos la línea.
                        int id = new_id;
                        QHash <QString,QVariant> h;
                        QString error;
                        h["id_cab"]= id;
                        h["descripcion"] = tr("Descuento especial:").arg(ui->txtCodigo->text());
                        h["tipo"] = "p";
                        h["valor"] = art.value("dto").toDouble();
                        // -----------------------------------------------------------------------
                        // Si ya existe una línea de dto la borramos para substituir por la nueva
                        //------------------------------------------------------------------------
                        bool succes = SqlCalls::SqlDelete("lin_tpv_2",Configuracion_global->empresaDB,QString("id_cab=%1").arg(id),error);
                        if(succes){
                            // ------------------
                            // Insertamos línea
                            //-------------------
                            if(h.value("valor").toDouble()>0){
                                int new_id = SqlCalls::SqlInsert(h,"lin_tpv_2",Configuracion_global->empresaDB,error);
                                if(new_id == -1)
                                    QMessageBox::warning(this,tr("Gestión de TPV"),tr("Ocurrió un error al insertar linea descuento"),tr("Aceptar"));

                            }
                        }
                        ui->txtCodigo->clear();
                        cargar_lineas(this->id);

                    }
                } else {
                    TimedMessageBox * t = new TimedMessageBox(qApp->activeWindow(),"No se encuentra el artículo");
                }


                ui->txtCodigo->setText("");
                ui->txtCodigo->setFocus();
            }
            //-----------------------
            // APLICAMOS DTO
            //-----------------------
            if(ui->btnDto->isChecked())
            {
               ui->btnDto->setChecked(false);
               int id = ui->tablaLineas_tiquet_actual->item(ui->tablaLineas_tiquet_actual->currentRow(),0)->text().toInt();
               QHash <QString,QVariant> h;
               QString error;
               h["id_cab"]= id;
               h["descripcion"] = tr("Descuento manual:").arg(ui->txtCodigo->text());
               h["tipo"] = "p";
               h["valor"] = ui->txtCodigo->text().toDouble();
               // -----------------------------------------------------------------------
               // Si ya existe una línea de dto la borramos para substituir por la nueva
               //------------------------------------------------------------------------
               bool succes = SqlCalls::SqlDelete("lin_tpv_2",Configuracion_global->empresaDB,QString("id_cab=%1").arg(id),error);
               if(succes){
                   // ------------------
                   // Insertamos línea
                   //-------------------
                   int new_id = SqlCalls::SqlInsert(h,"lin_tpv_2",Configuracion_global->empresaDB,error);
                   if(new_id == -1)
                       QMessageBox::warning(this,tr("Gestión de TPV"),tr("Ocurrió un error al insertar linea descuento"),tr("Aceptar"));
               }
               ui->txtCodigo->clear();
               cargar_lineas(this->id);


            } else if(ui->btnCantidad->isChecked())
            {
                //-----------------------------
                // CAMBIAMOS CANTIDAD
                //-----------------------------
                int id = ui->tablaLineas_tiquet_actual->item(ui->tablaLineas_tiquet_actual->currentRow(),0)->text().toInt();
                double importe = Configuracion_global->MonedatoDouble(
                            ui->tablaLineas_tiquet_actual->item(ui->tablaLineas_tiquet_actual->currentRow(),3)->text());
                QHash <QString,QVariant> h;
                h["cantidad"] = ui->txtCodigo->text().toDouble();
                h["importe"] = ui->txtCodigo->text().toDouble() * importe;
                QString error;
                bool updated = SqlCalls::SqlUpdate(h,"lin_tpv",Configuracion_global->empresaDB,QString("id=%1").arg(id),error);
                if(!updated)
                    QMessageBox::warning(this,tr("Gestión TPV"),tr("Ocurrió un error al actualizar: %1").arg(error));


                cargar_lineas(this->id);
            } else if(ui->btnPrecio->isChecked())
            {
                //---------------------
                // CAMBIAMOS PRECIO
                //---------------------
                int id = ui->tablaLineas_tiquet_actual->item(ui->tablaLineas_tiquet_actual->currentRow(),0)->text().toInt();
                double cantidad = ui->tablaLineas_tiquet_actual->item(ui->tablaLineas_tiquet_actual->currentRow(),2)->text().toDouble();
                QHash <QString,QVariant> h;
                h["precio"] = ui->txtCodigo->text().toDouble();
                h["importe"] = ui->txtCodigo->text().toDouble() * cantidad;
                QString error;
                bool updated = SqlCalls::SqlUpdate(h,"lin_tpv",Configuracion_global->empresaDB,QString("id=%1").arg(id),error);
                if(!updated)
                    QMessageBox::warning(this,tr("Gestión TPV"),tr("Ocurrió un error al actualizar: %1").arg(error));

                if(ui->frame_ticket->currentIndex() == 0)
                {
                    cargar_lineas(this->id);
                }
            }
        }
    }
    ui->txtCodigo->blockSignals(false);
}

void FrmTPV::on_btnScanear_toggled(bool checked)
{
    if(checked)
    {
        ui->txtCodigo->clear();
        ui->txtCodigo->setEnabled(true);
        ui->btnCalculadora->setChecked(false);
        ui->btnCantidad->setChecked(false);
        ui->btnPrecio->setChecked(false);
        ui->btnDto->setChecked(false);
        ui->btnBuscarArt_ext->setVisible(true);
        ui->txtCodigo->setFocus();
        ui->txtCodigo->setStyleSheet("background-color: rgb(0,200,0); font: 12pt 'Sans Serif';");
    } else
        ui->btnBuscarArt_ext->setVisible(false);
        ui->txtCodigo->setStyleSheet("background-color:rgb(255, 255, 191); font: 12pt 'Sans Serif';");

}


void FrmTPV::on_btnbotones_clicked()
{
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
    animationOp->setEasingCurve(QEasingCurve::Linear);
    animationOp->setDuration(300);
    animationOp->setStartValue(1.0);
    animationOp->setEndValue(0.0);
    connect(animationOp,SIGNAL(finished()),this,SLOT(fin_fade_buttons()));
    animationOp->start();


}

void FrmTPV::on_btnticket_clicked()
{
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
    animationOp->setEasingCurve(QEasingCurve::Linear);
    animationOp->setDuration(300);
    animationOp->setStartValue(1.0);
    animationOp->setEndValue(0.0);
    connect(animationOp,SIGNAL(finished()),this,SLOT(fin_fade_ticket()));
    animationOp->start();
}

void FrmTPV::fin_fade_ticket()
{
    ui->frame_ticket->setCurrentIndex(0);
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp2 = new QPropertyAnimation(fade_effect, "opacity");
    animationOp2->setEasingCurve(QEasingCurve::Linear);
    animationOp2->setDuration(500);
    animationOp2->setStartValue(0.0);
    animationOp2->setEndValue(1.0);
    animationOp2->start();

}

void FrmTPV::fin_fade_ticket2()
{
    ui->frame_ticket->setCurrentIndex(1);
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp2 = new QPropertyAnimation(fade_effect, "opacity");
    animationOp2->setEasingCurve(QEasingCurve::Linear);
    animationOp2->setDuration(300);
    animationOp2->setStartValue(0.0);
    animationOp2->setEndValue(1.0);
    animationOp2->start();
}
void FrmTPV::fin_fade_buttons()
{
    ui->frame_ticket->setCurrentIndex(2);
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp2 = new QPropertyAnimation(fade_effect, "opacity");
    animationOp2->setEasingCurve(QEasingCurve::Linear);
    animationOp2->setDuration(300);
    animationOp2->setStartValue(0.0);
    animationOp2->setEndValue(1.0);
    animationOp2->start();

}

void FrmTPV::on_btnlista_clicked()
{
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp = new QPropertyAnimation(fade_effect, "opacity");
    animationOp->setEasingCurve(QEasingCurve::Linear);
    animationOp->setDuration(500);
    animationOp->setStartValue(1.0);
    animationOp->setEndValue(0.0);
    connect(animationOp,SIGNAL(finished()),this,SLOT(fin_fade_lista()));
    animationOp->start();
}
void FrmTPV::fin_fade_lista()
{
    ui->frame_ticket->setCurrentIndex(1);
    QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(this);
    //this->setGraphicsEffect(fade_effect);
    ui->frame_ticket->setGraphicsEffect(fade_effect);
    QPropertyAnimation *animationOp2 = new QPropertyAnimation(fade_effect, "opacity");
    animationOp2->setEasingCurve(QEasingCurve::Linear);
    animationOp2->setDuration(500);
    animationOp2->setStartValue(0.0);
    animationOp2->setEndValue(1.0);
    animationOp2->start();
    model_lista_tpv->setQuery(QString("select id,serie, ticket, caja,importe, fecha,hora,nombre_cliente "
                                      "from cab_tpv where ejercicio = %1 order by ticket desc").arg(
                           Configuracion_global->cEjercicio),Configuracion_global->empresaDB);
}

void FrmTPV::on_btn1_clicked()
{
    if(ui->frame_ticket->currentIndex()==3)
    {
        if(ui->btnEfectivo->isChecked()){
            if(ui->txtEfectivo->text() == "0,00")
                ui->txtEfectivo->setText("1");
            else
                ui->txtEfectivo->setText(ui->txtEfectivo->text().append("1"));
        }
        if(ui->btnTarjeta->isChecked())
        {
            if(ui->txtTarjeta->text() == "0,00")
                ui->txtTarjeta->setText("1");
            else
                ui->txtTarjeta->setText(ui->txtTarjeta->text().append("1"));
        }
        if(ui->btnCheque->isChecked())
        {
            if(ui->txtCheque->text() =="0,00")
                ui->txtCheque->setText("1");
           else
                ui->txtCheque->setText(ui->txtCheque->text().append("1"));
        }
        if(ui->btnDomiciliacion->isChecked())
        {
            if(ui->txtDomiciliacion->text() =="0,00")
                ui->txtDomiciliacion->setText("1");
            else
                ui->txtDomiciliacion->setText(ui->txtDomiciliacion->text().append("1"));
        }
        if(ui->btnTransferencia->isChecked())
        {
            if(ui->txtTransferencia->text() == "0,00")
                ui->txtTransferencia->setText("1");
            else
                ui->txtTransferencia->setText(ui->txtTransferencia->text().append("1"));
        }
        if(ui->btnVales->isChecked())
        {
            if(ui->txtVales->text() == "0,00")
                ui->txtVales->setText("1");
            else
                ui->txtVales->setText(ui->txtVales->text().append("1"));
        }
        if(ui->btnInternet->isChecked())
        {
            if(ui->txtInternet->text() == "0,00")
                ui->txtInternet->setText("1");
            else
                ui->txtInternet->setText(ui->txtInternet->text().append("1"));
        }
    } else
        ui->txtCodigo->setText(ui->txtCodigo->text().append("1"));
}

void FrmTPV::on_btnBack_clicked()
{
    ui->txtCodigo->setText(ui->txtCodigo->text().left(ui->txtCodigo->text().length()-1));
}

void FrmTPV::on_btn2_clicked()
{
    if(ui->frame_ticket->currentIndex()==3)
    {
        if(ui->btnEfectivo->isChecked())
        {
            if(ui->txtEfectivo->text() =="0,00")
                ui->txtEfectivo->setText("2");
            else
                ui->txtEfectivo->setText(ui->txtEfectivo->text().append("2"));
        }
        if(ui->btnTarjeta->isChecked())
        {
            if(ui->txtTarjeta->text() == "0,00")
                ui->txtTarjeta->setText("2");
            else
                ui->txtTarjeta->setText(ui->txtTarjeta->text().append("2"));
        }
        if(ui->btnCheque->isChecked())
        {
            if(ui->txtCheque->text() =="0,00")
                ui->txtCheque->setText("2");
            else
                ui->txtCheque->setText(ui->txtCheque->text().append("2"));
        }
        if(ui->btnDomiciliacion->isChecked())
        {
            if(ui->txtDomiciliacion->text() == "0,00")
                ui->txtDomiciliacion->setText("2");
            else
                ui->txtDomiciliacion->setText(ui->txtDomiciliacion->text().append("2"));
        }
        if(ui->btnTransferencia->isChecked())
        {
            if(ui->txtTransferencia->text() == "0,00")
                ui->txtTransferencia->setText("2");
            else
                ui->txtTransferencia->setText(ui->txtTransferencia->text().append("2"));
        }
        if(ui->btnVales->isChecked())
        {
            if(ui->txtVales->text() == " 0,00")
                ui->txtVales->setText("2");
            else
                ui->txtVales->setText(ui->txtVales->text().append("2"));
        }

        if(ui->btnInternet->isChecked())
        {
            if(ui->txtInternet->text() == "0,00")
                ui->txtInternet->setText("2");
           else
                ui->txtInternet->setText(ui->txtInternet->text().append("2"));
        }
    } else
        ui->txtCodigo->setText(ui->txtCodigo->text().append("2"));
}

void FrmTPV::on_btn3_clicked()
{
    if(ui->frame_ticket->currentIndex()==3)
    {
        if(ui->btnEfectivo->isChecked())
            if(ui->txtEfectivo->text() == "0,00")
            {
                ui->txtEfectivo->setText("3");
            } else
            {
                ui->txtEfectivo->setText(ui->txtEfectivo->text().append("3"));
            }

        if(ui->btnTarjeta->isChecked())
        {
            if(ui->txtTarjeta->text() == "0,00")
                ui->txtTarjeta->setText("3");
             else
                ui->txtTarjeta->setText(ui->txtTarjeta->text().append("3"));
        }
        if(ui->btnCheque->isChecked())
        {
            if(ui->txtCheque->text() == "0,00")
            {
                ui->txtCheque->setText("3");
            } else
                ui->txtCheque->setText(ui->txtCheque->text().append("3"));
        }
        if(ui->btnDomiciliacion->isChecked())
        {
            if(ui->txtDomiciliacion->text() == "0,00")
                ui->txtDomiciliacion->setText("3");
            else
                ui->txtDomiciliacion->setText(ui->txtDomiciliacion->text().append("3"));
        }
        if(ui->btnTransferencia->isChecked())
        {
            if(ui->txtTransferencia->text() == "0,00")
                ui->txtTransferencia->setText("3");
            else
                ui->txtTransferencia->setText(ui->txtTransferencia->text().append("3"));
        }
        if(ui->btnVales->isChecked())
        {
            if(ui->txtVales->text()== "0,00")
                ui->txtVales->setText("3");
            else
                ui->txtVales->setText(ui->txtVales->text().append("3"));
        }
        if(ui->btnInternet->isChecked())
        {
            if(ui->txtInternet->text()== "0,00")
                ui->txtInternet->setText("3");
            else
                ui->txtInternet->setText(ui->txtInternet->text().append("3"));
        }
    } else
        ui->txtCodigo->setText(ui->txtCodigo->text().append("3"));
}

void FrmTPV::on_btn4_clicked()
{
    if(ui->frame_ticket->currentIndex()==3)
    {
        if(ui->btnEfectivo->isChecked()){
            if(ui->txtEfectivo->text() == "0,00")
                ui->txtEfectivo->setText("4");
            else
                ui->txtEfectivo->setText(ui->txtEfectivo->text().append("4"));
        }
        if(ui->btnTarjeta->isChecked())
        {
            if(ui->txtTarjeta->text() == "0,00")
                ui->txtTarjeta->setText("4");
            else
                ui->txtTarjeta->setText(ui->txtTarjeta->text().append("4"));
        }
        if(ui->btnCheque->isChecked())
        {
            if(ui->txtCheque->text() == "0,00")
                ui->txtCheque->setText("4");
            else
                ui->txtCheque->setText(ui->txtCheque->text().append("4"));
        }
        if(ui->btnDomiciliacion->isChecked())
        {
            if(ui->txtDomiciliacion->text() == "0,00")
                ui->txtDomiciliacion->setText("4");
            else
                ui->txtDomiciliacion->setText(ui->txtDomiciliacion->text().append(""));
        }
        if(ui->btnTransferencia->isChecked())
        {
            if(ui->txtTransferencia->text() == "0,00")
                ui->txtTransferencia->setText("4");
            else
                ui->txtTransferencia->setText(ui->txtTransferencia->text().append("4"));
        }

        if(ui->btnVales->isChecked())
        {
            if(ui->txtVales->text() =="0,00")
                ui->txtVales->setText("4");
            else
                ui->txtVales->setText(ui->txtVales->text().append("4"));
        }
        if(ui->btnInternet->isChecked())
        {
            if(ui->txtInternet->text() == "0,00")
                ui->txtInternet->setText("4");
            else
                ui->txtInternet->setText(ui->txtInternet->text().append("4"));
         }
    } else
        ui->txtCodigo->setText(ui->txtCodigo->text().append("4"));
}

void FrmTPV::on_btn5_clicked()
{
    if(ui->frame_ticket->currentIndex()==3)
    {
        if(ui->btnEfectivo->isChecked())
        {
            if(ui->txtEfectivo->text() == "0,00")
                ui->txtEfectivo->setText("5");
            else
                ui->txtEfectivo->setText(ui->txtEfectivo->text().append("5"));
        }
        if(ui->btnTarjeta->isChecked())
        {
            if(ui->txtTarjeta->text() == "0,00")
                ui->txtTarjeta->setText("5");
            else
                 ui->txtTarjeta->setText(ui->txtTarjeta->text().append("5"));
        }
        if(ui->btnCheque->isChecked())
        {
            if(ui->txtCheque->text() == "0,00")
                ui->txtCheque->setText("5");
            else
                ui->txtCheque->setText(ui->txtCheque->text().append("5"));
        }
        if(ui->btnDomiciliacion->isChecked())
        {
            if(ui->txtDomiciliacion->text() == "0,00")
                ui->txtDomiciliacion->setText("5");
            else
                ui->txtDomiciliacion->setText(ui->txtDomiciliacion->text().append("5"));
        }
        if(ui->btnTransferencia->isChecked())
        {
            if(ui->txtTransferencia->text() == "0,00")
                ui->txtTransferencia->setText("5");
            else
                ui->txtTransferencia->setText(ui->txtTransferencia->text().append("5"));
        }

        if(ui->btnVales->isChecked())
        {
            if(ui->txtVales->text() == "0,00")
                ui->txtVales->setText("5");
            else
                ui->txtVales->setText(ui->txtVales->text().append("5"));
        }

        if(ui->btnInternet->isChecked())
        {
            if(ui->txtInternet->text() == "0,00")
                ui->txtInternet->setText("5");
            else
                ui->txtInternet->setText(ui->txtInternet->text().append("5"));
        }
    } else
        ui->txtCodigo->setText(ui->txtCodigo->text().append("5"));
}

void FrmTPV::on_btn6_clicked()
{
    if(ui->frame_ticket->currentIndex()==3)
    {
        if(ui->btnEfectivo->isChecked())
        {
            if(ui->txtEfectivo->text() == "0,00")
                ui->txtEfectivo->setText("6");
            else
                ui->txtEfectivo->setText(ui->txtEfectivo->text().append("6"));
        }

        if(ui->btnTarjeta->isChecked())
        {
            if(ui->txtTarjeta->text() == "0,00")
                ui->txtTarjeta->setText("6");
            else
                ui->txtTarjeta->setText(ui->txtTarjeta->text().append("6"));
        }

        if(ui->btnCheque->isChecked()){
            if(ui->txtCheque->text()=="0,00")
                ui->txtCheque->setText("6");
            else
                ui->txtCheque->setText(ui->txtCheque->text().append("6"));
        }

        if(ui->btnDomiciliacion->isChecked()){
            if(ui->txtDomiciliacion->text() =="0,00")
                ui->txtDomiciliacion->setText("6");
            else
                ui->txtDomiciliacion->setText(ui->txtDomiciliacion->text().append("6"));
        }
        if(ui->btnTransferencia->isChecked())
        {
            if(ui->txtTransferencia->text() =="0,00")
                ui->txtTransferencia->setText("6");
            else
                ui->txtTransferencia->setText(ui->txtTransferencia->text().append("6"));
        }

        if(ui->btnVales->isChecked())
        {
            if(ui->txtVales->text() =="0,00")
                ui->txtVales->setText("6");
            else
                ui->txtVales->setText(ui->txtVales->text().append("6"));
        }

        if(ui->btnInternet->isChecked())
        {
            if(ui->txtInternet->text() =="0,00")
                ui->txtInternet->setText("6");
            else
                ui->txtInternet->setText(ui->txtInternet->text().append("6"));
        }
    } else
        ui->txtCodigo->setText(ui->txtCodigo->text().append("6"));
}

void FrmTPV::on_btn7_clicked()
{
    if(ui->frame_ticket->currentIndex()==3)
    {
        if(ui->btnEfectivo->isChecked()){
            if(ui->txtEfectivo->text() =="0,00")
                ui->txtEfectivo->setText("7");
            else
                ui->txtEfectivo->setText(ui->txtEfectivo->text().append("7"));
        }
        if(ui->btnTarjeta->isChecked())
        {
            if(ui->txtTarjeta->text() =="0,00")
                ui->txtTarjeta->setText("7");
            else
                ui->txtTarjeta->setText(ui->txtTarjeta->text().append("7"));
        }

        if(ui->btnCheque->isChecked())
        {
            if(ui->txtCheque->text() =="0,00")
                ui->txtCheque->setText("7");
            else
                ui->txtCheque->setText(ui->txtCheque->text().append("7"));
        }

        if(ui->btnDomiciliacion->isChecked())
        {
            if(ui->txtDomiciliacion->text() =="0,00")
                ui->txtDomiciliacion->setText("7");
            else
                ui->txtDomiciliacion->setText(ui->txtDomiciliacion->text().append("7"));
        }

        if(ui->btnTransferencia->isChecked())
        {
            if(ui->txtTransferencia->text() =="0,00")
                ui->txtTransferencia->setText("7");
            else
                ui->txtTransferencia->setText(ui->txtTransferencia->text().append("7"));
        }
        if(ui->btnVales->isChecked())
        {
            if(ui->txtVales->text() =="0,00")
                ui->txtVales->setText("7");
            else
                ui->txtVales->setText(ui->txtVales->text().append("7"));
        }

        if(ui->btnInternet->isChecked())
        {
            if(ui->txtInternet->text() =="0,00")
                ui->txtInternet->setText("7");
            else
                ui->txtInternet->setText(ui->txtInternet->text().append("7"));
        }
    } else
        ui->txtCodigo->setText(ui->txtCodigo->text().append("7"));
}

void FrmTPV::on_btn8_clicked()
{
    if(ui->frame_ticket->currentIndex()==3)
    {
        if(ui->btnEfectivo->isChecked())
        {
            if(ui->txtEfectivo->text() =="0,00")
                ui->txtEfectivo->setText("8");
            else
                ui->txtEfectivo->setText(ui->txtEfectivo->text().append("8"));
        }

        if(ui->btnTarjeta->isChecked())
        {
            if(ui->txtTarjeta->text() =="0,00")
                ui->txtTarjeta->setText("8");
            else
                ui->txtTarjeta->setText(ui->txtTarjeta->text().append("8"));
        }

        if(ui->btnCheque->isChecked())
        {
            if(ui->txtCheque->text() =="0,00")
                ui->txtCheque->setText("8");
            else
                ui->txtCheque->setText(ui->txtCheque->text().append("8"));
        }

        if(ui->btnDomiciliacion->isChecked())
        {
            if(ui->txtDomiciliacion->text() =="0,00")
                ui->txtDomiciliacion->setText("8");
            else
                ui->txtDomiciliacion->setText(ui->txtDomiciliacion->text().append("8"));
        }

        if(ui->btnTransferencia->isChecked())
        {
            if(ui->txtTransferencia->text() =="0,00")
                ui->txtTransferencia->setText("8");
            else
                ui->txtTransferencia->setText(ui->txtTransferencia->text().append("8"));
        }
        if(ui->btnVales->isChecked())
        {
            if(ui->txtVales->text() =="0,00")
                ui->txtVales->setText("8");
            else
                ui->txtVales->setText(ui->txtVales->text().append("8"));
        }

        if(ui->btnInternet->isChecked())
        {
            if(ui->txtInternet->text() =="0,00")
                ui->txtInternet->setText("8");
            else
                ui->txtInternet->setText(ui->txtInternet->text().append("8"));
        }
    } else
        ui->txtCodigo->setText(ui->txtCodigo->text().append("8"));
}

void FrmTPV::on_btn9_clicked()
{
    if(ui->frame_ticket->currentIndex()==3)
    {
        if(ui->btnEfectivo->isChecked())
        {
            if(ui->txtEfectivo->text() =="0,00")
                ui->txtEfectivo->setText("9");
            else
                ui->txtEfectivo->setText(ui->txtEfectivo->text().append("9"));
        }
        if(ui->btnTarjeta->isChecked())
        {
            if(ui->txtTarjeta->text() =="0,00")
                ui->txtTarjeta->setText("9");
            else
                ui->txtTarjeta->setText(ui->txtTarjeta->text().append("9"));

        if(ui->btnCheque->isChecked())
        {
            if(ui->txtCheque->text() =="0,00")
                ui->txtCheque->setText("9");
            else
                ui->txtCheque->setText(ui->txtCheque->text().append("9"));
        }
        if(ui->btnDomiciliacion->isChecked())
        {
            if(ui->txtDomiciliacion->text() =="0,00")
                ui->txtDomiciliacion->setText("9");
            else
                ui->txtDomiciliacion->setText(ui->txtDomiciliacion->text().append("9"));
        }
        if(ui->btnTransferencia->isChecked())
        {
            if(ui->txtTransferencia->text() == "0,00")
                ui->txtTransferencia->setText("9");
            else
                ui->txtTransferencia->setText(ui->txtTransferencia->text().append("9"));
        }
        if(ui->btnVales->isChecked())
        {
            if(ui->txtVales->text() =="0,00")
                ui->txtVales->setText("9");
            else
                ui->txtVales->setText(ui->txtVales->text().append("9"));
        }

        if(ui->btnInternet->isChecked())
            if(ui->txtInternet->text() =="0,00")
                ui->txtInternet->setText("9");
            else
                ui->txtInternet->setText(ui->txtInternet->text().append("9"));
        }
    } else
        ui->txtCodigo->setText(ui->txtCodigo->text().append("9"));
}

void FrmTPV::on_btnIntro_clicked()
{
    qreal resultado;
    if (ui->btnCalculadora->isChecked())
    {
        if(this->valor1 != 0 && !this->symbol.isEmpty())
            this->valor2 = ui->txtCodigo->text().toDouble();
            if(this->symbol == "+")
                resultado = valor1 + valor2;
            if(this->symbol == "-")
                resultado = valor1 - valor2;
            if(this->symbol == "*")
                resultado = valor1 * valor2;
            if(this->symbol == "/")
                resultado = valor1 / valor2;
            if(this->symbol == "%")
                resultado = valor1 * (valor2/100);

            ui->txtCodigo->setText(Configuracion_global->toFormatoMoneda(QString::number(resultado,'f',
                                                                  Configuracion_global->decimales_campos_totales)));

    } else if(!ui->frame_ticket->currentIndex()==3) // modo teclado
    {
        on_txtCodigo_editingFinished();
        ui->txtCodigo->clear();
    } else if(ui->frame_ticket->currentIndex() == 3) // modo cobro ticket
    {
        calcularcambio();
    }

    if(ui->btnScanear->isChecked()){
        QMap <int,QSqlRecord> art;
        QString error,clausula;
        clausula = QString("codigo ='%1'").arg(ui->txtCodigo->text());
        art = SqlCalls::SelectRecord("vistaart_tarifa",clausula,Configuracion_global->groupDB,error);
        int id_articulo = 0;
        QMapIterator <int,QSqlRecord> iter(art);
        while(iter.hasNext()){
            iter.next();
            id_articulo = iter.value().value("id").toInt();
        }

        if(id_articulo > 0)
        {
            //-----------------------
            // Inserto lineas venta
            //-----------------------
            // CAPTURO DATOS
            //-----------------------

            QString codigo,descripcion;
            double precio;
            codigo = art.value(id_articulo).value("codigo").toString();
            descripcion = art.value(id_articulo).value("descripcion_reducida").toString();

            if(this->tipo_dto_tarifa == 1)
                precio = art.value(id_articulo).value("pvp").toDouble()-(art.value(id_articulo).value("pvp").toDouble()
                                                                         *(art.value(id_articulo).value("porc_dto1").toDouble()/100));
            else if (this->tipo_dto_tarifa == 2)
                precio = art.value(id_articulo).value("pvp").toDouble()-(art.value(id_articulo).value("pvp").toDouble()
                                                                         *(art.value(id_articulo).value("porc_dto2").toDouble()/100));
            else if (this->tipo_dto_tarifa == 3)
                precio = art.value(id_articulo).value("pvp").toDouble()-(art.value(id_articulo).value("pvp").toDouble()
                                                                         *(art.value(id_articulo).value("porc_dto3").toDouble()/100));
            else if (this->tipo_dto_tarifa == 4)
                precio = art.value(id_articulo).value("pvp").toDouble()-(art.value(id_articulo).value("pvp").toDouble()
                                                                        *(art.value(id_articulo).value("porc_dto4").toDouble()/100));
            else if (this->tipo_dto_tarifa == 5)
                precio = art.value(id_articulo).value("pvp").toDouble()-(art.value(id_articulo).value("pvp").toDouble()
                                                                         *(art.value(id_articulo).value("porc_dto5").toDouble()/100));
            else if (this->tipo_dto_tarifa == 6)
                precio = art.value(id_articulo).value("pvp").toDouble()-(art.value(id_articulo).value("pvp").toDouble()
                                                                         *(art.value(id_articulo).value("porc_dto6").toDouble()/100));
            else
                precio = art.value(id_articulo).value("pvp").toDouble();

            //--------------
            // IVA
            //--------------
            double iva_art = art.value(id_articulo).value("tipo_iva").toDouble();

            //--------------------
            // INSERCIÓN DE DATOS
            //--------------------
            QHash <QString,QVariant> h;
            h["id_cab"] = this->id;
            h["id_articulo"] = id_articulo;
            h["codigo"] = codigo;
            h["descripcion"] = descripcion;
            h["precio"] = precio;
            h["porc_iva"] = iva_art;
            h["cantidad"] = 1;
            h["importe"] = precio;
            h["total"] = precio;

            int new_id = SqlCalls::SqlInsert(h,"lin_tpv",Configuracion_global->empresaDB,error);
            if(new_id == -1)
                QMessageBox::warning(this,tr("Gestión de TPV"),tr("Ocurrió un error al insertar: %1").arg(error),tr("Aceptar"));
            else
            {
                cargar_lineas(this->id);
                ui->tablaLineas_tiquet_actual->selectRow(this->row_tabla);
                ui->txtCodigo->clear();
            }

        } else{
            if(!ui->txtCodigo->text().isEmpty()) {
                QMessageBox::warning(this,tr("Gestión de artículos"),tr("No se encuentra el artículo"),tr("Aceptar"));
                ui->txtCodigo->clear();
            }
        }
        ui->btnScanear->setChecked(true);
        ui->txtCodigo->setFocus();
    }

}

void FrmTPV::on_btnSumar_clicked()
{
    if(ui->btnCalculadora->isChecked())
    {
        this->valor1 = Configuracion_global->MonedatoDouble(ui->txtCodigo->text());
        ui->txtCodigo->clear();
        ui->txtCodigo->setFocus();
        this->symbol = "+";
    } else
    {
        int id = ui->tablaLineas_tiquet_actual->item(ui->tablaLineas_tiquet_actual->currentRow(),0)->text().toInt();
        QString tipo = ui->tablaLineas_tiquet_actual->item(ui->tablaLineas_tiquet_actual->currentRow(),5)->text();
        int cant = ui->tablaLineas_tiquet_actual->item(ui->tablaLineas_tiquet_actual->currentRow(),2)->text().toInt();

        if(id >0)
        {
            if(tipo =="*")
            {
                cant++;
                QHash<QString,QVariant> h;
                QString error;
                h["cantidad"] = cant;

                bool succes = SqlCalls::SqlUpdate(h,"lin_tpv",Configuracion_global->empresaDB,QString("id=%1").arg(id),error);
                if(!succes)
                    QMessageBox::warning(this,tr("Gestión TPV"),tr("Ocurrió un error al incrementar cantidad: %1").arg(error),
                                         tr("Aceptar"));
                else
                    cargar_lineas(this->id);
            }
            cargar_lineas(this->id);
        }
    }
}

void FrmTPV::on_btnRestar_clicked()
{
    this->valor1 = Configuracion_global->MonedatoDouble(ui->txtCodigo->text());
    ui->txtCodigo->clear();
    ui->txtCodigo->setFocus();
    this->symbol = "-";
}

void FrmTPV::on_btnMultiplicar_clicked()
{
    this->valor1 = Configuracion_global->MonedatoDouble(ui->txtCodigo->text());
    ui->txtCodigo->clear();
    ui->txtCodigo->setFocus();
    this->symbol = "*";
}

void FrmTPV::on_btnDividir_clicked()
{
    this->valor1 = Configuracion_global->MonedatoDouble(ui->txtCodigo->text());
    ui->txtCodigo->clear();
    ui->txtCodigo->setFocus();
    this->symbol = "/";
}

void FrmTPV::on_btnPorc_clicked()
{
    this->valor1 = Configuracion_global->MonedatoDouble(ui->txtCodigo->text());
    ui->txtCodigo->setText(0);
    ui->txtCodigo->setFocus();
    this->symbol = "%";
}


void FrmTPV::on_btnCalculadora_clicked(bool checked)
{
    ui->btnPorc->setEnabled(false);
    ui->btnMultiplicar->setEnabled(checked);
    ui->btnDividir->setEnabled(checked);
    ui->btnPrecio->setChecked(!checked);
    ui->btnScanear->setChecked(false);
    ui->txtCodigo->setEnabled(true);
    ui->txtCodigo->clear();
    ui->txtCodigo->setFocus();
}

bool FrmTPV::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() ==Qt::Key_Escape)
            return true;
        if(keyEvent->key() ==Qt::Key_E && ui->frame_ticket->currentIndex() ==3)
        {
            ui->btnEfectivo->setChecked(true);
            on_btnEfectivo_clicked(true);
            ui->txtEfectivo->setSelection(0,100);
            return true;
        }

        if(keyEvent->key() == Qt::Key_T && ui->frame_ticket->currentIndex()==3)
        {
            ui->btnTarjeta->setChecked(true);
            on_btnTarjeta_clicked(true);
            ui->txtTarjeta->setSelection(0,100);
            return true;
        }
        if (keyEvent->key() ==Qt::Key_H && ui->frame_ticket->currentIndex()==3)
        {
            ui->btnCheque->setChecked(true);
            on_btnCheque_clicked(true);
            ui->txtCheque->setSelection(0,100);
            return true;
        }
        if( keyEvent->key() == Qt::Key_D && ui->frame_ticket->currentIndex()==3)
        {
            ui->btnDomiciliacion->setChecked(true);
            on_btnDomiciliacion_clicked(true);
            ui->txtDomiciliacion->setSelection(0,100);
            return  true;
        }
        if( keyEvent->key() == Qt::Key_R && ui->frame_ticket->currentIndex()==3)
        {
            ui->btnTransferencia->setChecked(true);
            on_btnTransferencia_clicked(true);
            ui->txtTransferencia->setSelection(0,100);
            return  true;
        }

        if(keyEvent->key() == Qt::Key_V && ui->frame_ticket->currentIndex() == 3)
        {
            ui->btnVales->setChecked(true);
            on_btnVales_clicked(true);
            ui->txtVales->setSelection(0,100);
            return true;
        }

        if(keyEvent->key() == Qt::Key_I && ui->frame_ticket->currentIndex() ==3)
        {
            ui->btnInternet->setChecked(true);
            on_btnInternet_clicked(true);
            ui->txtInternet->setSelection(0,100);
            return true;
        }

        if(keyEvent->key() == Qt::Key_O && ui->frame_ticket->currentIndex() == 3)
        {
            on_btnContinuarEdicionTicket_clicked();
        }

        if(keyEvent->key() == Qt::Key_B  && ui->frame_ticket->currentIndex() == 3)
        {
            on_btnCobrar__clicked(); // cobrar una vez entrados los datos btnCobrar_
        }

        if(keyEvent->key() == Qt::Key_N && ui->frame_ticket->currentIndex() ==3)
        {
            on_btnCobrar_imprimir_nuevo_clicked();
        }

        if(keyEvent->key() == Qt::Key_A && ui->frame_ticket->currentIndex() == 3)
        {
            on_btnCobrar_imprimir_clicked();
        }

        if(keyEvent->key() == Qt::Key_U && ui->frame_ticket->currentIndex() == 3)
        {
            on_btnCobrar_nuevo_clicked();
        }

        if(keyEvent->key() == Qt::Key_F1)
        {
            if(ui->frame_ticket->currentIndex() == 3)
            {
               on_btnContinuarEdicionTicket_clicked();
            }
            ui->btnCantidad->setChecked(true);
            on_btnCantidad_clicked(true);
            ui->txtCodigo->setFocus();
        }
        if(keyEvent->key() == Qt::Key_F2)
        {
            if(ui->frame_ticket->currentIndex() == 3)
            {
               on_btnContinuarEdicionTicket_clicked();
            }
            ui->btnDto->setChecked(true);
            on_btnDto_clicked(true);
        }
        if(keyEvent->key() == Qt::Key_F3)
        {
            if(ui->frame_ticket->currentIndex() == 3)
            {
               on_btnContinuarEdicionTicket_clicked();
            }
            ui->btnScanear->setChecked(true);
            //on_btnScanear_clicked(true);

        }
        if(keyEvent->key() == Qt::Key_F4)
        {
            if(ui->frame_ticket->currentIndex() == 3)
            {
               on_btnContinuarEdicionTicket_clicked();
            }
            ui->btnPrecio->setChecked(true);
            on_btnPrecio_clicked(true);

        }
        if(keyEvent->key() == Qt::Key_F5)
        {
            if(ui->frame_ticket->currentIndex() == 3)
            {
               on_btnContinuarEdicionTicket_clicked();
            }
            on_btnBuscarArt_clicked();
        }

        if(keyEvent->key() == Qt::Key_F6)
        {
            if(ui->frame_ticket->currentIndex() == 3)
            {
               on_btnContinuarEdicionTicket_clicked();
            }
            on_btnAnadir_ticket_clicked();
        }

        if(keyEvent->key() == Qt::Key_F7)
        {
            if(ui->frame_ticket->currentIndex() == 3)
            {
               on_btnContinuarEdicionTicket_clicked();
            }
            on_btnPoner_en_espera_clicked();
        }

        if(keyEvent->key() == Qt::Key_F8)
        {
            if(ui->frame_ticket->currentIndex() == 3)
            {
               on_btnContinuarEdicionTicket_clicked();
            }
            on_btnCobrar_clicked(); // entrar los datos de cobro btncobrar
        }

        if(keyEvent->key() == Qt::Key_F9)
        {
            if(ui->frame_ticket->currentIndex() == 3)
            {
               on_btnContinuarEdicionTicket_clicked();
            }
            on_btnTraspasar_clicked();
        }

        if(keyEvent->key() == Qt::Key_F10)
        {
            if(ui->frame_ticket->currentIndex() == 3)
            {
               on_btnContinuarEdicionTicket_clicked();
            }
            on_btnAsignarCliente_clicked();
        }

        if(keyEvent->key() == Qt::Key_F11)
        {
            if(ui->frame_ticket->currentIndex() == 3)
            {
               on_btnContinuarEdicionTicket_clicked();
            }
            on_btnBorrar_linea_clicked();
        }

        if(obj == ui->txtCodigo)
            {
            if(keyEvent->key() == Qt::Key_Enter)
                    on_btnIntro_clicked();

            if (ui->btnCalculadora->isChecked())
            {
                if(keyEvent->key() == 42) // *
                {
                    on_btnMultiplicar_clicked();
                    return true;
                }
                if(keyEvent->key() == 47 ) // /
                {
                    on_btnDividir_clicked();
                    return true;
                }
                if(keyEvent->key() == 43) // +
                {
                    on_btnSumar_clicked();
                    return true;
                }
                if(keyEvent->key() == 45) // -
                {
                    on_btnRestar_clicked();
                    return true;
                }
                if(keyEvent->key() == 37) // %
                {
                    on_btnPorc_clicked();
                    return true;
                }
                if(keyEvent->key() == Qt::Key_Enter){
                    on_btnIntro_clicked();
                }
                if(keyEvent->key()==Qt::Key_Back)
                    on_btnBack_clicked();
            }
        }
        return false;
    }
    return false;
}

void FrmTPV::estructura_lista()
{
    // id,serie, ticket, caja, fecha,hora,nombre_cliente,total
    QStringList headers;
    QVariantList Sizes;
    headers << tr("id") <<tr("Serie") <<tr("Ticket") <<tr("Caja") << tr("Total") << tr("Fecha") <<tr("hora") << tr("Cliente");
    Sizes << 0 << 45 << 60 << 45 <<80 << 120 <<50 << 180;
    for(int i = 0; i<headers.size();i++)
    {
        ui->lista_tickets->setColumnWidth(i,Sizes.at(i).toInt());
        model_lista_tpv->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
    ui->lista_tickets->setItemDelegateForColumn(5,new DateDelegate);
    ui->lista_tickets->setItemDelegateForColumn(4, new MonetaryDelegate_totals);
}

void FrmTPV::llenar_campos()
{

}

void FrmTPV::insertar_datos_pago()
{
    //-----------------
    // Cobrar ticket
    //-----------------
    QHash <QString, QVariant> cab;
    QString error;

    cab["importe_efectivo"] = Configuracion_global->MonedatoDouble(ui->txtEfectivo->text());
    cab["importe_tarjeta"] = Configuracion_global->MonedatoDouble(ui->txtTarjeta->text());
    cab["importe_cheque"] = Configuracion_global->MonedatoDouble(ui->txtCheque->text());
    cab["importe_vale"] = Configuracion_global->MonedatoDouble(ui->txtVales->text());
    cab["importe_domiciliacion"] = Configuracion_global->MonedatoDouble(ui->txtDomiciliacion->text());
    cab["importe_transferencia"] = Configuracion_global->MonedatoDouble(ui->txtTransferencia->text());
    cab["importe_cambio"] = Configuracion_global->MonedatoDouble(ui->txtCambio->text());
    cab["importe_pendiente_cobro"] = Configuracion_global->MonedatoDouble(ui->txtPendiente->text());
    cab["cobrado"] = true;

    bool updated = SqlCalls::SqlUpdate(cab,"cab_tpv",Configuracion_global->empresaDB,QString("id=%1").arg(oTpv->id),error);
    if(!updated)
        QMessageBox::warning(this,tr("Gestión de TPV"),tr("Ocurrió un error al cobrar: %1").arg(error));
    else
        TimedMessageBox *t = new TimedMessageBox(this,tr("Se ha cobrado el ticket"));
}

void FrmTPV::imprimir()
{
    FrmDialogoImprimir dlg_print(this);
    //dlg_print.set_email(oCliente3->email);
    dlg_print.set_preview(false);
    if(dlg_print.exec() == dlg_print.Accepted)
    {
      //ui->lblimpreso->setVisible(true);
        int valor = dlg_print.get_option();
        QMap <QString,QString> parametros_sql;
        parametros_sql["Empresa.cab_tpv"] = QString("id = %1").arg(oTpv->id);
        parametros_sql["Empresa.lin_tpv"] = QString("id_cab = %1").arg(oTpv->id);
        QString report;
        if(oTpv->id_cliente >0)
            report = "ticket_";/*+QString::number(->ididioma);*/
        else
            report ="ticket_0";

        QMap <QString,QString> parametros;
        //TODO parametros
        switch (valor) {
        case 1: // Impresora
            Configuracion::ImprimirDirecto(report,parametros_sql,parametros);
            break;
        case 2: // email
            // TODO - enviar pdf por mail
            break;
        case 3: // PDF
            Configuracion::ImprimirPDF(report,parametros_sql,parametros);
            break;
        case 4: //preview
            Configuracion::ImprimirPreview(report,parametros_sql,parametros);
            break;
        default:
            break;
        }

    }
}

bool FrmTPV::comprovar_pago()
{
    if(ui->txtPendiente->text() == "0,00")
        return true;
    else
        return false;
}

void FrmTPV::on_btn0_clicked()
{
    if(ui->frame_ticket->currentIndex()==3)
    {
        if(ui->btnEfectivo->isChecked())
        {
            if(ui->txtEfectivo->text() == "0,00")
                ui->txtEfectivo->setText("0,");
            else
                ui->txtEfectivo->setText(ui->txtEfectivo->text().append("0"));
        }

        if(ui->btnTarjeta->isChecked())
        {
            if(ui->txtTarjeta->text() =="0,00")
                ui->txtTarjeta->setText("0,");
            else
                ui->txtTarjeta->setText(ui->txtTarjeta->text().append("0"));
        }

        if(ui->btnCheque->isChecked())
        {
            if(ui->txtCheque->text() =="0,00")
                ui->txtCheque->setText("0,");
            else
                ui->txtCheque->setText(ui->txtCheque->text().append("0"));
        }
        if(ui->btnDomiciliacion->isChecked())
        {
            if(ui->txtDomiciliacion->text() =="0,00")
                ui->txtDomiciliacion->setText("0,");
            else
                ui->txtDomiciliacion->setText(ui->txtDomiciliacion->text().append("0"));
        }
        if(ui->btnTransferencia->isChecked())
        {
            if(ui->txtTransferencia->text() =="0,00")
                ui->txtTransferencia->setText("0,");
            else
                ui->txtTransferencia->setText(ui->txtTransferencia->text().append("0"));
        }

        if(ui->btnVales->isChecked())
        {
            if(ui->txtVales->text() =="0,00")
                ui->txtVales->setText("0,");
            else
                ui->txtVales->setText(ui->txtVales->text().append("0"));
        }
        if(ui->btnInternet->isChecked())
        {
            if(ui->txtInternet->text() =="0,00")
                ui->txtInternet->setText("0,");
            else
                ui->txtInternet->setText(ui->txtInternet->text().append("0"));
        }
    } else
        ui->txtCodigo->setText(ui->txtCodigo->text().append("0"));
}


void FrmTPV::on_lblDependiente_linkActivated(const QString &link)
{

}

void FrmTPV::on_btnDto_clicked(bool checked)
{
    this->scanning = false;
    if(checked)
   {
        ui->btnScanear->setChecked(false);
        ui->btnCalculadora->setChecked(false);
        ui->btnCantidad->setChecked(false);
        ui->btnPrecio->setChecked(false);
        ui->txtCodigo->setEnabled(true);
        ui->txtCodigo->clear();
        ui->txtCodigo->setFocus();
        ui->txtCodigo->setStyleSheet("color:rgb(255,255,255);background-color: rgb(200,0,0); font: 12pt 'Sans Serif';");
    } else
        ui->txtCodigo->setStyleSheet("color:rgb(0,0,0);background-color:rgb(255, 255, 191); font: 12pt 'Sans Serif';");
}

void FrmTPV::on_btnCantidad_clicked(bool checked)
{
    this->scanning = false;
    if(checked)
    {
        ui->btnScanear->setChecked(false);
        ui->btnCalculadora->setChecked(false);
        ui->btnDto->setChecked(false);
        ui->btnPrecio->setChecked(false);
        ui->txtCodigo->clear();
        ui->txtCodigo->setFocus();
        ui->txtCodigo->setStyleSheet("color:rgb(0,0,100);background-color: rgb(200,250,0); font: 12pt 'Sans Serif';");
    } else
        ui->txtCodigo->setStyleSheet("color:rgb(0,0,0);background-color:rgb(255, 255, 191); font: 12pt 'Sans Serif';");
}


void FrmTPV::on_btnBorrar_linea_clicked()
{
    int id = ui->tablaLineas_tiquet_actual->item(ui->tablaLineas_tiquet_actual->currentRow(),0)->text().toInt();
    QString tipo = ui->tablaLineas_tiquet_actual->item(ui->tablaLineas_tiquet_actual->currentRow(),5)->text();
    if(id >0)
    {
        if(QMessageBox::question(this,tr("Gestión de Tickets"),tr("¿Desea borrar esta línea del ticket?"),tr("no"),
                                 tr("Sí"))==QMessageBox::Accepted)
        {
            bool succes;
            QString error;
            // TODO - Restaurar Stock y acumulados.
            if(tipo == "*")
                succes = SqlCalls::SqlDelete("lin_tpv",Configuracion_global->empresaDB,
                                              QString("id=%1").arg(id),error);
            else
                succes = SqlCalls::SqlDelete("lin_tpv_2",Configuracion_global->empresaDB,
                                              QString("id=%1").arg(id),error);
            if(!succes)
                QMessageBox::warning(this,tr("Gestión de TPV"),tr("Falló el borrado de línea"),tr("Aceptar"));
        }
        cargar_lineas(this->id);
    }
}


void FrmTPV::on_btnPrecio_clicked(bool checked)
{

    if(checked)
   {
        ui->btnScanear->setChecked(false);
        ui->btnCalculadora->setChecked(false);
        ui->btnCantidad->setChecked(false);
        ui->btnDto->setChecked(false);
        ui->txtCodigo->clear();
        ui->txtCodigo->setEnabled(true);
        ui->txtCodigo->setFocus();
        ui->txtCodigo->setStyleSheet("color:rgb(255,255,255);background-color: rgb(0,200,200); font: 12pt 'Sans Serif';");
    } else
        ui->txtCodigo->setStyleSheet("color:rgb(0,0,0);background-color:rgb(255, 255, 191); font: 12pt 'Sans Serif';");
}

void FrmTPV::on_btnCobrar_clicked()
{
    this->ticket_height = ui->frame_ticket->height();
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_ticket, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(ui->frame_ticket->width(),ui->frame_ticket->height()));
    animation->setEndValue(QSize(ui->frame_ticket->width(),0));

    //animation->setEasingCurve(QEasingCurve::OutBounce);
    connect(animation,SIGNAL(finished()),this,SLOT(final_anim_cobro()));
    animation->start();
}
void FrmTPV::final_anim_cobro()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_ticket, "size",this);
    animation->setDuration(800);
    animation->setStartValue(QSize(ui->frame_ticket->width(),0));
    animation->setEndValue(QSize(ui->frame_ticket->width(),this->ticket_height));

    animation->setEasingCurve(QEasingCurve::OutBounce);
    ui->frame_ticket->setCurrentIndex(3);
    animation->start();
    calcularcambio();
    ui->txtEfectivo->setFocus();
}

void FrmTPV::final_anim_teclado()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frameTeclado, "size",this);
    animation->setDuration(800);
    animation->setStartValue(QSize(ui->frameTeclado->width(),0));
    animation->setEndValue(QSize(ui->frameTeclado->width(),this->teclado_height));

    animation->setEasingCurve(QEasingCurve::OutBounce);
    ui->frameTeclado->setCurrentIndex(0);
    animation->start();
}

void FrmTPV::on_btnTeclado_clicked()
{
    this->teclado_height = ui->frameTeclado->height();
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frameTeclado, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(ui->frameTeclado->width(),ui->frameTeclado->height()));
    animation->setEndValue(QSize(ui->frameTeclado->width(),0));

   // animation->setEasingCurve(QEasingCurve::OutBounce);
    connect(animation,SIGNAL(finished()),this,SLOT(final_anim_cobro()));
    animation->start();
}



void FrmTPV::on_txtEfectivo_editingFinished()
{
    ui->txtEfectivo->blockSignals(true);
    ui->txtEfectivo->setText(Configuracion_global->toFormatoMoneda(ui->txtEfectivo->text()));
    ui->txtEfectivo->blockSignals(false);
    calcularcambio();
}

void FrmTPV::on_txtTarjeta_editingFinished()
{
    ui->txtTarjeta->blockSignals(true);
    ui->txtTarjeta->setText(Configuracion_global->toFormatoMoneda(ui->txtTarjeta->text()));
    ui->txtTarjeta->blockSignals(false);
    calcularcambio();
}

void FrmTPV::calcularcambio()
{
    ui->txtEfectivo->setText(Configuracion_global->toFormatoMoneda(ui->txtEfectivo->text()));
    ui->txtTarjeta->setText(Configuracion_global->toFormatoMoneda(ui->txtTarjeta->text()));
    ui->txtCheque->setText(Configuracion_global->toFormatoMoneda(ui->txtCheque->text()));
    ui->txtTransferencia->setText(Configuracion_global->toFormatoMoneda(ui->txtTransferencia->text()));
    ui->txtDomiciliacion->setText(Configuracion_global->toFormatoMoneda(ui->txtDomiciliacion->text()));
    ui->txtVales->setText(Configuracion_global->toFormatoMoneda(ui->txtVales->text()));
    ui->txtInternet->setText(Configuracion_global->toFormatoMoneda(ui->txtInternet->text()));



    double efectivo,tarjeta,cheque,domiciliacion,transferencia,internet,vales,pagado, pendiente,cambio;
    efectivo = Configuracion_global->MonedatoDouble(ui->txtEfectivo->text());
    tarjeta = Configuracion_global->MonedatoDouble(ui->txtTarjeta->text());
    cheque = Configuracion_global->MonedatoDouble(ui->txtCheque->text());
    domiciliacion = Configuracion_global->MonedatoDouble(ui->txtDomiciliacion->text());
    transferencia = Configuracion_global->MonedatoDouble(ui->txtTransferencia->text());
    internet = Configuracion_global->MonedatoDouble(ui->txtInternet->text());
    vales = Configuracion_global->MonedatoDouble(ui->txtVales->text());
    pagado = efectivo + tarjeta + cheque + domiciliacion + transferencia + internet + vales;
    pendiente = Configuracion_global->MonedatoDouble(ui->lbl_total_ticket->text()) - pagado;
    if(pendiente <0)
        pendiente = 0;
    ui->txtPendiente->setText(Configuracion_global->toFormatoMoneda(
                                  QString::number(pendiente,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtPagado->setText(Configuracion_global->toFormatoMoneda(
                               QString::number(pagado,'f',Configuracion_global->decimales_campos_totales)));
    cambio = Configuracion_global->MonedatoDouble(ui->lbl_total_ticket->text()) - pagado;
    cambio = cambio-(cambio*2);
    if(cambio <0)
        cambio = 0;
    ui->txtCambio->setText(Configuracion_global->toFormatoMoneda(QString::number(cambio,'f',
                                                                                 Configuracion_global->decimales_campos_totales)));
}

void FrmTPV::on_txtCheque_editingFinished()
{
    ui->txtCheque->blockSignals(true);
    ui->txtCheque->setText(Configuracion_global->toFormatoMoneda(ui->txtCheque->text()));
    ui->txtCheque->blockSignals(false);
    calcularcambio();
}

void FrmTPV::on_txtDomiciliacion_editingFinished()
{
    ui->txtDomiciliacion->blockSignals(true);
    ui->txtDomiciliacion->setText(Configuracion_global->toFormatoMoneda(ui->txtDomiciliacion->text()));
    ui->txtDomiciliacion->blockSignals(false);
    calcularcambio();
}

void FrmTPV::on_txtTransferencia_editingFinished()
{
    ui->txtTransferencia->blockSignals(true);
    ui->txtTransferencia->setText(Configuracion_global->toFormatoMoneda(ui->txtTransferencia->text()));
    ui->txtTransferencia->blockSignals(false);
    calcularcambio();
}

void FrmTPV::on_txtVales_editingFinished()
{
    ui->txtVales->blockSignals(true);
    ui->txtVales->setText(Configuracion_global->toFormatoMoneda(ui->txtVales->text()));
    ui->txtVales->blockSignals(false);
    calcularcambio();
}

void FrmTPV::on_txtInternet_editingFinished()
{
    ui->txtInternet->blockSignals(true);
    ui->txtInternet->setText(Configuracion_global->toFormatoMoneda(ui->txtInternet->text()));
    ui->txtInternet->blockSignals(false);
    calcularcambio();
}

void FrmTPV::on_btnAnadir_ticket_clicked()
{

    int id_ticket = oTpv->nuevoticket(Configuracion_global->serie,Configuracion_global->caja);
    cargar_ticket( id_ticket);
    ui->frame_ticket->setCurrentIndex(0);

}

void FrmTPV::on_lista_tickets_doubleClicked(const QModelIndex &index)
{
    int id = ui->lista_tickets->model()->data(ui->lista_tickets->model()->index(index.row(),0)).toInt();
    //int id = ui->lista_tickets->model()->data(ui->table2->model()->index(index.row(),0),Qt::EditRole).toInt();
    oTpv->recuperar(id);

}


void FrmTPV::on_btnBuscarArt_clicked()
{
    this->teclado_height = ui->frameTeclado->height();
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frameTeclado, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(ui->frameTeclado->width(),ui->frameTeclado->height()));
    animation->setEndValue(QSize(ui->frameTeclado->width(),0));

   // animation->setEasingCurve(QEasingCurve::OutBounce);
    connect(animation,SIGNAL(finished()),this,SLOT(final_anim_busqueda()));
    animation->start();
    ui->txtbuscar_art->setFocus();

}

void FrmTPV::final_anim_busqueda()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frameTeclado, "size",this);
    animation->setDuration(800);
    animation->setStartValue(QSize(ui->frameTeclado->width(),0));
    animation->setEndValue(QSize(ui->frameTeclado->width(),this->teclado_height));

    animation->setEasingCurve(QEasingCurve::OutBounce);
    ui->frameTeclado->setCurrentIndex(1);
    animation->start();
    calcularcambio();
    ui->txtEfectivo->setSelection(0,100);
    ui->txtEfectivo->setFocus();
    on_txtbuscar_art_textEdited("");
}

void FrmTPV::on_rt_clicked()
{
    this->teclado_height = ui->frameTeclado->height();
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frameTeclado, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(ui->frameTeclado->width(),ui->frameTeclado->height()));
    animation->setEndValue(QSize(ui->frameTeclado->width(),0));

   // animation->setEasingCurve(QEasingCurve::OutBounce);
    connect(animation,SIGNAL(finished()),this,SLOT(final_anim_teclado()));
    animation->start();
}


void FrmTPV::on_btnAbrirCaja_clicked()
{
    this->control_width = ui->frmcontrol->width();
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frmcontrol, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(ui->frmcontrol->width(),ui->frmcontrol->height()));
    animation->setEndValue(QSize(0,ui->frmcontrol->height()));

   // animation->setEasingCurve(QEasingCurve::OutBounce);
    connect(animation,SIGNAL(finished()),this,SLOT(final_anim_abrir_caja()));
    animation->start();
}

void FrmTPV::final_anim_abrir_caja()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frmcontrol, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(0,ui->frmcontrol->height()));
    animation->setEndValue(QSize(520,ui->frmcontrol->height()));
    ui->frmcontrol->setCurrentIndex(1);
    ui->spinHorarioAbertura->setTime(QTime::currentTime());
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}

void FrmTPV::on_btnContinuarEdicionTicket_clicked()
{
    this->ticket_height = ui->frame_ticket->height();
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_ticket, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(ui->frame_ticket->width(),ui->frame_ticket->height()));
    animation->setEndValue(QSize(ui->frame_ticket->width(),0));

    connect(animation,SIGNAL(finished()),this,SLOT(final_anim_edicion_ticket()));
    animation->start();
}

void FrmTPV::final_anim_edicion_ticket()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_ticket, "size",this);
    animation->setDuration(800);
    animation->setStartValue(QSize(ui->frame_ticket->width(),0));
    animation->setEndValue(QSize(ui->frame_ticket->width(),this->ticket_height));

    animation->setEasingCurve(QEasingCurve::OutBounce);
    ui->frame_ticket->setCurrentIndex(0);
    animation->start();
}

void FrmTPV::on_btnEfectivo_clicked(bool checked)
{
    if(checked)
    {
        ui->txtEfectivo->clear();
        ui->btnTarjeta->setChecked(false);
        ui->btnCheque->setChecked(false);
        ui->btnDomiciliacion->setChecked(false);
        ui->btnTransferencia->setChecked(false);
        ui->btnVales->setChecked(false);
        ui->btnInternet->setChecked(false);
        calcularcambio();
        ui->txtEfectivo->setFocus();
    }
}

void FrmTPV::on_btnTarjeta_clicked(bool checked)
{
    if (checked)
    {
        ui->txtTarjeta->clear();
        ui->btnEfectivo->setChecked(false);
        ui->btnCheque->setChecked(false);
        ui->btnDomiciliacion->setChecked(false);
        ui->btnTransferencia->setChecked(false);
        ui->btnVales->setChecked(false);
        ui->btnInternet->setChecked(false);
        calcularcambio();
        ui->txtTarjeta->setText(ui->txtPendiente->text());
        ui->txtTarjeta->setFocus();
    }
}

void FrmTPV::on_btnCheque_clicked(bool checked)
{
    if(checked)
    {
        ui->txtCheque->clear();
        ui->btnTarjeta->setChecked(false);
        ui->btnEfectivo->setChecked(false);
        ui->btnDomiciliacion->setChecked(false);
        ui->btnTransferencia->setChecked(false);
        ui->btnVales->setChecked(false);
        ui->btnInternet->setChecked(false);
        calcularcambio();
        ui->txtCheque->setFocus();
    }
}

void FrmTPV::on_btnDomiciliacion_clicked(bool checked)
{
    if(checked)
    {
        ui->txtDomiciliacion->clear();

        ui->btnTarjeta->setChecked(false);
        ui->btnCheque->setChecked(false);
        ui->btnDomiciliacion->setChecked(false);
        ui->btnVales->setChecked(false);
        ui->btnInternet->setChecked(false);
        calcularcambio();
        ui->txtTransferencia->setFocus();
    }

}
void FrmTPV::on_btnTransferencia_clicked(bool checked)
{
    if(checked)
    {
        ui->txtTransferencia->clear();

        ui->btnTarjeta->setChecked(false);
        ui->btnCheque->setChecked(false);
        ui->btnTransferencia->setChecked(false);
        ui->btnVales->setChecked(false);
        ui->btnInternet->setChecked(false);
        calcularcambio();
        ui->txtTransferencia->setFocus();
    }

}

void FrmTPV::on_btnVales_clicked(bool checked)
{
    if(checked)
    {
        ui->txtVales->clear();
        ui->btnTarjeta->setChecked(false);
        ui->btnCheque->setChecked(false);
        ui->btnDomiciliacion->setChecked(false);

        ui->btnEfectivo->setChecked(false);
        ui->btnInternet->setChecked(false);
        calcularcambio();
        ui->txtVales->setFocus();
    }
}

void FrmTPV::on_btnInternet_clicked(bool checked)
{
    if(checked)
    {
        ui->txtInternet->clear();
        ui->btnTarjeta->setChecked(false);
        ui->btnCheque->setChecked(false);
        ui->btnDomiciliacion->setChecked(false);
        ui->btnTransferencia->setChecked(false);
        ui->btnVales->setChecked(false);
        ui->btnEfectivo->setChecked(false);
        calcularcambio();
        ui->txtInternet->setFocus();
    }
}

void FrmTPV::on_btnPunto_decimal_clicked()
{
    if(ui->frame_ticket->currentIndex()==3)
    {
        if(ui->btnEfectivo->isChecked())
            ui->txtEfectivo->setText(ui->txtEfectivo->text().append(","));
        if(ui->btnTarjeta->isChecked())
            ui->txtTarjeta->setText(ui->txtTarjeta->text().append(","));
        if(ui->btnCheque->isChecked())
            ui->txtCheque->setText(ui->txtCheque->text().append(","));
        if(ui->btnDomiciliacion->isChecked())
            ui->txtDomiciliacion->setText(ui->txtDomiciliacion->text().append(","));
        if(ui->btnTransferencia->isChecked())
            ui->txtTransferencia->setText(ui->txtTransferencia->text().append(","));
        if(ui->btnVales->isChecked())
            ui->txtVales->setText(ui->txtVales->text().append(","));
        if(ui->btnInternet->isChecked())
            ui->txtInternet->setText(ui->txtInternet->text().append(","));
    } else
        ui->txtCodigo->setText(ui->txtCodigo->text().append(","));
}

void FrmTPV::on_btnPoner_en_espera_clicked()
{

}

void FrmTPV::on_btnCobrar__clicked()
{
    if(comprovar_pago())
    {
        //-----------------
        // Insertar pago
        //-----------------
        insertar_datos_pago();
        //-----------
        // ANIMACION
        //-----------
        this->ticket_height = ui->frame_ticket->height();
        QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_ticket, "size",this);
        animation->setDuration(600);
        animation->setStartValue(QSize(ui->frame_ticket->width(),ui->frame_ticket->height()));
        animation->setEndValue(QSize(ui->frame_ticket->width(),0));

        connect(animation,SIGNAL(finished()),this,SLOT(final_anim_edicion_ticket()));
        animation->start();
    } else
    {
        QMessageBox::warning(this,tr("Gestión de Tickets"),tr("No se puede marcar como cobrado un ticket com importe pendiente")+
                             tr("\nEspecifique las formas correctas de cobro"),tr("Aceptar"));
    }


}

void FrmTPV::on_btnCobrar_imprimir_nuevo_clicked()
{
    if(comprovar_pago())
    {
        //-----------------
        // Insertar pago
        //-----------------
        insertar_datos_pago();

        //----------------------
        //  Imprimir ticket
        //----------------------
        imprimir();
        //----------------------
        // Añadir nuevo ticket
        //----------------------
        on_btnAnadir_ticket_clicked();
        //-----------
        // ANIMACION
        //-----------
        this->ticket_height = ui->frame_ticket->height();
        QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_ticket, "size",this);
        animation->setDuration(600);
        animation->setStartValue(QSize(ui->frame_ticket->width(),ui->frame_ticket->height()));
        animation->setEndValue(QSize(ui->frame_ticket->width(),0));

        connect(animation,SIGNAL(finished()),this,SLOT(final_anim_edicion_ticket()));
        animation->start();
    } else
    {
        QMessageBox::warning(this,tr("Gestión de Tickets"),tr("No se puede marcar como cobrado un ticket com importe pendiente")+
                             tr("\nEspecifique las formas correctas de cobro"),tr("Aceptar"));
    }

}

void FrmTPV::on_btnCobrar_imprimir_clicked()
{
    if(comprovar_pago())
    {
        //-----------------
        // Insertar pago
        //-----------------
        insertar_datos_pago();

        //-------------------
        //  Imprimir ticket
        //-------------------
        imprimir();

        //-----------
        // ANIMACION
        //-----------
        this->ticket_height = ui->frame_ticket->height();
        QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_ticket, "size",this);
        animation->setDuration(600);
        animation->setStartValue(QSize(ui->frame_ticket->width(),ui->frame_ticket->height()));
        animation->setEndValue(QSize(ui->frame_ticket->width(),0));

        connect(animation,SIGNAL(finished()),this,SLOT(final_anim_edicion_ticket()));
        animation->start();
    } else
    {
        QMessageBox::warning(this,tr("Gestión de Tickets"),tr("No se puede marcar como cobrado un ticket com importe pendiente")+
                             tr("\nEspecifique las formas correctas de cobro"),tr("Aceptar"));
    }


}

void FrmTPV::on_btnCobrar_nuevo_clicked()
{
    if(comprovar_pago())
    {
        //-----------------
        // Insertar pago
        //-----------------
        insertar_datos_pago();


        //----------------------
        // Añadir nuevo ticket
        //----------------------
        on_btnAnadir_ticket_clicked();


        //-----------
        // ANIMACION
        //-----------
        this->ticket_height = ui->frame_ticket->height();
        QPropertyAnimation *animation = new QPropertyAnimation(ui->frame_ticket, "size",this);
        animation->setDuration(600);
        animation->setStartValue(QSize(ui->frame_ticket->width(),ui->frame_ticket->height()));
        animation->setEndValue(QSize(ui->frame_ticket->width(),0));

        connect(animation,SIGNAL(finished()),this,SLOT(final_anim_edicion_ticket()));
        animation->start();
    } else
    {
        QMessageBox::warning(this,tr("Gestión de Tickets"),tr("No se puede marcar como cobrado un ticket com importe pendiente")+
                             tr("\nEspecifique las formas correctas de cobro"),tr("Aceptar"));
    }

}

void FrmTPV::on_btnTraspasar_clicked()
{

}

void FrmTPV::on_btnAsignarCliente_clicked()
{
    db_consulta_view consulta;
    QStringList campos;
    campos  <<"nombre_fiscal" <<"codigo_cliente" << "cif_nif"<< "poblacion" << "telefono1";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("clientes");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,codigo_cliente,nombre_fiscal,cif_nif,poblacion,telefono1 from clientes");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Código") << tr("Nombre") << tr("CIF/NIF") << tr("Población") << tr("Teléfono");
    tamanos <<"0" << "100" << "300" << "100" << "180" <<"130";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Clientes");
    if(consulta.exec())
    {
        int id = consulta.get_id();
        QString error;
        QHash <QString, QVariant > cli;
        cli["id_cliente"] = id;
        QString nombre_fiscal = SqlCalls::SelectOneField("clientes","nombre_fiscal",QString("id=%1").arg(id),
                                                         Configuracion_global->groupDB,error).toString();
        QString nombre = tr("Cliente: %1").arg(nombre_fiscal);
        if(error.isEmpty())
        {
            cli["nombre_cliente"] = nombre_fiscal;
            bool success = SqlCalls::SqlUpdate(cli,"cab_tpv",Configuracion_global->empresaDB,QString("id =%1").arg(oTpv->id),
                                               error);
            if(success){
                ui->btnAsignarCliente->setText(nombre);
                this->tipo_dto_tarifa = SqlCalls::SelectOneField("clientes","tipo_dto_tarifa",QString("id=%1").arg(oTpv->id_cliente),
                                                                 Configuracion_global->groupDB,error).toInt();
                oTpv->id_cliente = id;
            } else
            {
                QMessageBox::warning(this,tr("Gestión de tickets"),tr("Ocurrió un error al guardar el nombre de cliente: %1").arg(error),
                                     tr("Aceptar"));
            }
        }
        else
            QMessageBox::warning(this,tr("Gestión de tickets"),tr("Ocurrió un error al recuperar el cliente: %1").arg(error),
                                 tr("Aceptar"));

    }

}

void FrmTPV::on_btnCancelar_caja_clicked()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frmcontrol, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(ui->frmcontrol->width(),ui->frmcontrol->height()));
    animation->setEndValue(QSize(0,ui->frmcontrol->height()));

    connect(animation,SIGNAL(finished()),this,SLOT(final_anim_abrir_caja_cancelado()));
    animation->start();
}

void FrmTPV::final_anim_abrir_caja_cancelado()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frmcontrol, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(ui->frmcontrol->width(),ui->frmcontrol->height()));
    animation->setEndValue(QSize(this->control_width,ui->frmcontrol->height()));

    animation->setEasingCurve(QEasingCurve::OutBounce);
    ui->frmcontrol->setCurrentIndex(0);
    animation->start();
}

void FrmTPV::on_btnConfirmarAbertura_caja_clicked()
{
    QHash <QString, QVariant> caja;
    QString error;
    caja["fecha_abertura"] = ui->calendarioAbertura->selectedDate();
    caja["hora_abertura"] = ui->spinHorarioAbertura->text();
    caja["id_usuario"] = Configuracion_global->id_usuario_activo;
    caja["importe_abertura_dia"] = Configuracion_global->MonedatoDouble(ui->txtImporteAbertura->text());
    caja["id_caja"] = SqlCalls::SelectOneField("cajas","id",QString("desc_caja= '%1'").arg(ui->cboCajas->currentText()),
                                               Configuracion_global->empresaDB,error).toInt();
    caja["ejercicio"] = Configuracion_global->cEjercicio;
    QStringList clausulas;
    clausulas << QString("fecha_abertura = %1").arg(ui->calendarioAbertura->selectedDate().toString("yyyyMMdd"))
              << QString("id_caja = %1").arg(caja.value("id_caja").toInt());
    int id_val = SqlCalls::SelectOneField("cierrecaja","id",clausulas,Configuracion_global->empresaDB,error).toInt();
    if(id_val > 0) {
        QMessageBox::warning(this,tr("Gestión de Caja"),tr("La caja ya está abierta para este día"),tr("Aceptar"));
        on_btnCancelar_caja_clicked();
    } else
    {
        int new_id = SqlCalls::SqlInsert(caja,"cierrecaja",Configuracion_global->empresaDB,error);
        if(new_id == -1)
            QMessageBox::warning(this,tr("Gestión de Caja"),tr("Ocurrió un error al abrir_caja : %1").arg(error),
                                 tr("Aceptar"));
        else
        {
            TimedMessageBox * t;
            t = new TimedMessageBox(this,"Se ha abierto la caja.");
            QString error;
            QString frase = tr("Caja: %1, fecha abertura caja: %2").arg(SqlCalls::SelectOneField("cajas","desc_caja",
                                               QString("id = %1").arg(caja.value("id_caja").toInt()),Configuracion_global->empresaDB,
                                                                                                 error).toString(),
                                                              ui->calendarioAbertura->selectedDate().toString("dd/MM/yyyy"));
            ui->lblregistrando->setText(frase);
           on_btnCancelar_caja_clicked();
        }
    }
}

void FrmTPV::on_btnDesglose_clicked()
{
    ui->btnDesglose->setEnabled(false);
    this->control_width = ui->frmcontrol->width();
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frmcontrol, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(ui->frmcontrol->width(),ui->frmcontrol->height()));
    animation->setEndValue(QSize(0,ui->frmcontrol->height()));

   // animation->setEasingCurve(QEasingCurve::OutBounce);
    connect(animation,SIGNAL(finished()),this,SLOT(final_anim_desglose()));
    animation->start();
}

void FrmTPV::final_anim_desglose()
{
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frmcontrol, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(0,ui->frmcontrol->height()));
    animation->setEndValue(QSize(520,ui->frmcontrol->height()));
    ui->frmcontrol->setCurrentIndex(2);
    ui->spinHorarioAbertura->setTime(QTime::currentTime());
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}






void FrmTPV::on_btnOcultar_clicked()
{
    ui->btnDesglose->setEnabled(true);
    QPropertyAnimation *animation = new QPropertyAnimation(ui->frmcontrol, "size",this);
    animation->setDuration(600);
    animation->setStartValue(QSize(ui->frmcontrol->width(),ui->frmcontrol->height()));
    animation->setEndValue(QSize(0,ui->frmcontrol->height()));
    connect(animation,SIGNAL(finished()),this,SLOT(final_anim_abrir_caja_cancelado()));
    animation->start();

}

void FrmTPV::on_txtImporteAbertura_editingFinished()
{
    blockSignals(true);
    ui->txtImporteAbertura->setText(Configuracion_global->toFormatoMoneda(ui->txtImporteAbertura->text()));
    blockSignals(false);

}

void FrmTPV::on_btnCerrarCaja_clicked()
{
        FrmCierreCaja cierre;
    // ----------------
    // Cajas abiertas
    // ----------------
    QMap <int,QSqlRecord> cajas;
    QString error;
    QStringList condiciones;
    condiciones << QString("fecha_abertura = '%1'").arg(QDate::currentDate().toString("yyyyMMdd"));
    condiciones << QString("id_caja = %1").arg(Configuracion_global->caja);
    condiciones << QString("ejercicio = '%1'").arg(Configuracion_global->cEjercicio);
    cajas = SqlCalls::SelectRecord("cierrecaja",condiciones,Configuracion_global->empresaDB,error);
    if(cajas.size() ==1)
    {
        QMapIterator <int, QSqlRecord> it(cajas);
        while (it.hasNext())
        {
            it.next();
            cierre.cargar_datos_caja(it.value().value("id").toInt());

            cierre.exec();
        }
    }
    else if(cajas.size() <1)
    {
        QMessageBox::warning(this,tr("Gestión de TPV"),tr("No hay cajas abiertas en este terminal"),tr("Aceptar"));
    }
    else if(cajas.size()>1)
    {
        frmCajasAbiertas cajas;
        cajas.cargar_datos(Configuracion_global->caja,Configuracion_global->cEjercicio);
        if(cajas.exec() == QDialog::Accepted)
        {
            int id = cajas.id;
           cierre.cargar_datos_caja(id);

            cierre.exec();
        }
    }

}

void FrmTPV::on_btnExtrasCaja_clicked()
{
    frmExtrasCaja extras;
    extras.exec();
}

void FrmTPV::on_btnRetirardinero_clicked()
{
    FrmRetirardeCaja retirar;
    retirar.exec();
}

void FrmTPV::on_btnInsertarDinero_clicked()
{
    FrmInsertarDineroCaja insertar;
    insertar.exec();
}

void FrmTPV::on_btnDevolucionTicket_clicked()
{
    FrmDevolucionTicket devolucion;
    devolucion.exec();
}

void FrmTPV::on_lista_tickets_clicked(const QModelIndex &index)
{
    int id = ui->lista_tickets->model()->data(index.model()->index(index.row(),0)).toInt();
    cargar_ticket(id);
    ui->frame_ticket->setCurrentIndex(0);
   // final_anim_edicion_ticket();
}

void FrmTPV::on_btnImprimir_clicked()
{
    imprimir();
}

void FrmTPV::on_txtbuscar_art_textEdited(const QString &arg1)
{
    QHash <QString, QString> orden;
    orden["Código "] = "codigo";
    orden["Descripción"] = "descripcion";

    QString ord = orden.value(ui->cboBusqueda->currentText());

    QString cSQL = QString(
                "select id,codigo, descripcion, pvp, pvp_con_iva from vistaart_tarifa where %1 like '%%2%' order by %1").arg(ord,arg1);

    model_articulos->setQuery(cSQL,Configuracion_global->groupDB);
}

void FrmTPV::on_tabla_buscar_art_doubleClicked(const QModelIndex &index)
{
    QString codigo = ui->tabla_buscar_art->model()->data(index.model()->index(index.row(),1)).toString();
    ui->txtCodigo->setText(codigo);
    ui->btnScanear->setChecked(true);
    on_txtCodigo_editingFinished();
}

void FrmTPV::on_btnBuscarArt_ext_clicked()
{
    db_consulta_view consulta;
    QStringList campos;
    QString error;
    campos << "descripcion" <<"codigo" <<"codigo_barras" << "codigo_fabricante"  << "coste";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("articulos");
    consulta.set_db("Maya");
    if(oTpv->id_cliente >0){
        consulta.setId_tarifa_cliente(SqlCalls::SelectOneField("clientes","tarifa_cliente",
                                                               QString("id=%1").arg(oTpv->id_cliente),
                                                               Configuracion_global->groupDB,error).toInt());
        consulta.setTipo_dto_tarifa(SqlCalls::SelectOneField("clientes","tipo_dto_tarifa",
                                                             QString("id=%1").arg(oTpv->id_cliente),
                                                             Configuracion_global->groupDB,error).toInt());
    } else {
        consulta.setId_tarifa_cliente(Configuracion_global->id_tarifa_predeterminada);
        consulta.setTipo_dto_tarifa(SqlCalls::SelectOneField("clientes","tipo_dto_tarifa",
                                                             QString("id=%1").arg(oTpv->id_cliente),
                                                             Configuracion_global->groupDB,error).toInt());
    }
    if(this->tipo_dto_tarifa ==1)
        consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                     "porc_dto1/100))) as pvp from vistaart_tarifa");
    else if(this->tipo_dto_tarifa ==2)
        consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                     "porc_dto2/100))) as pvp from vistaart_tarifa");
    else if(this->tipo_dto_tarifa ==3)
        consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                     "porc_dto3/100))) as pvp from vistaart_tarifa");
    else if(this->tipo_dto_tarifa ==4)
        consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                     "porc_dto4/100))) as pvp from vistaart_tarifa");
    else if(this->tipo_dto_tarifa ==5)
        consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                     "porc_dto5/100))) as pvp from vistaart_tarifa");
    else if(this->tipo_dto_tarifa ==6)
        consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,(pvp-(pvp*("
                     "porc_dto6/100))) as pvp from vistaart_tarifa");
    else if(this->tipo_dto_tarifa == 0)
        consulta.set_SQL("select id,codigo,codigo_barras,codigo_fabricante,descripcion,coste,pvp from vistaart_tarifa");
    QStringList cabecera;
    QVariantList tamanos;
    QVariantList moneda;
    cabecera  << tr("id") << tr("Código") << tr("Código Barras") << tr("Referencia") << tr("Descripción") << tr("Coste") <<tr("pvp");
    tamanos <<"0" << "100" << "100" << "100" << "320" <<"130" <<"130";
    moneda <<"5" <<"6";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_delegate_monetary(moneda);
    consulta.set_titulo("Busqueda de Artículos");
    consulta.set_filtro("");
    if(consulta.exec())
    {
        int id = consulta.get_id();
        QString error;
        QString codigo = SqlCalls::SelectOneField("vistaart_tarifa","codigo",QString("id=%1").arg(id),
                                                  Configuracion_global->groupDB,error).toString();
        ui->txtCodigo->setText(codigo);
        ui->btnScanear->setChecked(true);
        on_txtCodigo_editingFinished();
   }
}
