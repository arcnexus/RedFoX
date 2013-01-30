#ifndef TABLE_HELPER_H
#define TABLE_HELPER_H

#include "Globlal_Include.h"

#include "searchdelegate.h"
#include "spinboxdelegate.h"
#include "readonlydelegate.h"
#include "comboboxdelegate.h"

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
    void blockTable(bool state);
    void resizeTable();
    bool saveTable(int id_cabecera , QString db , QString db_table);
signals:
    void totalChanged(QString newTotal);
public slots:
    void set_UsarRE(bool state);
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
    void rellenar_con_Articulo(int row);
    bool comprando;
    bool use_re;
    QHash <QString, QSqlRecord> ivas;   
    bool saveLine(int row,int id_cabecera,QString db, QString db_table, QStringList headers);
};

#endif // TABLE_HELPER_H
