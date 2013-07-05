#ifndef ADDGROUPFROM_H
#define ADDGROUPFROM_H

#include <QDialog>
#include "../Auxiliares/Globlal_Include.h"
namespace Ui {
class addGroupFrom;
}

class addGroupFrom : public QDialog
{
    Q_OBJECT
    
public:
    explicit addGroupFrom(QWidget *parent = 0);
    ~addGroupFrom();

    QString getNombre() const;
    void setNombre(const QString &value);

    QString getHost() const;
    void setHost(const QString &value);

    QString getUser() const;
    void setUser(const QString &value);

    QString getPass() const;
    void setPass(const QString &value);

    int getPort() const;
    void setPort(int value);
    void setSPort(const QString &value);

    void _insertMonedas(QSqlQuery q, bool error);
    void _insertIVA(QSqlQuery q, bool error);
    void _insertNivelAcesso(bool error, QSqlQuery q);
    void _createTables(bool error, QSqlDatabase db);
    void _insertNewGroup(QString grupo);
    void _insertAdminUser(bool error, QSqlQuery q);
    void _insertPoblaciones(bool error, QSqlQuery q);

    int id;
private slots:
    void on_pushButton_2_clicked();

private:
    Ui::addGroupFrom *ui;
    QString nombre;
    QString host;
    QString user;
    QString pass;
    int port;
};

#endif // ADDGROUPFROM_H
