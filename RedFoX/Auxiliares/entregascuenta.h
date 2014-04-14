#ifndef ENTREGASCUENTA_H
#define ENTREGASCUENTA_H

#include <QObject>
#include "Auxiliares/Globlal_Include.h"

class EntregasCuenta : public QObject
{
    Q_OBJECT
public:
    explicit EntregasCuenta(QObject *parent = 0);
    bool Anadir(int tipo, int id_pro_cli, double importe, QDate fecha,QString concepto);
    
signals:
    
public slots:
    
};

#endif // ENTREGASCUENTA_H
