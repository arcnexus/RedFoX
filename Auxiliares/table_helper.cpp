#include "table_helper.h"

Table_Helper::Table_Helper(QObject *parent) :
    QObject(parent)
{
    helped_table = 0;
    searcher = 0;
    moneda = "€";
    comprando = true;
    use_re = false;
    QSqlQuery query(QSqlDatabase::database("empresa"));
    if(query.exec("SELECT * FROM tiposiva"))
    {
        while(query.next())
        {
            QString key = query.record().value("cTipo").toString();
            ivas.insert(key,query.record());
        }
    }
}

Table_Helper::~Table_Helper()
{
    if(searcher)
        searcher->deleteLater();
}

void Table_Helper::help_table(QTableWidget *table)
{
    this->helped_table = table;    
    helped_table->setItemDelegateForColumn(0,new SearchDelegate(helped_table,searcher));
    helped_table->setItemDelegateForColumn(1,new SpinBoxDelegate(helped_table));
    helped_table->setItemDelegateForColumn(3,new SpinBoxDelegate(helped_table,true,0));
    helped_table->setItemDelegateForColumn(4,new ReadOnlyDelegate(helped_table));
    helped_table->setItemDelegateForColumn(5,new SpinBoxDelegate(helped_table,true,0));
    helped_table->setItemDelegateForColumn(6,new SpinBoxDelegate(helped_table,true,0,100));

    comboboxDelegate* comboModel = new comboboxDelegate(helped_table);
    comboModel->setUpModel("empresa","tiposiva","cTipo");
    helped_table->setItemDelegateForColumn(7,comboModel);

    helped_table->setItemDelegateForColumn(8,new ReadOnlyDelegate(helped_table));

    QStringList headers;
    headers << "Codigo" << "Cantidad" << "Descripcion" << "P.V.P" << "SubTotal";
    headers << "DTO" << "DTO%" << "% I.V.A." << "Total";

    helped_table->setHorizontalHeaderLabels(headers);
    resizeTable();

    connect(helped_table,SIGNAL(cellChanged(int,int)),this,SLOT(handle_cellChanged(int,int)));

    helped_table->installEventFilter(this);
}

void Table_Helper::set_Searcher(Db_table_View *table_view)
{
    searcher = table_view;
}

void Table_Helper::set_moneda(QString moneda)
{
    this->moneda = moneda;
}

void Table_Helper::set_Tipo(bool is_compra)
{
    comprando = is_compra;
}

void Table_Helper::blockTable(bool state)
{
    if(helped_table)
    {
        if(state)
            helped_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        else
        {
            helped_table->setEditTriggers(QAbstractItemView::DoubleClicked);
        }
    }
}

void Table_Helper::set_UsarRE(bool state)
{
    use_re = state;
    calcularTotal();
    calcularDesglose();
}

void Table_Helper::resizeTable()
{
    if(helped_table)
    {
        helped_table->horizontalHeader()->setUpdatesEnabled(false);

        for (int i = 0; i < helped_table->columnCount(); i++)
            helped_table->horizontalHeader()->resizeSection(i, 100);


        int wDesc = helped_table->horizontalHeader()->sectionSize(2);
        int wLast = helped_table->horizontalHeader()->sectionSize(8);
        helped_table->horizontalHeader()->resizeSection(2,wLast);
        helped_table->horizontalHeader()->resizeSection(8,wDesc);

        helped_table->horizontalHeader()->setUpdatesEnabled(true);
    }
}

void Table_Helper::fillTable(QString db, QString table, QString filter)
{
    helped_table->blockSignals(true);
    helped_table->clear();
    helped_table->setRowCount(0);

    QStringList headers;
    headers << "Codigo" << "Cantidad" << "Descripcion" << "P.V.P" << "SubTotal";
    headers << "DTO" << "DTO%" << "% I.V.A." << "Total";

    helped_table->setHorizontalHeaderLabels(headers);

    QSqlQuery query(QSqlDatabase::database(db));
    QString sql;
    if(filter.isEmpty())
        sql = QString("SELECT * FROM %1").arg(table);
    else
        sql = QString("SELECT * FROM %1 WHERE %2").arg(table).arg(filter);

    if(query.exec(sql))
    {
        while(query.next())
            addRow(query.record());
    }
    helped_table->blockSignals(false);
    calcularTotal();
    calcularDesglose();
}

bool Table_Helper::saveTable(int id_cabecera, QString db, QString db_table)
{
    bool succes = true;
    QSqlQuery query(QSqlDatabase::database(db));
    QStringList headers;
    headers.clear();

    if(query.exec("SHOW COLUMNS FROM "+db_table))
    {
        while(query.next())
        {
            QSqlRecord r = query.record();
            headers << r.value(0).toString();
        }
    }
    else
    {
        succes = false;
    }
    if(!headers.isEmpty())
    {
        for(int row= 0; row < helped_table->rowCount() ; row++)
        {
            succes &= saveLine(row , id_cabecera , db , db_table , headers);
            if(!succes)
                break;
        }
    }
    return succes;
}

void Table_Helper::addRow()
{
    if(helped_table)
    {
        resizeTable();
        helped_table->blockSignals(true);
        helped_table->setRowCount(helped_table->rowCount()+1);
        int row = helped_table->rowCount()-1;

        QTableWidgetItem * item0 = new QTableWidgetItem;
        helped_table->setItem(row,0,item0);

        QTableWidgetItem * item = new QTableWidgetItem("1");
        helped_table->setItem(row,1,item);
        for(int i = 2;i < helped_table->columnCount();i++)
        {
            QTableWidgetItem * item = new QTableWidgetItem;
            helped_table->setItem(row,i,item);
        }
        QTableWidgetItem * subtotal = helped_table->item(row,4);
        subtotal->setFlags(subtotal->flags()^Qt::ItemIsEnabled);

        QTableWidgetItem * total = helped_table->item(row,8);
        total->setFlags(total->flags()^Qt::ItemIsEnabled);
        helped_table->blockSignals(false);
    }
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
            helped_table->removeRow(rowsList.at(i));
    }
    calcularTotal();
    calcularDesglose();
}

void Table_Helper::handle_cellChanged(int row, int column)
{
    if (column == 0)
        rellenar_con_Articulo(row);
    else if(column == 1 && !helped_table->item(row,0)->text().isEmpty())
        comprobarCantidad(row);
    else if(column == 5 && !helped_table->item(row,4)->text().isEmpty())
        comprobarDescuento(row);
    calcularTotal();
    calcularDesglose();
}

void Table_Helper::calcularTotal()
{
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
    emit totalChanged( base , dto , subtotal ,  iva,  re,  total,  moneda);
}

double Table_Helper::calcularDtoLinea(int row)
{
    double base = helped_table->item(row,4)->text().toDouble();
    double dtoNeto = helped_table->item(row,5)->text().toDouble();
    double descPerc = helped_table->item(row,6)->text().toDouble();

    double total_descPerc = (base * descPerc) / 100;

    return total_descPerc + dtoNeto;
}

double Table_Helper::calcularBaseLinea(int row)
{
    return helped_table->item(row,4)->text().toDouble();
}

double Table_Helper::calcularIVALinea(int row)
{
    double base = helped_table->item(row,4)->text().toDouble();
    double dtoNeto = helped_table->item(row,5)->text().toDouble();
    double descPerc = helped_table->item(row,6)->text().toDouble();

    double total_descPerc = (base * descPerc) / 100;

    base -= total_descPerc;
    base -= dtoNeto;

    double iva = ivas[helped_table->item(row,7)->text()].value("nIva").toDouble();

    double total_iva = (base * iva)/100;

    return total_iva;
}

double Table_Helper::calcularRELinea(int row)
{
    if(use_re)
    {
        double base = helped_table->item(row,4)->text().toDouble();
        double dtoNeto = helped_table->item(row,5)->text().toDouble();
        double descPerc = helped_table->item(row,6)->text().toDouble();

        double total_descPerc = (base * descPerc) / 100;

        base -= total_descPerc;
        base -= dtoNeto;

        double re = ivas[helped_table->item(row,7)->text()].value("nRegargoEquivalencia").toDouble();

        double total_re = (base * re)/100;

        return total_re;
    }
    else
        return 0;
}

void Table_Helper::comprobarCantidad(int row)
{
    int cantidad = helped_table->item(row,1)->text().toInt();
    int stock = 0;
    int stockMax = 0;
    int stockMin = 0;
    QString codigo = helped_table->item(row,0)->text();
    QSqlQuery query(QSqlDatabase::database("empresa"));
    QString sql = QString("SELECT * FROM articulos WHERE cCodigo = '%1'").arg(codigo);
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next())
        {
            stock = query.record().value("nStockReal").toInt();
            stockMax  = query.record().value("nStockMaximo").toInt();
            stockMin  = query.record().value("nStockMinimo").toInt();
        }
        else
        {
            QMessageBox::critical(qApp->activeWindow(),tr("No encontado"),tr("Articulo no encontrado"),tr("Aceptar"));
        }
    }
    if(comprando)
    {
        if((stock + cantidad) > stockMax)
            QMessageBox::warning(qApp->activeWindow(),tr("Stock superado"),
                                 tr("Con esta cantidad se superará el stock máximo establecido"),tr("Aceptar"));
    }
    else
    {
        if((stock - cantidad) < 0)
            QMessageBox::warning(qApp->activeWindow(),tr("Stock insuficiente"),
                                 tr("No existe suficiente stock para satisfacer esta cantidad"),tr("Aceptar"));
        else if ((stock - cantidad) < stockMin)
            QMessageBox::warning(qApp->activeWindow(),tr("Stock bajo minimos"),
                                 tr("Con esta cantidad el stock estará por debajo del minimo establecido"),tr("Aceptar"));
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
    double pvp = helped_table->item(row,3)->text().toDouble();
    double subtotal = cantidad * pvp;
    helped_table->item(row,4)->setText(QString::number(subtotal,'f',2));

    double dto = helped_table->item(row,5)->text().toDouble();
    double dto_percent = helped_table->item(row,6)->text().toDouble();

    double iva = ivas[helped_table->item(row,7)->text()].value("nIVA").toDouble();

    double re = ivas[helped_table->item(row,7)->text()].value("nRegargoEquivalencia").toDouble();

    double total = subtotal - dto;

    double x = 1 - (dto_percent / 100);
    total = total * x;

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

}

void Table_Helper::rellenar_con_Articulo(int row)
{
    QString codigo = helped_table->item(row,0)->text();
    QSqlQuery query(QSqlDatabase::database("empresa"));
    QString sql = QString("SELECT * FROM articulos WHERE cCodigo = '%1'").arg(codigo);
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next())
        {
            QSqlRecord r = query.record();
            helped_table->item(row,2)->setText(r.value("cDescripcionReducida").toString());
            //TODO recopilar otros datos del articulo
        }
    }
}

bool Table_Helper::saveLine(int row, int id_cabecera, QString db, QString db_table, QStringList headers)
{
    bool succes = true;

    QSqlQuery query(QSqlDatabase::database(db));
    QString item_id = QString("SELECT Id FROM articulos WHERE cCodigo = '%1' LIMIT 1")
            .arg(helped_table->item(row,0)->text());
    int articulo_id = -1;

    if(query.exec(item_id))
    {
        if(query.next())
            articulo_id = query.record().value(0).toInt();
    }
    else
        succes = false;

    if(succes)
    {
        QVariantList values;
        values.clear();
        values << "NULL" << id_cabecera << articulo_id;
        values << helped_table->item(row,0)->text();
        values << helped_table->item(row,1)->text().toInt();
        values << helped_table->item(row,2)->text();

        values << helped_table->item(row,3)->text().toDouble();
        values << helped_table->item(row,4)->text().toDouble();
        values << helped_table->item(row,5)->text().toDouble();
        values << helped_table->item(row,6)->text().toDouble();

        values << ivas[helped_table->item(row,7)->text()].value("nIva");
        values << helped_table->item(row,8)->text().toDouble();

        //cantidad = cantidadaservir por defecto
        values << helped_table->item(row,1)->text().toInt();

        QString sql = "INSERT INTO ";
        sql.append(db_table);
        sql.append("(");
        sql.append(headers.join(","));
        sql.append(") VALUES (");
        for (int i=0;i<headers.size();i++)
        {
            if(i!= headers.size()-1)
                sql.append(QString(":%1,").arg(headers.at(i)));
            else
                sql.append(QString(":%1)").arg(headers.at(i)));
        }
        query.prepare(sql);
        for(int i = 0; i<headers.size();i++)
        {
            QString id = QString(":%1").arg(headers.at(i));
            query.bindValue(id,values.at(i).toString());
        }
        if(!query.exec())
        {
            succes = false;            
        }
    }
    return succes;
}

void Table_Helper::addRow(QSqlRecord r)
{
    addRow();
    helped_table->blockSignals(true);
    int row = helped_table->rowCount()-1;
    helped_table->item(row,0)->setText(r.value(3).toString());
    helped_table->item(row,1)->setText(r.value(4).toString());
    helped_table->item(row,2)->setText(r.value(5).toString());
    helped_table->item(row,3)->setText(r.value(6).toString());
    helped_table->item(row,4)->setText(r.value(7).toString());
    helped_table->item(row,5)->setText(r.value(8).toString());
    helped_table->item(row,6)->setText(r.value(9).toString());
    //helped_table->item(row,7)->setText(r.value(10).toString());
    QList<QString> keys = ivas.uniqueKeys();
    for(int i = 0;i<keys.size();i++)
    {
        if(ivas[keys.at(i)].value("nIVA").toDouble() == r.value(10).toDouble())
        {
            helped_table->item(row,7)->setText(keys.at(i));
            break;
        }
    }
    helped_table->item(row,8)->setText(r.value(11).toString());
    helped_table->blockSignals(false);
}

bool Table_Helper::eventFilter(QObject *target, QEvent *event)
{
    Q_UNUSED(target);
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Down)
        {
            if(helped_table->currentRow() == (helped_table->rowCount()-1))
                addRow();
        }
    }
    return false;
}


