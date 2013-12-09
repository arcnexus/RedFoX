#ifndef EMPRESA_H
#define EMPRESA_H

#include "../Auxiliares/Globlal_Include.h"

class Empresa : public QObject
{
    Q_OBJECT
public:
    explicit Empresa(QObject *parent = 0);
    void Anadir(QString id);
    void Recuperar(QString cSQL);
    void Recuperar(QString cSQL,int nProcede);
    void cargar(QSqlRecord record);
    void Guardar();
    void Vaciar();
    bool Borrar(int nid);


    // Getters
    int getid();
    QString getcodigo();
    QString getnombre();
    int getndigitos_factura();
    QString getserie();
    QString getruta_bd_sqlite();
    QString getcHost();
    QString getcUser();
    QString getcContrasena();
    QString getnombre_bd();
    QString getcPuerto();
    QString getcDriverBD();

    QString getruta_bd_sqliteMed();
    QString getcHostMed();
    QString getcUserMed();
    QString getcContrasenaMed();
    QString getnombre_bdMed();
    QString getcPuertoMed();
    QString getcDriverBDMed();

    QString getdireccion1();
    QString getcp();
    QString getpoblacion();
    QString getprovincia();
    QString getpais();
    QString gettelefono1();
    QString gettelefono2();
    QString getfax();
    QString getcMail();
    QString getweb();
    int getejercicio();
    QString getcif();
    QString getcInscripcion();
    QString getccomentario_factura();
    QString getccomentario_albaran();
    QString getccomentario_ticket();
    int getdigitos_cuentas();
    QString getcuenta_clientes();
    QString getcuenta_proveedores();
    QString getcuenta_acreedores();

    // Setters

    void setid(int id);
    void setcodigo(QString codigo);
    void setnombre(QString nombre);
    void setndigitos_factura(int ndigitos_factura);
    void setserie(QString serie);
    void setruta_bd_sqlite(QString ruta_bd_sqlite);
    void setcHost(QString cHost);
    void setcUser(QString cUser);
    void setcContrasena(QString cContrasena);
    void setnombre_bd(QString nombre_bd);
    void setcPuerto(QString cPuerto);
    void setcDriverBD(QString cDriverBD);

    void setruta_bd_sqliteMed(QString ruta_bd_sqliteMed);
    void setcHostMed(QString cHostMed);
    void setcUserMed(QString cUserMed);
    void setcContrasenaMed(QString cContrasenaMed);
    void setnombre_bdMed(QString nombre_bdMed);
    void setcPuertoMed(QString cPuertoMed);
    void setcDriverBDMed(QString cDriverBDMed);



    void setdireccion1(QString direccion1);
    void setcp(QString cp);
    void setpoblacion(QString poblacion);
    void setprovincia(QString provincia);
    void setpais(QString pais);
    void settelefono1(QString telefono1);
    void settelefono2(QString telefono2);
    void setfax(QString fax);
    void setcMail(QString cMail);
    void setweb(QString web);
    void setcif(QString cif);
    void setejercicio(int ejercicio);
    void setcInscripcion(QString cInscripcion);
    void setccomentario_factura(QString ccomentario_factura);
    void setcComemtarioAlbaran(QString ccomentario_albaran);
    void setccomentario_ticket(QString ccomentario_ticket);
    void setdigitos_cuentas(int digitos_cuentas);
    void setcodigo_cuenta_clientes(QString cuenta_clientes);
    void setcodigoCuentaProveedor(QString cuenta_proveedores);
    void setcodigo_cuenta_acreedores(QString cuentaAcreeedores);

signals:
    
public slots:

public:
    // Propiedades
    //Empresa
    QSqlQuery qEmpresa;
    QString cCodEmpresa;
    int id  ;
    int id_divisa;
    QString codigo;
    QString nombre;
    int ndigitos_factura;
    QString serie;
    QString direccion1;
    QString cp;
    QString poblacion;
    QString provincia;
    QString pais;
    QString telefono1;
    QString telefono2;
    QString fax;
    QString cMail;
    QString web;
    QString cif;
    int ejercicio;
    QString cInscripcion;
    QString ccomentario_factura;
    QString ccomentario_albaran;
    QString ccomentario_ticket;
    bool Autocodificar;
    int tamano_codigo;
    int id_tarifa_predeterminada;
    bool empresa_medica;
    bool empresa_internacional;
    bool enlace_web;
    bool irpf;
    float porc_irpf;
    int consultas;
    QString primer_dia_laborable;
    QString ultimo_dia_laborable;
    QString horario_primer_dia;
    QString horario_dia_normal;
    QString horario_ultimo_dia;
    bool actualizar_divisas;
    bool seguimiento;
    double margen;
    double margen_min;
    QString clave1;
    QString clave2;
    int caducidad_vales;

    // ConexiónBD
    QString ruta_bd_sqlite;
    QString cHost;
    QString cUser;
    QString cContrasena;
    QString nombre_bd;
    QString cPuerto;
    QString cDriverBD;
    QString ruta_bd_sqliteMed;
    QString cHostMed;
    QString cUserMed;
    QString cContrasenaMed;
    QString nombre_bdMed;
    QString cPuertoMed;
    QString cDriverBDMed;

    // Contabilidad
    bool contabilidad;
    int digitos_cuentas;
    QString cuenta_clientes;
    QString cuenta_proveedores;
    QString cuentaAcreeedores;
    QString cuenta_cobros;
    QString cuenta_pagos;
    QString cuenta_venta_mercaderias;
    QString cuenta_venta_servicios;
    QString cuenta_iva_repercutido1;
    QString cuenta_iva_repercutido2;
    QString cuenta_iva_repercutido3;
    QString cuenta_iva_repercutido4;
    QString cuenta_iva_soportado1;
    QString cuenta_iva_soportado2;
    QString cuenta_iva_soportado3;
    QString cuenta_iva_soportado4;
    QString cuenta_iva_repercutido_re1;
    QString cuenta_iva_repercutido_re2;
    QString cuenta_iva_repercutido_re3;
    QString cuenta_iva_repercutido_re4;
    QString cuenta_iva_soportado_re1;
    QString cuenta_iva_soportado_re2;
    QString cuenta_iva_soportado_re3;
    QString cuenta_iva_soportado_re4;
    // BD Contabilidad
    QString HostBD_contabilidad;
    QString nombre_bd_contabilidad;
    QString UsuarioBD_contabilidad;
    QString ContrasenaBD_contabilidad;
    QString puertoDB_contabilidad;
    QString driver_db_contabilidad;
    QString RutaBD_Contabilidad_sqlite;
    bool ticket_factura;
    // EMAIL
    QString Email_contrasena;
    QString Email_imap;
    QString Email_pop;
    QString Email_smtp;
    QString Email_usuario;

};

#endif // EMPRESA_H
