#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QSettings>
#include <QVariant>
#include <QMessageBox>
#include <QApplication>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QObject>
#include <QDoubleValidator>
#include <QNetworkReply>

//CryptoPP headers
#ifdef Q_OS_WIN32
#include <C:/CriptoCpp/modes.h>
#include <C:/CriptoCpp/filters.h>
#include <C:/CriptoCpp/aes.h>
#include <C:/CriptoCpp/sha.h>
#include <C:/CriptoCpp/base64.h>
#else
#include </usr/include/cryptopp/modes.h>
#include </usr/include/cryptopp/filters.h>
#include </usr/include/cryptopp/aes.h>
#include </usr/include/cryptopp/sha.h>
#include </usr/include/cryptopp/base64.h>
#endif

class Configuracion: public QObject
{
    Q_OBJECT
public:
    Configuracion(QObject * parent = 0);

    QSqlDatabase globalDB;
    QString global_driver;
    QString global_host;
    QString global_user;
    QString global_pass;
    int global_port;
    QString global_routeLite;

    QSqlDatabase groupDB;
    QString group_DBName;
    QString group_host;
    QString group_user;
    QString group_pass;
    int group_port;
    QString group_Driver;

    QSqlDatabase empresaDB;
    QString nombre_bd_empresa;
    QString ruta_bd_empresa;

    QSqlDatabase reportsDB;

    QSqlDatabase medicaDB;
    QString nombre_bd_medica;
    QString ruta_bd_medica;

    QSqlDatabase contaDB;
    QString nombre_bd_conta;
    QString RutaBD_Conta;

    QString nombre_bdTiendaWeb;
    QString cHostBDTiendaWeb;
    QString cUsuarioTiendaWeb;
    QString cPasswordTiendaWeb;
    QString cPuertoTiendaWeb;

    QString HostDB_MediTec;
    QString NombreDB_MediTec;
    QString UsuarioDB_MediTec;
    QString PasswordDB_MediTec;
    QString PortDB_MediTec;

    QString serie;
    int ndigitos_factura;
    QString cConector;
    int idEmpresa;
    QString cCodEmpresaActiva;
    QString nombreEmpresa;

    bool contabilidad;
    bool ticket_factura;

    bool enlace_web;
    QString pais;
    QString cEjercicio;
    QString cEmpresaActiva;
    QString cUsuarioActivo;
    int id_usuario_activo;
    QString cNivelUsuarioActivo;
    QSqlDatabase dbConfiguracion;
    QSqlDatabase dbWeb;
    QSqlDatabase db_meditec;
    QSqlQuery *QryConfiguracion;
    QFileDialog dialogo;
    bool auto_codigoart;
    int tamano_codigo;
    int decimales_campos_totales;
    int decimales;
    bool mostrar_siempre; // si está activo mostrará los 0 en decimales a partir de estandar (2) hasta valor decimales
    bool medic;
    bool internacional;
    QString divisa_local;
    QString cod_divisa_local;
    QString divisa_local_reducida;
    double margen;
    double margen_minimo;
    bool activar_seguimiento;

    QHash <QString,QSqlRecord> ivas;
    QSqlTableModel* iva_model;
    QStringList ivaList;
    QStringList grupo_iva;
    QStringList reList;
    void Cargar_iva();
    int getidIva(QString cIva);
    QString setTipoIva(int idIva);

    QHash <QString,QSqlRecord> paises;
    QSqlTableModel* paises_model;
    void Cargar_paises();

    int Devolver_id_pais(QString pais);
    static QString Devolver_pais(int id);
    QString Devolver_moneda(int id);
    QString Devolver_codDivisa(int id);
    int Devolver_id_moneda(QString cDivisa);
    QString Devolver_idioma(int id);
    int Devolver_id_idioma(QString idioma);
    int Devolver_id_forma_pago(QString forma_pago);
    int Devolver_id_codigo_forma_pago(QString codigo);
    QString Devolver_forma_pago(int id);
    QString Devolver_codigo_forma_pago(int id);
    int Devolver_id_tarifa(QString cTarifa);
    QString Devolver_tarifa(int id_tarifa);
    QString Devolver_tipo_gasto(int id_gasto);
    int Devolver_id_tipo_gasto(QString desc);
    int Devolver_id_cuenta_contable(QString codigo_cta);
    QString Devolver_descripcion_tipo_iva(double tipo);
    QString Devolver_descripcion_cuenta_contable(QString codigo_cta);
    QString Devolver_codigo_cta_contable(int id);
    QString devolver_codigo_articulo(int id);
    QString devolver_codigo_barras(int id);
    QString devolver_referencia_articulo(int id);
    float devolver_rec_iva(float porc_iva);
    QString devolver_agente(int id);
    int devolver_id_agente(QString agente);
    QString devolver_transportista(int id);
    int devolver_id_transportista(QString transportista);
    QSqlTableModel* client_model;
    int devolver_id_tabla(QSqlQueryModel *model, QModelIndex index);
    void CargarClientes();

    QSqlTableModel* usuarios_model;
    void CargarUsuarios();

    bool lProfesional;
    int irpf;
    int digitos_cuentas_contables;
    QString cuenta_clientes;
    QString cuenta_proveedores;
    QString cuenta_acreedores;
    QString cuenta_iva_repercutido1;
    QString cuenta_iva_repercutido2;
    QString cuenta_iva_repercutido3;
    QString cuenta_iva_repercutido4;
    QString cuenta_iva_soportado1;
    QString cuenta_iva_soportado2;
    QString cuenta_iva_soportado3;
    QString cuenta_iva_soportado4;
    QString cuenta_iva_repercutidoRe1;
    QString cuenta_iva_repercutidoRe2;
    QString cuenta_iva_repercutidoRe3;
    QString cuenta_iva_repercutidoRe4;
    QString cuenta_iva_soportado_re1;
    QString cuenta_iva_soportado_re2;
    QString cuenta_iva_soportado_re3;
    QString cuenta_iva_soportado_re4;
    QString cuenta_deudasClientes;
    QString cuenta_cobrosClientes;
    QString cuenta_deudasProveedor;
    QString cuenta_pagosProveedor;
    QString cuenta_venta_mercaderias;
    QString cuenta_venta_servicios;
    int id_tarifa_predeterminada;
    bool actualizar_divisas;



    QString toFormatoMoneda(QString cTexto);
    double MonedatoDouble(QString moneda);
    bool EsNumero(QString texto);
    void CargarDatos(int id);
    bool CargarDatosBD();
    void AbrirDbWeb();
    void CerrarDbWeb();
    void AbridBDMediTec();
    void CerrarBDMediTec();
    QString ValidarCC(QString Entidad,QString Oficina, QString DC, QString CC);
    QString ValidarCC(QString Entidad, QString Oficina, QString CC);
    QString ValidarnifE(QString Nif);

    QDoubleValidator *validator_cantidad;
    QDoubleValidator *validator_porciento;

    static QString letraDNI(QString Nif);

    static bool comprobarNIF(QString country_code, QString nif);
    static QString Crypt(QString input);
    static QString DeCrypt(QString input);
    static QString SHA256HashString(QString input);
    void getCambio(QString from, QString to , float cuanty = 1);
    float getCambioBlock(QString from, QString to , float cuanty = 1);

    void updateTablaDivisas(QString current);


    static void ImprimirDirecto(QString report, QMap<QString,QString> queryClausules);
    static void ImprimirPDF(QString report, QMap<QString,QString> queryClausules);
    static void ImprimirPreview(QString report, QMap<QString,QString> queryClausules);
public slots:
    void format_text();    
private slots:
    void cambioReply(QNetworkReply * reply);
    void cambioReplyBlock(QNetworkReply * reply);
    void applyCambio(float f,QString target);
signals:
    void cambioReady(float,QString target = "");
private:
    void readCambio(QString s);
    float readCambioBlock(QString s);
    static byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ];
    static byte iv[ CryptoPP::AES::BLOCKSIZE ];
    bool _block;
    float _cambio;
};

#endif // CONFIGURACION_H
