#include "frmrecepcion_pedidos.h"
#include "ui_frmrecepcion_pedidos.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Almacen/frmtarifas.h"
#include "../Busquedas/frmbuscarcie.h"
#include "../Almacen/frmaddlotes.h"
#include "../Busquedas/db_consulta_view.h"

#include "dlgpedidoalbfact.h"
#include "../Almacen/articulo.h"

Frmrecepcion_pedidos::Frmrecepcion_pedidos(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::Frmrecepcion_pedidos),
    menuButton(QIcon(":Icons/PNG/recepcion_ped.png"),tr("Recepción de pedidos"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/recepcion_ped.png"),"",this))
{
    ui->setupUi(this);
    ui->txtFechaFin->setDate(QDate::currentDate());
    ui->lblAlbaran->setVisible(false);
    ui->lblfecha_albaran->setVisible(false);
    ui->txtAlbaran->setVisible(false);
    ui->txtFecha_albaran->setVisible(false);
    ui->lblfactura->setVisible(false);
    ui->lblFecha_factura->setVisible(false);
    ui->txtFactura->setVisible(false);
    ui->txtFecha_factura->setVisible(false);
    paso = false;
    albaran = false;
    factura = false;

    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Recepción de pedidos a proveedores"));

    connect(ui->btalbaran, SIGNAL(clicked()),this,SLOT(abrir_albaran()));    
    modelPedidos = new QSqlQueryModel(this);
    connect(ui->tablaLineas,SIGNAL(cellChanged(int,int)),this,SLOT(validarcantidad(int, int)));
}

Frmrecepcion_pedidos::~Frmrecepcion_pedidos()
{
    ui->tablaLineas->clear();
    delete ui;
}

void Frmrecepcion_pedidos::on_btnBuscar_clicked()
{    
    QString consulta;
    // ver solo pendientes
    if(ui->chkpendientes->isChecked())
        consulta = "select id,pedido,ejercicio,fecha,proveedor from ped_pro where fecha >="+ui->txtFecha_ini->date().toString("yyMMdd")+
                " and fecha <="+ui->txtFechaFin->date().toString("yyMMdd")+" and proveedor = '"+ ui->txtproveedor->text()+
                "' and recibido_completo = 0";
    if(ui->chkcompletados->isChecked())
        consulta = "select id,pedido,ejercicio,fecha,proveedor from ped_pro where fecha >="+ui->txtFecha_ini->date().toString("yyMMdd")+
                " and fecha <="+ui->txtFechaFin->date().toString("yyMMdd")+" and proveedor ='"+ ui->txtproveedor->text()+
                "' and recibido_completo = 1";
    if(ui->chktodos->isChecked())
        consulta = "select id,pedido,ejercicio,fecha,proveedor from ped_pro where fecha >="+ui->txtFecha_ini->date().toString("yyMMdd")+
                " and fecha <="+ui->txtFechaFin->date().toString("yyMMdd")+" and proveedor ='"+ ui->txtproveedor->text()+
                "'";

    modelPedidos->setQuery(consulta,Configuracion_global->empresaDB);
    ui->tablaPedidos->setModel(modelPedidos);
    ui->tablaPedidos->setColumnHidden(0,true);
    modelPedidos->setHeaderData(1,Qt::Horizontal,tr("Pedido"));
    ui->tablaPedidos->setColumnWidth(1,60);
    modelPedidos->setHeaderData(2,Qt::Horizontal,tr("Ejercicio"));
    ui->tablaPedidos->setColumnWidth(2,60);
    modelPedidos->setHeaderData(3,Qt::Horizontal,tr("Fecha"));
    ui->tablaPedidos->setColumnWidth(3,60);
    modelPedidos->setHeaderData(4,Qt::Horizontal,tr("Proveedor"));
    ui->tablaPedidos->setColumnWidth(4,150);

    connect(ui->tablaPedidos->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this,SLOT(selectionChanged(QItemSelection,QItemSelection)));
}


void Frmrecepcion_pedidos::on_chkForzarCierre_clicked()
{
//    if(QMessageBox::question(this,tr("Gestión pedidos"),
//                             tr("¿desea cambiar el estado de este pedido?"),tr("No"),tr("Si")) ==QDialog::Accepted)
//    {
//        QModelIndex index = ui->tablaPedidos->currentIndex();
//        int nid =ui->tablaPedidos->model()->data(ui->tablaPedidos->model()->index(index.row(),0),Qt::EditRole).toInt();
//        QSqlQuery queryPedido(Configuracion_global->empresaDB);
//        queryPedido.prepare("update ped_pro set recibido_completo =:estado where id = :id");
//        int estado;
//        if(ui->chkForzarCierre->isChecked())
//            estado=1;
//        else
//            estado = 0;

//        queryPedido.bindValue(":estado",estado);
//        queryPedido.bindValue(":id",nid);
//        if(!queryPedido.exec())
//            QMessageBox::warning(this,tr("Gestión de pedidos"),
//                                 tr("ATENCIÓN, Falló el cambio de estado: %1").arg(queryPedido.lastError().text()),
//                                 tr("Aceptar"));
//    }
}

void Frmrecepcion_pedidos::on_tablaPedidos_doubleClicked(const QModelIndex &index)
{
    return;

    int nid =ui->tablaPedidos->model()->data(ui->tablaPedidos->model()->index(index.row(),0),Qt::EditRole).toInt();
    id_pedido = nid;
    QSqlQuery query_lineas(Configuracion_global->empresaDB);
    query_lineas.prepare("select * from lin_ped_pro where id_cab = :nid");
    query_lineas.bindValue(":nid",nid);
    int pos = 0;
    ui->tablaLineas->setRowCount(0);
    ui->tablaLineas->clear();
    ui->tablaLineas->setColumnCount(11);
    QStringList cabecera;
    cabecera <<tr("id") << tr("C.prov") << tr("Descripción") << tr("Cant") <<tr("Pdte.")
            <<tr("Recibidos") <<tr("A.R.") <<tr("Bruto") <<tr("Etiquetas");
    ui->tablaLineas->setHorizontalHeaderLabels(cabecera);

    if(query_lineas.exec())
    {
        ui->tablaLineas->blockSignals(true);
        while (query_lineas.next())
        {
            ui->tablaLineas->setRowCount(pos+1);

            QTableWidgetItem *item_columna0 = new QTableWidgetItem(query_lineas.record().value("id").toString());            
            item_columna0->setFlags(item_columna0->flags() & (~Qt::ItemIsEditable));
            item_columna0->setTextColor(Qt::blue);
            ui->tablaLineas->setItem(pos,0,item_columna0);
            ui->tablaLineas->setColumnHidden(0,true);

            QTableWidgetItem *item_columna1 = new QTableWidgetItem(query_lineas.record().value("codigo").toString());
            item_columna1->setCheckState(Qt::Unchecked);
            item_columna1->setFlags(item_columna1->flags() & (~Qt::ItemIsEditable));
            item_columna1->setTextColor(Qt::blue);
            ui->tablaLineas->setItem(pos,1,item_columna1);

            QTableWidgetItem *item_columna2 = new QTableWidgetItem(query_lineas.record().value("codigo").toString());
            item_columna2->setFlags(item_columna2->flags() & (~Qt::ItemIsEditable));
            item_columna2->setTextColor(Qt::blue);
            ui->tablaLineas->setItem(pos,2,item_columna2);

            QTableWidgetItem *item_columna3 = new QTableWidgetItem(query_lineas.record().value("descripcion").toString());
            item_columna3->setFlags(item_columna3->flags() & (~Qt::ItemIsEditable));
            item_columna3->setTextColor(Qt::blue);
            ui->tablaLineas->setItem(pos,3,item_columna3);
            ui->tablaLineas->setColumnWidth(3,140);

            QTableWidgetItem *item_columna4 = new QTableWidgetItem(query_lineas.record().value("cantidad").toString());
            item_columna4->setFlags(item_columna4->flags() & (~Qt::ItemIsEditable));
            item_columna4->setTextColor(Qt::blue);
            ui->tablaLineas->setItem(pos,4,item_columna4);
            ui->tablaLineas->setColumnWidth(4,60);

            QTableWidgetItem *item_columna5 = new QTableWidgetItem(query_lineas.record().value("cantidad_pendiente").toString());
            double pend = query_lineas.record().value("cantidad_pendiente").toDouble();
            item_columna5->setFlags(item_columna5->flags() & (~Qt::ItemIsEditable));
            item_columna5->setTextColor(Qt::blue);
            ui->tablaLineas->setItem(pos,5,item_columna5);
            ui->tablaLineas->setColumnWidth(5,60);

            QTableWidgetItem *item_columna6 = new QTableWidgetItem(query_lineas.record().value("cantidad_recibida").toString());
            if(query_lineas.record().value("cantidad_recibida").toInt()>0)
                item_columna6->setBackgroundColor(Qt::yellow);
            if(query_lineas.record().value("cantidad_pendiente").toInt() <=0)
                item_columna6->setBackgroundColor(Qt::green);
            if(query_lineas.record().value("cantidad_recibida").toInt()==0 )
                item_columna6->setBackgroundColor(Qt::yellow);

            item_columna6->setTextColor(Qt::blue); // color de los items
            item_columna6->setFlags(item_columna6->flags() & (~Qt::ItemIsEditable));
            item_columna6->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            ui->tablaLineas->setItem(pos,6,item_columna6);


            QTableWidgetItem *item_columna7 = new QTableWidgetItem("0");
            item_columna7->setTextColor(Qt::black);
            item_columna7->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item_columna7->setBackgroundColor(Qt::white);
            if(pend == 0) //BLOQUEO SI NO QUEDAN PENDIENTES
            {
                item_columna7->setFlags(item_columna5->flags() & (~Qt::ItemIsEditable));
            }
            ui->tablaLineas->setItem(pos,7,item_columna7);

            QTableWidgetItem *item_columna8 = new QTableWidgetItem(QString::number(query_lineas.record().value("precio").toDouble(),'f',Configuracion_global->decimales));
            item_columna8->setTextColor(Qt::black); // color de los items
            ui->tablaLineas->setItem(pos,8,item_columna8);
            ui->tablaLineas->setItemDelegateForColumn(8,new MonetaryDelegate(this));

            QTableWidgetItem *item_columna9 = new QTableWidgetItem(query_lineas.record().value("etiquetas").toString());
            item_columna9->setTextColor(Qt::black); // color de los items
            item_columna9->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            ui->tablaLineas->setItem(pos,9,item_columna9);

            QTableWidgetItem *item_columna10 = new QTableWidgetItem(query_lineas.record().value("id_articulo").toString());
            item_columna10->setTextColor(Qt::black); // color de los items
            item_columna10->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            ui->tablaLineas->setItem(pos,10,item_columna10);
            ui->tablaLineas->setColumnHidden(10,true);
            pos++;

        }
        ui->tablaLineas->blockSignals(false);

    }
    ui->btalbaran->setEnabled(true);
    ui->btnFactura->setEnabled(true);
    ui->btnImprimir->setEnabled(true);
    ui->btetiquetas->setEnabled(true);

    connect(ui->tablaLineas,SIGNAL(cellChanged(int,int)),this,SLOT(validarcantidad(int, int)));
    ui->tablaLineas->setFocus();
    if(ui->tablaLineas->rowCount()>0)
        ui->tablaLineas->setCurrentCell(0,7);

}

void Frmrecepcion_pedidos::validarcantidad(int row, int col)
{
    ui->tablaLineas->blockSignals(true);
    if (col == 6 )//Cambió columna Acabo Recibir
    {
        Configuracion_global->transaction();

        double pend   = ui->tablaLineas->item(row,4)->text().toDouble();
        double recibi = ui->tablaLineas->item(row,5)->text().toDouble();
        double rec_now= ui->tablaLineas->item(row,6)->text().toDouble();

        double pend_now = pend - rec_now;
        int art_id = ui->tablaLineas->item(row,8)->data(Qt::DisplayRole).toInt();
        int line_id= ui->tablaLineas->item(row,0)->data(Qt::DisplayRole).toInt();
        int id_cab = _lineas.value(line_id).value("id_cab").toInt();
        double precio = _lineas.value(line_id).value("precio").toDouble();

        if(rec_now > 0) //unidades recibidas
        {
            QSqlQuery queryCabecera(Configuracion_global->empresaDB);
            QString error;
            bool ok =  queryCabecera.exec(QString("update ped_pro set recibido = 1 where id = %1").arg(id_cab));
            if(!ok)
                error = queryCabecera.lastError().text();

            QHash<QString,QVariant> _line_data;
            _line_data["cantidad_pendiente"] = pend_now;
            _line_data["cantidad_recibida"] = recibi + rec_now;
            ok &= SqlCalls::SqlUpdate(_line_data,"lin_ped_pro",Configuracion_global->empresaDB,QString("id=%1").arg(line_id),error);


           /* QString sql = QString("update articulos set stock_fisico_almacen = stock_fisico_almacen + %1").arg(rec_now);
            sql.append(QString(",cantidad_pendiente_recibir = cantidad_pendiente_recibir - %1").arg(rec_now));
            sql.append(QString(",stock_real = stock_fisico_almacen + cantidad_pendiente_recibir where id = %1").arg(art_id));
            QSqlQuery query_art(Configuracion_global->groupDB);
            ok &= query_art.exec(sql);*/

            ok &= Articulo::acumulado_compras(art_id,rec_now,rec_now * precio,QDate::currentDate(),true);

            //LOTES
            if(SqlCalls::SelectOneField("articulos","lotes",QString("id=%1").arg(art_id),Configuracion_global->groupDB,error).toBool())
            {
                frmaddLotes frmLotes(this);
                frmLotes.cargar_articulo(art_id);
                frmLotes.exec();
            }
            if(ok)
            {
                Configuracion_global->commit();
                ui->tablaLineas->item(row,4)->setText(QString::number(pend_now));
                ui->tablaLineas->item(row,5)->setText(QString::number(recibi + rec_now));
                ui->tablaLineas->item(row,6)->setText("0");

                if(pend_now > 0)
                    ui->tablaLineas->item(row,5)->setBackgroundColor(Qt::yellow);
                else
                    ui->tablaLineas->item(row,5)->setBackgroundColor(Qt::green);

            }
            else
            {
                Configuracion_global->rollback();
                QMessageBox::critical(this,tr("Error al actualizar stocks/pendientes"),error);
            }
        }
    }
    if(col ==8 ) // Coste
    {
        int id = ui->tablaLineas->item(row,10)->text().toInt();
        QSqlQuery queryProducto(Configuracion_global->groupDB);
        queryProducto.prepare("select coste from articulos where id =:nid");
        queryProducto.bindValue(":nid",id);
        if(queryProducto.exec())
        {
            queryProducto.next();
            double coste = queryProducto.record().value("coste").toDouble();
            QString svalor = ui->tablaLineas->item(row,8)->text();
            double valor = Configuracion_global->MonedatoDouble(svalor);

            if(coste != valor)
            {
                //----------------------------
                // Cambio pvp en linea_pedido
                //----------------------------
                int id_linea = ui->tablaLineas->item(row,0)->text().toInt();
                QSqlQuery queryLineas(Configuracion_global->empresaDB);
                queryLineas.prepare("update lin_ped_pro set coste_bruto =:coste where id =:id_linea");
                queryLineas.bindValue(":coste",valor);
                queryLineas.bindValue(":id_linea",id_linea);
                if(!queryLineas.exec())
                    QMessageBox::question(this,tr("ATENCIÓN:"),
                                          tr("Ocurrió un error al guardar el coste en la línea: %1").arg(queryLineas.lastError().text()),
                                          tr("Aceptar"));

                if( QMessageBox::question(this,tr("Entrada de Pedidos"),tr("¿Desea actualizar coste en la ficha del producto?"),
                                      tr("No"),tr("Sí")) == QMessageBox::Accepted)
                {
                    queryProducto.prepare("update articulos set coste = :coste where id =:id");
                    queryProducto.bindValue(":coste",valor);
                    queryProducto.bindValue(":id",id);
                    if(!queryProducto.exec())
                    {
                        QMessageBox::warning(this,tr("ATENCIÓN"),
                                             tr("Ocurrió un error al cambiar el coste del producto: %1").arg(queryProducto.lastError().text()),
                                             tr("Aceptar"));

                    } else
                    {
                        if (QMessageBox::question(this,tr("Entrada de productos"),
                                                  tr("¿Desea modificar las tarifas del producto?"),
                                                  tr("No"),tr("Si"))==QMessageBox::Accepted)
                        {
                            FrmTarifas frmtarifas;
                            frmtarifas.capturar_datos(queryProducto.record().field("id").value().toInt(),
                                                      QString::number(queryProducto.record().field("coste").value().toDouble(),
                                                                      'f',Configuracion_global->decimales),id);
                            frmtarifas.exec();
                        }
                    }
                }

            }
        }

    }
    ui->tablaLineas->blockSignals(false);
}


void Frmrecepcion_pedidos::abrir_albaran()
{
//    ui->tablaPedidos->setEnabled(false);
//    ui->btncancelar_alb->setEnabled(true);
//    ui->txtFecha_albaran->setDate(QDate::currentDate());
//    emit block();

//    if(albaran == false) {
//        //--------------------------------------
//        // Creo cabecera en blanco del albarán
//        //--------------------------------------
//        ui->lblAlbaran->setVisible(true);
//        ui->lblfecha_albaran->setVisible(true);
//        ui->txtAlbaran->setVisible(true);
//        ui->txtFecha_albaran->setVisible(true);

//        ui->btalbaran->setText(tr("Crear y cerrar Albarán"));
//        ui->txtAlbaran->setFocus();
//        albaran =  true;
//        QSqlQuery query_albaran(Configuracion_global->empresaDB);
//        if(query_albaran.exec("insert into alb_pro  (albaran) values ('temp')"))
//            id_albaran = query_albaran.lastInsertId().toInt();
//        else
//            QMessageBox::warning(this,tr("Recepción Pedidos"),
//                                 tr("No se pudo crear el albarán, Error:")+query_albaran.lastError().text(),
//                                 tr("Aceptar"));
//    } else
//    {
//        if(ui->txtAlbaran->text().isEmpty()){
//            QMessageBox::warning(this,tr("Recepción de pedidos"),tr("Debe rellenar el número de albarán"),tr("Aceptar"));

//        } else
//        {
//            QSqlQuery query_pedido(Configuracion_global->empresaDB);
//            query_pedido.exec("Select * from ped_pro where id = "+QString::number(id_pedido));
//            query_pedido.next();
//            QDate fecha;
//            fecha = ui->txtFecha_albaran->date();

//            //----------------------------
//            // Rellenar cabecera Albarán
//            //----------------------------


//            QSqlQuery query_albaran(Configuracion_global->empresaDB);
//            query_albaran.prepare("update alb_pro  set albaran =:albaran, fecha = :fecha,id_proveedor=:id_proveedor,"
//                                  "proveedor =:proveedor, cif_proveedor =:cif,pedido =:pedido,"
//                                  "iva1 =:iva1, iva2 =:iva2, iva3 =:iva3, iva4 =:iva4, "
//                                  "base1 =:base1,base2 =:base2,base3 =:base3, base4 =:base4, "
//                                  "iva1 =:iva1,iva2 =:iva2, iva3 =:iva3, iva4 =:iva4, "
//                                  "total1 =:total1,total2 =:total2,total3 =:total3,total4 =:total4,"
//                                  "porc_rec1=:porc_rec1,porc_rec2=:porc_rec2,"
//                                  "porc_rec3=:porc_rec3,porc_rec4=:porc_rec4,"
//                                  "rec1=:rec1,rec2= :rec2,rec3=:rec3,"
//                                  "rec4=:rec4,importe_rec_total =:importe_rec_total,"
//                                  "base_total =:base,iva_total =:iva,total= :total "
//                                  "where id =:id");
//            query_albaran.bindValue(":albaran",ui->txtAlbaran->text());
//            query_albaran.bindValue(":fecha",fecha);
//            query_albaran.bindValue(":id_proveedor",query_pedido.record().value("id_proveedor").toInt());
//            query_albaran.bindValue(":proveedor",query_pedido.record().value("proveedor").toString());
//            query_albaran.bindValue(":cif",query_pedido.record().value("cif_nif").toString());
//            query_albaran.bindValue(":pedido",ui->txtNumPedido->text().toInt());
//            query_albaran.bindValue(":iva1",Configuracion_global->ivaList.at(0));
//            query_albaran.bindValue(":iva2",Configuracion_global->ivaList.at(1));
//            query_albaran.bindValue(":iva3",Configuracion_global->ivaList.at(2));
//            query_albaran.bindValue(":iva4",Configuracion_global->ivaList.at(3));

//            query_albaran.bindValue(":id",id_albaran);

//            // ----------------------
//            // Sumar líneas albarán
//            // ----------------------
//            QSqlQuery query_lin_alb_pro(Configuracion_global->empresaDB);
//            query_lin_alb_pro.prepare("select * from lin_alb_pro where id_cab = :id");
//            query_lin_alb_pro.bindValue(":id",id_albaran);
//            double base1,base2,base3,base4,iva1,iva2,iva3,iva4,dto1,dto2,dto3,dto4,total1,total2,total3,total4;
//            double rec1,rec2,rec3,rec4,rec_total;
//            double base,iva,total;
//            bool recargo = query_pedido.record().value("recargo_equivalencia").toBool();
//            base1 = 0; base2 = 0; base3 = 0; base4 = 0;
//            iva1 = 0;  iva2 = 0;  iva3 = 0;  iva4 = 0;
//            dto1 = 0; dto2 = 0; dto3 = 0; dto4 = 0;
//            total1 = 0; total2 = 0; total3 = 0; total4 = 0;
//            rec1 = 0, rec2 = 0; rec3 =0; rec4 =0; rec_total = 0;
//            base = 0; iva = 0; total=0;
//            if(query_lin_alb_pro.exec())
//            {
//                while (query_lin_alb_pro.next())
//                {

//                    if(query_lin_alb_pro.record().value("iva").toDouble() == Configuracion_global->ivaList.at(0).toDouble())
//                    {
//                        base1 +=query_lin_alb_pro.record().value("subtotal").toDouble();
//                        iva1 +=query_lin_alb_pro.record().value("iva").toDouble();
//                        total1 +=query_lin_alb_pro.record().value("total").toDouble();
//                        if(recargo)
//                            rec1 +=query_lin_alb_pro.record().value("rec_equivalencia").toDouble();
//                    }
//                    if(query_lin_alb_pro.record().value("iva").toDouble() == Configuracion_global->ivaList.at(1).toDouble())
//                    {
//                        base2 +=query_lin_alb_pro.record().value("subtotal").toDouble();
//                        iva2 +=query_lin_alb_pro.record().value("iva").toDouble();
//                        total2 +=query_lin_alb_pro.record().value("total").toDouble();
//                        if(recargo)
//                            rec2 +=query_lin_alb_pro.record().value("rec_equivalencia").toDouble();
//                    }
//                    if(query_lin_alb_pro.record().value("iva").toDouble() == Configuracion_global->ivaList.at(2).toDouble())
//                    {
//                        base3 +=query_lin_alb_pro.record().value("subtotal").toDouble();
//                        iva3 +=query_lin_alb_pro.record().value("iva").toDouble();
//                        total3 +=query_lin_alb_pro.record().value("total").toDouble();
//                        if(recargo)
//                            rec3 +=query_lin_alb_pro.record().value("rec_equivalencia").toDouble();
//                    }
//                    if(query_lin_alb_pro.record().value("iva").toDouble() == Configuracion_global->ivaList.at(3).toDouble())
//                    {
//                        base4 +=query_lin_alb_pro.record().value("subtotal").toDouble();
//                        iva4 +=query_lin_alb_pro.record().value("iva").toDouble();
//                        total4 +=query_lin_alb_pro.record().value("total").toDouble();
//                        if(recargo)
//                            rec4 +=query_lin_alb_pro.record().value("rec_equivalencia").toDouble();
//                    }

//                }
//            }
//            base = base1+base2+base3+base4;
//            iva = iva1+iva2+iva3+iva4;
//            total = total1+total2+total3+total4;
//            rec_total = rec1+rec2+rec3+rec4;
//            query_albaran.bindValue(":base1",base1);
//            query_albaran.bindValue(":base2",base2);
//            query_albaran.bindValue(":base3",base3);
//            query_albaran.bindValue(":base4",base4);
//            query_albaran.bindValue(":iva1",iva1);
//            query_albaran.bindValue(":iva2",iva2);
//            query_albaran.bindValue(":iva3",iva3);
//            query_albaran.bindValue(":iva4",iva4);
//            query_albaran.bindValue(":total1",total1);
//            query_albaran.bindValue(":total2",total2);
//            query_albaran.bindValue(":total3",total3);
//            query_albaran.bindValue(":total4",total4);
//            query_albaran.bindValue(":porc_rec1",Configuracion_global->reList.at(0));
//            query_albaran.bindValue(":porc_rec2",Configuracion_global->reList.at(1));
//            query_albaran.bindValue(":porc_rec3",Configuracion_global->reList.at(2));
//            query_albaran.bindValue(":porc_rec4",Configuracion_global->reList.at(3));
//            query_albaran.bindValue(":rec1",rec1);
//            query_albaran.bindValue(":rec2",rec2);
//            query_albaran.bindValue(":rec3",rec3);
//            query_albaran.bindValue(":rec4",rec4);
//            query_albaran.bindValue(":importe_rec_total",rec_total);
//            query_albaran.bindValue(":total",total);
//            query_albaran.bindValue(":iva",iva);
//            query_albaran.bindValue(":base",base);


//            if(!query_albaran.exec()){
//                QMessageBox::warning(this,tr("Recepción Pedidos"),
//                                     tr("No se pudo crear el albarán, Error:")+query_albaran.lastError().text(),
//                                     tr("Aceptar"));
//                QString error;
//                SqlCalls::SqlDelete("alb_pro",Configuracion_global->empresaDB,QString("id=%1").arg(id_albaran),error);
//                SqlCalls::SqlDelete("lin_alb_pro",Configuracion_global->empresaDB,QString("id_cab = %1").arg(id_albaran),error);
//            } else {
//                QString cadena = tr("Se ha creado el albarán: ");
//                cadena.append(ui->txtAlbaran->text());
//                cadena.append("\nde : ");
//                cadena.append(query_pedido.record().value("proveedor").toString());
//                cadena.append("\ny de fecha:");
//                cadena.append(fecha.toString("dd/MM/yyyy"));
//                QMessageBox::information(this,tr("Recepción pedidos"),cadena,
//                                     tr("Aceptar"));
//                id_albaran = 0;
//            }
//            ui->lblAlbaran->setVisible(false);
//            ui->txtAlbaran->setText("");
//            ui->txtAlbaran->setVisible(false);
//            ui->txtFecha_albaran->setDate(QDate::currentDate());
//            ui->lblfecha_albaran->setVisible(false);
//            ui->txtFecha_albaran->setVisible(false);
//            ui->btalbaran->setText("Abrir albarán");
//            ui->lblfecha_albaran->setVisible(false);
//            albaran = false;
//            ui->tablaPedidos->setEnabled(true);
//            ui->btncancelar_alb->setEnabled(false);
//            emit unblock();

//        }
//    }
    QMap<int, QSqlRecord> selected_lineas;
    bool empty_selected = false;
    for(auto i=0; i< ui->tablaLineas->rowCount(); ++i)
    {
        if(ui->tablaLineas->item(i,1)->checkState() == Qt::Checked)
        {
            if(ui->tablaLineas->item(i,5)->data(Qt::DisplayRole).toDouble() > 0)
            {
                int id = ui->tablaLineas->item(i,0)->data(Qt::DisplayRole).toInt();
                selected_lineas.insert(id,_lineas.value(id));
            }
            else
            {
                empty_selected = true;
            }
        }
    }
    if(selected_lineas.isEmpty())
        return;
    if(empty_selected)
    {
        QMessageBox::information(this,tr("Lineas sin recibir")
                                 ,tr("Tiene lineas seleccionadas con 0 unidades recibidas.\n"
                                     "Dichas lineas no se usarán para crear el albarán."));
    }
    DlgPedidoAlbFact dlg(tr("Creación de Albarán"),this);
    dlg.setLineas(selected_lineas);
    dlg.exec();
}

void Frmrecepcion_pedidos::on_btnFactura_clicked()
{
//    ui->BtnCancelar->setEnabled(true);
//    ui->txtFecha_factura->setDate(QDate::currentDate());
//    emit block();
//    if(factura == false) {
//        //---------------------------------------
//        // Creo cabecera en blanco de la factura
//        //---------------------------------------
//        ui->lblfactura->setVisible(true);
//        ui->lblFecha_factura->setVisible(true);
//        ui->txtFactura->setVisible(true);
//        ui->txtFecha_factura->setVisible(true);

//        ui->btnFactura->setText(tr("Crear y cerrar Factura"));
//        ui->txtFactura->setFocus();
//        factura =  true;
//        QSqlQuery query_factura(Configuracion_global->empresaDB);
//        query_factura.prepare("insert into fac_pro  (factura,porc_iva1,porc_iva2,porc_iva3,porc_iva4,"
//                              "porc_rec1,porc_rec2,porc_rec3,porc_rec4) values "
//                              "('temp',:iva1,:iva2,:iva3,:iva4,:porc_rec1,:porc_rec2,:porc_rec3,:porc_rec4)");
//        query_factura.bindValue(":iva1",Configuracion_global->ivaList.at(0));
//        query_factura.bindValue(":iva2",Configuracion_global->ivaList.at(1));
//        query_factura.bindValue(":iva3",Configuracion_global->ivaList.at(2));
//        query_factura.bindValue(":iva4",Configuracion_global->ivaList.at(3));
//        query_factura.bindValue(":porc_rec1",Configuracion_global->reList.at(0));
//        query_factura.bindValue(":porc_rec2",Configuracion_global->reList.at(1));
//        query_factura.bindValue(":porc_rec3",Configuracion_global->reList.at(2));
//        query_factura.bindValue(":porc_rec4",Configuracion_global->reList.at(3));

//        if(query_factura.exec())

//            id_factura = query_factura.lastInsertId().toInt();
//        else{
//            QMessageBox::warning(this,tr("Recepción Pedidos"),
//                                 tr("No se pudo crear la factura, Error:")+query_factura.lastError().text(),
//                                 tr("Aceptar"));
//            QString error;

//            SqlCalls::SqlDelete("fac_pro",Configuracion_global->empresaDB,QString("id=%1").arg(id_factura),error);
//            SqlCalls::SqlDelete("lin_fac_pro",Configuracion_global->empresaDB,QString("id_cab = %1").arg(id_factura),error);
//        }
//    } else
//    {
//        QSqlQuery query_pedido(Configuracion_global->empresaDB);
//        query_pedido.exec("Select * from ped_pro where id = "+QString::number(id_pedido));
//        query_pedido.next();
//        QDate fecha;
//        fecha = ui->txtFecha_factura->date();

//        //----------------------------
//        // Rellenar cabecera Factura
//        //----------------------------


//        QSqlQuery query_factura(Configuracion_global->empresaDB);
//        query_factura.prepare("update fac_pro  set factura =:factura, fecha = :fecha,id_proveedor=:id_proveedor,"
//                              "proveedor =:proveedor, cif_proveedor =:cif,pedido =:pedido,"
//                              "base1 =:base1,base2 =:base2,base3 =:base3, base4 =:base4, "
//                              "iva1 =:iva1,iva2 =:iva2, iva3 =:iva3, iva4 =:iva4, "
//                              "total1 =:total1,total2 =:total2,total3 =:total3,total4 =:total4,"
//                              "total_base =:total_base, total_iva =:total_iva, total =:total,"
//                              "total_dto =:total_dto,recargo_equivalencia =:recargo_equivalencia,"
//                              "rec1=:rec1,rec2 =:rec2,rec3=:rec3,rec4=:rec4,total_retencion=:total_retencion where id =:id");
//        query_factura.bindValue(":factura",ui->txtFactura->text());
//        query_factura.bindValue(":fecha",fecha);
//        query_factura.bindValue(":id_proveedor",query_pedido.record().value("id_proveedor").toInt());
//        query_factura.bindValue(":proveedor",query_pedido.record().value("proveedor").toString());
//        query_factura.bindValue(":cif",query_pedido.record().value("cif_nif").toString());
//        query_factura.bindValue(":pedido",ui->txtNumPedido->text().toInt());
//        query_factura.bindValue(":id",id_factura);
//        query_factura.bindValue(":recargo_equivalencia",query_pedido.record().value("recargo_equivalencia").toBool());


//        // ----------------------
//        // Sumar líneas factura
//        // ----------------------
//        QSqlQuery query_lin_fac_pro(Configuracion_global->empresaDB);
//        query_lin_fac_pro.prepare("select * from lin_fac_pro where id_cab = :id");
//        query_lin_fac_pro.bindValue(":id",id_factura);
//        double base1,base2,base3,base4,iva1,iva2,iva3,iva4,dto1,dto2,dto3,dto4,total1,total2,total3,total4,base,iva,dto,total;
//        double  rec1,rec2,rec3,rec4,rec;
//        bool recargo = query_pedido.record().value("recargo_equivalencia").toBool();
//        base1 = 0; base2 = 0; base3 = 0; base4 = 0;base=0;
//        iva1 = 0;  iva2 = 0;  iva3 = 0;  iva4 = 0;
//        dto1 = 0; dto2 = 0; dto3 = 0; dto4 = 0; dto=0;
//        rec1 = 0; rec2 = 0; rec3 =0; rec4=0; rec =0;
//        total1 = 0; total2 = 0; total3 = 0; total4 = 0;



//        if(query_lin_fac_pro.exec())
//        {
//            while (query_lin_fac_pro.next())
//            {

//                if(query_lin_fac_pro.record().value("iva").toDouble() == Configuracion_global->ivaList.at(0).toDouble())
//                {
//                    base1 +=query_lin_fac_pro.record().value("subtotal").toDouble();
//                    iva1 +=query_lin_fac_pro.record().value("iva").toDouble();
//                    dto1 +=query_lin_fac_pro.record().value("dto").toDouble();
//                    total1 +=query_lin_fac_pro.record().value("total").toDouble();
//                    if(recargo)
//                        rec1 +=query_lin_fac_pro.record().value("importe_rec").toDouble();

//                }
//                if(query_lin_fac_pro.record().value("iva").toDouble() == Configuracion_global->ivaList.at(1).toDouble())
//                {
//                    base2 +=query_lin_fac_pro.record().value("subtotal").toDouble();
//                    iva2 +=query_lin_fac_pro.record().value("iva").toDouble();
//                    dto2 +=query_lin_fac_pro.record().value("dto").toDouble();
//                    total2 +=query_lin_fac_pro.record().value("total").toDouble();
//                    if(recargo)
//                        rec2 +=query_lin_fac_pro.record().value("importe_rec").toDouble();
//                }
//                if(query_lin_fac_pro.record().value("iva").toDouble() == Configuracion_global->ivaList.at(2).toDouble())
//                {
//                    base3 +=query_lin_fac_pro.record().value("subtotal").toDouble();
//                    iva3 +=query_lin_fac_pro.record().value("iva").toDouble();
//                    total3 +=query_lin_fac_pro.record().value("total").toDouble();
//                    dto3 +=query_lin_fac_pro.record().value("dto").toDouble();
//                    if(recargo)
//                        rec2 +=query_lin_fac_pro.record().value("importe_rec").toDouble();
//                }
//                if(query_lin_fac_pro.record().value("iva").toDouble() == Configuracion_global->ivaList.at(3).toDouble())
//                {
//                    base4 +=query_lin_fac_pro.record().value("subtotal").toDouble();
//                    iva4 +=query_lin_fac_pro.record().value("iva").toDouble();
//                    total4 +=query_lin_fac_pro.record().value("total").toDouble();
//                    dto4 +=query_lin_fac_pro.record().value("dto").toDouble();
//                    if(recargo)
//                        rec2 +=query_lin_fac_pro.record().value("importe_rec").toDouble();
//                }
//                base = base1 + base2 +base3 + base4;
//                iva = iva1+iva2+iva3+iva4;
//                total = total1+total2+total3+total4;
//                dto = dto1+dto2+dto3+dto4;
//                rec = rec1+rec2+rec3+rec4;


//            }
//        }
//        query_factura.bindValue(":base1",base1);
//        query_factura.bindValue(":base2",base2);
//        query_factura.bindValue(":base3",base3);
//        query_factura.bindValue(":base4",base4);
//        query_factura.bindValue(":iva1",iva1);
//        query_factura.bindValue(":iva2",iva2);
//        query_factura.bindValue(":iva3",iva3);
//        query_factura.bindValue(":iva4",iva4);
//        query_factura.bindValue(":total1",total1);
//        query_factura.bindValue(":total2",total2);
//        query_factura.bindValue(":total3",total3);
//        query_factura.bindValue(":total4",total4);
//        query_factura.bindValue(":total_base",base);
//        query_factura.bindValue(":total_iva",iva);
//        query_factura.bindValue(":total",total);
//        query_factura.bindValue("total_dto",dto);
//        query_factura.bindValue(":rec1",rec1);
//        query_factura.bindValue(":rec2",rec2);
//        query_factura.bindValue(":rec3",rec3);
//        query_factura.bindValue(":rec4",rec4);
//        query_factura.bindValue(":total_retencion",rec);


//        if(!query_factura.exec())
//            QMessageBox::warning(this,tr("Recepción Pedidos"),
//                                 tr("No se pudo crear la Factura, Error:")+query_factura.lastError().text(),
//                                 tr("Aceptar"));
//        else {
//            QString cadena = tr("Se ha creado la factura: ");
//            cadena.append(ui->txtFactura->text());
//            cadena.append("\nde : ");
//            cadena.append(query_pedido.record().value("proveedor").toString());
//            cadena.append("\ny de fecha:");
//            cadena.append(fecha.toString("dd/MM/yyyy"));
//            QMessageBox::information(this,tr("Recepción pedidos"),cadena,
//                                 tr("Aceptar"));
//            id_factura = 0;
//        }
//        ui->lblfactura->setVisible(false);
//        ui->txtFactura->setText("");
//        ui->txtFactura->setVisible(false);
//        ui->txtFecha_factura->setDate(QDate::currentDate());
//        ui->lblFecha_factura->setVisible(false);
//        ui->txtFecha_factura->setVisible(false);
//        ui->btnFactura->setText("Abrir Factura");
//        ui->lblFecha_factura->setVisible(false);

//        // TODO - CONTABILIZAR FACTURA COMPRAS

//        factura = false;
//        ui->BtnCancelar->setEnabled(false);
//        emit unblock();
//    }
}



void Frmrecepcion_pedidos::on_BtnCancelar_clicked()
{
//    QSqlQuery queryFactura(Configuracion_global->groupDB);
//    queryFactura.exec("delete from fac_pro where id ="+QString::number(this->id_factura));
//    ui->lblfactura->setVisible(false);
//    ui->txtFactura->setText("");
//    ui->txtFactura->setVisible(false);
//    ui->txtFecha_factura->setDate(QDate::currentDate());
//    ui->lblFecha_factura->setVisible(false);
//    ui->txtFecha_factura->setVisible(false);
//    ui->btnFactura->setText("Abrir Factura");
//    ui->lblFecha_factura->setVisible(false);
//    ui->BtnCancelar->setEnabled(false);
//    emit unblock();


}

void Frmrecepcion_pedidos::on_btncancelar_alb_clicked()
{

//    QSqlQuery queryAlbaran(Configuracion_global->groupDB);
//    queryAlbaran.exec("delete from alb_pro where id ="+QString::number(this->id_albaran));
//    ui->lblAlbaran->setVisible(false);
//    ui->txtAlbaran->setText("");
//    ui->txtAlbaran->setVisible(false);
//    ui->txtFecha_albaran->setDate(QDate::currentDate());
//    ui->lblfecha_albaran->setVisible(false);
//    ui->txtFecha_albaran->setVisible(false);
//    ui->btalbaran->setText("Abrir albarán");
//    ui->lblfecha_albaran->setVisible(false);
//    albaran = false;
//    ui->tablaPedidos->setEnabled(true);
//    ui->btncancelar_alb->setEnabled(false);
//    emit unblock();
}

void Frmrecepcion_pedidos::on_btnBuscaProv_clicked()
{
    db_consulta_view consulta;
    QStringList campos;
    campos  << "codigo" << "proveedor";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Proveedores");
    consulta.set_db("Group");
    consulta.set_SQL("select id, codigo,proveedor,cif,poblacion from proveedores");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Código") << tr("Proveedor") << tr("DNI/CIF/NIE") << tr("Población");
    tamanos <<"0" << "100" << "300" << "180" << "300";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de proveedores");
    if(consulta.exec())
    {
        ui->txtproveedor->setText(consulta.get_record().value("proveedor").toString());
    }
}

void Frmrecepcion_pedidos::selectionChanged(const QItemSelection & /*selected*/, const QItemSelection & /*deselected*/)
{
    _lineas.clear();
    ui->tablaLineas->blockSignals(true);
    ui->tablaLineas->clear();
    ui->tablaLineas->setColumnCount(0);
    ui->tablaLineas->setRowCount(0);
    QModelIndexList indexs = ui->tablaPedidos->selectionModel()->selectedRows();
    if(indexs.isEmpty())
    {
        ui->tablaLineas->blockSignals(false);
        return;
    }

    QString error;
    int pos = 0;
    ui->tablaLineas->setColumnCount(9);
    QStringList cabecera;
    cabecera <<tr("id") << tr("C.prov") <<tr("Descripción") << tr("Cant") <<tr("Pdte.")
            <<tr("Recibidos") <<tr("A.R.") <<tr("Etiquetas");
    ui->tablaLineas->setHorizontalHeaderLabels(cabecera);
    for(QModelIndexList::const_iterator i = indexs.begin() ; i != indexs.end() ; ++i)
    {       
        QMap<int, QSqlRecord> lineas = SqlCalls::SelectRecord("lin_ped_pro",QString("id_cab = %1").arg(i->data().toInt()),Configuracion_global->empresaDB,error);
        for(auto l = lineas.cbegin() ; l != lineas.cend() ; ++l)
        {
            ui->tablaLineas->setRowCount(pos+1);
            const QSqlRecord r = l.value();
            _lineas.insert(r.value("id").toInt(),r);
            QTableWidgetItem *item_columna0 = new QTableWidgetItem(r.value("id").toString());
            item_columna0->setFlags(item_columna0->flags() & (~Qt::ItemIsEditable));
            item_columna0->setTextColor(Qt::blue);
            ui->tablaLineas->setItem(pos,0,item_columna0);
            ui->tablaLineas->setColumnHidden(0,true);

            QTableWidgetItem *item_columna1 = new QTableWidgetItem(r.value("codigo").toString());
            item_columna1->setCheckState(Qt::Unchecked);
            item_columna1->setFlags(item_columna1->flags() & (~Qt::ItemIsEditable));
            item_columna1->setTextColor(Qt::blue);
            ui->tablaLineas->setItem(pos,1,item_columna1);

            QTableWidgetItem *item_columna3 = new QTableWidgetItem(r.value("descripcion").toString());
            item_columna3->setFlags(item_columna3->flags() & (~Qt::ItemIsEditable));
            item_columna3->setTextColor(Qt::blue);
            ui->tablaLineas->setItem(pos,2,item_columna3);
            ui->tablaLineas->setColumnWidth(2,140);

            QTableWidgetItem *item_columna4 = new QTableWidgetItem(r.value("cantidad").toString());
            item_columna4->setFlags(item_columna4->flags() & (~Qt::ItemIsEditable));
            item_columna4->setTextColor(Qt::blue);
            ui->tablaLineas->setItem(pos,3,item_columna4);
            ui->tablaLineas->setColumnWidth(3,60);

            QTableWidgetItem *item_columna5 = new QTableWidgetItem(r.value("cantidad_pendiente").toString());
            double pend = r.value("cantidad_pendiente").toDouble();
            item_columna5->setFlags(item_columna5->flags() & (~Qt::ItemIsEditable));
            item_columna5->setTextColor(Qt::blue);
            ui->tablaLineas->setItem(pos,4,item_columna5);
            ui->tablaLineas->setColumnWidth(4,60);

            QTableWidgetItem *item_columna6 = new QTableWidgetItem(r.value("cantidad_recibida").toString());
            if(r.value("cantidad_recibida").toInt()>0)
                item_columna6->setBackgroundColor(Qt::yellow);
            if(r.value("cantidad_pendiente").toInt() <=0)
                item_columna6->setBackgroundColor(Qt::green);
            if(r.value("cantidad_recibida").toInt()==0 )
                item_columna6->setBackgroundColor(Qt::yellow);

            item_columna6->setTextColor(Qt::blue); // color de los items
            item_columna6->setFlags(item_columna6->flags() & (~Qt::ItemIsEditable));
            item_columna6->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            ui->tablaLineas->setItem(pos,5,item_columna6);


            QTableWidgetItem *item_columna7 = new QTableWidgetItem("0");
            item_columna7->setTextColor(Qt::black);
            item_columna7->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item_columna7->setBackgroundColor(Qt::white);
            if(pend == 0) //BLOQUEO SI NO QUEDAN PENDIENTES
            {
                item_columna7->setFlags(item_columna7->flags() & (~Qt::ItemIsEditable));
            }
            ui->tablaLineas->setItem(pos,6,item_columna7);

            QTableWidgetItem *item_columna9 = new QTableWidgetItem(r.value("etiquetas").toString());
            item_columna9->setTextColor(Qt::black); // color de los items
            item_columna9->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            ui->tablaLineas->setItem(pos,7,item_columna9);

            QTableWidgetItem *item_columna10 = new QTableWidgetItem(r.value("id_articulo").toString());
            item_columna10->setTextColor(Qt::black); // color de los items
            item_columna10->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            ui->tablaLineas->setItem(pos,8,item_columna10);
            ui->tablaLineas->setColumnHidden(8,true);

            pos++;
        }
    }
    ui->tablaLineas->blockSignals(false);
}
