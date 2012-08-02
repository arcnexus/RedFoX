#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <configuracion.h>
#include <QSqlDatabase>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT
    
public:
    explicit Login(Configuracion *m_config, QWidget *parent = 0);
    ~Login();

    const QString &getUsuario() const;
    const QString &getEmpresa() const;
    
private slots:
    void on_btnAcceder_clicked();

    void on_Crearconfiguracin_clicked();

private:
    Ui::Login *ui;
    QString path;
    QSqlDatabase dbTerra;

};

#endif // LOGIN_H
