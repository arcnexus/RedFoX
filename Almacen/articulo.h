#ifndef ARTICULO_H
#define ARTICULO_H

#include "../Auxiliares/Globlal_Include.h"

class Articulo : public QObject
{
    Q_OBJECT
public:

    int id;
    QString cCodigo;
    QString cCodigoBarras;
    QString cCodigoFabricante;
    QString cDescripcion;
    QString cDescripcionReducida;
    int id_Proveedor;
    QString cProveedor;
    QString cCodProveedor;
    int id_Familia;
    QString cFamilia;
    int id_Seccion;
    QString cSeccion;
    int id_SubFamilia;
    QString cSubfamilia;
    QString cCodigoIva;
    double nTipoIva;
    double rCoste;
    double rTarifa1;
    double rTarifa2;
    double rTarifa3;
    double rDto;
    double nDtoProveedor;
    double nDtoProveedor2;
    double nDtoProveedor3;
    QDate dUltimaCompra;
    QDate dUltimaVenta;
    double nMargen1;
    double nMargen2;
    double nMargen3;
    double rPrecioMedio;
    double nUnidadesCompradas;
    double rPrecioMedio2;
    double nUnidadesVendidas;
    double rPrecioMedio3;
    double rAcumuladoCompras;
    double rAcumuladoVentas;
    QString tComentario;
    int nStockMaximo;
    int nStockMinimo;
    int nStockReal;
    QString cTipoUnidad;
    int lControlarStock;
    QString cModelo;
    QString cTalla;
    QString cColor;
    QString cComposicion;
    int lPvpIncluyeIva;
    QDate dFechaPrevistaRecepcion;
    int nCantidadPendienteRecibir;
    int nReservados;
    int lMostrarWeb;
    int nEtiquetas;
    int nPaquetes;
    QString cLocalizacion;
    double pvp;


    Articulo(QObject* parent = 0);
    void Anadir();
    bool Recuperar(QString cSQL);
    void Recuperar(QString cSQL,int nProcede);
    void Guardar();
    void Vaciar();
    void Borrar(int nId);
    static void Vender(int id, int cantidad, double rPVP);
    void Devolucion(int id, int cantidad, double rImporte, QString cMotivo, QString dFechaDevolucion);
    void CargarImagen(QLabel *label);

private:

    QSqlQuery qryArticulo;
    QSqlQuery qryTipoIva;

};

#endif // ARTICULO_H
