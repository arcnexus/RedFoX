#ifndef CLINICAEXT_H
#define CLINICAEXT_H

#include <QObject>
#include "../moduleextension.h"
class ClinicaExt : public ModuleExtension
{
    Q_OBJECT
public:
    explicit ClinicaExt(QObject *parent = 0);
    
    QList<QAction *> Extensions();
    QString ExtensionPath(){return "";}// Use '|' to subPaths

    MayaModule::moduleZone ModuleZone(){return MayaModule::SecretariaMedica;}
signals:
    
public slots:
private slots:
    void handle_doctores();
    void handle_tiposImagen();
    void handle_tipoAnalitica();
    void handle_campoAnalitica();
    void handle_motivoInterConsulta();
private:
    QAction doctores;
    QAction tImagen;
    QAction tAnalitica;
    QAction cAnalitica;
    QAction mInterCon;
    QVector<QPair<QAction *, MayaModule::accessLevel> > _actions;
};

#endif // CLINICAEXT_H
