#include "frmfacturas.h"
#include "ui_frmfacturas.h"
#include "Busquedas/frmbuscarcliente.h"
#include "Busquedas/frmBuscarFactura.h"

#include "Almacen/articulo.h"

frmFacturas::frmFacturas( QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmFacturas),
    helper(this)
{
    oFactura = new Factura();
    oCliente1 = new Cliente();
    ui->setupUi(this);
    // Escondo/muestro campos según configuración
    if(Configuracion_global->lProfesional) {
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

    ui->comboPais->setModel(Configuracion_global->paises_model);
    ui->comboPais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));
    // Pongo valores por defecto
    ui->lblContabilizada->setVisible(false);
    ui->lblFacturaCobrada->setVisible(false);
    ui->lblFacturaImpresa->setVisible(false);
    // Rellenar formas de pago
    QSqlQueryModel *  modelFP = new QSqlQueryModel();
    modelFP->setQuery("Select cFormaPago,id from FormPago",QSqlDatabase::database("empresa"));
    ui->txtcFormaPago->setModel(modelFP);
    // valores edicion
    this->Altas = false;
    //ui->txtcCodigoArticulo->setFocus();
    BloquearCampos(true);
    /* -----------------------------------------
     *CONEXIONES
     *----------------------------------------*/
    //connect(ui->txtcCodigoArticulo,SIGNAL(editingFinished()),this,SLOT(on_txtcCodigoArticulo_lostFocus()));

    helper.set_Tipo(false);
    helper.help_table(ui->Lineas);

    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(&helper,SIGNAL(totalChanged(double,double,double,double,double,double,QString)),
            this,SLOT(totalChanged(double,double,double,double,double,double,QString)));

    connect(&helper,SIGNAL(desglose1Changed(double,double,double,double)),
            this,SLOT(desglose1Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose2Changed(double,double,double,double)),
            this,SLOT(desglose2Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose3Changed(double,double,double,double)),
            this,SLOT(desglose3Changed(double,double,double,double)));

    connect(&helper,SIGNAL(desglose4Changed(double,double,double,double)),
            this,SLOT(desglose4Changed(double,double,double,double)));

    connect(ui->chklRecargoEquivalencia,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    ui->txtnRec1->setText(QString::number(Configuracion_global->ivas[keys.at(0)].value("nRegargoEquivalencia").toDouble(),'f',1));
    ui->txtnRec2->setText(QString::number(Configuracion_global->ivas[keys.at(1)].value("nRegargoEquivalencia").toDouble(),'f',1));
    ui->txtnRec3->setText(QString::number(Configuracion_global->ivas[keys.at(2)].value("nRegargoEquivalencia").toDouble(),'f',1));
    ui->txtnRec4->setText(QString::number(Configuracion_global->ivas[keys.at(3)].value("nRegargoEquivalencia").toDouble(),'f',1));


    actionGuardaBorrador = new QAction("Guardar borrador",this);
    actionGuardaFactura = new QAction("Guardar factura",this);
    menu_guardar = new QMenu(this);

    connect(actionGuardaBorrador,SIGNAL(triggered()),this,SLOT(Guardar_factura()));
    connect(actionGuardaFactura,SIGNAL(triggered()),this,SLOT(Guardar_factura()));

    menu_guardar->addAction(actionGuardaFactura);
    menu_guardar->addAction(actionGuardaBorrador);

    ui->btnGuardar->setMenu(menu_guardar);

    if(oFactura->RecuperarFactura("Select * from cab_fac where cFactura > -1 order by cFactura  limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oFactura->Id);
        helper.fillTable("empresa","lin_fac",filter);
    }
}

frmFacturas::~frmFacturas()
{
    delete ui;
    delete oCliente1;
    delete oFactura;
}

void frmFacturas::LLenarCampos() {
    int lEstado;
    ui->txtcCodigoCliente->setText(oFactura->cCodigoCliente);
    ui->txtcFactura->setText(oFactura->cFactura);
    ui->txtdFecha->setDate(oFactura->dFecha);
    ui->txtdFechaCobro->setDate(oFactura->dFechaCobro);
    ui->txtcCliente->setText(oFactura->cCliente);
    ui->txtcDireccion->setText(oFactura->cDireccion);
    ui->txtcDireccion2->setText(oFactura->cDireccion2);
    ui->txtcCp->setText(oFactura->cCp);
    ui->txtcPoblacion->setText(oFactura->cPoblacion);
    ui->txtcProvincia->setText(oFactura->cProvincia);

    QList<QString> keys = Configuracion_global->paises.uniqueKeys();
    for (int i = 0;i<keys.size();i++)
    {
        if(Configuracion_global->paises[keys.at(i)].value("id").toInt() == oFactura->idPais)
        {
            int index = ui->comboPais->findText(keys.at(i));
            ui->comboPais->setCurrentIndex(index);
        }
    }

    ui->txtcCif->setText(oFactura->cCif);
     lEstado = oFactura->lRecargoEquivalencia;
    if ((lEstado= 1)) {
        ui->chklRecargoEquivalencia->setChecked(true);
    } else {
        ui->chklRecargoEquivalencia->setChecked(false);
    }
    ui->txtrSubtotal->setText(Configuracion_global->FormatoNumerico( QString::number(oFactura->rSubtotal,'f',2)));
    //ui->txtnDto->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->nDto,'f',2)));
    //ui->txtnDtoPP->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->nDtoPP,'f',2)));
    ui->txtrImporteDescuento->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rImporteDescuento,'f',2)));
    //ui->txtrImporteDescuentoPP->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rImporteDescuentoPP,'f',2)));
    ui->txtrBase->setText(Configuracion_global->FormatoNumerico(QString::number( oFactura->rBase,'f',2)));
    ui->txtnIva_2->setText(Configuracion_global->FormatoNumerico(QString::number( oFactura->nIva,'f',2)));
    ui->txtrImporteIva->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rImporteIva,'f',2)));
    ui->txtrTotal->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rTotal,'f',2)));
    lEstado = oFactura->lImpresa;
    if((lEstado == 1)) {
        ui->lblFacturaImpresa->setVisible(true);
    } else {
        ui->lblFacturaImpresa->setVisible(false);
    }
    lEstado = oFactura->lCobrada;
    if ((lEstado == 1)) {
        ui->lblFacturaCobrada->setVisible(true);
        ui->txtdFechaCobro->setVisible(true);
    } else {
        ui->lblFacturaCobrada->setVisible(false);
        ui->txtdFechaCobro->setVisible(false);
    }
    lEstado = oFactura->lContablilizada;

    if((lEstado == 1)) {
        ui->lblContabilizada->setVisible(true);
    } else {
        ui->lblContabilizada->setVisible(false);
    }
    int indice=ui->txtcFormaPago->findText(oFactura->cFormaPago);
    if(indice!=-1)
     ui->txtcFormaPago->setCurrentIndex(indice);
    ui->txttComentario->setText(oFactura->tComentario);
    ui->txtrBase1->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rBase1,'f',2)));
    ui->txtrBase2->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rBase2,'f',2)));
    ui->txtrBase3->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rBase3,'f',2)));
    ui->txtrBase4->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rBase4,'f',2)));
    ui->txtnPorcentajeIva1->setText(QString::number(oFactura->nPorcentajeIVA1));
    ui->txtnPorcentajeIva2->setText(QString::number(oFactura->nPorcentajeIVA2));
    ui->txtnPorcentajeIva3->setText(QString::number(oFactura->nPorcentajeIVA3));
    ui->txtnPorcentajeIva4->setText(QString::number(oFactura->nPorcentajeIVA4));
    ui->txtrIVA1->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rIVA1,'f',2)));
    ui->txtrIVA2->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rIVA2,'f',2)));
    ui->txtrIVA3->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rIVA3,'f',2)));
    ui->txtrIVA4->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rIVA4,'f',2)));
    ui->txtrTotal1->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rTotal1,'f',2)));
    ui->txtrTotal2->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rTotal2,'f',2)));
    ui->txtrTotal3->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rTotal3,'f',2)));
    ui->txtrTotal4->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rTotal4,'f',2)));
    ui->txtnRec1->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->nRec1,'f',2)));
    ui->txtnRec2->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->nRec2,'f',2)));
    ui->txtnRec3->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->nRec3,'f',2)));
    ui->txtnRec4->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->nRec4,'f',2)));
    ui->txtrRecargoEq1->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rRecargoEq1,'f',2)));
    ui->txtrRecargoEq2->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rRecargoEq2,'f',2)));
    ui->txtrRecargoEq3->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rRecargoEq3,'f',2)));
    ui->txtrRecargoEq4->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rRecargoEq4,'f',2)));
    ui->txtrTotalRecargoEq->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rTotalRecargoEq,'f',2)));
    ui->txtrEntregadoaCuenta->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rEntregadoaCuenta,'f',2)));
    ui->txtrImportePendiente->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rImportePendiente,'f',2)));
    ui->txtcCodigoEntidad->setText(oFactura->cCodigoEntidad);
    ui->txtcOficinaEntidad->setText(oFactura->cOficinaEntidad);
    ui->txtcDCCuenta->setText(oFactura->cDCCuenta);
    ui->txtcNumeroCuenta->setText(oFactura->cNumeroCuenta);
    ui->txtcPedidoCliente->setText(oFactura->cPedidoCliente);
    if(oFactura->nIRPF!=0)
        ui->lblIRPF_3->setVisible(true);
    else
        ui->lblIRPF_3->setVisible(false);
    if(oFactura->lCobrada==1){
        ui->lblFacturaCobrada->setVisible(true);
        ui->txtdFechaCobro->setVisible(true);
    } else {
        ui->lblFacturaCobrada->setVisible(false);
        ui->txtdFechaCobro->setVisible(false);
    }
    ui->txtnIRPF->setText(QString::number(oFactura->nIRPF));
    ui->txtrImporteIRPF->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rImporteIRPF,'f',2)));
    ui->txtrImporteIRPF_2->setText(Configuracion_global->FormatoNumerico(QString::number(oFactura->rImporteIRPF,'f',2)));

    QString filter = QString("Id_Cab = '%1'").arg(oFactura->Id);
    helper.fillTable("empresa","lin_fac",filter);
}

void frmFacturas::LLenarCamposCliente()
{
    ui->txtcCodigoCliente->setText(oCliente1->cCodigoCliente);
    ui->txtcCliente->setText(oCliente1->cNombreFiscal);
    ui->txtcDireccion->setText(oCliente1->cDireccion1);
    ui->txtcDireccion2->setText(oCliente1->cDireccion2);
    ui->txtcCp->setText(oCliente1->cCp);
    ui->txtcPoblacion->setText(oCliente1->cPoblacion);
    ui->txtcProvincia->setText(oCliente1->cProvincia);
    //ui->txtcPais->setText(oCliente1->getcPais());
    ui->txtcCif->setText(oCliente1->cCifNif);
    int lEstado = 0;
     lEstado = oCliente1->lRecargoEquivalencia;
    if ((lEstado= 1)) {
        ui->chklRecargoEquivalencia->setChecked(true);
    } else {
        ui->chklRecargoEquivalencia->setChecked(false);
    }
    if (oCliente1->lIRPF == true) {
        ui->lblIRPF_3->setVisible(true);
        ui->txtnIRPF->setText(QString::number(Configuracion_global->nIRPF));
        oFactura->nIRPF = (Configuracion_global->nIRPF);
    } else {
        ui->lblIRPF_3->setVisible(false);
        ui->txtnIRPF->setText("0,00");
        oFactura->nIRPF = (Configuracion_global->nIRPF);
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
    //ui->txtcPais->setText("");
    ui->txtcCif->setText("");
    ui->txtrSubtotal->setText(0);
    //ui->txtnDto->setText(0);
    //ui->txtnDtoPP->setText(0);
    ui->txtrImporteDescuento->setText("0,00");
    //ui->txtrImporteDescuentoPP->setText("0,00");
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

    QList<QString> keys = Configuracion_global->ivas.uniqueKeys();


    ui->txtnPorcentajeIva1->setText(QString::number(Configuracion_global->ivas[keys.at(0)].value("nIVA").toDouble()));
    ui->txtnPorcentajeIva2->setText(QString::number(Configuracion_global->ivas[keys.at(1)].value("nIVA").toDouble()));
    ui->txtnPorcentajeIva3->setText(QString::number(Configuracion_global->ivas[keys.at(2)].value("nIVA").toDouble()));
    ui->txtnPorcentajeIva4->setText(QString::number(Configuracion_global->ivas[keys.at(3)].value("nIVA").toDouble()));
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

    helper.fillTable("empresa","lin_fac","Id_Cab = -1");
}

void frmFacturas::BloquearCampos(bool state)
{
    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(state);
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(!state);
    }

    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!state);
    }
    // QTextEdit
    QList<QTextEdit *> textEditList = this->findChildren<QTextEdit *>();
    QTextEdit *textEdit;
    foreach (textEdit,textEditList) {
        textEdit->setReadOnly(state);
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!state);
    }

    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    ui->btnBuscar->setEnabled(state);
    ui->btnDeshacer->setEnabled(!state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->botBuscarCliente->setEnabled(!state);

    ui->btnAnadirLinea->setEnabled(!state);
    ui->btn_borrarLinea->setEnabled(!state);
    helper.blockTable(state);
    ui->txtcFactura->setReadOnly(true);
}

void frmFacturas::LLenarFactura() {
    oFactura->cCodigoCliente = (ui->txtcCodigoCliente->text());
    oFactura->cFactura = (ui->txtcFactura->text());
    oFactura->dFecha = (ui->txtdFecha->date());
    oFactura->dFechaCobro = (ui->txtdFechaCobro->date());
    oFactura->cCliente = (ui->txtcCliente->text());
    oFactura->cDireccion = (ui->txtcDireccion->text());
    oFactura->cDireccion2 = (ui->txtcDireccion2->text());
    oFactura->cCp = (ui->txtcCp->text());
    oFactura->cPoblacion = (ui->txtcPoblacion->text());
    oFactura->cProvincia = (ui->txtcProvincia->text());
    oFactura->idPais = Configuracion_global->paises[ui->comboPais->currentText()].value("id").toInt();
    oFactura->cCif = (ui->txtcCif->text());
    if  (ui->chklRecargoEquivalencia->isChecked())
    {
        oFactura->lRecargoEquivalencia = (1);
    }
    else
    {
        oFactura->lRecargoEquivalencia = (0);
    }
    oFactura->rSubtotal = (ui->txtrSubtotal->text().replace(".","").toDouble());
    //oFactura->nDto = (ui->txtnDto->text().replace(".","").toDouble());
    //oFactura->nDtoPP = (ui->txtnDtoPP->text().replace(".","").toDouble());
    oFactura->rImporteDescuento = (ui->txtrImporteDescuento->text().replace(".","").toDouble());
    //oFactura->rImporteDescuentoPP = (ui->txtrImporteDescuentoPP->text().replace(".","").toDouble());
    oFactura->rBase = (ui->txtrBase->text().replace(".","").toDouble());
    oFactura->rImporteIva = (ui->txtrImporteIva->text().replace(".","").toDouble());
    oFactura->rTotal = (ui->txtrTotal->text().replace(".","").toDouble());
    oFactura->cFormaPago = (ui->txtcFormaPago->currentText());
    oFactura->tComentario = (ui->txttComentario->toPlainText());
    oFactura->rBase1 = (ui->txtrBase1->text().replace(".","").toDouble());
    oFactura->rBase2 = (ui->txtrBase2->text().replace(".","").toDouble());
    oFactura->rBase3 = (ui->txtrBase3->text().replace(".","").toDouble());
    oFactura->rBase4 = (ui->txtrBase4->text().replace(".","").toDouble());
    oFactura->nPorcentajeIVA1 = (ui->txtnPorcentajeIva1->text().replace(".","").toDouble());
    oFactura->nPorcentajeIVA2 = (ui->txtnPorcentajeIva2->text().replace(".","").toDouble());
    oFactura->nPorcentajeIVA3 = (ui->txtnPorcentajeIva3->text().replace(".","").toDouble());
    oFactura->nPorcentajeIVA4 = (ui->txtnPorcentajeIva4->text().replace(".","").toDouble());
    oFactura->rIVA1 = (ui->txtrIVA1->text().replace(".","").toDouble());
    oFactura->rIVA2 = (ui->txtrIVA2->text().replace(".","").toDouble());
    oFactura->rIVA3 = (ui->txtrIVA3->text().replace(".","").toDouble());
    oFactura->rIVA4 = (ui->txtrIVA4->text().replace(".","").toDouble());
    oFactura->rTotal1 = (ui->txtrTotal1->text().replace(".","").toDouble());
    oFactura->rTotal2 = (ui->txtrTotal2->text().replace(".","").toDouble());
    oFactura->rTotal3 = (ui->txtrTotal3->text().replace(".","").toDouble());
    oFactura->rTotal4 = (ui->txtrTotal4->text().replace(".","").toDouble());
    oFactura->nRec1 = (ui->txtnRec1->text().replace(".","").toDouble());
    oFactura->nRec2 = (ui->txtnRec2->text().replace(".","").toDouble());
    oFactura->nRec3 = (ui->txtnRec3->text().replace(".","").toDouble());
    oFactura->nRec4 = (ui->txtnRec4->text().replace(".","").toDouble());
    oFactura->rRecargoEq1 = (ui->txtrRecargoEq1->text().replace(".","").toDouble());
    oFactura->rRecargoEq2 = (ui->txtrRecargoEq2->text().replace(".","").toDouble());
    oFactura->rRecargoEq3 = (ui->txtrRecargoEq3->text().replace(".","").toDouble());
    oFactura->rRecargoEq4 = (ui->txtrRecargoEq4->text().replace(".","").toDouble());
    oFactura->rTotalRecargoEq = (ui->txtrTotalRecargoEq->text().replace(".","").toDouble());
    oFactura->rEntregadoaCuenta = (ui->txtrEntregadoaCuenta->text().replace(".","").toDouble());
    oFactura->rImportePendiente = (ui->txtrImportePendiente->text().replace(".","").toDouble());
    oFactura->cCodigoEntidad = (ui->txtcCodigoEntidad->text());
    oFactura->cOficinaEntidad = (ui->txtcOficinaEntidad->text());
    oFactura->cDCCuenta = (ui->txtcDCCuenta->text());
    oFactura->cNumeroCuenta = (ui->txtcNumeroCuenta->text());
    oFactura->cPedidoCliente = (ui->txtcPedidoCliente->text());
    oFactura->nIRPF = (ui->txtnIRPF->text().replace(".","").toDouble());
    oFactura->rImporteIRPF = (ui->txtrImporteIRPF->text().replace(".","").toDouble());
}

void frmFacturas::on_btnSiguiente_clicked()
{
    QString cFactura = oFactura->cFactura;
    if(oFactura->RecuperarFactura("Select * from cab_fac where cFactura >'"+cFactura+"' order by cFactura  limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oFactura->Id);
        helper.fillTable("empresa","lin_fac",filter);
    }
}
void frmFacturas::on_btnAnterior_clicked()
{
    QString cFactura = oFactura->cFactura;
    if(oFactura->RecuperarFactura("Select * from cab_fac where cFactura <'"+cFactura+"' order by cFactura desc limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oFactura->Id);
        helper.fillTable("empresa","lin_fac",filter);
    }
}


void frmFacturas::Guardar_factura()
{
    QSqlDatabase::database("empresa").transaction();
    bool succes = true;
    LLenarFactura();

    if(sender()==actionGuardaBorrador)
        oFactura->cFactura = ("BORRADOR");
    else
        oFactura->cFactura = oFactura->NuevoNumeroFactura();

    if(in_edit)
    {
        //FIXME factura legal?
        succes &= oFactura->GuardarFactura(oFactura->Id,false);
        succes &= oFactura->BorrarLineasFactura(oFactura->Id);
        succes &= helper.saveTable(oFactura->Id,"empresa","lin_fac");
    }
    else
    {
        succes &= oFactura->AnadirFactura();
        succes &= helper.saveTable(oFactura->Id,"empresa","lin_fac");
    }
    if(succes)
    {
        LLenarCampos();
        succes = QSqlDatabase::database("empresa").commit();
    }
    if(succes)
    {
        QMessageBox::information(this,tr("Guardado"),tr("Guardado con éxito"),tr("&Aceptar"));
        BloquearCampos(true);
        emit unblock();
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),
                              tr("Error al guardar la factura.\n")+QSqlDatabase::database("empresa").lastError().text(),
                              tr("&Aceptar"));
        QSqlDatabase::database("empresa").rollback();
    }
}

void frmFacturas::on_btnAnadir_clicked()
{
    ui->btnGuardar->setMenu(menu_guardar);
    BloquearCampos(false);
    in_edit = false;
    VaciarCampos();    
    ui->chklRecargoEquivalencia->setCheckState(Qt::Unchecked);
    ui->txtcCodigoCliente->setFocus();
    emit block();
}

void frmFacturas::on_btnDeshacer_clicked()
{
    BloquearCampos(true);
    QString cId = QString::number(oFactura->Id);
    oFactura->RecuperarFactura("Select * from cab_fac where id ="+cId+" order by id limit 1 ");
    LLenarCampos();
    emit unblock();
}

void frmFacturas::on_botBuscarCliente_clicked()
{
    FrmBuscarCliente BuscarClientes;
    BuscarClientes.exec();
    int nId = BuscarClientes.DevolverID();
    //qDebug() << nId;
    QString cId = QString::number(nId);
    oFactura->iId_Cliente = (nId);
    oCliente1->Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
    LLenarCamposCliente();
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

void frmFacturas::on_btnImprimir_clicked()
{
   // KDReports::Report report;
    //TODO Reports
}

void frmFacturas::totalChanged(double base, double dto, double subTotal, double iva, double re, double total, QString moneda)
{
    ui->txtrBase->setText(QString::number(base)+moneda);
    ui->txtrImporteDescuento->setText(QString::number(dto)+moneda);
    ui->txtrSubtotal->setText(QString::number(subTotal)+moneda);
    ui->txtrImporteIva->setText(QString::number(iva)+moneda);
    ui->txtrTotalRecargoEq->setText(QString::number(re)+moneda);
    ui->txtrTotal->setText(QString::number(total)+moneda);
    ui->lbl_total->setText(QString::number(total)+moneda);

    ui->txtrBaseTotal_2->setText(QString::number(base)+moneda);
    ui->txtnIva_2->setText(QString::number(iva)+moneda);
    ui->txtrTotalRecargoEq->setText(QString::number(re)+moneda);
    ui->txtrTotal_2->setText(QString::number(total)+moneda);
}

void frmFacturas::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtrBase1->setText(QString::number(base));
    ui->txtrIVA1->setText(QString::number(iva));
    ui->txtrRecargoEq1->setText(QString::number(re));
    ui->txtrTotal1->setText(QString::number(total));
}

void frmFacturas::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtrBase2->setText(QString::number(base));
    ui->txtrIVA2->setText(QString::number(iva));
    ui->txtrRecargoEq2->setText(QString::number(re));
    ui->txtrTotal2->setText(QString::number(total));
}

void frmFacturas::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtrBase3->setText(QString::number(base));
    ui->txtrIVA3->setText(QString::number(iva));
    ui->txtrRecargoEq3->setText(QString::number(re));
    ui->txtrTotal3->setText(QString::number(total));
}

void frmFacturas::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtrBase4->setText(QString::number(base));
    ui->txtrIVA4->setText(QString::number(iva));
    ui->txtrRecargoEq4->setText(QString::number(re));
    ui->txtrTotal4->setText(QString::number(total));
}

void frmFacturas::on_btnEditar_clicked()
{
    BloquearCampos(false);
    in_edit = true;
    emit block();
}
