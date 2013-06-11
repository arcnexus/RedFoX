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
    tus includes aqui
#endif

class Configuracion: public QObject
{
    Q_OBJECT
public:
    Configuracion(QObject * parent = 0);

    QString cRutaBdMaya;
    QString cSerie;
    int nDigitosFactura;
    QString cConector;
    int idEmpresa;
    QString cCodEmpresaActiva;
    QString cNombreEmpresa;
    QString cRutaBdEmpresa;
    QString cDriverBDEmpresa;
    QString cHostBDEmpresa;
    QString cUsuarioBDEmpresa;
    QString cPasswordBDEmpresa;
    QString cNombreBDEmpresa;
    QString cPuertoBDEmpresa;
    QString cRutaBdMedica;
    QString cDriverBDMedica;
    QString cHostBDMedica;
    QString cUsuarioBDMedica;
    QString cPasswordBDMedica;
    QString cNombreBDMedica;
    QString cPuertoBDMedica;
    QString cDriverBDMaya;
    QString cHostBDMaya;
    QString cUsuarioBDMaya;
    QString cPasswordBDMaya;
    QString cNombreBDMaya;
    QString cPuertoBDMaya;
    QString cNombreBDTiendaWeb;
    QString cHostBDTiendaWeb;
    QString cUsuarioTiendaWeb;
    QString cPasswordTiendaWeb;
    QString cPuertoTiendaWeb;
    QString HostDB_MediTec;
    QString NombreDB_MediTec;
    QString UsuarioDB_MediTec;
    QString PasswordDB_MediTec;
    QString PortDB_MediTec;
    QString RutaBD_Conta;
    QString DriverBD_Conta;
    QString HostDB_Conta;
    QString NombreDB_Conta;
    QString usuarioDB_Conta;
    QString passwordDB_Conta;
    QString portDB_Conta;
    bool contabilidad;
    bool ticket_factura;

    bool EnlaceWeb;
    QString cPais;
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
    bool Autocodigoart;
    int tamanocodigo;
    bool medic;
    bool internacional;
    QString DivisaLocal;
    QString codDivisaLocal;
    QString DivisaLocalReducida;
    double margen;
    double margen_minimo;
    bool activar_seguimiento;

    QHash <QString,QSqlRecord> ivas;
    QSqlTableModel* iva_model;
    QStringList ivaList;
    QStringList reList;
    void Cargar_iva();
    int getIdIva(QString cIva);
    QString setTipoIva(int idIva);

    QHash <QString,QSqlRecord> paises;
    QSqlTableModel* paises_model;
    void Cargar_paises();

    int Devolver_id_pais(QString cPais);
    static QString Devolver_pais(int id);
    QString Devolver_moneda(int id);
    QString Devolver_codDivisa(int id);
    int Devolver_id_moneda(QString cDivisa);
    QString Devolver_idioma(int id);
    int Devolver_id_idioma(QString idioma);
    int Devolver_id_forma_pago(QString cFormaPago);
    int Devolver_id_codigo_forma_pago(QString cCodigo);
    QString Devolver_forma_pago(int id);
    QString Devolver_codigo_forma_pago(int id);
    int Devolver_id_tarifa(QString cTarifa);
    QString Devolver_tarifa(int id_tarifa);
    QString Devolver_tipo_gasto(int id_gasto);
    int Devolver_id_tipo_gasto(QString desc);

    QSqlTableModel* client_model;
    void CargarClientes();

    QSqlTableModel* usuarios_model;
    void CargarUsuarios();

    bool lProfesional;
    int nIRPF;
    int nDigitosCuentasContables;
    QString cCuentaClientes;
    QString cCuentaProveedores;
    QString cCuentaAcreedores;
    QString cCuentaIvaRepercutido;
    QString cCuentaIvaSoportado;
    QString cCuentaDeudasClientes;
    QString cCuentaCobrosClientes;
    QString cCuentaDeudasProveedor;
    QString cCuentaPagosProveedor;
    int id_tarifa_predeterminada;
    bool actualizardivisas;



    QString FormatoNumerico(QString cTexto);
    double MonedatoDouble(QString moneda);
    bool EsNumero(QString texto);
    void CargarDatos();
    void CargarDatosBD();
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
    static void imprimir(bool toPDF , bool preview = true, QWidget* parent = 0);
    static bool comprobarNIF(QString country_code, QString nif);
    static QString Crypt(QString input);
    static QString DeCrypt(QString input);
    static QString SHA256HashString(QString input);
    void getCambio(QString from, QString to , float cuanty = 1);
    void generarTablaDivisas();
    void updateTablaDivisas(QString current);
public slots:
    void format_text();    
private slots:
    void cambioReply(QNetworkReply * reply);
    void applyCambio(float f,QString target);
signals:
    void cambioReady(float,QString target = "");
private:
    void readCambio(QString s);
    static byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ];
    static byte iv[ CryptoPP::AES::BLOCKSIZE ];
};

#endif // CONFIGURACION_H
