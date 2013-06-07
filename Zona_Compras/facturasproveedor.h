#ifndef FACTURASPROVEEDOR_H
#define FACTURASPROVEEDOR_H

#include <QObject>

class FacturasProveedor : public QObject
{
    Q_OBJECT
public:
    explicit FacturasProveedor(QObject *parent = 0);
    int id;
    
signals:
    
public slots:
    
};

#endif // FACTURASPROVEEDOR_H
