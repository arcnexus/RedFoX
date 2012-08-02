#include "frmfacturas.h"
#include "ui_frmfacturas.h"
#include "factura.h"
#include <QSqlDatabase>
#include <QtSql>
#include <QErrorMessage>
#include<QSqlError>
#include<QDate>
#include <QSqlQueryModel>
#include <QDebug>
#include "frmbuscarcliente.h"
#include <QTableView>
#include <QHeaderView>
#include "pruebas.h"

Factura *oFactura = new Factura();
frmFacturas::frmFacturas(Configuracion *m_config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmFacturas)
{
    ui->setupUi(this);

    ui->lblContabilizada->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lblFacturaImpresa->setVisible(false);
    // Rellenar formas de pago
    modelFP = new QSqlQueryModel();
    modelFP->setQuery("Select cFormaPago,id from FormPago",QSqlDatabase::database("emp"));
    ui->txtcFormaPago->setModel(modelFP);
    // valores edicion
    this->Altas = false;
}

frmFacturas::~frmFacturas()
{
    delete ui;
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
    ui->txtrSubtotal->setValue(oFactura->getrSubtotal());
    ui->txtnDto->setValue(oFactura->getnDto());
    ui->txtnDtoPP->setValue(oFactura->getnDtoPP());
    ui->txtrImporteDescuento->setValue(oFactura->getrImporteDescuento());
    ui->txtrImporteDescuentoPP->setValue(oFactura->getrImporteDescuentoPP());
    ui->txtrBase->setValue(oFactura->getrBase());
    ui->txtnIva_2->setValue(oFactura->getnIva());
    ui->txtrImporteIva->setValue(oFactura->getrImporteIva());
    ui->txtrTotal->setValue(oFactura->getrTotal());
    lEstado = oFactura->getlImpresa();
    if((lEstado == 1)) {
        ui->lblFacturaImpresa->setVisible(true);
    } else {
        ui->lblFacturaImpresa->setVisible(false);
    }
    lEstado = oFactura->getlCobrada();
    if ((lEstado == 1)) {
        ui->lblFacturaCobrada->setVisible(true);
    } else {
        ui->lblFacturaCobrada->setVisible(false);
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
    ui->txtrBase1->setValue(oFactura->getrBase1());
    ui->txtrBase2->setValue(oFactura->getrBase2());
    ui->txtrBase3->setValue(oFactura->getrBase3());
    ui->txtrbase4->setValue(oFactura->getrBase4());
    ui->txtnPorcentajeIva1->setValue(oFactura->getnPorcentajeIVA1());
    ui->txtnPorcentajeIva2->setValue(oFactura->getnPorcentajeIVA2());
    ui->txtnPorcentajeIva3->setValue(oFactura->getnPorcentajeIVA3());
    ui->txtnPorcentajeIva4->setValue(oFactura->getnPorcentajeIVA4());
    ui->txtrIVA1->setValue(oFactura->getrIVA1());
    ui->txtrIVA2->setValue(oFactura->getrIVA2());
    ui->txtrIVA3->setValue(oFactura->getrIVA3());
    ui->txtrIVA4->setValue(oFactura->getrIVA4());
    ui->txtrTotal1->setValue(oFactura->getrTotal1());
    ui->txtrTotal2->setValue(oFactura->getrTotal2());
    ui->txtrTotal3->setValue(oFactura->getrTotal3());
    ui->txtrTotal4->setValue(oFactura->getrTotal4());
    ui->txtnRec1->setValue(oFactura->getnRec1());
    ui->txtnRec2->setValue(oFactura->getnRec2());
    ui->txtnRec3->setValue(oFactura->getnRec3());
    ui->txtnRec4->setValue(oFactura->getnRec4());
    ui->txtrRecargoEq1->setValue(oFactura->getrRecargoEq1());
    ui->txtrRecargoEq2->setValue(oFactura->getrRecargoEq2());
    ui->txtrRecargoEq3->setValue(oFactura->getrRecargoEq3());
    ui->txtrRecargoEq4->setValue(oFactura->getrRecargoEq4());
    ui->txtrTotalRecargoEq->setValue(oFactura->getrTotalRecargoEq());
    ui->txtrEntregadoaCuenta->setValue(oFactura->getrEntregadoaCuenta());
    ui->txtrImportePendiente->setValue(oFactura->getrImportePendiente());
    ui->txtcCodigoEntidad->setText(oFactura->getcCodigoEntidad());
    ui->txtcOficinaEntidad->setText(oFactura->getcOficinaEntidad());
    ui->txtcDCCuenta->setText(oFactura->getcDCCuenta());
    ui->txtcNumeroCuenta->setText(oFactura->getcNumeroCuenta());
    ui->txtcPedidoCliente->setText(oFactura->getcPedidoCliente());
    // lineas de ventas
    QString cSQL;
    QString cId;
    cId = cId.number(oFactura->Getid());
     cSQL ="select id,cCodigo,nCantidad,cDescripcion,rPvp,rSubtotal,nDto,rDto,rTotal from lin_fac "
             " where id_Cab = "+cId;
     ModelLin_fac = new QSqlQueryModel();
     ModelLin_fac->setQuery(cSQL,QSqlDatabase::database("empresa"));
     ui->Lineas->setModel(ModelLin_fac);
      //Creamos Objeto de la clase Cabecera para las cabeceras horizontales
     Cabecera = new QHeaderView(Qt::Horizontal,this);
     // Le decimos a nuestro objeto QTableView  que use la instancia de QHeaderView que acabamos de crear.
     ui->Lineas->setHorizontalHeader(Cabecera);
     /*Ponemos el tamaño deseado para cada columna, teniendo en cuenta que la primera columna es la "0". (en nuestro caso está oculta ya que muestra el id de la tabla y esto no nos interesa que lo vea el usuario */
     Cabecera->setResizeMode(1,QHeaderView::Fixed);
     Cabecera->resizeSection(0,0);
     Cabecera->setResizeMode(1,QHeaderView::Fixed);
     Cabecera->resizeSection(1,85);
     Cabecera->setResizeMode(2,QHeaderView::Fixed);
     Cabecera->resizeSection(2,65);
     Cabecera->setResizeMode(3,QHeaderView::Fixed);
     Cabecera->resizeSection(3,253);
     Cabecera->setResizeMode(4,QHeaderView::Fixed);
     Cabecera->resizeSection(4,100);
     ModelLin_fac->setHeaderData(1, Qt::Horizontal, QObject::tr("CÓDIGO"));
     ModelLin_fac->setHeaderData(2, Qt::Horizontal, QObject::tr("CANTIDAD"));
     ModelLin_fac->setHeaderData(3, Qt::Horizontal, QObject::tr("DESCRIPCIÓN"));
     ModelLin_fac->setHeaderData(4, Qt::Horizontal, QObject::tr("PVP"));
     ModelLin_fac->setHeaderData(5, Qt::Horizontal, QObject::tr("SUBTOTAL"));
     ModelLin_fac->setHeaderData(6, Qt::Horizontal, QObject::tr("%DTO"));
     ModelLin_fac->setHeaderData(7, Qt::Horizontal, QObject::tr("DTO"));
     ModelLin_fac->setHeaderData(8, Qt::Horizontal, QObject::tr("TOTAL"));

    // hacemos visible la cabecera
     Cabecera->setVisible(true);
     ui->Lineas->setItemDelegateForColumn(2, new ColumnaGrid(this));
     ui->Lineas->setItemDelegateForColumn(4, new ColumnaGrid(this));
     ui->Lineas->setItemDelegateForColumn(5, new ColumnaGrid(this));
     ui->Lineas->setItemDelegateForColumn(6, new ColumnaGrid(this));
     ui->Lineas->setItemDelegateForColumn(7, new ColumnaGrid(this));
     ui->Lineas->setItemDelegateForColumn(8, new ColumnaGrid(this));



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
    ui->chklRecargoEquivalencia->setChecked(false);
    ui->txtrSubtotal->setValue(0);
    ui->txtnDto->setValue(0);
    ui->txtnDtoPP->setValue(0);
    ui->txtrImporteDescuento->setValue(0);
    ui->txtrImporteDescuentoPP->setValue(0);
    ui->txtrBase->setValue(0);
    ui->txtnIva_2->setValue(0);
    ui->txtrImporteIva->setValue(0);
    ui->txtrTotal->setValue(0);
    ui->lblFacturaImpresa->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lblContabilizada->setVisible(false);
    int indice=ui->txtcFormaPago->findText("Contado");
    if(indice!=-1) ui->txtcFormaPago->setCurrentIndex(indice);
    ui->txttComentario->setText("");
    ui->txtrBase1->setValue(0);
    ui->txtrBase2->setValue(0);
    ui->txtrBase3->setValue(0);
    ui->txtrbase4->setValue(0);
    QSqlQuery tiposiva(QSqlDatabase::database("empresa"));
    // IVA NORMAL
    tiposiva.prepare("Select nIva from tiposiva where cTipo = 'NORMAL'");
    if (!tiposiva.exec()) {
            QMessageBox::critical(0, "error:", tiposiva.lastError().text());
    } else {
        tiposiva.next();
        ui->txtnPorcentajeIva1->setValue(tiposiva.value(0).toInt());
    }
    // IVA REDUCIDO
    tiposiva.prepare("Select nIva from tiposiva where cTipo = 'REDUCIDO'");
    if (!tiposiva.exec()) {
            QMessageBox::critical(0, "error:", tiposiva.lastError().text());
    } else {
        tiposiva.next();
        ui->txtnPorcentajeIva2->setValue(tiposiva.value(0).toInt());
    }
    // IVA SUPER-REDUCIDO
    tiposiva.prepare("Select nIva from tiposiva where cTipo = 'SUPER-REDUCIDO'");
    if (!tiposiva.exec()) {
            QMessageBox::critical(0, "error:", tiposiva.lastError().text());
    } else {
        tiposiva.next();
        ui->txtnPorcentajeIva3->setValue(tiposiva.value(0).toInt());
    }
    // IVA EXCENTO
    tiposiva.prepare("Select nIva from tiposiva where cTipo = 'EXENTO'");
    if (!tiposiva.exec()) {
            QMessageBox::critical(0, "error:", tiposiva.lastError().text());
    } else {
        tiposiva.next();
        ui->txtnPorcentajeIva4->setValue(tiposiva.value(0).toInt());
    }

    ui->txtrIVA1->setValue(0);
    ui->txtrIVA2->setValue(0);
    ui->txtrIVA3->setValue(0);
    ui->txtrIVA4->setValue(0);
    ui->txtrTotal1->setValue(0);
    ui->txtrTotal2->setValue(0);
    ui->txtrTotal3->setValue(0);
    ui->txtrTotal4->setValue(0);
    ui->txtnRec1->setValue(0);
    ui->txtnRec2->setValue(0);
    ui->txtnRec3->setValue(0);
    ui->txtnRec4->setValue(0);
    ui->txtrRecargoEq1->setValue(0);
    ui->txtrRecargoEq2->setValue(0);
    ui->txtrRecargoEq3->setValue(0);
    ui->txtrRecargoEq4->setValue(0);
    ui->txtrTotalRecargoEq->setValue(0);
    ui->txtrEntregadoaCuenta->setValue(0);
    ui->txtrImportePendiente->setValue(0);
    ui->txtcCodigoEntidad->setText("");
    ui->txtcOficinaEntidad->setText("");
    ui->txtcDCCuenta->setText("");
    ui->txtcNumeroCuenta->setText("");
    ui->txtcPedidoCliente->setText("");
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
    // SpinBox
    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    QSpinBox *SpinBox;
    foreach (SpinBox, SpinBoxList) {
        SpinBox->setReadOnly(true);
        //qDebug() << lineEdit->objectName();
    }
    // DoubleSpinBox
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

    ui->btnAnadir->setEnabled(true);
    ui->btnAnterior->setEnabled(true);
   // ui->btnBorrar->setEnabled(true);
    ui->btnBuscar->setEnabled(true);
    ui->btnCerrar->setEnabled(true);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);

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
    // SpinBox
    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
    QSpinBox *SpinBox;
    foreach (SpinBox, SpinBoxList) {
        SpinBox->setReadOnly(false);
        //qDebug() << lineEdit->objectName();
    }
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
    ui->btnAnadir->setEnabled(false);
    ui->btnAnterior->setEnabled(false);
   // ui->btnBorrar->setEnabled(false);
    ui->btnBuscar->setEnabled(false);
    ui->btnCerrar->setEnabled(false);
    ui->btnDeshacer->setEnabled(true);
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnSiguiente->setEnabled(false);
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
    oFactura->setrSubtotal(ui->txtrSubtotal->value());
    oFactura->setnDto(ui->txtnDto->value());
    oFactura->setnDtoPP(ui->txtnDtoPP->value());
    oFactura->setrImporteDescuento(ui->txtrImporteDescuento->value());
    oFactura->setrImporteDescuentoPP(ui->txtrImporteDescuentoPP->value());
    oFactura->setrBase(ui->txtrBase->value());
    oFactura->setrImporteIva(ui->txtrImporteIva->value());
    oFactura->setrTotal(ui->txtrTotal->value());
    oFactura->setcFormaPago(ui->txtcFormaPago->currentText());
    oFactura->settComentario(ui->txttComentario->toPlainText());
    oFactura->setrBase1(ui->txtrBase1->value());
    oFactura->setrBase2(ui->txtrBase2->value());
    oFactura->setrBase3(ui->txtrBase3->value());
    oFactura->setrBase4(ui->txtrbase4->value());
    oFactura->setnPorcentajeIVA1(ui->txtnPorcentajeIva1->value());
    oFactura->setnPorcentajeIVA2(ui->txtnPorcentajeIva2->value());
    oFactura->setnPorcentajeIVA3(ui->txtnPorcentajeIva3->value());
    oFactura->setnPorcentajeIVA4(ui->txtnPorcentajeIva4->value());
    oFactura->setrIVA1(ui->txtrIVA1->value());
    oFactura->setrIVA2(ui->txtrIVA2->value());
    oFactura->setrIVA3(ui->txtrIVA3->value());
    oFactura->setrIVA4(ui->txtrIVA4->value());
    oFactura->setrTotal1(ui->txtrTotal1->value());
    oFactura->setrTotal2(ui->txtrTotal2->value());
    oFactura->setrTotal3(ui->txtrTotal3->value());
    oFactura->setrTotal4(ui->txtrTotal4->value());
    oFactura->setnRec1(ui->txtnRec1->value());
    oFactura->setnRec2(ui->txtnRec2->value());
    oFactura->setnRec3(ui->txtnRec3->value());
    oFactura->setnRec4(ui->txtnRec4->value());
    oFactura->setrRecargoEq1(ui->txtrRecargoEq1->value());
    oFactura->setrRecargoEq2(ui->txtrRecargoEq2->value());
    oFactura->setrRecargoEq3(ui->txtrRecargoEq3->value());
    oFactura->setrRecargoEq4(ui->txtrRecargoEq4->value());
    oFactura->setrTotalRecargoEq(ui->txtrTotalRecargoEq->value());
    oFactura->setrEntregadoaCuenta(ui->txtrEntregadoaCuenta->value());
    oFactura->setrImportePendiente(ui->txtrImportePendiente->value());
    oFactura->setcCodigoEntidad(ui->txtcCodigoEntidad->text());
    oFactura->setcOficinaEntidad(ui->txtcOficinaEntidad->text());
    oFactura->setcDCCuenta(ui->txtcDCCuenta->text());
    oFactura->setcNumeroCuenta(ui->txtcNumeroCuenta->text());
    oFactura->setcPedidoCliente(ui->txtcPedidoCliente->text());
}

void frmFacturas::on_btnSiguiente_clicked()
{
    QString cId = QString::number(oFactura->Getid());
    oFactura->RecuperarFactura("Select * from cab_fac where id >"+cId+" order by id limit 0,1 ");
    LLenarCampos();

}

void frmFacturas::on_btnGuardar_clicked()
{

    LLenarFactura();
    BloquearCampos();
    if(this->Altas) {
       this->Altas = false;
       oFactura->NuevoNumeroFactura();
       oFactura->AnadirFactura();
    } else {

        oFactura->GuardarFactura(oFactura->Getid());
    }
    //bloquearCampos();
}

void frmFacturas::on_btnAnadir_clicked()
{
    this->Altas = true;
    DesbloquearCampos();
    VaciarCampos();
    ui->txtcCodigoCliente->setFocus();
}
ColumnaGrid::ColumnaGrid(QObject *parent)
    :QItemDelegate(parent)
{
}

void ColumnaGrid::paint(QPainter *painter,
                 const QStyleOptionViewItem & option,
                 const QModelIndex & index) const
{

    QString text = index.model()->data(index, Qt::DisplayRole).toString();

    /* Verificamos el Index */
    if (index.column() == 4 || index.column() == 5 || index.column() == 8)
        if (text.length() >6) {
            text.insert(1, ".");
        }



    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;

    drawDisplay(painter, myOption, myOption.rect,text);
    drawFocus(painter, myOption, myOption.rect);

}

void frmFacturas::on_AnadirLinea_clicked()
{
    Pruebas *frmPrueba =new  Pruebas;
    frmPrueba->exec();
}
