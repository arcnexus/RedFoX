#ifndef APUNTES_H
#define APUNTES_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"

class apuntes : public QObject
{
    Q_OBJECT
public:
    explicit apuntes(QObject *parent = 0);

    int id;
    int id_cuenta;
    int id_documento;
    QString DH;
    QString cuenta_d;
    QString descripcion_d;
    QString cuenta_h;
    QString descripcion_h;
    double importe_d;
    double importe_h;
    int asiento;
    int id_cuenta_d;
    int id_cuenta_h;
    QDate fecha_asiento;
    int pos_en_asiento;
    QString cta_principal;
    QString comentario_d;
    QString comentario_h;

    
signals:
    
public slots:
    int nuevo_numero_apunte();
    bool nuevalinea();
    void clear();
};

#endif // APUNTES_H
