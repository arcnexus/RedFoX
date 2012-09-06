#include "frmarticulos.h"
#include "ui_frmarticulos.h"
#include "configuracion.h"
#include "articulo.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlRecord>
#include <QString>


Articulo *oArticulo = new Articulo();
FrmArticulos::FrmArticulos(Configuracion *o_config,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmArticulos)
{
    ui->setupUi(this);
    o_config->CargarDatos();
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA1));
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA2));
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA3));
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA4));
}

FrmArticulos::~FrmArticulos()
{
    delete ui;
}

void FrmArticulos::on_botAnadir_clicked()
{
    desbloquearCampos();

   VaciarCampos();
   oArticulo->Anadir();
   LLenarCampos();
    ui->txtcCodigo->setFocus();


}

void FrmArticulos::on_botGuardar_clicked()
{
    bloquearCampos();
    oArticulo->Guardar();
}

void FrmArticulos::on_botSiguiente_clicked()
{
    QString cId = QString::number(oArticulo->getId());
    oArticulo->Recuperar("Select * from articulos where id >"+cId+" order by id limit 0,1 ");
    LLenarCampos();

}
void FrmArticulos::on_botAnterior_clicked()
{
    QString cId = QString::number(oArticulo->getId());
    oArticulo->Recuperar("Select * from articulos where id <"+cId+" order by id desc limit 0,1 ");
    LLenarCampos();
}

void FrmArticulos::bloquearCampos() {

    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
//    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
//    QSpinBox *SpinBox;
//    foreach (SpinBox, SpinBoxList) {
//        SpinBox->setReadOnly(true);
//        //qDebug() << lineEdit->objectName();
//   }
    // DoubleSpinBox
//    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
//    QDoubleSpinBox *DSpinBox;
//    foreach (DSpinBox, DSpinBoxList) {
//        DSpinBox->setReadOnly(true);
//        //qDebug() << lineEdit->objectName();
//    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }

    ui->botAnadir->setEnabled(true);
    ui->botAnterior->setEnabled(true);
    ui->botBorrar->setEnabled(true);
    ui->botBuscar->setEnabled(true);
    ui->botCerrar->setEnabled(true);
    ui->botDeshacer->setEnabled(false);
    ui->botEditar->setEnabled(true);
    ui->botGuardar->setEnabled(false);
    ui->botSiguiente->setEnabled(true);


}
void FrmArticulos::desbloquearCampos() {
    // LineEdit
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
//    // SpinBox
//    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
//    QSpinBox *SpinBox;
//    foreach (SpinBox, SpinBoxList) {
//        SpinBox->setReadOnly(false);
//        //qDebug() << lineEdit->objectName();
//    }
    // DoubleSpinBox
//    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
//    QDoubleSpinBox *DSpinBox;
//    foreach (DSpinBox, DSpinBoxList) {
//        DSpinBox->setReadOnly(false);
//        //qDebug() << lineEdit->objectName();
//    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    ui->botAnadir->setEnabled(false);
    ui->botAnterior->setEnabled(false);
    ui->botBorrar->setEnabled(false);
    ui->botBuscar->setEnabled(false);
    ui->botCerrar->setEnabled(false);
    ui->botDeshacer->setEnabled(true);
    ui->botEditar->setEnabled(false);
    ui->botGuardar->setEnabled(true);
    ui->botSiguiente->setEnabled(false);
}

void FrmArticulos::LLenarCampos()
{
    Configuracion *oConfig = new Configuracion();
    oConfig->CargarDatos();
    ui->txtcCodigo->setText(oArticulo->getcCodigo());
   ui->txtcCodigoBarras->setText(oArticulo->getcCodigoBarras());
   ui->txtcCodigoFabricante->setText(oArticulo->getcCodigoFabricante());
   ui->txtcDescripcion->setText(oArticulo->getcDescripcion());
   ui->txtcDescripcionResumida->setText(oArticulo->getcDescripcionReducida());
   // Recupero proveedor
   QSqlQuery *qryProveedor = new QSqlQuery(QSqlDatabase::database("empresa"));
   qryProveedor->prepare("select id,cProveedor from Proveedores where id = :id");
   qryProveedor->bindValue(":id",oArticulo->getid_Proveedor());
   if (!qryProveedor->exec()) {
       QMessageBox::warning(NULL,tr("Error Datos"),tr("No se encuentra el proveedor asociado \n Deberá comprovar ficha producto"),tr("OK"));

   } else {
       qryProveedor->next();
       QSqlRecord record = qryProveedor->record();
       ui->txtcProveedor->setText(record.field("cProveedor").value().toString());
   }
   delete qryProveedor;
   ui->cboFamilia->setEditText(oArticulo->getcFamilia());
   ui->cboSeccion->setEditText(oArticulo->getcSeccion());
   ui->cboSubfamilia->setEditText(oArticulo->getcSubfamilia());
   ui->cboTipoIVA->setEditText(QString::number(oArticulo->getnTipoIva()));
   ui->txtrCoste->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrCoste(),'f',2)));
   ui->txtrTarifa1->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrTarifa1(),'f',2)));
   ui->txtrTarifa1_2->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrTarifa1(),'f',2)));
   ui->txtrDto->setText(QString::number(oArticulo->getrDto(),'f',2));
   ui->txtnDtoProveedor->setText(QString::number(oArticulo->getnDtoProveedor(),'f',2));
   ui->txtnDtoProveedor2->setText(QString::number(oArticulo->getnDtoProveedor2(),'f',2));
   ui->txtnDtoProveedor3->setText(QString::number(oArticulo->getnDtoProveedor3(),'f',2));
   ui->txtdFechaUltimaCompra->setDate(oArticulo->getdUltimaCompra());
   ui->txtdFechaUltimaVenta->setDate(oArticulo->getdUltimaVenta());
   ui->txtnMargen1->setText(QString::number(oArticulo->getnMargen1(),'f',2));
   ui->txtnMargen2->setText(QString::number(oArticulo->getnMargen2(),'f',2));
   ui->txtnMargen3->setText(QString::number(oArticulo->getnMargen3(),'f',2));
   ui->txtrPrecioMedio1->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrPrecioMedio(),'f',2)));
   ui->txtrPrecioMedio2->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrPrecioMedio2(),'f',2)));
   ui->txtrPrecioMedio3->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrPrecioMedio3(),'f',2)));
   ui->txtnUnidadesCompradas->setText(QString::number(oArticulo->getnUnidadesCompradas()));
   ui->txtnUnidadesVendidas->setText(QString::number(oArticulo->getnUnidadesVendidas()));
   ui->txtrAcumuladoCompras->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrAcumuladoCompras(),'f',2)));
   ui->txtrAcumuladoVentas->setText(oConfig->FormatoNumerico(QString::number(oArticulo->getrAcumuladoVentas(),'f',2)));
   ui->txttComentario->setText(oArticulo->gettComentario());
   ui->txtnStockMaximo->setText(QString::number(oArticulo->getnStockMaximo()));
   ui->txtnStockMinimo->setText(QString::number(oArticulo->getnStockMinimo()));
   ui->txtnStockReal->setText(QString::number(oArticulo->getnStockReal()));
   ui->txtnStockReal_2->setText(QString::number(oArticulo->getnStockReal()));
   if (oArticulo->getlControlarStock()==1)
        ui->chklControlarStock->setChecked(true);
   else
       ui->chklControlarStock->setChecked(false);
   ui->cboModelo->setEditText(oArticulo->getcModelo());
   ui->cboTalla->setEditText(oArticulo->getcTalla());
   ui->cboColor->setEditText(oArticulo->getcColor());
   if (oArticulo->getlPvpIncluyeIva()== 1)
        ui->chklPvpIncluyeIva->setChecked(true);
   else
        ui->chklPvpIncluyeIva->setChecked(false);
   ui->txtnCantidadPendienteRecibir->setText(QString::number(oArticulo->getnCantidadPendienteRecibir()));
   ui->txtdFechaPrevistaRecepcion->setDate(oArticulo->getdFechaPrevistaRecepcion());
   ui->txtnReservados->setText(QString::number(oArticulo->getnReservados()));
   ui->txtnEtiquetas->setText(QString::number(oArticulo->getnEtiquetas()));
   if (oArticulo->getlMostrarWeb()==1)
        ui->chklMostrarWeb->setChecked(true);
    else
       ui->chklMostrarWeb->setChecked(false);
   ui->txtcLocalizacion->setText(oArticulo->getcLocalizacion());

}

void FrmArticulos::VaciarCampos()
{
   oArticulo->Vaciar();
   ui->txtcCodigo->clear();
   ui->txtcCodigoBarras->clear();
   ui->txtcCodigoFabricante->clear();
   ui->txtcDescripcion->clear();
   ui->txtcDescripcionResumida->clear();
   ui->txtcProveedor->clear();
   ui->cboFamilia->clear();
   ui->cboSeccion->clear();
   ui->cboSubfamilia->clear();
   ui->cboTipoIVA->clear();
   ui->txtrCoste->clear();
   ui->txtrTarifa1->clear();
   ui->txtrTarifa1_2->clear();
   ui->txtrDto->clear();
   ui->txtnDtoProveedor->clear();
   ui->txtnDtoProveedor2->clear();
   ui->txtnDtoProveedor3->clear();
   ui->txtdFechaUltimaCompra->clear();
   ui->txtdFechaUltimaVenta->clear();
   ui->txtnMargen1->clear();
   ui->txtnMargen2->clear();
   ui->txtnMargen3->clear();
   ui->txtrPrecioMedio1->clear();
   ui->txtrPrecioMedio2->clear();
   ui->txtrPrecioMedio3->clear();
   ui->txtnUnidadesCompradas->clear();
   ui->txtnUnidadesVendidas->clear();
   ui->txtrAcumuladoCompras->clear();
   ui->txtrAcumuladoVentas->clear();
   ui->txttComentario->clear();
   ui->txtnStockMaximo->clear();
   ui->txtnStockMinimo->clear();
   ui->txtnStockReal->clear();
   ui->txtnStockReal_2->clear();
   ui->chklControlarStock->setChecked(false);
   ui->cboModelo->clear();
   ui->cboTalla->clear();
   ui->cboColor->clear();
   ui->chklPvpIncluyeIva->setChecked(false);
   ui->txtnCantidadPendienteRecibir->clear();
   ui->txtdFechaPrevistaRecepcion->clear();
   ui->txtnReservados->clear();
   ui->txtnEtiquetas->clear();
   ui->chklMostrarWeb->setChecked(false);
   ui->txtcLocalizacion->clear();

}

void FrmArticulos::on_botEditar_clicked()
{
    desbloquearCampos();
    ui->txtcCodigo->setFocus();
}

void FrmArticulos::on_botBorrar_clicked()
{
    int id_Art = oArticulo->getId();
    id_Art --;

    oArticulo->Borrar(oArticulo->getId());
    VaciarCampos();
}


