#ifndef TIPOSTARIFA_H
#define TIPOSTARIFA_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"

class TiposTarifa : public QObject
{
    Q_OBJECT
public:
    explicit TiposTarifa(QObject *parent = 0);
    int id;
    QString descripcion;
    QString codigo_tarifa;
    double dto;
    QString desc_promo;
    int cant_promo;
    int cant_base;
    double importe_dto;
    int id_pais;
    int id_monedas;
    float margen;
    float margen_min;
public slots:
    void recuperar(QString cSql);
    void recuperar(int id);
    void cargar_datos(QSqlQuery queryTarifa);
    
signals:
    
    
};

#endif // TIPOSTARIFA_H
