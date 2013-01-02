#include "frmanalitica.h"
#include "ui_frmanalitica.h"
#include <QSql>
#include<QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include "frmanalitica2.h"
#include <QMessageBox>
#include "analitica.h"
Analitica * oAnalitica = new Analitica();

FrmAnalitica::FrmAnalitica(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAnalitica)
{
    ui->setupUi(this);
    /*----------------------------------------
     * CONEXIONES
     *--------------------------------------*/
    connect(ui->btnCerrar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->btnAnadirCampos,SIGNAL(clicked()),this,SLOT(AnadirCamposAnalitica()));
    /* -----------------------------------
     * RELLENO COMBO
     *------------------------------------*/
    QSqlQueryModel *qTipos = new QSqlQueryModel(this);
    qTipos->setQuery("select tipoanalisis from tiposanalitica",QSqlDatabase::database("dbmedica"));
    ui->cboTipoAnalitica->setModel(qTipos);

    /*------------------------------------
     * LLenar Tabla con valores analítica
     *----------------------------------*/
    llenartabla();
}

FrmAnalitica::~FrmAnalitica()
{
    delete ui;
}

void FrmAnalitica::llenartabla()
{
    // Cargar farmacología episodio
    QStringList list;
    list <<tr("DESCRIPCIÓN")<<tr("VALOR") <<tr("REFERENCIA")<<tr("id");
    QSqlQuery *qAnalitica = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL = " select * from analitica2 where idanalitica =:idanalitica";
    qAnalitica->prepare(cSQL);
    qAnalitica->bindValue(":idanalitica",oAnalitica->getId());
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
                newItem->setTextColor(Qt::blue); // color de los Medicamentos activos
            ui->tablaAnalitica->setItem(pos,0,newItem);

            // Valor
            QTableWidgetItem *newItem1 = new QTableWidgetItem(reg.field("valor").value().toString());
            // para que los elementos no sean editables
            newItem1->setFlags(newItem1->flags() & (~Qt::ItemIsEditable));
            newItem1->setTextColor(Qt::blue); // color de los items
            ui->tablaAnalitica->setItem(pos,1,newItem1);


            // Valor referencia
            QTableWidgetItem *newItem2 = new QTableWidgetItem(reg.field("referencia").value().toString());
            // para que los elementos no sean editables
            newItem2->setFlags(newItem2->flags() & (~Qt::ItemIsEditable));
            newItem2->setTextColor(Qt::blue); // color de los items
            ui->tablaAnalitica->setItem(pos,2,newItem2);


            // Id
            QTableWidgetItem *newItem3 = new QTableWidgetItem(QString::number(reg.field("id").value().toInt()));
            // para que los elementos no sean editables
            newItem3->setFlags(newItem3->flags() & (~Qt::ItemIsEditable));
            newItem3->setTextColor(Qt::blue); // color de los items
            ui->tablaAnalitica->setItem(pos,2,newItem3);

            pos++;

        }
    }
}

void FrmAnalitica::AnadirCamposAnalitica()
{
    Frmanalitica2 *frmanalitica2 = new Frmanalitica2(this);
    connect(this,SIGNAL(pasartipo(QString,int)),frmanalitica2,SLOT(cargarDatos(QString,int)));
    emit pasartipo(ui->cboTipoAnalitica->currentText(),oAnalitica->getId());
    llenartabla();

}

void FrmAnalitica::capturaID(int idAna)
{
    oAnalitica->setId(idAna);
}
