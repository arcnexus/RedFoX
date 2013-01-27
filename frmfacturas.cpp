#include "frmfacturas.h"
#include "ui_frmfacturas.h"

#include <QSqlDatabase>
#include <QtSql>
#include <QErrorMessage>
#include<QSqlError>
#include<QDate>
#include <QSqlQueryModel>
#include <QDebug>
#include "Busquedas/frmbuscarcliente.h"
#include "Busquedas/frmBuscarFactura.h"
#include <QTableView>
#include <QHeaderView>
#include "articulo.h"

#include "frmmodificarlin_fac.h"





frmFacturas::frmFacturas(Configuracion *o_config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmFacturas),
    helper(this)
{
    oFactura = new Factura();
    oCliente1 = new Cliente();
    o_configuracion =o_config;
    o_configuracion->CargarDatos();
    ui->setupUi(this);
    // Escondo/muestro campos según configuración
    if(o_configuracion->lProfesional) {
        ui->txtnIRPF->setVisible(true);
        ui->txtrImporteIRPF->setVisible(true);
        ui->txtrImporteIRPF_2->setVisible(true);
        ui->lblIRPF->setVisible(true);
        ui->lblIRPF_2->setVisible(true);
    } else {
        ui->txtnIRPF->setVisible(false);
        ui->txtrImporteIRPF->setVisible(false);
        ui->txtrImporteIRPF_2->setVisible(false);
        ui->lblIRPF->setVisible(false);
        ui->lblIRPF_2->setVisible(false);
    }


    // Pongo valores por defecto
    ui->lblContabilizada->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lblFacturaImpresa->setVisible(false);
    // Rellenar formas de pago
    modelFP = new QSqlQueryModel();
    modelFP->setQuery("Select cFormaPago,id from FormPago",QSqlDatabase::database("empresa"));
    ui->txtcFormaPago->setModel(modelFP);
    // valores edicion
    this->Altas = false;
    //ui->txtcCodigoArticulo->setFocus();
    BloquearCampos();
    /* -----------------------------------------
     *CONEXIONES
     *----------------------------------------*/
    //connect(ui->txtcCodigoArticulo,SIGNAL(editingFinished()),this,SLOT(on_txtcCodigoArticulo_lostFocus()));

    Db_table_View* searcher = new Db_table_View(this);
    searcher->set_db("empresa");
    searcher->set_table("articulos");

    searcher->setWindowTitle(tr("Articulos"));

    QStringList headers;
    headers << tr("Codigo")<< "1" << "2" << tr("Descripción");
    searcher->set_table_headers(headers);

    searcher->set_columnHide(0);
    searcher->set_columnHide(2);
    searcher->set_columnHide(3);

    for(int i = 5; i<50;i++)
        searcher->set_columnHide(i);

    helper.set_Searcher(searcher);
    helper.help_table(ui->Lineas);

    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(&helper,SIGNAL(totalChanged(QString)),this,SLOT(totalChanged(QString)));
}

frmFacturas::~frmFacturas()
{
    delete ui;
    delete oCliente1;
    delete oFactura;
}

void frmFacturas::lineasVentas()
{
    // lineas de ventas
    QString cSQL;
    QString cId;

    cId = cId.number(oFactura->Getid());
     cSQL ="select id,cCodigo,nCantidad,cDescripcion,rPvp,rSubtotal,nDto,rDto,rTotal, nPorcIva from lin_fac "
             " where id_Cab = "+cId;
     ModelLin_fac = new QSqlQueryModel();
     ModelLin_fac->setQuery(cSQL,QSqlDatabase::database("empresa"));
     /*
     ui->Lineas->setModel(ModelLin_fac);
      //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
     Cabecera = new QHeaderView(Qt::Horizontal,this);
     // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
     ui->Lineas->setHorizontalHeader(Cabecera);
     Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0". (en nuestro caso está oculta ya que muestra el id de la tabla y esto no nos interesa que lo vea el usuario
    // Cabecera->setResizeMode(0,QHeaderView::Fixed);
     Cabecera->resizeSection(0,0);
    // Cabecera->setResizeMode(1,QHeaderView::Fixed);
     Cabecera->resizeSection(1,85);
    // Cabecera->setResizeMode(2,QHeaderView::Fixed);
     Cabecera->resizeSection(2,65);
    // Cabecera->setResizeMode(3,QHeaderView::Fixed);
     Cabecera->resizeSection(3,213);
    // Cabecera->setResizeMode(4,QHeaderView::Fixed);
     Cabecera->resizeSection(4,120);
    // Cabecera->setResizeMode(9,QHeaderView::Fixed);
     Cabecera->resizeSection(9,35);
     ModelLin_fac->setHeaderData(1, Qt::Horizontal, QObject::tr("CÓDIGO"));
     ModelLin_fac->setHeaderData(2, Qt::Horizontal, QObject::tr("CANTIDAD"));
     ModelLin_fac->setHeaderData(3, Qt::Horizontal, QObject::tr("DESCRIPCIÓN"));
     ModelLin_fac->setHeaderData(4, Qt::Horizontal, QObject::tr("PVP"));
     ModelLin_fac->setHeaderData(5, Qt::Horizontal, QObject::tr("SUBTOTAL"));
     ModelLin_fac->setHeaderData(6, Qt::Horizontal, QObject::tr("%DTO"));
     ModelLin_fac->setHeaderData(7, Qt::Horizontal, QObject::tr("DTO"));
     ModelLin_fac->setHeaderData(8, Qt::Horizontal, QObject::tr("BASE"));
     ModelLin_fac->setHeaderData(9, Qt::Horizontal, QObject::tr("%IVA"));

    // Hacemos visible la cabecera
     Cabecera->setVisible(true);
     // Delegamos el control contenido en las columnas al nuevo objeto ColumnaGrid
     //ui->Lineas->setItemDelegateForColumn(4, Columna);
     //ui->Lineas->setItemDelegateForColumn(5, Columna);
     //ui->Lineas->setItemDelegateForColumn(6, Columna);
     //ui->Lineas->setItemDelegateForColumn(7, Columna);
     //ui->Lineas->setItemDelegateForColumn(8, Columna);*/
}


void frmFacturas::LLenarCampos() {
    int lEstado;
    ui->txtcCodigoCliente->setText(oFactura->getcCodigoCliente());
    ui->txtcFactura->setText(oFactura->getcFactura());
    ui->txtdFecha->setDate(oFactura->getdFecha());
    ui->txtdFechaCobro->setDate(oFactura->getdFechaCobro());
    ui->txtcCliente->setText(oFactura->getcCliente());
    ui->txtcDireccion->setText(oFactura->getcDireccion());
    ui->txtcDireccion2->setText(oFactura->getcDireccion2());
    ui->txtcCp->setText(oFactura->getcCp());
    ui->txtcPoblacion->setText(oFactura->getcPoblacion());
    ui->txtcProvincia->setText(oFactura->getcProvincia());
    ui->txtcPais->setText(oFactura->getcPais());
    ui->txtcCif->setText(oFactura->getcCif());
     lEstado = oFactura->getlRecargoEquivalencia();
    if ((lEstado= 1)) {
        ui->chklRecargoEquivalencia->setChecked(true);
    } else {
        ui->chklRecargoEquivalencia->setChecked(false);
    }
    ui->txtrSubtotal->setText(o_configuracion->FormatoNumerico( QString::number(oFactura->getrSubtotal(),'f',2)));
    ui->txtnDto->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnDto(),'f',2)));
    ui->txtnDtoPP->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnDtoPP(),'f',2)));
    ui->txtrImporteDescuento->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteDescuento(),'f',2)));
    ui->txtrImporteDescuentoPP->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteDescuentoPP(),'f',2)));
    ui->txtrBase->setText(o_configuracion->FormatoNumerico(QString::number( oFactura->getrBase(),'f',2)));
    ui->txtnIva_2->setText(o_configuracion->FormatoNumerico(QString::number( oFactura->getnIva(),'f',2)));
    ui->txtrImporteIva->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteIva(),'f',2)));
    ui->txtrTotal->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal(),'f',2)));
    lEstado = oFactura->getlImpresa();
    if((lEstado == 1)) {
        ui->lblFacturaImpresa->setVisible(true);
    } else {
        ui->lblFacturaImpresa->setVisible(false);
    }
    lEstado = oFactura->getlCobrada();
    if ((lEstado == 1)) {
        ui->lblFacturaCobrada->setVisible(true);
        ui->txtdFechaCobro->setVisible(true);
    } else {
        ui->lblFacturaCobrada->setVisible(false);
        ui->txtdFechaCobro->setVisible(false);
    }
    lEstado = oFactura->getlContablilizada();

    if((lEstado == 1)) {
        ui->lblContabilizada->setVisible(true);
    } else {
        ui->lblContabilizada->setVisible(false);
    }
    int indice=ui->txtcFormaPago->findText(oFactura->getcFormaPago());
    if(indice!=-1)
     ui->txtcFormaPago->setCurrentIndex(indice);
    ui->txttComentario->setText(oFactura->gettComentario());
    ui->txtrBase1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase1(),'f',2)));
    ui->txtrBase2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase2(),'f',2)));
    ui->txtrBase3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase3(),'f',2)));
    ui->txtrBase4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase4(),'f',2)));
    ui->txtnPorcentajeIva1->setText(QString::number(oFactura->getnPorcentajeIVA1()));
    ui->txtnPorcentajeIva2->setText(QString::number(oFactura->getnPorcentajeIVA2()));
    ui->txtnPorcentajeIva3->setText(QString::number(oFactura->getnPorcentajeIVA3()));
    ui->txtnPorcentajeIva4->setText(QString::number(oFactura->getnPorcentajeIVA4()));
    ui->txtrIVA1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA1(),'f',2)));
    ui->txtrIVA2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA2(),'f',2)));
    ui->txtrIVA3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA3(),'f',2)));
    ui->txtrIVA4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA4(),'f',2)));
    ui->txtrTotal1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal1(),'f',2)));
    ui->txtrTotal2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal2(),'f',2)));
    ui->txtrTotal3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal3(),'f',2)));
    ui->txtrTotal4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal4(),'f',2)));
    ui->txtnRec1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnRec1(),'f',2)));
    ui->txtnRec2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnRec2(),'f',2)));
    ui->txtnRec3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnRec3(),'f',2)));
    ui->txtnRec4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getnRec4(),'f',2)));
    ui->txtrRecargoEq1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq1(),'f',2)));
    ui->txtrRecargoEq2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq2(),'f',2)));
    ui->txtrRecargoEq3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq3(),'f',2)));
    ui->txtrRecargoEq4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq4(),'f',2)));
    ui->txtrTotalRecargoEq->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotalRecargoEq(),'f',2)));
    ui->txtrEntregadoaCuenta->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrEntregadoaCuenta(),'f',2)));
    ui->txtrImportePendiente->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImportePendiente(),'f',2)));
    ui->txtcCodigoEntidad->setText(oFactura->getcCodigoEntidad());
    ui->txtcOficinaEntidad->setText(oFactura->getcOficinaEntidad());
    ui->txtcDCCuenta->setText(oFactura->getcDCCuenta());
    ui->txtcNumeroCuenta->setText(oFactura->getcNumeroCuenta());
    ui->txtcPedidoCliente->setText(oFactura->getcPedidoCliente());
    if(oFactura->getnIRPF()!=0)
        ui->lblIRPF_3->setVisible(true);
    else
        ui->lblIRPF_3->setVisible(false);
    if(oFactura->getlCobrada()==1){
        ui->lblFacturaCobrada->setVisible(true);
        ui->txtdFechaCobro->setVisible(true);
    } else {
        ui->lblFacturaCobrada->setVisible(false);
        ui->txtdFechaCobro->setVisible(false);
    }
    ui->txtnIRPF->setText(QString::number(oFactura->getnIRPF()));
    ui->txtrImporteIRPF->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteIRPF(),'f',2)));
    ui->txtrImporteIRPF_2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteIRPF(),'f',2)));

    // cargamos líneas de ventas
   lineasVentas();
}

void frmFacturas::LLenarCamposCliente()
{
    ui->txtcCodigoCliente->setText(oCliente1->getcCodigoCliente());
    ui->txtcCliente->setText(oCliente1->getcNombreFiscal());
    ui->txtcDireccion->setText(oCliente1->getcDireccion1());
    ui->txtcDireccion2->setText(oCliente1->getcDireccion2());
    ui->txtcCp->setText(oCliente1->getcCP());
    ui->txtcPoblacion->setText(oCliente1->getcPoblacion());
    ui->txtcProvincia->setText(oCliente1->getcProvincia());
    ui->txtcPais->setText(oCliente1->getcPais());
    ui->txtcCif->setText(oCliente1->getcCifNif());
    int lEstado = 0;
     lEstado = oCliente1->islRecargoEquivalencia();
    if ((lEstado= 1)) {
        ui->chklRecargoEquivalencia->setChecked(true);
    } else {
        ui->chklRecargoEquivalencia->setChecked(false);
    }
    if (oCliente1->getlIRPF() == true) {
        ui->lblIRPF_3->setVisible(true);
        ui->txtnIRPF->setText(QString::number(o_configuracion->nIRPF));
        oFactura->setnIRPF(o_configuracion->nIRPF);
    } else {
        ui->lblIRPF_3->setVisible(false);
        ui->txtnIRPF->setText("0,00");
        oFactura->setnIRPF(o_configuracion->nIRPF);
    }

}

void frmFacturas::VaciarCampos() {
    QDate dFecha;
    ui->txtcCodigoCliente->setText("");
    ui->txtcFactura->setText("");
    ui->txtdFecha->setDate(dFecha.currentDate());
    ui->txtdFechaCobro->setDate(dFecha.currentDate());
    ui->txtcCliente->setText("");
    ui->txtcDireccion->setText("");
    ui->txtcDireccion2->setText("");
    ui->txtcCp->setText("");
    ui->txtcPoblacion->setText("");
    ui->txtcProvincia->setText("");
    ui->txtcPais->setText("");
    ui->txtcCif->setText("");
    ui->txtrSubtotal->setText(0);
    ui->txtnDto->setText(0);
    ui->txtnDtoPP->setText(0);
    ui->txtrImporteDescuento->setText("0,00");
    ui->txtrImporteDescuentoPP->setText("0,00");
    ui->txtrBase->setText("0,00");
    ui->txtnIva_2->setText(0);
    ui->txtrImporteIva->setText("0,00");
    ui->txtrTotal->setText("0,00");
    ui->lblFacturaImpresa->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lblContabilizada->setVisible(false);
    int indice=ui->txtcFormaPago->findText("Contado");
    if(indice!=-1) ui->txtcFormaPago->setCurrentIndex(indice);
    ui->txttComentario->setText("");
    ui->txtrBase1->setText(0);
    ui->txtrBase2->setText(0);
    ui->txtrBase3->setText(0);
    ui->txtrBase4->setText(0);
    ui->txtnPorcentajeIva1->setText(QString::number(o_configuracion->nIVA1));
    ui->txtnPorcentajeIva2->setText(QString::number(o_configuracion->nIVA2));
    ui->txtnPorcentajeIva3->setText(QString::number(o_configuracion->nIVA3));
    ui->txtnPorcentajeIva4->setText(QString::number(o_configuracion->nIVA4));
    ui->txtnRec1->clear();
    ui->txtnRec2->clear();
    ui->txtnRec3->clear();
    ui->txtnRec4->clear();
    ui->txtrIVA1->setText(0);
    ui->txtrIVA2->setText(0);
    ui->txtrIVA3->setText(0);
    ui->txtrIVA4->setText(0);
    ui->txtrTotal1->setText(0);
    ui->txtrTotal2->setText(0);
    ui->txtrTotal3->setText(0);
    ui->txtrTotal4->setText(0);
    ui->txtrRecargoEq1->setText(0);
    ui->txtrRecargoEq2->setText(0);
    ui->txtrRecargoEq3->setText(0);
    ui->txtrRecargoEq4->setText(0);
    ui->txtrTotalRecargoEq->setText(0);
    ui->txtrEntregadoaCuenta->setText("0,00");
    ui->txtrImportePendiente->setText("0,00");
    ui->txtcCodigoEntidad->setText("");
    ui->txtcOficinaEntidad->setText("");
    ui->txtcDCCuenta->setText("");
    ui->txtcNumeroCuenta->setText("");
    ui->txtcPedidoCliente->setText("");
    ui->chklRecargoEquivalencia->setCheckState(Qt::Unchecked);
}

void frmFacturas::BloquearCampos()
{
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
    /*
//    // SpinBox
//    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
//    QSpinBox *SpinBox;
//    foreach (SpinBox, SpinBoxList) {
//        SpinBox->setReadOnly(true);
//        //qDebug() << lineEdit->objectName();
//    }
//    // DoubleSpinBox
//    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
//    QDoubleSpinBox *DSpinBox;
//    foreach (DSpinBox, DSpinBoxList) {
//        DSpinBox->setReadOnly(true);
//        //qDebug() << lineEdit->objectName();
//    } */
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> textEditList = this->findChildren<QTextEdit *>();
    QTextEdit *textEdit;
    foreach (textEdit,textEditList) {
        textEdit->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(false);
        //qDebug() << lineEdit->objectName();
    }

    ui->btnAnadir->setEnabled(true);
    ui->btnAnterior->setEnabled(true);
   // ui->btnBorrar->setEnabled(true);
    ui->btnBuscar->setEnabled(true);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);
    //ui->botBorrarLinea->setEnabled(false);
    //ui->botEditarLinea->setEnabled(false);
    ui->botBuscarCliente->setEnabled(false);
}

void frmFacturas::DesbloquearCampos()
{
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
//    // DoubleSpinBox
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
    QList<QTextEdit *> textEditList = this->findChildren<QTextEdit *>();
    QTextEdit *textEdit;
    foreach (textEdit,textEditList) {
        textEdit->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(true);
        //qDebug() << lineEdit->objectName();
    }
    ui->btnAnadir->setEnabled(false);
    ui->btnAnterior->setEnabled(false);
   // ui->btnBorrar->setEnabled(false);
    ui->btnBuscar->setEnabled(false);
    ui->btnDeshacer->setEnabled(true);
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnSiguiente->setEnabled(false);
    //ui->botBorrarLinea->setEnabled(true);
    //ui->botEditarLinea->setEnabled(true);
    ui->botBuscarCliente->setEnabled(true);
}

void frmFacturas::LLenarFactura() {
    oFactura->setcCodigoCliente(ui->txtcCodigoCliente->text());
    oFactura->setcFactura(ui->txtcFactura->text());
    oFactura->setdFecha(ui->txtdFecha->date());
    oFactura->setdFechaCobro(ui->txtdFechaCobro->date());
    oFactura->setcCliente(ui->txtcCliente->text());
    oFactura->setcDireccion(ui->txtcDireccion->text());
    oFactura->setcDireccion2(ui->txtcDireccion2->text());
    oFactura->setcCp(ui->txtcCp->text());
    oFactura->setcPoblacion(ui->txtcPoblacion->text());
    oFactura->setcProvincia(ui->txtcProvincia->text());
    oFactura->setcPais(ui->txtcPais->text());
    oFactura->setcCif(ui->txtcCif->text());
    if (ui->chklRecargoEquivalencia->isChecked()) {
        oFactura->setlRecargoEquivalencia(1);
    } else {
        oFactura->setlRecargoEquivalencia(0);
    }
    oFactura->setrSubtotal(ui->txtrSubtotal->text().replace(".","").toDouble());
    oFactura->setnDto(ui->txtnDto->text().replace(".","").toDouble());
    oFactura->setnDtoPP(ui->txtnDtoPP->text().replace(".","").toDouble());
    oFactura->setrImporteDescuento(ui->txtrImporteDescuento->text().replace(".","").toDouble());
    oFactura->setrImporteDescuentoPP(ui->txtrImporteDescuentoPP->text().replace(".","").toDouble());
    oFactura->setrBase(ui->txtrBase->text().replace(".","").toDouble());
    oFactura->setrImporteIva(ui->txtrImporteIva->text().replace(".","").toDouble());
    oFactura->setrTotal(ui->txtrTotal->text().replace(".","").toDouble());
    oFactura->setcFormaPago(ui->txtcFormaPago->currentText());
    oFactura->settComentario(ui->txttComentario->toPlainText());
    oFactura->setrBase1(ui->txtrBase1->text().replace(".","").toDouble());
    oFactura->setrBase2(ui->txtrBase2->text().replace(".","").toDouble());
    oFactura->setrBase3(ui->txtrBase3->text().replace(".","").toDouble());
    oFactura->setrBase4(ui->txtrBase4->text().replace(".","").toDouble());
    oFactura->setnPorcentajeIVA1(ui->txtnPorcentajeIva1->text().replace(".","").toDouble());
    oFactura->setnPorcentajeIVA2(ui->txtnPorcentajeIva2->text().replace(".","").toDouble());
    oFactura->setnPorcentajeIVA3(ui->txtnPorcentajeIva3->text().replace(".","").toDouble());
    oFactura->setnPorcentajeIVA4(ui->txtnPorcentajeIva4->text().replace(".","").toDouble());
    oFactura->setrIVA1(ui->txtrIVA1->text().replace(".","").toDouble());
    oFactura->setrIVA2(ui->txtrIVA2->text().replace(".","").toDouble());
    oFactura->setrIVA3(ui->txtrIVA3->text().replace(".","").toDouble());
    oFactura->setrIVA4(ui->txtrIVA4->text().replace(".","").toDouble());
    oFactura->setrTotal1(ui->txtrTotal1->text().replace(".","").toDouble());
    oFactura->setrTotal2(ui->txtrTotal2->text().replace(".","").toDouble());
    oFactura->setrTotal3(ui->txtrTotal3->text().replace(".","").toDouble());
    oFactura->setrTotal4(ui->txtrTotal4->text().replace(".","").toDouble());
    oFactura->setnRec1(ui->txtnRec1->text().replace(".","").toDouble());
    oFactura->setnRec2(ui->txtnRec2->text().replace(".","").toDouble());
    oFactura->setnRec3(ui->txtnRec3->text().replace(".","").toDouble());
    oFactura->setnRec4(ui->txtnRec4->text().replace(".","").toDouble());
    oFactura->setrRecargoEq1(ui->txtrRecargoEq1->text().replace(".","").toDouble());
    oFactura->setrRecargoEq2(ui->txtrRecargoEq2->text().replace(".","").toDouble());
    oFactura->setrRecargoEq3(ui->txtrRecargoEq3->text().replace(".","").toDouble());
    oFactura->setrRecargoEq4(ui->txtrRecargoEq4->text().replace(".","").toDouble());
    oFactura->setrTotalRecargoEq(ui->txtrTotalRecargoEq->text().replace(".","").toDouble());
    oFactura->setrEntregadoaCuenta(ui->txtrEntregadoaCuenta->text().replace(".","").toDouble());
    oFactura->setrImportePendiente(ui->txtrImportePendiente->text().replace(".","").toDouble());
    oFactura->setcCodigoEntidad(ui->txtcCodigoEntidad->text());
    oFactura->setcOficinaEntidad(ui->txtcOficinaEntidad->text());
    oFactura->setcDCCuenta(ui->txtcDCCuenta->text());
    oFactura->setcNumeroCuenta(ui->txtcNumeroCuenta->text());
    oFactura->setcPedidoCliente(ui->txtcPedidoCliente->text());
    oFactura->setnIRPF(ui->txtnIRPF->text().replace(".","").toDouble());
    oFactura->setrImporteIRPF(ui->txtrImporteIRPF->text().replace(".","").toDouble());
}

void frmFacturas::on_btnSiguiente_clicked()
{
    QString cFactura = oFactura->getcFactura();
    oFactura->RecuperarFactura("Select * from cab_fac where cFactura >'"+cFactura+"' order by cFactura  limit 1 ");
    LLenarCampos();
    //delete cFactura;
}
void frmFacturas::on_btnAnterior_clicked()
{
    QString cFactura = oFactura->getcFactura();
    oFactura->RecuperarFactura("Select * from cab_fac where cFactura <'"+cFactura+"' order by cFactura desc limit 1 ");
    LLenarCampos();
    //delete cFactura;
}


void frmFacturas::on_btnGuardar_clicked()
{
    int nId = oFactura->Getid();
    if (ui->txtcFactura->text() == tr("BORRADOR"))
        ui->txtcFactura->setText(oFactura->NuevoNumeroFactura());
    LLenarFactura();
    BloquearCampos();
    oFactura->GuardarFactura(nId,true);
    oFactura->RecuperarFactura("Select * from cab_fac where Id="+QString::number(nId)+"  limit 1 ");
    LLenarCampos();
}

void frmFacturas::on_btnAnadir_clicked()
{
    VaciarCampos();
    LLenarFactura();
    oFactura->setcFactura("BORRADOR");
    oFactura->AnadirFactura();

    LLenarCampos();
    DesbloquearCampos();
    ui->chklRecargoEquivalencia->setCheckState(Qt::Unchecked);

    ui->txtcCodigoCliente->setFocus();

}

void frmFacturas::calcularTotalLinea()
{
    /*
    // Calculo totales línea
    double impDto,impTot,impSubtotal;
    impSubtotal = (ui->txtcCantidadArticulo->text().replace(".","").toDouble() * ui->txtPVPArticulo->text().replace(".","").toDouble());
    ui->txtSubtotalArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impSubtotal,'f',2)));
    impDto = (ui->txtSubtotalArticulo->text().replace(".","").toDouble() * ui->txtPorcDtoArticulo->text().replace(".","").toDouble())/100 ;
    ui->txtDtoArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impDto,'f',2)));
    impTot = ui->txtSubtotalArticulo->text().replace(".","").toDouble() - ui->txtDtoArticulo->text().replace(".","").toDouble();
    ui->txtTotalArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impTot,'f',2)));
    */
}

void frmFacturas::on_btnDeshacer_clicked()
{
    BloquearCampos();
    QString cId = QString::number(oFactura->Getid());
    oFactura->RecuperarFactura("Select * from cab_fac where id ="+cId+" order by id limit 1 ");
    LLenarCampos();
}

void frmFacturas::on_botBuscarCliente_clicked()
{
    FrmBuscarCliente BuscarClientes;
    BuscarClientes.exec();
    int nId = BuscarClientes.DevolverID();
    //qDebug() << nId;
    QString cId = QString::number(nId);
    oFactura->setiId_Cliente(nId);
    oCliente1->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
    LLenarCamposCliente();
}

void frmFacturas::on_btnBuscarArt_clicked()
{
    if(!ui->txtcTextoBuscar->text().isEmpty()) {
        modArt = new QSqlQueryModel();
        QString cSQL = "select cCodigo,cDescripcion,rTarifa1,rTarifa2,rTarifa3 from articulos where cDescripcion like '%" +
                ui->txtcTextoBuscar->text().trimmed()+"%'";
        modArt->setQuery(cSQL,QSqlDatabase::database("empresa"));
        ui->tablaBuscaArt->setModel(modArt);
       // ui->tablaBuscaArt->setItemDelegateForColumn(2, Columna);
       // ui->tablaBuscaArt->setItemDelegateForColumn(3, Columna);
       // ui->tablaBuscaArt->setItemDelegateForColumn(4, Columna);
    } else {
        QMessageBox::critical(this,tr("Buscar Artículos"),tr("Debe especificar algún criterio de búsqueda"),tr("OK"));
    }
    //delete modArt;

}


void frmFacturas::on_tablaBuscaArt_doubleClicked(const QModelIndex &index)
{
    /*
    QString texto = index.model()->data(index, Qt::DisplayRole).toString();
    ui->btnEditar->click();
    ui->txtcCodigoArticulo->setText(texto);
    ui->txtcCodigoArticulo->setFocus();
    */
}

void frmFacturas::on_chklRecargoEquivalencia_stateChanged(int arg1)
{
    if(arg1==2){
        ui->txtnRec1->setText(QString::number(o_configuracion->nRE1,'f',1));
        ui->txtnRec2->setText(QString::number(o_configuracion->nRE2,'f',1));
        ui->txtnRec3->setText(QString::number(o_configuracion->nRE3,'f',1));
        ui->txtnRec4->setText(QString::number(o_configuracion->nRE4,'f',1));
    } else {
        ui->txtnRec1->setText("0");
        ui->txtnRec2->setText("0");
        ui->txtnRec3->setText("0");
        ui->txtnRec4->setText("0");
    }
}

void frmFacturas::on_btnBuscar_clicked()
{
    FrmBuscarFactura BuscarFactura;
    BuscarFactura.exec();
    int nId = BuscarFactura.DevolverID();
    //qDebug() << nId;
    QString cId = QString::number(nId);
    oFactura->RecuperarFactura("Select * from cab_fac where Id ="+cId+" limit 1 ");
    LLenarCampos();
}

void frmFacturas::on_txtcCodigoCliente_lostFocus()
{
    if (!ui->txtcCodigoCliente->text().isEmpty() && ui->txtcCliente->text().isEmpty()) {
        QString cSQL = "Select * from clientes where cCodigoCliente = '"+ui->txtcCodigoCliente->text().trimmed() +"' limit 1";
        oCliente1->Recuperar(cSQL);
        LLenarCamposCliente();
        //ui->txtcCodigoArticulo->setFocus();
    }
}

void frmFacturas::on_btnImprimir_clicked()
{
   // KDReports::Report report;
    //TODO Reports
}

void frmFacturas::RellenarDespuesCalculo()
{
    ui->txtrSubtotal->setText(o_configuracion->FormatoNumerico( QString::number(oFactura->getrSubtotal(),'f',2)));
    ui->txtrImporteDescuento->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteDescuento(),'f',2)));
    ui->txtrImporteDescuentoPP->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteDescuentoPP(),'f',2)));
    ui->txtrBase->setText(o_configuracion->FormatoNumerico(QString::number( oFactura->getrBase(),'f',2)));
    ui->txtnIva_2->setText(o_configuracion->FormatoNumerico(QString::number( oFactura->getnIva(),'f',2)));
    ui->txtrImporteIva->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteIva(),'f',2)));
    ui->txtrTotal->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal(),'f',2)));
    ui->txtrBase1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase1(),'f',2)));
    ui->txtrBase2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase2(),'f',2)));
    ui->txtrBase3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase3(),'f',2)));
    ui->txtrBase4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrBase4(),'f',2)));
    ui->txtrIVA1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA1(),'f',2)));
    ui->txtrIVA2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA2(),'f',2)));
    ui->txtrIVA3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA3(),'f',2)));
    ui->txtrIVA4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrIVA4(),'f',2)));
    ui->txtrTotal1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal1(),'f',2)));
    ui->txtrTotal2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal2(),'f',2)));
    ui->txtrTotal3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal3(),'f',2)));
    ui->txtrTotal4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotal4(),'f',2)));
    ui->txtrRecargoEq1->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq1(),'f',2)));
    ui->txtrRecargoEq2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq2(),'f',2)));
    ui->txtrRecargoEq3->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq3(),'f',2)));
    ui->txtrRecargoEq4->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrRecargoEq4(),'f',2)));
    ui->txtrTotalRecargoEq->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrTotalRecargoEq(),'f',2)));
    ui->txtrImporteIRPF->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteIRPF(),'f',2)));
    ui->txtrImporteIRPF_2->setText(o_configuracion->FormatoNumerico(QString::number(oFactura->getrImporteIRPF(),'f',2)));
}

void frmFacturas::on_botBorrador_clicked()
{
    int nId = oFactura->Getid();
    LLenarFactura();
    BloquearCampos();
    oFactura->setcFactura("BORRADOR");
    oFactura->GuardarFactura(nId,false);
    oFactura->RecuperarFactura("Select * from cab_fac where Id="+QString::number(nId)+"  limit 1 ");
    LLenarCampos();
}

void frmFacturas::totalChanged(QString total)
{
    ui->lbl_total->setText(total);
}
