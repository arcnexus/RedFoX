#include "table_helper.h"
#include "monetarydelegate.h"
#include "../Almacen/frmarticulos.h"
#include "../Busquedas/db_consulta_view.h"

Table_Helper::Table_Helper(QObject *parent) :
    QObject(parent)
{
    helped_table = 0;
    moneda = "€";
    comprando = true;
    use_re = false;
    m_cantidadArticulo=0;
    isActive = false;
    connect(this,SIGNAL(i_recalc()),this,SLOT(recalc()));

}

Table_Helper::~Table_Helper()
{
    foreach (lineaDetalle* d, m_rows) {
        delete d;
    }
}

void Table_Helper::help_table(QTableWidget *table)
{
    this->helped_table = table;
   /// helped_table->setItemDelegateForColumn(0,new SearchDelegate(helped_table));
    helped_table->setItemDelegateForColumn(1,new SpinBoxDelegate(helped_table));
    helped_table->setItemDelegateForColumn(3,new MonetaryDelegate(helped_table,true));
    helped_table->setItemDelegateForColumn(4,new MonetaryDelegate(helped_table,false));
    helped_table->setItemDelegateForColumn(6,new MonetaryDelegate(helped_table,true));
    helped_table->setItemDelegateForColumn(5,new SpinBoxDelegate(helped_table,true,0,100));
    helped_table->setItemDelegateForColumn(7,new ReadOnlyDelegate(helped_table)); // el tipo de iva pertenece al artículo y solo se puede modificar en a ficha del artículo.
    helped_table->setItemDelegateForColumn(8,new ReadOnlyDelegate(helped_table)); // el tipo de re pertenece al artículo y solo se puede modificar en a ficha del artículo.
    helped_table->setItemDelegateForColumn(9,new MonetaryDelegate(helped_table,false));
    helped_table->setColumnWidth(2,220);
    helped_table->verticalHeader()->setVisible(false);



    QStringList headers;
    if (!this->comprando) {
        headers << tr("Referencia") << tr("Cantidad") << tr("Descripcion") << tr("P.V.P") << tr("subtotal");
        headers  << tr("%DTO") << tr("DTO") << tr("%I.V.A.") <<tr("%R.E.") << tr("Total");
    } else
    {
        headers << tr("Codigo") << tr("Cantidad") << tr("Descripcion") << tr("COSTE") << tr("subtotal");
        headers << tr("%DTO") << tr("DTO") << tr("%I.V.A.") <<tr("%R.E.") << tr("Total");
    }

    helped_table->setHorizontalHeaderLabels(headers);
//    resizeTable();

    connect(helped_table,SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),this,SLOT(handle_currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)));
    helped_table->installEventFilter(this);
}

void Table_Helper::setDatabase(QString db, QString table)
{
    m_db = db;
    m_db_table = table;
}

void Table_Helper::set_moneda(QString moneda)
{
    this->moneda = moneda;
}

void Table_Helper::set_tarifa(int tarifa)
{
    this->tarifa = tarifa;
}

void Table_Helper::set_tipo_dto_tarifa(int tipo_dto_tarifa)
{
    this->tipo_dto_tarifa = tipo_dto_tarifa;
}

void Table_Helper::set_Tipo(bool is_compra)
{
    comprando = is_compra;
}

void Table_Helper::blockTable(bool state)
{
    isActive = !state;
    if(helped_table)
    {
        if(state)
            helped_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        else
        {
            helped_table->setEditTriggers(QAbstractItemView::CurrentChanged|QAbstractItemView::AnyKeyPressed|QAbstractItemView::DoubleClicked);
        }
    }
}

void Table_Helper::set_UsarRE(bool state)
{
    use_re = state;
    emit i_recalc();
}

void Table_Helper::resizeTable()
{
    if(helped_table)
    {
        helped_table->horizontalHeader()->setUpdatesEnabled(false);

        for (int i = 0; i < helped_table->columnCount(); i++)
            helped_table->horizontalHeader()->resizeSection(i, 90);


        int columnCount = helped_table->columnCount()-1;
        int wDesc = helped_table->horizontalHeader()->sectionSize(2);
        int wLast = helped_table->horizontalHeader()->sectionSize(columnCount)-10;
        helped_table->horizontalHeader()->resizeSection(2,wLast);
        helped_table->horizontalHeader()->resizeSection(columnCount,wDesc);

        helped_table->horizontalHeader()->setUpdatesEnabled(true);
    }
}

void Table_Helper::fillTable(QString db, QString table, QString filter)
{
    helped_table->blockSignals(true);
    helped_table->clearContents();
    helped_table->setRowCount(0);

    foreach (lineaDetalle* d, m_rows) {
        delete d;
    }
    m_rows.clear();

    QSqlQuery query(QSqlDatabase::database(db));
    QString sql;
    if(filter.isEmpty())
        sql = QString("SELECT * FROM %1").arg(table);
//    else
//    {
//        int ntabla;
//        ntabla = 0;

//        if(table =="lin_ped_pro")
//            ntabla = 1;
//        if(table =="lin_alb_pro")
//            ntabla = 2;
//        if(table =="lin_fac_pro")
//            ntabla = 3;

//        switch (ntabla) {
//        case 1:
//            sql = QString("select id,id_cab,id_articulo,codigo_articulo_proveedor,cantidad,descripcion,precio,subtotal_coste,dto,porc_dto,porc_iva,total"
//                          " from %1 where %2").arg(table).arg(filter);
//            break;
//        case 2:
//            sql = QString("select id,id_cab,id_articulo,codigo_articulo_proveedor,cantidad,descripcion,precio,subtotal,dto,dto,iva,total"
//                          " from %1 where %2").arg(table).arg(filter);
//            break;
//        case 3:
//            sql = QString("select id,id_cab,id_articulo,codigo_articulo_proveedor,cantidad,descripcion,precio,subtotal,dto,dto,iva,total"
//                          " from %1 where %2").arg(table).arg(filter);

        else
           sql = QString("SELECT * FROM %1 WHERE %2").arg(table).arg(filter);
//            break;
//        }
//    }
    if(query.exec(sql))
    {
        while(query.next())
            addRow(query.record());
    }
    helped_table->blockSignals(false);
    emit i_recalc();
}

void Table_Helper::addRow()
{
    if(helped_table)
    {

        if(helped_table->rowCount()>0)
            updateLinea(helped_table->rowCount()-1);

        resizeTable();
        helped_table->blockSignals(true);
        helped_table->setRowCount(helped_table->rowCount()+1);
        int row = helped_table->rowCount()-1;

        QTableWidgetItem * item0 = new QTableWidgetItem;
        helped_table->setItem(row,0,item0);

        QTableWidgetItem * item = new QTableWidgetItem;
        helped_table->setItem(row,1,item);
        item->setText("1");

        for(int i = 2;i < helped_table->columnCount();i++)
        {
            QTableWidgetItem * itemX = new QTableWidgetItem;
            helped_table->setItem(row,i,itemX);
        }

        QTableWidgetItem * subtotal = helped_table->item(row,4);
        subtotal->setFlags(subtotal->flags()^Qt::ItemIsEnabled);
        subtotal->setText("0");

        QTableWidgetItem * porc_re = helped_table->item(row,8);
        porc_re->setFlags(porc_re->flags()^Qt::ItemIsEnabled);
        porc_re->setText("0");

        QTableWidgetItem * total = helped_table->item(row,9);
        total->setFlags(total->flags()^Qt::ItemIsEnabled);
        total->setText("0");

        helped_table->item(row,3)->setText("0");
        helped_table->item(row,5)->setText("0");
        helped_table->item(row,6)->setText("0");
//        QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
//        for (int i=0;i<keys.size();i++)
//            if(Configuracion_global->ivas[keys.at(i)].value("nombre_interno") == "base1")
//                helped_table->item(row,7)->setText(keys.at(i));
        helped_table->item(row,7)->setText("0");
        helped_table->item(row,8)->setText("0");
        helped_table->item(row,9)->setText("0");

        //-----------------
        // Alineación
        //-----------------
        helped_table->item(row,1)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        helped_table->item(row,3)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        helped_table->item(row,4)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        helped_table->item(row,5)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        helped_table->item(row,6)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        helped_table->item(row,7)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        helped_table->item(row,8)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        helped_table->item(row,9)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        helped_table->blockSignals(false);
        helped_table->setFocus();
        helped_table->setCurrentCell(helped_table->rowCount()-1,0);
        helped_table->editItem(helped_table->item(helped_table->currentRow(),helped_table->currentColumn()));
        lineaDetalle * lrow = new lineaDetalle;
        lrow->idLinea = -1;
        m_rows.append(lrow);
    }
}
lineaDetalle * Table_Helper::getLineaDetalleFromRecord(QSqlRecord r)
{
    lineaDetalle * lrow = new lineaDetalle;

    lrow->idLinea = -1;
    lrow->idLinea = r.value("id").toInt();
    lrow->codigo = r.value("codigo").toString();
    lrow->cantidad = r.value("cantidad").toDouble();
    lrow->cantidad_old = r.value("cantidad").toDouble();
    lrow->descripcion = r.value("descripcion").toString();
    lrow->precio = r.value("precio").toDouble();
    lrow->subtotal = r.value("subtotal").toDouble();
    lrow->dto = r.value("dto").toDouble();
    lrow->dto_perc = r.value("porc_dto").toFloat();
    lrow->iva_perc = r.value("porc_iva").toFloat();
    lrow->rec_perc = r.value("porc_rec").toFloat();

    return lrow;
}

void Table_Helper::addRow(QSqlRecord r)
{
    resizeTable();

    helped_table->setRowCount(helped_table->rowCount()+1);
    disconnect(helped_table,SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),this,SLOT(handle_currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)));
    int row = helped_table->rowCount()-1;

    QTableWidgetItem * item0 = new QTableWidgetItem;
    helped_table->setItem(row,0,item0);

    QTableWidgetItem * item = new QTableWidgetItem;
    helped_table->setItem(row,1,item);
    item->setText("1");
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    for(int i = 2;i < helped_table->columnCount();i++)
    {
        QTableWidgetItem * itemX = new QTableWidgetItem;
        helped_table->setItem(row,i,itemX);
    }

    QTableWidgetItem * subtotal = helped_table->item(row,4);
    subtotal->setFlags(subtotal->flags()^Qt::ItemIsEnabled);
    subtotal->setText("0");

    QTableWidgetItem * porc_re = helped_table->item(row,8);
    porc_re->setFlags(porc_re->flags()^Qt::ItemIsEnabled);
    porc_re->setText("0");
    porc_re->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QTableWidgetItem * total = helped_table->item(row,9);
    total->setFlags(total->flags()^Qt::ItemIsEnabled);
    total->setText("0");
    helped_table->item(row,3)->setText("0");
    helped_table->item(row,5)->setText("0");
    helped_table->item(row,6)->setText("0");
    helped_table->item(row,5)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    helped_table->item(row,7)->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);


    helped_table->blockSignals(false);
    helped_table->setFocus();
    helped_table->setCurrentCell(helped_table->rowCount()-1,0);
    helped_table->editItem(helped_table->item(helped_table->currentRow(),helped_table->currentColumn()));

    lineaDetalle* lrow = getLineaDetalleFromRecord(r);

    helped_table->item(row,0)->setText(r.value("codigo").toString());
    helped_table->item(row,1)->setText(r.value("cantidad").toString());
    helped_table->item(row,2)->setText(r.value("descripcion").toString());
    helped_table->item(row,3)->setText(QString::number(r.value("precio").toDouble(),'f',Configuracion_global->decimales));
    helped_table->item(row,4)->setText(QString::number(r.value("subtotal").toDouble(),'f',Configuracion_global->decimales_campos_totales));
    helped_table->item(row,5)->setText(QString::number(r.value("porc_dto").toDouble(),'f',Configuracion_global->decimales_campos_totales));
    helped_table->item(row,6)->setText(QString::number(r.value("dto").toDouble(),'f',Configuracion_global->decimales_campos_totales));
    helped_table->item(row,7)->setText(QString::number(r.value("porc_iva").toDouble(),'f',Configuracion_global->decimales_campos_totales));
    helped_table->item(row,8)->setText(QString::number(r.value("porc_rec").toDouble(),'f',Configuracion_global->decimales_campos_totales));
    m_rows.append(lrow);

//    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
//    for(int i = 0;i<keys.size();i++)
//    {
//        if(Configuracion_global->ivas[keys.at(i)].value("iva").toDouble() == r.value("porc_iva").toDouble())
//        {
//            helped_table->item(row,7)->setText(keys.at(i));
//            break;
//        }
//    }
    connect(helped_table,SIGNAL(currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)),this,SLOT(handle_currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)));
}


void Table_Helper::removeRow()
{
    if(helped_table)
    {
        QList<QTableWidgetItem*> items = helped_table->selectedItems();
        QMap<int, int> rowsMap;
        for(int i = 0; i < items.count(); i++)
        {
          rowsMap[items.at(i)->row()] = -1; //garbage value
        }
        QList<int> rowsList = rowsMap.uniqueKeys();
        qSort(rowsList);

        for(int i = rowsList.count() - 1; i >= 0; i--)
        {
            lineaDetalle * aux = new lineaDetalle;
            aux->idLinea = m_rows.at(i)->idLinea;
            //copiar todos los valores a 'aux'
            helped_table->removeRow(rowsList.at(i));
            m_rows.remove(rowsList.at(i));//esto borra rowList(i) de la memoria....
            emit lineaDeleted(aux);
        }
    }
    emit i_recalc();
}

void Table_Helper::handle_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    //guardar cantidad al entrar a editar
    if(current){
        if(current->column() == 1)
            m_rows[current->row()]->cantidad_old = current->text().toDouble();
        if(current->column() == 0)
            codigo_art = current->text();
    }
    if(previous)
    {
        int column = previous->column();
        int row = previous->row();
        helped_table->blockSignals(true);
        if (column == 0)
            rellenar_con_Articulo(row);
        else if(column == 1 && !helped_table->item(row,0)->text().isEmpty())
        {
            comprobarCantidad(row);
            //guardar cantidad al terminar de editar
            m_rows[row]->cantidad = previous->text().toDouble();
            calculatotalLinea(row);
        }
//        else if(column == 6 && !helped_table->item(row,4)->text().isEmpty())
//        {
//            calcPercDescuento(row);
//            comprobadto(row);
//        }
        else if(column == 5 && !helped_table->item(row,4)->text().isEmpty())
        {
            calcNetoDescuento(row);
            comprobadto(row);

        }
        else if(column == 10)
            comprobarStock(row);

        updateLinea(row);
        helped_table->blockSignals(false);

        emit i_recalc();
    }
}

void Table_Helper::recalc()
{
    calculatotal();
    calcularDesglose();
}

void Table_Helper::calculatotal()
{
    helped_table->blockSignals(true);
    double subtotal = 0;
    double base = 0;
    double dto = 0;
    double total = 0;
    double iva = 0;
    double re = 0;
    QString StrDto;
    for(int i = 0; i< helped_table->rowCount() ; i++)
    {
        subtotal +=calcularsubtotalLinea(i);
        base += calculabaseLinea(i);
        StrDto = Configuracion_global->toRound(calculadtoLinea(i),Configuracion_global->decimales_campos_totales);
        dto += Configuracion_global->MonedatoDouble(StrDto);
        //dto += calculadtoLinea(i);
        iva += calculaivaLinea(i);
        re += calcularRELinea(i);
        total += calculatotalLinea(i);
    }
    //double subtotal = base + dto;
    //base = subtotal -dto;
    helped_table->blockSignals(false);
    emit totalChanged( base , dto , subtotal ,  iva,  re,  total,  moneda);
}

double Table_Helper::calcularsubtotalLinea(int row)
{
    int cantidad = helped_table->item(row,1)->text().toInt();
    double pvp = helped_table->item(row,3)->text().toDouble();
    double subtotal = cantidad * pvp;
    return subtotal;
}

double Table_Helper::calculadtoLinea(int row)
{
    double subtotal = helped_table->item(row,4)->text().toDouble();
    double porc_dto = helped_table->item(row,5)->text().toDouble();
    double dto = subtotal *(porc_dto/100.0);
    return dto;

}

double Table_Helper::calculabaseLinea(int row)
{
    int cantidad = helped_table->item(row,1)->text().toInt();
    double pvp = helped_table->item(row,3)->text().toDouble();
    double subtotal = cantidad * pvp;
    double dto = calculadtoLinea(row);
    double base = subtotal -dto;
    return base;
}

double Table_Helper::calculaivaLinea(int row)
{
    double base = calculabaseLinea(row);
    double porc_iva,total_iva;
    //base -= calculadtoLinea(row);
    QString error;
    float grupo_iva = SqlCalls::SelectOneField("clientes","grupo_iva",
                                QString("id=%1").arg(id_cliente),Configuracion_global->groupDB,error).toFloat();
    if(grupo_iva ==1){
        porc_iva = helped_table->item(row,7)->text().toDouble();
        total_iva = base * (porc_iva/100);
    } else
        total_iva =0;

    return total_iva;
}

double Table_Helper::calcularRELinea(int row)
{
    QString error;
    float grupo_iva = SqlCalls::SelectOneField("clientes","grupo_iva",
                                QString("id=%1").arg(id_cliente),Configuracion_global->groupDB,error).toFloat();
    if(grupo_iva ==1){
        if(use_re)
        {
            int cantidad = helped_table->item(row,1)->text().toInt();
            double pvp = helped_table->item(row,3)->text().toDouble();
            double subtotal = cantidad * pvp;
            double dto = calculadtoLinea(row);
            double base = subtotal -dto;
            float porc_rec = helped_table->item(row,8)->text().toFloat();
            double rec = base * (porc_rec/100.1);
            return rec;

        } else
            return 0;
    } else
        return 0;
}

void Table_Helper::comprobarCantidad(int row)
{
    int cantidad = helped_table->item(row,1)->text().toInt();
    if (helped_table->columnCount() == 11)
        helped_table->item(row,10)->setText(QString::number(cantidad));
    int stock = 0;
    int stockMax = 0;
    int stockMin = 0;
    bool controlarStock = true;
    QString codigo = helped_table->item(row,0)->text();
    QSqlQuery query(Configuracion_global->groupDB);
    QString sql = QString("SELECT * FROM articulos WHERE codigo = '%1'").arg(codigo);
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next())
        {
            stock = query.record().value("stock_real").toInt();
            stockMax  = query.record().value("stock_maximo").toInt();
            stockMin  = query.record().value("stock_minimo").toInt();
            controlarStock = query.record().value("controlar_stock").toBool();

        }
     //   else
     //   {
     //       QMessageBox::critical(qApp->activeWindow(),tr("No encontado"),tr("Articulo no encontrado"),tr("Aceptar"));
     //   }
    } else{
        QMessageBox::critical(qApp->activeWindow(),tr("No encontado"),
                              tr("Error al buscar stock: %1").arg(query.lastError().text()),tr("Aceptar"));
    }
    if(comprando)
    {
        if((stock + cantidad) > stockMax && stockMax >0 && controlarStock)
            QMessageBox::warning(qApp->activeWindow(),tr("Stock superado"),
                                 tr("Con esta cantidad se superará el stock máximo establecido"),tr("Aceptar"));
    }
    else
    {
        if((stock - cantidad) < 0 && controlarStock)
        {
            QMessageBox::warning(qApp->activeWindow(),tr("Stock insuficiente"),
                                 tr("No existe suficiente stock para satisfacer esta cantidad"),tr("Aceptar"));
            if (helped_table->columnCount() == 11)
                helped_table->item(row,10)->setText(QString::number(stock));
        }
        else if ((stock - cantidad) < stockMin && controlarStock)
        {
            QMessageBox::warning(qApp->activeWindow(),tr("Stock bajo minimos"),
                                 tr("Con esta cantidad el stock estará por debajo del minimo establecido"),tr("Aceptar"));
            if (helped_table->columnCount() == 11)
                helped_table->item(row,10)->setText(QString::number(cantidad));
        }
    }
}

void Table_Helper::comprobarStock(int row)
{
    int cantidad = helped_table->item(row,1)->text().toInt();
    int aServir = helped_table->item(row,9)->text().toInt();
    int stock = 0;
    int stockMax = 0;
    int stockMin = 0;
    bool controlarstock = true;
    QString codigo = helped_table->item(row,0)->text();
    QSqlQuery query(Configuracion_global->groupDB);
    QString sql = QString("SELECT * FROM articulos WHERE codigo = '%1'").arg(codigo);
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next())
        {
            stock = query.record().value("stock_real").toInt();
            controlarstock = query.record().value("controlar_stock").toBool();

        }
       // else
       // {
       //     QMessageBox::critical(qApp->activeWindow(),tr("No encontado"),tr("Articulo no encontrado"),tr("Aceptar"));
       // }
    }
    if(aServir > cantidad && controlarstock)
        helped_table->item(row,10)->setText(QString::number(cantidad));
    if(aServir > stock && controlarstock)
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Stock insuficiente"),
                             tr("No existe suficiente stock para satisfacer esta cantidad"),tr("Aceptar"));
        helped_table->item(row,10)->setText(QString::number(cantidad));
    }
}

void Table_Helper::comprobadto(int row)
{
    double subtotal = helped_table->item(row,4)->text().toDouble();
    double dto = helped_table->item(row,6)->text().toDouble();
    if((subtotal - dto) < 0)
        QMessageBox::warning(qApp->activeWindow(),tr("Descuento excesivo"),
                             tr("Descuento mayor del valor del producto"),tr("Aceptar"));
}

double Table_Helper::calculatotalLinea(int row)
{
    int cantidad = helped_table->item(row,1)->text().toInt();
    double pvp = helped_table->item(row,3)->text().toDouble();
    double subtotal = cantidad * pvp;
    helped_table->item(row,4)->setText(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales));

    double dto = calculadtoLinea(row);
    helped_table->item(row,6)->setText(QString::number(dto,'f',Configuracion_global->decimales_campos_totales));

    double total = subtotal - dto;

    helped_table->item(row,9)->setText(QString::number(total,'f',Configuracion_global->decimales_campos_totales));
    return total;
}

void Table_Helper::calcularDesglose()
{
    calcular_por_Base("base1");
    calcular_por_Base("base2");
    calcular_por_Base("base3");
    calcular_por_Base("base4");
}

void Table_Helper::calcular_por_Base(QString sbase)
{
    double base = 0;
    double iva = 0;
    double re = 0;
    double total = 0;
    for(int i=0; i<helped_table->rowCount();i++)
    {


    if(sbase == "base1")
    {
        if( helped_table->item(i,7)->text().toDouble() == this->porc_iva1)
        {
            base += calculabaseLinea(i);
            iva += calculaivaLinea(i);
            re += calcularRELinea(i);
            total += calculatotalLinea(i);
        }
        emit desglose1Changed(base, iva, re, total);

    }
    else if(sbase == "base2")
    {
        if( helped_table->item(i,7)->text().toDouble() == this->porc_iva2)
        {

            base += calculabaseLinea(i);
            iva += calculaivaLinea(i);
            re += calcularRELinea(i);
            total += calculatotalLinea(i);
        }
        emit desglose2Changed(base, iva, re, total);
    }
    else if(sbase == "base3"){
        if( helped_table->item(i,7)->text().toDouble() == this->porc_iva3)
        {
            base += calculabaseLinea(i);
            iva += calculaivaLinea(i);
            re += calcularRELinea(i);
            total += calculatotalLinea(i);
        }
        emit desglose3Changed(base, iva, re, total);
    }
    else if(sbase == "base4")
    {
        if( helped_table->item(i,7)->text().toDouble() == this->porc_iva4)
        {
            base += calculabaseLinea(i);
            iva += calculaivaLinea(i);
            re += calcularRELinea(i);
            total += calculatotalLinea(i);
        }
        emit desglose4Changed(base, iva, re, total);
    }
    }
}

void Table_Helper::rellenar_con_Articulo(int row)
{
    QString codigo = helped_table->item(row,0)->text();
    if(codigo =="*")
    {
       searchArticulo();
    } else
    {
        QSqlQuery query(Configuracion_global->groupDB);
        QString sql;
        if (this->comprando)
            sql = QString("SELECT * FROM vistaart_tarifa WHERE codigo_fabricante = '%1'").arg(codigo);
        else
            sql = QString("SELECT * FROM vistaart_tarifa WHERE codigo = '%1' and tarifa = %2").arg(codigo).arg(tarifa);
        query.prepare(sql);
        if(query.exec())
        {
            if(query.next() && codigo !=codigo_art)

            {
                QSqlRecord r = query.record();


                if(r.value("descripcion_reducida").toString().isEmpty())
                    helped_table->item(row,2)->setText(r.value("descripcion").toString());
                else
                    helped_table->item(row,2)->setText(r.value("descripcion_reducida").toString());

                if (this->comprando)
                {
                    double coste = r.value("coste").toDouble();
                    helped_table->item(row,0)->setText(r.value("codigo_fabricante").toString().toUpper());
                    helped_table->item(row,3)->setText(QString::number(coste,'f',Configuracion_global->decimales));
                }
                else
                {
                    helped_table->item(row,0)->setText(r.value("codigo").toString().toUpper());
                    double precio;
                    if(this->tipo_dto_tarifa == 1)
                        precio = r.value("pvp").toDouble()-(r.value("pvp").toDouble() *(r.value("porc_dto1").toDouble()/100));
                    else if (this->tipo_dto_tarifa == 2)
                        precio = r.value("pvp").toDouble()-(r.value("pvp").toDouble() *(r.value("porc_dto2").toDouble()/100));
                    else if (this->tipo_dto_tarifa == 3)
                        precio = r.value("pvp").toDouble()-(r.value("pvp").toDouble() *(r.value("porc_dto3").toDouble()/100));
                    else if (this->tipo_dto_tarifa == 4)
                        precio = r.value("pvp").toDouble()-(r.value("pvp").toDouble() *(r.value("porc_dto4").toDouble()/100));
                    else if (this->tipo_dto_tarifa == 5)
                        precio = r.value("pvp").toDouble()-(r.value("pvp").toDouble() *(r.value("porc_dto5").toDouble()/100));
                    else if (this->tipo_dto_tarifa == 6)
                        precio = r.value("pvp").toDouble()-(r.value("pvp").toDouble() *(r.value("porc_dto6").toDouble()/100));
                    else
                        precio = r.value("pvp").toDouble()-(r.value("pvp").toDouble());

                    helped_table->item(row,3)->setText(QString::number(precio,'f',Configuracion_global->decimales));
                    helped_table->item(row,4)->setText(QString::number(precio,'f',Configuracion_global->decimales));
                }
                helped_table->item(row,7)->setText(QString::number(r.value("tipo_iva").toFloat(),'f',
                                                                   Configuracion_global->decimales_campos_totales));
                if(use_re)
                {
                   float rec = Configuracion_global->devolver_rec_iva(r.value("tipo_iva").toFloat());
                   helped_table->item(row,8)->setText(QString::number(rec,'f',Configuracion_global->decimales_campos_totales));
                }
               Articulo oArt(this);
               float porc_dto = oArt.asigna_dto_linea(r.value("id").toInt(),id_cliente,0,0);
               helped_table->item(row,5)->setText(QString::number(porc_dto,'f',Configuracion_global->decimales_campos_totales));
               float dto = helped_table->item(row,4)->text().toDouble()*(porc_dto/100.0);
               helped_table->item(row,6)->setText(QString::number(dto,'f',Configuracion_global->decimales_campos_totales));
               double total = helped_table->item(row,4)->text().toDouble() - dto;
               helped_table->item(row,9)->setText(QString::number(total,'f',Configuracion_global->decimales));
               //-----------------
               // Alineación
               //-----------------
               helped_table->item(row,1)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
               helped_table->item(row,3)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
               helped_table->item(row,4)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
               helped_table->item(row,5)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
               helped_table->item(row,6)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
               helped_table->item(row,7)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
               helped_table->item(row,8)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
               helped_table->item(row,9)->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);



            }
            else if(helped_table->item(row,0)->text() !="" && helped_table->item(row,2)->text().isEmpty())
            {
                qDebug() << sql;
                if(QMessageBox::question(helped_table,
                                     tr("Codigo desconocido"),
                                     tr("Codigo de articulo desconocido "
                                        "¿Desea introducir el árticulo? (si no lo hace no se registraran acumulados)")
                                        ,tr("Usar de todos modos"),tr("Crear uno nuevo"))== QMessageBox::Accepted)
                {
                    FrmArticulos f((QWidget*)helped_table->parent(),true);
                    f.exec();
                }
            }
        }
    }
}

bool Table_Helper::eventFilter(QObject *target, QEvent *event)
{
    Q_UNUSED(target)
    if (event->type() == QEvent::KeyPress && isActive)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Down)
        {
            if(helped_table->currentRow() == (helped_table->rowCount()-1))
                addRow();
        }
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Tab)
        {

            if(helped_table->currentColumn() != helped_table->columnCount()-1)
            {
                if(helped_table->currentColumn()== 3 )
                    helped_table->setCurrentCell(helped_table->currentRow(),helped_table->currentColumn()+2);
                else if(helped_table->currentColumn()>= 6)
                {
                    if(helped_table->currentRow() != helped_table->rowCount()-1)
                    {
                        helped_table->setCurrentCell(helped_table->currentRow()+1,0);
                        helped_table->editItem(helped_table->item(helped_table->currentRow(),helped_table->currentColumn()));
                    }
                    else
                    addRow();
                    return true;
                }
                else
                    helped_table->setCurrentCell(helped_table->currentRow(),helped_table->currentColumn()+1);
                helped_table->editItem(helped_table->item(helped_table->currentRow(),helped_table->currentColumn()));
            }
            else
                addRow();
        }
        if(keyEvent->key() == Qt::Key_Escape)
        {
            searchArticulo();
            return false;
        }
        emit i_recalc();
    }    
    return false;
}

void Table_Helper::searchArticulo()
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
    cabecera  << tr("Código") << tr("Código Barras") << tr("Referencia") << tr("Descripción") << tr("Coste") <<tr("pvp");
    tamanos <<"0" << "100" << "100" << "100" << "320" <<"130" <<"130";
    moneda <<"5" <<"6";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_delegate_monetary(moneda);
    consulta.set_titulo("Busqueda de Artículos");
    consulta.set_filtro("");
    if(consulta.exec())
    {

        if(!comprando)
            helped_table->item(helped_table->currentRow(),0)->setText(Configuracion_global->devolver_codigo_articulo(consulta.get_id()));
        else
            helped_table->item(helped_table->currentRow(),0)->setText(Configuracion_global->devolver_referencia_articulo(consulta.get_id()));

        helped_table->setCurrentCell(helped_table->currentRow(),1);
        helped_table->editItem(helped_table->item(helped_table->currentRow(),helped_table->currentColumn()));
        rellenar_con_Articulo(helped_table->currentRow());
   }
}
int Table_Helper::getId_cliente() const
{
    return id_cliente;
}

void Table_Helper::setId_cliente(int value)
{
    id_cliente = value;
}


void Table_Helper::updateLinea(int row)
{
    m_rows[row]->codigo = helped_table->item(row,0)->text();
    m_rows[row]->cantidad= helped_table->item(row,1)->text().toInt();
    m_rows[row]->descripcion= helped_table->item(row,2)->text();
    m_rows[row]->precio= helped_table->item(row,3)->text().toDouble();
    m_rows[row]->subtotal= helped_table->item(row,4)->text().toDouble();
    m_rows[row]->dto_perc= helped_table->item(row,5)->text().toDouble();
    m_rows[row]->dto= helped_table->item(row,6)->text().toDouble();
    m_rows[row]->iva_perc= helped_table->item(row,7)->text().toDouble();
    double base = m_rows[row]->subtotal - m_rows[row]->dto;
    m_rows[row]->iva = base *(m_rows[row]->iva_perc/100.0);
    m_rows[row]->rec_perc= helped_table->item(row,8)->text().toDouble();
    m_rows[row]->rec = base *(m_rows[row]->rec_perc/100.0);
    m_rows[row]->total= helped_table->item(row,9)->text().toDouble();

    emit lineaReady(m_rows[row]);
}

void Table_Helper::calcPercDescuento(int row)
{
//    double base = calculabaseLinea(row);
//    double dto = helped_table->item(row,6)->text().toDouble();
//    double perc = (dto * 100)/base;
//    helped_table->item(row,5)->setText(QString::number(perc,'f',2));
//    updateLinea(row);
//
}

void Table_Helper::calcNetoDescuento(int row)
{
    blockSignals(true);
    double subtotal = helped_table->item(row,4)->text().toDouble();
    double perc_dto = helped_table->item(row,5)->text().toDouble();
    double dto = subtotal * (perc_dto / 100.0);
    helped_table->item(row,5)->setText(QString::number(perc_dto,'f',Configuracion_global->decimales_campos_totales));
    helped_table->item(row,6)->setText(QString::number(dto,'f',Configuracion_global->decimales_campos_totales));
    updateLinea(row);
    blockSignals(false);
}
