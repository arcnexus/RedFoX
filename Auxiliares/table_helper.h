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
    void setDatabase(QString db, QString table);
    void setIdHeader(int id){m_idCab = id;}
    void set_Searcher(Db_table_View* table_view);
    void set_moneda(QString moneda);
    void set_tarifa(int tarifa);
    void set_Tipo(bool is_compra);
    void blockTable(bool state);
    void resizeTable();
    void fillTable(QString db , QString table , QString filter);
    bool saveTable(int id_cabecera , QString db , QString db_table);
signals:
    void totalChanged(double base , double dto ,double subTotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
public slots:
    void set_UsarRE(bool state);
    void addRow();
    void removeRow();
    void handle_currentItemChanged(QTableWidgetItem * current, QTableWidgetItem * previous);
private:
    QTableWidget* helped_table;
    QString moneda;
    int tarifa;

    void calcularTotal();
    double calcularDtoLinea(int row);
    double calcularBaseLinea(int row);
    double calcularIVALinea(int row);
    double calcularRELinea(int row);
    double calcularTotalLinea(int row);

    void calcularDesglose();
    void calcular_por_Base(QString base);

    void comprobarCantidad(int row);
    void comprobarStock(int row);
    void comprobarDescuento(int row);
    void rellenar_con_Articulo(int row);
    bool comprando;
    bool use_re;  
    bool saveLine(int row,int id_cabecera,QString db, QString db_table, QStringList headers);
    bool saveLine(int id, int row, int id_cabecera, QString db, QString db_table, QStringList headers);
    void addRow(QSqlRecord r);

    bool eventFilter(QObject *target, QEvent *event);
    void searchArticulo();
    int m_cantidadArticulo;
    QVector<int> row_id;
    int m_idCab;
    QString m_db;
    QString m_db_table;
    QStringList m_headers;
};

#endif // TABLE_HELPER_H
