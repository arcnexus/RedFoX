#include "frmrecepcion_pedidos.h"
#include "ui_frmrecepcion_pedidos.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Almacen/frmtarifas.h"
#include "../Busquedas/frmbuscarcie.h"
#include "../Almacen/frmaddlotes.h"
#include "../Busquedas/db_consulta_view.h"

#include "dlgpedidoalbfact.h"
#include "../Almacen/articulo.h"
#include "../Zona_Compras/proveedor.h"

Frmrecepcion_pedidos::Frmrecepcion_pedidos(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::Frmrecepcion_pedidos),
    menuButton(QIcon(":Icons/PNG/recepcion_ped.png"),tr("Recepción de pedidos"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/recepcion_ped.png"),"",this))
{
    ui->setupUi(this);
    ui->txtFechaFin->setDate(QDate::currentDate());

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

        if(rec_now > 0) //unidades recibidas
        {
            QSqlQuery queryCabecera(Configuracion_global->empresaDB);
            QString error;

            QHash<QString,QVariant> _line_data;
            _line_data["cantidad_pendiente"] = pend_now;
            _line_data["cantidad_recibida"] = recibi + rec_now;
            _lineas[line_id].setValue("cantidad_recibida",_line_data.value("cantidad_recibida"));

            bool ok = SqlCalls::SqlUpdate(_line_data,"lin_ped_pro",Configuracion_global->empresaDB,QString("id=%1").arg(line_id),error);

            //STOCK ARTICULO
            ok &= Articulo::set_pendiente_recibir(art_id, -1.0 * rec_now);
            ok &= Articulo::agregar_stock_fisico(art_id,rec_now);

            //LOTES
            if(SqlCalls::SelectOneField("articulos","lotes",QString("id=%1").arg(art_id),Configuracion_global->groupDB,error).toBool())
            {
                frmaddLotes frmLotes(this);
                frmLotes.cargar_articulo(art_id);
                frmLotes.exec();
            }

            if(SqlCalls::SelectRecord("lin_ped_pro",QString("id_cab = %1 AND cantidad_pendiente > 0").arg(id_cab),Configuracion_global->empresaDB,error)
                    .isEmpty())
            {
                ok &=  queryCabecera.exec(QString("update ped_pro set recibido = 1,recibido_completo = 1 where id = %1").arg(id_cab));
                if(!ok)
                    error = queryCabecera.lastError().text();
            }
            else
            {
                ok &=  queryCabecera.exec(QString("update ped_pro set recibido = 1 where id = %1").arg(id_cab));
                if(!ok)
                    error = queryCabecera.lastError().text();
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

void Frmrecepcion_pedidos::crear_documento(bool es_albaran)
{
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
                                     "Dichas lineas no se usarán para crear %1.").arg(es_albaran ? tr("el albarán") : tr ("la factura")));
    }
    DlgPedidoAlbFact dlg(tr("Creación de %1").arg(es_albaran ? tr("Albarán") : tr("Factura")),ui->txtproveedor->text(),this);
    dlg.setLineas(selected_lineas);
    dlg.set_proveedor(id_prov);
    dlg.set_tipo(es_albaran ? DlgPedidoAlbFact::Albaran : DlgPedidoAlbFact::Factura);
    dlg.exec();
}

void Frmrecepcion_pedidos::abrir_albaran()
{
   crear_documento(true);
}

void Frmrecepcion_pedidos::on_btnFactura_clicked()
{
   crear_documento(false);
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
    cabecera  << "id" << tr("Código") << tr("Proveedor") << tr("DNI/CIF/NIE") << tr("Población");
    tamanos <<"0" << "100" << "300" << "180" << "300";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de proveedores");
    if(consulta.exec())
    {
        ui->txtproveedor->setText(consulta.get_record().value("proveedor").toString());
        id_prov = consulta.get_record().value("id").toInt();
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
            <<tr("Recibidos") <<tr("Acabo Recibir") <<tr("Etiquetas");
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
