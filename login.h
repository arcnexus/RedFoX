#ifndef LOGIN_H
#define LOGIN_H

#include "Auxiliares/Globlal_Include.h"



namespace Ui {
class Login;
}
/*THEFOX*/
class Login : public QDialog
{
    Q_OBJECT
    
public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    const QString getUsuario() const;
    const QString getPass() const;
    const QString getEmpresa() const;
    int getIdUser();
private slots:
	void init();
    void on_btnAcceder_clicked();

    void Crearconfiguracion_clicked();

    void btnEmpresa_clicked();

    void on_pushButton_clicked();

private:
    Ui::Login *ui;
    QString path;
    QSqlDatabase dbTerra;
    int m_id;

};

#endif // LOGIN_H
