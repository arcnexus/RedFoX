#ifndef LOGIN_H
#define LOGIN_H

#include "../Auxiliares/Globlal_Include.h"



namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT
    
public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    const QString getUsuario() const;
    const QString getPass() const;
    const QString getEmpresaName();
    const QSqlRecord getEmpresa() const;
    int getid_user();


private slots:
    bool init();
    void on_btnAcceder_clicked();

    void Crearconfiguracion_clicked();

    void btnEmpresa_clicked();

    void on_pushButton_clicked();

    void on_comboGroup_currentTextChanged(const QString &arg1);

private:
    Ui::Login *ui;
    QString path;
    QSqlDatabase dbMaya;
    int m_id;

    struct strc_empresa{
        QSqlRecord rec_grupo;
        QStringList empresas;
        QMap<QString, QSqlRecord> rec_empresas;
    };

    QMap<QString,strc_empresa> _empresas;
};

#endif // LOGIN_H
