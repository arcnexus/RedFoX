#ifndef TRANSPORTISTAS_H
#define TRANSPORTISTAS_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"
//#include "../Zona_Compras/proveedor.h"

class transportistas : public QObject
{
    Q_OBJECT
public:
    explicit transportistas(QObject *parent = 0);
    QHash <QString, QVariant> h_transportista;
//    QHash <QString, QVariant> h_proveedor;

    int anadir();
    bool guardar();
    bool recuperar(QStringList filtro, QStringList extras);
    QString RecuperarPais(int nid);
//    Proveedor oProveedor;
private:


signals:
    
public slots:
    
};

#endif // TRANSPORTISTAS_H
