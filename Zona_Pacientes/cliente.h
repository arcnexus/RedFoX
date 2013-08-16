#ifndef CLIENTE_H
#define CLIENTE_H

#include "../Auxiliares/Globlal_Include.h"
#include "../Zona_Contabilidad/cuentas_contables.h"
class Cliente : public QObject
{
    Q_OBJECT

public:
    explicit Cliente(QObject *parent = 0);
        QSqlQueryModel *model;
    int id;
    int id_web;
    QString codigo_cliente;
    QString apellido1;
    QString apellido2;
    QString nombre;
    QString nombre_fiscal;
    QString nombre_comercial;
    QString persona_contacto;
    QString cif_nif;
    int tipo_dto_tarifa;
    QString cifVies;
    QString direccion1;
    QString direccion2;
    QString cp;
    QString poblacion;
    QString provincia;
    int id_pais;
    QString pais;
    QString telefono1;
    QString telefono2;
    QString fax;
    QString movil;
    QString email;
    QString web;
    QString direccion1Factura1;
    QString direccion1Factura2;
    QString cpFactura;
    QString poblacionFactura;
    QString provinciaFactura;
    int id_paisFactura;
    QString paisFactura;
    QString direccion_almacen;
    QString direccion_almacen2;
    QString cp_almacen;
    QString poblacion_almacen;
    QString provincia_almacen;
    int id_pais_almacen;
    QString paisAlmacen;
    QDate fecha_alta;
    QDate fechaCompra;
    double acumulado_ventas;
    double ventas_ejercicio;
    double riesgo_maximo;
    double deuda_actual;
    QString comentarios;
    int bloqueado;
    QString comentario_bloqueo;
    float porc_dto_cliente;
    double recargo_equivalencia;
    QString cuenta_contable;
    QString cuenta_iva_repercutido;
    QString cuenta_deudas;
    QString cuenta_cobros;
    QString forma_pago;
    int dia_pago1;
    int dia_pago2;
    int tarifa_cliente;
    double importe_a_cuenta;
    double vales;
    QString entidad_bancaria;
    QString oficina_bancaria;
    QString dc;
    QString cuenta_corriente;
    QDate fecha_nacimiento;
    double importe_pendiente;
    QString acceso_web;
    QString password_web;
    bool lIRPF;
    int idTarifa;
    int ididioma;
    QString idioma;
    QString observaciones;
    QString visa_distancia1;
    QString visa_distancia2;
    int visa1_caduca_mes;
    int visa2_caduca_mes;
    int visa1_caduca_ano;
    int visa2_caduca_ano;
    int visa1_cod_valid;
    int visa2_cod_valid;
    int id_agente;
    int id_transportista;
    int grupo_iva;
    QSqlQuery queryClienteWeb;
    QSqlDatabase db;
    QSqlQuery *qryCliente;


signals:
    
public slots:
    // Funciones de clase
    void Recuperar(QString cSQL);
    void Recuperar(int id);
    void cargar(QSqlQuery &query);
    void clear();
    void Guardar();
    void GuardarWeb();
    void Anadir();
    void anadirWeb();
    void AnadirDeuda(int id_cliente, QDate fechaDeuda,QDate fechaVto,QString documento, int id_Tiquet,
                     int id_factura, int tipo, double importe_deuda, double pagado, double pendiente_cobro,
                     QString entidad, QString oficina,QString dc ,QString cuenta);
    void CobrarDeudaTotal(int id_deuda);

    void anadir_entrega_cuenta(int id_cliente, QDate fecha, double importe_a_cuenta, QString concepto);

    void AnadirPersonaContacto(int id, QString Nombre, QString desc_telefono1, QString Telefono1, QString desc_telefono2,
                               QString Telefono2, QString desc_telefono3, QString Telefono3,
                               QString desc_movil1, QString Movil1, QString desc_movil2, QString Movil2, QString cargo,
                               QString email);
    void GuardarPersonaContacto(int id, QString Nombre, QString desc_telefono1, QString Telefono1, QString desc_telefono2,
                               QString Telefono2, QString desc_telefono3, QString Telefono3,
                               QString desc_movil1, QString Movil1, QString desc_movil2, QString Movil2, QString cargo,
                               QString email, int id_cliente);
    bool BorrarPersona_contacto(int id_persona);
    void Guardardireccion(bool Anadir, QString Descripcion, QString direccion1, QString direccion2, QString CP, QString Poblacion,
                         QString Provincia, QString Pais, int id_cliente, int id);
    void DescontarDeuda(int id_deuda,double pagado);
    void Borrar(int id_cliente);
    void BorrarWeb(int id_web);
    void Actualizar_de_web();
    QString Nuevocodigo_cliente();
    int Buscarid_pais(QString Pais);
    QString RecuperarPais(int nid);

};

#endif // CLIENTE_H
