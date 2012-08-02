#ifndef CONEXIONTERRA_H
#define CONEXIONTERRA_H

#include <QObject>
#include "configuracion.h"

class ConexionTerra : public QObject
{
    Q_OBJECT
public:
    explicit ConexionTerra(QObject *parent = 0);
    
signals:
    
public slots:
    void conectar();
    
};

#endif // CONEXIONTERRA_H
