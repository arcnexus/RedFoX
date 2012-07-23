#include "login.h"
#include "ui_login.h"
#include <QSqlDatabase>
//#include "conexion.h"
#include <QtSql>
#include <QErrorMessage>
#include <QMessageBox>
#include<QSqlError>
#include<mainwindow.h>
#include <QDir>

/*
 * Conexión postgres
//QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL7");
*/
// Conexión SQLite3
//QString privatePathQt(QApplication::applicationDirPath());
//    QString path(privatePathQt);
//    path.append(QDir::separator()).append("DB");
//    path = QDir::toNativeSeparators(path);

//#ifdef Q_WS_SIMULATOR
//    path = "sqlite"
//#endif

//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    qDebug() << path;
//    db.setDatabaseName(path);
//    bool ok = db.open();
//    qDebug() << db.tables();
QSqlDatabase dbTerra = QSqlDatabase::addDatabase("QSQLITE","terra");

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
/* Datos conexión postGres
   // db.setDatabaseName("postgres");
    //db.setHostName("localhost");
    //return db;
   // QSqlDatabase db =createConnection();
   // db.open("postgres","PatataBullida_99");
   */
 // Conexión SQLite
    dbTerra.setDatabaseName("/home/arcnexus/Informatica-Intelligent/programacio/Terra/DB/Terra.sqlite");
    dbTerra.open();
    if (dbTerra.lastError().isValid())
        {
            QMessageBox::critical(0, "error:", dbTerra.lastError().text());

        }
    this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    ui->setupUi(this);

    this->ui->lineUsuario->setFocus();
}

Login::~Login()
{

    delete ui;

    dbTerra.close();
}

const QString &Login::getUsuario() const
{
    return ui->lineUsuario->text();
}

void Login::on_btnAcceder_clicked()
{

    QSqlQuery qryUsers(QSqlDatabase::database("terra"));
    qryUsers.prepare( "SELECT * FROM usuarios where nombre =:nombre" );
    qryUsers.bindValue(":nombre",ui->lineUsuario->text());
    if( !qryUsers.exec() ) {
        QMessageBox::critical(NULL, "error:", qryUsers.lastError().text());
    } else {
        if (qryUsers.next()) {
            if (ui->linePassword->text()==qryUsers.value(2).toString()) {
               //MainWindow.ui.lineEdit->setText(qryUsers.value(1).toString());
                Login::done( QDialog::Accepted);

            } else {
                QMessageBox::critical(0,"ACCESO DENEGADO","El usuario y la contraseña no se corresponden\n\n Verifique los datos");
                ui->linePassword->setText("");
                ui->linePassword->setFocus();
            }

        } else {
            //ui->lineEdit->setText("No existe");
            QMessageBox::critical(0,"Error","No existe ningún usuario con este nombre");
            ui->lineUsuario->setText("");
            //ui->lineEdit->setText("");
            ui->lineUsuario->setFocus();
        }
    }
    qryUsers.clear();
    qryUsers.finish();
}
