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
#include <QDate>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

Analitica * oAnalitica = new Analitica();

FrmAnalitica::FrmAnalitica(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAnalitica)
{
    ui->setupUi(this);
    /*-----------------------------------
     * CONEXIONES
     *---------------------------------*/
    connect(ui->btnCerrar,SIGNAL(clicked()),this,SLOT(GuardarYCerrar()));
    connect(ui->btnAnadirCampos,SIGNAL(clicked()),this,SLOT(AnadirCamposAnalitica()));
    connect(ui->txtAnalitica,SIGNAL(editingFinished()),this,SLOT(AsignarAnalitica()));
    connect(ui->txtFechaAnalitica,SIGNAL(editingFinished()),this,SLOT(AsignarFecha()));

    /* ----------------------------------
     * RELLENO COMBO
     *---------------------------------*/
    QSqlQueryModel *qTipos = new QSqlQueryModel(this);
    qTipos->setQuery("select tipoanalisis from tiposanalitica",QSqlDatabase::database("dbmedica"));
    ui->cboTipoAnalitica->setModel(qTipos);


    /*-----------------------------------
     * VALORES CONTROLES
     *---------------------------------*/
    ui->txtFechaAnalitica->setDate(QDate::currentDate());
    oAnalitica->setFechaAnalisis(QDate::currentDate());

    /*----------------------------------
     *LLenar Tabla
     *--------------------------------*/
    llenartabla();
}

FrmAnalitica::~FrmAnalitica()
{
    delete ui;
}

void FrmAnalitica::llenartabla()
{
    // Cargar analitica
    ui->tablaAnalitica->clear();
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

void FrmAnalitica::AnadirCamposAnalitica()
{
    Frmanalitica2 *frmanalitica2 = new Frmanalitica2(this);
    connect(this,SIGNAL(pasartipo(QString,int)),frmanalitica2,SLOT(cargarDatos(QString,int)));
    emit pasartipo(ui->cboTipoAnalitica->currentText(),oAnalitica->getId());

    frmanalitica2->exec();
    llenartabla();

}

void FrmAnalitica::capturaID(int idAna)
{
    oAnalitica->setId(idAna);
}

void FrmAnalitica::capturaPaciente(QString Paciente)
{
    ui->txtNombrePaciente->setText(Paciente);
}

void FrmAnalitica::AsignarAnalitica()
{
    oAnalitica->setAnalisis(ui->txtAnalitica->text());
}

void FrmAnalitica::AsignarFecha()
{
    oAnalitica->setFechaAnalisis(ui->txtFechaAnalitica->date());
}

void FrmAnalitica::GuardarYCerrar()
{
    QSqlQuery *qAna = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    qAna->prepare("UPDATE Analitica SET "
                  "analisis = :analisis,"
                  "fechaanalisis = :fechaanalisis,"
                  "tipoanalisis = :tipoanalisis "
                  "WHERE id =:id");
    qAna->bindValue(":analisis",oAnalitica->getAnalisis());
    qAna->bindValue(":fechaanalisis",oAnalitica->getFechaAnalisis());
    qAna->bindValue(":id",oAnalitica->getId());

    if(!qAna->exec()){
        QMessageBox::warning(this,tr("Analítica"),tr("No se pudo actualizar la cabecera de analítica:")+
                             qAna->lastError().text(), tr("Aceptar"));
        qDebug() << qAna->lastQuery();
    }
    delete qAna;
    close();
}
