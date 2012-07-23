#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QtCore>
#include <QtGui>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT
    
public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    const QString &getUsuario() const;
    
private slots:
    void on_btnAcceder_clicked();

private:
    Ui::Login *ui;
    QString path;

};

#endif // LOGIN_H
