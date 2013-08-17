#include "frmaddtipocliente.h"
#include "ui_frmaddtipocliente.h"
#include "../Auxiliares/Globlal_Include.h"
#include  "QtSql"
#include "QSqlDatabase"
#include <QMessageBox>

FrmAddTipoCliente::FrmAddTipoCliente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAddTipoCliente)
{
    ui->setupUi(this);
    //----------------------
    // CARGAR LISTA/TABLA
    //----------------------
    QSqlQueryModel *m_familia = new QSqlQueryModel(this);
    m_familia->setQuery("select descripcion, id from maestro_familia_cliente",Configuracion_global->groupDB);
    ui->list_Familia->setModel(m_familia);

    //-----------------------
    // Conexiones
    //-----------------------
    connect(ui->list_Familia,SIGNAL(clicked(QModelIndex)),this,SLOT(LLenarTablaSubfamilias(QModelIndex)));
}

FrmAddTipoCliente::~FrmAddTipoCliente()
{
    delete ui;
}

void FrmAddTipoCliente::LLenarTablaSubfamilias(QModelIndex index)
{

    QSqlQueryModel* modelo = (QSqlQueryModel*)ui->list_Familia->model();
    int nid = modelo->record(index.row()).value("id").toInt();
    familiaRetorno = modelo->record(index.row()).value("descripcion").toString();
    QSqlQuery qFamilia(Configuracion_global->groupDB);
    qFamilia.prepare(QString("select * from maestro_subfamilia_cliente where id_maestro_familia_cliente = %1 ").arg(nid));
    if (qFamilia.exec()) {
        // Cargo datos en tabla
        QStringList cabecera;
        cabecera  <<tr("A")<< tr("Subfamilia");
        QSqlRecord reg;
        QString descripcion;
        QString cValores;

        ui->table_Subfamilia->setColumnCount(2);
        ui->table_Subfamilia->setRowCount(0);
        ui->table_Subfamilia->setHorizontalHeaderLabels(cabecera);
        ui->table_Subfamilia->setColumnWidth(0,0);
        ui->table_Subfamilia->setColumnWidth(1,180);

        int pos = 0;
        while (qFamilia.next()) {
            // Datos subfamilia
            reg = qFamilia.record();
            ui->table_Subfamilia->setRowCount(pos+1);
            QTableWidgetItem *newItem = new QTableWidgetItem();
            newItem->setCheckState(Qt::Checked);
            newItem->setTextColor(Qt::black); // color texto
            ui->table_Subfamilia->setItem(pos,0,newItem);

            // Sub-familia
            descripcion = reg.field("descripcion").value().toString();
            QTableWidgetItem *newItem1 = new QTableWidgetItem(descripcion);
            // para que los elementos no sean editables
            newItem1->setFlags(newItem1->flags() & (~Qt::ItemIsEditable));
            newItem1->setTextColor(Qt::black); // color de los items
            ui->table_Subfamilia->setItem(pos,1,newItem1);
            pos++;
        }
    } else {
        qDebug() <<"omg an error:"<< qFamilia.lastError() << "||" << qFamilia.executedQuery();

    }
}

void FrmAddTipoCliente::on_btnAceptar_clicked()
{
    QAbstractItemModel* model = ui->table_Subfamilia->model() ;
    QString valorSubfamilia =  model->index( ui->table_Subfamilia->currentRow(), 1 ).data( Qt::DisplayRole ).toString();
    if(ui->table_Subfamilia->currentRow()>-1){
        familiaRetorno += " - "+ valorSubfamilia;

    }
    accept();
}
