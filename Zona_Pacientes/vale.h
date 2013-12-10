#ifndef VALE_H
#define VALE_H

#include <QObject>
#include <Auxiliares/Globlal_Include.h>

class vale : public QObject
{
    Q_OBJECT
public:
    explicit vale(QObject *parent = 0);
    static int crearvale(int id_cliente, QString cliente, QDate fecha_ini, QDate fecha_fin, double importe, int id_ticket );
    static int nuevo_numero_vale();

signals:

public slots:

};

#endif // VALE_H
