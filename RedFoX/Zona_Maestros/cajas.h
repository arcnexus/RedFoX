#ifndef CAJAS_H
#define CAJAS_H

#include <QObject>
#include "Auxiliares/Globlal_Include.h"
class cajas : public QObject
{
    Q_OBJECT
public:
    explicit cajas(QObject *parent = 0);
    QHash <QString, QVariant> h_cajas;

    bool anadir();
    bool guardar(QString nuevo);
    bool recuperar(QStringList filtro, QStringList extras);
    bool borrar();
    bool vaciar();

signals:

public slots:

};

#endif // CAJAS_H
