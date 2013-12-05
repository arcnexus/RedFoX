#include "frmdevolucionticket.h"
#include "ui_frmdevolucionticket.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/datedelegate.h"
#include "../TPV/frmcausadevolucion.h"

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

    //-----------------
    // Conexiones
    //-----------------
    connect(ui->tabla_cabeceras->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(refrescar_lineas(QModelIndex,QModelIndex)));

    ui->tabla_cabeceras->selectRow(0);

 }

FrmDevolucionTicket::~FrmDevolucionTicket()
{
    delete ui;
}

void FrmDevolucionTicket::cargarlineas(int id_cab)
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


        QString cant = QString::number(cantidad,'f',2);
        QTableWidgetItem *item_col2 = new QTableWidgetItem(cant);
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
    ui->tablaLineas_tiquet_actual->selectRow(pos);

}

void FrmDevolucionTicket::refrescar_lineas(QModelIndex current, QModelIndex previous)
{
    Q_UNUSED (previous);
    int id_cab = ui->tabla_cabeceras->model()->data(current.model()->index(current.row(),0)).toInt();
    cargarlineas(id_cab);

}

void FrmDevolucionTicket::on_btnDevolucion_clicked()
{
    frmCausaDevolucion causa;
    causa.exec();
}

void FrmDevolucionTicket::on_btnParcial_clicked()
{
    QItemSelection selection( ui->tablaLineas_tiquet_actual->selectionModel()->selection() );
     QList<int> rows;
     foreach( const QModelIndex & index, selection.indexes() ) {
         if(!rows.contains(index.row()))
            rows.append( index.row() );
     }
    if(rows.count())
     for( int i = rows.count() - 1; i >= 0; i --) {
        qDebug() << rows.at(i);
     }
     ui->btnParcial->setEnabled(false);
}
