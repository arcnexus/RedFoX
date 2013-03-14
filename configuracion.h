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
class Configuracion: public QObject
{
    Q_OBJECT
public:
    Configuracion(QObject * parent = 0);

    QString cRutaBdTerra;
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
    QString cDriverBDTerra;
    QString cHostBDTerra;
    QString cUsuarioBDTerra;
    QString cPasswordBDTerra;
    QString cNombreBDTerra;
    QString cPuertoBDTerra;
    QString cPais;
    QString cEjercicio;
    QString cEmpresaActiva;
    QString cUsuarioActivo;
    int id_usuario_activo;
    QString cNivelUsuarioActivo;
    QSqlDatabase dbConfiguracion;
    QSqlQuery *QryConfiguracion;
    QFileDialog dialogo;
    bool Autocodigoart;
    int tamanocodigo;
    bool medic;
    bool internacional;

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

    QString Devolver_pais(int id);
    QString Devolver_moneda(int id);
    QString Devolver_idioma(int id);
    int Devolver_id_idioma(QString idioma);

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




    QString FormatoNumerico(QString cTexto);
    bool EsNumero(QString texto);
    void CargarDatos();
    void CargarDatosBD();
    QString ValidarCC(QString Entidad,QString Oficina, QString DC, QString CC);
    QString ValidarCC(QString Entidad, QString Oficina, QString CC);
    QString ValidarnifE(QString Nif);

    QDoubleValidator *validator_cantidad;
    QDoubleValidator *validator_porciento;

    static QString letraDNI(QString Nif);
    static void imprimir(QString db , QString report , bool toPDF , bool preview = true, QWidget* parent = 0);
    static bool comprobarNIF(QString country_code, QString nif);
    static QString Crypt(QString input);
    static QString DeCrypt(QString input);
public slots:
    void format_text();
};

#endif // CONFIGURACION_H
