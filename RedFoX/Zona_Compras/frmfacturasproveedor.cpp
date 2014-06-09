#include "frmfacturasproveedor.h"
#include "ui_frmfacturasproveedor.h"
#include "../Busquedas/db_consulta_view.h"
#include "Auxiliares/monetarydelegate.h"
#include "Auxiliares/datedelegate.h"
#include "Auxiliares/frmeditline.h"

void FrmFacturasProveedor::init_querys()
{    
    modeltipogasto->setQuery("id,select descripcion from grupos_gasto",Configuracion_global->groupDB);
    model_busqueda->setQuery("select id,factura,fecha,cif_proveedor,telefono,proveedor,total from fac_pro order by factura desc",Configuracion_global->empresaDB);
    modelLineas->setQuery("select id,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                          "from lin_fac_pro where id = 0;",Configuracion_global->empresaDB);
    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget_2->setCurrentIndex(0);
    formato_tabla();
}

void FrmFacturasProveedor::init()
{
    ui->setupUi(this);
    ui->btn_cerrar->setVisible(_showCerrar);

    setUpBusqueda();
    oFacPro = new FacturasProveedor(this);
    model_busqueda = new QSqlQueryModel(this);
    modeltipogasto = new QSqlQueryModel(this);
    modelLineas = new QSqlQueryModel(this);

    lineas_anterior = new QSqlQueryModel(this);



    ui->tabla->setModel(model_busqueda);
    ui->Lineas->setModel(modelLineas);

    ui->cbo_grupo_gasto->setModel(modeltipogasto);
    ui->cbo_grupo_gasto->setModelColumn(1);
    ui->cboforma_pago->setModel(Configuracion_global->formapago_model);
    ui->cboforma_pago->setModelColumn(1);
    ui->combo_pais->setModel(Configuracion_global->paises_model);
    ui->combo_pais->setModelColumn(1);

    ui->tabla->setItemDelegateForColumn(2,new DateDelegate(this));
    ui->tabla->setItemDelegateForColumn(6, new MonetaryDelegate(this));

    llenar_campos_iva();
    ui->lbcontabilizada->setVisible(false);
    ui->lblFacturaPagada->setVisible(false);
    ui->lblIRPF_4->setVisible(false);
    ui->lblproveedor->setText("");

    formato_tabla();
    ui->stackedWidget->setCurrentIndex(1);

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));
    // CONTROL DE EVENTOS
    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;

    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);
    bloquearcampos(true);

    __init = true;
}

FrmFacturasProveedor::FrmFacturasProveedor(QWidget *parent, bool showCerrar) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmFacturasProveedor),
    prov(this),
    menuButton(QIcon(":/Icons/PNG/Factura_pro.png"),tr("Facturas Prov."),this),
    push(new QPushButton(QIcon(":/Icons/PNG/Factura_pro.png"),"",this))
{
    __init = false;
    _showCerrar = showCerrar;
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de facturas de proveedores/acreeedores"));
    this->installEventFilter(this);
}

FrmFacturasProveedor::~FrmFacturasProveedor()
{
    delete ui;
}

void FrmFacturasProveedor::bloquearcampos(bool state)
{

    QList<QLineEdit *> lineEditList = this->findChildren<QLineEdit *>();
    QLineEdit *lineEdit;
    foreach (lineEdit, lineEditList)
        lineEdit->setReadOnly(state);

    // ComboBox
    QList<QComboBox *> ComboBoxList = this->findChildren<QComboBox *>();
    QComboBox *ComboBox;
    foreach (ComboBox, ComboBoxList)
        ComboBox->setEnabled(!state);    

    //DoubleSpinBox
    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList)
        DSpinBox->setReadOnly(state);

    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList)
        CheckBox->setEnabled(!state);

    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList)
        TextEdit->setReadOnly(state);

    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList)
        DateEdit->setEnabled(!state);

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

    ui->btnPagarVencimiento->setEnabled(!state);
    ui->btnAddVencimiento->setEnabled(!state);
    ui->btnDeleteVencimiento->setEnabled(!state);

    m_busqueda->block(!state);
}

void FrmFacturasProveedor::llenarProveedor(int id)
{
    prov.Recuperar(id);
    ui->txtcodigo_proveedor->setText(prov.codigo);
    ui->txtproveedor->setText(prov.proveedor);
    ui->lblproveedor->setText(prov.proveedor);
    ui->txtdireccion1->setText(prov.direccion1);
    ui->txtdireccion2->setText(prov.direccion2);
    ui->txtpoblacion->setText(prov.poblacion);
    ui->txtprovincia->setText(prov.provincia);
    ui->txtcp->setText(prov.cp);
    ui->txtcif->setText(prov.cif);
    ui->combo_pais->setCurrentText(Configuracion_global->Devolver_pais(prov.id_pais));
    oFacPro->id_proveedor = prov.id;
}

void FrmFacturasProveedor::on_btnAnadir_clicked()
{
    editando = false;
    Configuracion_global->transaction();
    if(oFacPro->anadir_factura())
    {
        pendiente_anterior = 0.0;
        oFacPro->Recuperar(oFacPro->id);
        llenar_campos();
        bloquearcampos(false);
        ui->stackedWidget->setCurrentIndex(0);
        ui->tabWidget_2->setCurrentIndex(0);
        ocultarBusqueda();
        ui->txtcodigo_proveedor->setFocus();
        emit block();
    }
    else
    {
        Configuracion_global->rollback();
    }
}

void FrmFacturasProveedor::llenar_campos()
{

    ui->txtfactura->setText(oFacPro->factura);
    ui->lblFactura->setText(oFacPro->factura);
    ui->txtfecha->setDate(oFacPro->fecha);
    ui->txtrecepcion->setDate(oFacPro->fecha_recepcion);
    ui->txtpedidoProveedor->setText(oFacPro->pedido);
    ui->txtproveedor->setText(oFacPro->proveedor);

    ui->txtcodigo_proveedor->setText(oFacPro->codigo_proveedor);
    ui->lblproveedor->setText(oFacPro->proveedor);
    ui->txtdireccion1->setText(oFacPro->direccion1);
    ui->txtdireccion2->setText(oFacPro->direccion2);
    ui->txtpoblacion->setText(oFacPro->poblacion);
    ui->txtprovincia->setText(oFacPro->provincia);
    ui->txtcp->setText(oFacPro->cp);
    ui->combo_pais->setCurrentText(Configuracion_global->Devolver_pais(oFacPro->id_pais));
    oFacPro->id_proveedor = oFacPro->id_proveedor;

    ui->txtcif->setText(oFacPro->cif_proveedor);
    //ui->chkRetencioirpf->setChecked(oFacPro->retencion_irpf);
    ui->chklporc_rec->setChecked(oFacPro->recargo_equivalencia);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->base1,'f',Configuracion_global->decimales)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->base2,'f',Configuracion_global->decimales)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->base3,'f',Configuracion_global->decimales)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->base4,'f',Configuracion_global->decimales)));

    ui->txtporc_iva1->setText(QString::number(oFacPro->porc_iva1,'f',Configuracion_global->decimales));
    ui->txtporc_iva2->setText(QString::number(oFacPro->porc_iva2,'f',Configuracion_global->decimales));
    ui->txtporc_iva3->setText(QString::number(oFacPro->porc_iva3,'f',Configuracion_global->decimales));
    ui->txtporc_iva4->setText(QString::number(oFacPro->porc_iva4,'f',Configuracion_global->decimales));

    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->iva1,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->iva2,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->iva3,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->iva4,'f',Configuracion_global->decimales)));

    ui->txtporc_rec1->setText(QString::number(oFacPro->porc_rec1));
    ui->txtporc_rec2->setText(QString::number(oFacPro->porc_rec2));
    ui->txtporc_rec3->setText(QString::number(oFacPro->porc_rec3));
    ui->txtporc_rec4->setText(QString::number(oFacPro->porc_rec4));

    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->rec1)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->rec2)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->rec3)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->rec4)));

    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total1)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total3)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total4)));


    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->base_total,'f',Configuracion_global->decimales)));
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->iva_total,'f',Configuracion_global->decimales)));
    ui->txtimporte_irpf->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->irpf,'f',Configuracion_global->decimales)));
    ui->txtimporteRE->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total_recargo,'f',Configuracion_global->decimales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->total)));

    for(auto i=0; i< Configuracion_global->formapago_model->rowCount();i++)
    {
        if(Configuracion_global->formapago_model->record(i).value("id").toInt() == oFacPro->id_forma_pago)
        {
            ui->cboforma_pago->setCurrentIndex(i);
            break;
        }
    }

    //ui->txtImportePagadoFactura->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->importe_pagado_total,'f',Configuracion_global->decimales)));
    QString cGasto = Configuracion_global->Devolver_tipo_gasto(oFacPro->id_tipo_gasto);
    int index = ui->cbo_grupo_gasto->findText(cGasto);
    ui->cbo_grupo_gasto->setCurrentIndex(index);
    ui->txtcomentario->setPlainText(oFacPro->comentario);
    ui->lblFacturaPagada->setVisible(oFacPro->pagado);
   // ui->txtimporte_pendiente->setText(Configuracion_global->toFormatoMoneda(QString::number(oFacPro->importe_deuda_pendiente)));
    ui->txtAlbaran->setText(oFacPro->albaran);
    ui->txtGastoDist1->setText(oFacPro->desc_gasto1);
    ui->txtGastoDist2->setText(oFacPro->desc_gasto2);
    ui->txtGastoDist3->setText(oFacPro->desc_gasto3);
    ui->SpinGastoDist1->setValue(oFacPro->imp_gasto1);
    ui->SpinGastoDist2->setValue(oFacPro->imp_gasto2);
    ui->SpinGastoDist3->setValue(oFacPro->imp_gasto3);
    ui->cboporc_iva_gasto1->setCurrentText(QString::number(oFacPro->porc_iva_gasto1));
    ui->cboporc_iva_gasto2->setCurrentText(QString::number(oFacPro->porc_iva_gasto2));
    ui->cboporc_iva_gasto3->setCurrentText(QString::number(oFacPro->porc_iva_gasto3));

    ui->spiniva_gasto1->setValue(oFacPro->iva_gasto1);
    ui->spiniva_gasto2->setValue(oFacPro->iva_gasto2);
    ui->spiniva_gasto3->setValue(oFacPro->iva_gasto3);
    ui->spinPorc_dto_pp->setValue(oFacPro->porc_dto);

    llenarLineas();
    llenar_vencimientos();
}

void FrmFacturasProveedor::llenar_fac_pro()
{
    oFacPro->factura = ui->txtfactura->text();
    oFacPro->fecha = ui->txtfecha->date();
    oFacPro->fecha_recepcion = ui->txtrecepcion->date();
    oFacPro->pedido = ui->txtpedidoProveedor->text();
    //oFacPro->id_proveedor =
    oFacPro->proveedor = ui->txtproveedor->text();
    oFacPro->cif_proveedor = ui->txtcif->text();
    oFacPro->direccion1 = ui->txtdireccion1->text();
    oFacPro->direccion2 = ui->txtdireccion2->text();
    oFacPro->cp = ui->txtcp->text();
    oFacPro->poblacion = ui->txtpoblacion->text();
    oFacPro->provincia = ui->txtprovincia->text();
    oFacPro->id_pais = Configuracion_global->paises_model->record(ui->combo_pais->currentIndex()).value("id").toInt();
    //oFacPro->telefono =
    //oFacPro->movil =
    //oFacPro->fax =
    oFacPro->porc_dto = ui->spinPorc_dto_pp->value();
    //oFacPro->porc_irpf =
    oFacPro->recargo_equivalencia = ui->chklporc_rec->isChecked();

    oFacPro->base1  = Configuracion_global->MonedatoDouble(ui->txtbase1->text());
    oFacPro->base2  = Configuracion_global->MonedatoDouble(ui->txtbase2->text());
    oFacPro->base3  = Configuracion_global->MonedatoDouble(ui->txtbase3->text());
    oFacPro->base4  = Configuracion_global->MonedatoDouble(ui->txtbase4->text());

    oFacPro->porc_iva1 = ui->txtporc_iva1->text().toDouble();
    oFacPro->porc_iva2 = ui->txtporc_iva2->text().toDouble();
    oFacPro->porc_iva3 = ui->txtporc_iva3->text().toDouble();
    oFacPro->porc_iva4 = ui->txtporc_iva4->text().toDouble();

    oFacPro->iva1  = Configuracion_global->MonedatoDouble(ui->txtiva1->text());
    oFacPro->iva2  = Configuracion_global->MonedatoDouble(ui->txtiva2->text());
    oFacPro->iva3  = Configuracion_global->MonedatoDouble(ui->txtiva3->text());
    oFacPro->iva4  = Configuracion_global->MonedatoDouble(ui->txtiva4->text());

    oFacPro->porc_rec1 = ui->txtporc_rec1->text().toDouble();
    oFacPro->porc_rec2 = ui->txtporc_rec2->text().toDouble();
    oFacPro->porc_rec3 = ui->txtporc_rec3->text().toDouble();
    oFacPro->porc_rec4 = ui->txtporc_rec4->text().toDouble();

    oFacPro->rec1  = Configuracion_global->MonedatoDouble(ui->txtrec1->text());
    oFacPro->rec2  = Configuracion_global->MonedatoDouble(ui->txtrec2->text());
    oFacPro->rec3  = Configuracion_global->MonedatoDouble(ui->txtrec3->text());
    oFacPro->rec4  = Configuracion_global->MonedatoDouble(ui->txtrec4->text());

    oFacPro->total1  = Configuracion_global->MonedatoDouble(ui->txttotal1->text());
    oFacPro->total2  = Configuracion_global->MonedatoDouble(ui->txttotal2->text());
    oFacPro->total3  = Configuracion_global->MonedatoDouble(ui->txttotal3->text());
    oFacPro->total4  = Configuracion_global->MonedatoDouble(ui->txttotal4->text());

    oFacPro->base_total  = Configuracion_global->MonedatoDouble(ui->txtbase_total_2->text());
    oFacPro->iva_total  = Configuracion_global->MonedatoDouble(ui->txtiva->text());
    oFacPro->irpf  = Configuracion_global->MonedatoDouble(ui->txtimporte_irpf->text());
    oFacPro->total_recargo  = Configuracion_global->MonedatoDouble(ui->txtimporteRE->text());
    oFacPro->total  = Configuracion_global->MonedatoDouble(ui->txttotal->text());

    oFacPro->id_forma_pago = Configuracion_global->formapago_model->record(ui->cboforma_pago->currentIndex()).value("id").toInt();
    oFacPro->id_tipo_gasto = modeltipogasto->record(ui->cbo_grupo_gasto->currentIndex()).value("id").toInt();
    oFacPro->comentario = ui->txtcomentario->toPlainText();
    //oFacPro->pagado = ui->
    oFacPro->albaran = ui->txtAlbaran->text();

    oFacPro->desc_gasto1 = ui->txtGastoDist1->text();
    oFacPro->desc_gasto2 = ui->txtGastoDist2->text();
    oFacPro->desc_gasto3 = ui->txtGastoDist3->text();

    oFacPro->imp_gasto1 = ui->SpinGastoDist1->value();
    oFacPro->imp_gasto2 = ui->SpinGastoDist2->value();
    oFacPro->imp_gasto3 = ui->SpinGastoDist3->value();

    //oFacPro->gasto_to_coste =
    oFacPro->porc_iva_gasto1 = ui->cboporc_iva_gasto1->currentText().toDouble();
    oFacPro->porc_iva_gasto2 = ui->cboporc_iva_gasto2->currentText().toDouble();
    oFacPro->porc_iva_gasto3 = ui->cboporc_iva_gasto3->currentText().toDouble();
    oFacPro->iva_gasto1 = ui->spiniva_gasto1->value();
    oFacPro->iva_gasto2 = ui->spiniva_gasto2->value();
    oFacPro->iva_gasto3 = ui->spiniva_gasto3->value();
    oFacPro->dto = Configuracion_global->MonedatoDouble(ui->txtimporte_descuento->text());
    oFacPro->ejercicio = ui->txtfecha->date().year();
    //oFacPro->contabilizada =
    oFacPro->subtotal = Configuracion_global->MonedatoDouble(ui->txtsubtotal->text());
    oFacPro->codigo_proveedor = ui->txtcodigo_proveedor->text();
    //oFacPro->impreso =
    //oFacPro->editable =
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
        double acum = 0.0;
        for(int i = 0; i < ui->tabla_vencimientos->rowCount(); ++i)
        {
            double d = ui->tabla_vencimientos->item(i,1)->text().toDouble();
            acum += d;
        }
        double max_fac = oFacPro->total - acum;
        if((int)max_fac > 0)
        {
            QMessageBox::information(this,tr("Vencimientos"), tr("Por favor, rellene los vencimientos de la factura antes de guardar"));
            return;
        }
        llenar_fac_pro();
        if(oFacPro->guardar_factura())
        {
            if(oFacPro->albaran.isEmpty())
            {
                double acumular = oFacPro->base_total;
                double pendiente = 0.0;
                for(auto i=0; i<ui->tabla_vencimientos->rowCount(); ++i)
                {
                    double d = ui->tabla_vencimientos->item(i,3)->text().toDouble();
                    pendiente += d;
                }

                if(editando)
                {
                    //BORRAR LOS ACUMULADOS ANTERIORES COMPLETAMENTE
                    if(!Proveedor::acumular(oFacPro->id_proveedor,fecha_anterior.month(),-1.0 * total_anterior))
                    {
                        Configuracion_global->rollback();
                        return;
                    }
                    for(auto i=0;i< lineas_anterior->rowCount();++i)
                    {
                        QSqlRecord r = lineas_anterior->record(i);
                        if(!Articulo::acumulado_compras(r.value("id_articulo").toInt(),-1.0* r.value("cantidad").toDouble(),-1.0* r.value("total").toDouble(),fecha_anterior))
                        {
                            Configuracion_global->rollback();
                            return;
                        }
                    }
                    pendiente -= pendiente_anterior;
                }

                if(!Proveedor::acumular_deuda(oFacPro->id_proveedor, pendiente))
                {
                    Configuracion_global->rollback();
                    return;
                }

                if(Proveedor::acumular(oFacPro->id_proveedor,oFacPro->fecha.month(),acumular))
                {
                    for(auto i=0; i<modelLineas->rowCount();++i)
                    {
                        QSqlRecord r = modelLineas->record(i);
                        if(!Articulo::acumulado_compras(r.value("id_articulo").toInt(),r.value("cantidad").toDouble(),r.value("total").toDouble(),oFacPro->fecha))
                        {
                            Configuracion_global->rollback();
                            return;
                        }
                    }
                    llenar_campos();
                    bloquearcampos(true);
                    Configuracion_global->commit();
                    emit unblock();
                }
            }
            else
            {
                llenar_campos();
                bloquearcampos(true);
                Configuracion_global->commit();
                emit unblock();
            }
        }
    }
    else
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
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if(!ui->tabla->currentIndex().isValid())
            return;
        int id = model_busqueda->record(ui->tabla->currentIndex().row()).value("id").toInt();
        oFacPro->Recuperar(id);
        llenar_campos();
    }
    pendiente_anterior = 0.0;
    for(auto i=0; i<ui->tabla_vencimientos->rowCount(); ++i)
    {
        double d = ui->tabla_vencimientos->item(i,3)->text().toDouble();
        pendiente_anterior += d;
    }
    total_anterior = oFacPro->base_total;
    fecha_anterior = oFacPro->fecha;
    lineas_anterior->setQuery(QString("select id,id_articulo,cantidad,total,albaran from lin_fac_pro where id_cab = %1;").arg(oFacPro->id),Configuracion_global->empresaDB);
    editando = true;
    Configuracion_global->transaction();
    bloquearcampos(false);
    ui->stackedWidget->setCurrentIndex(0);
    ocultarBusqueda();
    ui->txtcodigo_proveedor->setFocus();
    emit block();
}

void FrmFacturasProveedor::on_btnDeshacer_clicked()
{
    Configuracion_global->rollback();
    oFacPro->Recuperar(oFacPro->id);
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
    ui->stackedWidget->setCurrentIndex(1);
}

void FrmFacturasProveedor::llenarLineas()
{
    if(!__init)
        return;
    modelLineas->setQuery(QString("select id,id_articulo,codigo,descripcion,cantidad,precio,subtotal, porc_dto,total,porc_iva,coste_real_unidad  "
                              "from lin_fac_pro where id_cab = %1;").arg(oFacPro->id),Configuracion_global->empresaDB);
    formato_tabla();
    calcular_factura();
}

void FrmFacturasProveedor::calcular_factura()
{
    if(!__init)
        return;
    double subtotal,dto,dtopp,base,irpf,iva,re,total;
    double base1,iva1,re1,total1;
    double base2,iva2,re2,total2;
    double base3,iva3,re3,total3;
    double base4,iva4,re4,total4;
    base1= 0; base2 = 0; base3 =0;base4 = 0;
    iva1=0; iva2 =0; iva3=0; iva4 = 0;
    re1=0;re2=0;re3=0;re4 =0;
    total1 =0; total2 =0;total3=0; total4=0;
    subtotal =0; dto=0; dtopp =0; base =0; irpf = 0; iva =0; re =0; total =0;

    ui->spiniva_gasto1->setValue((ui->SpinGastoDist1->value()*(ui->cboporc_iva_gasto1->currentText().toDouble()/100)));
    ui->spiniva_gasto2->setValue((ui->SpinGastoDist2->value()*(ui->cboporc_iva_gasto2->currentText().toDouble()/100)));
    ui->spiniva_gasto3->setValue((ui->SpinGastoDist3->value()*(ui->cboporc_iva_gasto3->currentText().toDouble()/100)));

    double _dtoPP = 1-(ui->spinPorc_dto_pp->value()/100);

    for(auto i = 0 ; i <modelLineas->rowCount(); ++i)
    {
        QSqlRecord r = modelLineas->record(i);

        subtotal += r.value("subtotal").toDouble();
        // base1
        if(r.value("porc_iva").toFloat() == ui->txtporc_iva1->text().toFloat())
            base1 += r.value("total").toDouble() * _dtoPP;

        // base2
        if(r.value("porc_iva").toFloat() == ui->txtporc_iva2->text().toFloat())
            base2 += r.value("total").toDouble()* _dtoPP;

        // base3
        if(r.value("porc_iva").toFloat() == ui->txtporc_iva3->text().toFloat())
            base3 += r.value("total").toDouble()* _dtoPP;

        // base4
        if(r.value("porc_iva").toFloat() == ui->txtporc_iva4->text().toFloat())
            base4 += r.value("total").toDouble()* _dtoPP;
    }

    //Los gastos no pueden añadir R.E.
    if(ui->chklporc_rec->isChecked())
        re1 = base1 * (ui->txtporc_rec1->text().toDouble()/100);
    if(ui->chklporc_rec->isChecked())
        re2 = base2 * (ui->txtporc_rec2->text().toDouble()/100);
    if(ui->chklporc_rec->isChecked())
        re3 = base3 * (ui->txtporc_rec3->text().toDouble()/100);
    if(ui->chklporc_rec->isChecked())
        re4 = base4 * (ui->txtporc_rec4->text().toDouble()/100);

    // añadir gastos extras

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist3->value();
    iva1 = base1 * (ui->txtporc_iva1->text().toFloat()/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist3->value();
    iva2 = base2 * (ui->txtporc_iva2->text().toFloat()/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist3->value();
    iva3 = base3 * (ui->txtporc_iva3->text().toFloat()/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist3->value();
    iva4 = base4 * (ui->txtporc_iva4->text().toFloat()/100);

    // TOTALES GENERALES
    dtopp = subtotal *(ui->spinPorc_dto_pp->value()/100.0);

    base = base1 + base2+base3+base4;
    iva = iva1 + iva2 +iva3+iva4;
    re  = re1 +re2 + re3 + re4;

    total = (base - irpf) + iva +re;   //TODO ¿IRPF?
    total1 = base1 +iva1 +re1;
    total2 = base2 +iva2 +re2;
    total3 = base3 +iva3 +re3;
    total4 = base4 +iva4 +re4;


    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dtopp,'f',Configuracion_global->decimales_campos_totales)));


    // Desglose llenar controles
    ui->txtbase1->setText (Configuracion_global->toFormatoMoneda(QString::number(base1 ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva1->setText  (Configuracion_global->toFormatoMoneda(QString::number(iva1  ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec1->setText  (Configuracion_global->toFormatoMoneda(QString::number(re1   ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total1,'f',Configuracion_global->decimales_campos_totales)));

    // Desglose llenar controles
    ui->txtbase2->setText (Configuracion_global->toFormatoMoneda(QString::number(base2 ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva2->setText  (Configuracion_global->toFormatoMoneda(QString::number(iva2  ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec2->setText  (Configuracion_global->toFormatoMoneda(QString::number(re2   ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total2,'f',Configuracion_global->decimales_campos_totales)));

    // Desglose llenar controles
    ui->txtbase3->setText (Configuracion_global->toFormatoMoneda(QString::number(base3 ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva3->setText  (Configuracion_global->toFormatoMoneda(QString::number(iva3  ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec3->setText  (Configuracion_global->toFormatoMoneda(QString::number(re3   ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total3,'f',Configuracion_global->decimales_campos_totales)));

    // Desglose llenar controles
    ui->txtbase4->setText (Configuracion_global->toFormatoMoneda(QString::number(base4 ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva4->setText  (Configuracion_global->toFormatoMoneda(QString::number(iva4  ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec4->setText  (Configuracion_global->toFormatoMoneda(QString::number(re4   ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total4,'f',Configuracion_global->decimales_campos_totales)));

    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(dtopp,Configuracion_global->decimales_campos_totales)));

    //this->moneda = moneda;
    ui->txtbase->setText             (Configuracion_global->toFormatoMoneda(QString::number(base    ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto     ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText         (Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText              (Configuracion_global->toFormatoMoneda(QString::number(iva     ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtimporteRE->setText    (Configuracion_global->toFormatoMoneda(QString::number(re      ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText            (Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));
    //ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_iva_2->setText (Configuracion_global->toFormatoMoneda(QString::number(iva ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));

    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));

    if(!editando)
        oFacPro->total = (base-irpf)+(iva+re);
}

void FrmFacturasProveedor::llenar_vencimientos()
{
    QString error;
    QMap<int,QSqlRecord> deudas = SqlCalls::SelectRecord("deudas_proveedores",
                                    QString("id_empresa=%1 and id_documento = %2").arg(Configuracion_global->idEmpresa).arg(oFacPro->id),
                                    Configuracion_global->groupDB,
                                    error);
    ui->tabla_vencimientos->clear();
    ui->tabla_vencimientos->setRowCount(0);
    QMapIterator<int,QSqlRecord> it(deudas);
    while(it.hasNext())
    {
        const QSqlRecord r = it.next().value();
        QTableWidgetItem * venc = new QTableWidgetItem(r.value("vencimiento").toDate().toString("dd/MM/yyyy"));
        QTableWidgetItem * importe = new QTableWidgetItem(QString::number(r.value("importe_deuda").toDouble(), 'f', Configuracion_global->decimales_campos_totales));
        QTableWidgetItem * pendiente = new QTableWidgetItem(QString::number(r.value("pendiente").toDouble(), 'f', Configuracion_global->decimales_campos_totales));
        QTableWidgetItem * pagado = new QTableWidgetItem(QString::number(r.value("pagado").toDouble(), 'f', Configuracion_global->decimales_campos_totales));

        int r_c = ui->tabla_vencimientos->rowCount();
        ui->tabla_vencimientos->insertRow(r_c);
        ui->tabla_vencimientos->setItem(r_c,0,venc);
        ui->tabla_vencimientos->setItem(r_c,1,importe);
        ui->tabla_vencimientos->setItem(r_c,2,pagado);
        ui->tabla_vencimientos->setItem(r_c,3,pendiente);
    }

    QStringList head;
    head << tr("Vencimiento") << tr("Importe") << tr("Pagado") << tr("Pendiente");
    ui->tabla_vencimientos->setHorizontalHeaderLabels(head);
}

void FrmFacturasProveedor::formato_tabla()
{
    QStringList header;
    header << "id" << tr("Factura") <<tr("Fecha") << tr("Cif") <<tr("Teléfono") <<tr("Proveedor")<< tr("Total")
            <<tr("Tipo Gasto");
    QVariantList sizes;
    sizes << 0 << 110 << 90  << 120 << 120 << 180 << 100 <<150;
    for(int i = 0; i< header.length(); i++)
    {
        ui->tabla->setColumnWidth(i,sizes.at(i).toInt());
        model_busqueda->setHeaderData(i,Qt::Horizontal,header.at(i));
    }
    ui->tabla->setColumnHidden(0,true);

    header.clear();
    sizes.clear();

    header << tr("id")<< tr("id_Art") << tr("Código") << tr("Descripción") << tr("cantidad") << tr("coste") << tr("Subtotal") << tr("%Dto")  << tr("Total");
    header  << tr("% I.V.A.") << tr("C.Real/u.") ;
    sizes << 0 << 0 << 100 << 350 << 100 << 100 <<100 <<100 << 100 <<100 << 100 <<110;
    for(int i = 0; i <header.size();i++)
    {
        ui->Lineas->setColumnWidth(i,sizes.at(i).toInt());
        modelLineas->setHeaderData(i,Qt::Horizontal,header.at(i));
    }
    ui->Lineas->setColumnHidden(0,true);
    ui->Lineas->setColumnHidden(1,true);

}

void FrmFacturasProveedor::filter_table(QString texto, QString orden, QString modo)
{
    if(ui->stackedWidget->currentIndex() == 0)
        ui->stackedWidget->setCurrentIndex(1);
    QHash <QString,QString> h;
    h[tr("Factura")] = "factura";
    h[tr("Fecha")] ="fecha";
    h[tr("Proveedor")] = "proveedor";
    h[tr("Teléfono")] = "telefono";
    h[tr("Total")] = "total";
    QString order = h.value(orden);

    QString tg;
    if(modo == tr("A-Z"))
        modo = "";
    else
        modo = "Desc";

    if(QString(tg).isEmpty())
    {
       QString cSQL = "select id,factura,fecha,cif_proveedor,telefono,proveedor,total from fac_pro "
                "where "+order+" like '%"+texto.trimmed()+"%' order by "+order+" "+modo;
        model_busqueda->setQuery(cSQL,Configuracion_global->empresaDB);
        formato_tabla();
    }
}

void FrmFacturasProveedor::borrarFactura()
{
  // TODO - BORRAR FACTURA PROVEEDOR
}

void FrmFacturasProveedor::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);

    QStringList orden;
    orden << tr("Factura") << tr("Fecha") << tr("Pedido") << tr("Proveedor") <<tr("Teléfono") << tr("Total");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(m_busqueda,SIGNAL(hideMe()),this,SLOT(ocultarBusqueda()));
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));

    QPushButton * btnAdd = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir"),this);
    connect(btnAdd,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(btnAdd);

    QPushButton * btnEdit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar"),this);
    connect(btnEdit,SIGNAL(clicked()),this,SLOT(on_btnEditar_clicked()));
    m_busqueda->addWidget(btnEdit);

    QPushButton * btnDelete = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    connect(btnDelete,SIGNAL(clicked()),this,SLOT(borrarFactura()));
    m_busqueda->addWidget(btnDelete);

    m_busqueda->addSpacer();
    connect(m_busqueda,SIGNAL(key_Down_Pressed()),ui->tabla,SLOT(setFocus()));
    //connect(m_busqueda,SIGNAL(key_F2_Pressed()),this,SLOT(ocultarBusqueda()));
}

void FrmFacturasProveedor::on_tabla_clicked(const QModelIndex &index)
{
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tabla->model());
    int id = Configuracion_global->devolver_id_tabla(model,index);
    oFacPro->Recuperar(id);
    //llenar_campos();
}

void FrmFacturasProveedor::on_tabla_doubleClicked(const QModelIndex &index)
{
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tabla->model());
    int id = Configuracion_global->devolver_id_tabla(model,index);
    oFacPro->Recuperar(id);
    llenar_campos();
    bloquearcampos(true);
    ui->stackedWidget->setCurrentIndex(0);
    ocultarBusqueda();
}

bool FrmFacturasProveedor::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Show && obj == this)
    {
        init_querys();
    }

    if(event->type() == QEvent::Resize && obj == this)
    {
        if(!__init)
            init();
        _resizeBarraBusqueda(m_busqueda);
    }

    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(ui->stackedWidget->currentIndex()==1)
        {

            if(keyEvent->key() == Qt::Key_Return)
            {
                on_tabla_doubleClicked(ui->tabla->currentIndex());
                return true;
            }
        }
        if(keyEvent->key() == Qt::Key_Escape)
            return true;
        if(keyEvent->key() == Qt::Key_F1){

            if(ui->btnEditar->isEnabled())
            {
                if(m_busqueda->isShow())
                    ocultarBusqueda();
                else
                    mostrarBusqueda();
            }
        }
    }
    return MayaModule::eventFilter(obj,event);
}
void FrmFacturasProveedor::mostrarBusqueda()
{
    _showBarraBusqueda(m_busqueda);
    ui->stackedWidget->setCurrentIndex(1);
    m_busqueda->doFocustoText();
}

void FrmFacturasProveedor::ocultarBusqueda()
{    
    _hideBarraBusqueda(m_busqueda);
}

void FrmFacturasProveedor::on_btnAnadirLinea_clicked()
{
    frmEditLine frmeditar(this);
    frmeditar.init();
    connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(llenarLineas()));

    frmeditar.set_venta(false);
    frmeditar.setTipoDoc(frmEditLine::Factura);
    frmeditar.setAdd_pendientes(false);

    frmeditar.setUse_re(ui->chklporc_rec->isChecked());

    frmeditar.set_linea(0,"lin_fac_pro");
    frmeditar.set_tabla("lin_fac_pro");
    frmeditar.set_id_cab(oFacPro->id);

    frmeditar.exec();
    llenarLineas();
}

void FrmFacturasProveedor::on_Lineas_doubleClicked(const QModelIndex &index)
{
    int id_lin = ui->Lineas->model()->data(index.model()->index(index.row(),0)).toInt();
    if(id_lin >0)
    {
        frmEditLine frmeditar(this);
        frmeditar.init();
        connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(llenarLineas()));

        frmeditar.set_venta(false);
        frmeditar.setTipoDoc(frmEditLine::Factura);
        frmeditar.setAdd_pendientes(false);

        frmeditar.setUse_re(ui->chklporc_rec->isChecked());

        frmeditar.set_id_cab(oFacPro->id);
        frmeditar.set_linea(id_lin,"lin_fac_pro");
        frmeditar.set_tabla("lin_fac_pro");
        frmeditar.set_editando();
        frmeditar.exec();

        calcular_factura();
        ui->Lineas->setFocus();
    }
}

void FrmFacturasProveedor::on_btn_borrarLinea_clicked()
{
    if(!ui->Lineas->currentIndex().isValid())
        return;

    if(QMessageBox::question(this,tr("Lineas de albarán"), tr("¿Borrar la linea?"),
                             tr("No"),tr("Borrar")) == QMessageBox::Accepted)
    {
        QModelIndex index = ui->Lineas->currentIndex();
        int id_lin = modelLineas->record(index.row()).value("id").toInt();
        int id_art = modelLineas->record(index.row()).value("id_articulo").toInt();
        double cantidad = modelLineas->record(index.row()).value("cantidad").toDouble();
        if(editando)
        {
            bool ok = true;
            if(QMessageBox::question(this,tr("Stock de almacén"), tr("¿Desea retirar el stock de almacén?"),
                                     tr("No"),tr("Sí")) == QMessageBox::Accepted)
            {
                ok = Articulo::agregar_stock_fisico(id_art, -1.0 * cantidad);
            }

            QString error;
            if(ok && SqlCalls::SqlDelete("lin_fac_pro",Configuracion_global->empresaDB,QString("id = %1").arg(id_lin),error))
            {
                llenarLineas();
            }
            else
            {
                QMessageBox::critical(this,tr("Error al borrar linea"),error);
            }
        }
        else
        {
            bool ok = Articulo::agregar_stock_fisico(id_art, -1.0 * cantidad);
            QString error;
            if(ok && SqlCalls::SqlDelete("lin_fac_pro",Configuracion_global->empresaDB,QString("id = %1").arg(id_lin),error))
            {
                llenarLineas();
            }
            else
            {
                QMessageBox::critical(this,tr("Error al borrar linea"),error);
            }
        }
        ui->Lineas->setFocus();
    }
}

void FrmFacturasProveedor::on_chklporc_rec_toggled(bool checked)
{
    if (checked) {
        oFacPro->recargo_equivalencia = 1;
        ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
        ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
        ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
        ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));
        oFacPro->porc_rec1 = ui->txtporc_rec1->text().toFloat();
        oFacPro->porc_rec2 = ui->txtporc_rec2->text().toFloat();
        oFacPro->porc_rec3 = ui->txtporc_rec3->text().toFloat();
        oFacPro->porc_rec4 = ui->txtporc_rec4->text().toFloat();
    } else {
        oFacPro->recargo_equivalencia = 0;
        ui->txtporc_rec1->setText("0.00");
        ui->txtporc_rec2->setText("0.00");
        ui->txtporc_rec3->setText("0.00");
        ui->txtporc_rec4->setText("0.00");
        oFacPro->porc_rec1 = 0;
        oFacPro->porc_rec2 = 0;
        oFacPro->porc_rec3 = 0;
        oFacPro->porc_rec4 = 0;
    }
    QString error;
    for(auto i = 0; i<modelLineas->rowCount();++i)
    {
        QSqlRecord r = modelLineas->record(i);
        QHash<QString,QVariant> lin;
        double iva_art = r.value("porc_iva").toDouble();
        double porc_re_art;

        if(iva_art == oFacPro->porc_iva1)
            porc_re_art = oFacPro->porc_rec1;
        if(iva_art == oFacPro->porc_iva2)
            porc_re_art = oFacPro->porc_rec2;
        if(iva_art == oFacPro->porc_iva3)
            porc_re_art = oFacPro->porc_rec3;
        if(iva_art == oFacPro->porc_iva4)
            porc_re_art = oFacPro->porc_rec4;

        lin["porc_rec"] = porc_re_art;
        lin["rec"] = r.value("total").toDouble() * (porc_re_art/100.0);

        if(!SqlCalls::SqlUpdate(lin,"lin_alb_pro",Configuracion_global->empresaDB,QString("id=%1").arg(r.value("id").toInt()),error))
        {
            QMessageBox::critical(this,tr("Error al actualizar lineas"),error);
            break;
        }
    }
    calcular_factura();
}

void FrmFacturasProveedor::on_btnAddVencimiento_clicked()
{
    if(ui->txtfactura->text().isEmpty())
    {
        QMessageBox::information(this,tr("Gestión de Facturas de proveedor"),
                                 tr("Debe especificar el numero de Factura antes de guardar"),tr("Aceptar"));
        ui->txtfactura->setFocus();
    }
    double max_fac = oFacPro->total;
    for(int i = 0; i < ui->tabla_vencimientos->rowCount(); ++i)
        max_fac -= ui->tabla_vencimientos->item(i,1)->text().toDouble();

    if(max_fac == 0)
    {
        QMessageBox::information(this,tr("Vencimientos completos"), tr("Los vencimientos actuales cubren el total de la factura.\n"
                                                                       "Borre/edite algún vencimiento si es erroneo"));
        return;
    }

    QDialog d(this);

    QLabel label(tr("Fecha de vencimiento"),&d);
    QDateEdit edit(ui->txtfecha->date(),&d);
    QPushButton aceptar(tr("Aceptar"),&d);
    QPushButton cancelar(tr("Cancelar"),&d);
    QGridLayout lay(&d);

    lay.addWidget(&label,0,0,1,1);
    lay.addWidget(&edit,0,1,1,1);
    lay.addWidget(&aceptar,1,0,1,1);
    lay.addWidget(&cancelar,1,1,1,1);

    connect(&aceptar,SIGNAL(clicked()),&d,SLOT(accept()));
    connect(&cancelar,SIGNAL(clicked()),&d,SLOT(reject()));
    if(d.exec() == QDialog::Accepted)
    {
        bool ok;
        double cantidad = QInputDialog::getDouble(this, tr("Valor del vencimiento"),
                                           tr("Cantidad:"), 0, 0, max_fac, 2, &ok);
        if (ok)
        {
            SqlData data;
            data["id_empresa"] = Configuracion_global->idEmpresa;
            data["id_documento"] = oFacPro->id;
            data["documento"] = ui->txtfactura->text();
            data["fecha_deuda"] = ui->txtfecha->date();
            data["vencimiento"] = edit.date();
            data["importe_deuda"] = cantidad;
            data["pendiente"] = cantidad;
            data["pagado"] = 0.0;
            data["id_proveedor"] = oFacPro->id_proveedor;
            QString error;
            if(SqlCalls::SqlInsert(data,"deudas_proveedores",Configuracion_global->groupDB,error))
            {
                TimedMessageBox::Box(this,tr("Vencimiento insertado con éxito"));                                               
                QTableWidgetItem * venc = new QTableWidgetItem(edit.date().toString("dd/MM/yyyy"));
                QTableWidgetItem * importe = new QTableWidgetItem(QString::number(cantidad, 'f', Configuracion_global->decimales_campos_totales));
                QTableWidgetItem * pendiente = new QTableWidgetItem(QString::number(cantidad, 'f', Configuracion_global->decimales_campos_totales));
                QTableWidgetItem * pagado = new QTableWidgetItem("0.0");

                int r_c = ui->tabla_vencimientos->rowCount();
                ui->tabla_vencimientos->insertRow(r_c);
                ui->tabla_vencimientos->setItem(r_c,0,venc);
                ui->tabla_vencimientos->setItem(r_c,1,importe);
                ui->tabla_vencimientos->setItem(r_c,2,pagado);
                ui->tabla_vencimientos->setItem(r_c,3,pendiente);
            }
            else
            {
                QMessageBox::critical(this,tr("Error al insertar vencimiento"),error);
            }
        }
    }
}

void FrmFacturasProveedor::on_btnDeleteVencimiento_clicked()
{

}

void FrmFacturasProveedor::on_btnPagarVencimiento_clicked()
{

}

