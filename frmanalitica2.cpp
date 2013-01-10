#include "frmanalitica2.h"
#include "ui_frmanalitica2.h"
#include <QtSql>
#include <QSqlQuery>
#include <QSqlRecord>
#include "analitica.h"

Frmanalitica2::Frmanalitica2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Frmanalitica2)
{
    ui->setupUi(this);
    connect(ui->btnAnadir,SIGNAL(clicked()),this,SLOT(aceptar()));
    connect(ui->btnCancelar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->btnMarcar,SIGNAL(clicked()),this,SLOT(marcartodos()));
    connect(ui->btnDesmarcar,SIGNAL(clicked()),this,SLOT(desmarcartodos()));
}

Frmanalitica2::~Frmanalitica2()
{
    delete ui;
}

void Frmanalitica2::cargarDatos(QString cTipo, int idAnalitica)
{
    this->cTipo = cTipo;
    this->idAnalitica = idAnalitica;

    QSqlQuery *qDatos = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qDatos->prepare("Select * from tiposanalitica2 where tipo = :cTipo");
    qDatos->bindValue(":cTipo",cTipo);
    QStringList cabecera;
    cabecera  <<tr("A")<< tr("Analítica")  << tr("Valor Real") <<tr("Valor Referencia") <<tr("Comentarios");
    QSqlRecord reg;
    QString cDescripcion;
    QString cValores;

    ui->tabla->setColumnCount(5);
    ui->tabla->setRowCount(0);
    ui->tabla->setHorizontalHeaderLabels(cabecera);
    ui->tabla->setColumnWidth(0,20);
    ui->tabla->setColumnWidth(1,180);
    ui->tabla->setColumnWidth(2,110);
    ui->tabla->setColumnWidth(3,110);
    ui->tabla->setColumnWidth(4,120);


    int pos = 0;

    if (qDatos->exec()) {
        while (qDatos->next()) {
            // Datos Analítica
            reg = qDatos->record();
            ui->tabla->setRowCount(pos+1);
            QTableWidgetItem *newItem = new QTableWidgetItem();
            newItem->setCheckState(Qt::Checked);
            newItem->setTextColor(Qt::black); // color texto
            ui->tabla->setItem(pos,0,newItem);

            // Descripcion
            cDescripcion = reg.field("descripcion").value().toString();
            QTableWidgetItem *newItem1 = new QTableWidgetItem(cDescripcion);
            // para que los elementos no sean editables
            newItem1->setFlags(!Qt::ItemIsEnabled);
            newItem1->setTextColor(Qt::black); // color de los items
            ui->tabla->setItem(pos,1,newItem1);

            // valores
            QTableWidgetItem *newItem2 = new QTableWidgetItem();
            newItem2->setTextColor(Qt::black); // color de los items
            ui->tabla->setItem(pos,2,newItem2);

            // valoresReferencia
            cValores = reg.field("valoresreferencia").value().toString();
            QTableWidgetItem *newItem3 = new QTableWidgetItem(cValores);
            newItem3->setTextColor(Qt::black); // color de los items
            ui->tabla->setItem(pos,3,newItem3);
            // comentarios
            cValores = reg.field("comentarios").value().toString();
            QTableWidgetItem *newItem4 = new QTableWidgetItem(cValores);
            newItem4->setTextColor(Qt::black); // color de los items
            ui->tabla->setItem(pos,4,newItem4);
            pos++;
        }
    }
}

void Frmanalitica2::aceptar()
{
    Analitica *oAnalitica = new Analitica();
    int lin = 0;
    int lineas = ui->tabla->rowCount();
    bool status;
    for(lin= 0; lin < lineas ; lin++) {
        status = ui->tabla->item(lin,0)->checkState();
        if(status==true){
            QString cAnalitica = ui->tabla->item(lin,1)->text();
            QString cValor = ui->tabla->item(lin,2)->text();
            QString cRefer = ui->tabla->item(lin,3)->text();
            QString cComent = ui->tabla->item(lin,4)->text();

            oAnalitica->AnadirLineas(this->idAnalitica,cAnalitica,cValor,cRefer,cComent,this->cTipo);

        }
    }
    this->close();

}

void Frmanalitica2::marcartodos()
{
    int lin = 0;
    int lineas = ui->tabla->rowCount();
    for(lin= 0; lin < lineas ; lin++) {
        ui->tabla->item(lin,0)->setCheckState(Qt::Checked);
    }
}

void Frmanalitica2::desmarcartodos()
{
    int lin = 0;
    int lineas = ui->tabla->rowCount();
    for(lin= 0; lin < lineas ; lin++) {
        ui->tabla->item(lin,0)->setCheckState(Qt::Unchecked);
    }
}




