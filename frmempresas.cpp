#include "frmempresas.h"
#include "ui_frmempresas.h"
#include <QtSql>
#include <QSqlDatabase>
#include<QSqlQuery>
#include <QMessageBox>

FrmEmpresas::FrmEmpresas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmEmpresas)
{
    ui->setupUi(this);
    ui->txtMensaje->setText("ATENCION: Modifique con cuidado. Cambiar estos\n valores puede hacer que el programa deje\n de funcionar");
}

FrmEmpresas::~FrmEmpresas()
{
    delete ui;
}

void FrmEmpresas::on_pushButton_clicked()
{
    QSqlQuery *Empresa = new QSqlQuery(QSqlDatabase::database("terra"));
    Empresa->prepare("insert into empresas (codigo,nombre,digitosfactura,serie,RutaBDSqLite,driverBD) "
                     "Values('04','Empresa Julian',5,'2012','/home/dismet/Dismet/Programming/terra/DB/emp0999.sqlite','QSQLITE')");
    if(Empresa->exec())
        QMessageBox::information(NULL,"Empresas","Enhorabuena, su empresa ha sido creada\n "
                                 "Debe salir y volver a entrar del programa para poder acceder a ella","Ok");
    else
        QMessageBox::warning(NULL,"Empresas","Ha ocurrido un error :" +Empresa->lastError().text());

}
