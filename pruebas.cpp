#include "pruebas.h"
#include "ui_pruebas.h"
#include <QPushButton>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QErrorMessage>
#include <QtSql>
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>

Pruebas::Pruebas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pruebas)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QSQLITE");

    /* esto es para mysql y postgres
    db.setDatabaseName("emp0999");
    db.setHostName("localhost");
    db.open("root","PatataBullida_99"); */

    db.setDatabaseName("/home/arcnexus/Informatica-Intelligent/programacio/Terra/DB/emp0999.sqlite");
    db.open();
    if (db.lastError().isValid())
        {
            QMessageBox::critical(0, "error:", db.lastError().text());

        }

    resultado = new QSqlQuery();
    QSqlRecord rec = resultado->record();
    int fila;
    resultado->prepare("Select * from tiposiva");
    if (resultado->exec()) {
        fila = 1;
        cabecera<<"No."<<"Name"<<"Adder";
        ui->tabla->setColumnCount(3);
        qDebug() <<rec.count();
        ui->tabla->setRowCount(10);
        boton = new QPushButton("click", this);
        qnumero = new QDoubleSpinBox();
        qtexto = new QLineEdit();
        ui->tabla->setHorizontalHeaderLabels(cabecera);
        while (resultado->next()) {
            qnumero->setValue(resultado->value(4).toDouble());
            qnumero->setAlignment(Qt::AlignRight);
            qtexto->setText(resultado->value(2).toString());
            ui->tabla->setCellWidget(fila,1,qtexto);
             ui->tabla->setCellWidget(fila, 2,qnumero);
             ui->tabla->setCellWidget(fila,0,boton);
             qDebug() <<fila <<qtexto->text();
             fila++;

        }
        qtexto->setText("Ta tia en boles");
        ui->tabla->setCellWidget(2,1,qtexto);
        ui->tabla->setColumnWidth(2,220);
     }   else {
        qDebug() << resultado->lastError();

      }

}

Pruebas::~Pruebas()
{
    delete ui;
    //delete resultado;
}
