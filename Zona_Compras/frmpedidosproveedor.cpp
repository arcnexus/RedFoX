#include "frmpedidosproveedor.h"
#include "ui_frmpedidosproveedor.h"
#include "Busquedas/frmbuscarproveedor.h"




FrmPedidosProveedor::FrmPedidosProveedor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmPedidosProveedor),
    helper(this)
{
    ui->setupUi(this);
    estadolectura();
    ui->lblImpreso->setVisible(false);
    ui->lblnombreProveedor->clear();
    ui->lblnumero_pedido->clear();

    //oProveedor = new Proveedor(this);
    ui->combo_pais->setModel(Configuracion_global->paises_model);
    ui->combo_pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    helper.set_Tipo(true);
    helper.help_table(ui->Lineas);
    helper.set_tarifa(1);


    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(ui->chklRecargoEq,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));
    connect(ui->btnAnadir,SIGNAL(clicked()),this,SLOT(anadir_pedido()));
    connect(ui->btnGuardar,SIGNAL(clicked()),this,SLOT(guardar_pedido()));
    connect(ui->btnSiguiente,SIGNAL(clicked()),this,SLOT(siguiente()));
    connect(ui->btnAnterior,SIGNAL(clicked()),this,SLOT(anterior()));

    connect(ui->tabWidget_2,SIGNAL(currentChanged(int)),this,SLOT(resizeTable(int)));

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

    ui->txtnIva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtnIva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtnIva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtnIva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtnRec1->setText(Configuracion_global->reList.at(0));
    ui->txtnRec2->setText(Configuracion_global->reList.at(1));
    ui->txtnRec3->setText(Configuracion_global->reList.at(2));
    ui->txtnRec4->setText(Configuracion_global->reList.at(3));

}

FrmPedidosProveedor::~FrmPedidosProveedor()
{
    delete ui;
}

void FrmPedidosProveedor::totalChanged(double base, double dto, double subTotal, double iva, double re, double total, QString moneda)
{
    ui->txtrBase->setText(QString::number(base)+moneda);
    ui->txtrImporteDescuento->setText(QString::number(dto)+moneda);
    ui->txtrSubtotal->setText(QString::number(subTotal)+moneda);
    ui->txtrImporteIva->setText(QString::number(iva)+moneda);
    ui->txtrTotalRecargoEq->setText(QString::number(re)+moneda);
    ui->txtrTotal->setText(QString::number(total)+moneda);
    ui->lbl_total->setText(QString::number(total)+moneda);

    ui->txtrBaseTotal_2->setText(QString::number(base)+moneda);
    ui->txtrTotalIVA_2->setText(QString::number(iva)+moneda);
    ui->txtrTotalRecargoEq_2->setText(QString::number(re)+moneda);
    ui->txtrTotal_2->setText(QString::number(total)+moneda);
}

void FrmPedidosProveedor::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtrBase1->setText(QString::number(base));
    ui->txtrIVA1->setText(QString::number(iva));
    ui->txtrRecargoEq1->setText(QString::number(re));
    ui->txtrTotal1->setText(QString::number(total));
}

void FrmPedidosProveedor::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtrBase2->setText(QString::number(base));
    ui->txtrIVA2->setText(QString::number(iva));
    ui->txtrRecargoEq2->setText(QString::number(re));
    ui->txtrTotal2->setText(QString::number(total));
}

void FrmPedidosProveedor::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtrBase3->setText(QString::number(base));
    ui->txtrIVA3->setText(QString::number(iva));
    ui->txtrRecargoEq3->setText(QString::number(re));
    ui->txtrTotal3->setText(QString::number(total));
}

void FrmPedidosProveedor::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtrBase4->setText(QString::number(base));
    ui->txtrIVA4->setText(QString::number(iva));
    ui->txtrRecargoEq4->setText(QString::number(re));
    ui->txtrTotal4->setText(QString::number(total));
}

void FrmPedidosProveedor::estadoedicion()
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
    ui->btn_borrar->setEnabled(false);
    ui->btnDeshacer->setEnabled(true);
    ui->btnEditar->setEnabled(false);
    ui->btnGuardar->setEnabled(true);
    ui->btnSiguiente->setEnabled(false);
}

void FrmPedidosProveedor::estadolectura()
{

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

    ui->btnAnadir->setEnabled(true);
    ui->btnAnterior->setEnabled(true);
    ui->btn_borrar->setEnabled(true);
    ui->btnDeshacer->setEnabled(false);
    ui->btnEditar->setEnabled(true);
    ui->btnGuardar->setEnabled(false);
    ui->btnSiguiente->setEnabled(true);
    // activo controles que deben estar activos.


}

void FrmPedidosProveedor::buscar_proveeedor()
{
    FrmBuscarProveedor buscar(this);
    if(buscar.exec() == QDialog::Accepted)
    {
        int id_proveedor = buscar.nIdProv;
        oProveedor->Recuperar("select * from proveedores where id = "+QString::number(id_proveedor));
        ui->txtcCodigoProveedor->setText(oProveedor->cCodigo);
        ui->txtcProveedor->setText(oProveedor->cProveedor);
        ui->txtcDireccion->setText(oProveedor->cDireccion1);
        ui->txtcDireccion2->setText(oProveedor->cDireccion2);
        ui->txtcCp->setText(oProveedor->cCP);
        ui->txtcPoblacion->setText(oProveedor->cPoblacion);
        ui->txtcProvincia->setText(oProveedor->cProvincia);
        ui->lblnombreProveedor->setText(oProveedor->cProveedor);

    }
}

void FrmPedidosProveedor::anadir_pedido()
{
    int id = oPedido_proveedor->nuevo_pedido_proveedor();
    clear();
    oPedido_proveedor->recuperar(id);
    this->id = id;

    llenar_campos();
    estadoedicion();
}

void FrmPedidosProveedor::guardar_pedido()
{
    estadolectura();
    guardar_campos_en_objeto();
    oPedido_proveedor->id =this->id;
    oPedido_proveedor->guardar();
    oPedido_proveedor->recuperar(oPedido_proveedor->id);

    llenar_campos();
    helper.saveTable(1,"empresa","lin_ped_pro");

}

void FrmPedidosProveedor::siguiente()
{
    oPedido_proveedor->recuperar("select * from ped_pro where nPedido > "+
                                 QString::number(oPedido_proveedor->nPedido)+ " order by nPedido limit 0,1",1);
    llenar_campos();
}

void FrmPedidosProveedor::anterior()
{
    oPedido_proveedor->recuperar("select * from ped_pro where nPedido < "
                                 +QString::number(oPedido_proveedor->nPedido)+ " order by nPedido desc  limit 0,1 ",2);
    llenar_campos();
}

void FrmPedidosProveedor::llenar_campos()
{
    this->id =oPedido_proveedor->id;
    ui->txtnPedido->setText(QString::number(oPedido_proveedor->nPedido));
    ui->lblSerie->setText(QString::number(oPedido_proveedor->nEjercicio));
    ui->lblnumero_pedido->setText(QString::number(oPedido_proveedor->nPedido));
    ui->lblnombreProveedor->setText(oPedido_proveedor->cProveedor);
    ui->txtdFecha->setDate(oPedido_proveedor->dFecha);
    ui->txtFechaLimite->setDate(oPedido_proveedor->dRecepcion);
    ui->txtcCodigoProveedor->setText(oPedido_proveedor->cCodigoProveedor);
    ui->txtcProveedor->setText(oPedido_proveedor->cProveedor);
    ui->txtcDireccion->setText(oPedido_proveedor->cDireccion1);
    ui->txtcDireccion2->setText(oPedido_proveedor->cDireccion2);
    ui->txtcCp->setText(oPedido_proveedor->cCP);
    ui->txtcPoblacion->setText(oPedido_proveedor->cPoblacion);
    ui->txtcProvincia->setText(oPedido_proveedor->cProvincia);
    ui->txtcCif->setText(oPedido_proveedor->cCifNif);
    ui->txtrBase->setText(QString::number(oPedido_proveedor->rBaseTotal));
    ui->txtrSubtotal->setText(QString::number(oPedido_proveedor->rSubotal));
    ui->txtrImporteDescuento->setText(QString::number(oPedido_proveedor->rDto));
    ui->txtrImporteIva->setText(QString::number(oPedido_proveedor->nIVA));
    ui->txtrRecargoEq1->setText(QString::number(oPedido_proveedor->rRecTotal));
    ui->txtrTotal->setText(QString::number(oPedido_proveedor->rTotal));
    ui->chklEnviado->setChecked(oPedido_proveedor->lEnviado);
    ui->chkRecibido->setChecked(oPedido_proveedor->lRecibido);
    ui->chklCompleto->setChecked(oPedido_proveedor->lRecibidoCompleto);

    ui->txttComentario->setText(oPedido_proveedor->tComentario);
    ui->txtdFechaRecepcion->setDate(oPedido_proveedor->dFechaEntrega);
    ui->txtcDireccionEntrega->setText(oPedido_proveedor->cDireccion1Entrega);
    ui->txtcDireccionEntrega_2->setText(oPedido_proveedor->cDireccion2Entrega);
    ui->txtcCpEntrega->setText(oPedido_proveedor->cCPEntrega);
    ui->txtcPoblacionEntrega->setText(oPedido_proveedor->cPoblacionEntrega);
    ui->txtcProvinciaEntrega->setText(oPedido_proveedor->cProvinciaEntrega);
    ui->txtHorarioEntrega->setText(oPedido_proveedor->cHorarioActivo);
    ui->txtrBase1->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rBase1,'f',2)));
    ui->txtrBase2->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rBase2,'f',2)));
    ui->txtrBase3->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rBase3,'f',2)));
    ui->txtrBase4->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rBase4,'f',2)));
    ui->txtnIva1->setText(QString::number(oPedido_proveedor->niva1));
    ui->txtnIva2->setText(QString::number(oPedido_proveedor->niva2));
    ui->txtnIva3->setText(QString::number(oPedido_proveedor->niva3));
    ui->txtnIva4->setText(QString::number(oPedido_proveedor->niva4));
    ui->txtrIVA1->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->riva1,'f',2)));
    ui->txtrIVA2->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->riva2,'f',2)));
    ui->txtrIVA3->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->riva3,'f',2)));
    ui->txtrIVA4->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->riva4,'f',2)));
    ui->txtnRec1->setText(QString::number(oPedido_proveedor->nrec1));
    ui->txtnRec2->setText(QString::number(oPedido_proveedor->nrec2));
    ui->txtnRec3->setText(QString::number(oPedido_proveedor->nrec3));
    ui->txtnRec4->setText(QString::number(oPedido_proveedor->nrec4));
    ui->txtrRecargoEq1->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rrec1,'f',2)));
    ui->txtrRecargoEq2->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rrec2,'f',2)));
    ui->txtrRecargoEq3->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rrec3,'f',2)));
    ui->txtrRecargoEq4->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rrec4,'f',2)));
    ui->txtrTotal1->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rtotal1,'f',2)));
    ui->txtrTotal2->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rtotal2,'f',2)));
    ui->txtrTotal3->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rtotal3,'f',2)));
    ui->txtrTotal4->setText(Configuracion_global->FormatoNumerico(QString::number(oPedido_proveedor->rtotal4,'f',2)));
    QString filter = QString("id_cab = '%1'").arg(oPedido_proveedor->id);
    helper.fillTable("empresa","lin_ped_pro",filter);
    helper.resizeTable();
}

void FrmPedidosProveedor::guardar_campos_en_objeto()
{
    oPedido_proveedor->id = this->id;
    oPedido_proveedor->nPedido = ui->lblnumero_pedido->text().toInt();
    oPedido_proveedor->nEjercicio = ui->lblSerie->text().toInt();
    oPedido_proveedor->cProveedor = ui->txtcProveedor->text();
    oPedido_proveedor->dFecha = ui->txtdFecha->date();
    oPedido_proveedor->dRecepcion =ui->txtdFechaRecepcion->date();
    oPedido_proveedor->cCodigoProveedor = ui->txtcCodigoProveedor->text();
    oPedido_proveedor->cDireccion1 = ui->txtcDireccion->text();
    oPedido_proveedor->cDireccion2 = ui->txtcDireccion2->text();
    oPedido_proveedor->cCP = ui->txtcCp->text();
    oPedido_proveedor->cPoblacion = ui->txtcPoblacion->text();
    oPedido_proveedor->cProvincia = ui->txtcProvincia->text();
    oPedido_proveedor->cCifNif = ui->txtcCif->text();
    oPedido_proveedor->rBaseTotal = ui->txtrBaseTotal_2->text().toDouble();
    oPedido_proveedor->rSubotal = ui->txtrSubtotal->text().toDouble();
    oPedido_proveedor->rDto = ui->txtrImporteDescuento->text().toDouble();
    oPedido_proveedor->nIVA = ui->txtrImporteIva->text().toDouble();
    oPedido_proveedor->rRecTotal= ui->txtrTotalRecargoEq->text().toDouble();
    oPedido_proveedor->rTotal = ui->txtrTotal->text().toDouble();
    oPedido_proveedor->lEnviado = ui->chklEnviado->isChecked();
    oPedido_proveedor->lRecibido = ui->chkRecibido->isChecked();
    oPedido_proveedor->lRecibidoCompleto = ui->chklCompleto->isChecked();
    oPedido_proveedor->tComentario = ui->txttComentario->toPlainText();
    oPedido_proveedor->dFechaEntrega = ui->txtdFechaRecepcion->date();
    oPedido_proveedor->cDireccion1Entrega = ui->txtcDireccionEntrega->text();
    oPedido_proveedor->cDireccion2Entrega = ui->txtcDireccionEntrega_2->text();
    oPedido_proveedor->cCPEntrega = ui->txtcCpEntrega->text();
    oPedido_proveedor->cPoblacionEntrega = ui->txtcPoblacionEntrega->text();
    oPedido_proveedor->cProvinciaEntrega = ui->txtcProvinciaEntrega->text();
    oPedido_proveedor->cHorarioActivo = ui->txtHorarioEntrega->text();
    oPedido_proveedor->rBase1 = ui->txtrBase1->text().toDouble();
    oPedido_proveedor->rBase2 = ui->txtrBase2->text().toDouble();
    oPedido_proveedor->rBase3 = ui->txtrBase3->text().toDouble();
    oPedido_proveedor->rBase4 = ui->txtrBase4->text().toDouble();
    oPedido_proveedor->niva1 = ui->txtnIva1->text().toInt();
    oPedido_proveedor->niva2 = ui->txtnIva2->text().toInt();
    oPedido_proveedor->niva3 = ui->txtnIva3->text().toInt();
    oPedido_proveedor->niva4 = ui->txtnIva4->text().toInt();
    oPedido_proveedor->riva1 = ui->txtrIVA1->text().toDouble();
    oPedido_proveedor->riva2 = ui->txtrIVA2->text().toDouble();
    oPedido_proveedor->riva3 = ui->txtrIVA3->text().toDouble();
    oPedido_proveedor->riva4 = ui->txtrIVA4->text().toDouble();
    oPedido_proveedor->nrec1 = ui->txtnRec1->text().toInt();
    oPedido_proveedor->nrec2 = ui->txtnRec2->text().toInt();
    oPedido_proveedor->nrec3 = ui->txtnRec3->text().toInt();
    oPedido_proveedor->nrec4 = ui->txtnRec4->text().toInt();
    oPedido_proveedor->rrec1 = ui->txtrRecargoEq1->text().toDouble();
    oPedido_proveedor->rrec2 = ui->txtrRecargoEq2->text().toDouble();
    oPedido_proveedor->rrec3 = ui->txtrRecargoEq3->text().toDouble();
    oPedido_proveedor->rrec4 = ui->txtrRecargoEq4->text().toDouble();
    oPedido_proveedor->rtotal1 = ui->txtrTotal1->text().toDouble();
    oPedido_proveedor->rtotal2 = ui->txtrTotal2->text().toDouble();
    oPedido_proveedor->rtotal3 = ui->txtrTotal3->text().toDouble();
    oPedido_proveedor->rtotal4 = ui->txtrTotal4->text().toDouble();
}



void FrmPedidosProveedor::clear()
{
    ui->txtnPedido->clear();
    ui->lblSerie->clear();
    ui->txtdFecha->clear();
    ui->txtFechaLimite->clear();
    ui->txtcCodigoProveedor->clear();
    ui->txtcProveedor->clear();
    ui->txtcDireccion->clear();
    ui->txtcDireccion2->clear();
    ui->txtcCp->clear();
    ui->txtcPoblacion->clear();
    ui->txtcProvincia->clear();
    ui->txtcCif->clear();
    ui->txtrBase->clear();
    ui->txtrSubtotal->clear();
    ui->txtrImporteDescuento->clear();
    ui->txtrImporteIva->clear();
    ui->txtrRecargoEq1->clear();
    ui->txtrTotal->clear();
    ui->chklEnviado->setChecked(false);
    ui->chkRecibido->setChecked(false);
    ui->chklCompleto->setChecked(false);

    ui->txttComentario->clear();
    ui->txtdFechaRecepcion->clear();
    ui->txtcDireccionEntrega->clear();
    ui->txtcDireccionEntrega_2->clear();
    ui->txtcCpEntrega->clear();
    ui->txtcPoblacionEntrega->clear();
    ui->txtcProvinciaEntrega->clear();
    ui->txtHorarioEntrega->clear();
    ui->txtrBase1->clear();
    ui->txtrBase2->clear();
    ui->txtrBase3->clear();
    ui->txtrBase4->clear();
    ui->txtnIva1->clear();
    ui->txtnIva2->clear();
    ui->txtnIva3->clear();
    ui->txtnIva4->clear();
    ui->txtrIVA1->clear();
    ui->txtrIVA2->clear();
    ui->txtrIVA3->clear();
    ui->txtrIVA4->clear();
    ui->txtnRec1->clear();
    ui->txtnRec2->clear();
    ui->txtnRec3->clear();
    ui->txtnRec4->clear();
    ui->txtrRecargoEq1->clear();
    ui->txtrRecargoEq2->clear();
    ui->txtrRecargoEq3->clear();
    ui->txtrRecargoEq4->clear();
    ui->txtrTotal1->clear();
    ui->txtrTotal2->clear();
    ui->txtrTotal3->clear();
    ui->txtrTotal4->clear();
}

void FrmPedidosProveedor::resizeTable(int x)
{
    Q_UNUSED(x);
    helper.resizeTable();
}
