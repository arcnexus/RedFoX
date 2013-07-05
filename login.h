#ifndef LOGIN_H
#define LOGIN_H

#include "../Auxiliares/Globlal_Include.h"



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
    int getid_user();


private slots:
    void init();
    void on_btnAcceder_clicked();

    void Crearconfiguracion_clicked();

    void btnEmpresa_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboGroup_currentIndexChanged(int index);

private:
    Ui::Login *ui;
    QString path;
    QSqlDatabase dbMaya;
    int m_id;
    QHash<int, QPair<QStringList , QSqlRecord>> _empresas;

};

#endif // LOGIN_H
