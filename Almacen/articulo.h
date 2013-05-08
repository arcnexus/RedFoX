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
    int id_subSubFamilia;
    QString cSubSubFamilia;
    int id_grupoart;
    QString cGrupoArt;
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
    int nStockFisico;
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
    int nUnidadesCompradas;
    int nUnidadesVendidas;
    double rCoste;
    bool articulopromocionado;
    QString descripcion_promocion;
    int tipo_oferta;
    int por_cada;
    int regalode;
    double porc_dto_web;
    double oferta_pvp_fijo;
    QString comentario_oferta;
    double margen;
    double margen_min;

    Articulo(QObject* parent = 0);
    //-----------
    // Metodos
    //-----------
    void Anadir();
    bool Recuperar(QString cSQL);
    void Recuperar(QString cSQL,int nProcede);
    void Guardar();
    void Vaciar();
    void Borrar(int nId);
    static void Vender(int id, int cantidad, double rPVP);
    void Devolucion(int id, int cantidad, double rImporte, QString cMotivo, QString dFechaDevolucion);
    void CargarImagen(QLabel *label, QLabel *label2, QLabel *label3, QLabel *label4);
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
    bool agregar_proveedor_alternativo(int id_art, int id_Proveedor, QString codigo, double pvd, QString descoferta,
                                       QString oferta, double pvdreal, int id_divisa);
    bool guardarProveedorAlternativo(int id, QString codigo, double pvd, QString descoferta,
                                       QString oferta, double pvdreal, int id_divisa);
    bool cambiarProveedorPrincipal(int id,int id_proveedor);

    QString autocodigo();
private:

    QSqlQuery qryArticulo;
    QSqlQuery qryTipoIva;

};

#endif // ARTICULO_H
