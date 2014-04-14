#include "editdinamycitemdlg.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include "Auxiliares/Globlal_Include.h"

bool EditDinamycItemDlg::isSet = false;
QStringList EditDinamycItemDlg::_zonas;
QMap<QString,QStringList> EditDinamycItemDlg::_tablas;
QMap<QString,QStringList> EditDinamycItemDlg::_campos;
#include <QSqlError>
EditDinamycItemDlg::EditDinamycItemDlg(QWidget *parent) :
    QDialog(parent)
{
    if(!isSet)
    {
        QSqlQuery x(QSqlDatabase::database("grupo"));
        if(x.exec("Select valor_stock FROM articulos"))
            while (x.next()) {
                qDebug() << "valor_stock" << x.record().value("valor_stock");
            }
        else
            qDebug() << x.lastError();
        _zonas << "General" << "Empresa";

        //TODO cambiar dentro de Maya

        QStringList groupTables;
        QSqlQuery q(Configuracion_global->groupDB);
        q.exec("show tables");
        while(q.next())
            groupTables << q.record().value(0).toString();

        _tablas.insert("General",groupTables);

        QStringListIterator it(groupTables);
        while(it.hasNext())
        {
            QString tabla = it.next();
            QStringList campos;
            QString s = QString("show columns from %1").arg(tabla);
            q.exec(s);
            while(q.next())
                campos << q.record().value(0).toString();
            _campos.insert(tabla,campos);            
        }

        QStringList empTables;
        QSqlQuery q2(Configuracion_global->empresaDB);
        q2.exec("show tables");
        while(q2.next())
            empTables << q2.record().value(0).toString();

        _tablas.insert("Empresa",empTables);

        QStringListIterator it2(empTables);
        while(it2.hasNext())
        {
            QString tabla = it2.next();
            QStringList campos;
            QString s = QString("show columns from %1").arg(tabla);
            q2.exec(s);
            while(q2.next())
                campos << q2.record().value(0).toString();


            _campos.insert(tabla,campos);
        }

        isSet = true;
    }
}

QStringList EditDinamycItemDlg::zonas()
{
    return EditDinamycItemDlg::_zonas;
}

QStringList EditDinamycItemDlg::tablas(QString zona)
{
    return _tablas.value(zona);
}

QStringList EditDinamycItemDlg::campos(QString tabla)
{
    return _campos.value(tabla);
}
