#ifndef ARTICULO_H
#define ARTICULO_H
#include <QString>
#include <QSqlQuery>

class Articulo
{
public:
    Articulo();
    void Recuperar(QString cSQL);
    void Vender(int id, int cantidad, double rPVP);
    void Devolucion(int id, int cantidad, double rImporte, QString cMotivo, QString dFechaDevolucion);

private:
    int id;
    QString cCodigo;
    QString cCodigoBarras;
    QString cCodigoFabricante;
    QString cDescripcion;
    double rTarifa1;
    QSqlQuery qryArticulo;

};

#endif // ARTICULO_H
