#include "frmfacturasproveedor.h"
#include "ui_frmfacturasproveedor.h"

FrmFacturasProveedor::FrmFacturasProveedor(QWidget *parent, bool showCerrar) :
    QDialog(parent),
    ui(new Ui::FrmFacturasProveedor),
    helper(this),
    prov(this)
{
    ui->setupUi(this);
    oFacPro = new FacturasProveedor(this);
    oFacPro->id = 0;

    ui->combo_pais->setModel(Configuracion_global->paises_model);
    ui->combo_pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    helper.set_Tipo(true);
    helper.help_table(ui->Lineas);

    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(ui->chklRecargoEq,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

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

    ui->txtnPorcentajeIva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtnPorcentajeIva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtnPorcentajeIva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtnPorcentajeIva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtnRec1->setText(Configuracion_global->reList.at(0));
    ui->txtnRec2->setText(Configuracion_global->reList.at(1));
    ui->txtnRec3->setText(Configuracion_global->reList.at(2));
    ui->txtnRec4->setText(Configuracion_global->reList.at(3));

    ui->btn_cerrar->setVisible(showCerrar);
}

FrmFacturasProveedor::~FrmFacturasProveedor()
{
    delete ui;
}


void FrmFacturasProveedor::lineaReady(lineaDetalle * ld)
{
    //-----------------------------------------------------
    // Insertamos línea de factura y controlamos acumulados
    //-----------------------------------------------------

    bool ok_empresa,ok_Maya;
    ok_empresa = true;
    ok_Maya = true;
    QSqlDatabase::database("empresa").transaction();
    QSqlDatabase::database("Maya").transaction();
    if (ld->idLinea == -1)
    {
        //qDebug()<< ld->idLinea;
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("select id from articulos where cCodigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery query_lin_fac_pro(QSqlDatabase::database("empresa"));
        query_lin_fac_pro.prepare("INSERT INTO lin_fac_pro (id_cab,id_articulo,codigo_articulo_proveedor,"
                                  "cDescripcion, nCantidad, rCoste,rSubTotal,nDto,rDto,nIva,"
                                  "rIva,rTotal) VALUES (:id_cab,:id_articulo,:codigo_articulo_proveedor,"
                                  ":descripcion,:cantidad,:coste_bruto,:subtotal_coste,:porc_dto,:dto,"
                                  ":porc_iva,:iva,:total);");
        query_lin_fac_pro.bindValue(":id_cab", oFacPro->id);
        query_lin_fac_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_fac_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_fac_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_fac_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_fac_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_fac_pro.bindValue(":subtotal_coste",ld->subTotal);
        query_lin_fac_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_fac_pro.bindValue(":dto",ld->dto);
        query_lin_fac_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_fac_pro.bindValue(":iva",(ld->subTotal * ld->iva_perc)/100);
        query_lin_fac_pro.bindValue(":total",ld->total);
        if (!query_lin_fac_pro.exec()){
            ok_empresa = false;
            QMessageBox::warning(this,tr("Gestión de albaranes"),
                                 tr("Ocurrió un error al insertar la nueva línea: %1").arg(query_lin_fac_pro.lastError().text()),
                                 tr("Aceptar"));
        }
        // ---------------------------------
        // Actualización stock y acumulados
        //----------------------------------

        queryArticulos.prepare("update articulos set nCantidadPendienteRecibir = nCantidadPendienteRecibir+:nRecibir, "
                               " nStockReal = nStockReal + :nRecibir2 where cCodigo=:codigo");
        queryArticulos.bindValue(":nRecibir",ld->cantidad);
        queryArticulos.bindValue(":nRecibir2",ld->cantidad);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec() && ok_empresa){
            QSqlDatabase::database("empresa").commit();
            QSqlDatabase::database("Maya").commit();
        } else
        {
            QSqlDatabase::database("empresa").rollback();
            QSqlDatabase::database("Maya").rollback();
        }

        ld->idLinea = query_lin_fac_pro.lastInsertId().toInt();

    } else
    {
        // --------------------------
        // Descuento unidades pedidas
        //---------------------------
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("update articulos set "
                               "nStockReal = nStockReal - :nRecibir where cCodigo=:codigo");
        queryArticulos.bindValue(":nRecibir",ld->cantidad_old);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if (!queryArticulos.exec())
        {
            ok_Maya = false;
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Se produjo un error al actualizar stock : %1").arg(queryArticulos.lastError().text()));

        }

        queryArticulos.prepare("select id from articulos where cCodigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;
        QSqlQuery query_lin_fac_pro(QSqlDatabase::database("empresa"));
        query_lin_fac_pro.prepare("UPDATE lin_fac_pro SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo_articulo_proveedor =:codigo_articulo_proveedor,"
                                  "cDescripcion =:descripcion,"
                                  "nCantidad =:cantidad,"
                                  "rCoste =:coste_bruto,"
                                  "rSubtotal =:subtotal_coste,"
                                  "nDto =:porc_dto,"
                                  "rDto =:dto,"
                                  "nIva =:porc_iva,"
                                  "rIva =:iva,"
                                  "rTotal =:total "
                                  "WHERE id = :id;");

        query_lin_fac_pro.bindValue(":id_cab", oFacPro->id);
        query_lin_fac_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_fac_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_fac_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_fac_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_fac_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_fac_pro.bindValue(":subtotal_coste",ld->subTotal);
        query_lin_fac_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_fac_pro.bindValue(":dto",ld->dto);
        query_lin_fac_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_fac_pro.bindValue(":iva",0); // importe iva hay que calcularlo
        query_lin_fac_pro.bindValue(":total",ld->total);
        query_lin_fac_pro.bindValue(":id",ld->idLinea);

        if (!query_lin_fac_pro.exec()) {
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Ocurrió un error al guardar la línea: %1").arg(query_lin_fac_pro.lastError().text()),
                                 tr("Aceptar"));
            ok_empresa = false;
        }
        // ---------------------------------
        // Actualización stock y acumulados
        //----------------------------------
        queryArticulos.prepare("update articulos set "
                               "nStockReal = nStockReal + :nRecibir where cCodigo=:codigo");
        queryArticulos.bindValue(":nRecibir",ld->cantidad);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec() && ok_empresa && ok_Maya){
            QSqlDatabase::database("empresa").commit();
            QSqlDatabase::database("Maya").commit();
        } else
        {
            QSqlDatabase::database("empresa").rollback();
            QSqlDatabase::database("Maya").rollback();
        }
    }
    ld->cantidad_old = ld->cantidad;
}

void FrmFacturasProveedor::bloquearcampos(bool state)
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
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
//    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
//    QSpinBox *SpinBox;
//    foreach (SpinBox, SpinBoxList) {
//        SpinBox->setReadOnly(!state);
//        //qDebug() << lineEdit->objectName();
//   }
//DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(state);
        //qDebug() << lineEdit->objectName();
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!state);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(state);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!state);
        //qDebug() << lineEdit->objectName();
    }

    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    //ui->btnBorrar->setEnabled(state);
    ui->btnDeshacer->setEnabled(!state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->btnAnadirLinea->setEnabled(!state);

    ui->btn_borrarLinea->setEnabled(!state);
    ui->botBuscarCliente->setEnabled(!state);
    helper.blockTable(state);
    // activo controles que deben estar activos.



}



void FrmFacturasProveedor::llenarProveedor(int id)
{
    prov.Recuperar("Select * from proveedores where id="+QString::number(id),1);
    ui->txtcCodigoProveedor->setText(prov.cCodigo);
    ui->txtcProveedor->setText(prov.cProveedor);
    ui->txtcDireccion->setText(prov.cDireccion1);
    ui->txtcDireccion2->setText(prov.cDireccion2);
    ui->txtcPoblacion->setText(prov.cPoblacion);
    ui->txtcProvincia->setText(prov.cProvincia);
    ui->txtcCp->setText(prov.cCP);
    ui->txtcCif->setText(prov.cCif);
    ui->combo_pais->setCurrentText(Configuracion::Devolver_pais(prov.idpais));
}

void FrmFacturasProveedor::totalChanged(double base, double dto, double subTotal, double iva, double re, double total, QString moneda)
{
    ui->txtrBase->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2))+moneda);
    ui->txtrImporteDescuento->setText(Configuracion_global->FormatoNumerico(QString::number(dto,'f',2))+moneda);
    ui->txtrSubtotal->setText(Configuracion_global->FormatoNumerico(QString::number(subTotal,'f',2))+moneda);
    ui->txtrImporteIva->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2))+moneda);
    //ui->txtrTotalRecargoEq_2->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2))+moneda);
    ui->txtrTotal->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2))+moneda);
    ui->lbl_total->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2))+moneda);

    ui->txtrBaseTotal_2->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2))+moneda);
    ui->txtrTotalIVA_2->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2))+moneda);
    ui->txtrTotalRecargoEq_2->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2))+moneda);
    ui->txtrTotal_2->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2))+moneda);
}

void FrmFacturasProveedor::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtrBase1->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2)));
    ui->txtrIVA1->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2)));
    ui->txtrRecargoEq1->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2)));
    ui->txtrTotal1->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2)));
}

void FrmFacturasProveedor::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtrBase2->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2)));
    ui->txtrIVA2->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2)));
    ui->txtrRecargoEq2->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2)));
    ui->txtrTotal2->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2)));
}

void FrmFacturasProveedor::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtrBase3->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2)));
    ui->txtrIVA3->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2)));
    ui->txtrRecargoEq3->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2)));
    ui->txtrTotal3->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2)));
}

void FrmFacturasProveedor::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtrBase4->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2)));
    ui->txtrIVA4->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2)));
    ui->txtrRecargoEq4->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2)));
    ui->txtrTotal4->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2)));
}



void FrmFacturasProveedor::on_btnAnadir_clicked()
{
    int id = oFacPro->anadir_factura();
    oFacPro->recuperar_factura(id);
    this->id = id;
    emit block();
    llenar_campos();
    bloquearcampos(false);
    ui->txtcCodigoProveedor->setFocus();

}

void FrmFacturasProveedor::llenar_campos()
{
    ui->txtcFactura->setText(oFacPro->cFactura);
    ui->lblFactura->setText(oFacPro->cFactura);
    ui->txtdFecha->setDate(oFacPro->dFecha);
    ui->txtdRecepcion->setDate(oFacPro->dRecepcion);
    ui->txtcPedidoProveedor->setText(oFacPro->cPedido);
    ui->txtcProveedor->setText(oFacPro->cProveedor);
    ui->txtcCif->setText(oFacPro->cCifProveedor);
    ui->chkRetencionIRPF->setChecked(oFacPro->lRetencionIRPF);
    ui->chklRecargoEq->setChecked(oFacPro->lRecargoEquivalencia);
    ui->txtrBase1->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rBase1,'f',2)));
    ui->txtrBase2->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rBase2,'f',2)));
    ui->txtrBase3->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rBase3,'f',2)));
    ui->txtrBase4->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rBase4,'f',2)));
    ui->txtnPorcentajeIva1->setText(QString::number(oFacPro->nPorcIva1,'f',2));
    ui->txtnPorcentajeIva2->setText(QString::number(oFacPro->nPorcIva2,'f',2));
    ui->txtnPorcentajeIva3->setText(QString::number(oFacPro->nPorcIva3,'f',2));
    ui->txtnPorcentajeIva4->setText(QString::number(oFacPro->nPorcIva4,'f',2));
    ui->txtrIVA1->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rIVA1,'f',2)));
    ui->txtrIVA2->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rIVA2,'f',2)));
    ui->txtrIVA3->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rIVA3,'f',2)));
    ui->txtrIVA4->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rIVA4,'f',2)));
    ui->txtnRec1->setText(QString::number(oFacPro->nREC1));
    ui->txtnRec2->setText(QString::number(oFacPro->nREC2));
    ui->txtnRec3->setText(QString::number(oFacPro->nREC3));
    ui->txtnRec4->setText(QString::number(oFacPro->nREC4));
    ui->txtrRecargoEq1->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rREC1)));
    ui->txtrRecargoEq2->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rREC2)));
    ui->txtrRecargoEq3->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rREC3)));
    ui->txtrRecargoEq4->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rREC4)));
    ui->txtrTotal1->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rTotal1)));
    ui->txtrTotal2->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rTotal2)));
    ui->txtrTotal3->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rTotal3)));
    ui->txtrTotal4->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rTotal4)));
    ui->txtrBaseTotal_2->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rTotalBase,'f',2)));
    ui->txtrTotalIVA_2->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rTotalIVA,'f',2)));
    ui->txtrImporteIRPF->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rTotalRetencion,'f',2)));
    ui->txtrImporteRE->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rTotalRecargoEq,'f',2)));
    ui->txtrTotal->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rTotal)));
    int index = ui->cbocFormaPago->findText(oFacPro->cFormaPago);
    ui->cbocFormaPago->setCurrentIndex(index);
    ui->txtImportePagadoFactura->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rImportePagadoTotal,'f',2)));
    QString cGasto = Configuracion_global->Devolver_tipo_gasto(oFacPro->Id_TipoGasto);
    index = ui->cbo_grupo_gasto->findText(cGasto);
    ui->cbo_grupo_gasto->setCurrentIndex(index);
    ui->txttComentario->setPlainText(oFacPro->tComentario);
    ui->lblFacturaPagada->setVisible(oFacPro->lPagado);
    ui->txtrImportePendiente->setText(Configuracion_global->FormatoNumerico(QString::number(oFacPro->rImporteDeudaPendiente)));
    ui->txtAlbaran->setText(oFacPro->cCAlbaran);
}

void FrmFacturasProveedor::llenar_fac_pro()
{

    oFacPro->cFactura = ui->txtcFactura->text();
    oFacPro->dFecha = ui->txtdFecha->date();
    oFacPro->dRecepcion = ui->txtdRecepcion->date();
    oFacPro->cPedido = ui->txtcPedidoProveedor->text();
    oFacPro->cProveedor = ui->txtcProveedor->text();
    oFacPro->cCifProveedor = ui->txtcCif->text();
    oFacPro->lRetencionIRPF = ui->chkRetencionIRPF->isChecked();
    oFacPro->lRecargoEquivalencia = ui->chklRecargoEq->isChecked();
    oFacPro->rBase1 = ui->txtrBase1->text().replace(",",".").toDouble();
    oFacPro->rBase2 = ui->txtrBase2->text().replace(",",".").toDouble();
    oFacPro->rBase3 = ui->txtrBase3->text().replace(",",".").toDouble();
    oFacPro->rBase4 = ui->txtrBase4->text().replace(",",".").toDouble();
    oFacPro->nPorcIva1 = ui->txtnPorcentajeIva1->text().toDouble();
    oFacPro->nPorcIva2 = ui->txtnPorcentajeIva2->text().toDouble();
    oFacPro->nPorcIva3 = ui->txtnPorcentajeIva3->text().toDouble();
    oFacPro->nPorcIva4 = ui->txtnPorcentajeIva4->text().toDouble();
    oFacPro->rIVA1 = ui->txtrIVA1->text().replace(",",".").toDouble();
    oFacPro->rIVA2 = ui->txtrIVA2->text().replace(",",".").toDouble();
    oFacPro->rIVA3 = ui->txtrIVA3->text().replace(",",".").toDouble();
    oFacPro->rIVA4 = ui->txtrIVA4->text().replace(",",".").toDouble();
    oFacPro->nREC1 = ui->txtnRec1->text().toDouble();
    oFacPro->nREC2 = ui->txtnRec2->text().toDouble();
    oFacPro->nREC3 = ui->txtnRec3->text().toDouble();
    oFacPro->nREC4 = ui->txtnRec4->text().toDouble();
    oFacPro->rREC1 = ui->txtrRecargoEq1->text().replace(",",".").toDouble();
    oFacPro->rREC2 = ui->txtrRecargoEq2->text().replace(",",".").toDouble();
    oFacPro->rREC3 = ui->txtrRecargoEq3->text().replace(",",".").toDouble();
    oFacPro->rREC4 = ui->txtrRecargoEq4->text().replace(",",".").toDouble();
    oFacPro->rTotal1 = ui->txtrTotal1->text().replace(",",".").toDouble();
    oFacPro->rTotal2 = ui->txtrTotal2->text().replace(",",".").toDouble();
    oFacPro->rTotal3 = ui->txtrTotal3->text().replace(",",".").toDouble();
    oFacPro->rTotal4 = ui->txtrTotal4->text().replace(",",".").toDouble();
    oFacPro->rTotalBase = ui->txtrBaseTotal_2->text().replace(",",".").toDouble();
    oFacPro->rTotalIVA = ui->txtrImporteIva->text().replace(",",".").toDouble();
    oFacPro->rTotalRetencion = ui->txtrImporteIRPF->text().replace(",",".").toDouble();
    oFacPro->rTotalRecargoEq = ui->txtrImporteRE->text().replace(",",".").toDouble();
    oFacPro->rTotal = ui->txtrTotal->text().replace(",",".").toDouble();
    oFacPro->cFormaPago = ui->cbocFormaPago->currentText();
    oFacPro->rImportePagadoTotal = ui->txtImportePagadoFactura->text().replace(",",".").toDouble();
    oFacPro->Id_TipoGasto = Configuracion_global->Devolver_id_tipo_gasto(ui->cbo_grupo_gasto->currentText());
    oFacPro->tComentario = ui->txttComentario->toPlainText();
    oFacPro->rImporteDeudaPendiente = ui->txtrImportePendiente->text().replace(",",".").toDouble();
    oFacPro->cCAlbaran = ui->txtAlbaran->text();
}

void FrmFacturasProveedor::on_btnGuardar_clicked()
{
    if(!ui->txtcFactura->text().isEmpty())
    {
        llenar_fac_pro();
        oFacPro->id =this->id;
        oFacPro->guardar_factura();

        //int regs = ui->Lineas->rowCount();
       // helper.saveTable(oPedido_proveedor->id,"empresa","lin_ped_pro");
        oFacPro->recuperar_factura(oFacPro->id);
        llenar_fac_pro();
        bloquearcampos(true);
        emit unblock();
    } else
    {
        QMessageBox::information(this,tr("Gestión de Facturas de proveedor"),
                                 tr("Debe especificar el numero de Factura antes de guardar"),tr("Aceptar"));
        ui->txtcFactura->setFocus();
    }
}

void FrmFacturasProveedor::on_btnSiguiente_clicked()
{
    oFacPro->recuperar_factura("Select * from fac_pro where id >"+QString::number(oFacPro->id)+" limit 0,1",1);
    llenar_campos();
    ui->btnAnterior->setEnabled(true);
    ui->btnImprimir->setEnabled(true);
}

void FrmFacturasProveedor::on_btnAnterior_clicked()
{
    oFacPro->recuperar_factura("Select * from fac_pro where id <"+QString::number(oFacPro->id)+" order by id desc limit 0,1",2);
    llenar_campos();
}
