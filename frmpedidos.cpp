#include "frmpedidos.h"
#include "ui_frmpedidos.h"
#include "pedidos.h"
#include "cliente.h"
#include "columnamoneda.h"
#include "configuracion.h"
#include "articulo.h"
#include "frmmodificarlin_ped.h"
#include "frmbuscarcliente.h"

Pedidos *oPedido = new Pedidos();
Cliente *oCliente3 = new Cliente();
ColumnaMoneda *Columna2 = new ColumnaMoneda();

FrmPedidos::FrmPedidos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmPedidos)
{
    ui->setupUi(this);
    // Pongo valores por defecto
    ui->lblFacturado->setVisible(false);
    ui->lblImpreso->setVisible(false);
    ui->lblNumFactura->setVisible(false);
    ui->txtcNumFra->setVisible(false);
    // valores edicion
    ui->txtcCodigoArticulo->setFocus();
    Configuracion *o_conf = new Configuracion();
    o_configuracion = o_conf;
    BloquearCampos();
}

FrmPedidos::~FrmPedidos()
{
    delete ui;
}

void FrmPedidos::lineasVentas()
{
    // lineas de ventas
    QString cSQL;
    QString cId;

    cId = cId.number(oPedido->Getid());
     cSQL ="select id,cCodigo,nCantidad,cDescripcion,rPvp,rSubtotal,nDto,rDto,rTotal, nPorcIva from lin_ped "
             " where Id_Cab = "+cId;
     ModelLin_ped = new QSqlQueryModel();
     ModelLin_ped->setQuery(cSQL,QSqlDatabase::database("empresa"));
     ui->Lineas->setModel(ModelLin_ped);
      //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
     QHeaderView *Cabecera = new QHeaderView(Qt::Horizontal,this);
     // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
     ui->Lineas->setHorizontalHeader(Cabecera);
     /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0". (en nuestro caso está oculta ya que muestra el id de la tabla y esto no nos interesa que lo vea el usuario */
     Cabecera->setResizeMode(0,QHeaderView::Fixed);
     Cabecera->resizeSection(0,0);
     Cabecera->setResizeMode(1,QHeaderView::Fixed);
     Cabecera->resizeSection(1,85);
     Cabecera->setResizeMode(2,QHeaderView::Fixed);
     Cabecera->resizeSection(2,65);
     Cabecera->setResizeMode(3,QHeaderView::Fixed);
     Cabecera->resizeSection(3,413);
     Cabecera->setResizeMode(4,QHeaderView::Fixed);
     Cabecera->resizeSection(4,120);
     Cabecera->setResizeMode(9,QHeaderView::Fixed);
     Cabecera->resizeSection(9,35);
     ModelLin_ped->setHeaderData(1, Qt::Horizontal, QObject::tr("CÓDIGO"));
     ModelLin_ped->setHeaderData(2, Qt::Horizontal, QObject::tr("CANTIDAD"));
     ModelLin_ped->setHeaderData(3, Qt::Horizontal, QObject::tr("DESCRIPCIÓN"));
     ModelLin_ped->setHeaderData(4, Qt::Horizontal, QObject::tr("PVP"));
     ModelLin_ped->setHeaderData(5, Qt::Horizontal, QObject::tr("SUBTOTAL"));
     ModelLin_ped->setHeaderData(6, Qt::Horizontal, QObject::tr("%DTO"));
     ModelLin_ped->setHeaderData(7, Qt::Horizontal, QObject::tr("DTO"));
     ModelLin_ped->setHeaderData(8, Qt::Horizontal, QObject::tr("BASE"));
     ModelLin_ped->setHeaderData(9, Qt::Horizontal, QObject::tr("%IVA"));

    // Hacemos visible la cabecera
     Cabecera->setVisible(true);
     // Delegamos el control contenido en las columnas al nuevo objeto ColumnaGrid
     ui->Lineas->setItemDelegateForColumn(4, Columna2);
     ui->Lineas->setItemDelegateForColumn(5, Columna2);
     ui->Lineas->setItemDelegateForColumn(6, Columna2);
     ui->Lineas->setItemDelegateForColumn(7, Columna2);
     ui->Lineas->setItemDelegateForColumn(8, Columna2);
}

void FrmPedidos::LLenarCampos()
{
    int lEstado;

    ui->txtcCodigoCliente->setText(oPedido->getcCodigoCliente());
    ui->txtcPedido->setText(oPedido->getcPedido());
    ui->txtnPedido->setText(QString::number(oPedido->getnPedido()));
    ui->txtdFecha->setDate(oPedido->getdFecha());
    ui->txtdFechaFactura->setDate(oPedido->getdFechaFactura());
    ui->txtcCliente->setText(oPedido->getcCliente());
    ui->txtcDireccion->setText(oPedido->getcDireccion());
    ui->txtcDireccion2->setText(oPedido->getcDireccion2());
    ui->txtcCp->setText(oPedido->getcCp());
    ui->txtcPoblacion->setText(oPedido->getcPoblacion());
    ui->txtcProvincia->setText(oPedido->getcProvincia());
    ui->txtcPais->setText(oPedido->getcPais());
    ui->txtcCif->setText(oPedido->getcCif());
    ui->txtrSubtotal->setText(o_configuracion->FormatoNumerico( QString::number(oPedido->getrSubtotal(),'f',2)));
    ui->txtnDto->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getnDto(),'f',2)));
    ui->txtrImporteDescuento->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrImporteDescuento(),'f',2)));
    ui->txtrBase->setText(o_configuracion->FormatoNumerico(QString::number( oPedido->getrBase(),'f',2)));
    ui->txtrBaseTotal_2->setText(o_configuracion->FormatoNumerico(QString::number( oPedido->getrBase(),'f',2)));

    ui->txtrImporteIva->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrImporteIva(),'f',2)));
    ui->txtrTotal->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrTotal(),'f',2)));
    ui->txtrTotal_2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrTotal(),'f',2)));
    ui->txtrTotalRecargoEq_2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrRecargoEqTotal(),'f',2)));
    lEstado = oPedido->getlImpreso();
    if((lEstado == 1)) {
        ui->lblImpreso->setVisible(true);
    } else {
        ui->lblImpreso->setVisible(false);
    }
    lEstado = oPedido->getlFacturado();
    if ((lEstado == 1)) {
        ui->lblFacturado->setVisible(true);
        ui->txtdFechaFactura->setVisible(true);
        ui->txtcNumFra->setVisible(true);
    } else {
        ui->lblFacturado->setVisible(false);
        ui->txtdFechaFactura->setVisible(false);
        ui->txtcNumFra->setVisible(false);

    }
    ui->txttComentario->setPlainText(oPedido->gettComentario());
    ui->txtrBase1->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrBase1(),'f',2)));
    ui->txtrBase2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrBase2(),'f',2)));
    ui->txtrBase3->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrBase3(),'f',2)));
    ui->txtrBase4->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrBase4(),'f',2)));
    ui->txtnPorcentajeIva1->setText(QString::number(oPedido->getnPorcentajeIVA1()));
    ui->txtnPorcentajeIva2->setText(QString::number(oPedido->getnPorcentajeIVA2()));
    ui->txtnPorcentajeIva3->setText(QString::number(oPedido->getnPorcentajeIVA3()));
    ui->txtnPorcentajeIva4->setText(QString::number(oPedido->getnPorcentajeIVA4()));
    ui->txtrIVA1->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrIVA1(),'f',2)));
    ui->txtrIVA2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrIVA2(),'f',2)));
    ui->txtrIVA3->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrIVA3(),'f',2)));
    ui->txtrIVA4->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrIVA4(),'f',2)));
    ui->txtrTotal1->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrTotal1(),'f',2)));
    ui->txtrTotal2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrTotal2(),'f',2)));
    ui->txtrTotal3->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrTotal3(),'f',2)));
    ui->txtrTotal4->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrTotal4(),'f',2)));
    ui->txtnRec1->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getnRec1(),'f',2)));
    ui->txtnRec2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getnRec2(),'f',2)));
    ui->txtnRec3->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getnRec3(),'f',2)));
    ui->txtnRec4->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getnRec4(),'f',2)));
    ui->txtrRecargoEq1->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrRecargoEq1(),'f',2)));
    ui->txtrRecargoEq2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrRecargoEq2(),'f',2)));
    ui->txtrRecargoEq3->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrRecargoEq3(),'f',2)));
    ui->txtrRecargoEq4->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrRecargoEq4(),'f',2)));
    ui->txtrTotalRecargoEq->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrRecargoEqTotal(),'f',2)));
    ui->txtrTotalIVA_2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrImporteIva(),'f',2)));
    ui->txtcPedidoCliente->setText(oPedido->getcPedido());
    if(oPedido->getlRecargoEquivalencia()==1)
        ui->chklRecargoEq->setChecked(true);
    else
        ui->chklRecargoEq->setChecked(false);

    if (oPedido->getlTraspasadoAlb() ==1)
        ui->lblAlbaran->setVisible(true);
    else
        ui->lblAlbaran->setVisible(false);

    if (oPedido->getlTraspasadoFac()==1)
        ui->lblFacturado->setVisible(true);
    else
        ui->lblFacturado->setVisible(false);

    ui->txtcDireccionEntrega->setText(oPedido->getcDireccionEntrega());
    ui->txtcDireccionEntrega_2->setText(oPedido->getcDireccionEntrega2());
    ui->txtcCpEntrega->setText(oPedido->getcCPEntrega());
    ui->txtcPoblacionEntrega->setText(oPedido->getcPoblacionEntrega());
    ui->txtcProvinciaEntrega->setText(oPedido->getcProvinciaEntrega());
    ui->txtcPaisEntrega->setText(oPedido->getcPaisEntrega());

    if (oPedido->getlEnviado() == 1)
        ui->chklEnviado->setChecked(true);
    else
        ui->chklEnviado->setChecked(false);
    if (oPedido->getlCompleto() == 1)
        ui->chklCompleto->setChecked(true);
    else
        ui->chklCompleto->setChecked(false);
    if (oPedido->getlEntregado() ==1)
        ui->chklEntregado->setChecked(true);
    else
        ui->chklEntregado->setChecked(false);

    ui->txtdFechaLimiteEntrega->setDate(oPedido->getdFechaLimiteEntrega());
    // cargamos líneas de ventas
   lineasVentas();


}

void FrmPedidos::LLenarCamposCliente()
{
    ui->txtcCodigoCliente->setText(oCliente3->getcCodigoCliente());
    ui->txtcCliente->setText(oCliente3->getcNombreFiscal());
    ui->txtcDireccion->setText(oCliente3->getcDireccion1());
    ui->txtcDireccion2->setText(oCliente3->getcDireccion2());
    ui->txtcCp->setText(oCliente3->getcCP());
    ui->txtcPoblacion->setText(oCliente3->getcPoblacion());
    ui->txtcProvincia->setText(oCliente3->getcProvincia());
    ui->txtcPais->setText(oCliente3->getcPais());
    ui->txtcCif->setText(oCliente3->getcCifNif());
    if (oCliente3->getlIRPF()==1) {
        ui->chklRecargoEq->setChecked(true);
        oPedido->setlRecargoEquivalencia(1);
    } else {
        ui->chklRecargoEq->setChecked(false);
        oPedido->setlRecargoEquivalencia(0);
    }
}

void FrmPedidos::VaciarCampos()
{
    QDate dFecha;
    ui->txtcCodigoCliente->setText("");
    ui->txtcPedido->setText("");
    ui->txtdFecha->setDate(dFecha.currentDate());
    ui->txtdFechaFactura->setDate(dFecha.currentDate());
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
    ui->txtrImporteDescuento->setText("0,00");
    ui->txtrBase->setText("0,00");
    ui->txtrImporteIva->setText("0,00");
    ui->txtrTotal->setText("0,00");
    ui->lblImpreso->setVisible(false);
    ui->lblFacturado->setVisible(false);
    ui->lblNumFactura->setVisible(false);
    ui->txtcNumFra->setVisible(false);
    ui->txttComentario->setText("");
    ui->txtrBase1->setText(0);
    ui->txtrBase2->setText(0);
    ui->txtrBase3->setText(0);
    ui->txtrBase4->setText(0);
    o_configuracion->CargarDatos();
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
    ui->txtrACuenta->setText("0,00");
    ui->txtcPedidoCliente->setText("");
    ui->txtrTotalIVA_2->setText("0,00");
    ui->txtrBaseTotal_2->setText("0,00");
    ui->txtrTotalRecargoEq_2->setText("0,00");
    ui->txtrTotal_2->setText("0,00");
    ui->txtrSubtotal->setText("0,00");
}

void FrmPedidos::BloquearCampos()
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
    ui->btnCerrar->setEnabled(true);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);
    ui->botBorrarLinea->setEnabled(false);
    ui->botEditarLinea->setEnabled(false);
    ui->botBuscarCliente->setEnabled(false);
   // ui->btnFacturar->setEnabled(false);
}

void FrmPedidos::DesbloquearCampos()
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
    ui->btnCerrar->setEnabled(false);
    ui->btnDeshacer->setEnabled(true);
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnSiguiente->setEnabled(false);
    ui->botBorrarLinea->setEnabled(true);
    ui->botEditarLinea->setEnabled(true);
    ui->botBuscarCliente->setEnabled(true);
    //ui->btnFacturar->setEnabled(true);
}

void FrmPedidos::LLenarPedido()
{
    oPedido->setcCodigoCliente(ui->txtcCodigoCliente->text());
    oPedido->setnPedido(ui->txtnPedido->text().toInt());
    oPedido->setdFecha(ui->txtdFecha->date());
    oPedido->setdFechaFactura(ui->txtdFechaFactura->date());
    oPedido->setcCliente(ui->txtcCliente->text());
    oPedido->setcDireccion(ui->txtcDireccion->text());
    oPedido->setcDireccion2(ui->txtcDireccion2->text());
    oPedido->setcCp(ui->txtcCp->text());
    oPedido->setcPoblacion(ui->txtcPoblacion->text());
    oPedido->setcProvincia(ui->txtcProvincia->text());
    oPedido->setcPais(ui->txtcPais->text());
    oPedido->setcCif(ui->txtcCif->text());
    oPedido->setrSubtotal(ui->txtrSubtotal->text().replace(".","").toDouble());
    oPedido->setnDto(ui->txtnDto->text().replace(".","").toDouble());
    oPedido->setrImporteDescuento(ui->txtrImporteDescuento->text().replace(".","").toDouble());
    oPedido->setrBase(ui->txtrBase->text().replace(".","").toDouble());
    oPedido->setrImporteIva(ui->txtrImporteIva->text().replace(".","").toDouble());
    oPedido->setrTotal(ui->txtrTotal->text().replace(".","").toDouble());
    oPedido->settComentario(ui->txttComentario->toPlainText());
    oPedido->setrBase1(ui->txtrBase1->text().replace(".","").toDouble());
    oPedido->setrBase2(ui->txtrBase2->text().replace(".","").toDouble());
    oPedido->setrBase3(ui->txtrBase3->text().replace(".","").toDouble());
    oPedido->setrBase4(ui->txtrBase4->text().replace(".","").toDouble());
    oPedido->setnPorcentajeIVA1(ui->txtnPorcentajeIva1->text().replace(".","").toDouble());
    oPedido->setnPorcentajeIVA2(ui->txtnPorcentajeIva2->text().replace(".","").toDouble());
    oPedido->setnPorcentajeIVA3(ui->txtnPorcentajeIva3->text().replace(".","").toDouble());
    oPedido->setnPorcentajeIVA4(ui->txtnPorcentajeIva4->text().replace(".","").toDouble());
    oPedido->setrIVA1(ui->txtrIVA1->text().replace(".","").toDouble());
    oPedido->setrIVA2(ui->txtrIVA2->text().replace(".","").toDouble());
    oPedido->setrIVA3(ui->txtrIVA3->text().replace(".","").toDouble());
    oPedido->setrIVA4(ui->txtrIVA4->text().replace(".","").toDouble());
    oPedido->setrTotal1(ui->txtrTotal1->text().replace(".","").toDouble());
    oPedido->setrTotal2(ui->txtrTotal2->text().replace(".","").toDouble());
    oPedido->setrTotal3(ui->txtrTotal3->text().replace(".","").toDouble());
    oPedido->setrTotal4(ui->txtrTotal4->text().replace(".","").toDouble());
    oPedido->setnRec1(ui->txtnRec1->text().replace(".","").toDouble());
    oPedido->setnRec2(ui->txtnRec2->text().replace(".","").toDouble());
    oPedido->setnRec3(ui->txtnRec3->text().replace(".","").toDouble());
    oPedido->setnRec4(ui->txtnRec4->text().replace(".","").toDouble());
    oPedido->setrRecargoEq1(ui->txtrRecargoEq1->text().replace(".","").toDouble());
    oPedido->setrRecargoEq2(ui->txtrRecargoEq2->text().replace(".","").toDouble());
    oPedido->setrRecargoEq3(ui->txtrRecargoEq3->text().replace(".","").toDouble());
    oPedido->setrRecargoEq4(ui->txtrRecargoEq4->text().replace(".","").toDouble());
    oPedido->setrRecargoEqTotal(ui->txtrTotalRecargoEq->text().replace(".","").toDouble());
    if (ui->chklRecargoEq->isChecked())
        oPedido->setlRecargoEquivalencia(1);
    else
        oPedido->setlRecargoEquivalencia(0);

    oPedido->setcPedido(ui->txtcPedidoCliente->text());

    if(ui->lblImpreso->isVisible())
        oPedido->setlImpreso(1);
    else
        oPedido->setlImpreso(0);
    if (ui->lblFacturado->isVisible())
        oPedido->setlFacturado(1);
    else
        oPedido->setlFacturado(0);
    oPedido->setcFactura(ui->txtcNumFra->text());
    oPedido->setdFechaFactura(ui->txtdFechaFactura->date());
    oPedido->setrACuenta(ui->txtrACuenta->text().replace(".","").toDouble());

    if (ui->lblAlbaran->isVisible())
        oPedido->setlTraspasadoAlb(1);
    else
        oPedido->setlTraspasadoAlb(0);

    if(ui->lblFacturado->isVisible())
        oPedido->setlTraspasadoFac(1);
    else
        oPedido->setlTraspasadoFac(0);
    oPedido->setcDireccionEntrega(ui->txtcDireccionEntrega->text());
    oPedido->setcDireccionEntrega2(ui->txtcDireccionEntrega_2->text());
    oPedido->setcCPEntrega(ui->txtcCpEntrega->text());
    oPedido->setcPoblacionEntrega(ui->txtcPoblacionEntrega->text());
    oPedido->setcProvinciaEntrega(ui->txtcProvinciaEntrega->text());
    oPedido->setcPaisEntrega(ui->txtcPaisEntrega->text());
    if (ui->chklEnviado->isChecked())
        oPedido->setlEnviado(1);
    else
        oPedido->setlEnviado(0);
    if (ui->chklCompleto->isChecked())
        oPedido->setlCompleto(1);
    else
        oPedido->setlCompleto(0);
    if(ui->chklEntregado->isChecked())
        oPedido->setlEntregado(1);
    else
        oPedido->setlEntregado(0);
    oPedido->setdFechaLimiteEntrega(ui->txtdFechaLimiteEntrega->date());
}

void FrmPedidos::CalcularTotalLinea()
{
    // Calculo totales línea
    double impDto,impTot,impSubtotal;
    impSubtotal = (ui->txtcCantidadArticulo->text().replace(".","").toDouble() * ui->txtPVPArticulo->text().replace(".","").toDouble());
    ui->txtSubtotalArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impSubtotal,'f',2)));
    impDto = (ui->txtSubtotalArticulo->text().replace(".","").toDouble() * ui->txtPorcDtoArticulo->text().replace(".","").toDouble())/100 ;
    ui->txtDtoArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impDto,'f',2)));
    impTot = ui->txtSubtotalArticulo->text().replace(".","").toDouble() - ui->txtDtoArticulo->text().replace(".","").toDouble();
    ui->txtTotalArticulo->setText(o_configuracion->FormatoNumerico(QString::number(impTot,'f',2)));
}

void FrmPedidos::RellenarDespuesCalculo()
{
    ui->txtrSubtotal->setText(o_configuracion->FormatoNumerico( QString::number(oPedido->getrSubtotal(),'f',2)));
    ui->txtrImporteDescuento->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrImporteDescuento(),'f',2)));
    ui->txtrBase->setText(o_configuracion->FormatoNumerico(QString::number( oPedido->getrBase(),'f',2)));
    ui->txtrImporteIva->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrImporteIva(),'f',2)));
    ui->txtrTotal->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrTotal(),'f',2)));

    ui->txtrBase1->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrBase1(),'f',2)));
    ui->txtrBase2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrBase2(),'f',2)));
    ui->txtrBase3->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrBase3(),'f',2)));
    ui->txtrBase4->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrBase4(),'f',2)));
    ui->txtrIVA1->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrIVA1(),'f',2)));
    ui->txtrIVA2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrIVA2(),'f',2)));
    ui->txtrIVA3->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrIVA3(),'f',2)));
    ui->txtrIVA4->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrIVA4(),'f',2)));
    ui->txtrTotal1->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrTotal1(),'f',2)));
    ui->txtrTotal2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrTotal2(),'f',2)));
    ui->txtrTotal3->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrTotal3(),'f',2)));
    ui->txtrTotal4->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrTotal4(),'f',2)));
    ui->txtrRecargoEq1->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrRecargoEq1(),'f',2)));
    ui->txtrRecargoEq2->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrRecargoEq2(),'f',2)));
    ui->txtrRecargoEq3->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrRecargoEq3(),'f',2)));
    ui->txtrRecargoEq4->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrRecargoEq4(),'f',2)));
    ui->txtrTotalRecargoEq->setText(o_configuracion->FormatoNumerico(QString::number(oPedido->getrRecargoEqTotal(),'f',2)));
}



void FrmPedidos::on_btnSiguiente_clicked()
{
    int nPedido = oPedido->getnPedido();
    oPedido->RecuperarPedido("Select * from ped_cli where nPedido >'"+QString::number(nPedido)+"' order by nPedido  limit 1 ");
    LLenarCampos();
}

void FrmPedidos::on_btnAnterior_clicked()
{
    int nPedido = oPedido->getnPedido();
    oPedido->RecuperarPedido("Select * from ped_cli where nPedido <'"+QString::number(nPedido)+"' order by nPedido desc limit 1 ");
    LLenarCampos();
}

void FrmPedidos::on_btnAnadir_clicked()
{
    VaciarCampos();
    LLenarPedido();
    oPedido->AnadirPedido();
    LLenarCampos();
    DesbloquearCampos();
    ui->txtcCodigoCliente->setFocus();
}



void FrmPedidos::on_btnEditar_clicked()
{
    if (!oPedido->getlFacturado()) {
        DesbloquearCampos();
    } else {
        QMessageBox::warning(NULL,tr("Editar Pedido"),tr("No se puede editar un pedido que ha sido facturado, solo los NO facturados se pueden editar")+
                             tr("<p><b> Si necesita modificar algo genere una factura nueva y realice el abono correspondiente</b>")+
                                tr("y luego si es preciso realice un nuevo pedido y facture de nuevo "),tr("OK"));
    }
}

void FrmPedidos::on_btnGuardar_clicked()
{
    int nId = oPedido->Getid();
    if (ui->txtnPedido->text().trimmed() =="0")
        ui->txtnPedido->setText(QString::number(oPedido->NuevoNumeroPedido()));
    LLenarPedido();
    BloquearCampos();
    oPedido->GuardarPedido(nId);
    oPedido->RecuperarPedido("Select * from ped_cli where id="+QString::number(nId)+"  limit 1 ");
    LLenarCampos();
}

void FrmPedidos::on_botBuscarCliente_clicked()
{
    FrmBuscarCliente BuscarClientes;
    BuscarClientes.exec();
    int nId = BuscarClientes.DevolverID();
    QString cId = QString::number(nId);
    oPedido->setiId_Cliente(cId.toInt());
    oCliente3->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
    LLenarCamposCliente();
}

void FrmPedidos::on_txtcCodigoArticulo_editingFinished()
{
    if (!ui->txtcCodigoArticulo->text().isEmpty()) {
        if (ui->txtDescripcionArticulo->text().isEmpty()) {
            Articulo *oArt =  new Articulo();
            oArt->Recuperar("Select * from articulos where cCodigo = '"+ui->txtcCodigoArticulo->text()+"'");
            ui->btnAnadirLinea->setToolTip("Código: "+ oArt->getcCodigo()+"<br>Descripción: "+oArt->getcDescripcion()+
                                           "<br><b>Stock:</b><font color = 'red'>"+QString::number(oArt->getnStockReal())+"</color>");
            ui->txtcCodigoArticulo->setText(oArt->getcCodigo());
            ui->txtDescripcionArticulo->setText(oArt->getcDescripcion());
            ui->txtPVPArticulo->setText(o_configuracion->FormatoNumerico(QString::number(oArt->getrTarifa1(),'f',2)));
            ui->txtcCantidadArticulo->setText("1");
            ui->txtSubtotalArticulo->setText(o_configuracion->FormatoNumerico(QString::number(oArt->getrTarifa1(),'f',2)));
            // Recupero datos cliente para determinar descuento en factura
            oCliente3->Recuperar("select * from clientes where id="+ QString::number(oPedido->getiId_Cliente()) );
            ui->txtPorcDtoArticulo->setText(QString::number(oCliente3->getnPorcDtoCliente(),'f',0));
            // Asigno el descuento mayor seleccionando entre dto ficha artículo y descuento ficha cliente
            if (oArt->getrDto() > oCliente3->getnPorcDtoCliente()) {
                ui->txtPorcDtoArticulo->setText(o_configuracion->FormatoNumerico(QString::number(oArt->getrDto(),'f',0)));
            }
            ui->txtPorcIVAArticulo->setText(QString::number(oArt->getnTipoIva(),'f',0));

        }
        CalcularTotalLinea();
    }

}

void FrmPedidos::on_btnAnadirLinea_clicked()
{
    if (!ui->txtDescripcionArticulo->text().isEmpty()) {
        double pvp =ui->txtPVPArticulo->text().replace(".","").toDouble();
        oPedido->AnadirLineaPedido(oPedido->Getid(),ui->txtcCodigoArticulo->text(),ui->txtcCantidadArticulo->text().replace(".","").toDouble(),
                                     ui->txtDescripcionArticulo->text(),ui->txtPVPArticulo->text().replace(".","").toDouble(),
                                     ui->txtSubtotalArticulo->text().replace(".","").toDouble(),ui->txtPorcDtoArticulo->text().replace(".","").toDouble(),
                                     ui->txtDtoArticulo->text().replace(".","").toDouble(),ui->txtTotalArticulo->text().replace(".","").toDouble(),
                                     ui->txtPorcIVAArticulo->text().replace(".","").toDouble());
        ui->txtcCodigoArticulo->setText("");
        ui->txtcCantidadArticulo->setText(0);
        ui->txtDescripcionArticulo->setText("");
        ui->txtPVPArticulo->setText(0);
        ui->txtSubtotalArticulo->setText(0);
        ui->txtPorcDtoArticulo->setText(0);
        ui->txtDtoArticulo->setText(0);
        ui->txtTotalArticulo->setText(0);
        ui->txtPorcIVAArticulo->setText(0);
        ui->txtcCodigoArticulo->setFocus();
    } else {
        QMessageBox::critical(NULL,tr("Insertar Línea"),tr("Debe especificar un artículo y una cantidad"),tr("&Aceptar"));
        ui->txtcCodigoArticulo->setFocus();
    }
    lineasVentas();
    // Calculo totales Albaran
    oPedido->calcularPedido();
    RellenarDespuesCalculo();
}

void FrmPedidos::on_botEditarLinea_clicked()
{
    QModelIndex celda=ui->Lineas->currentIndex();
    QModelIndex index= ModelLin_ped->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=ModelLin_ped->data(index,Qt::EditRole);
    int Id_lin =  pKey.toInt();
    FrmModificarLin_ped *Modificar = new FrmModificarLin_ped();
    Modificar->PonerCampos(Id_lin);
    if (Modificar->exec() == QDialog::Accepted)

    {
        lineasVentas();
        oPedido->calcularPedido();
        RellenarDespuesCalculo();
    }
    delete Modificar;
}


void FrmPedidos::on_btnDeshacer_clicked()
{
    BloquearCampos();
    QString cId = QString::number(oPedido->Getid());
    oPedido->RecuperarPedido("Select * from ped_cli where id ="+cId+" order by id limit 1 ");
    LLenarCampos();
}

void FrmPedidos::on_botBorrarLinea_clicked()
{
    QModelIndex celda=ui->Lineas->currentIndex();
    QModelIndex index=ModelLin_ped->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

    QVariant pKey=ModelLin_ped->data(index,Qt::EditRole);
    int Id_lin =  pKey.toInt();
    oPedido->BorrarLineaPedido(Id_lin);
    lineasVentas();
    RellenarDespuesCalculo();
}

void FrmPedidos::on_pushButton_clicked()
{
    int iRespuesta = QMessageBox::question(NULL,tr("Gestión de pedidos"),tr("Esta opción borrará el pedido y sus líneas de detalle\n"
                                                                            "¿Desea continuar?"),tr("Sí"),tr("no"));
    if(iRespuesta == 0) {
        //Borrar
        QSqlQuery *lin_ped = new QSqlQuery(QSqlDatabase::database("empresa"));

     }
}
