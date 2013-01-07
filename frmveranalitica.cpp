#include "frmveranalitica.h"
#include "ui_frmveranalitica.h"
#include <QtSql>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "analitica.h"

    Analitica *oAnalitica1 = new Analitica();
    FrmVerAnalitica::FrmVerAnalitica(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmVerAnalitica)
{
    ui->setupUi(this);
    /*---------------------------------------
     *Conexiones
     *-------------------------------------*/
    connect(ui->btnCerrar,SIGNAL(clicked()),this,SLOT(close()));

}
FrmVerAnalitica::~FrmVerAnalitica()
{
    delete ui;
    delete oAnalitica1;
}

void FrmVerAnalitica::llenarTabla()
{

    // Cargar analitica
    ui->tablaAnalitica->clear();
    QStringList list;
    list <<tr("DESCRIPCIÓN")<<tr("VALOR") <<tr("REFERENCIA")<<tr("id");
    QSqlQuery *qAnalitica = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL = " select * from analitica2 where idanalitica =:idanalitica";
    qAnalitica->prepare(cSQL);
    qAnalitica->bindValue(":idanalitica",oAnalitica1->getId());
    ui->tablaAnalitica->setRowCount(0);
    ui->tablaAnalitica->setColumnCount(4);
    ui->tablaAnalitica->setColumnWidth(0,450);
    ui->tablaAnalitica->setColumnWidth(1,120);
    ui->tablaAnalitica->setColumnWidth(2,120);
    ui->tablaAnalitica->setColumnWidth(3,0);
    ui->tablaAnalitica->setHorizontalHeaderLabels(list);
    int pos = 0;
    QSqlRecord reg ;
    // Relleno la tabla
    if (qAnalitica->exec()) {
        while (qAnalitica->next()) {
            // Descripción
            reg = qAnalitica->record();
            ui->tablaAnalitica->setRowCount(pos+1);
            QTableWidgetItem *newItem = new QTableWidgetItem(reg.field("descripcion").value().toString());
            // para que los elementos no sean editables
            newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
            newItem->setTextColor(Qt::blue); // color de los Medicamentos activos
            ui->tablaAnalitica->setItem(pos,0,newItem);

            // Valor
            QTableWidgetItem *newItem1 = new QTableWidgetItem(reg.field("valor").value().toString());
            newItem1->setTextColor(Qt::blue); // color de los items
            ui->tablaAnalitica->setItem(pos,1,newItem1);


            // Valor referencia
            QTableWidgetItem *newItem2 = new QTableWidgetItem(reg.field("referencia").value().toString());

            newItem2->setTextColor(Qt::blue); // color de los items
            ui->tablaAnalitica->setItem(pos,2,newItem2);


            // Id
            QTableWidgetItem *newItem3 = new QTableWidgetItem(QString::number(reg.field("id").value().toInt()));
            // para que los elementos no sean editables
            newItem3->setFlags(newItem3->flags() & (~Qt::ItemIsEditable));
            newItem3->setTextColor(Qt::blue); // color de los items
            ui->tablaAnalitica->setItem(pos,3,newItem3);

            pos++;

        }
    }
}

void FrmVerAnalitica::capturaId(int nID)
{
 oAnalitica1->setId(nID);
 cargarDatos();
llenarTabla();
}

void FrmVerAnalitica::capturaPaciente(QString Paciente)
{
    ui->txtPaciente->setText(Paciente);
}

void FrmVerAnalitica::cargarDatos()
{
    oAnalitica1->recuperarDatos(oAnalitica1->getId());
    ui->txtAnalitica->setText(oAnalitica1->getAnalisis());
    ui->txtFechaAnalitica->setDateTime(oAnalitica1->getFechaAnalisis());
    ui->txtComentarios->setText(oAnalitica1->getComentarios());

}
