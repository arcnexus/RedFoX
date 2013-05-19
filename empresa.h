#ifndef EMPRESA_H
#define EMPRESA_H

#include "Auxiliares/Globlal_Include.h"

class Empresa : public QObject
{
    Q_OBJECT
public:
    explicit Empresa(QObject *parent = 0);
    void Anadir(QString id);
    void Recuperar(QString cSQL);
    void Recuperar(QString cSQL,int nProcede);
    void Guardar();
    void Vaciar();
    bool Borrar(int nId);


    // Getters
    int getid();
    QString getcCodigo();
    QString getcNombre();
    int getnDigitosFactura();
    QString getcSerie();
    QString getcRutaBDSqLite();
    QString getcHost();
    QString getcUser();
    QString getcContrasena();
    QString getcNombreBD();
    QString getcPuerto();
    QString getcDriverBD();

    QString getcRutaBDSqLiteMed();
    QString getcHostMed();
    QString getcUserMed();
    QString getcContrasenaMed();
    QString getcNombreBDMed();
    QString getcPuertoMed();
    QString getcDriverBDMed();

    QString getcDireccion();
    QString getcCp();
    QString getcPoblacion();
    QString getcProvincia();
    QString getcPais();
    QString getcTelefono1();
    QString getcTelefono2();
    QString getcFax();
    QString getcMail();
    QString getcWeb();
    int getnEjercicio();
    QString getcCif();
    QString getcInscripcion();
    QString getcComentarioFactura();
    QString getcComentarioAlbaran();
    QString getcComentarioTicket();
    int getnDigitosCuentas();
    QString getcCuentaClientes();
    QString getcCuentaProveedores();
    QString getcCuentaAcreedores();

    // Setters

    void setid(int id);
    void setcCodigo(QString cCodigo);
    void setcNombre(QString cNombre);
    void setnDigitosFactura(int nDigitosFactura);
    void setcSerie(QString cSerie);
    void setcRutaBDSqLite(QString cRutaBDSqLite);
    void setcHost(QString cHost);
    void setcUser(QString cUser);
    void setcContrasena(QString cContrasena);
    void setcNombreBD(QString cNombreBD);
    void setcPuerto(QString cPuerto);
    void setcDriverBD(QString cDriverBD);

    void setcRutaBDSqLiteMed(QString cRutaBDSqLiteMed);
    void setcHostMed(QString cHostMed);
    void setcUserMed(QString cUserMed);
    void setcContrasenaMed(QString cContrasenaMed);
    void setcNombreBDMed(QString cNombreBDMed);
    void setcPuertoMed(QString cPuertoMed);
    void setcDriverBDMed(QString cDriverBDMed);



    void setcDireccion(QString cDireccion);
    void setcCP(QString cCp);
    void setcPoblacion(QString cPoblacion);
    void setcProvincia(QString cProvincia);
    void setcPais(QString cPais);
    void setcTelefono1(QString cTelefono1);
    void setcTelefono2(QString cTelefono2);
    void setcFax(QString cFax);
    void setcMail(QString cMail);
    void setcWeb(QString cWeb);
    void setcCif(QString cCif);
    void setnEjercicio(int nEjercicio);
    void setcInscripcion(QString cInscripcion);
    void setcComentarioFactura(QString cComentarioFactura);
    void setcComemtarioAlbaran(QString cComentarioAlbaran);
    void setcComentarioTicket(QString cComentarioTicket);
    void setnDigitosCuentas(int nDigitosCuentas);
    void setcCodigoCuentaClientes(QString cCuentaClientes);
    void setcCodigoCuentaProveedor(QString cCuentaProveedores);
    void setcCodigoCuentaAcreedores(QString cCuentaAcreeedores);

signals:
    
public slots:

public:
    // Propiedades
    //Empresa
    QSqlQuery qEmpresa;
    QString cCodEmpresa;
    int id  ;
    int id_divisa;
    QString cCodigo;
    QString cNombre;
    int nDigitosFactura;
    QString cSerie;
    QString cDireccion;
    QString cCp;
    QString cPoblacion;
    QString cProvincia;
    QString cPais;
    QString cTelefono1;
    QString cTelefono2;
    QString cFax;
    QString cMail;
    QString cWeb;
    QString cCif;
    int nEjercicio;
    QString cInscripcion;
    QString cComentarioFactura;
    QString cComentarioAlbaran;
    QString cComentarioTicket;
    bool Autocodificar;
    int Tamanocodigo;

    int consultas;
    QString primer_dia_laborable;
    QString ultimo_dia_laborable;
    QString horario_primer_dia;
    QString horario_dia_normal;
    QString horario_ultimo_dia;

    // ConexiónBD
    QString cRutaBDSqLite;
    QString cHost;
    QString cUser;
    QString cContrasena;
    QString cNombreBD;
    QString cPuerto;
    QString cDriverBD;
    QString cRutaBDSqLiteMed;
    QString cHostMed;
    QString cUserMed;
    QString cContrasenaMed;
    QString cNombreBDMed;
    QString cPuertoMed;
    QString cDriverBDMed;

    // Contabilidad
    bool contabilidad;
    int nDigitosCuentas;
    QString cCuentaClientes;
    QString cCuentaProveedores;
    QString cCuentaAcreeedores;
    QString cCuentaCobros;
    QString cCuentaPagos;
    QString HostBD_contabilidad;
    QString NombreBD_contabilidad;
    QString UsuarioBD_contabilidad;
    QString ContrasenaBD_contabilidad;
    QString puertoDB_contabilidad;
    QString DriverDB_contabilidad;
    QString RutaBD_Contabilidad_sqlite;
    bool ticket_factura;

};

#endif // EMPRESA_H
