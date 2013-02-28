#ifndef DESACTIVARCAMPOS_H
#define DESACTIVARCAMPOS_H

#include <QObject>

class DesactivarCampos : public QObject
{
    Q_OBJECT
public:
    explicit DesactivarCampos(QObject *parent = 0);
    
signals:
    
public slots:
    void Desactivar(QObject Objeto);
    
};

#endif // DESACTIVARCAMPOS_H
