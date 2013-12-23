#include "frmasociarproveedor.h"
#include "ui_frmasociarproveedor.h"


FrmAsociarProveedor::FrmAsociarProveedor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmAsociarProveedor)
{
    ui->setupUi(this);
    modelProveedor = new QSqlQueryModel(this);
    modelProveedor->setQuery("select proveedor from proveedores",Configuracion_global->groupDB);
    ui->tablaproveedores->setModel(modelProveedor);
    // -----------------------------
    // Divisa
    //------------------------------
    QSqlQueryModel *modelDivisa = new QSqlQueryModel(this);
    modelDivisa->setQuery("Select moneda from monedas",Configuracion_global->groupDB);
    ui->cboDivisa->setModel(modelDivisa);
    //------------------------------
    // Conexiones
    //------------------------------
    connect(ui->btnFiltrar,SIGNAL(clicked()),this,SLOT(filtrar_proveedor()));
    connect(ui->txtCodigo,SIGNAL(editingFinished()),this,SLOT(setcodigo()));
    connect(ui->txtDescoferta,SIGNAL(editingFinished()),this,SLOT(setDescOferta()));
    connect(ui->txtPVD,SIGNAL(editingFinished()),Configuracion_global,SLOT(format_text()));
    connect(ui->txtpvd_real,SIGNAL(editingFinished()),Configuracion_global,SLOT(format_text()));
    connect(ui->txtPVD,SIGNAL(editingFinished()),this,SLOT(setpvd()));
    connect(ui->txtpvd_real,SIGNAL(editingFinished()),this,SLOT(setpvd_real()));
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
    QSqlQuery queryProvAlt(Configuracion_global->groupDB);
    queryProvAlt.prepare("select * from proveedores_frecuentes where id= "+id);
    if (queryProvAlt.exec()){
        if( queryProvAlt.next()){
            ui->txtCodigo->setText(queryProvAlt.record().value("codigo").toString());
            this->codigo = ui->txtCodigo->text();
            ui->txtPVD->setText(QString::number(queryProvAlt.record().value("pvd").toDouble(),'f',Configuracion_global->decimales));
            this->pvd = ui->txtPVD->text().toDouble();
            ui->txtpvd_real->setText(QString::number(queryProvAlt.record().value("pvd_real").toDouble(),'f',Configuracion_global->decimales));
            this->pvd_real = ui->txtpvd_real->text().toDouble();
            ui->txtDescoferta->setText(queryProvAlt.record().value("descoferta").toString());
            this->DescOferta = ui->txtDescoferta->text();
            int index = ui->cboDivisa->findText(queryProvAlt.record().value("moneda").toString());
            ui->cboDivisa->setCurrentIndex(index);
            modelProveedor->setQuery("select proveedor from proveedores where id="+
                                     queryProvAlt.record().value("id_prov").toString(),Configuracion_global->groupDB);
            ui->tablaproveedores->setModel(modelProveedor);
            ui->tablaproveedores->selectRow(0);
        }
    }
}

void FrmAsociarProveedor::filtrar_proveedor()
{
    modelProveedor = new QSqlQueryModel(this);
    modelProveedor->setQuery("select proveedor from proveedores where proveedor like '"+ui->txtBuscarProveedores->text().trimmed()+"%'",
                             Configuracion_global->groupDB);

    ui->tablaproveedores->setModel(modelProveedor);
}

void FrmAsociarProveedor::setcodigo()
{
    this->codigo = ui->txtCodigo->text();
}

void FrmAsociarProveedor::setpvd()
{
    ui->txtPVD->setText(configuracion.toFormatoMoneda(ui->txtPVD->text()));
    this->pvd = Configuracion_global->MonedatoDouble(ui->txtPVD->text());
}

void FrmAsociarProveedor::setDescOferta()
{
    this->DescOferta = ui->txtDescoferta->text();
}


void FrmAsociarProveedor::seleccionarPro(QModelIndex indice)
{
    QSqlQueryModel* modelo = (QSqlQueryModel*)ui->tablaproveedores->model();
    QString prov = modelo->record(indice.row()).value("proveedor").toString();
    QSqlQuery qProv(Configuracion_global->groupDB);
    qProv.prepare("select id from proveedores where proveedor ='"+prov+"'");
    if (qProv.exec()) {
        qProv.next();
        this->id_proveedor = qProv.record().value("id").toInt();

    } else{
        QMessageBox::warning(this,tr("Buscar Proveedor"),
                             tr("Falló la búsqueda de el proveedor : %1").arg(qProv.lastError().text()),tr("Aceptar"));
    }

}

void FrmAsociarProveedor::setpvd_real()
{
    this->pvd_real = Configuracion_global->MonedatoDouble(ui->txtpvd_real->text());
}

void FrmAsociarProveedor::setidDivisa()
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
    if(ui->txtpvd_real->text().trimmed() == "0,00")
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



