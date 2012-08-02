#ifndef EMPRESA_H
#define EMPRESA_H

#include <QObject>
#include <QString>

class Empresa : public QObject
{
    Q_OBJECT
public:
    explicit Empresa(QObject *parent = 0);
    
signals:
    
public slots:

private:
    QString cCodEmpresa;

    
};

#endif // EMPRESA_H
