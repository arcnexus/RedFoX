#include "frmdevolucionticket.h"
#include "ui_frmdevolucionticket.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/datedelegate.h"
#include "../TPV/frmcausadevolucion.h"
#include "../TPV/tpv.h"
#include "../Almacen/articulo.h"
#include "../TPV/frmdevolucion2.h"


FrmDevolucionTicket::FrmDevolucionTicket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmDevolucionTicket)
{
    ui->setupUi(this);
    //-------------------
    // Controles
    //-------------------
    ui->txtImporteMax->setText("9.999.999,99");
    ui->txtImportemin->setText("0,00");
    ui->dateDesde->setDate(QDate::currentDate().addYears(-1));
    ui->dateHasta->setDate(QDate::currentDate());
    //-------------------
    // Tabla tickets
    //-------------------
    tickets = new QSqlQueryModel(this);
    QString cSQL = "select id,caja,fecha,hora,serie,ticket,importe from cab_tpv where fecha >'"+
            ui->dateDesde->date().toString("yyyyMMdd")+"' and fecha < '" +
            ui->dateHasta->date().toString("yyyyMMdd")+"' and nombre_cliente like '%"+
            ui->txtCliente->text().trimmed()+"%' and importe > "+
            QString::number(Configuracion_global->MonedatoDouble(ui->txtImportemin->text()),'f',2) +
            " and importe < "+ QString::number(Configuracion_global->MonedatoDouble(ui->txtImporteMax->text()),'f',2);
    qDebug() << cSQL;
    tickets->setQuery(cSQL,Configuracion_global->empresaDB);
    ui->tabla_cabeceras->setModel(tickets);

    QStringList headers;
    QVariantList sizes;
    headers << "id" <<tr("Caja") <<tr("Fecha") << tr("Hora") <<tr("Serie") <<tr("Ticket") <<tr("Total");
    sizes << 0 << 45 << 90  <<45 << 45 << 75 << 80;
    for(int i = 0; i<sizes.size();i++)
    {
        ui->tabla_cabeceras->setColumnWidth(i,sizes.at(i).toInt());
        tickets->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
    ui->tabla_cabeceras->setItemDelegateForColumn(2,new DateDelegate);
    ui->tabla_cabeceras->setItemDelegateForColumn(6,new MonetaryDelegate);
    ui->tabla_cabeceras->selectRow(0);
    //-----------------
    // Conexiones
    //-----------------
    connect(ui->tabla_cabeceras->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(refrescar_lineas(QModelIndex,QModelIndex)));



 }

FrmDevolucionTicket::~FrmDevolucionTicket()
{
    delete ui;
}

int FrmDevolucionTicket::get_id_ticket()
{
    return this->id_ticket;
}

QString FrmDevolucionTicket::get_forma_devolucion()
{
    if(ui->radEfectivo->isChecked())
        return "E";
    if(ui->radTarjeta->isChecked())
        return "T";
    if(ui->radTransferencia->isChecked())
        return "B";
    if(ui->radVales->isChecked())
        return "V";
}

void FrmDevolucionTicket::cargarlineas(int id_cab, int id_cab_old)
{
    QHash <QString,QVariant> val_act;
    QString error;
    val_act["dev_act"] = 0;
    SqlCalls::SqlUpdate(val_act,"lin_tpv",Configuracion_global->empresaDB,QString("id_cab=%1").arg(id_cab_old),error);

    ui->tablaLineas_tiquet_actual->clear();
    ui->tablaLineas_tiquet_actual->setColumnCount(6);
    ui->tablaLineas_tiquet_actual->setRowCount(1);

    QMap <int,QSqlRecord> lineas;
    QMap <int,QSqlRecord> lineas_extra;

    lineas = SqlCalls::SelectRecord("lin_tpv",QString("id_cab=%1").arg(id_cab),Configuracion_global->empresaDB,error);
    QMapIterator <int, QSqlRecord> rec_lin(lineas);

    QColor  rowcolor,row_dev_color;
    QString codigo,descripcion;
    int id;
    float cantidad;
    float porc_iva;
    double precio= 0;
    double importe = 0;
    float devolucion = 0;
    int pos = -1;
    int lin =0;
    //-----------------
    // formato tabla
    //-----------------
    QVariantList sizes;
    sizes << 0 << 370 <<50 <<80 <<80;
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
        devolucion = rec_lin.value().value("devolucion").toFloat();

        if(!error.isEmpty())
            QMessageBox::warning(this,tr("Devolucion"),tr("Ocurrió un error al preparar la devolución: %1").arg(error));
        else
        {

            //--------------------------------
            // Creo línea base en tablewidget
            //--------------------------------

            pos++;
            ui->tablaLineas_tiquet_actual->setRowCount(pos+1);
            lin++;
            row_dev_color.setRgb(255,164,164);
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
            if(devolucion == 0)
                item_col1->setBackgroundColor(rowcolor);
            else
                item_col1->setBackgroundColor(row_dev_color);
            ui->tablaLineas_tiquet_actual->setItem(pos,1,item_col1);


            QString cant = QString::number(cantidad,'f',2);
            QTableWidgetItem *item_col2 = new QTableWidgetItem(cant);
            item_col2->setFlags(item_col2->flags() & (~Qt::ItemIsEditable));
            if(devolucion == 0)
                item_col2->setBackgroundColor(rowcolor);
            else
                item_col2->setBackgroundColor(row_dev_color);
            item_col2->setTextAlignment(Qt::AlignRight);
            ui->tablaLineas_tiquet_actual->setItem(pos,2,item_col2);

            QTableWidgetItem *item_col3 = new QTableWidgetItem(Configuracion_global->toFormatoMoneda(QString::number(precio,'f',
                                                                                             Configuracion_global->decimales)));
            item_col3->setFlags(item_col3->flags() & (~Qt::ItemIsEditable));
            if(devolucion == 0)
                item_col3->setBackgroundColor(rowcolor);
            else
                item_col3->setBackgroundColor(row_dev_color);
            item_col3->setTextAlignment(Qt::AlignRight);
            ui->tablaLineas_tiquet_actual->setItem(pos,3,item_col3);

            QTableWidgetItem *item_col4 = new QTableWidgetItem(Configuracion_global->toFormatoMoneda(QString::number(importe,'f',
                                                                              Configuracion_global->decimales_campos_totales)));
            item_col4->setFlags(item_col4->flags() & (~Qt::ItemIsEditable));
            if(devolucion ==0)
                item_col4->setBackgroundColor(rowcolor);
            else
                item_col4->setBackgroundColor(row_dev_color);
            item_col4->setTextAlignment(Qt::AlignRight);
            ui->tablaLineas_tiquet_actual->setItem(pos,4,item_col4);

            QTableWidgetItem *item_col5 = new QTableWidgetItem("*");
            item_col5->setFlags(item_col5->flags() & (~Qt::ItemIsEditable));
            if(devolucion==0)
                item_col5->setBackgroundColor(rowcolor);
            else
                item_col5->setBackgroundColor(row_dev_color);
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
                    item_s_col2->setFlags(item_s_col2->flags() & (~Qt::ItemIsEditable));
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



                }

            }else
                QMessageBox::warning(this,tr("Gestión de tickets"),
                                     tr("Falló la recuperación de sub-lineas de ticket: %1").arg(error));

        }
        //ui->tablaLineas_tiquet_actual->selectRow(pos);
    }

}

void FrmDevolucionTicket::refrescar_lineas(QModelIndex current, QModelIndex previous)
{
  //  Q_UNUSED (previous);
    int id_cab = ui->tabla_cabeceras->model()->data(current.model()->index(current.row(),0)).toInt();
    int id_cab_old = ui->tabla_cabeceras->model()->data(previous.model()->index(previous.row(),0)).toInt();
    cargarlineas(id_cab,id_cab_old);

}

void FrmDevolucionTicket::on_btnDevolucion_clicked()
{

    bool devolver = false;
    bool isOk = false;

    if(ui->radEfectivo->isChecked())
    {
        if(QMessageBox::question(this,tr("Devolución de tickets"),
                             tr("Ha seleccionado devolución en efectivo, ¿está conforme?"),
                             tr("No"),tr("Sí"))==QMessageBox::Accepted)
            devolver = true;
    }
    if(ui->radTarjeta->isChecked())
    {
        if(QMessageBox::question(this,tr("Devolución de tickets"),
                             tr("Ha seleccionado devolución en tarjeta, ¿está conforme?"),
                             tr("No"),tr("Sí"))==QMessageBox::Accepted)
            devolver = true;
    }
    if(ui->radTransferencia->isChecked())
    {
        if(QMessageBox::question(this,tr("Devolución de tickets"),
                             tr("Ha seleccionado devolución por transferencia, ¿está conforme?"),
                             tr("No"),tr("Sí"))==QMessageBox::Accepted)
            devolver = true;
    }
    if(ui->radVales->isChecked())
    {
        if(QMessageBox::question(this,tr("Devolución de tickets"),
                             tr("Ha seleccionado devolución por vale, ¿está conforme?"),
                             tr("No"),tr("Sí"))==QMessageBox::Accepted)
            devolver = true;
    }
     if(devolver)
     {
         Configuracion_global->empresaDB.transaction();
         Configuracion_global->groupDB.transaction();
         frmCausaDevolucion causa;
         causa.exec();

         QString error;
         QHash <QString,QVariant> h;
         QHash <QString,QVariant> l;
        QItemSelection selection( ui->tablaLineas_tiquet_actual->selectionModel()->selection() );
         QList<int> rows;
         foreach( const QModelIndex & index, selection.indexes() ) {
             if(!rows.contains(index.row()))
                rows.append( index.row() );
         }

        if(rows.count()>0){
            // Crear ticket de devolución.
            tpv oTpv;
            // TODO Cargar caja y serie
            int new_id = oTpv.nuevoticket("A","1");
            double imp =0 ;
            this->id_ticket = new_id;
            for( int i = rows.count() - 1; i >= 0; i --) {
                int id = ui->tablaLineas_tiquet_actual->item(rows.at(i),0)->text().toInt();

                if(new_id > -1)
                {
                    // Capturamos info de artículo a devolver
                    QMap <int,QSqlRecord> lin;
                    lin = SqlCalls::SelectRecord("lin_tpv",QString("id = %1").arg(id),
                                                          Configuracion_global->empresaDB,error);
                    // Restaurar Stock y acumulados.
                    Articulo oArticulo;
                    isOk = oArticulo.Devolucion(lin.value(id).value("id_articulo").toInt(),lin.value(id).value("cantidad").toFloat(),
                                         lin.value(id).value("importe").toDouble(),0);

                    if(isOk){
                        // añadir linea ticket devolución.
                        double tot_lin;
                        float cant;
                        double pvp;
                        cant = lin.value(id).value("cantidad").toFloat();

                        pvp = lin.value(id).value("precio").toDouble();
                        tot_lin = cant * pvp;

                        h["id_cab"] = new_id;
                        h["id_articulo"] = lin.value(id).value("id_articulo").toInt();
                        h["codigo"] = lin.value(id).value("codigo").toString();
                        h["descripcion"] = lin.value(id).value("descripcion").toString();
                        h["precio"] = lin.value(id).value("precio").toDouble();
                        h["cantidad"] = -cant;
                        h["importe"] =  -tot_lin;
                        h["porc_iva"]= lin.value(id).value("porc_iva").toFloat();
                        h["iva"] = lin.value(id).value("iva").toDouble();
                        h["porc_rec"] = lin.value(id).value("porc_rec").toDouble();
                        h["rec"] = lin.value(id).value("rec").toDouble();
                        h["porc_dto"] = lin.value(id).value("porc_dto").toFloat();
                        h["total"] =  -tot_lin;
                        h["subtotal"] = -tot_lin;
                        h["fecha_linea"] = QDate::currentDate().toString("yyyyMMdd");
                        h["promocion"] = lin.value(id).value("promocion").toInt();
                        h["id_tarifa"] = lin.value(id).value("id_tarifa").toInt();
                        h["tipo_dto"] = lin.value(id).value("tipo_dto").toInt();
                        SqlCalls::SqlInsert(h,"lin_tpv",Configuracion_global->empresaDB,error);
                        if(!error.isEmpty()){
                            QMessageBox::warning(qApp->activeWindow(),tr("Devolución tickets"),
                                                 tr("Ocurrió un error al guardar la línea en la base de datos %1").arg(error),
                                                 tr("Aceptar"));
                            isOk = false;
                            break;
                        }else {
                            imp += h.value("importe").toDouble();
                            l["dev_act"] = 0;
                            l["devolucion"] = lin.value(id).value("devolucion").toFloat() + cant;
                            bool success = SqlCalls::SqlUpdate(l,"lin_tpv",Configuracion_global->empresaDB,QString("id=%1").arg(id),error);
                            if(!success)
                                QMessageBox::warning(this,tr("Devoluciones"),tr("Ocurrió un error al resetear lineas: %1").arg(error));
                        }

                    }

                }


            }

        }
     }
     if(isOk)
     {
         Configuracion_global->empresaDB.commit();
         Configuracion_global->groupDB.commit();
     } else
     {
         Configuracion_global->empresaDB.rollback();
         Configuracion_global->empresaDB.rollback();
     }
     accept();
}

void FrmDevolucionTicket::on_btnParcial_clicked()
{

    bool devolver = false;
    bool isOk = false;

    if(ui->radEfectivo->isChecked())
    {
        if(QMessageBox::question(this,tr("Devolución de tickets"),
                             tr("Ha seleccionado devolución en efectivo, ¿está conforme?"),
                             tr("No"),tr("Sí"))==QMessageBox::Accepted)
            devolver = true;
    }
    if(ui->radTarjeta->isChecked())
    {
        if(QMessageBox::question(this,tr("Devolución de tickets"),
                             tr("Ha seleccionado devolución en tarjeta, ¿está conforme?"),
                             tr("No"),tr("Sí"))==QMessageBox::Accepted)
            devolver = true;
    }
    if(ui->radTransferencia->isChecked())
    {
        if(QMessageBox::question(this,tr("Devolución de tickets"),
                             tr("Ha seleccionado devolución por transferencia, ¿está conforme?"),
                             tr("No"),tr("Sí"))==QMessageBox::Accepted)
            devolver = true;
    }
    if(ui->radVales->isChecked())
    {
        if(QMessageBox::question(this,tr("Devolución de tickets"),
                             tr("Ha seleccionado devolución por vale, ¿está conforme?"),
                             tr("No"),tr("Sí"))==QMessageBox::Accepted)
            devolver = true;
    }
     if(devolver)
     {
         Configuracion_global->empresaDB.transaction();
         Configuracion_global->groupDB.transaction();
         frmCausaDevolucion causa;
        if(causa.exec() == QDialog::Accepted)
        {

             QString error;
             QHash <QString,QVariant> h;
             QHash <QString,QVariant> l;
            QItemSelection selection( ui->tablaLineas_tiquet_actual->selectionModel()->selection() );
             QList<int> rows;
             foreach( const QModelIndex & index, selection.indexes() ) {
                 if(!rows.contains(index.row()))
                    rows.append( index.row() );
             }

            if(rows.count()>0)
            {
                // Crear ticket de devolución.
                tpv oTpv;
                // TODO Cargar caja y serie
                int new_id = oTpv.nuevoticket("A","1");
                double imp =0 ;
                this->id_ticket = new_id;
                for( int i = rows.count() - 1; i >= 0; i --) {
                    int id = ui->tablaLineas_tiquet_actual->item(rows.at(i),0)->text().toInt();

                    if(new_id > -1)
                    {
                        // Capturamos info de artículo a devolver
                        QMap <int,QSqlRecord> lin;
                        lin = SqlCalls::SelectRecord("lin_tpv",QString("id = %1").arg(id),
                                                              Configuracion_global->empresaDB,error);
                        // Restaurar Stock y acumulados.
                        Articulo oArticulo;
                        isOk = oArticulo.Devolucion(lin.value(id).value("id_articulo").toInt(),lin.value(id).value("dev_act").toFloat(),
                                             lin.value(id).value("importe").toDouble(),0);

                        if(isOk){
                            // añadir linea ticket devolución.
                            double tot_lin;
                            float cant;
                            double pvp;
                            cant = lin.value(id).value("dev_act").toFloat();
                            if(cant ==0) // Si está seleccionada y no hay valor especifico se toma el de la cantidad total.
                                cant = lin.value(id).value("cantidad").toFloat();
                            pvp = lin.value(id).value("precio").toDouble();
                            tot_lin = cant * pvp;

                            h["id_cab"] = new_id;
                            h["id_articulo"] = lin.value(id).value("id_articulo").toInt();
                            h["codigo"] = lin.value(id).value("codigo").toString();
                            h["descripcion"] = lin.value(id).value("descripcion").toString();
                            h["precio"] = lin.value(id).value("precio").toDouble();
                            h["cantidad"] = -cant;
                            h["importe"] =  -tot_lin;
                            h["porc_iva"]= lin.value(id).value("porc_iva").toFloat();
                            h["iva"] = lin.value(id).value("iva").toDouble();
                            h["porc_rec"] = lin.value(id).value("porc_rec").toDouble();
                            h["rec"] = lin.value(id).value("rec").toDouble();
                            h["porc_dto"] = lin.value(id).value("porc_dto").toFloat();
                            h["total"] =  -tot_lin;
                            h["subtotal"] = -tot_lin;
                            h["fecha_linea"] = QDate::currentDate().toString("yyyyMMdd");
                            h["promocion"] = lin.value(id).value("promocion").toInt();
                            h["id_tarifa"] = lin.value(id).value("id_tarifa").toInt();
                            h["tipo_dto"] = lin.value(id).value("tipo_dto").toInt();
                            SqlCalls::SqlInsert(h,"lin_tpv",Configuracion_global->empresaDB,error);
                            if(!error.isEmpty()){
                                QMessageBox::warning(qApp->activeWindow(),tr("Devolución tickets"),
                                                     tr("Ocurrió un error al guardar la línea en la base de datos %1").arg(error),
                                                     tr("Aceptar"));
                                isOk = false;
                                break;
                            }else {
                                imp += h.value("importe").toDouble();
                                l["dev_act"] = 0;
                                l["devolucion"] = lin.value(id).value("devolucion").toFloat() + cant;
                                bool success = SqlCalls::SqlUpdate(l,"lin_tpv",Configuracion_global->empresaDB,QString("id=%1").arg(id),error);
                                if(!success)
                                    QMessageBox::warning(this,tr("Devoluciones"),tr("Ocurrió un error al resetear lineas: %1").arg(error));
                            }

                        }



                        ui->btnParcial->setEnabled(false);
                    }


                }

            }
        }
     }
     if(isOk)
     {
         Configuracion_global->empresaDB.commit();
         Configuracion_global->groupDB.commit();
     } else
     {
         Configuracion_global->empresaDB.rollback();
         Configuracion_global->empresaDB.rollback();
     }
     accept();
}



void FrmDevolucionTicket::on_tablaLineas_tiquet_actual_doubleClicked(const QModelIndex &index)
{
    int id = ui->tablaLineas_tiquet_actual->model()->index(index.row(),0).data().toInt();
    QString error;
    QMap <int,QSqlRecord> lin;
    lin = SqlCalls::SelectRecord("lin_tpv",QString("id=%1").arg(id),Configuracion_global->empresaDB,error);
    if (lin.size() >0)
    {
        frmdevolucion2 devol(this);
        devol.set_values(lin.value(id).value("id").toInt(),lin.value(id).value("codigo").toString(),
                         lin.value(id).value("descripcion").toString(),lin.value(id).value("cantidad").toFloat(),
                         lin.value(id).value("dev_act").toFloat());
        devol.exec();
    }

}


