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

#include <QJsonObject>
class Configuracion: public QObject
{
    Q_OBJECT
public:
    Configuracion(QObject * parent = 0);

    //Datos ususario activo
    QSqlQueryModel* usuarios_model;
    void CargarUsuarios();
    int id_usuario_activo;
    QString user_name;
    QString user_long_name;
    QString user_pass;
    bool    super_user;

    QString user_mail_smpt;
    QString user_mail_acc;
    QString user_mail_pass;
    int user_mail_port;

    //Bases de datos
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

    QJsonObject groupIMGJson;

    QSqlDatabase empresaDB;
    QString nombre_bd_empresa;
    QString ruta_bd_empresa;

    bool medic;
    QSqlDatabase medicaDB;
    QString nombre_bd_medica;
    QString ruta_bd_medica;

    bool contabilidad;
    QSqlDatabase contaDB;
    QString nombre_bd_conta;
    QString RutaBD_Conta;

    void transaction();
    void rollback();
    void commit();

    //Bases de datos web
    bool enlace_web;
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

    ///
    QSqlTableModel* client_model;

    //Datos empresa activa
    QSqlRecord empresa_record;
    int idEmpresa;
    QString nombreEmpresa;
    QString cCodEmpresaActiva;
    QString cEjercicio;
    QString serie;
    int ndigitos_factura;
    bool ticket_factura;
    bool descripcion_resumida_lineas;
    bool referencia_fabricante_lineas_compras;
    int id_divisa_local;
    QString pais;

    bool auto_codigoart;
    int tamano_codigo;
    int decimales_campos_totales;
    int decimales;
    bool mostrar_siempre; // si está activo mostrará los 0 en decimales a partir de estandar (2) hasta valor decimales

    bool internacional;
    bool tpv_forzar_cantidad;
    QString divisa_local;
    QString cod_divisa_local;
    QString divisa_local_reducida;
    double margen;
    double margen_minimo;
    bool activar_seguimiento;
    QString caja;
    double Importe_apertura;
    int caducidad_vales;
    bool lProfesional;
    bool irpf;
    float porc_irpf;
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

    //Models centralizados
    QHash <QString,QSqlRecord> ivas;
    QSqlQueryModel* iva_model;        
    QSqlQueryModel *paises_model;
    QSqlQueryModel *divisas_model;
    QSqlQueryModel *formapago_model;
    QSqlQueryModel *agentes_model;
    QSqlQueryModel *idiomas_model;
    QSqlQueryModel *tarifas_model;
    QSqlQueryModel *grupos_gasto_model;

    QStringList ivaList;
    QStringList grupo_iva;
    QStringList reList;

    void Cargar_iva();
    int getidIva(QString cIva);
    QString setTipoIva(int idIva);
    void CargarDatosMaestros();
    void Cargar_paises();
    void Cargar_divisas();
    void Cargar_formas_pago();
    void Cargar_agentes();
    void Cargar_idiomas();
    void Cargar_tarifas();
    void Cargar_gastos();

    int Devolver_id_tarifa(QString nombre);
    int Devolver_id_moneda(QString nombre);
    QString Devolver_pais(int id);
    QString Devolver_codigo_forma_pago(int id);
    QString Devolver_moneda(int id);
    QString Devolver_codDivisa(int id);
    QString Devolver_descripcion_tipo_iva(int id);
    int Devolver_id_pais(QString nombre);
    QString Devolver_tarifa(int id);
    QString Devolver_idioma(int id);
    QString Devolver_tipo_gasto(int id_gasto);
    int Devolver_id_tipo_gasto(QString desc);
    int Devolver_id_cuenta_contable(QString codigo_cta);
    QString Devolver_descripcion_cuenta_contable(QString codigo_cta);
    QString Devolver_codigo_cta_contable(int id);

    QString toRound(double number, int decimals);

    QString devolver_transportista(int id);
    int devolver_id_transportista(QString transportista);

    int devolver_id_tabla(QSqlQueryModel *model, QModelIndex index);
    void CargarClientes();

    QString toFormatoMoneda(QString cTexto);
    double MonedatoDouble(QString moneda);
    bool EsNumero(QString texto);
    void CargarDatos(QSqlRecord r);
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

    //Crypto wrapper
    static QString Crypt(QString input);
    static QString DeCrypt(QString input);
    static QString SHA256HashString(QString input);


    void getCambio(QString from, QString to , float cuanty = 1);
    float getCambioBlock(QString from, QString to , float cuanty = 1);

    void updateTablaDivisas(QString current);


    //UTILES IMPRIMIR - ODS
    static void ImprimirDirecto(QString report, QMap<QString,QString> queryClausules,QMap<QString, QString> params);
    static void ImprimirPDF(QString report, QMap<QString,QString> queryClausules, QMap<QString, QString> params);
    static void ImprimirPreview(QString report, QMap<QString,QString> queryClausules,QMap<QString, QString> params);
    static void EviarMail(QString report, QMap<QString,QString> queryClausules, QMap<QString, QString> params, QString pdfName, QString dest_mail, QString dest_name, QString asunto, QString texto);
    static bool SqlToODS(QString fileName, QString query, QSqlDatabase db, QStringList headers, QString& error);


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


    bool _block;
    float _cambio;


    QString search_string(QSqlQueryModel* model,int id, QString column);
    QVariant _search(QSqlQueryModel* model, QVariant search_for, QString in_column , QString output_column);
};

#endif // CONFIGURACION_H
