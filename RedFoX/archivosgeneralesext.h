#ifndef ARCHIVOSGENERALESEXT_H
#define ARCHIVOSGENERALESEXT_H

#include <QObject>
#include "moduleextension.h"
class ArchivosGeneralesExt : public ModuleExtension
{
    Q_OBJECT
public:
    explicit ArchivosGeneralesExt(QObject *parent = 0);
    
    QList<QAction *> Extensions();
    QString ExtensionPath(){return "";}// Use '|' to subPaths

    MayaModule::module_zone module_zone(){return MayaModule::Mantenimiento;}
signals:
    
public slots:
private slots:
    void tipostarifa();
    void handle_monedas();
    void handle_paises();
   // void handle_fomasPago();
    void handle_tiposIVA();
    void handle_bancos();
    void handle_avisos();
    void handle_tipoCli();
private:
    QAction tarifa;
    QAction moneda;
    QAction paises;
//    QAction fPago;
    QAction iva;
    QAction bancos;
    QAction avisos;
    QAction tipoCli;
    QVector<QPair<QAction *, MayaModule::accessLevel> > _actions;
};

#endif // ARCHIVOSGENERALESEXT_H
