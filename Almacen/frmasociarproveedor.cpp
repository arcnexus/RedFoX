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
    // -----------------------------
    // Divisa
    //------------------------------
    QSqlQueryModel *modelDivisa = new QSqlQueryModel(this);
    modelDivisa->setQuery("Select moneda from monedas",QSqlDatabase::database("terra"));
    ui->cboDivisa->setModel(modelDivisa);
    //------------------------------
    // Conexiones
    //------------------------------
    connect(ui->btnFiltrar,SIGNAL(clicked()),this,SLOT(filtrar_proveedor()));
    connect(ui->txtCodigo,SIGNAL(editingFinished()),this,SLOT(setcodigo()));
    connect(ui->txtDescoferta,SIGNAL(editingFinished()),this,SLOT(setDescOferta()));
    connect(ui->txtPVD,SIGNAL(editingFinished()),Configuracion_global,SLOT(format_text()));
    connect(ui->txtPVDReal,SIGNAL(editingFinished()),Configuracion_global,SLOT(format_text()));
    connect(ui->txtPVD,SIGNAL(editingFinished()),this,SLOT(setpvd()));
    connect(ui->txtPVDReal,SIGNAL(editingFinished()),this,SLOT(setpvdReal()));
    connect(ui->tablaproveedores,SIGNAL(clicked(QModelIndex)), this,SLOT(seleccionarPro(QModelIndex)));
//    connect(ui->btnAnadir,SIGNAL(clicked()),this,SLOT(accept()));

}

FrmAsociarProveedor::~FrmAsociarProveedor()
{
    delete ui;
}

void FrmAsociarProveedor::setAnadir()
{
    ui->btnAnadir->setText(tr("Añadir"));

}

void FrmAsociarProveedor::seteditar(QString id)
{
    ui->btnAnadir->setText(tr("Guardar"));
    ui->txtBuscarProveedores->setVisible(false);
    ui->btnFiltrar->setVisible(false);
    QSqlQuery queryProvAlt(QSqlDatabase::database("terra"));
    queryProvAlt.prepare("select * from proveedores_frecuentes where id= "+id);
    if (queryProvAlt.exec()){
        if( queryProvAlt.next()){
            ui->txtCodigo->setText(queryProvAlt.record().value("codigo").toString());
            this->codigo = ui->txtCodigo->text();
            ui->txtPVD->setText(QString::number(queryProvAlt.record().value("pvd").toDouble(),'f',2));
            this->pvd = ui->txtPVD->text().toDouble();
            ui->txtPVDReal->setText(QString::number(queryProvAlt.record().value("pvdreal").toDouble(),'f',2));
            this->pvdreal = ui->txtPVDReal->text().toDouble();
            ui->txtDescoferta->setText(queryProvAlt.record().value("descoferta").toString());
            this->DescOferta = ui->txtDescoferta->text();
            int index = ui->cboDivisa->findText(queryProvAlt.record().value("moneda").toString());
            ui->cboDivisa->setCurrentIndex(index);
            modelProveedor->setQuery("select cProveedor from proveedores where id="+
                                     queryProvAlt.record().value("id_prov").toString(),QSqlDatabase::database("terra"));
            ui->tablaproveedores->setModel(modelProveedor);
            ui->tablaproveedores->selectRow(0);
        }
    }
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

void FrmAsociarProveedor::setpvdReal()
{
    this->pvdreal = ui->txtPVDReal->text().toDouble();
}

void FrmAsociarProveedor::setIdDivisa()
{
    this->id_divisa = Configuracion_global->Devolver_id_moneda(ui->cboDivisa->currentText());
}
void FrmAsociarProveedor::Aceptar()
{
    QString cMensajeError;
    this->id_divisa = Configuracion_global->Devolver_id_moneda(ui->cboDivisa->currentText());

    if(this->id_proveedor == 0)
        cMensajeError =tr("Falta seleccionar un proveedor\n");
    if(this->codigo.isEmpty())
        cMensajeError = cMensajeError +tr("Falta el código del artículo en el proveedor\n");
    if(ui->txtPVD->text().trimmed()=="0,00")
        cMensajeError = cMensajeError + tr("Falta el precio de coste\n");
    if(ui->txtPVDReal->text().trimmed() == "0,00")
        cMensajeError = cMensajeError + tr("Falta poner el precio de coste real\n");
    if(ui->cboDivisa->currentText().isEmpty())
        cMensajeError = cMensajeError + tr("Falta establecer Divisa tarifa proveedor");
    if (cMensajeError.isEmpty()) {
        QDialog::accept();
    } else {
        QMessageBox msgBox;
        msgBox.setStyleSheet("color: blue; background-color: yellow");
        msgBox.setText(cMensajeError);
        msgBox.setWindowTitle(tr("Añadir Proveedor frecuente"));
        msgBox.exec();
    }

}

