#include "frmarticulos.h"
#include "ui_frmarticulos.h"
#include "Almacen/frmtarifas.h"

#include "db_table_view.h"

FrmArticulos::FrmArticulos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmArticulos)
{
    oArticulo = new Articulo();
    modArt = new QSqlQueryModel();
    ui->setupUi(this);
    // Cargar valores IVA
    Configuracion_global->CargarDatos();

    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    ui->cboTipoIVA->addItem(QString::number(Configuracion_global->ivas[keys.at(0)].value("nIVA").toDouble()));
    ui->cboTipoIVA->addItem(QString::number(Configuracion_global->ivas[keys.at(1)].value("nIVA").toDouble()));
    ui->cboTipoIVA->addItem(QString::number(Configuracion_global->ivas[keys.at(2)].value("nIVA").toDouble()));
    ui->cboTipoIVA->addItem(QString::number(Configuracion_global->ivas[keys.at(3)].value("nIVA").toDouble()));

    ui->cboTipoIVA_2->addItem(QString::number(Configuracion_global->ivas[keys.at(0)].value("nIVA").toDouble()));
    ui->cboTipoIVA_2->addItem(QString::number(Configuracion_global->ivas[keys.at(1)].value("nIVA").toDouble()));
    ui->cboTipoIVA_2->addItem(QString::number(Configuracion_global->ivas[keys.at(2)].value("nIVA").toDouble()));
    ui->cboTipoIVA_2->addItem(QString::number(Configuracion_global->ivas[keys.at(3)].value("nIVA").toDouble()));

    if(internacional == false) {
        ui->cboPais1->setVisible(false);
        ui->cboPais2->setVisible(false);
        ui->textoPais1->setVisible(false);
        ui->textopais2->setVisible(false);
        ui->btnVerTarifasporPais->setVisible(false);
    }
    // Cargar paises
    QSqlQueryModel *qpaises = new QSqlQueryModel(this);
    qpaises->setQuery("Select pais from paises",QSqlDatabase::database("terra"));
    ui->cboPais1->setModel(qpaises);
    ui->cboPais2->setModel(qpaises);


    // Control objetos
    ui->lblMensajeRecuperar->setVisible(false);
    // --------------------
    // TARIFAS
    //---------------------
    tarifa_model = new QSqlRelationalTableModel(this,QSqlDatabase::database("terra"));
    tarifa_model->setTable("viewTarifa");
    tarifa_model->setFilter("id_Articulo = "+QString::number(oArticulo->id));
    tarifa_model->select();
    ui->TablaTarifas->setModel(tarifa_model);
    ui->TablaTarifas->setColumnHidden(0,true);
    QStringList headers;
    headers << "Pais" << "Moneda" << "Codigo de tarifa" << "Descripción";
    headers<< "Margen" << "Margen minimo" << "P.V.P";
    for (int i = 0; i< headers.size();i++)
        tarifa_model->setHeaderData(i+1, Qt::Horizontal, headers.at(i));



    bloquearCampos();

    //--------------------------------
    // CAMPOS MONEDA
    //--------------------------------
    ui->txtrCoste->setValidator(Configuracion_global->validator_cantidad);

    //-----------------------------------------
    // CONEXIONES
    //-----------------------------------------
    connect(ui->txtrCoste,SIGNAL(editingFinished()),Configuracion_global,SLOT(format_text()));

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
    this->anadir = true;
    //LLenarCampos();
    //ui->lblImagenArticulo->setPixmap(QPixmap::fromImage());
    ui->txtcCodigo->setFocus();
}

void FrmArticulos::on_botGuardar_clicked()
{
    bloquearCampos();
    CargarCamposEnArticulo();
    if(this->anadir){
        this->anadir = false;
        oArticulo->Anadir();
    } else {
        oArticulo->Guardar();
    }
}

void FrmArticulos::on_botSiguiente_clicked()
{
    QString cCodigo = oArticulo->cCodigo;
    oArticulo->Recuperar("Select * from articulos where cCodigo >'"+cCodigo+"' order by cCodigo limit 1 ",1);
    LLenarCampos();
    tarifa_model->setFilter("id_Articulo = "+QString::number(oArticulo->id));
    tarifa_model->select();
}
void FrmArticulos::on_botAnterior_clicked()
{
    QString cCodigo =oArticulo->cCodigo;
    oArticulo->Recuperar("Select * from articulos where cCodigo <'"+cCodigo+"' order by cCodigo desc limit 1 ",2);
    LLenarCampos();
    tarifa_model->setFilter("id_Articulo = "+QString::number(oArticulo->id));
    tarifa_model->select();

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
//DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
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
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
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
    ui->botCambiarImagen->setEnabled(true);
}

void FrmArticulos::LLenarCampos()
{
   ui->txtcCodigo->setText(oArticulo->cCodigo);
   ui->txtcCodigoBarras->setText(oArticulo->cCodigoBarras);
   ui->txtcCodigoFabricante->setText(oArticulo->cCodigoFabricante);
   ui->txtcDescripcion->setText(oArticulo->cDescripcion);
   ui->txtcDescripcionResumida->setText(oArticulo->cDescripcionReducida);
   // Recupero proveedor
   QSqlQuery *qryProveedor = new QSqlQuery(QSqlDatabase::database("terra"));
   qryProveedor->prepare("select id,cProveedor from Proveedores where id = :id");
   qryProveedor->bindValue(":id",oArticulo->id_Proveedor);
   if (!qryProveedor->exec()) {
       QMessageBox::warning(qApp->activeWindow(),tr("Error Datos"),tr("No se encuentra el proveedor asociado \n Deberá comprovar ficha producto"),tr("OK"));

   } else {
       qryProveedor->next();
       QSqlRecord record = qryProveedor->record();
       ui->txtcProveedor->setText(record.field("cProveedor").value().toString());
   }
   delete qryProveedor;
   //Busco index

   ui->txtcFamilia->setText(oArticulo->cFamilia);
   ui->txtcSeccion->setText(oArticulo->cSeccion);
   ui->txtcSubFamilia->setText(oArticulo->cSubfamilia);
   int nIndex = ui->cboTipoIVA->findText(QString::number(oArticulo->nTipoIva));
   if (nIndex !=-1)
           ui->cboTipoIVA->setCurrentIndex(nIndex);
   ui->txtrCoste->setText(QString::number(oArticulo->rCoste,'f',2));
   ui->txtrTarifa1->setValue(oArticulo->rTarifa1);
   ui->txtrDto->setText(QString::number(oArticulo->rDto,'f',2));
   ui->txtdFechaUltimaCompra->setDate(oArticulo->dUltimaCompra);
   ui->txtdFechaUltimaVenta->setDate(oArticulo->dUltimaVenta);
   ui->txtrPrecioMedio1->setText(QString::number(oArticulo->rPrecioMedio,'f',2));
   ui->txtrPrecioMedio2->setText(QString::number(oArticulo->rPrecioMedio2,'f',2));
   ui->txtrPrecioMedio3->setText(QString::number(oArticulo->rPrecioMedio3,'f',2));
   ui->txtnUnidadesCompradas->setText(QString::number(oArticulo->nUnidadesCompradas));
   ui->txtnUnidadesVendidas->setText(QString::number(oArticulo->nUnidadesVendidas));
   ui->txtrAcumuladoCompras->setText(QString::number(oArticulo->rAcumuladoCompras,'f',2));
   ui->txtrAcumuladoVentas->setText(QString::number(oArticulo->rAcumuladoVentas,'f',2));
   ui->txttComentario->setText(oArticulo->tComentario);
   ui->txtnStockMaximo->setText(QString::number(oArticulo->nStockMaximo));
   ui->txtnStockMinimo->setText(QString::number(oArticulo->nStockMinimo));
   ui->txtnStockReal->setText(QString::number(oArticulo->nStockReal));
   ui->txtnStockReal_2->setText(QString::number(oArticulo->nStockReal));
   if (oArticulo->lControlarStock==1)
        ui->chklControlarStock->setChecked(true);
   else
       ui->chklControlarStock->setChecked(false);

   ui->txtnCantidadPendienteRecibir->setText(QString::number(oArticulo->nCantidadPendienteRecibir));
   ui->txtdFechaPrevistaRecepcion->setDate(oArticulo->dFechaPrevistaRecepcion);
   ui->txtnReservados->setText(QString::number(oArticulo->nReservados));
   if (oArticulo->lMostrarWeb==1)
        ui->chklMostrarWeb->setChecked(true);
    else
       ui->chklMostrarWeb->setChecked(false);
//    // Recuperamos imagen desde BD
   oArticulo->CargarImagen(ui->lblImagenArticulo);
}

void FrmArticulos::CargarCamposEnArticulo()
{
    oArticulo->cCodigo= ui->txtcCodigo->text();
    oArticulo->cCodigoBarras =ui->txtcCodigoBarras->text();
    oArticulo->cCodigoFabricante=ui->txtcCodigoFabricante->text();
    oArticulo->cDescripcion =ui->txtcDescripcion->text();
    oArticulo->cDescripcionReducida = ui->txtcDescripcionResumida->text();
    oArticulo->cFamilia=ui->txtcFamilia->text();
    oArticulo->cSeccion=ui->txtcSeccion->text();
    oArticulo->cSubfamilia=ui->txtcSubFamilia->text();
    oArticulo->nTipoIva=ui->cboTipoIVA->currentText().toDouble();
    oArticulo->rCoste=ui->txtrCoste->text().toDouble();
    oArticulo->rTarifa1=ui->txtrTarifa1->text().toDouble();
    oArticulo->rTarifa3=ui->txtrDto->text().toDouble();
    oArticulo->dUltimaCompra= ui->txtdFechaUltimaCompra->date();
    oArticulo->dUltimaVenta= ui->txtdFechaUltimaVenta->date();
    oArticulo->rPrecioMedio= ui->txtrPrecioMedio1->text().toDouble();
    oArticulo->rPrecioMedio2= ui->txtrPrecioMedio2->text().toDouble();
    oArticulo->rPrecioMedio3= ui->txtrPrecioMedio3->text().toDouble();
    oArticulo->nUnidadesCompradas= ui->txtnUnidadesCompradas->text().toDouble();
    oArticulo->nUnidadesVendidas=ui->txtnUnidadesVendidas->text().toDouble();
    oArticulo->rAcumuladoCompras= ui->txtrAcumuladoCompras->text().toDouble();
    oArticulo->rAcumuladoVentas= ui->txtrAcumuladoVentas->text().toDouble();
    oArticulo->tComentario=ui->txttComentario->toPlainText();
    oArticulo->nStockMaximo=ui->txtnStockMaximo->text().toInt();
    oArticulo->nStockMinimo=ui->txtnStockMinimo->text().toInt();
    oArticulo->nStockReal=ui->txtnStockReal->text().toInt();
    if (ui->chklControlarStock->isChecked())
        oArticulo->lControlarStock=1;
    else
        oArticulo->lControlarStock=0;

    oArticulo->nCantidadPendienteRecibir=ui->txtnCantidadPendienteRecibir->text().toInt();
    oArticulo->dFechaPrevistaRecepcion =ui->txtdFechaPrevistaRecepcion->date();
    oArticulo->nReservados =ui->txtnReservados->text().toInt();
    if (ui->chklMostrarWeb->isChecked())
        oArticulo->lMostrarWeb = 1;
    else
        oArticulo->lMostrarWeb = 0;

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
   ui->txtrDto->clear();
   ui->txtdFechaUltimaCompra->clear();
   ui->txtdFechaUltimaVenta->clear();
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
   ui->txtnCantidadPendienteRecibir->clear();
   ui->txtdFechaPrevistaRecepcion->clear();
   ui->txtnReservados->clear();
   ui->chklMostrarWeb->setChecked(false);
   ui->lblImagenArticulo->clear();

}

void FrmArticulos::on_botEditar_clicked()
{
    desbloquearCampos();
    ui->txtcCodigo->setFocus();
}

void FrmArticulos::on_botBorrar_clicked()
{
    oArticulo->Borrar(oArticulo->id);
    oArticulo->Recuperar("Select * from articulos where id = "+QString::number(oArticulo->id));
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
        QSqlQuery *Articulo = new QSqlQuery(QSqlDatabase::database("terra"));
        Articulo->prepare("update articulos set bImagen =:imagen where Id = :nid");
        Articulo->bindValue(":imagen",ba);
        Articulo->bindValue(":nid",oArticulo->id);
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

    QSqlQuery *Articulo = new QSqlQuery(QSqlDatabase::database("terra"));
    Articulo->prepare("update articulos set bImagen =:imagen where Id = :nid");
   Articulo->bindValue(":imagen",bArray);
    Articulo->bindValue(":nid",oArticulo->id);
   if (!Articulo->exec())
        QMessageBox::warning(qApp->activeWindow(),tr("Guardar Imagen"),tr("No se ha podido guardar la imagen en la base de datos"),tr("Ok"));
    delete Articulo;
    ui->lblImagenArticulo->setPixmap(pixmap);
    ui->lblImagenArticulo->setScaledContents(true);
}

void FrmArticulos::on_botDeshacer_clicked()
{
    QString cSql = "Select * from articulos where Id =" +QString::number(oArticulo->id);
    oArticulo->Recuperar(cSql);
    LLenarCampos();
    bloquearCampos();
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

        modArt->setQuery(cSQL,QSqlDatabase::database("terra"));
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
    Db_table_View form(this);
    form.set_db("terra");
    form.set_table("secciones");

    form.setWindowTitle(tr("Secciones"));

    QStringList headers;
    headers << tr("Seccion");
    form.set_table_headers(headers);

    form.set_columnHide(0);
    form.set_columnHide(2);

    form.set_selection("cSeccion");
    if(form.exec() == QDialog::Accepted)
    {
        ui->txtcSeccion->setText(form.selected_value);
        QSqlQuery qSeccion(QSqlDatabase::database("terra"));
        qSeccion.prepare("select id from secciones where cSeccion = :seccion");
        qSeccion.bindValue(":seccion",form.selected_value);
        if(qSeccion.exec())
        {
            qSeccion.next();
            oArticulo->id_Seccion= qSeccion.value(0).toInt();
        }
        else
        {
            QMessageBox::warning(this,tr("Secciones"),tr("No se ha podido vincular la seccion: %1").arg(qSeccion.lastError().text()));
        }
    }
}



void FrmArticulos::AnadirSeccion()
{
    QLineEdit *txtcSeccionNueva = new QLineEdit(ventana);
    layout->addWidget(txtcSeccionNueva,2,1,2,1);
}

void FrmArticulos::on_botBuscarFamilia_clicked()
{
    Db_table_View form(this);
    form.set_db("terra");
    form.set_table("familias");

    form.setWindowTitle(tr("Familias"));

    QStringList headers;
    headers << tr("Codigo") << tr("Familia") << tr("Pertenece a");
    form.set_table_headers(headers);

    form.set_relation(3,QSqlRelation("secciones","id","cSeccion"));

    form.set_columnHide(0);

    form.set_selection("cCodigo");//FIXME cCodigo o cFamilia???
    QSqlQuery query(QSqlDatabase::database("terra"));
    query.prepare(QString("SELECT id FROM secciones WHERE cSeccion = '%1' ").arg(ui->txtcSeccion->text()));
    if (query.exec())
        if(query.next())
            form.set_filter("Id_Seccion = "+query.record().value(0).toString());

    if(form.exec() == QDialog::Accepted)
    {
        ui->txtcFamilia->setText(form.selected_value);        
    }
}

void FrmArticulos::on_botBuscarSubfamilia_clicked()
{
    Db_table_View form(this);
    form.set_db("terra");
    form.set_table("subfamilias");

    form.setWindowTitle(tr("Subfamilias"));

    QStringList headers;
    headers << tr("SubFamilia") << tr("Pertenece a");
    form.set_table_headers(headers);

    form.set_relation(2,QSqlRelation("familias","id","cFamilia"));

    form.set_columnHide(0);

    form.set_selection("cSubfamilia");
    QSqlQuery query(QSqlDatabase::database("terra"));
    query.prepare(QString("SELECT id FROM familias WHERE cFamilia = '%1'").arg(ui->txtcFamilia->text()));
    if (query.exec())
        if(query.next())
            form.set_filter("Id_Seccion = "+query.record().value(0).toString());

    if(form.exec() == QDialog::Accepted)
    {
        ui->txtcSubFamilia->setText(form.selected_value);
    }
}


