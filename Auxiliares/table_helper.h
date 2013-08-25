#ifndef TABLE_HELPER_H
#define TABLE_HELPER_H

#include "Globlal_Include.h"

#include "searchdelegate.h"
#include "spinboxdelegate.h"
#include "readonlydelegate.h"
#include "comboboxdelegate.h"

struct lineaDetalle
{
    int idLinea;
    QString codigo;
    int cantidad;
    int cantidad_old;
    QString descripcion;
    double precio;
    double subtotal;
    double dto;
    double dto_perc;
    double iva_perc;
    double iva;
    double rec_perc;
    double rec;
    double total;
    double importe_moneda_extrangera;
    int cantidad_pendiente;
};

class Table_Helper : public QObject
{
    Q_OBJECT
public:
    explicit Table_Helper(QObject *parent = 0);
    ~Table_Helper();
    void help_table(QTableWidget* table);
    void setDatabase(QString db, QString table);
    void setidHeader(int id){m_idCab = id;}
    void set_Searcher(Db_table_View* table_view);
    void set_moneda(QString moneda);
    void set2Divisas(bool b){use2divisas = b;}
    void set_tarifa(int tarifa);
    void set_tipo_dto_tarifa(int tipo_dto_tarifa);
    void set_Tipo(bool is_compra);
    void blockTable(bool state);
    void resizeTable();
    void fillTable(QString db , QString table , QString filter);
    void calculatotal();
    lineaDetalle * getLineaDetalleFromRecord(QSqlRecord r);
    double porc_iva1,porc_iva2,porc_iva3,porc_iva4;
    int getId_cliente() const;
    void setId_cliente(int value);

signals:
    void lineaReady(lineaDetalle*);
    void lineaDeleted(lineaDetalle*);
    void totalChanged(double base , double dto ,double subtotal , double iva, double re, double total, QString moneda);
    void desglose1Changed(double base, double iva, double re, double total);
    void desglose2Changed(double base, double iva, double re, double total);
    void desglose3Changed(double base, double iva, double re, double total);
    void desglose4Changed(double base, double iva, double re, double total);
    void i_recalc();
public slots:
    void set_UsarRE(bool state);
    void addRow();
    void removeRow();
    void handle_currentItemChanged(QTableWidgetItem * current, QTableWidgetItem * previous);
private slots:
    void recalc();
private:
    QTableWidget* helped_table;
    QString moneda;
    int tarifa;

    double calcularsubtotalLinea(int row);
    double calculadtoLinea(int row);
    double calculabaseLinea(int row);
    double calculaivaLinea(int row);
    double calcularRELinea(int row);
    double calculatotalLinea(int row);

    void calcularDesglose();
    void calcular_por_Base(QString base);

    void comprobarCantidad(int row);
    void comprobarStock(int row);
    void comprobadto(int row);
    void rellenar_con_Articulo(int row);
    bool comprando;
    bool use_re;  
    void addRow(QSqlRecord r);

    bool eventFilter(QObject *target, QEvent *event);
    void searchArticulo();
    int m_cantidadArticulo;
    QVector<lineaDetalle*> m_rows;
    int m_idCab;
    QString m_db;
    QString m_db_table;
    QString codigo_art;
    bool use2divisas;
    bool isActive;
    int tipo_dto_tarifa;
    int id_cliente;
    void updateLinea(int row);
    void calcPercDescuento(int row);
    void calcNetoDescuento(int row);
};

#endif // TABLE_HELPER_H
