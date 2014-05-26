#ifndef CAJAS_H
#define CAJAS_H

#include <QObject>
#include "Auxiliares/Globlal_Include.h"
class cajas : public QObject
{
    Q_OBJECT
public:
    explicit cajas(QObject *parent = 0);
    QString id;
    QString descripcion;

    bool anadir();
    bool guardar(QString id, QString descripcion, bool anadiendo);
    bool recuperar(QString codigo);
    bool borrar(QString id);
    void vaciar();

signals:

public slots:

};

#endif // CAJAS_H
