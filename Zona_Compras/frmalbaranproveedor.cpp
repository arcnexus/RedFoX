#include "frmalbaranproveedor.h"
#include "ui_frmalbaranproveedor.h"
#include "../Busquedas/db_consulta_view.h"


FrmAlbaranProveedor::FrmAlbaranProveedor(QWidget *parent, bool showCerrar) :
    QDialog(parent),
    ui(new Ui::FrmAlbaranProveedor),
    helper(this),
    prov(this)
{
    ui->setupUi(this);
    oAlbPro = new AlbaranProveedor(this);
    ui->cbo_pais->setModel(Configuracion_global->paises_model);
    ui->cbo_pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    helper.set_Tipo(true);
    helper.help_table(ui->Lineas);

    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(ui->chklRecargoEq,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));


    connect(&helper,SIGNAL(lineaReady(lineaDetalle*)),this,SLOT(lineaReady(lineaDetalle*)));
    connect(&helper,SIGNAL(lineaDeleted(int)),this,SLOT(lieaDeleted(int)));
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

    ui->txtnPorcentajeIva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtnPorcentajeIva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtnPorcentajeIva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtnPorcentajeIva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtnRec1->setText(Configuracion_global->reList.at(0));
    ui->txtnRec2->setText(Configuracion_global->reList.at(1));
    ui->txtnRec3->setText(Configuracion_global->reList.at(2));
    ui->txtnRec4->setText(Configuracion_global->reList.at(3));

    ui->btn_cerrar->setVisible(showCerrar);
    // ------------------------------
    // control estado widgets
    //-------------------------------
    bloquearcampos(true);
    ui->btnAnterior->setEnabled(false);
    ui->btnBorrar->setEnabled(false);
    ui->btnImprimir->setEnabled(false);
    ui->btnFacturar->setEnabled(false);
    oAlbPro->id = 0;
    ui->lblAlbaran->setText("");
    ui->lblproveedor->setText("");
}

FrmAlbaranProveedor::~FrmAlbaranProveedor()
{
    delete ui;
}

void FrmAlbaranProveedor::llenarProveedor(int id)
{
    prov.clear();

    prov.Recuperar("Select * from proveedores where id="+QString::number(id),0);
    ui->txtcCodigoProveedor->setText(prov.cCodigo);
    ui->txtcProveedor->setText(prov.cProveedor);
    ui->txtcDireccion->setText(prov.cDireccion1);
    ui->txtcDireccion2->setText(prov.cDireccion2);
    ui->txtcPoblacion->setText(prov.cPoblacion);
    ui->txtcProvincia->setText(prov.cProvincia);
    ui->txtcCp->setText(prov.cCP);
    ui->txtcCif->setText(prov.cCif);
    int ind = ui->cbo_pais->findText(Configuracion_global->Devolver_pais(prov.idpais));
    ui->cbo_pais->setCurrentIndex(ind);
    ui->chklRecargoEq->setChecked(prov.lRecargoEquivalencia);
    ui->lblproveedor->setText(prov.cProveedor);
    oAlbPro->id_Proveedor = prov.id;
}

void FrmAlbaranProveedor::bloquearcampos(bool estado)
{

    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList) {
        lineEdit->setReadOnly(estado);
    }
    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList) {
        ComboBox->setEnabled(!estado);
        //qDebug() << lineEdit->objectName();
    }
    // SpinBox
//    QList<QSpinBox *> SpinBoxList = this->findChildren<QSpinBox *>();
//    QSpinBox *SpinBox;
//    foreach (SpinBox, SpinBoxList) {
//        SpinBox->setReadOnly(!estado);
//        //qDebug() << lineEdit->objectName();
//   }
//DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(estado);
        //qDebug() << lineEdit->objectName();
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!estado);
        //qDebug() << lineEdit->objectName();
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(estado);
        //qDebug() << lineEdit->objectName();
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!estado);
        //qDebug() << lineEdit->objectName();
    }

    ui->btnAnadir->setEnabled(estado);
    ui->btnAnterior->setEnabled(estado);
    ui->btnBorrar->setEnabled(estado);
    ui->btnDeshacer->setEnabled(!estado);
    ui->btnEditar->setEnabled(estado);
    ui->btnGuardar->setEnabled(!estado);
    ui->btnSiguiente->setEnabled(estado);
    ui->btnAnadirLinea->setEnabled(!estado);

    ui->btn_borrarLinea->setEnabled(!estado);
    ui->botBuscarCliente->setEnabled(!estado);
    ui->btnFacturar->setEnabled(!estado);
    helper.blockTable(estado);
    // activo controles que deben estar activos.


}

void FrmAlbaranProveedor::buscar_proveeedor()
{
//    FrmBuscarProveedor buscar(this);
//    if(buscar.exec() == QDialog::Accepted)
    db_consulta_view consulta;
    QStringList campos;
    campos << "cProveedor";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Proveedores");
    consulta.set_db("Maya");
    consulta.set_SQL("select id, cCodigo,cProveedor,cCif,cPoblacion from proveedores");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Código") << tr("Proveedor") << tr("DNI/CIF/NIE") << tr("Población");
    tamanos <<"0" << "100" << "300" << "180" << "300";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de proveedores");
    if(consulta.exec())
    {
        int id_proveedor = consulta.get_id();
        llenarProveedor(id_proveedor);

        oAlbPro->id_Proveedor = id_proveedor;
    }
}


void FrmAlbaranProveedor::totalChanged(double base, double dto, double subTotal, double iva, double re, double total, QString moneda)
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
    this->moneda = moneda;
}

void FrmAlbaranProveedor::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtrBase1->setText(QString::number(base));
    ui->txtrIVA1->setText(QString::number(iva));
    ui->txtrRecargoEq1->setText(QString::number(re));
    ui->txtrTotal1->setText(QString::number(total));
}

void FrmAlbaranProveedor::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtrBase2->setText(QString::number(base));
    ui->txtrIVA2->setText(QString::number(iva));
    ui->txtrRecargoEq2->setText(QString::number(re));
    ui->txtrTotal2->setText(QString::number(total));
}

void FrmAlbaranProveedor::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtrBase3->setText(QString::number(base));
    ui->txtrIVA3->setText(QString::number(iva));
    ui->txtrRecargoEq3->setText(QString::number(re));
    ui->txtrTotal3->setText(QString::number(total));
}

void FrmAlbaranProveedor::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtrBase4->setText(QString::number(base));
    ui->txtrIVA4->setText(QString::number(iva));
    ui->txtrRecargoEq4->setText(QString::number(re));
    ui->txtrTotal4->setText(QString::number(total));
}

void FrmAlbaranProveedor::on_btnSiguiente_clicked()
{
    oAlbPro->Recuperar("Select * from alb_pro where id >"+QString::number(oAlbPro->id),1);
    llenar_campos();
    ui->btnAnterior->setEnabled(true);
    ui->btnBorrar->setEnabled(true);
    ui->btnImprimir->setEnabled(true);
}

void FrmAlbaranProveedor::on_btnAnterior_clicked()
{
    oAlbPro->Recuperar("Select * from alb_pro where id <"+QString::number(oAlbPro->id)+" order by id desc",2);
    llenar_campos();
}

void FrmAlbaranProveedor::llenar_campos()
{

    ui->txtcAlbaran->setText(oAlbPro->cAlbaran);
    ui->lblAlbaran->setText(oAlbPro->cAlbaran);
    ui->txtdFecha->setDate(oAlbPro->dFecha);
    ui->txtcProveedor->setText(oAlbPro->cProveedor);
    ui->txtcCif->setText(oAlbPro->cCifproveedor);
    llenarProveedor(oAlbPro->id_Proveedor);
    ui->txtrTotal1->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rTotal1,'f',2)));
    ui->txtrTotal2->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rTotal2,'f',2)));
    ui->txtrTotal3->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rTotal3,'f',2)));
    ui->txtrTotal4->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rTotal4,'f',2)));
    ui->txtnPorcentajeIva1->setText(QString::number(oAlbPro->nIva1));
    ui->txtnPorcentajeIva2->setText(QString::number(oAlbPro->nIva2));
    ui->txtnPorcentajeIva3->setText(QString::number(oAlbPro->nIva3));
    ui->txtnPorcentajeIva4->setText(QString::number(oAlbPro->nIva4));
    ui->txtrIVA1->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rIva1,'f',2)));
    ui->txtrIVA2->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rIva2,'f',2)));
    ui->txtrIVA3->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rIva3,'f',2)));
    ui->txtrIVA4->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rIva4,'f',2)));
    ui->txtrBase1->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rBase1,'f',2)));
    ui->txtrBase2->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rBase2,'f',2)));
    ui->txtrBase3->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rBase3,'f',2)));
    ui->txtrBase4->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rBase4,'f',2)));
    ui->txtcNumFra->setText(oAlbPro->cFactura);
    ui->txtrBase->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rBaseTotal,'f',2)));
    ui->txtrImporteIva->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rIvaTotal,'f',2)));
    ui->txtrTotal->setText(Configuracion_global->FormatoNumerico(QString::number(oAlbPro->rTotal,'f',2)));
    ui->txttComentario->setText(oAlbPro->tComentario);
    ui->txtnPedido->setText(QString::number(oAlbPro->nPedido));
    if(oAlbPro->cFactura.isEmpty())
        ui->btnFacturar->setEnabled(true);
    else
        ui->btnFacturar->setEnabled(false);
    this->id = oAlbPro->id;
    QString filter = QString("id_cab = '%1'").arg(oAlbPro->id);
    helper.fillTable("empresa","lin_alb_pro",filter);
    helper.resizeTable();
}
void FrmAlbaranProveedor::resizeTable(int x)
{
    Q_UNUSED(x);
    helper.resizeTable();
}
void FrmAlbaranProveedor::lineaReady(lineaDetalle * ld)
{
    //-----------------------------------------------------
    // Insertamos línea de pedido y controlamos acumulados
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

        QSqlQuery query_lin_alb_pro(QSqlDatabase::database("empresa"));
        query_lin_alb_pro.prepare("INSERT INTO lin_alb_pro (id_cab,id_articulo,codigo_articulo_proveedor,"
                                  "cDescripcion, nCantidad, rCoste,rSubTotal,nDto,rDto,nIva,"
                                  "rIva,rTotal) VALUES (:id_cab,:id_articulo,:codigo_articulo_proveedor,"
                                  ":descripcion,:cantidad,:coste_bruto,:subtotal_coste,:porc_dto,:dto,"
                                  ":porc_iva,:iva,:total);");
        query_lin_alb_pro.bindValue(":id_cab", oAlbPro->id);
        query_lin_alb_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_alb_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_alb_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_alb_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_alb_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_alb_pro.bindValue(":subtotal_coste",ld->subTotal);
        query_lin_alb_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_alb_pro.bindValue(":dto",ld->dto);
        query_lin_alb_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_alb_pro.bindValue(":iva",(ld->subTotal * ld->iva_perc)/100);
        query_lin_alb_pro.bindValue(":total",ld->total);
        if (!query_lin_alb_pro.exec()){
            ok_empresa = false;
            QMessageBox::warning(this,tr("Gestión de albaranes"),
                                 tr("Ocurrió un error al insertar la nueva línea: %1").arg(query_lin_alb_pro.lastError().text()),
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

        ld->idLinea = query_lin_alb_pro.lastInsertId().toInt();

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
        QSqlQuery query_lin_alb_pro(QSqlDatabase::database("empresa"));
        query_lin_alb_pro.prepare("UPDATE lin_alb_pro SET "
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

        query_lin_alb_pro.bindValue(":id_cab", oAlbPro->id);
        query_lin_alb_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_alb_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_alb_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_alb_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_alb_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_alb_pro.bindValue(":subtotal_coste",ld->subTotal);
        query_lin_alb_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_alb_pro.bindValue(":dto",ld->dto);
        query_lin_alb_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_alb_pro.bindValue(":iva",0); // importe iva hay que calcularlo
        query_lin_alb_pro.bindValue(":total",ld->total);
        query_lin_alb_pro.bindValue(":id",ld->idLinea);

        if (!query_lin_alb_pro.exec()) {
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Ocurrió un error al guardar la línea: %1").arg(query_lin_alb_pro.lastError().text()),
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


void FrmAlbaranProveedor::on_btnEditar_clicked()
{
    bloquearcampos(false);
    emit block();
    ui->txtcCodigoProveedor->setFocus();
}

void FrmAlbaranProveedor::on_btnGuardar_clicked()
{
    if(!ui->txtcAlbaran->text().isEmpty())
    {
        guardar_campos_en_objeto();
        oAlbPro->id =this->id;
        oAlbPro->guardar();

        //int regs = ui->Lineas->rowCount();
       // helper.saveTable(oPedido_proveedor->id,"empresa","lin_ped_pro");
        oAlbPro->Recuperar(oAlbPro->id);
        llenar_campos();
        bloquearcampos(true);
        emit unblock();
    } else
    {
        QMessageBox::information(this,tr("Gestión de Albaranes de proveedor"),
                                 tr("Debe especificar el numero de albarán antes de guardar"),tr("Aceptar"));
        ui->txtcAlbaran->setFocus();
    }
}

void FrmAlbaranProveedor::guardar_campos_en_objeto()
{
    oAlbPro->cAlbaran = ui->txtcAlbaran->text();
    oAlbPro->dFecha = ui->txtdFecha->date();
    oAlbPro->cProveedor = ui->txtcProveedor->text();
    oAlbPro->cCifproveedor = ui->txtcCif->text();
    oAlbPro->rTotal1 = ui->txtrTotal1->text().replace(",",".").toDouble();
    oAlbPro->rTotal2 = ui->txtrTotal2->text().replace(",",".").toDouble();
    oAlbPro->rTotal3 = ui->txtrTotal3->text().replace(",",".").toDouble();
    oAlbPro->rTotal4 = ui->txtrTotal4->text().replace(",",".").toDouble();
    oAlbPro->nIva1 = ui->txtnPorcentajeIva1->text().toDouble();
    oAlbPro->nIva2 = ui->txtnPorcentajeIva2->text().toDouble();
    oAlbPro->nIva3 = ui->txtnPorcentajeIva3->text().toDouble();
    oAlbPro->nIva4 = ui->txtnPorcentajeIva4->text().toDouble();
    oAlbPro->rIva1 = ui->txtrIVA1->text().replace(",",".").toDouble();
    oAlbPro->rIva2 = ui->txtrIVA2->text().replace(",",".").toDouble();
    oAlbPro->rIva3 = ui->txtrIVA3->text().replace(",",".").toDouble();
    oAlbPro->rIva4 = ui->txtrIVA4->text().replace(",",".").toDouble();
    oAlbPro->rBase1 = ui->txtrBase1->text().replace(",",".").toDouble();
    oAlbPro->rBase2 = ui->txtrBase2->text().replace(",",".").toDouble();
    oAlbPro->rBase3 = ui->txtrBase3->text().replace(",",".").toDouble();
    oAlbPro->rBase4 = ui->txtrBase4->text().replace(",",".").toDouble();
    oAlbPro->cFactura = ui->txtcNumFra->text().replace(",",".").toDouble();
    oAlbPro->rBaseTotal = ui->txtrBase->text().replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->rIvaTotal = ui->txtrImporteIva->text().replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->rTotal = ui->txtrTotal->text().replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->tComentario = ui->txttComentario->toPlainText();
}



void FrmAlbaranProveedor::on_btnBuscar_clicked()
{
    db_consulta_view busca_albaran;
    busca_albaran.set_db("empresa");
    QStringList cCampo,cCabecera;
    QVariantList vTamanos;
    cCampo << "cAlbaran" << "cProveedor";
    cCabecera <<"Albarán" << "Fecha" << "Proveedor" << "Total";
    vTamanos << 0 << 120 << 130 << 250 <<120;
    busca_albaran.set_SQL("Select id, cAlbaran,dFecha, cProveedor,rTotal from alb_pro");
    busca_albaran.set_campoBusqueda(cCampo);
    busca_albaran.set_headers(cCabecera);

    busca_albaran.set_tamano_columnas(vTamanos);
    busca_albaran.set_texto_tabla(tr("Busqueda de Albaranes de proveedor"));
    busca_albaran.set_titulo("Busquedas....");
    if(busca_albaran.exec() == QMessageBox::Accepted)
    {
        int id_alb = busca_albaran.get_id();
        oAlbPro->Recuperar(id_alb);
        llenar_campos();

        ui->btnAnterior->setEnabled(true);
        ui->btnBorrar->setEnabled(true);
        ui->btnImprimir->setEnabled(true);
    } else
    {
        QMessageBox::warning(this,tr("Búsqueda de albaranes"),
                             tr("No se localizó o no se especifico ningún albarán "), tr("Aceptar"));
    }
}

void FrmAlbaranProveedor::on_botBuscarCliente_clicked()
{
    db_consulta_view busca_prov;
    busca_prov.set_db("Maya");
    QStringList cCampo,cCabecera;
    QVariantList vTamanos;
    cCampo << "cProveedor" << "cCodigo" <<"cCif";
    cCabecera <<"Código" << "Proveedor" << "CIF/NIF/NIE";
    vTamanos << 0 << 120 << 250 <<120;
    busca_prov.set_SQL("Select Id, cCodigo,cProveedor,cCif from proveedores");
    busca_prov.set_campoBusqueda(cCampo);
    busca_prov.set_headers(cCabecera);

    busca_prov.set_tamano_columnas(vTamanos);
    busca_prov.set_texto_tabla(tr("Busqueda  de proveedor"));
    busca_prov.set_titulo("Busquedas...");
    if(busca_prov.exec() == QMessageBox::Accepted)
    {
        int id_prov = busca_prov.get_id();
        llenarProveedor(id_prov);
    }
}

void FrmAlbaranProveedor::on_btnAnadir_clicked()
{
    int id = oAlbPro->anadir();
    oAlbPro->Recuperar(id);
    this->id = id;
    emit block();
    llenar_campos();
    bloquearcampos(false);
    ui->txtcCodigoProveedor->setFocus();
}
