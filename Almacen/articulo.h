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
    double rDto;
    QDate dUltimaCompra;
    QDate dUltimaVenta;
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
    int id_tiposiva;
    int idsubsubfamilia;
    int idgrupoart;
    int idweb;
    int stockfisico;
    int nUnidadesCompradas;
    int nUnidadesVendidas;
    double rCoste;

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
    int getIdSeccion(QString cSeccion_);
    int getIdFamilia(QString cFamilia_);
    int getIdSubFamilia(QString cSubfamilia_);
    int getIdSubSufFamilia(QString cSubSubFamilia_);
    int getIdGrupo(QString cGrupo_);
    QString getcSeccion(int nId);
    QString getcFamilia(int nId);
    QString getcSubFamilia(int nId);
    QString getcSubSubFamilia(int nId);
    QString getcGrupo(int nId);
    bool agregar_proveedor_alternativo(int id_art,int id_Proveedor,QString codigo,double pvd,QString descoferta,
                                       QString oferta);

private:

    QSqlQuery qryArticulo;
    QSqlQuery qryTipoIva;

};

#endif // ARTICULO_H
