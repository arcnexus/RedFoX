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
    QString cuentaD;
    QString descripcionD;
    QString cuentaH;
    QString descripcionH;
    double importeD;
    double importeH;
    int asiento;
    int id_cuentaD;
    int id_cuentaH;
    QDate fechaAsiento;
    int posenasiento;
    QString cta_principal;
    QString comentarioD;
    QString comentarioH;

    
signals:
    
public slots:
    int nuevo_numero_apunte();
    bool nuevalinea();
    void clear();
};

#endif // APUNTES_H
