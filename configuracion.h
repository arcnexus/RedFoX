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
    QString cNivelUsuarioActivo;
    QSqlDatabase dbConfiguracion;
    QSqlQuery *QryConfiguracion;
    QFileDialog dialogo;

    QHash <QString,QSqlRecord> ivas;
    QSqlTableModel* iva_model;
    void Cargar_iva();

    QHash <QString,QSqlRecord> paises;
    QSqlTableModel* paises_model;
    void Cargar_paises();

    bool lProfesional;
    int nIRPF;
    int nDigitosCuentasContables;
    QString cCuentaClientes;
    QString cCuentaProveedores;
    QString cCuentaAcreedores;


    QString FormatoNumerico(QString cTexto);
    bool EsNumero(QString texto);
    void CargarDatos();
    QString ValidarCC(QString Entidad,QString Oficina, QString DC, QString CC);
    QString ValidarCC(QString Entidad, QString Oficina, QString CC);

    QDoubleValidator *validator_cantidad;
    QDoubleValidator *validator_porciento;
public slots:
    void format_text();
};

#endif // CONFIGURACION_H
