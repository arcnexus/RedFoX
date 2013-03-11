#include "frmarticulos.h"
#include "ui_frmarticulos.h"
#include "../Almacen/frmtarifas.h"
#include "Busquedas/frmbuscarproveedor.h"
#include "Almacen/frmasociarproveedor.h"

#include "../db_table_view.h"

FrmArticulos::FrmArticulos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmArticulos)
{
    oArticulo = new Articulo();
    modArt = new QSqlQueryModel();
    ui->setupUi(this);
    // Cargar valores IVA
    //Configuracion_global->CargarDatos();
    ui->cboTipoIVA->setModel(Configuracion_global->iva_model);
    ui->cboTipoIVA->setModelColumn(Configuracion_global->iva_model->fieldIndex("cTipo"));


    ui->grafica->EnableLeyenda(true);
    ui->grafica->addItem("En",Qt::red,10);
    ui->grafica->addItem("Fe",Qt::red,15);
    ui->grafica->addItem("Ma",Qt::red,10);
    ui->grafica->addItem("Ab",Qt::red,25);
    ui->grafica->addItem("Ma",Qt::red,40);
    ui->grafica->addItem("Ju",Qt::red,30);
    ui->grafica->addItem("Jul",Qt::red,20);
    ui->grafica->addItem("Ag",Qt::red,60);
    ui->grafica->addItem("Se",Qt::red,50);
    ui->grafica->addItem("Oc",Qt::red,70);
    ui->grafica->addItem("No",Qt::red,75);
    ui->grafica->addItem("Di",Qt::red,90);
    // Control objetos
    ui->lblMensajeRecuperar->setVisible(false);
    // --------------------
    // TARIFAS
    //---------------------
    tarifa_model = new QSqlRelationalTableModel(this,QSqlDatabase::database("terra"));
    tarifa_model->setTable("viewTarifa");
    tarifa_model->setFilter("id_Articulo = "+QString::number(oArticulo->id));
    tarifa_model->select();
    //ui->TablaTarifas->setModel(tarifa_model);
    //ui->TablaTarifas->setColumnHidden(0,true);
    QStringList headers;
    headers << "Pais" << "Moneda" << "Codigo de tarifa" << "Descripción";
    headers<< "Margen" << "Margen minimo" << "P.V.P";
    for (int i = 0; i< headers.size();i++)
        tarifa_model->setHeaderData(i+1, Qt::Horizontal, headers.at(i));


    bloquearCampos();


    //--------------------------------
    // CAMPOS MONEDA
    //--------------------------------
    //ui->txtrCoste->setValidator(Configuracion_global->validator_cantidad);


    //-----------------------------------------
    // CONEXIONES
    //-----------------------------------------
    connect(ui->txtrCoste,SIGNAL(editingFinished()),Configuracion_global,SLOT(format_text()));
    connect(ui->btnAnadirProveedores,SIGNAL(clicked()),this,SLOT(anadir_proveedor_clicked()));

}

FrmArticulos::~FrmArticulos()
{
    delete ui;
    delete oArticulo;
    delete modArt;
}

void FrmArticulos::on_botAnadir_clicked()
{
    QSqlQuery querySecciones(QSqlDatabase::database("terra"));
    querySecciones.prepare("select id from secciones order by id desc limit 1 ");
    if (querySecciones.exec()){
        querySecciones.next();
        if (querySecciones.record().value("id").toInt()<=0)
        {
                QMessageBox message;
                message.setWindowTitle(tr("Gestión de artículos"));
                message.setText("Para poder añadir artículos debe tener registros de sección");
                message.setAutoFillBackground(true);
                message.setButtonText(1,tr("Aceptar"));
                message.exec();
        } else {
            desbloquearCampos();
            VaciarCampos();
            oArticulo->Anadir();
            this->anadir = true;
            LLenarCampos();
            //ui->lblImagenArticulo->setPixmap(QPixmap::fromImage());
            ui->txtcCodigo->setFocus();
        }
    }
}

void FrmArticulos::on_botGuardar_clicked()
{
    if(!ui->txtcSeccion->text().isEmpty())
    {
        bloquearCampos();
        CargarCamposEnArticulo();
        oArticulo->Guardar();
    } else
    {
        QMessageBox::warning(this,tr("Gestion de Productos/servicios"),
                             tr("Es necesario que asocie el registro a una sección antes de poder guardar"),
                             tr("Aceptar"));
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
    ui->botBuscarSubSubFamilia->setEnabled(false);
    ui->botCambiarImagen->setEnabled(false);
    ui->botBuscarGrupo->setEnabled(false);
    ui->btnBuscarProveedor->setEnabled(false);

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
    ui->botBuscarSubSubFamilia->setEnabled(true);
    ui->botBuscarGrupo->setEnabled(true);
    ui->botCambiarImagen->setEnabled(true);
    ui->btnBuscarProveedor->setEnabled(true);
}

void FrmArticulos::LLenarCampos()
{
   ui->txtcCodigo->setText(oArticulo->cCodigo);
   ui->lblCodigo->setText(oArticulo->cCodigo);
   ui->txtcCodigoBarras->setText(oArticulo->cCodigoBarras);
   ui->txtcCodigoFabricante->setText(oArticulo->cCodigoFabricante);
   ui->txtcDescripcion->setText(oArticulo->cDescripcion);
   ui->lblDescripcion->setText(oArticulo->cDescripcion);
   ui->txtcDescripcionResumida->setText(oArticulo->cDescripcionReducida);
   ui->txtcProveedor->setText(oArticulo->cProveedor);
   ui->txtCodigoProveedor->setText(oArticulo->cCodProveedor);
   ui->txtcFamilia->setText(oArticulo->cFamilia);
   ui->txtcSeccion->setText(oArticulo->cSeccion);
   ui->txtcSubFamilia->setText(oArticulo->cSubfamilia);
   int nIndex = ui->cboTipoIVA->findText(QString::number(oArticulo->nTipoIva));
   if (nIndex !=-1)
           ui->cboTipoIVA->setCurrentIndex(nIndex);
   ui->txtrDto->setText(QString::number(oArticulo->rDto,'f',2));
   ui->txtdFechaUltimaCompra->setDate(oArticulo->dUltimaCompra);
   ui->txtdFechaUltimaVenta->setDate(oArticulo->dUltimaVenta);
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
   ui->txtcSeccion->setText(oArticulo->getcSeccion(oArticulo->id_Seccion));
   ui->txtcFamilia->setText(oArticulo->getcFamilia(oArticulo->id_Familia));
   ui->txtcSubFamilia->setText(oArticulo->getcSubFamilia(oArticulo->id_SubFamilia));
   ui->txtcSubSubFamilia->setText((oArticulo->getcSubSubFamilia(oArticulo->idsubsubfamilia)));
   ui->txtcGupoArt->setText(oArticulo->getcGrupo(oArticulo->idgrupoart));
   if (oArticulo->lMostrarWeb==1)
        ui->chklMostrarWeb->setChecked(true);
    else
       ui->chklMostrarWeb->setChecked(false);
//    // Recuperamos imagen desde BD
   oArticulo->CargarImagen(ui->lblImagenArticulo);
    // llenamos combo iva
  nIndex = ui->cboTipoIVA->findText(Configuracion_global->setTipoIva(oArticulo->id_tiposiva));
  if(nIndex >-1)
      ui->cboTipoIVA->setCurrentIndex(nIndex);
  //---------------------
  // Tarifas
  //---------------------
  QSqlQueryModel *modelTarifa = new QSqlQueryModel(this);
  modelTarifa->setQuery("select codigo_tarifa,descripcion,pais,moneda,margen, margenminimo, pvp "
                       "from viewTarifa where id_Articulo = "+QString::number(oArticulo->id),
                       QSqlDatabase::database("terra"));
  ui->TablaTarifas->setModel(modelTarifa);

  //-----------------------
  // Proveedores frecuentes
  //-----------------------
  modelProv = new QSqlQueryModel(this);
  modelProv->setQuery("select  codpro,cProveedor,codigo,pvd,descoferta,oferta from proveedores_frecuentes where id_art = "+
                      QString::number(oArticulo->id),QSqlDatabase::database("terra"));
  ui->tablaProveedores->setModel(modelProv);
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
    oArticulo->nTipoIva=Configuracion_global->ivas[ui->cboTipoIVA->currentText()].value("nIva").toDouble();
    oArticulo->id_tiposiva = Configuracion_global->getIdIva(ui->cboTipoIVA->currentText());
    oArticulo->rCoste=ui->txtrCoste->text().toDouble();
    oArticulo->dUltimaCompra= ui->txtdFechaUltimaCompra->date();
    oArticulo->dUltimaVenta= ui->txtdFechaUltimaVenta->date();
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
    oArticulo->id_tiposiva = Configuracion_global->getIdIva(ui->cboTipoIVA->currentText());
    oArticulo->id_Seccion = oArticulo->getIdSeccion(ui->txtcSeccion->text());
    oArticulo->id_Familia  = oArticulo->getIdFamilia(ui->txtcFamilia->text());
    oArticulo->id_SubFamilia = oArticulo->getIdSubFamilia(ui->txtcSubFamilia->text());
    oArticulo->idsubsubfamilia = oArticulo->getIdSubSufFamilia(ui->txtcSubSubFamilia->text());
    oArticulo->idgrupoart = oArticulo->getIdGrupo(ui->txtcGupoArt->text());
    oArticulo->cCodProveedor = ui->txtCodigoProveedor->text();
    oArticulo->cProveedor = ui->txtcProveedor->text();

//    this->idweb = registro.field("idweb").value().toInt();
    oArticulo->stockfisico = ui->txtStockFisico->text().toInt();

}

void FrmArticulos::VaciarCampos()
{
   oArticulo->Vaciar();
   ui->txtcCodigo->setText("");
   ui->txtcCodigoBarras->setText("");
   ui->txtcCodigoFabricante->setText("");
   ui->txtcDescripcion->setText("");
   ui->txtcDescripcionResumida->setText("");
   ui->txtcProveedor->setText("");
   ui->txtcFamilia->setText("");
   ui->txtcSeccion->setText("");
   ui->txtcSubFamilia->setText("");
   ui->cboTipoIVA->setEditText("");
   ui->txtrCoste->setText("0,00");
   ui->txtrDto->setText("0");
   ui->txtdFechaUltimaCompra->setDate(QDate::currentDate());
   ui->txtdFechaUltimaVenta->setDate(QDate::currentDate());
   ui->txtnUnidadesCompradas->setText("0");
   ui->txtnUnidadesVendidas->setText("0");
   ui->txtrAcumuladoCompras->setText("0,00");
   ui->txtrAcumuladoVentas->setText("0,00");
   ui->txttComentario->setText("");
   ui->txtnStockMaximo->setText("0");
   ui->txtnStockMinimo->setText("0");
   ui->txtnStockReal->setText("0");
   ui->txtnStockReal_2->setText("0");
   ui->txtStockFisico->setText("0");
   ui->chklControlarStock->setChecked(false);
   ui->txtnCantidadPendienteRecibir->setText("0");
   ui->txtdFechaPrevistaRecepcion->setDate(QDate::currentDate());
   ui->txtnReservados->setText("0");
   ui->chklMostrarWeb->setChecked(false);
   ui->lblImagenArticulo->setText("");
   ui->txtCodigoProveedor->setText("");

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
    if(this->anadir)
    {
        QSqlQuery qArt(QSqlDatabase::database("terra"));
        qArt.prepare("delete from articulos where id = :nId");
        qArt.bindValue("nId",oArticulo->id);
        if(qArt.exec());
            VaciarCampos();
        this->anadir = false;
    } else
        {
        QString cSql = "Select * from articulos where Id =" +QString::number(oArticulo->id);
        oArticulo->Recuperar(cSql);
        LLenarCampos();
    }
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

    form.set_selection("cFamilia");
    QSqlQuery query(QSqlDatabase::database("terra"));
    query.prepare(QString("SELECT id FROM secciones WHERE cFamilia = '%1' ").arg(ui->txtcFamilia->text()));
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

void FrmArticulos::on_botBuscarSubSubFamilia_clicked()
{
    Db_table_View form(this);
    form.set_db("terra");
    form.set_table("subsubfamilias");

    form.setWindowTitle(tr("Subsubfamilias"));

    QStringList headers;
    headers << tr("SubsubFamilia") << tr("Pertenece a");
    form.set_table_headers(headers);

    form.set_relation(2,QSqlRelation("subfamilias","id","cSubfamilia"));

    form.set_columnHide(0);

    form.set_selection("cSubsubfamilia");
    QSqlQuery query(QSqlDatabase::database("terra"));
    query.prepare(QString("SELECT id FROM subfamilias WHERE cFamilia = '%1'").arg(ui->txtcFamilia->text()));
    if (query.exec())
        if(query.next())
            form.set_filter("idsubfamilia = "+query.record().value(0).toString());

    if(form.exec() == QDialog::Accepted)
    {
        ui->txtcSubFamilia->setText(form.selected_value);
    }
}

void FrmArticulos::on_btnBuscarProveedor_clicked()
{
    FrmBuscarProveedor buscar(this);
    if(buscar.exec()==QDialog::Accepted)
    {
        QSqlQuery qProv(QSqlDatabase::database("terra"));
        qProv.prepare("Select * from proveedores where id =:nId");
        qProv.bindValue(":nId",buscar.nIdProv);
        if(qProv.exec()){
            qProv.next();
            ui->txtCodigoProveedor->setText(qProv.record().field("cCodigo").value().toString());
            ui->txtcProveedor->setText(qProv.record().field("cProveedor").value().toString());
            oArticulo->id_Proveedor = buscar.nIdProv;
        }
    }

}

void FrmArticulos::on_btnAnadirTarifa_clicked()
{
    FrmTarifas addTarifa(this);
    if(addTarifa.exec() ==QDialog::Accepted)
    {
        QSqlQuery qTarifa(QSqlDatabase::database("terra"));
        qTarifa.prepare("INSERT INTO `TerraGeneral`.`tarifas` (`id_Articulo`, `id_pais`,"
                        "`id_monedas`, `margen`, `margenminimo`, `pvp`, `id_codigotarifa`) "
                        "VALUES (:id, :id_pais,:id_monedas,:margen,:margenminimo,:pvp,:id_codigotarifa);");
        qTarifa.bindValue(":id",oArticulo->id);
        qTarifa.bindValue(":id_pais",addTarifa.id_pais);
        qTarifa.bindValue(":id_monedas",addTarifa.id_moneda);
        qTarifa.bindValue(":margen",addTarifa.margen);
        qTarifa.bindValue(":margenminimo",addTarifa.margen_min);
        qTarifa.bindValue(":pvp",addTarifa.pvp);
        qTarifa.bindValue(":id_codigotarifa",addTarifa.codigoTarifa);
        if(qTarifa.exec()) {
            QSqlQueryModel *modelTarifa = new QSqlQueryModel(this);
            modelTarifa->setQuery("select codigo_tarifa,descripcion,pais,moneda,margen, margenminimo, pvp "
                                 "from viewTarifa where id_Articulo = "+QString::number(oArticulo->id),
                                 QSqlDatabase::database("terra"));
            ui->TablaTarifas->setModel(modelTarifa);
        } else {
            QMessageBox::information(this,tr("Gestión de Productos/Servicios"),
                                      tr("Ocurrió un error al insertar una tarifa en el artículo: %1").arg(qTarifa.lastError().text()),
                                     tr("Acceptar"));
        }
    }
}

void FrmArticulos::anadir_proveedor_clicked()
{
    FrmAsociarProveedor frmAsociar;
    if(frmAsociar.exec() == QDialog::Accepted) {
        bool ok = oArticulo->agregar_proveedor_alternativo(oArticulo->id,frmAsociar.id_proveedor,frmAsociar.codigo,frmAsociar.pvd,
                                                 frmAsociar.DescOferta,frmAsociar.Oferta);
        if (!ok)
            QMessageBox::warning(this,tr("Nuevo proveedor frecuente"),tr("Falló la inserción del proveedor"),tr("Aceptar"));

    }

    //-----------------------
    // Proveedores frecuentes
    //-----------------------
    modelProv = new QSqlQueryModel(this);
    modelProv->setQuery("select  codpro,cProveedor,codigo,pvd,descoferta,oferta from proveedores_frecuentes where id_art = "+
                        QString::number(oArticulo->id),QSqlDatabase::database("terra"));
    ui->tablaProveedores->setModel(modelProv);
}

void FrmArticulos::calcular_codigo()
{
    if (Configuracion_global->Autocodigoart == true) {
        if(!ui->txtcSeccion->text().isEmpty() && (ui->txtcCodigo->text().isEmpty() || ui->txtcCodigo->text() =="autocodigo"))
                ui->txtcCodigo->setText(oArticulo->autocodigo());
        else {

            QMessageBox mensaje;
            mensaje.setWindowTitle(tr("Nuevo código"));
            mensaje.setText(tr("Para autogenerar un código de producto como mínimo debe haber insertado una sección"));
            mensaje.setAutoFillBackground(true);
            mensaje.warning(this,tr("Nuevo código"),
                            tr("Para autogenerar un código de producto como mínimo debe haber insertado una sección"),
                            tr("Aceptar"));

        }

    }

}


