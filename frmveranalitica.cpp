#include "frmveranalitica.h"
#include "ui_frmveranalitica.h"
#include <QtSql>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "analitica.h"
#include "frmanalitica.h"
#include <QDate>

    FrmVerAnalitica::FrmVerAnalitica(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmVerAnalitica)
{
    ui->setupUi(this);
    /*---------------------------------------
     *Conexiones
     *-------------------------------------*/
    connect(ui->btnCerrar,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->btnEditar,SIGNAL(clicked()), this,SLOT(editarDatos()));
    connect(ui->btnGuardar,SIGNAL(clicked()),this,SLOT(guardarDatos()));
    connect(ui->btnDeshacer,SIGNAL(clicked()),this,SLOT(Deshacer()));
    connect(ui->btnaddlinea,SIGNAL(clicked()),this,SLOT(addLineas()));

}
FrmVerAnalitica::~FrmVerAnalitica()
{
    delete ui;
}

void FrmVerAnalitica::llenarTabla(int nID)
{

    //QTableWidget *tablaAnalitica = new QTableWidget(this);
    connect(ui->tablaAnalitica,SIGNAL(itemChanged(QTableWidgetItem*)),this, SLOT(guardarDatosItems(QTableWidgetItem*)));
    // Cargar analitica
    //ui->tablaAnalitica->clear();
    QStringList list;
    list <<tr("DESCRIPCIÓN")<<tr("VALOR") <<tr("REFERENCIA") <<tr("COMENTARIOS") <<tr("id");
    QSqlQuery *qAnalitica = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL = " select * from analitica2 where idanalitica =:idanalitica";
    qAnalitica->prepare(cSQL);
    qAnalitica->bindValue(":idanalitica",nID);
    ui->tablaAnalitica->setRowCount(0);
    ui->tablaAnalitica->setColumnCount(5);
    ui->tablaAnalitica->setColumnWidth(0,150);
    ui->tablaAnalitica->setColumnWidth(1,90);
    ui->tablaAnalitica->setColumnWidth(2,90);
    ui->tablaAnalitica->setColumnWidth(3,120);
    ui->tablaAnalitica->setColumnWidth(4,0);
    ui->tablaAnalitica->setHorizontalHeaderLabels(list);
    int pos = 0;
    QSqlRecord reg ;
    // Relleno la tabla
    ui->tablaAnalitica->blockSignals(true);
    if (qAnalitica->exec()) {
        while (qAnalitica->next()) {
            // Descripción
            reg = qAnalitica->record();
            ui->tablaAnalitica->setRowCount(pos+1);
            //QTableWidgetItem *newItem = new QTableWidgetItem(reg.field("descripcion").value().toString());
            // para que los elementos no sean editables
            //newItem->setFlags(newItem->flags() & (~Qt::ItemIsEditable));
            //newItem->setTextColor(Qt::blue); // color de los Medicamentos activos

            //ui->tablaAnalitica->setItem(pos,0,newItem);
            ui->tablaAnalitica->setItem(pos,0,new QTableWidgetItem (reg.field("descripcion").value().toString()));

            // Valor
            QTableWidgetItem *newItem1 = new QTableWidgetItem(reg.field("valor").value().toString());
            newItem1->setTextColor(Qt::blue); // color de los items
            ui->tablaAnalitica->removeCellWidget(pos,1);
            ui->tablaAnalitica->setItem(pos,1,newItem1);


            // Valor referencia
            QTableWidgetItem *newItem2 = new QTableWidgetItem(reg.field("referencia").value().toString());

            newItem2->setTextColor(Qt::blue); // color de los items
            ui->tablaAnalitica->removeCellWidget(pos,2);
            ui->tablaAnalitica->setItem(pos,2,newItem2);


            // Comentarios
            QTableWidgetItem *newItem3 = new QTableWidgetItem(reg.field("comentarios").value().toString());

            newItem3->setTextColor(Qt::blue); // color de los items
            ui->tablaAnalitica->removeCellWidget(pos,3);
            ui->tablaAnalitica->setItem(pos,3,newItem3);


            // Id
            QTableWidgetItem *newItem4 = new QTableWidgetItem(QString::number(reg.field("id").value().toInt()));
            // para que los elementos no sean editables
            newItem4->setFlags(newItem4->flags() & (~Qt::ItemIsEditable));
            newItem4->setTextColor(Qt::blue); // color de los items
            ui->tablaAnalitica->removeCellWidget(pos,4);
            ui->tablaAnalitica->setItem(pos,4,newItem4);

            pos++;

        }
        nEdited = 1;
    }
    ui->tablaAnalitica->blockSignals(false);
}

void FrmVerAnalitica::capturaId(int nId)
{
nID = nId;
 cargarDatos(nID);
llenarTabla(nID);
}

void FrmVerAnalitica::capturaPaciente(QString Paciente)
{
    ui->txtPaciente->setText(Paciente);
}

void FrmVerAnalitica::cargarDatos(int nID)
{
    Analitica oAna;
    oAna.recuperarDatos(nID);
    ui->txtAnalitica->setText(oAna.getAnalisis());
    ui->txtFechaAnalitica->setDate(oAna.getFechaAnalisis());
    ui->txtComentarios->setText(oAna.getComentarios());

}

void FrmVerAnalitica::cargarDatos()
{
    Analitica oAna;
    oAna.recuperarDatos(nID);
    ui->txtAnalitica->setText(oAna.getAnalisis());
    ui->txtFechaAnalitica->setDate(oAna.getFechaAnalisis());
    ui->txtComentarios->setText(oAna.getComentarios());

}

void FrmVerAnalitica::editarDatos()
{
    ui->txtAnalitica->setReadOnly(false);
    ui->txtFechaAnalitica->setReadOnly(false);
    ui->txtComentarios->setReadOnly(false);
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnDeshacer->setEnabled(true);
    ui->btnCerrar->setEnabled(false);
    ui->txtAnalitica->setFocus();

}

void FrmVerAnalitica::guardarDatos()
{
    ui->txtAnalitica->setReadOnly(true);
    ui->txtFechaAnalitica->setReadOnly(true);
    ui->txtComentarios->setReadOnly(true);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnDeshacer->setEnabled(false);
    ui->btnCerrar->setEnabled(true);
    Analitica oAnalit;
    oAnalit.setId(nID);
    oAnalit.setAnalisis(ui->txtAnalitica->text());
    oAnalit.setFechaAnalisis(ui->txtFechaAnalitica->date());
    oAnalit.setComentarios(ui->txtComentarios->toPlainText());
    oAnalit.GuardarDatos(nID);

}

void FrmVerAnalitica::guardarDatosItems(QTableWidgetItem* tItem)
{
   int fila = tItem->row();
   if (nEdited == 1) {
       int id = ui->tablaAnalitica->item(fila,4)->text().toInt();
       QString valor = ui->tablaAnalitica->item(fila,1)->text();
       QString refer = ui->tablaAnalitica->item(fila,2)->text();
       QString coment = ui->tablaAnalitica->item(fila,3)->text();

       Analitica oAna;
       oAna.GuardarLineas(id,valor,refer,coment);
   }
}

void FrmVerAnalitica::Deshacer()
{
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnDeshacer->setEnabled(true);
    ui->btnCerrar->setEnabled(false);
    cargarDatos();
}

void FrmVerAnalitica::addLineas()
{
   FrmAnalitica frmAnalitica;
   connect(this,SIGNAL(pasarID(int)),&frmAnalitica,SLOT(capturaID(int)));
   connect(this,SIGNAL(pasarPaciente(QString)),&frmAnalitica,SLOT(capturaPaciente(QString)));
   connect(this,SIGNAL(pasarFecha(QDate)),&frmAnalitica,SLOT(AsignarFecha(QDate)));
   connect(this,SIGNAL(pasarAnalitica(QString)),&frmAnalitica,SLOT(AsignarAnalitica(QString)));
    emit pasarID(nID);
    emit pasarPaciente(ui->txtPaciente->text());
   emit pasarFecha(ui->txtFechaAnalitica->date());
   emit pasarAnalitica(ui->txtAnalitica->text());
    frmAnalitica.exec();
 llenarTabla(nID);
}

