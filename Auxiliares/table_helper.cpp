#include "table_helper.h"
#include "monetarydelegate.h"
#include "../Almacen/frmarticulos.h"
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

}

void Table_Helper::help_table(QTableWidget *table)
{
    this->helped_table = table;
   /// helped_table->setItemDelegateForColumn(0,new SearchDelegate(helped_table));
    helped_table->setItemDelegateForColumn(1,new SpinBoxDelegate(helped_table));
    helped_table->setItemDelegateForColumn(3,new MonetaryDelegate(helped_table,true));
    helped_table->setItemDelegateForColumn(4,new MonetaryDelegate(helped_table,false));
    helped_table->setItemDelegateForColumn(5,new MonetaryDelegate(helped_table,false));
    helped_table->setItemDelegateForColumn(6,new SpinBoxDelegate(helped_table,true,0,100));
    helped_table->setItemDelegateForColumn(7,new ReadOnlyDelegate(helped_table)); // el tipo de iva pertenece al artículo y solo se puede modificar en a ficha del artículo.
    helped_table->setItemDelegateForColumn(8,new MonetaryDelegate(helped_table,false));
    helped_table->setColumnWidth(2,220);

    QStringList headers;
    if (!this->comprando) {
        headers << "Codigo" << "Cantidad" << "Descripcion" << "P.V.P" << "SubTotal";
        headers << "DTO" << "DTO%" << "% I.V.A." << "Total";
    } else
    {
        headers << "Codigo" << "Cantidad" << "Descripcion" << "COSTE" << "SubTotal";
        headers << "DTO" << "DTO%" << "% I.V.A." << "Total";
    }

    helped_table->setHorizontalHeaderLabels(headers);
    resizeTable();

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
    m_rows.clear();

    QSqlQuery query(QSqlDatabase::database(db));
    QString sql;
    if(filter.isEmpty())
        sql = QString("SELECT * FROM %1").arg(table);
    else
    {
        int ntabla;
        ntabla = 0;

        if(table =="lin_ped_pro")
            ntabla = 1;
        if(table =="lin_alb_pro")
            ntabla = 2;

        switch (ntabla) {
        case 1:
            sql = QString("select id,id_cab,id_articulo,codigo_articulo_proveedor,cantidad,descripcion,coste_bruto,subtotal_coste,dto,porc_dto,porc_iva,total"
                          " from %1 where %2").arg(table).arg(filter);
            break;
        case 2:
            sql = QString("select id,id_cab,id_articulo,codigo_articulo_proveedor,nCantidad,cDescripcion,rCoste,rSubtotal,rDto,nDto,nIva,rTotal"
                          " from %1 where %2").arg(table).arg(filter);
            break;
        default:
            sql = QString("SELECT * FROM %1 WHERE %2").arg(table).arg(filter);
            break;
        }
    }
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

        QTableWidgetItem * total = helped_table->item(row,8);
        total->setFlags(total->flags()^Qt::ItemIsEnabled);
        total->setText("0");
        helped_table->item(row,3)->setText("0");
        helped_table->item(row,5)->setText("0");
        helped_table->item(row,6)->setText("0");
        QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
        for (int i=0;i<keys.size();i++)
            if(Configuracion_global->ivas[keys.at(i)].value("nombre_interno") == "base1")
                helped_table->item(row,7)->setText(keys.at(i));
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
    lrow->idLinea = r.value(0).toInt();
    lrow->codigo = r.value(3).toString();
    lrow->cantidad = r.value(4).toDouble();
    lrow->cantidad_old = r.value(4).toDouble();
    lrow->descripcion = r.value(5).toString();
    lrow->importe = r.value(6).toDouble();
    lrow->subTotal = r.value(7).toDouble();
    lrow->dto = r.value(8).toDouble();
    lrow->dto_perc = r.value(9).toDouble();
    lrow->iva_perc = r.value(10).toDouble();

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

    for(int i = 2;i < helped_table->columnCount();i++)
    {
        QTableWidgetItem * itemX = new QTableWidgetItem;
        helped_table->setItem(row,i,itemX);
    }

    QTableWidgetItem * subtotal = helped_table->item(row,4);
    subtotal->setFlags(subtotal->flags()^Qt::ItemIsEnabled);
    subtotal->setText("0");

    QTableWidgetItem * total = helped_table->item(row,8);
    total->setFlags(total->flags()^Qt::ItemIsEnabled);
    total->setText("0");
    helped_table->item(row,3)->setText("0");
    helped_table->item(row,5)->setText("0");
    helped_table->item(row,6)->setText("0");



    helped_table->blockSignals(false);
    helped_table->setFocus();
    helped_table->setCurrentCell(helped_table->rowCount()-1,0);
    helped_table->editItem(helped_table->item(helped_table->currentRow(),helped_table->currentColumn()));

    lineaDetalle* lrow = getLineaDetalleFromRecord(r);

    helped_table->item(row,0)->setText(r.value(3).toString());
    helped_table->item(row,1)->setText(r.value(4).toString());
    helped_table->item(row,2)->setText(r.value(5).toString());
    helped_table->item(row,3)->setText(r.value(6).toString());
    helped_table->item(row,4)->setText(r.value(7).toString());
    helped_table->item(row,5)->setText(r.value(8).toString());
    helped_table->item(row,6)->setText(r.value(9).toString());

    m_rows.append(lrow);

    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    for(int i = 0;i<keys.size();i++)
    {
        if(Configuracion_global->ivas[keys.at(i)].value("nIVA").toDouble() == r.value(10).toDouble())
        {
            helped_table->item(row,7)->setText(keys.at(i));
            break;
        }
    }
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
    if(current)
        if(current->column() == 1)
            m_rows[current->row()]->cantidad_old = current->text().toDouble();

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
        }
        else if(column == 5 && !helped_table->item(row,4)->text().isEmpty())
        {
            calcPercDescuento(row);
            comprobarDescuento(row);
        }
        else if(column == 6 && !helped_table->item(row,4)->text().isEmpty())
        {
            calcNetoDescuento(row);
            comprobarDescuento(row);
        }
        else if(column == 9)
            comprobarStock(row);

        updateLinea(row);
        helped_table->blockSignals(false);

        emit i_recalc();
    }
}

void Table_Helper::recalc()
{
    calcularTotal();
    calcularDesglose();
}

void Table_Helper::calcularTotal()
{
    helped_table->blockSignals(true);
    double base = 0;
    double dto = 0;
    double total = 0;
    double iva = 0;
    double re = 0;
    for(int i = 0; i< helped_table->rowCount() ; i++)
    {
        base += calcularBaseLinea(i);
        dto += calcularDtoLinea(i);
        iva += calcularIVALinea(i);
        re += calcularRELinea(i);
        total += calcularTotalLinea(i);
    }
    double subtotal = base - dto;
    helped_table->blockSignals(false);
    emit totalChanged( base , dto , subtotal ,  iva,  re,  total,  moneda);
}

double Table_Helper::calcularDtoLinea(int row)
{
    return helped_table->item(row,5)->text().replace(",",".").toDouble();
}

double Table_Helper::calcularBaseLinea(int row)
{
    int cantidad = helped_table->item(row,1)->text().toInt();
    double pvp = helped_table->item(row,3)->text().replace(",",".").toDouble();
    double subtotal = cantidad * pvp;

    return subtotal;
}

double Table_Helper::calcularIVALinea(int row)
{
    double base = calcularBaseLinea(row);
    base -= calcularDtoLinea(row);

    double iva = Configuracion_global->ivas[helped_table->item(row,7)->text()].value("nIva").toDouble();

    double total_iva = (base * iva)/100;

    return total_iva;
}

double Table_Helper::calcularRELinea(int row)
{
    if(use_re)
    {
        double base = helped_table->item(row,4)->text().replace(",",".").toDouble();
        base -= calcularDtoLinea(row);

        double re = Configuracion_global->ivas[helped_table->item(row,7)->text()].value("nRegargoEquivalencia").toDouble();

        double total_re = (base * re)/100;

        return total_re;
    }
    else
        return 0;
}

void Table_Helper::comprobarCantidad(int row)
{
    int cantidad = helped_table->item(row,1)->text().toInt();
    if (helped_table->columnCount() == 10)
        helped_table->item(row,9)->setText(QString::number(cantidad));
    int stock = 0;
    int stockMax = 0;
    int stockMin = 0;
    bool controlarStock = true;
    QString codigo = helped_table->item(row,0)->text();
    QSqlQuery query(QSqlDatabase::database("Maya"));
    QString sql = QString("SELECT * FROM articulos WHERE cCodigo = '%1'").arg(codigo);
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next())
        {
            stock = query.record().value("nStockReal").toInt();
            stockMax  = query.record().value("nStockMaximo").toInt();
            stockMin  = query.record().value("nStockMinimo").toInt();
            controlarStock = query.record().value("lControlarStock").toBool();

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
            if (helped_table->columnCount() == 10)
                helped_table->item(row,9)->setText(QString::number(stock));
        }
        else if ((stock - cantidad) < stockMin && controlarStock)
        {
            QMessageBox::warning(qApp->activeWindow(),tr("Stock bajo minimos"),
                                 tr("Con esta cantidad el stock estará por debajo del minimo establecido"),tr("Aceptar"));
            if (helped_table->columnCount() == 10)
                helped_table->item(row,9)->setText(QString::number(cantidad));
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
    QSqlQuery query(QSqlDatabase::database("Maya"));
    QString sql = QString("SELECT * FROM articulos WHERE cCodigo = '%1'").arg(codigo);
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next())
        {
            stock = query.record().value("nStockReal").toInt();
            controlarstock = query.record().value("lControlarStock").toBool();

        }
       // else
       // {
       //     QMessageBox::critical(qApp->activeWindow(),tr("No encontado"),tr("Articulo no encontrado"),tr("Aceptar"));
       // }
    }
    if(aServir > cantidad && controlarstock)
        helped_table->item(row,9)->setText(QString::number(cantidad));
    if(aServir > stock && controlarstock)
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Stock insuficiente"),
                             tr("No existe suficiente stock para satisfacer esta cantidad"),tr("Aceptar"));
        helped_table->item(row,9)->setText(QString::number(cantidad));
    }
}

void Table_Helper::comprobarDescuento(int row)
{
    double subTotal = helped_table->item(row,4)->text().toDouble();
    double dto = helped_table->item(row,5)->text().toDouble();
    if((subTotal - dto) < 0)
        QMessageBox::warning(qApp->activeWindow(),tr("Descuento excesivo"),
                             tr("Descuento mayor del valor del producto"),tr("Aceptar"));
}

double Table_Helper::calcularTotalLinea(int row)
{
    int cantidad = helped_table->item(row,1)->text().toInt();
    double pvp = helped_table->item(row,3)->text().replace(",",".").toDouble();
    double subtotal = cantidad * pvp;
    helped_table->item(row,4)->setText(QString::number(subtotal,'f',2));

    double dto = calcularDtoLinea(row);

    double iva = Configuracion_global->ivas[helped_table->item(row,7)->text()].value("nIVA").toDouble();

    double re = Configuracion_global->ivas[helped_table->item(row,7)->text()].value("nRegargoEquivalencia").toDouble();

    double total = subtotal - dto;

    double add_re = (re / 100) * total;

    double y = 1 + (iva/100);
    total = total * y;

    if(use_re)
        total = total + add_re;
    helped_table->item(row,8)->setText(QString::number(total,'f',2));
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
        QString name = helped_table->item(i,7)->text();
        if(Configuracion_global->ivas[name].value("nombre_interno").toString() == sbase)
        {
            double row_base = calcularBaseLinea(i);
            row_base-= calcularDtoLinea(i);

            double iva_value = Configuracion_global->ivas[name].value("nIVA").toDouble();
            double aux_iva = (row_base * iva_value) /100;

            //Acumulo por linea
            base += row_base;
            iva += aux_iva;
            /*********************/

            if(use_re)
            {
                double re_value = Configuracion_global->ivas[name].value("nRegargoEquivalencia").toDouble();
                double aux_re = (row_base * re_value) /100;
                re += aux_re;
                total+=aux_re;
            }
            total += row_base;
            total += aux_iva;
        }
    }

    if(sbase == "base1")
        emit desglose1Changed(base, iva, re, total);
    else if(sbase == "base2")
        emit desglose2Changed(base, iva, re, total);
    else if(sbase == "base3")
        emit desglose3Changed(base, iva, re, total);
    else if(sbase == "base4")
        emit desglose4Changed(base, iva, re, total);
}

void Table_Helper::rellenar_con_Articulo(int row)
{
    QString codigo = helped_table->item(row,0)->text();
    QSqlQuery query(QSqlDatabase::database("Maya"));
    QString sql;
    if (this->comprando)
        sql = QString("SELECT * FROM vistaArt_tarifa WHERE cCodigo = '%1'").arg(codigo);
    else
        sql = QString("SELECT * FROM vistaArt_tarifa WHERE cCodigo = '%1' and tarifa = %2").arg(codigo).arg(tarifa);
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next())
        {
            QSqlRecord r = query.record();
            if(r.value("cDescripcionReducida").toString().isEmpty())
                helped_table->item(row,2)->setText(r.value("cDescripcion").toString());
            else
                helped_table->item(row,2)->setText(r.value("cDescripcionReducida").toString());

            if (this->comprando)
                 helped_table->item(row,3)->setText(r.value("rCoste").toString());
            else
                helped_table->item(row,3)->setText(r.value("pvp").toString());

            QList<QString> keys = Configuracion_global->ivas.uniqueKeys();

            double iva = r.value("nTipoIva").toDouble();
            QString sIva = QString::number(iva);
            for (int i = 0;i<keys.size();i++)
            {
                if(Configuracion_global->ivas[keys.at(i)].value("nIva").toDouble() == iva)
                    sIva = Configuracion_global->ivas[keys.at(i)].value("cTipo").toString();
            }
            helped_table->item(row,7)->setText(sIva);
        }
        else if(helped_table->item(row,0)->text() !="" && helped_table->item(row,2)->text().isEmpty())
        {
            if(QMessageBox::question(helped_table,
                                 tr("Codigo desconocido"),
                                 tr("Codigo de articulo desconocido "
                                    "¿Desea introducir el árticulo? (si no lo hace no se registraran acumulados)")
                                    ,tr("Usar de todos modos"),tr("Sí"))== QMessageBox::Accepted)
            {
                FrmArticulos f((QWidget*)helped_table->parent(),true);
                f.exec();
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
        if(keyEvent->key() == Qt::Key_F1)
        {
            searchArticulo();
        }
        emit i_recalc();
    }    
    return false;
}

void Table_Helper::searchArticulo()
{
    Db_table_View searcher(qApp->activeWindow());
    searcher.set_db("Maya");
    searcher.set_table("vistaArt_tarifa");

    searcher.setWindowTitle(tr("Articulos"));

    QStringList headers;
    headers << tr("Codigo")<< tr("Cod.Barras") << tr("Cod.Fabricante") << tr("Descripción");
    searcher.set_table_headers(headers);
    searcher.set_tarifa(this->tarifa);

    searcher.set_readOnly(true);
    searcher.set_selection("cCodigo");

    if(searcher.exec() == QDialog::Accepted)
    {
        helped_table->item(helped_table->currentRow(),0)->setText(searcher.selected_value);
        helped_table->setCurrentCell(helped_table->currentRow(),1);
        helped_table->editItem(helped_table->item(helped_table->currentRow(),helped_table->currentColumn()));
        rellenar_con_Articulo(helped_table->currentRow());
    }
}

void Table_Helper::updateLinea(int row)
{
    m_rows[row]->codigo = helped_table->item(row,0)->text();
    m_rows[row]->cantidad= helped_table->item(row,1)->text().toInt();
    m_rows[row]->descripcion= helped_table->item(row,2)->text();
    m_rows[row]->importe= helped_table->item(row,3)->text().replace(",",".").toDouble();
    m_rows[row]->subTotal= helped_table->item(row,4)->text().replace(",",".").toDouble();
    m_rows[row]->dto= helped_table->item(row,5)->text().replace(",",".").toDouble();
    m_rows[row]->dto_perc= helped_table->item(row,6)->text().toDouble();
    double iva = Configuracion_global->ivas[helped_table->item(row,7)->text()].value("nIva").toDouble();
    m_rows[row]->iva_perc= iva;
    m_rows[row]->total= helped_table->item(row,8)->text().replace(",",".").toDouble();


    //etc
    /*
    helped_table->item(row,index)->setText(r.value(4).toString()); index++;
    m_rows[row]->cantidad = r.value(4).toDouble();
    helped_table->item(row,index)->setText(r.value(5).toString()); index++;
    m_rows[row]->descripcion = r.value(5).toString();
    helped_table->item(row,index)->setText(r.value(6).toString()); index++;
    m_rows[row]->importe = r.value(6).toDouble();
    helped_table->item(row,index)->setText(r.value(7).toString());index++;
    m_rows[row]->subTotal = r.value(7).toDouble();
    helped_table->item(row,index)->setText(r.value(8).toString());index++;
    m_rows[row]->dto = r.value(8).toDouble();
    helped_table->item(row,index)->setText(r.value(9).toString());index++;
    m_rows[row]->dto_perc = r.value(9).toDouble();
    m_rows[row]->iva_perc = r.value(10).toDouble();*/
    emit lineaReady(m_rows[row]);
}

void Table_Helper::calcPercDescuento(int row)
{
    double base = calcularBaseLinea(row);
    double dto = helped_table->item(row,5)->text().replace(",",".").toDouble();
    double perc = (dto * 100)/base;
    helped_table->item(row,6)->setText(QString::number(perc));
}

void Table_Helper::calcNetoDescuento(int row)
{
    double base = calcularBaseLinea(row);
    double perc_dto = helped_table->item(row,6)->text().replace(",",".").toDouble();
    double dto = perc_dto * base / 100;
    helped_table->item(row,5)->setText(QString::number(dto));
}
