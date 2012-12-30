#include "frmanalitica2.h"
#include "ui_frmanalitica2.h"
#include <QtSql>
#include <QSqlQuery>
#include <QSqlRecord>

Frmanalitica2::Frmanalitica2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Frmanalitica2)
{
    ui->setupUi(this);
}

Frmanalitica2::~Frmanalitica2()
{
    delete ui;
}

void Frmanalitica2::cargarDatos(QString cTipo)
{
    QSqlQuery *qDatos = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qDatos->prepare("Select * from tiposanalitica2 where tipo = :cTipo");
    qDatos->bindValue(":cTipo",cTipo);
    QStringList cabecera;
    cabecera  <<QObject::tr("A")<< QObject::tr("Analítica")<<QObject::tr("Valor Referencia");
    QSqlRecord reg;
    QString cDescripcion;
    QString cValores;

    ui->tabla->setColumnCount(3);
    ui->tabla->setRowCount(0);
    ui->tabla->setHorizontalHeaderLabels(cabecera);
    ui->tabla->setColumnWidth(0,20);
    ui->tabla->setColumnWidth(1,300);
    ui->tabla->setColumnWidth(2,100);


    int pos = 0;

    if (qDatos->exec()) {
        while (qDatos->next()) {
            // Datos Analítica
            reg = qDatos->record();
            ui->tabla->setRowCount(pos+1);
            QTableWidgetItem *newItem = new QTableWidgetItem("A");
            // para que los elementos no sean editables
            newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
            newItem->setTextColor(Qt::black); // color texto
            ui->tabla->setItem(pos,0,newItem);

            // Descripcion
            cDescripcion = reg.field("descripcion").value().toString();
            QTableWidgetItem *newItem1 = new QTableWidgetItem(cDescripcion);
            // para que los elementos no sean editables
            newItem1->setFlags(newItem1->flags() & (~Qt::ItemIsEditable));
            newItem1->setTextColor(Qt::black); // color de los items
            ui->tabla->setItem(pos,1,newItem1);

            // valores
            QTableWidgetItem *newItem2 = new QTableWidgetItem(reg.field("valoresreferencia").value().toString());
            // para que los elementos no sean editables
            newItem2->setFlags(newItem2->flags() & (~Qt::ItemIsEditable));
            newItem2->setTextColor(Qt::black); // color de los items
            ui->tabla->setItem(pos,2,newItem2);

            pos++;
        }
    }
}




