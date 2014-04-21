#ifndef IMPORTARARTICULOEXT_H
#define IMPORTARARTICULOEXT_H

#include <QObject>
#include "../moduleextension.h"

class ImportarArticuloExt : public ModuleExtension
{
    Q_OBJECT
public:
    explicit ImportarArticuloExt(QObject *parent = 0);

    QList<QAction *> Extensions();
    QString ExtensionPath(){return tr("Importar artículos");}

    MayaModule::module_zone module_zone(){return MayaModule::Almacen;}
signals:
private slots:
    void do_ods();

private:
    QAction fromOds;
    QVector<QPair<QAction *, MayaModule::accessLevel> > _actions;

};

#endif // IMPORTARARTICULOEXT_H
