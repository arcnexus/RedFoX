#include "table_helper.h"

Table_Helper::Table_Helper(QObject *parent) :
    QObject(parent)
{
    helped_table = 0;
    searcher = 0;
    moneda = "€";
    comprando = true;
}

Table_Helper::~Table_Helper()
{
    if(searcher)
        searcher->deleteLater();
}

void Table_Helper::help_table(QTableWidget *table)
{
    this->helped_table = table;    
    //TODO cambia iva delegate
    helped_table->setItemDelegateForColumn(0,new SearchDelegate(helped_table,searcher));
    helped_table->setItemDelegateForColumn(1,new SpinBoxDelegate(helped_table));
    helped_table->setItemDelegateForColumn(3,new SpinBoxDelegate(helped_table,true,0));
    helped_table->setItemDelegateForColumn(4,new ReadOnlyDelegate(helped_table));
    helped_table->setItemDelegateForColumn(5,new SpinBoxDelegate(helped_table,true,0));
    helped_table->setItemDelegateForColumn(6,new SpinBoxDelegate(helped_table,true,0,100));
    helped_table->setItemDelegateForColumn(7,new SpinBoxDelegate(helped_table,true,0,100));
    helped_table->setItemDelegateForColumn(8,new ReadOnlyDelegate(helped_table));
    connect(helped_table,SIGNAL(cellChanged(int,int)),this,SLOT(handle_cellChanged(int,int)));
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

void Table_Helper::resizeTable()
{
    if(helped_table)
    {
        helped_table->horizontalHeader()->setUpdatesEnabled(false);

        for (unsigned int i = 0; i < helped_table->columnCount(); i++)
            helped_table->horizontalHeader()->resizeSection(i, 100);


        int wDesc = helped_table->horizontalHeader()->sectionSize(2);
        int wLast = helped_table->horizontalHeader()->sectionSize(8);
        helped_table->horizontalHeader()->resizeSection(2,wLast);
        helped_table->horizontalHeader()->resizeSection(8,wDesc);

        helped_table->horizontalHeader()->setUpdatesEnabled(true);
    }
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
}

void Table_Helper::handle_cellChanged(int row, int column)
{
    if(column == 1 && !helped_table->item(row,0)->text().isEmpty())
        comprobarCantidad(row);
    if(column == 5 && !helped_table->item(row,4)->text().isEmpty())
        comprobarDescuento(row);
    calcularTotal();
}

void Table_Helper::calcularTotal()
{
    double total = 0;
    for(int i = 0; i< helped_table->rowCount() ; i++)
        total += calcularLinea(i);
    emit totalChanged(QString::number(total,'f',2)+moneda);
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
    qDebug()<<query.lastQuery();
    qDebug()<<query.lastError();
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

double Table_Helper::calcularLinea(int row)
{
    int cantidad = helped_table->item(row,1)->text().toInt();
    double pvp = helped_table->item(row,3)->text().toDouble();
    double subtotal = cantidad * pvp;
    helped_table->item(row,4)->setText(QString::number(subtotal,'f',2));

    double dto = helped_table->item(row,5)->text().toDouble();
    double dto_percent = helped_table->item(row,6)->text().toDouble();
    double iva = helped_table->item(row,7)->text().toDouble();

    double total = subtotal - dto;

    double x = 1 - (dto_percent / 100);
    total = total * x;

    double y = 1 + (iva/100);
    total = total * y;

    helped_table->item(row,8)->setText(QString::number(total,'f',2));
    return total;
}
