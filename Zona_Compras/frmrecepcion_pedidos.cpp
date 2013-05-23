#include "frmrecepcion_pedidos.h"
#include "ui_frmrecepcion_pedidos.h"
#include "Auxiliares/monetarydelegate.h"
#include "Almacen/frmeditar_tarifas.h"
#include "Busquedas/frmbuscarcie.h"


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
            ui->tablaLineas->setColumnWidth(3,150);

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
    connect(ui->tablaLineas,SIGNAL(cellChanged(int,int)),this,SLOT(validarcantidad(int, int)));

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
        QSqlQuery queryLineas(QSqlDatabase::database("empresa"));
        queryLineas.prepare("update lin_ped_pro set cantidad_pendiente =:pend, cantidad_recibida = :rec where id =:id");
        queryLineas.bindValue(":pend",nuevo_pend);
        queryLineas.bindValue(":rec",rec);
        queryLineas.bindValue(":id",nid);
        if(!queryLineas.exec()) {
            QMessageBox::warning(this,tr("ATENCIÓN"),
                                 tr("Fallo la modificación de pendientes: %1").arg(queryLineas.lastError().text()),
                                 tr("Aceptar"));
          fallo = true;}
        else {
            if(nuevo_pend!=0)
            {


                int stockfisico, stockreal,nCPR;
                QSqlQuery queryProducto(QSqlDatabase::database("Maya"));
                if(queryProducto.exec("select stockfisico, nStockReal, nCantidadPendienteRecibir from articulos where id = " +QString::number(nid)))
                {
                    queryProducto.next();
                    stockfisico = queryProducto.record().value("stockfisico").toInt();
                    stockreal = queryProducto.record().value("nStockReal").toInt();
                    nCPR = queryProducto.record().value("nCantidadPendienteRecibir").toInt();

                    if(!queryProducto.exec("update articulos set stockfisico = " +QString::number(stockfisico+rec_act)+
                                           ",nStockReal = "+QString::number(stockfisico+rec_act+nuevo_pend)+
                                           ",nCantidadPendienteRecibir ="+QString::number(nCPR -rec_act)+
                                           " where id = "+QString::number(nid)))
                    {
                        QMessageBox::warning(this,tr("ATENCIÓN"),
                                             tr("Falló la actualización de stock %1").arg(queryProducto.lastError().text()),
                                             tr("Aceptar"));
                        fallo = true;

                    } else {
                        qDebug() << queryProducto.lastQuery();
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
    ui->lblAlbaran->setVisible(true);
    ui->lblfecha_albaran->setVisible(true);
    ui->txtAlbaran->setVisible(true);
    ui->txtFecha_albaran->setVisible(true);
    ui->btnAlbaran->setText(tr("Crear y cerrar Albarán"));

}
