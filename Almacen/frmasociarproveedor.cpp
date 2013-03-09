#include "frmasociarproveedor.h"
#include "ui_frmasociarproveedor.h"


FrmAsociarProveedor::FrmAsociarProveedor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAsociarProveedor)
{
    ui->setupUi(this);
    modelProveedor = new QSqlQueryModel(this);
    modelProveedor->setQuery("select cProveedor from proveedores",QSqlDatabase::database("terra"));
    ui->tablaproveedores->setModel(modelProveedor);
    //------------------------------
    // Conexiones
    //------------------------------
    connect(ui->btnFiltrar,SIGNAL(clicked()),this,SLOT(filtrar_proveedor()));
    connect(ui->txtCodigo,SIGNAL(editingFinished()),this,SLOT(setcodigo()));
    connect(ui->txtDescoferta,SIGNAL(editingFinished()),this,SLOT(setDescOferta()));
    connect(ui->txtOferta,SIGNAL(editingFinished()),this,SLOT(setOferta()));
    connect(ui->txtPVD,SIGNAL(editingFinished()),this,SLOT(setpvd()));
    connect(ui->tablaproveedores,SIGNAL(clicked(QModelIndex)), this,SLOT(seleccionarPro(QModelIndex)));

}

FrmAsociarProveedor::~FrmAsociarProveedor()
{
    delete ui;
}

void FrmAsociarProveedor::filtrar_proveedor()
{
    modelProveedor = new QSqlQueryModel(this);
    modelProveedor->setQuery("select cProveedor from proveedores where cProveedor like '"+ui->txtBuscarProveedores->text().trimmed()+"%'",
                             QSqlDatabase::database("terra"));

    ui->tablaproveedores->setModel(modelProveedor);
}

void FrmAsociarProveedor::setcodigo()
{
    this->codigo = ui->txtCodigo->text();
}

void FrmAsociarProveedor::setpvd()
{
    ui->txtPVD->setText(configuracion.FormatoNumerico(ui->txtPVD->text()));
    this->pvd = ui->txtPVD->text().toDouble();
}

void FrmAsociarProveedor::setDescOferta()
{
    this->DescOferta = ui->txtDescoferta->text();
}

void FrmAsociarProveedor::setOferta()
{
    this->Oferta = ui->txtOferta->text();
}

void FrmAsociarProveedor::seleccionarPro(QModelIndex indice)
{
    QSqlQueryModel* modelo = (QSqlQueryModel*)ui->tablaproveedores->model();
    QString prov = modelo->record(indice.row()).value("cProveedor").toString();
    QSqlQuery qProv(QSqlDatabase::database("terra"));
    qProv.prepare("select id from proveedores where cProveedor ='"+prov+"'");
    if (qProv.exec()) {
        qProv.next();
        this->id_proveedor = qProv.record().value("id").toInt();

    } else{
        QMessageBox::warning(this,tr("Buscar Proveedor"),
                             tr("Falló la búsqueda de el proveedor : %1").arg(qProv.lastError().text()),tr("Aceptar"));
    }

}

