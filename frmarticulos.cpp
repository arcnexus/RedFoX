#include "frmarticulos.h"
#include "ui_frmarticulos.h"
#include "configuracion.h"

#include "Gestion_Almacen/gestion_seccionalmacen.h"

FrmArticulos::FrmArticulos(Configuracion *o_config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmArticulos)
{
    oArticulo = new Articulo();
    modArt = new QSqlQueryModel();
    ui->setupUi(this);
    // Cargar valores IVA
    o_config->CargarDatos();
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA1));
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA2));
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA3));
    ui->cboTipoIVA->addItem(QString::number(o_config->nIVA4));
    // Control objetos
    ui->lblMensajeRecuperar->setVisible(false);
    ui->botRotarImagen90->setVisible(false);

    bloquearCampos();
    //-----------------------------------------
    // CONEXIONES
    //-----------------------------------------
    connect(ui->txtrTarifa1,SIGNAL(editingFinished()),this,SLOT(on_txtrTarifa1_editingFinished()));
    connect(ui->txtrCoste,SIGNAL(editingFinished()),this,SLOT(on_txtrCoste_editingFinished()));
    connect(ui->txtrTarifa1_2,SIGNAL(editingFinished()),this,SLOT(on_txtrTarifa1_2_editingFinished()));
    connect(ui->txtrTarifa2,SIGNAL(editingFinished()),this,SLOT(on_txtrTarifa2_editingFinished()));
    connect(ui->txtrTarifa3,SIGNAL(editingFinished()),this,SLOT(on_txtrTarifa3_editingFinished()));

}

FrmArticulos::~FrmArticulos()
{
    delete ui;
    delete oArticulo;
    delete modArt;
}

void FrmArticulos::on_botAnadir_clicked()
{
    desbloquearCampos();
    VaciarCampos();
    //LLenarCampos();
    //ui->lblImagenArticulo->setPixmap(QPixmap::fromImage());
    ui->txtcCodigo->setFocus();
}

void FrmArticulos::on_botGuardar_clicked()
{
    bloquearCampos();
    oArticulo->Anadir();
    CargarCamposEnArticulo();
    oArticulo->Guardar();
}

void FrmArticulos::on_botSiguiente_clicked()
{
    QString cId = QString::number(oArticulo->getId());
    oArticulo->Recuperar("Select * from articulos where id >"+cId+" order by id limit 1 ",1);
    LLenarCampos();
}
void FrmArticulos::on_botAnterior_clicked()
{
    QString cId = QString::number(oArticulo->getId());
    oArticulo->Recuperar("Select * from articulos where id <"+cId+" order by id desc limit 1 ",2);
    LLenarCampos();
}

void FrmArticulos::bloquearCampos() {

    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(true);
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
    ui->botDeshacer->setEnabled(false);
    ui->botEditar->setEnabled(true);
    ui->botGuardar->setEnabled(false);
    ui->botSiguiente->setEnabled(true);
    // activo controles que deben estar activos.

    ui->radDescripcion->setEnabled(true);
    ui->radPrecio->setEnabled(true);
    ui->txtBuscarArticulo->setReadOnly(false);
    ui->botBuscarArtRapido->setEnabled(true);
    // Botones artículos
    ui->botBuscarSeccion->setEnabled(false);
    ui->botBuscarFamilia->setEnabled(false);
    ui->botBuscarSubfamilia->setEnabled(false);
    ui->botBuscarModelo->setEnabled(false);
    ui->botBuscarTalla->setEnabled(false);
    ui->botBuscarColor->setEnabled(false);
    ui->botCambiarImagen->setEnabled(false);

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
    ui->botDeshacer->setEnabled(true);
    ui->botEditar->setEnabled(false);
    ui->botGuardar->setEnabled(true);
    ui->botSiguiente->setEnabled(false);
    // Botones artículos
    ui->botBuscarSeccion->setEnabled(true);
    ui->botBuscarFamilia->setEnabled(true);
    ui->botBuscarSubfamilia->setEnabled(true);
    ui->botBuscarModelo->setEnabled(true);
    ui->botBuscarTalla->setEnabled(true);
    ui->botBuscarColor->setEnabled(true);
    ui->botCambiarImagen->setEnabled(true);
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
       QMessageBox::warning(qApp->activeWindow(),tr("Error Datos"),tr("No se encuentra el proveedor asociado \n Deberá comprovar ficha producto"),tr("OK"));

   } else {
       qryProveedor->next();
       QSqlRecord record = qryProveedor->record();
       ui->txtcProveedor->setText(record.field("cProveedor").value().toString());
   }
   delete qryProveedor;
   //Busco index

   ui->txtcFamilia->setText(oArticulo->getcFamilia());
   ui->txtcSeccion->setText(oArticulo->getcSeccion());
   ui->txtcSubFamilia->setText(oArticulo->getcSubfamilia());
   int nIndex = ui->cboTipoIVA->findText(QString::number(oArticulo->getnTipoIva()));
   if (nIndex !=-1)
           ui->cboTipoIVA->setCurrentIndex(nIndex);
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
   ui->txtcModelo->setText(oArticulo->getcModelo());
   ui->txtcTalla->setText(oArticulo->getcTalla());
   ui->txtcColor->setText(oArticulo->getcColor());
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
   delete oConfig;
}

void FrmArticulos::CargarCamposEnArticulo()
{
    oArticulo->setcCodigo(ui->txtcCodigo->text());
    oArticulo->setcCodigoBarras(ui->txtcCodigoBarras->text());
    oArticulo->setcCodigoFabricante(ui->txtcCodigoFabricante->text());
    oArticulo->setcDescripcion( ui->txtcDescripcion->text());
    oArticulo->setcDescripcionReducida( ui->txtcDescripcionResumida->text());
    oArticulo->setcFamilia(ui->txtcFamilia->text());
    oArticulo->setcSeccion(ui->txtcSeccion->text());
    oArticulo->setcSubfamilia(ui->txtcSubFamilia->text());
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

    oArticulo->setcModelo(ui->txtcModelo->text());
    oArticulo->setcTalla(ui->txtcTalla->text());
    oArticulo->setcColor( ui->txtcColor->text());
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
   ui->txtcFamilia->clear();
   ui->txtcSeccion->clear();
   ui->txtcSubFamilia->clear();
   ui->cboTipoIVA->setEditText("");
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
   ui->txtcModelo->clear();
   ui->txtcTalla->clear();
   ui->txtcColor->clear();
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
    ficheroImagen = QFileDialog::getOpenFileName(this,tr("Abrir fichero de imagen"),"","Imagenes (*.bmp *.png *.xpm *.jpg)");
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
            QMessageBox::warning(qApp->activeWindow(),tr("Guardar Imagen"),tr("No se ha podido guardar la imagen en la base de datos"),tr("Ok"));
        delete Articulo;
    }
}

void FrmArticulos::on_botRotarImagen90_clicked()
{
    ui->lblImagenArticulo->setScaledContents(false);
    // Create new rotatedPixmap that size is same as original
    //QPixmap img(*ui->nombre_del_qlabel->pixmap());
    QPixmap pixmap(*ui->lblImagenArticulo->pixmap());
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

//    // Guardamos la imagen una vez rotada
  //  QPixmap pixmap;
    // Preparation of our QPixmap

    QByteArray bArray;
   QBuffer buffer( &bArray );
    buffer.open( QIODevice::WriteOnly );
    pixmap.save( &buffer, "PNG" );

    QSqlQuery *Articulo = new QSqlQuery(QSqlDatabase::database("empresa"));
    Articulo->prepare("update articulos set bImagen =:imagen where Id = :nid");
   Articulo->bindValue(":imagen",bArray);
    Articulo->bindValue(":nid",oArticulo->getId());
   if (!Articulo->exec())
        QMessageBox::warning(qApp->activeWindow(),tr("Guardar Imagen"),tr("No se ha podido guardar la imagen en la base de datos"),tr("Ok"));
    delete Articulo;
    ui->lblImagenArticulo->setPixmap(pixmap);
    ui->lblImagenArticulo->setScaledContents(true);
}

void FrmArticulos::on_botDeshacer_clicked()
{
    QString cSql = "Select * from articulos where Id =" +QString::number(oArticulo->getId());
    oArticulo->Recuperar(cSql);
    LLenarCampos();
    bloquearCampos();
}

void FrmArticulos::on_txtrTarifa1_editingFinished()
{
    Configuracion *oConf = new Configuracion();
    ui->txtrTarifa1->setText( oConf->FormatoNumerico(ui->txtrTarifa1->text()));
    ui->txtrTarifa1_2->setText(ui->txtrTarifa1->text());
    delete oConf;
}

void FrmArticulos::on_txtrCoste_editingFinished()
{
    Configuracion *oConf = new Configuracion();
    ui->txtrCoste->setText( oConf->FormatoNumerico(ui->txtrCoste->text()));
    delete oConf;
}

void FrmArticulos::on_txtrTarifa1_2_editingFinished()
{
    Configuracion *oConf = new Configuracion();
    ui->txtrTarifa1_2->setText( oConf->FormatoNumerico(ui->txtrTarifa1_2->text()));
    ui->txtrTarifa1->setText(ui->txtrTarifa1_2->text());
    delete oConf;
}

void FrmArticulos::on_txtrTarifa2_editingFinished()
{
    Configuracion *oConf = new Configuracion();
    ui->txtrTarifa2->setText( oConf->FormatoNumerico(ui->txtrTarifa2->text()));
    delete oConf;
}

void FrmArticulos::on_txtrTarifa3_editingFinished()
{
    Configuracion *oConf = new Configuracion();
    ui->txtrTarifa3->setText( oConf->FormatoNumerico(ui->txtrTarifa3->text()));
    delete oConf;
}

void FrmArticulos::on_botBuscarArtRapido_clicked()
{
    if(!ui->txtBuscarArticulo->text().isEmpty()) {
        ui->lblMensajeRecuperar->setVisible(true);
        modArt = new QSqlQueryModel();
        QString cSQL;
        if(ui->radDescripcion->isChecked())
            cSQL = "select id,cDescripcion,rTarifa from articulos where cDescripcion like '%" +
                    ui->txtBuscarArticulo->text().trimmed()+"%' order by cDescripcion";
       else
            cSQL = "select id,cDescripcion,rTarifa1 from articulos where cDescripcion like '%" +
                    ui->txtBuscarArticulo->text().trimmed()+"%' order by rTarifa1";

        modArt->setQuery(cSQL,QSqlDatabase::database("empresa"));
        ui->TablaBuscarArt->setModel(modArt);
        QHeaderView *Cabecera = new QHeaderView(Qt::Horizontal,this);
        // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
        ui->TablaBuscarArt->setHorizontalHeader(Cabecera);
        /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0". (en nuestro caso está oculta ya que muestra el id de la tabla y esto no nos interesa que lo vea el usuario */
    //    Cabecera->setResizeMode(0,QHeaderView::Fixed);
        Cabecera->resizeSection(0,0);
    //    Cabecera->setResizeMode(1,QHeaderView::Fixed);
        Cabecera->resizeSection(1,155);
    //    Cabecera->setResizeMode(2,QHeaderView::Fixed);
        Cabecera->resizeSection(2,65);
        Cabecera->setVisible(true);
        modArt->setHeaderData(1, Qt::Horizontal, QObject::tr("DESCRIPCIÓN"));
        modArt->setHeaderData(2, Qt::Horizontal, QObject::tr("PVP"));

        //ui->TablaBuscarArt->setItemDelegateForColumn(2, Columna);
    } else {
        QMessageBox::critical(this,tr("Buscar Artículos"),tr("Debe especificar algún criterio de búsqueda"),tr("OK"));
    }
}

void FrmArticulos::on_TablaBuscarArt_doubleClicked(const QModelIndex &index)
{
    QModelIndex celda=ui->TablaBuscarArt->currentIndex();
    QModelIndex index1=modArt->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=modArt->data(index1,Qt::EditRole);
    QString cSQL = "Select * from articulos where id = "+pKey.toString();
    oArticulo->Recuperar(cSQL);
    LLenarCampos();
}



void FrmArticulos::on_botBuscarSeccion_clicked()
{
    gestion_SeccionAlmacen form(this,"secciones",editar);
    form.setWindowTitle(tr("Secciones"));
    if(form.exec() == QDialog::Accepted)
    {
        ui->txtcSeccion->setText(form.value);
    }
}



void FrmArticulos::AnadirSeccion()
{
    QLineEdit *txtcSeccionNueva = new QLineEdit(ventana);
    layout->addWidget(txtcSeccionNueva,2,1,2,1);

}

void FrmArticulos::on_botBuscarFamilia_clicked()
{
    gestion_SeccionAlmacen form(this,"familias",editar);
    form.setWindowTitle(tr("Familias"));
    QSqlQuery query(QSqlDatabase::database("empresa"));
    query.prepare(QString("SELECT id FROM secciones WHERE cSeccion = '%1' ").arg(ui->txtcSeccion->text()));
    if (query.exec())
        if(query.next())
            form.filter("Id_Seccion = "+query.record().value(0).toString());

    qDebug () << query.lastQuery();
    qDebug () << query.lastError();
    if(form.exec() == QDialog::Accepted)
    {
        ui->txtcFamilia->setText(form.value);
    }
}

void FrmArticulos::on_botBuscarSubfamilia_clicked()
{
    gestion_SeccionAlmacen form(this,"subfamilias",editar);
    form.setWindowTitle(tr("SubFamilias"));
    QSqlQuery query(QSqlDatabase::database("empresa"));
    query.prepare(QString("SELECT id FROM familias WHERE cFamilia = '%1'").arg(ui->txtcFamilia->text()));
    if (query.exec())
        if(query.next())
            form.filter("Id_Seccion = "+query.record().value(0).toString());

    if(form.exec() == QDialog::Accepted)
    {
        ui->txtcSubFamilia->setText(form.value);
    }
}
