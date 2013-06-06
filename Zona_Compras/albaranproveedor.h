#ifndef ALBARANPROVEEDOR_H
#define ALBARANPROVEEDOR_H

#include <QObject>
#include "../Auxiliares/Globlal_Include.h"

class AlbaranProveedor : public QObject
{
    Q_OBJECT
public:
    explicit AlbaranProveedor(QObject *parent = 0);
    int anadir();
    void Recuperar(int id);
    void Recuperar(QString cSQL);
    void Recuperar(QString cSQL, int val_accion);
    void Cargar(QSqlQuery query);
    void guardar();
    void Recuperar();
    void llenarCampos();

    // -------
    // CAMPOS
    //--------
    int id;
    QString cAlbaran;
    QDate dFecha;
    int id_Proveedor;
    QString cProveedor;
    QString cCifproveedor;
    double rTotal1,rTotal2,rTotal3,rTotal4;
    double nIva1,nIva2,nIva3,nIva4,rIva1,rIva2,rIva3,rIva4;
    double rBase1,rBase2,rBase3,rBase4;
    QString cFactura;
    double rBaseTotal,rIvaTotal,rTotal;
    QString tComentario;
    int nPedido;
signals:
    
public slots:
private:
    int accion;
    
};

#endif // ALBARANPROVEEDOR_H
