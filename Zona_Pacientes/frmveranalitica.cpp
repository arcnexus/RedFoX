#include "frmveranalitica.h"
#include "ui_frmveranalitica.h"
#include "analitica.h"
#include "frmanalitica.h"


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
    connect(ui->btnDeleteLinea,SIGNAL(clicked()), this,SLOT(BorrarLineas()));



}
FrmVerAnalitica::~FrmVerAnalitica()
{
    delete ui;
}

void FrmVerAnalitica::llenarTabla(int nid)
{

    //QTableWidget *tablaAnalitica = new QTableWidget(this);
    connect(ui->tablaAnalitica,SIGNAL(itemChanged(QTableWidgetItem*)),this, SLOT(guardarDatosItems(QTableWidgetItem*)));
    // Cargar analitica
    //ui->tablaAnalitica->clear();
    QStringList list;
    list <<tr("DESCRIPCIÓN")<<tr("VALOR") <<tr("REFERENCIA") <<tr("COMENTARIOS") <<tr("id");
    QScopedPointer<QSqlQuery>qAnalitica (new QSqlQuery(QSqlDatabase::database("dbmedica")));
    //QSqlQuery *qAnalitica = new QSqlQuery(QSqlDatabase::database("dbmedica"));
    QString cSQL = " select * from analitica2 where id_analitica =:id_analitica";
    qAnalitica->prepare(cSQL);
    qAnalitica->bindValue(":id_analitica",nid);
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


            // id
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

void FrmVerAnalitica::capturaid(int nid)
{
    nid = nid;
    cargarDatos(nid);
    llenarTabla(nid);
}

void FrmVerAnalitica::capturaPaciente(QString Paciente)
{
    ui->txtPaciente->setText(Paciente);
}

void FrmVerAnalitica::cargarDatos(int nid)
{
    Analitica oAna;
    oAna.recuperarDatos(nid);
    ui->txtAnalitica->setText(oAna.getAnalisis());
    ui->txtFechaAnalitica->setDate(oAna.getfecha_analisis());
    ui->txcomentarios->setText(oAna.gecomentarios());

}

void FrmVerAnalitica::cargarDatos()
{
    Analitica oAna;
    oAna.recuperarDatos(nid);
    ui->txtAnalitica->setText(oAna.getAnalisis());
    ui->txtFechaAnalitica->setDate(oAna.getfecha_analisis());
    ui->txcomentarios->setText(oAna.gecomentarios());

}

void FrmVerAnalitica::editarDatos()
{
    ui->txtAnalitica->setReadOnly(false);
    ui->txtFechaAnalitica->setReadOnly(false);
    ui->txcomentarios->setReadOnly(false);
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
    ui->txcomentarios->setReadOnly(true);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnDeshacer->setEnabled(false);
    ui->btnCerrar->setEnabled(true);
    Analitica oAnalit;
    oAnalit.setid(nid);
    oAnalit.setAnalisis(ui->txtAnalitica->text());
    oAnalit.setfecha_analisis(ui->txtFechaAnalitica->date());
    oAnalit.secomentarios(ui->txcomentarios->toPlainText());
    oAnalit.GuardarDatos(nid);

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
    connect(this,SIGNAL(pasarid(int)),&frmAnalitica,SLOT(capturaid(int)));
    connect(this,SIGNAL(pasarPaciente(QString)),&frmAnalitica,SLOT(capturaPaciente(QString)));
    connect(this,SIGNAL(pasarFecha(QDate)),&frmAnalitica,SLOT(AsignarFecha(QDate)));
    connect(this,SIGNAL(pasarAnalitica(QString)),&frmAnalitica,SLOT(AsignarAnalitica(QString)));
    emit pasarid(nid);
    emit pasarPaciente(ui->txtPaciente->text());
    emit pasarFecha(ui->txtFechaAnalitica->date());
    emit pasarAnalitica(ui->txtAnalitica->text());
    frmAnalitica.exec();
    llenarTabla(nid);
}

void FrmVerAnalitica::BorrarLineas()
{
    if(!ui->tablaAnalitica->selectedItems().isEmpty())
    {
        Analitica oAnalitica;
        oAnalitica.EliminarLinea(ui->tablaAnalitica->item(ui->tablaAnalitica->currentRow(),4)->text().toInt());
        llenarTabla(nid);
    }
}

