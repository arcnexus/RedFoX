#ifndef ARTICULO_H
#define ARTICULO_H

#include "../Auxiliares/Globlal_Include.h"

class Articulo : public QObject
{
    Q_OBJECT
public:

    int id;
    QString codigo;
    QString codigo_barras;
    QString codigo_fabricante;
    QString descripcion;
    QString descripcion_reducida;
    int id_proveedor;
    QString proveedor;
    QString cCodProveedor;
    int id_familia;
    QString familia;
    int id_seccion;
    QString seccion;
    int id_subfamilia;
    QString subfamilia;
    int id_subSubFamilia;
    QString cSubSubFamilia;
    int id_grupoart;
    QString cGrupoArt;
    QString codigo_iva;
    double tipo_iva;
    double dto;
    QDate ultima_compra;
    QDate ultima_venta;
    double acumulado_compras;
    double acumulado_ventas;
    QString comentario;
    int stock_maximo;
    int stock_minimo;
    int stock_real;
    int nstock_fisico_almacen;
    QString tipo_unidad;
    int controlar_stock;
    QString modelo;
    QString talla;
    QString color;
    QString composicion;
    int pvp_incluye_iva;
    QDate fecha_prevista_recepcion;
    int cantidad_pendiente_recibir;
    int reservados;
    int mostrar_web;
    int etiquetas;
    int paquetes;
    QString localizacion_en_almacen;
    double pvp;
    int id_tipos_iva;
    int id_subsub_familia;
    int id_grupo_art;
    int id_web;
    int unidades_compradas;
    int unidades_vendidas;
    double coste;
    bool articulo_promocionado;
    QString descripcion_promocion;
    int tipo_oferta;
    int por_cada;
    int regalo_de;
    float porc_dto_web;
    double oferta_pvp_fijo;
    QString comentario_oferta;
    double margen;
    double margen_min;
    double coste_real;

    Articulo(QObject* parent = 0);
    //-----------
    // Metodos
    //-----------
    void Anadir();
    bool Recuperar(QString cSQL);
    void Recuperar(QString cSQL,int nProcede);
    void Guardar();
    void Vaciar();
    void Borrar(int nid);
    static void Vender(int id, int cantidad, double pvp);
    void Devolucion(int id, int cantidad, double importe, QString cMotivo, QString fechaDevolucion);
    void CargarImagen(QLabel *label, QLabel *label2, QLabel *label3, QLabel *label4);
    int getidSeccion(QString seccion_);
    int getidFamilia(QString familia_);
    int getidSubFamilia(QString subfamilia_);
    int getidSubSufFamilia(QString cSubSubFamilia_);
    int getidGrupo(QString cGrupo_);
    QString getseccion(int nid);
    QString getfamilia(int nid);
    QString getsubfamilia(int nid);
    QString getcSubSubFamilia(int nid);
    QString getcGrupo(int nid);
    bool agregar_proveedor_alternativo(int id_art, int id_proveedor, QString codigo, double pvd, QString descoferta,
                                       QString oferta, double pvd_real, int id_divisa);
    bool guardarProveedorAlternativo(int id, QString codigo, double pvd, QString descoferta,
                                       QString oferta, double pvd_real, int id_divisa);
    bool cambiarProveedorPrincipal(int id,int id_proveedor);
    bool cambiar_pvp();
    static bool agregarStock(int id, int value);

    QString auto_codigo();
private:

    QSqlQuery qryArticulo;
    QSqlQuery qryTipoIva;

};

#endif // ARTICULO_H
