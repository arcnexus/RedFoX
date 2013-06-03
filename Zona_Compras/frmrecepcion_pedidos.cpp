#include "frmrecepcion_pedidos.h"
#include "ui_frmrecepcion_pedidos.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Almacen/frmeditar_tarifas.h"
#include "../Busquedas/frmbuscarcie.h"


Frmrecepcion_pedidos::Frmrecepcion_pedidos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Frmrecepcion_pedidos)
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
    


    connect(ui->btnAlbaran, SIGNAL(clicked()),this,SLOT(abrir_albaran()));

}

Frmrecepcion_pedidos::~Frmrecepcion_pedidos()
{
    ui->tablaLineas->clear();
    delete ui;
}

void Frmrecepcion_pedidos::on_btnBuscar_clicked()
{
    QSqlQueryModel *modelPedidos = new QSqlQueryModel(this);
    QString consulta;
    if(ui->txtNumPedido->text().isEmpty())
    {
        int recibido;
        if(ui->chkPedientes->isChecked())
            recibido = 1;
        else
            recibido = 0;

        consulta = "select id,nPedido,nEjercicio,dFecha,cProveedor from ped_pro where dFecha >="+ui->txtFecha_ini->date().toString("yyMMdd")+
            " and dFecha <="+ui->txtFechaFin->date().toString("yyMMdd")+" and cProveedor like '"+ ui->txtproveedor->text()+
            "%' and lRecibidoCompleto =" + QString::number(recibido);
    } else
    {
        consulta = " select id,nPedido,nEjercicio,dFecha,cProveedor from ped_pro where nPedido = "+ui->txtNumPedido->text();
    }

    modelPedidos->setQuery(consulta,QSqlDatabase::database("empresa"));
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


}


void Frmrecepcion_pedidos::on_chkForzarCierre_clicked()
{
    if(QMessageBox::question(this,tr("Gestión pedidos"),
                             tr("¿desea cambiar el estado de este pedido?"),tr("No"),tr("Si")) ==QDialog::Accepted)
    {
        QModelIndex index = ui->tablaPedidos->currentIndex();
        int nid =ui->tablaPedidos->model()->data(ui->tablaPedidos->model()->index(index.row(),0),Qt::EditRole).toInt();
        QSqlQuery queryPedido(QSqlDatabase::database("empresa"));
        queryPedido.prepare("update ped_pro set lRecibidoCompleto =:estado where id = :id");
        int estado;
        if(ui->chkForzarCierre->isChecked())
            estado=1;
        else
            estado = 0;

        queryPedido.bindValue(":estado",estado);
        queryPedido.bindValue(":id",nid);
        if(!queryPedido.exec())
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("ATENCIÓN, Falló el cambio de estado: %1").arg(queryPedido.lastError().text()),
                                 tr("Aceptar"));

    }
}

void Frmrecepcion_pedidos::on_tablaPedidos_doubleClicked(const QModelIndex &index)
{
    int nid =ui->tablaPedidos->model()->data(ui->tablaPedidos->model()->index(index.row(),0),Qt::EditRole).toInt();
    id_pedido = nid;
    QSqlQuery query_lineas(QSqlDatabase::database("empresa"));
    query_lineas.prepare("select * from lin_ped_pro where id_cab = :nid");
    query_lineas.bindValue(":nid",nid);
    int pos = 0;
    ui->tablaLineas->setRowCount(0);
    ui->tablaLineas->clear();
    ui->tablaLineas->setColumnCount(11);
    QStringList cabecera;
    cabecera <<tr("id") << tr("C.prov") << tr("Cod.") <<tr("Descripción") << tr("Cant") <<tr("Pdte.")
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
            item_columna0->setTextColor(Qt::blue); // color de los items
            ui->tablaLineas->setItem(pos,0,item_columna0);
            ui->tablaLineas->setColumnHidden(0,true);

            QTableWidgetItem *item_columna1 = new QTableWidgetItem(query_lineas.record().value("codigo_articulo_proveedor").toString());
            item_columna1->setFlags(item_columna1->flags() & (~Qt::ItemIsEditable));
            item_columna1->setTextColor(Qt::blue); // color de los items
            ui->tablaLineas->setItem(pos,1,item_columna1);

            QTableWidgetItem *item_columna2 = new QTableWidgetItem(query_lineas.record().value("codigo_articulo_interno").toString());
            item_columna2->setFlags(item_columna2->flags() & (~Qt::ItemIsEditable));
            item_columna2->setTextColor(Qt::blue); // color de los items
            ui->tablaLineas->setItem(pos,2,item_columna2);

            QTableWidgetItem *item_columna3 = new QTableWidgetItem(query_lineas.record().value("descripcion").toString());
            item_columna3->setFlags(item_columna3->flags() & (~Qt::ItemIsEditable));
            item_columna3->setTextColor(Qt::blue); // color de los items
            ui->tablaLineas->setItem(pos,3,item_columna3);
            ui->tablaLineas->setColumnWidth(3,140);

            QTableWidgetItem *item_columna4 = new QTableWidgetItem(query_lineas.record().value("cantidad").toString());
            item_columna4->setFlags(item_columna4->flags() & (~Qt::ItemIsEditable));
            item_columna4->setTextColor(Qt::blue); // color de los items
            ui->tablaLineas->setItem(pos,4,item_columna4);
            ui->tablaLineas->setColumnWidth(4,60);

            QTableWidgetItem *item_columna5 = new QTableWidgetItem(query_lineas.record().value("cantidad_pendiente").toString());
            item_columna5->setFlags(item_columna5->flags() & (~Qt::ItemIsEditable));
            item_columna5->setTextColor(Qt::blue); // color de los items
            ui->tablaLineas->setItem(pos,5,item_columna5);
            ui->tablaLineas->setColumnWidth(5,60);

            QTableWidgetItem *item_columna6 = new QTableWidgetItem(query_lineas.record().value("cantidad_recibida").toString());
            if(query_lineas.record().value("cantidad_recibida").toInt()>0)
                item_columna6->setBackgroundColor(Qt::yellow);
            if(query_lineas.record().value("cantidad_pendiente").toInt() <=0)
                item_columna6->setBackgroundColor(Qt::green);
            if(query_lineas.record().value("cantidad_recibida").toInt()==0 )
                item_columna6->setBackgroundColor(Qt::white);

            item_columna6->setTextColor(Qt::blue); // color de los items
            item_columna6->setFlags(item_columna6->flags() & (~Qt::ItemIsEditable));
            item_columna6->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            ui->tablaLineas->setItem(pos,6,item_columna6);


            QTableWidgetItem *item_columna7 = new QTableWidgetItem("0");
            item_columna7->setTextColor(Qt::black); // color de los items
            item_columna7->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            ui->tablaLineas->setItem(pos,7,item_columna7);

            QTableWidgetItem *item_columna8 = new QTableWidgetItem(QString::number(query_lineas.record().value("coste_bruto").toDouble(),'f',2));
            item_columna8->setTextColor(Qt::black); // color de los items
            ui->tablaLineas->setItem(pos,8,item_columna8);
            ui->tablaLineas->setItemDelegateForColumn(8,new MonetaryDelegate);

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
    ui->btnAlbaran->setEnabled(true);
    ui->btnFactura->setEnabled(true);
    ui->btnImprimir->setEnabled(true);
    ui->btnEtiquetas->setEnabled(true);

    connect(ui->tablaLineas,SIGNAL(cellChanged(int,int)),this,SLOT(validarcantidad(int, int)));
    ui->tablaLineas->setFocus();
    if(ui->tablaLineas->rowCount()>0)
        ui->tablaLineas->setCurrentCell(0,7);

}

void Frmrecepcion_pedidos::validarcantidad(int row, int col)
{

    if (col ==7 ){
        blockSignals(true);


        int pend = ui->tablaLineas->item(row,5)->text().toInt();
        int rec = ui->tablaLineas->item(row,6)->text().toInt();
        int rec_act = ui->tablaLineas->item(row,7)->text().toInt();
        int nuevo_pend = pend - rec_act;
        int nid = ui->tablaLineas->item(row,10)->text().toInt();
        rec = rec + rec_act;

        bool fallo = false;
        QSqlDatabase::database("Maya").transaction();
        QSqlDatabase::database("empresa").transaction();
        //----------------------------------------------------
        // Marco el pedido como recibido al cambiar una linea
        //----------------------------------------------------
        QSqlQuery queryCabecera(QSqlDatabase::database("empresa"));
        if(!queryCabecera.exec("update ped_pro set lRecibido = 1 where id ="+QString::number(this->id_pedido)))
            fallo = true;
        //---------------------------
        // Actualizo linea de pedido
        //---------------------------
        QSqlQuery queryLineas(QSqlDatabase::database("empresa"));
        queryLineas.prepare("update lin_ped_pro set cantidad_pendiente =:pend, cantidad_recibida = :rec where id =:id");
        queryLineas.bindValue(":pend",nuevo_pend);
        queryLineas.bindValue(":rec",rec);
        int linid = ui->tablaLineas->item(row,0)->text().toInt();
        queryLineas.bindValue(":id",linid);
        if(!queryLineas.exec()) {
            QMessageBox::warning(this,tr("ATENCIÓN"),
                                 tr("Fallo la modificación de pendientes: %1").arg(queryLineas.lastError().text()),
                                 tr("Aceptar"));
          fallo = true;}
        else {
            if(nuevo_pend!=0)
            {
                // ----------------------------------------
                //  Si albaran = true  añado Linea albarán
                // ----------------------------------------
                if(albaran){
                    QSqlQuery query_lin_alb_pro(QSqlDatabase::database("empresa"));
                    query_lin_alb_pro.prepare("INSERT INTO lin_alb_pro "
                                              "( id_cab,cCodigo,cDescripcion,rCoste,nDto,"
                                              "rPvp,nIva,nCantidad,rTotal,"
                                              "rDto,rSUBTOTAL,id_Articulo) VALUES "
                                              "(:id_cab,:cCodigo,:cDescripcion,:rCoste,"
                                              ":nDto,:rPvp,:nIva,:nCantidad,:rTotal"
                                              ":rDto,:rSUBTOTAL,:id_Articulo);");

                    // TODO - Terminar entrada lineas alabarán
                    
                }
                // -------------------
                // Actualizo artículo
                // -------------------

                int stockfisico, stockreal,nCPR;
                QSqlQuery queryProducto(QSqlDatabase::database("Maya"));
                if(queryProducto.exec("select stockfisico, nStockReal, nCantidadPendienteRecibir from articulos where id = " +
                                      QString::number(nid)))
                {
                    if(queryProducto.next())
                    {
                        stockfisico = queryProducto.record().value("stockfisico").toInt();
                        stockreal = queryProducto.record().value("nStockReal").toInt();
                        nCPR = queryProducto.record().value("nCantidadPendienteRecibir").toInt();


                        if(!queryProducto.exec("update articulos set stockfisico = stockfisico +" +QString::number(rec_act)+
                                               ",nStockReal = nStockReal+"+QString::number(rec_act)+
                                               ",nCantidadPendienteRecibir = nCantidadPendienteRecibir-"+QString::number(rec_act)+
                                               " where id = "+QString::number(nid)))
                        {
                            QMessageBox::warning(this,tr("ATENCIÓN"),
                                                 tr("Falló la actualización de stock %1").arg(queryProducto.lastError().text()),
                                                 tr("Aceptar"));
                            fallo = true;

                        }
                    }

                }
            }

        }
        if (!fallo)
        {
            QSqlDatabase::database("empresa").commit();
            QSqlDatabase::database("Maya").commit();
        } else
        {
            QSqlDatabase::database("empresa").rollback();
            QSqlDatabase::database("Maya").rollback();
        }

        ui->tablaLineas->item(row,5)->setText(QString::number(nuevo_pend));

        ui->tablaLineas->item(row,6)->setText(QString::number(rec));
        ui->tablaLineas->item(row,7)->setText("0");
        if(rec>0)
            ui->tablaLineas->item(row,6)->setBackgroundColor(Qt::yellow);
        if(nuevo_pend <=0)
            ui->tablaLineas->item(row,6)->setBackgroundColor(Qt::green);
        if(rec ==0 )
            ui->tablaLineas->item(row,6)->setBackgroundColor(Qt::white);
    }
    blockSignals(false);


    if(col ==8 ) // Coste
    {
        int id = ui->tablaLineas->item(row,10)->text().toInt();
        QSqlQuery queryProducto(QSqlDatabase::database("Maya"));
        queryProducto.prepare("select rCoste from articulos where id =:nid");
        queryProducto.bindValue(":nid",id);
        if(queryProducto.exec())
        {
            queryProducto.next();
            double coste = queryProducto.record().value("rCoste").toDouble();
            QString svalor = ui->tablaLineas->item(row,8)->text();
            double valor = Configuracion_global->MonedatoDouble(svalor);

            if(coste != valor)
            {
                //----------------------------
                // Cambio pvp en linea_pedido
                //----------------------------
                int id_linea = ui->tablaLineas->item(row,0)->text().toInt();
                QSqlQuery queryLineas(QSqlDatabase::database("empresa"));
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
                    queryProducto.prepare("update articulos set rCoste = :coste where id =:id");
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
                            frmEditar_tarifas frmtarifas;
                            frmtarifas.setFiltro(id);
                            frmtarifas.exec();
                        }
                    }
                }

            }
        }

    }
    blockSignals(false);
    // connect(ui->tablaLineas,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(validarcantidad(QTableWidgetItem*)));
}

void Frmrecepcion_pedidos::reconectar()
{
 //   connect(ui->tablaLineas,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(validarcantidad(QTableWidgetItem*)));
}

void Frmrecepcion_pedidos::abrir_albaran()
{
    if(albaran == false) {
        //--------------------------------------
        // Creo cabecera en blanco del albarán
        //--------------------------------------
        ui->lblAlbaran->setVisible(true);
        ui->lblfecha_albaran->setVisible(true);
        ui->txtAlbaran->setVisible(true);
        ui->txtFecha_albaran->setVisible(true);

        ui->btnAlbaran->setText(tr("Crear y cerrar Albarán"));
        ui->txtAlbaran->setFocus();
        albaran =  true;
        QSqlQuery query_albaran(QSqlDatabase::database("empresa"));
        if(query_albaran.exec("insert into alb_pro  (cAlbaran) values ('temp')"))
            id_albaran = query_albaran.lastInsertId().toInt();
        else
            QMessageBox::warning(this,tr("Recepción Pedidos"),
                                 tr("No se pudo crear el albarán, Error:")+query_albaran.lastError().text(),
                                 tr("Aceptar"));
    } else
    {
        QSqlQuery query_pedido(QSqlDatabase::database("empresa"));
        query_pedido.exec("Select * from ped_pro where id = "+QString::number(id_pedido));
        query_pedido.next();
        QDate fecha;
        fecha = ui->txtFecha_albaran->date();

        //----------------------------
        // Rellenar cabecera Albarán
        //----------------------------


        QSqlQuery query_albaran(QSqlDatabase::database("empresa"));
        query_albaran.prepare("update alb_pro  set cAlbaran =:albaran, dFecha = :fecha,id_Proveedor=:id_proveedor,"
                              "cProveedor =:proveedor, cCifProveedor =:cif,nPedido =:pedido,"
                              "nIva1 =:iva1, nIva2 =:iva2, nIva3 =:iva3, nIva4 =:iva4, "
                              "rBase1 =:base1,rBase2 =:base2,rBase3 =:base3, rBase4 =:base4, "
                              "rIva1 =:riva1,rIva2 =:riva2, rIva3 =:riva3, rIva4 =:riva4, "
                              "rDto1 =:dto1,rDto2 =:dto2, rDto3 =:dto3, rDto4 =:dto4, "
                              "rTotal1 =:total1,rTotal2 =:total2,rTotal3 =:total3,rTotal4 =:total4 where id =:id");
        query_albaran.bindValue(":albaran",ui->txtAlbaran->text());
        query_albaran.bindValue(":fecha",fecha);
        query_albaran.bindValue(":id_proveedor",query_pedido.record().value("id_Proveedor").toInt());
        query_albaran.bindValue(":proveedor",query_pedido.record().value("cProveedor").toString());
        query_albaran.bindValue(":cif",query_pedido.record().value("cCifNif").toString());
        query_albaran.bindValue(":pedido",ui->txtNumPedido->text().toInt());
        query_albaran.bindValue(":iva1",Configuracion_global->ivaList.at(0));
        query_albaran.bindValue(":iva2",Configuracion_global->ivaList.at(1));
        query_albaran.bindValue(":iva3",Configuracion_global->ivaList.at(2));
        query_albaran.bindValue(":iva4",Configuracion_global->ivaList.at(3));
        
        query_albaran.bindValue(":id",id_albaran);
        
        // ----------------------
        // Sumar líneas albarán
        // ----------------------
        QSqlQuery query_lin_alb_pro(QSqlDatabase::database("empresa"));
        query_lin_alb_pro.prepare("select * from lin_alb_pro where id_cab = :id");
        query_lin_alb_pro.bindValue(":id",id_albaran);
        double base1,base2,base3,base4,iva1,iva2,iva3,iva4,dto1,dto2,dto3,dto4,total1,total2,total3,total4;
        base1 = 0; base2 = 0; base3 = 0; base4 = 0;
        iva1 = 0;  iva2 = 0;  iva3 = 0;  iva4 = 0;
        dto1 = 0; dto2 = 0; dto3 = 0; dto4 = 0;
        total1 = 0; total2 = 0; total3 = 0; total4 = 0;
        
        if(query_lin_alb_pro.exec())
        {
            while (query_lin_alb_pro.next())
            {

                if(query_lin_alb_pro.record().value("nIva").toDouble() == query_lin_alb_pro.record().value("nIva1").toDouble())
                {
                    base1 &+query_lin_alb_pro.record().value("rSUBTOTAL").toDouble();
                    iva1 &+query_lin_alb_pro.record().value("rIva").toDouble();
                    dto1 &+query_lin_alb_pro.record().value("rDto").toDouble();
                    total1 &+query_lin_alb_pro.record().value("rTotal").toDouble();
                }
                if(query_lin_alb_pro.record().value("nIva").toDouble() == query_lin_alb_pro.record().value("nIva2").toDouble())
                {
                    base2 &+query_lin_alb_pro.record().value("rSUBTOTAL").toDouble();
                    iva2 &+query_lin_alb_pro.record().value("rIva").toDouble();
                    dto2 &+query_lin_alb_pro.record().value("rDto").toDouble();
                    total2 &+query_lin_alb_pro.record().value("rTotal").toDouble();
                }
                if(query_lin_alb_pro.record().value("nIva").toDouble() == query_lin_alb_pro.record().value("nIva3").toDouble())
                {
                    base3 &+query_lin_alb_pro.record().value("rSUBTOTAL").toDouble();
                    iva3 &+query_lin_alb_pro.record().value("rIva").toDouble();
                    dto3 &+query_lin_alb_pro.record().value("rDto").toDouble();
                    total3 &+query_lin_alb_pro.record().value("rTotal").toDouble();
                }
                if(query_lin_alb_pro.record().value("nIva").toDouble() == query_lin_alb_pro.record().value("nIva4").toDouble())
                {
                    base4 &+query_lin_alb_pro.record().value("rSUBTOTAL").toDouble();
                    iva4 &+query_lin_alb_pro.record().value("rIva").toDouble();
                    dto4 &+query_lin_alb_pro.record().value("rDto").toDouble();
                    total4 &+query_lin_alb_pro.record().value("rTotal").toDouble();
                }
                
            }
        }
        query_albaran.bindValue(":base1",base1);
        query_albaran.bindValue(":base2",base2);
        query_albaran.bindValue(":base3",base3);
        query_albaran.bindValue(":base4",base4);
        query_albaran.bindValue(":riva1",iva1);
        query_albaran.bindValue(":riva2",iva2);
        query_albaran.bindValue(":riva3",iva3);
        query_albaran.bindValue(":riva4",iva4);
        query_albaran.bindValue(":rdto1",dto1);
        query_albaran.bindValue(":rdto2",dto2);
        query_albaran.bindValue(":rdto3",dto3);
        query_albaran.bindValue(":rdto4",dto4);
        query_albaran.bindValue(":rtotal1",total1);
        query_albaran.bindValue(":rtotal2",total2);
        query_albaran.bindValue(":rtotal3",total3);
        query_albaran.bindValue(":rtotal4",total4);
        
        if(!query_albaran.exec())
            QMessageBox::warning(this,tr("Recepción Pedidos"),
                                 tr("No se pudo crear el albarán, Error:")+query_albaran.lastError().text(),
                                 tr("Aceptar"));
        else {
            QString cadena = tr("Se ha creado el albarán: ");
            cadena.append(ui->txtAlbaran->text());
            cadena.append("\nde : ");
            cadena.append(query_pedido.record().value("cProveedor").toString());
            cadena.append("\ny de fecha:");
            cadena.append(fecha.toString("dd/MM/yyyy"));
            QMessageBox::warning(this,tr("Recepción pedidos"),cadena,
                                 tr("Aceptar"));
            id_albaran = 0;
        }
        ui->lblAlbaran->setVisible(false);
        ui->txtAlbaran->setText("");
        ui->txtAlbaran->setVisible(false);
        ui->txtFecha_albaran->setDate(QDate::currentDate());
        ui->lblfecha_albaran->setVisible(false);
        ui->txtFecha_albaran->setVisible(false);
        ui->btnAlbaran->setText("Abrir albarán");
        albaran = false;
    }
}
