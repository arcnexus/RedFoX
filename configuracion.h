#ifndef CONFIGURACION_H
#define CONFIGURACION_H

//#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileDialog>
#include <QLineEdit>

class Configuracion{

public:
    Configuracion();
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
    double nIVA1,nIVA2,nIVA3,nIVA4,nRE1,nRE2,nRE3,nRE4;
    bool lProfesional;
    int nIRPF;
    int nDigitosCuentasContables;
    QString cCuentaClientes;
    QString cCuentaProveedores;
    QString cCuentaAcreedores;


    QString FormatoNumerico(QString cTexto);
    bool EsNumero(QString texto);
    void CargarDatos();

    

};

#endif // CONFIGURACION_H
