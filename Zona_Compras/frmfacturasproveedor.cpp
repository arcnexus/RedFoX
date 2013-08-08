#include "frmfacturasproveedor.h"
#include "ui_frmfacturasproveedor.h"
#include "../Busquedas/db_consulta_view.h"

FrmFacturasProveedor::FrmFacturasProveedor(QWidget *parent, bool showCerrar) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmFacturasProveedor),
    helper(this),
    prov(this),
    toolButton(tr("Facturas \nProv."),":/Icons/PNG/Factura_pro.png",this),
    menuButton(QIcon(":/Icons/PNG/Factura_pro.png"),tr("Facturas Prov."),this),
    push(new QPushButton(QIcon(":/Icons/PNG/Factura_pro.png"),"",this))
{
    ui->setupUi(this);
    oFacPro = new FacturasProveedor(this);
    oFacPro->id = 0;
    bloquearcampos(true);

    ui->combo_pais->setModel(Configuracion_global->paises_model);
    ui->combo_pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de facturas de proveedores/acreeedores"));

    helper.set_Tipo(true);
    helper.help_table(ui->Lineas);

    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(ui->chklporc_rec,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

    connect(ui->tabWidget_2,SIGNAL(currentChanged(int)),this,SLOT(resizeTable(int)));

    connect(&helper,SIGNAL(lineaReady(lineaDetalle*)),this,SLOT(lineaReady(lineaDetalle*)));

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


    llenar_campos_iva();

    //-----------------------------
    // Estado controles al iniciar
    //-----------------------------
    ui->btn_cerrar->setVisible(showCerrar);
    QSqlQueryModel *modeltipogasto = new QSqlQueryModel(this);
    modeltipogasto->setQuery("select descripcion from grupos_gasto",Configuracion_global->groupDB);
    ui->cbo_grupo_gasto->setModel(modeltipogasto);
    ui->cbo_grupo_gasto->setCurrentIndex(-1);
    QSqlQueryModel *modelfp = new QSqlQueryModel(this);

    modelfp->setQuery("select forma_pago from formpago",Configuracion_global->groupDB);
    ui->cboforma_pago->setModel(modelfp);
    ui->cboforma_pago->setCurrentIndex(-1);

    ui->lbcontabilizada->setVisible(false);
    ui->lblFacturaPagada->setVisible(false);
    ui->lblIRPF_4->setVisible(false);
    ui->lblproveedor->setText("");
    //-----------
    //  tabla
    //-----------
    m = new QSqlQueryModel(this);
    m->setQuery("select id,factura,fecha,cif_proveedor,proveedor from fac_pro order by factura desc",
                Configuracion_global->empresaDB);
    ui->tabla->setModel(m);
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
    Configuracion_global->empresaDB.transaction();
    Configuracion_global->groupDB.transaction();
    if (ld->idLinea == -1)
    {
        //qDebug()<< ld->idLinea;
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery query_lin_fac_pro(Configuracion_global->empresaDB);
        query_lin_fac_pro.prepare("INSERT INTO lin_fac_pro (id_cab,id_articulo,codigo,"
                                  "descripcion, cantidad, precio,subtotal,porc_dto,dto,porc_iva,"
                                  "iva,total) VALUES (:id_cab,:id_articulo,:codigo,"
                                  ":descripcion,:cantidad,:precio,:subtotal,:porc_dto,:dto,"
                                  ":porc_iva,:iva,:total);");
        query_lin_fac_pro.bindValue(":id_cab", oFacPro->id);
        query_lin_fac_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_fac_pro.bindValue(":codigo",ld->codigo);
        query_lin_fac_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_fac_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_fac_pro.bindValue(":precio",ld->precio);
        query_lin_fac_pro.bindValue(":subtotal",ld->subtotal);
        query_lin_fac_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_fac_pro.bindValue(":dto",ld->dto);
        query_lin_fac_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_fac_pro.bindValue(":iva",(ld->subtotal * ld->iva_perc)/100);
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

        queryArticulos.prepare("update articulos set cantidad_pendiente_recibir = cantidad_pendiente_recibir+:cantidad, "
                               " stock_real = stock_real + :cantidad2 where codigo=:codigo");
        queryArticulos.bindValue(":cantidad",ld->cantidad);
        queryArticulos.bindValue(":cantidad2",ld->cantidad);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec() && ok_empresa){
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        } else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
        }

        ld->idLinea = query_lin_fac_pro.lastInsertId().toInt();

    } else
    {
        // --------------------------
        // Descuento unidades pedidas
        //---------------------------
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("update articulos set "
                               "stock_real = stock_real - :cantidad where codigo=:codigo");
        queryArticulos.bindValue(":cantidad",ld->cantidad_old);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if (!queryArticulos.exec())
        {
            ok_Maya = false;
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Se produjo un error al actualizar stock : %1").arg(queryArticulos.lastError().text()));

        }

        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;
        QSqlQuery query_lin_fac_pro(Configuracion_global->empresaDB);
        query_lin_fac_pro.prepare("UPDATE lin_fac_pro SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo =:codigo,"
                                  "descripcion =:descripcion,"
                                  "cantidad =:cantidad,"
                                  "precio =:precio,"
                                  "subtotal =:subtotal,"
                                  "porc_dto =:porc_dto,"
                                  "dto =:dto,"
                                  "porc_iva =:porc_iva,"
                                  "iva =:iva,"
                                  "total =:total "
                                  "WHERE id = :id;");

        query_lin_fac_pro.bindValue(":id_cab", oFacPro->id);
        query_lin_fac_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_fac_pro.bindValue(":codigo",ld->codigo);
        query_lin_fac_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_fac_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_fac_pro.bindValue(":precio",ld->precio);
        query_lin_fac_pro.bindValue(":subtotal",ld->subtotal);
        query_lin_fac_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_fac_pro.bindValue(":dto",ld->dto);
        query_lin_fac_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_fac_pro.bindValue(":iva",(ld->subtotal * ld->iva_perc)/100);
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
                               "stock_real = stock_real + :cantidad_recibir where codigo=:codigo");
        queryArticulos.bindValue(":cantidad_recibir",ld->cantidad);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec() && ok_empresa && ok_Maya){
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        } else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
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
    prov.Recuperar("Select * from proveedores where id="+QString::number(id),0);

    ui->txtcodigo_proveedor->setText(prov.codigo);
    ui->txtproveedor->setText(prov.proveedor);
    ui->lblproveedor->setText(prov.proveedor);
    ui->txtdireccion1->setText(prov.direccion1);
    ui->txtdireccion2->setText(prov.direccion2);
    ui->txtpoblacion->setText(prov.poblacion);
    ui->txtprovincia->setText(prov.provincia);
    ui->txtcp->setText(prov.cp);
    ui->txtcif->setText(prov.cif);
    ui->combo_pais->setCurrentText(Configuracion::Devolver_pais(prov.id_pais));
    oFacPro->id_proveedor = prov.id;
}

void FrmFacturasProveedor::totalChanged(double base, double dto, double subtotal, double iva, double re, double total, QString moneda)
{
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',2))+moneda);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',2))+moneda);
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    //ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);
    ui->lbl_total->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);
}

void FrmFacturasProveedor::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmFacturasProveedor::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmFacturasProveedor::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmFacturasProveedor::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}


void FrmFacturasProveedor::on_btnAnadir_clicked()
{
    int id_fac;
    id_fac= oFacPro->anadir_factura();
    oFacPro->recuperar_factura(id_fac);
    this->id = id_fac;
    emit block();
    llenar_campos();
    bloquearcampos(false);
    ui->txtcodigo_proveedor->setFocus();
}

void FrmFacturasProveedor::llenar_campos()
{
    ui->txtfactura->setText(oFacPro->factura);
    ui->lblFactura->setText(oFacPro->factura);
    ui->txtfecha->setDate(oFacPro->fecha);
    ui->txtrecepcion->setDate(oFacPro->fecha_recepcion);
    ui->txtpedidoProveedor->setText(oFacPro->pedido);
    ui->txtproveedor->setText(oFacPro->proveedor);
    llenarProveedor(oFacPro->id_proveedor);
    ui->txtcif->setText(oFacPro->cif_proveedor);
    ui->chkRetencioirpf->setChecked(oFacPro->retencion_irpf);
    ui->chklporc_rec->setChecked(oFacPro->recargo_equivalencia);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->base1,'f',2)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->base2,'f',2)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->base3,'f',2)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->base4,'f',2)));
    ui->txtporc_iva1->setText(QString::number(oFacPro->porc_iva1,'f',2));
    ui->txtporc_iva2->setText(QString::number(oFacPro->porc_iva2,'f',2));
    ui->txtporc_iva3->setText(QString::number(oFacPro->porc_iva3,'f',2));
    ui->txtporc_iva4->setText(QString::number(oFacPro->porc_iva4,'f',2));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->iva1,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->iva2,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->iva3,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->iva4,'f',2)));
    ui->txtporc_rec1->setText(QString::number(oFacPro->porc_rec1));
    ui->txtporc_rec2->setText(QString::number(oFacPro->porc_rec2));
    ui->txtporc_rec3->setText(QString::number(oFacPro->porc_rec3));
    ui->txtporc_rec4->setText(QString::number(oFacPro->porc_rec4));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->rec1)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->rec2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->rec3)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->rec4)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total1)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total3)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total4)));
    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total_base,'f',2)));
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total_iva,'f',2)));
    ui->txtimporte_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total_retencion,'f',2)));
    ui->txtimporteRE->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total_recargo,'f',2)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total)));
    int index = ui->cboforma_pago->findText(oFacPro->forma_pago);
    ui->cboforma_pago->setCurrentIndex(index);
    ui->txtImportePagadoFactura->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->importe_pagado_total,'f',2)));
    QString cGasto = Configuracion_global->Devolver_tipo_gasto(oFacPro->id_tipo_gasto);
    index = ui->cbo_grupo_gasto->findText(cGasto);
    ui->cbo_grupo_gasto->setCurrentIndex(index);
    ui->txtcomentario->setPlainText(oFacPro->comentario);
    ui->lblFacturaPagada->setVisible(oFacPro->pagado);
    ui->txtimporte_pendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->importe_deuda_pendiente)));
    ui->txtAlbaran->setText(oFacPro->albaran);
    this->id = oFacPro->id;
    ui->txtGastoDist1->setText(oFacPro->desc_gasto1);
    ui->txtGastoDist2->setText(oFacPro->desc_gasto2);
    ui->txtGastoDist3->setText(oFacPro->desc_gasto3);
    ui->SpinGastoDist1->setValue(oFacPro->imp_gasto1);
    ui->SpinGastoDist2->setValue(oFacPro->imp_gasto2);
    ui->SpinGastoDist3->setValue(oFacPro->imp_gasto3);
    ui->GastoDist_toLineas->setChecked(oFacPro->gasto_to_coste);
    ui->GastoDist_toTotal->setChecked(!oFacPro->gasto_to_coste);

    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();

    QString filter = QString("id_cab = '%1'").arg(oFacPro->id);
    helper.fillTable("empresa","lin_fac_pro",filter);
    helper.resizeTable();

}

void FrmFacturasProveedor::resizeTable(int x)
{
    Q_UNUSED(x);
    helper.resizeTable();
}

void FrmFacturasProveedor::llenar_fac_pro()
{

    oFacPro->factura = ui->txtfactura->text();
    oFacPro->fecha = ui->txtfecha->date();
    oFacPro->fecha_recepcion = ui->txtrecepcion->date();
    oFacPro->pedido = ui->txtpedidoProveedor->text();
    oFacPro->proveedor = ui->txtproveedor->text();
    oFacPro->cif_proveedor = ui->txtcif->text();
    oFacPro->retencion_irpf = ui->chkRetencioirpf->isChecked();
    oFacPro->recargo_equivalencia = ui->chklporc_rec->isChecked();
    oFacPro->base1 = ui->txtbase1->text().replace(".","").replace(",",".").toDouble();
    oFacPro->base2 = ui->txtbase2->text().replace(".","").replace(",",".").toDouble();
    oFacPro->base3 = ui->txtbase3->text().replace(".","").replace(",",".").toDouble();
    oFacPro->base4 = ui->txtbase4->text().replace(".","").replace(",",".").toDouble();
    oFacPro->porc_iva1 = ui->txtporc_iva1->text().toDouble();
    oFacPro->porc_iva2 = ui->txtporc_iva2->text().toDouble();
    oFacPro->porc_iva3 = ui->txtporc_iva3->text().toDouble();
    oFacPro->porc_iva4 = ui->txtporc_iva4->text().toDouble();
    oFacPro->iva1 = ui->txtiva1->text().replace(".","").replace(",",".").toDouble();
    oFacPro->iva2 = ui->txtiva2->text().replace(".","").replace(",",".").toDouble();
    oFacPro->iva3 = ui->txtiva3->text().replace(".","").replace(",",".").toDouble();
    oFacPro->iva4 = ui->txtiva4->text().replace(".","").replace(",",".").toDouble();
    oFacPro->porc_rec1 = ui->txtporc_rec1->text().toDouble();
    oFacPro->porc_rec2 = ui->txtporc_rec2->text().toDouble();
    oFacPro->porc_rec3 = ui->txtporc_rec3->text().toDouble();
    oFacPro->porc_rec4 = ui->txtporc_rec4->text().toDouble();
    oFacPro->rec1 = ui->txtporc_rec1->text().replace(".","").replace(",",".").toDouble();
    oFacPro->rec2 = ui->txtporc_rec2->text().replace(".","").replace(",",".").toDouble();
    oFacPro->rec3 = ui->txtporc_rec3->text().replace(".","").replace(",",".").toDouble();
    oFacPro->rec4 = ui->txtporc_rec4->text().replace(".","").replace(",",".").toDouble();
    oFacPro->total1 = ui->txttotal1->text().replace(".","").replace(",",".").toDouble();
    oFacPro->total2 = ui->txttotal2->text().replace(".","").replace(",",".").toDouble();
    oFacPro->total3 = ui->txttotal3->text().replace(".","").replace(",",".").toDouble();
    oFacPro->total4 = ui->txttotal4->text().replace(".","").replace(",",".").toDouble();
    oFacPro->total_base = ui->txtbase_total_2->text().replace(".","").replace(",",".").toDouble();
    oFacPro->total_iva = ui->txtiva->text().replace(".","").replace(",",".").toDouble();
    oFacPro->total_retencion = ui->txtimporte_irpf->text().replace(".","").replace(",",".").toDouble();
    oFacPro->total_recargo = ui->txtimporteRE->text().replace(".","").replace(",",".").toDouble();
    oFacPro->total = ui->txttotal->text().replace(".","").replace(",",".").toDouble();
    oFacPro->forma_pago = ui->cboforma_pago->currentText();
    oFacPro->importe_pagado_total = ui->txtImportePagadoFactura->text().replace(".","").replace(",",".").toDouble();
    oFacPro->id_tipo_gasto = Configuracion_global->Devolver_id_tipo_gasto(ui->cbo_grupo_gasto->currentText());
    oFacPro->comentario = ui->txtcomentario->toPlainText();
    oFacPro->importe_deuda_pendiente = ui->txtimporte_pendiente->text().replace(".","").replace(",",".").toDouble();
    oFacPro->albaran = ui->txtAlbaran->text();
    oFacPro->desc_gasto1 = ui->txtGastoDist1->text();
    oFacPro->desc_gasto2 = ui->txtGastoDist2->text();
    oFacPro->desc_gasto3 = ui->txtGastoDist3->text();
    oFacPro->imp_gasto1 = ui->SpinGastoDist1->value();
    oFacPro->imp_gasto2 = ui->SpinGastoDist2->value();
    oFacPro->imp_gasto3 = ui->SpinGastoDist3->value();
    oFacPro->gasto_to_coste = ui->GastoDist_toLineas->isChecked();
}

void FrmFacturasProveedor::llenar_campos_iva()
{

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));
}

void FrmFacturasProveedor::on_btnGuardar_clicked()
{
    if(!ui->txtfactura->text().isEmpty())
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
        ui->txtfactura->setFocus();
    }
}

void FrmFacturasProveedor::on_btnSiguiente_clicked()
{
    oFacPro->recuperar_factura("Select * from fac_pro where id >"+QString::number(oFacPro->id)+" limit 0,1",2);
    llenar_campos();
    ui->btnAnterior->setEnabled(true);
    ui->btnImprimir->setEnabled(true);
    ui->btnEditar->setEnabled(true);
}

void FrmFacturasProveedor::on_btnAnterior_clicked()
{
    oFacPro->recuperar_factura("Select * from fac_pro where id <"+QString::number(oFacPro->id)+" order by id desc limit 0,1",1);
    llenar_campos();
}

void FrmFacturasProveedor::on_botBuscarCliente_clicked()
{
    db_consulta_view busca_prov;
    busca_prov.set_db("Maya");
    QStringList cCampo,cCabecera;
    QVariantList vTamanos;
    cCampo << "proveedor" << "codigo" <<"cif";
    cCabecera <<"Código" << "Proveedor" << "CIF/NIF/NIE";
    vTamanos << 0 << 120 << 250 <<120;
    busca_prov.set_SQL("Select id, codigo,proveedor,cif from proveedores");
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

void FrmFacturasProveedor::on_btnEditar_clicked()
{
    bloquearcampos(false);
    emit block();
    ui->txtcodigo_proveedor->setFocus();
}

void FrmFacturasProveedor::on_btnDeshacer_clicked()
{
    oFacPro->recuperar_factura(oFacPro->id);
    llenar_campos();
    emit unblock();
    bloquearcampos(true);
}

void FrmFacturasProveedor::on_btnListados_clicked()
{
    // TODO - LISTADOS FACTURAS PROV
}

void FrmFacturasProveedor::on_btnBuscar_clicked()
{
    ui->radBusqueda->setChecked(true);
}


void FrmFacturasProveedor::on_radBusqueda_toggled(bool checked)
{
    if(checked)
        ui->stackedWidget->setCurrentIndex(1);
    else
        ui->stackedWidget->setCurrentIndex(0);
}
