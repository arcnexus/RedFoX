#ifndef TABLE_HELPER_H
#define TABLE_HELPER_H

#include "Globlal_Include.h"

#include "searchdelegate.h"
#include "spinboxdelegate.h"
#include "readonlydelegate.h"

class Table_Helper : public QObject
{
    Q_OBJECT
public:
    explicit Table_Helper(QObject *parent = 0);
    ~Table_Helper();
    void help_table(QTableWidget* table);
    void set_Searcher(Db_table_View* table_view);
    void set_moneda(QString moneda);
    void set_Tipo(bool is_compra);
    void resizeTable();
signals:
    void totalChanged(QString newTotal);
public slots:
    void addRow();
    void removeRow();
    void handle_cellChanged(int row, int column);
private:
    QTableWidget* helped_table;
    Db_table_View* searcher;
    QString moneda;
    void calcularTotal();
    void comprobarCantidad(int row);
    void comprobarDescuento(int row);
    double calcularLinea(int row);
    bool comprando;
};

#endif // TABLE_HELPER_H
