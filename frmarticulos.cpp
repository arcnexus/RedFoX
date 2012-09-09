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
#include <QPixmap>
#include <QtGui>


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
    bloquearCampos();
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
    CargarCamposEnArticulo();
    oArticulo->Guardar();
}

void FrmArticulos::on_botSiguiente_clicked()
{
    QString cId = QString::number(oArticulo->getId());
    oArticulo->Recuperar("Select * from articulos where id >"+cId+" order by id limit 0,1 ",1);
    LLenarCampos();

}
void FrmArticulos::on_botAnterior_clicked()
{
    QString cId = QString::number(oArticulo->getId());
    oArticulo->Recuperar("Select * from articulos where id <"+cId+" order by id desc limit 0,1 ",2);
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
//    // Recuperamos imagen desde BD
   oArticulo->CargarImagen(ui->lblImagenArticulo);

}

void FrmArticulos::CargarCamposEnArticulo()
{
    oArticulo->setcCodigo(ui->txtcCodigo->text());
    oArticulo->setcCodigoBarras(ui->txtcCodigoBarras->text());
    oArticulo->setcCodigoFabricante(ui->txtcCodigoFabricante->text());
    oArticulo->setcDescripcion( ui->txtcDescripcion->text());
    oArticulo->setcDescripcionReducida( ui->txtcDescripcionResumida->text());
    oArticulo->setcFamilia(ui->cboFamilia->currentText());
    oArticulo->setcSeccion(ui->cboSeccion->currentText());
    oArticulo->setcSubfamilia(ui->cboSubfamilia->currentText());
    oArticulo->setnTipoIva(ui->cboTipoIVA->currentText().toDouble());
    oArticulo->setrCoste(ui->txtrCoste->text().toDouble());
    oArticulo->setrTarifa1(ui->txtrTarifa1->text().toDouble());
    oArticulo->setrTarifa2(ui->txtrTarifa1_2->text().toDouble());
    oArticulo->setrTarifa3(ui->txtrDto->text().toDouble());
    oArticulo->setnDtoProveedor(ui->txtnDtoProveedor->text().toDouble());
    oArticulo->setnDtoProveedor2(ui->txtnDtoProveedor2->text().toDouble());
    oArticulo->setnDtoProveedor3(ui->txtnDtoProveedor3->text().toDouble());
    oArticulo->setdUltimaCompra( ui->txtdFechaUltimaCompra->date());
    oArticulo->setdUltimaVenta( ui->txtdFechaUltimaVenta->date());
    oArticulo->setnMargen1( ui->txtnMargen1->text().toDouble());
    oArticulo->setnMargen2( ui->txtnMargen2->text().toDouble());
    oArticulo->setnMargen3( ui->txtnMargen3->text().toDouble());
    oArticulo->setrPrecioMedio( ui->txtrPrecioMedio1->text().toDouble());
    oArticulo->setrPrecioMedio2( ui->txtrPrecioMedio2->text().toDouble());
    oArticulo->setrPrecioMedio3( ui->txtrPrecioMedio3->text().toDouble());
    oArticulo->setnUnidadesCompradas( ui->txtnUnidadesCompradas->text().toDouble());
    oArticulo->setnUnidadesVendidas(ui->txtnUnidadesVendidas->text().toDouble());
    oArticulo->setrAcumuladoCompras( ui->txtrAcumuladoCompras->text().toDouble());
    oArticulo->setrAcumuladoVentas( ui->txtrAcumuladoVentas->text().toDouble());
    oArticulo->settComentario( ui->txttComentario->toPlainText());
    oArticulo->setnStockMaximo(ui->txtnStockMaximo->text().toInt());
    oArticulo->setnStockMinimo( ui->txtnStockMinimo->text().toInt());
    oArticulo->setnStockReal( ui->txtnStockReal->text().toInt());
    if (ui->chklControlarStock->isChecked())
        oArticulo->setlControlarStock(1);
    else
        oArticulo->setlControlarStock(0);

    oArticulo->setcModelo(ui->cboModelo->currentText());
    oArticulo->setcTalla(ui->cboTalla->currentText());
    oArticulo->setcColor( ui->cboColor->currentText());
    if(ui->chklPvpIncluyeIva->isChecked())
        oArticulo->setlPvpIncluyeIva(1);
    else
        oArticulo->setlPvpIncluyeIva(0);
    oArticulo->setnCantidadPendienteRecibir(ui->txtnCantidadPendienteRecibir->text().toInt());
    oArticulo->setdFechaPrevistaRecepcion( ui->txtdFechaPrevistaRecepcion->date());
    oArticulo->setnReservados( ui->txtnReservados->text().toInt());
    oArticulo->setnEtiquetas( ui->txtnEtiquetas->text().toInt());
    if (ui->chklMostrarWeb->isChecked())
        oArticulo->setlMostrarWeb(1);
    else
        oArticulo->setlMostrarWeb(0);
    oArticulo->setcLocalizacion( ui->txtcLocalizacion->text());

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
   ui->lblImagenArticulo->clear();

}

void FrmArticulos::on_botEditar_clicked()
{
    desbloquearCampos();
    ui->txtcCodigo->setFocus();
}

void FrmArticulos::on_botBorrar_clicked()
{
    oArticulo->Borrar(oArticulo->getId());
    oArticulo->Recuperar("Select * from articulos where id = "+QString::number(oArticulo->getId()));
    LLenarCampos();
}



void FrmArticulos::on_botCambiarImagen_clicked()
{
    QString ficheroImagen;
    ficheroImagen = QFileDialog::getOpenFileName(this,tr("Abrir fichero de imagen"));
    if (!ficheroImagen.isEmpty()) {

        QImage imagen(ficheroImagen);

        ui->lblImagenArticulo->setPixmap(QPixmap::fromImage(imagen));
        ui->lblImagenArticulo->setScaledContents(true);
        QByteArray ba;
        QFile f(ficheroImagen);
        if(f.open(QIODevice::ReadOnly)) {
            ba = f.readAll();
            f.close();
        }
        QSqlQuery *Articulo = new QSqlQuery(QSqlDatabase::database("empresa"));
        Articulo->prepare("update articulos set bImagen =:imagen where Id = :nid");
        Articulo->bindValue(":imagen",ba);
        Articulo->bindValue(":nid",oArticulo->getId());
        if (!Articulo->exec())
            QMessageBox::warning(NULL,tr("Guardar Imagen"),tr("No se ha podido guardar la imagen en la base de datos"),tr("Ok"));
        delete Articulo;
    }
}

void FrmArticulos::on_pushButton_clicked()
{
    // Create new rotatedPixmap that size is same as original
    QPixmap pixmap; //(ui->lblImagenArticulo->pixmap());
    QPixmap rotatedPixmap(pixmap.size());

    // Create a QPainter for it
    QPainter p(&rotatedPixmap);

    // Set rotation origo into pixmap center
    QSize size = pixmap.size();
    p.translate(size.height()/2,size.height()/2);

    // Rotate the painter 90 degrees
    p.rotate(90);

    // Set origo back to upper left corner
    p.translate(-size.height()/2,-size.height()/2);

    // Draw your original pixmap on it
    p.drawPixmap(0, 0, pixmap);
    p.end();

    // Change original pixmap reference into new rotated pixmap
    pixmap = rotatedPixmap;
    ui->lblImagenArticulo->setPixmap(pixmap);
}

void FrmArticulos::on_botDeshacer_clicked()
{
    QString cSql = "Select * from articulos where Id =" +QString::number(oArticulo->getId());
    oArticulo->Recuperar(cSql);
    LLenarCampos();
    bloquearCampos();
}

void FrmArticulos::on_txtrTarifa1_lostFocus()
{
    Configuracion *oConf = new Configuracion();
    ui->txtrTarifa1->setText( oConf->FormatoNumerico(ui->txtrTarifa1->text()));
    ui->txtrTarifa1_2->setText(ui->txtrTarifa1->text());
    delete oConf;
}

void FrmArticulos::on_txtrCoste_lostFocus()
{
    Configuracion *oConf = new Configuracion();
    ui->txtrCoste->setText( oConf->FormatoNumerico(ui->txtrCoste->text()));
    delete oConf;
}

void FrmArticulos::on_txtrTarifa1_2_lostFocus()
{
    Configuracion *oConf = new Configuracion();
    ui->txtrTarifa1_2->setText( oConf->FormatoNumerico(ui->txtrTarifa1_2->text()));
    ui->txtrTarifa1->setText(ui->txtrTarifa1_2->text());
    delete oConf;
}

void FrmArticulos::on_txtrTarifa2_lostFocus()
{
    Configuracion *oConf = new Configuracion();
    ui->txtrTarifa2->setText( oConf->FormatoNumerico(ui->txtrTarifa2->text()));
    delete oConf;
}

void FrmArticulos::on_txtrTarifa3_lostFocus()
{
    Configuracion *oConf = new Configuracion();
    ui->txtrTarifa3->setText( oConf->FormatoNumerico(ui->txtrTarifa3->text()));
    delete oConf;
}
