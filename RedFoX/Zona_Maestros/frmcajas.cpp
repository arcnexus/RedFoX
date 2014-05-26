#include "frmcajas.h"
#include "ui_frmcajas.h"


#include "../Busquedas/db_consulta_view.h"
FrmCajas::FrmCajas(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmCajas),
    menuButton(QIcon(":/Iconos/PNG/avisos.png"),tr("Cajas"),this)

{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    anadiendo = false;
    oCajas = new cajas;



    //SEARCH BAR
    setUpBusqueda();
    bloquearCampos(true);
    llenarTabla();
}

FrmCajas::~FrmCajas()
{
    delete ui;


}

void FrmCajas::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);

    QStringList orden;
    orden << tr("Descripción") <<tr("Código");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(m_busqueda,SIGNAL(hideMe()),this,SLOT(ocultarBusqueda()));
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));

    QPushButton * addproduct = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir"),this);
    connect(addproduct,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(addproduct);

    QPushButton * editproduct = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar"),this);
    connect(editproduct,SIGNAL(clicked()),this,SLOT(on_btnEditar_clicked()));
    m_busqueda->addWidget(editproduct);

    QPushButton * deleteproduct = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    connect(deleteproduct,SIGNAL(clicked()),this,SLOT(on_btnBorrar_clicked()));
    m_busqueda->addWidget(deleteproduct);
    m_busqueda->addSpacer();


    connect(m_busqueda,SIGNAL(key_Down_Pressed()),ui->tablaBusqueda,SLOT(setFocus()));
}
void FrmCajas::mostrarBusqueda()
{
    ui->stackedWidget->setCurrentIndex(0);
    _showBarraBusqueda(m_busqueda);
    m_busqueda->doFocustoText();
}

void FrmCajas::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}

void FrmCajas::filter_table(QString texto, QString orden, QString modo)
{
    if(ui->stackedWidget->currentIndex() ==1)
        ui->stackedWidget->setCurrentIndex(0);
    QHash <QString, QString> lista;
    lista[tr("Descripción")] = "desc_caja";
    lista[tr("Código")] = "id";
    QString order = lista.value(orden);
    qDebug() << order;
    if(modo == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    QString cSQL = "SELECT * FROM cajas WHERE "+order+" LIKE '%"+texto.trimmed()+"%' ORDER BY "+order+" "+modo;
    qDebug() << cSQL;
    model->setQuery(cSQL,Configuracion_global->empresaDB);

    ui->tablaBusqueda->selectRow(0);
}

void FrmCajas::on_btnAnadir_clicked()
{
    anadiendo = true;
    oCajas->vaciar();
    llenarCampos();
    bloquearCampos(false);
    ui->lineEditCaja->setFocus();
}

void FrmCajas::on_btnAnterior_clicked()
{
    QSqlQuery qry(Configuracion_global->empresaDB);
    if(qry.exec("SELECT id FROM cajas WHERE id < '"+ui->lineEditCaja->text()+"' ORDER BY id LIMIT 0,1")){
        qry.first();
        QString id = qry.value("id").toString();
        qDebug() << id;
        if(!oCajas->recuperar(id)){
            QMessageBox::warning(this,"Aviso","Se ha llegado al principio");
            return;

        }
        llenarCampos();
    }

}

void FrmCajas::on_btnSiguiente_clicked()
{
    QSqlQuery qry(Configuracion_global->empresaDB);
    if(qry.exec("SELECT id FROM cajas WHERE id > '"+ui->lineEditCaja->text()+"' ORDER BY id LIMIT 0,1")){
        qry.first();
        QString id = qry.value("id").toString();
        qDebug() << id;
        if(!oCajas->recuperar(id)){
            QMessageBox::warning(this,"Aviso","Se ha llegado al final");
            return;

        }
        llenarCampos();
    }

}

void FrmCajas::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    llenarTabla();
    mostrarBusqueda();
}

void FrmCajas::on_btnEditar_clicked()
{
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if (!ui->tablaBusqueda->currentIndex().isValid()) {
            return;
            on_tablaBusqueda_doubleClicked(ui->tablaBusqueda->currentIndex());
        }
    }
    ui->stackedWidget->setCurrentIndex(1);
    ui->lineEditDescripcion->setReadOnly(false);
    bloquearCampos(false);
    ui->lineEditCaja->setFocus();
}

void FrmCajas::on_btnGuardar_clicked()
{
    if(ui->lineEditCaja->text().isEmpty() || ui->lineEditDescripcion->text().isEmpty()){
        QMessageBox::information(this,"Error","El campo de código y descripción deben tener un valor",QMessageBox::Ok);
        return;
    }
    oCajas->id = ui->lineEditCaja->text();
    oCajas->descripcion = ui->lineEditDescripcion->text();
    oCajas->guardar(oCajas->id,oCajas->descripcion,anadiendo);
    anadiendo = false;
    bloquearCampos(true);

}

void FrmCajas::on_btnDeshacer_clicked()
{
    ui->lineEditCaja->setText(oCajas->id);
    ui->lineEditDescripcion->setText(oCajas->descripcion);
}

void FrmCajas::on_btnBorrar_clicked()
{
    QString id ="";
    if(ui->stackedWidget->currentIndex() == 0){

        QMessageBox mensaje(this);
        mensaje.setStyleSheet("background-color: rgb(221,221,221)");
        mensaje.setWindowTitle(tr("Borrar caja"));
        mensaje.setText(tr("¿Desea borrar la caja?\nEsta opción no se puede deshacer"));
        mensaje.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        mensaje.setButtonText(0,tr("No"));
        mensaje.setButtonText(1,tr("Sí"));

        mensaje.setIcon(QMessageBox::Question);

        if(mensaje.exec()==QMessageBox::Yes){
            if(!oCajas->borrar(id)) QMessageBox::warning(this,"Eror","Ha ocurrido un problema borrando la caja");
            }
        llenarTabla();
    }
    if(ui->stackedWidget->currentIndex() == 1){
        if(!ui->lineEditCaja->text().isEmpty()){
            id = ui->lineEditCaja->text();
            QMessageBox mensaje(this);
            mensaje.setStyleSheet("background-color: rgb(221,221,221)");
            mensaje.setWindowTitle(tr("Borrar caja"));
            mensaje.setText(tr("¿Desea borrar la caja?\nEsta opción no se puede deshacer"));
            mensaje.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
            mensaje.setButtonText(0,tr("No"));
            mensaje.setButtonText(1,tr("Sí"));

            mensaje.setIcon(QMessageBox::Question);

            if(mensaje.exec()==QMessageBox::Yes){
                if(!oCajas->borrar(id)) QMessageBox::warning(this,"Eror","Ha ocurrido un problema borrando la caja");
                }
        }
        ui->stackedWidget->setCurrentIndex(0);
        llenarTabla();
     }

}

void FrmCajas::on_tablaBusqueda_doubleClicked(const QModelIndex &index)
{
    QString id = ui->tablaBusqueda->model()->data(ui->tablaBusqueda->model()->index(index.row(),0),Qt::EditRole).toString();
    if(!oCajas->recuperar(id)){
        QMessageBox::warning(this,"Error","No se han podido recuperar los datos de la caja");
        return;

    }
    llenarCampos();
}

void FrmCajas::bloquearCampos(bool estado)
{

    ui->lineEditDescripcion->setReadOnly(estado);
    ui->lineEditCaja->setReadOnly(estado);
    ui->btnAnadir->setEnabled(estado);
    ui->btnAnterior->setEnabled(estado);
    ui->btnBorrar->setEnabled(estado);
    ui->btnBuscar->setEnabled(estado);
    ui->btnEditar->setEnabled(estado);
    ui->btnGuardar->setEnabled(!estado);
    ui->btnSiguiente->setEnabled(estado);
}

void FrmCajas::recuperarDatos(QString id)
{

}

void FrmCajas::llenarCampos()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->lineEditCaja->setText(oCajas->id);
    ui->lineEditDescripcion->setText(oCajas->descripcion);
}

void FrmCajas::llenarTabla()
{
    model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM cajas",Configuracion_global->empresaDB);
    ui->tablaBusqueda->setModel(model);
    model->setHeaderData(0,Qt::Horizontal,"ID");
    model->setHeaderData(1,Qt::Horizontal,"Descripción");
}

void FrmCajas::on_lineEditCaja_editingFinished()
{
    if (anadiendo) {
        QSqlQuery qry(Configuracion_global->empresaDB);
        qry.exec("SELECT * FROM cajas WHERE id ='"+ui->lineEditCaja->text()+"'");
        if (qry.numRowsAffected() > 0) {
            QMessageBox::warning(this,"Error","El código que está usando ya está en uso",QMessageBox::Ok);
            ui->lineEditCaja->setFocus();
        }
    }
}
