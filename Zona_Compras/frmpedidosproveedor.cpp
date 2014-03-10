#include "frmpedidosproveedor.h"
#include "ui_frmpedidosproveedor.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Auxiliares/frmdialogoimprimir.h"
#include "../Auxiliares/frmaddentregascuenta.h"
#include "../Auxiliares/entregascuenta.h"
#include <QPrintDialog>
#include "../Auxiliares/frmeditline.h"
#include"../Auxiliares/datedelegate.h"
#include"../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/monetarydelegate_totals.h"
#include "../Auxiliares/numericdelegate.h"
#include "../Auxiliares/frmgastos_ped_pro.h"

FrmPedidosProveedor::FrmPedidosProveedor(QWidget *parent, bool showCerrar) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmPedidosProveedor),
    prov(this),
    menuButton(QIcon(":/Icons/PNG/pedido_pro.png"),tr("Pedidos Prov."),this),
    shortCut(new QPushButton(QIcon(":/Icons/PNG/pedido_pro.png"),"",this))
{
    ui->setupUi(this);
    ui->btn_cerrar->setVisible(showCerrar);
}

FrmPedidosProveedor::~FrmPedidosProveedor()
{
    delete ui;
}

void FrmPedidosProveedor::init_querys()
{
    modelLineas->setQuery(QString("select id,codigo,descripcion,cantidad,precio,subtotal, porc_dto,total,porc_iva,coste_real_unidad "
                              "from lin_ped_pro where id_cab = %1;").arg(oPedido_proveedor->id),Configuracion_global->empresaDB);
    model_busqueda->setQuery("select id,pedido,fecha,recepcion,cif_nif,codigo_proveedor,proveedor from ped_pro where ejercicio = "+
                    Configuracion_global->cEjercicio+" order by pedido desc",Configuracion_global->empresaDB);
    modelgastos->setQuery(QString("select id,descripcion,importe from gastos_ped_pro where id_cab = %1").arg(oPedido_proveedor->id),
                          Configuracion_global->empresaDB);

    formatotabla();
}

void FrmPedidosProveedor::init()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->tabWidget_2->setCurrentIndex(0);

    oPedido_proveedor = new PedidoProveedor(this);
    setUpBusqueda();
    // Models

    model_busqueda = new QSqlQueryModel(this);
    modelLineas = new QSqlQueryModel(this);
    modelgastos = new QSqlQueryModel(this);
    modelEntregas = new  QSqlQueryModel(this);

    ui->lbimpreso->setVisible(false);
    ui->lblnombreProveedor->clear();
    ui->lblnumero_pedido->clear();
    shortCut->setToolTip(tr("Gestión de pedidos a proveedor"));
    shortCut->setStyleSheet("background-color: rgb(133, 170, 142)");

    ui->combo_pais->setModel(Configuracion_global->paises_model);
    ui->combo_pais->setModelColumn(1);
    ui->combo_pais_entrega->setModel(Configuracion_global->paises_model);
    ui->combo_pais_entrega->setModelColumn(1);   

    ui->cboporc_iva_gasto1->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto1->setModelColumn(4);
    ui->cboporc_iva_gasto1->setCurrentIndex(ui->cboporc_iva_gasto1->findText(Configuracion_global->ivaList.at(0)));

    ui->cboporc_iva_gasto2->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto2->setModelColumn(4);
    ui->cboporc_iva_gasto2->setCurrentIndex(ui->cboporc_iva_gasto2->findText(Configuracion_global->ivaList.at(0)));

    ui->cboporc_iva_gasto3->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto3->setModelColumn(4);
    ui->cboporc_iva_gasto3->setCurrentIndex(ui->cboporc_iva_gasto3->findText(Configuracion_global->ivaList.at(0)));


    connect(ui->btnAnadir,SIGNAL(clicked()),this,SLOT(anadir_pedido()));
    connect(ui->btnEditar,SIGNAL(clicked()),this,SLOT(editar_pedido()));
    connect(ui->btnGuardar,SIGNAL(clicked()),this,SLOT(guardar_pedido()));
    connect(ui->btnDeshacer,SIGNAL(clicked()),this,SLOT(deshacer()));
    connect(ui->btn_borrar,SIGNAL(clicked()),this,SLOT(borrar_pedido()));

    ui->txtiva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtiva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtiva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtiva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));   

    ui->tabla->setModel(model_busqueda);
    ui->Lineas->setModel(modelLineas);
    ui->tablagastos->setModel(modelgastos);

    //Delegates
    ui->Lineas->setItemDelegateForColumn(3,new NumericDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(4,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(5,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(6,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(7,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(8,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(9,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(10,new MonetaryDelegate_totals(this,true));

    ui->tablagastos->setItemDelegateForColumn(2,new MonetaryDelegate(this));

    // Events
    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;
    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);
    bloquearcampos(true);
}

void FrmPedidosProveedor::llenarProveedor(int id)
{
    prov.Recuperar(id);
    oPedido_proveedor->id_forma_pago = prov.idFormadePago;
    ui->txtcodigo_proveedor->setText(prov.codigo);
    ui->txtproveedor->setText(prov.proveedor);
    ui->txtdireccion1->setText(prov.direccion1);
    ui->txtdireccion2->setText(prov.direccion2);
    ui->txtpoblacion->setText(prov.poblacion);
    ui->txtprovincia->setText(prov.provincia);
    ui->txtcp->setText(prov.cp);
    ui->txtcif->setText(prov.cif);
    ui->combo_pais->setCurrentText(Configuracion_global->Devolver_pais(prov.id_pais));
    ui->lblnombreProveedor->setText(prov.proveedor);
    ui->chkrecargo_equivalencia->setChecked(prov.recargo_equivalencia);
}

void FrmPedidosProveedor::bloquearcampos(bool state)
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
    ui->btnBuscar->setEnabled(state);
    ui->btnPrev->setEnabled(state);
    ui->btn_borrar->setEnabled(state);
    ui->btnDeshacer->setEnabled(!state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btnNext->setEnabled(state);
    ui->btnAnadirLinea->setEnabled(!state);
    ui->btn_borrarLinea->setEnabled(!state);
    ui->botBuscarCliente->setEnabled(!state);
    m_busqueda->block(!state);
}

void FrmPedidosProveedor::buscar_proveeedor()
{
    db_consulta_view consulta;
    QStringList campos;
    campos  << "codigo" << "proveedor";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("Proveedores");
    consulta.set_db("Maya");
    consulta.set_SQL("select id, codigo,proveedor,cif,poblacion from proveedores");
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
        oPedido_proveedor->id_proveedor = id_proveedor;
        llenarProveedor(id_proveedor);
    }
}

void FrmPedidosProveedor::anadir_pedido()
{
    Configuracion_global->transaction();
    int id = oPedido_proveedor->nuevo_pedido_proveedor();
    if(id>=0)
    {
        clear();
        oPedido_proveedor->recuperar(id);
        this->id = id;        
        llenar_campos();
        bloquearcampos(false);
        ocultarBusqueda();
        ui->lblnumero_pedido->setText("0");
        ui->stackedWidget->setCurrentIndex(0);
        emit block();
    }
}

void FrmPedidosProveedor::guardar_pedido()
{
    guardar_campos_en_objeto();
    oPedido_proveedor->id =this->id;
    if(oPedido_proveedor->guardar())
    {
        Configuracion_global->commit();
        oPedido_proveedor->recuperar(oPedido_proveedor->id);
        llenar_campos();
        bloquearcampos(true);
        emit unblock();
    }
}

void FrmPedidosProveedor::editar_pedido()
{
    Configuracion_global->transaction();
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if(!ui->tabla->currentIndex().isValid())
            return;
        int _id = model_busqueda->record(ui->tabla->currentIndex().row()).value("id").toInt();
        oPedido_proveedor->recuperar(_id);
        llenar_campos();
    }
    ui->stackedWidget->setCurrentIndex(0);
    bloquearcampos(false);
    emit block();
    ui->txtcodigo_proveedor->setFocus();
}
void FrmPedidosProveedor::deshacer()
{
    Configuracion_global->rollback();
    llenar_campos();
    bloquearcampos(true);
    emit unblock();
}

void FrmPedidosProveedor::borrar_pedido()
{
    int _id = oPedido_proveedor->id;
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if(!ui->tabla->currentIndex().isValid())
            return;
        _id = model_busqueda->record(ui->tabla->currentIndex().row()).value("id").toInt();
    }
    if(QMessageBox::question(this,tr("Gestión de Pedidos a proveedores"),
                             tr("¿Desea eliminar este pedido?\nEsta opción no se podrá deshacer"),
                             tr("Cancelar"),tr("Aceptar")) == QMessageBox::Accepted)
    {
        if (oPedido_proveedor->borrar(_id))
            clear();
    }
}

void FrmPedidosProveedor::llenar_campos()
{
    this->id =oPedido_proveedor->id;
    ui->txtpedido->setText(oPedido_proveedor->pedido);
    ui->lblnumero_pedido->setText(oPedido_proveedor->pedido);
    ui->lblnombreProveedor->setText(oPedido_proveedor->proveedor);
    ui->txtfecha->setDate(oPedido_proveedor->fecha);
    ui->txtFechaLimite->setDate(oPedido_proveedor->recepcion);
    ui->txtcodigo_proveedor->setText(oPedido_proveedor->codigo_proveedor);
    ui->txtproveedor->setText(oPedido_proveedor->proveedor);
    ui->txtdireccion1->setText(oPedido_proveedor->direccion1);
    ui->txtdireccion2->setText(oPedido_proveedor->direccion2);
    ui->txtcp->setText(oPedido_proveedor->cp);
    ui->txtpoblacion->setText(oPedido_proveedor->poblacion);
    ui->txtprovincia->setText(oPedido_proveedor->provincia);
    for(int i =0;i<Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oPedido_proveedor->id_pais)
        {
            int ind_pais = ui->combo_pais->findText(Configuracion_global->paises_model->record(i).value("pais").toString());
            ui->combo_pais->setCurrentIndex(ind_pais);
            break;
        }
        else
        {
            ui->combo_pais->setCurrentIndex(-1);
        }
    }
    ui->txtcif->setText(oPedido_proveedor->cif_nif);
    ui->txtbase->setText(QString::number(oPedido_proveedor->base_total));
    ui->txtsubtotal->setText(QString::number(oPedido_proveedor->subtotal));
    ui->txtimporte_descuento->setText(QString::number(oPedido_proveedor->dto));
    ui->txtiva->setText(QString::number(oPedido_proveedor->iva));
    ui->txtporc_rec1->setText(QString::number(oPedido_proveedor->rec_total));
    ui->chkrecargo_equivalencia->setChecked(oPedido_proveedor->recargo_equivalencia);
    ui->txttotal->setText(QString::number(oPedido_proveedor->total));
    ui->chkenviado->setChecked(oPedido_proveedor->enviado);
    ui->chkRecibido->setChecked(oPedido_proveedor->recibido);
    ui->chkcompleto->setChecked(oPedido_proveedor->recibido_completo);

    ui->txtcomentario->setText(oPedido_proveedor->comentario);
    ui->txtfechaRecepcion->setDate(oPedido_proveedor->fecha_entrega);
    ui->txtdireccion_entrega1->setText(oPedido_proveedor->direccion_entrega1);
    ui->txtdireccion_entrega1_2->setText(oPedido_proveedor->direccion_entrega2);
    ui->txtcp_entrega->setText(oPedido_proveedor->cp_entrega);
    ui->txtpoblacion_entrega->setText(oPedido_proveedor->poblacion_entrega);
    ui->txtprovincia_entrega->setText(oPedido_proveedor->provincia_entrega);
    for(int i =0;i<Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oPedido_proveedor->id_pais_entrega)
        {
            int ind_pais = ui->combo_pais_entrega->findText(Configuracion_global->paises_model->record(i).value("pais").toString());
            ui->combo_pais_entrega->setCurrentIndex(ind_pais);
            break;
        }
        else
        {
            ui->combo_pais_entrega->setCurrentIndex(-1);
        }
    }
    ui->txtHorarioEntrega->setText(oPedido_proveedor->horario_activo);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->base1,'f',Configuracion_global->decimales)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->base2,'f',Configuracion_global->decimales)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->base3,'f',Configuracion_global->decimales)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->base4,'f',Configuracion_global->decimales)));
    ui->txtporc_iva1->setText(QString::number(oPedido_proveedor->porc_iva1));
    ui->txtporc_iva2->setText(QString::number(oPedido_proveedor->porc_iva2));
    ui->txtporc_iva3->setText(QString::number(oPedido_proveedor->porc_iva3));
    ui->txtporc_iva4->setText(QString::number(oPedido_proveedor->porc_iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->iva1,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->iva2,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->iva3,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->iva4,'f',Configuracion_global->decimales)));
    ui->txtporc_rec1->setText(QString::number(oPedido_proveedor->porc_rec1));
    ui->txtporc_rec2->setText(QString::number(oPedido_proveedor->porc_rec2));
    ui->txtporc_rec3->setText(QString::number(oPedido_proveedor->porc_rec3));
    ui->txtporc_rec4->setText(QString::number(oPedido_proveedor->porc_rec4));
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->rec1,'f',Configuracion_global->decimales)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->rec2,'f',Configuracion_global->decimales)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->rec3,'f',Configuracion_global->decimales)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->rec4,'f',Configuracion_global->decimales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->total1,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->total2,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->total3,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oPedido_proveedor->total4,'f',Configuracion_global->decimales)));

    ui->txtGastoDist1->setText(oPedido_proveedor->desc_gasto1);
    ui->txtGastoDist2->setText(oPedido_proveedor->desc_gasto2);
    ui->txtGastoDist3->setText(oPedido_proveedor->desc_gasto3);
    ui->SpinGastoDist1->setValue(oPedido_proveedor->imp_gasto1);
    ui->SpinGastoDist2->setValue(oPedido_proveedor->imp_gasto2);
    ui->SpinGastoDist3->setValue(oPedido_proveedor->imp_gasto3);

    ui->cboporc_iva_gasto1->setCurrentText(QString::number(oPedido_proveedor->porc_iva_gasto1));
    ui->cboporc_iva_gasto2->setCurrentText(QString::number(oPedido_proveedor->porc_iva_gasto2));
    ui->cboporc_iva_gasto3->setCurrentText(QString::number(oPedido_proveedor->porc_iva_gasto3));


    ui->spiniva_gasto1->setValue(oPedido_proveedor->iva_gasto1);
    ui->spiniva_gasto2->setValue(oPedido_proveedor->iva_gasto2);
    ui->spiniva_gasto3->setValue(oPedido_proveedor->iva_gasto3);

    llenarProveedor(oPedido_proveedor->id_proveedor);
    refrescar_modelo();
    calcular_pedido();
}

void FrmPedidosProveedor::refrescar_modelo()
{
    modelLineas->setQuery(QString("select id,codigo,descripcion,cantidad,precio,subtotal, porc_dto,total,porc_iva,coste_real_unidad  "
                              "from lin_ped_pro where id_cab = %1;").arg(oPedido_proveedor->id),Configuracion_global->empresaDB);
    modelgastos->setQuery(QString("select id,descripcion,importe from gastos_ped_pro where id_cab = %1").arg(oPedido_proveedor->id),
                          Configuracion_global->empresaDB);
    formatotabla();
}

void FrmPedidosProveedor::guardar_campos_en_objeto()
{
    oPedido_proveedor->id = this->id;
    oPedido_proveedor->pedido = ui->lblnumero_pedido->text();
    oPedido_proveedor->proveedor = ui->txtproveedor->text();
    oPedido_proveedor->fecha = ui->txtfecha->date();
    oPedido_proveedor->recepcion =ui->txtfechaRecepcion->date();
    oPedido_proveedor->codigo_proveedor = ui->txtcodigo_proveedor->text();
    oPedido_proveedor->direccion1 = ui->txtdireccion1->text();
    oPedido_proveedor->direccion2 = ui->txtdireccion2->text();
    oPedido_proveedor->cp = ui->txtcp->text();
    oPedido_proveedor->poblacion = ui->txtpoblacion->text();
    oPedido_proveedor->provincia = ui->txtprovincia->text();
    oPedido_proveedor->id_pais = Configuracion_global->paises_model->record(ui->combo_pais->currentIndex()).value("id").toInt();
    oPedido_proveedor->cif_nif = ui->txtcif->text();               
    oPedido_proveedor->enviado = ui->chkenviado->isChecked();
    oPedido_proveedor->recibido = ui->chkRecibido->isChecked();
    oPedido_proveedor->recibido_completo = ui->chkcompleto->isChecked();
    oPedido_proveedor->recargo_equivalencia = ui->chkrecargo_equivalencia->isChecked();
    oPedido_proveedor->comentario = ui->txtcomentario->toPlainText();
    oPedido_proveedor->fecha_entrega = ui->txtfechaRecepcion->date();
    oPedido_proveedor->direccion_entrega1 = ui->txtdireccion_entrega1->text();
    oPedido_proveedor->direccion_entrega2 = ui->txtdireccion_entrega1_2->text();
    oPedido_proveedor->cp_entrega = ui->txtcp_entrega->text();
    oPedido_proveedor->poblacion_entrega = ui->txtpoblacion_entrega->text();
    oPedido_proveedor->provincia_entrega = ui->txtprovincia_entrega->text();
    oPedido_proveedor->id_pais_entrega = Configuracion_global->paises_model->record(ui->combo_pais_entrega->currentIndex()).value("id").toInt();
    oPedido_proveedor->horario_activo = ui->txtHorarioEntrega->text();

    oPedido_proveedor->base_total= Configuracion_global->MonedatoDouble(ui->txtbase_total_2->text());
    oPedido_proveedor->subtotal  = Configuracion_global->MonedatoDouble(ui->txtsubtotal->text());
    oPedido_proveedor->dto       = Configuracion_global->MonedatoDouble(ui->txtimporte_descuento->text());
    oPedido_proveedor->iva       = Configuracion_global->MonedatoDouble(ui->txtiva->text());
    oPedido_proveedor->rec_total = Configuracion_global->MonedatoDouble(ui->txttotal_recargo->text());
    oPedido_proveedor->total     = Configuracion_global->MonedatoDouble(ui->txttotal->text());

    oPedido_proveedor->base1     = Configuracion_global->MonedatoDouble(ui->txtbase1->text());
    oPedido_proveedor->base2     = Configuracion_global->MonedatoDouble(ui->txtbase2->text());
    oPedido_proveedor->base3     = Configuracion_global->MonedatoDouble(ui->txtbase3->text());
    oPedido_proveedor->base4     = Configuracion_global->MonedatoDouble(ui->txtbase4->text());

    oPedido_proveedor->iva1      = Configuracion_global->MonedatoDouble(ui->txtiva1->text());
    oPedido_proveedor->iva2      = Configuracion_global->MonedatoDouble(ui->txtiva2->text());
    oPedido_proveedor->iva3      = Configuracion_global->MonedatoDouble(ui->txtiva3->text());
    oPedido_proveedor->iva4      = Configuracion_global->MonedatoDouble(ui->txtiva4->text());
    oPedido_proveedor->porc_iva1 = Configuracion_global->MonedatoDouble(ui->txtporc_iva1->text());
    oPedido_proveedor->porc_iva2 = Configuracion_global->MonedatoDouble(ui->txtporc_iva2->text());
    oPedido_proveedor->porc_iva3 = Configuracion_global->MonedatoDouble(ui->txtporc_iva3->text());
    oPedido_proveedor->porc_iva4 = Configuracion_global->MonedatoDouble(ui->txtporc_iva4->text());

    oPedido_proveedor->porc_rec1 = Configuracion_global->MonedatoDouble(ui->txtporc_rec1->text());
    oPedido_proveedor->porc_rec2 = Configuracion_global->MonedatoDouble(ui->txtporc_rec2->text());
    oPedido_proveedor->porc_rec3 = Configuracion_global->MonedatoDouble(ui->txtporc_rec3->text());
    oPedido_proveedor->porc_rec4 = Configuracion_global->MonedatoDouble(ui->txtporc_rec4->text());
    oPedido_proveedor->rec1      = Configuracion_global->MonedatoDouble(ui->txtporc_rec1->text());
    oPedido_proveedor->rec2      = Configuracion_global->MonedatoDouble(ui->txtporc_rec2->text());
    oPedido_proveedor->rec3      = Configuracion_global->MonedatoDouble(ui->txtporc_rec3->text());
    oPedido_proveedor->rec4      = Configuracion_global->MonedatoDouble(ui->txtporc_rec4->text());

    oPedido_proveedor->total1    = Configuracion_global->MonedatoDouble(ui->txttotal1->text());
    oPedido_proveedor->total2    = Configuracion_global->MonedatoDouble(ui->txttotal2->text());
    oPedido_proveedor->total3    = Configuracion_global->MonedatoDouble(ui->txttotal3->text());
    oPedido_proveedor->total4    = Configuracion_global->MonedatoDouble( ui->txttotal4->text());

    oPedido_proveedor->desc_gasto1 = ui->txtGastoDist1->text();
    oPedido_proveedor->desc_gasto2 = ui->txtGastoDist2->text();
    oPedido_proveedor->desc_gasto3 = ui->txtGastoDist3->text();
    oPedido_proveedor->imp_gasto1 = ui->SpinGastoDist1->value();
    oPedido_proveedor->imp_gasto2 = ui->SpinGastoDist2->value();
    oPedido_proveedor->imp_gasto3 = ui->SpinGastoDist3->value();
    oPedido_proveedor->porc_iva_gasto1 = ui->cboporc_iva_gasto1->currentText().toDouble();
    oPedido_proveedor->porc_iva_gasto2 = ui->cboporc_iva_gasto2->currentText().toDouble();
    oPedido_proveedor->porc_iva_gasto3 = ui->cboporc_iva_gasto3->currentText().toDouble();
    oPedido_proveedor->iva_gasto1 = ui->spiniva_gasto1->value();
    oPedido_proveedor->iva_gasto2 = ui->spiniva_gasto2->value();
    oPedido_proveedor->iva_gasto3 = ui->spiniva_gasto3->value();

    oPedido_proveedor->telefono = prov.telefono1;
    oPedido_proveedor->fax = prov.fax;
    oPedido_proveedor->movil = prov.movil;
}

void FrmPedidosProveedor::clear()
{
    ui->txtpedido->clear();
    ui->txtfecha->clear();
    ui->txtFechaLimite->clear();
    ui->txtcodigo_proveedor->clear();
    ui->txtproveedor->clear();
    ui->txtdireccion1->clear();
    ui->txtdireccion2->clear();
    ui->txtcp->clear();
    ui->txtpoblacion->clear();
    ui->txtprovincia->clear();
    ui->txtcif->clear();
    ui->txtbase->clear();
    ui->txtsubtotal->clear();
    ui->txtimporte_descuento->clear();
    ui->txtiva->clear();
    ui->txtporc_rec1->clear();
    ui->txttotal->clear();
    ui->chkenviado->setChecked(false);
    ui->chkRecibido->setChecked(false);
    ui->chkcompleto->setChecked(false);

    ui->txtcomentario->clear();
    ui->txtfechaRecepcion->clear();
    ui->txtdireccion_entrega1->clear();
    ui->txtdireccion_entrega1_2->clear();
    ui->txtcp_entrega->clear();
    ui->txtpoblacion_entrega->clear();
    ui->txtprovincia_entrega->clear();
    ui->txtHorarioEntrega->clear();
    ui->txtbase1->clear();
    ui->txtbase2->clear();
    ui->txtbase3->clear();
    ui->txtbase4->clear();
    ui->txtiva1->clear();
    ui->txtiva2->clear();
    ui->txtiva3->clear();
    ui->txtiva4->clear();
    ui->txtiva1->clear();
    ui->txtiva2->clear();
    ui->txtiva3->clear();
    ui->txtiva4->clear();
    ui->txtporc_rec1->clear();
    ui->txtporc_rec2->clear();
    ui->txtporc_rec3->clear();
    ui->txtporc_rec4->clear();
    ui->txtporc_rec1->clear();
    ui->txtporc_rec2->clear();
    ui->txtporc_rec3->clear();
    ui->txtporc_rec4->clear();
    ui->txttotal1->clear();
    ui->txttotal2->clear();
    ui->txttotal3->clear();
    ui->txttotal4->clear();
    ui->lblnombreProveedor->clear();
    ui->lblnumalb->clear();
    ui->lblnumero_pedido->clear();
}

void FrmPedidosProveedor::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);
    this->installEventFilter(this);

    QStringList orden;
    orden  << tr("pedido") <<tr("fecha") <<tr("recepción") <<tr("codigo_proveedor") <<tr("proveedor");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
        connect(m_busqueda,SIGNAL(hideMe()),this, SLOT(ocultarBusqueda()));
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));

    QPushButton *btnAdd = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir"),this);
    connect(btnAdd,SIGNAL(clicked()),this,SLOT(anadir_pedido()));
    m_busqueda->addWidget(btnAdd);

    QPushButton *btnEdit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar"),this);
    connect(btnEdit,SIGNAL(clicked()),this,SLOT(editar_pedido()));
    m_busqueda->addWidget(btnEdit);

    QPushButton *btnDelete = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    connect(btnDelete,SIGNAL(clicked()),this,SLOT(borrar_pedido()));
    m_busqueda->addWidget(btnDelete);

    m_busqueda->addSpacer();

    connect(m_busqueda,SIGNAL(key_Down_Pressed()),ui->tabla,SLOT(setFocus()));
    connect(m_busqueda,SIGNAL(key_F2_Pressed()),this,SLOT(ocultarBusqueda()));
}

void FrmPedidosProveedor::on_btnImprimir_clicked()
{
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if(!ui->tabla->currentIndex().isValid())
            return;
        int _id = model_busqueda->record(ui->tabla->currentIndex().row()).value("id").toInt();
        oPedido_proveedor->recuperar(_id);
        llenar_campos();
    }
    FrmDialogoImprimir dlg_print(this);
    dlg_print.set_email(prov.email);
    dlg_print.set_preview(false);
    if(dlg_print.exec() == dlg_print.Accepted)
    {
        bool ok  /* = oFactura->set_impresa(true)*/;
        if(ok)
        {
            ui->lbimpreso->setVisible(true);
            int valor = dlg_print.get_option();
            QMap <QString,QString> parametros_sql;
            parametros_sql["General.empresas"] = QString("id = %1").arg(Configuracion_global->idEmpresa);
            parametros_sql["Empresa.ped_pro"] = QString("id = %1").arg(oPedido_proveedor->id);
            parametros_sql["Empresa.lin_ped_pro"] = QString("id_cab = %1").arg(oPedido_proveedor->id);
            QString report = "ped_pro_"+QString::number(1);//TODO idioma en proveedores??

            QString pdfname = QString("Pedido-%1").arg(oPedido_proveedor->pedido);
            QString asunto = tr("Envio de pedido %1").arg(oPedido_proveedor->pedido);
            QString texto = tr("Estimado/a %1:\n"
                             "Le adjunto el pedido nº %2.\n\n"
                             "Un saludo.").arg(oPedido_proveedor->proveedor).arg(oPedido_proveedor->pedido);
            QMap <QString,QString> parametros;
            //TODO add parametros
            switch (valor) {
            case 1: // Impresora
                Configuracion::ImprimirDirecto(report,parametros_sql,parametros);
                break;
            case 2: // email
                Configuracion::EviarMail(report,parametros_sql,parametros,pdfname,dlg_print.get_email(),oPedido_proveedor->proveedor,asunto,texto);
                break;
            case 3: // PDF
                Configuracion::ImprimirPDF(report,parametros_sql,parametros);
                break;
            case 4: //preview
                Configuracion::ImprimirPreview(report,parametros_sql,parametros);
                break;
            default:
                break;
            }
        }
    }
}

void FrmPedidosProveedor::formatotabla()
{
    QStringList header;
    QVariantList sizes;
    header <<"id" << tr("Pedido") << tr("Fecha") <<tr("F.Recepción") <<tr("Cif/Nif") <<tr("Código") <<tr("Proveedor");
    sizes <<0 << 120 << 100 <<100 << 120 <<120 <<300;
    for(int cnt =0;cnt<header.length();cnt++)
    {
        model_busqueda->setHeaderData(cnt,Qt::Horizontal,header.at(cnt));
        ui->tabla->setColumnWidth(cnt,sizes.at(cnt).toInt());

    }
    ui->tabla->setColumnHidden(0,true);

    header.clear();
    sizes.clear();

    header << tr("id") << tr("Código") << tr("Descripción") << tr("cantidad") << tr("coste") << tr("Subtotal") << tr("%Dto")  << tr("Total");
    header  << tr("% I.V.A.") << tr("C.Real/u.") ;
    sizes << 0 << 100 << 350 << 100 << 100 <<100 <<100 << 100 <<100 << 100 <<110;
    for(int i = 0; i <header.size();i++)
    {
        ui->Lineas->setColumnWidth(i,sizes.at(i).toInt());
        modelLineas->setHeaderData(i,Qt::Horizontal,header.at(i));
    }
    ui->Lineas->setColumnHidden(0,true);


    header.clear();
    sizes.clear();
    header <<tr("id") << tr("Descripción") <<tr("Importe");
    sizes <<0 <<250 << 100;
    for(int i = 0; i <header.size();i++)
    {
        ui->tablagastos->setColumnWidth(i,sizes.at(i).toInt());
        modelgastos->setHeaderData(i,Qt::Horizontal,header.at(i));
    }
    ui->tablagastos->setColumnHidden(0,true);
}

void FrmPedidosProveedor::filter_table(QString texto, QString orden, QString modo)
{
    ui->stackedWidget->setCurrentIndex(1);

    QHash <QString,QString> h;
    h[tr("pedido")] ="pedido";
    h[tr("fecha")] = "fecha";
    h[tr("recepción")] = "recepcion";
    h[tr("codigo_proveedor")] ="codigo_proveedor";
    h[tr("proveedor")] = "proveedor";

    QString order = h.value(orden);

    if(modo==tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    model_busqueda->setQuery("select id,pedido,fecha,recepcion,cif_nif,codigo_proveedor,proveedor from ped_pro where ejercicio = "+
                    Configuracion_global->cEjercicio+" and "+order+ " like '%"+texto.trimmed()+ "%' order by "+orden +" "+modo,
                    Configuracion_global->empresaDB);
    ui->tabla->selectRow(0);
    formatotabla();
}

void FrmPedidosProveedor::on_tabla_doubleClicked(const QModelIndex &index)
{
    int id = ui->tabla->model()->data(ui->tabla->model()->index(index.row(),0),Qt::EditRole).toInt();
    oPedido_proveedor->recuperar("select * from ped_pro where id =" +QString::number(id));
    llenar_campos();
    ui->stackedWidget->setCurrentIndex(0);
    ocultarBusqueda();
}

void FrmPedidosProveedor::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    mostrarBusqueda();
}

void FrmPedidosProveedor::mostrarBusqueda()
{
    _showBarraBusqueda(m_busqueda);
    m_busqueda->doFocustoText();
}

void FrmPedidosProveedor::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}

bool FrmPedidosProveedor::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Show && obj == this)
    {
        init_querys();
    }
    else if(event->type() == QEvent::Resize)
        _resizeBarraBusqueda(m_busqueda);
    else if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Return)
            on_tabla_doubleClicked(ui->tabla->currentIndex());
        if(keyEvent->key() == Qt::Key_F1)
            if(ui->btnEditar->isEnabled())
                mostrarBusqueda();
        if(keyEvent->key() == Qt::Key_Escape)
            return true;

    }
    return MayaModule::eventFilter(obj,event);
}

void FrmPedidosProveedor::on_btnAnadirLinea_clicked()
{
    if(ui->btnGuardar->isEnabled())
    {
            frmEditLine frmeditar(this);
            frmeditar.init();
            connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(refrescar_modelo()));
            frmeditar.set_acumula(true);
            frmeditar.set_linea(0,"lin_ped_pro");
            frmeditar.set_tabla("lin_ped_pro");
            frmeditar.set_id_cab(oPedido_proveedor->id);
            frmeditar.set_tipo(false);
            if(frmeditar.exec() == QDialog::Accepted)
            {
                refrescar_modelo();
                calcular_pedido();
            }
    }
    else
    {
        QMessageBox::warning(this,tr("Gestión de Pedidos"),tr("Debe editar el pedido para añadir líneas"),tr("Aceptar"));
    }
}

void FrmPedidosProveedor::on_Lineas_doubleClicked(const QModelIndex &index)
{
    if(ui->btnGuardar->isEnabled())
    {
        int id_lin = ui->Lineas->model()->data(index.model()->index(index.row(),0)).toInt();
        if(id_lin >0)
        {
            frmEditLine frmeditar(this);
            frmeditar.init();
            connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(refrescar_modelo()));
            frmeditar.set_acumula(true);
            frmeditar.set_id_cab(oPedido_proveedor->id);
            frmeditar.set_tipo(false);
            frmeditar.set_linea(id_lin,"lin_ped_pro");
            frmeditar.set_tabla("lin_ped_pro");
            frmeditar.set_editando();
            frmeditar.exec();

            calcular_pedido();
            ui->Lineas->setFocus();
        }
        else
        {
            QMessageBox::warning(this, tr("Gestión de Pedidos"),tr("Debe editar el pedido para poder modificar las líneas"),
                                 tr("Aceptar"));
        }
    }
}

void FrmPedidosProveedor::calcular_pedido()
{
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

    QMap <int,QSqlRecord> l;
    QString error;
    l = SqlCalls::SelectRecord("lin_ped_pro",QString("id_cab=%1").arg(oPedido_proveedor->id),Configuracion_global->empresaDB,error);
    QMapIterator <int,QSqlRecord> li(l);
    while(li.hasNext())
    {
        li.next();

        subtotal += li.value().value("subtotal").toDouble();
        // base1
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva1->text().toFloat())     
            base1 += li.value().value("total").toDouble() * _dtoPP;

        // base2
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva2->text().toFloat())
            base2 += li.value().value("total").toDouble()* _dtoPP;

        // base3
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva3->text().toFloat())
            base3 += li.value().value("total").toDouble()* _dtoPP;

        // base4
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva4->text().toFloat())
            base4 += li.value().value("total").toDouble()* _dtoPP;
    }
    // añadir gastos extras
    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva1->text().toFloat())
        base1 += ui->SpinGastoDist3->value();
    iva1 = base1 * (ui->txtporc_iva1->text().toFloat()/100);
    if(ui->chkrecargo_equivalencia->isChecked())
        re1 = base1 * (ui->txtporc_rec1->text().toFloat()/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva2->text().toFloat())
        base2 += ui->SpinGastoDist3->value();
    iva2 = base2 * (ui->txtporc_iva2->text().toFloat()/100);
    if(ui->chkrecargo_equivalencia->isChecked())
        re2 = base2 * (ui->txtporc_rec1->text().toFloat()/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva3->text().toFloat())
        base3 += ui->SpinGastoDist3->value();
    iva3 = base3 * (ui->txtporc_iva3->text().toFloat()/100);
    if(ui->chkrecargo_equivalencia->isChecked())
        re3 = base3 * (ui->txtporc_rec3->text().toFloat()/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist3->value();
    iva4 = base4 * (ui->txtporc_iva4->text().toFloat()/100);
    if(ui->chkrecargo_equivalencia->isChecked())
        re4 = base4 * (ui->txtporc_rec4->text().toFloat()/100);

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


    ui->txtDto_pp->setText(Configuracion_global->toFormatoMoneda(QString::number(dtopp,'f',Configuracion_global->decimales_campos_totales)));


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

    this->moneda = moneda;
    ui->txtbase->setText             (Configuracion_global->toFormatoMoneda(QString::number(base    ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto     ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText         (Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText              (Configuracion_global->toFormatoMoneda(QString::number(iva     ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText    (Configuracion_global->toFormatoMoneda(QString::number(re      ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText            (Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));
    //ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));

    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText (Configuracion_global->toFormatoMoneda(QString::number(iva ,'f',Configuracion_global->decimales_campos_totales)));
   // ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));

    //-------------------------------------------------------------------------------------
    // Distribuidos líneas
    //--------------------------------------
    // Estos valores afectan al campo coste_real de las líneas y al artículo especificado
    //-------------------------------------------------------------------------------------
    double total_gastos;
    total_gastos = 0;
    for(int r = 0; r<=modelgastos->rowCount();r++)
    {
        total_gastos += modelgastos->record(r).field("importe").value().toDouble();
    }
    ui->txtTotal_gastos_linea->setText(Configuracion_global->toFormatoMoneda(QString::number(total_gastos,'f',Configuracion_global->decimales_campos_totales)));
    //--------------------------------------------------------------------------------------
}

void FrmPedidosProveedor::on_btnAnadir_costes_clicked()
{
    frmgastos_ped_pro frmgastos;
    frmgastos.setId_cabecera(oPedido_proveedor->id);
    frmgastos.exec();
    modelgastos->setQuery(QString("select id,descripcion,importe from gastos_ped_pro where id_cab = %1").arg(oPedido_proveedor->id),
                          Configuracion_global->empresaDB);
    calcular_pedido();
    formatotabla();
}

void FrmPedidosProveedor::on_SpinGastoDist1_valueChanged(double /*arg1*/)
{
    calcular_pedido();
}

void FrmPedidosProveedor::on_SpinGastoDist2_valueChanged(double /*arg1*/)
{
    calcular_pedido();
}

void FrmPedidosProveedor::on_SpinGastoDist3_valueChanged(double /*arg1*/)
{
    calcular_pedido();
}


void FrmPedidosProveedor::on_cboporc_iva_gasto1_currentIndexChanged(int /*index*/)
{
    calcular_pedido();
}

void FrmPedidosProveedor::on_cboporc_iva_gasto2_currentIndexChanged(int /*index*/)
{
    calcular_pedido();
}

void FrmPedidosProveedor::on_cboporc_iva_gasto3_currentIndexChanged(int /*index*/)
{
    calcular_pedido();
}

void FrmPedidosProveedor::calcular_coste_real_linea(int id_linea)
{
    // Calcular coste real
    float porcentaje;
    double tot_lin,coste,c_real;
    QMap<int,QSqlRecord>  lin;
    QString error;
    lin = SqlCalls::SelectRecord("lin_ped_pro",QString("id=%1").arg(id_linea),Configuracion_global->empresaDB,error);
    tot_lin = lin.value(id_linea).value("total").toDouble();
    coste = lin.value(id_linea).value("precio").toDouble();
    porcentaje = (tot_lin *100)/Configuracion_global->MonedatoDouble(ui->txttotal->text());
    c_real = coste + ((coste*(porcentaje/100))/lin.value(id_linea).value("cantidad").toFloat());
    QHash <QString,QVariant> h;
    h["coste_real_unidad"] =c_real;

    if(!SqlCalls::SqlUpdate(h,"lin_ped_pro",Configuracion_global->empresaDB,QString("id=%1").arg(id_linea),error))
    {
        QMessageBox::warning(this,tr("Pedidos a proveedores"),tr("Ocurrió un error al actualizar el coste_real: %1").arg(error),
                             tr("Aceptar"));
    }
    else
    {
        h.clear();
        h["coste_real"] = c_real;
        int id_art = lin.value(id_linea).value("id_articulo").toInt();
        bool success = SqlCalls::SqlUpdate(h,"articulos",Configuracion_global->groupDB,QString("id=%1").arg(id_art),error);
        if(!success)
            QMessageBox::warning(this,tr("Pedidos a proveedores"),tr("Ocurrió un error al actualizar el coste_real: %1").arg(error),
                                 tr("Aceptar"));

    }
    refrescar_modelo();
}

void FrmPedidosProveedor::calcular_coste_real_todas_lineas()
{
    // Repartir entre líneas
    for(int lin= 0;lin<=modelLineas->rowCount();lin++)
    {
        calcular_coste_real_linea(modelLineas->record(lin).value("id").toInt());
    }
    TimedMessageBox *t = new TimedMessageBox(this,tr("Proceso realizado"));
}

void FrmPedidosProveedor::on_btnRefrescar__clicked()
{
    calcular_coste_real_todas_lineas();
}

void FrmPedidosProveedor::on_spinPorc_dto_pp_valueChanged(double /*arg1*/)
{
    calcular_pedido();
}

void FrmPedidosProveedor::on_chkrecargo_equivalencia_toggled(bool checked)
{
    if (checked) {
        ui->chkrecargo_equivalencia->setChecked(true);
        oPedido_proveedor->recargo_equivalencia = 1;
        ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
        ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
        ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
        ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));
        oPedido_proveedor->porc_rec1 = ui->txtporc_rec1->text().toFloat();
        oPedido_proveedor->porc_rec2 = ui->txtporc_rec2->text().toFloat();
        oPedido_proveedor->porc_rec3 = ui->txtporc_rec3->text().toFloat();
        oPedido_proveedor->porc_rec4 = ui->txtporc_rec4->text().toFloat();
    } else {
        ui->chkrecargo_equivalencia->setChecked(false);
        oPedido_proveedor->recargo_equivalencia = 0;
        ui->txtporc_rec1->setText("0.00");
        ui->txtporc_rec2->setText("0.00");
        ui->txtporc_rec3->setText("0.00");
        ui->txtporc_rec4->setText("0.00");
        oPedido_proveedor->porc_rec1 = 0;
        oPedido_proveedor->porc_rec2 = 0;
        oPedido_proveedor->porc_rec3 = 0;
        oPedido_proveedor->porc_rec4 = 0;
    }
    calcular_pedido();
}

void FrmPedidosProveedor::on_btnPrev_clicked()
{
    if(oPedido_proveedor->prev())
        llenar_campos();
    else
        TimedMessageBox * t = new TimedMessageBox(this,tr("Principio de archivo alcanzado"));
}

void FrmPedidosProveedor::on_btnNext_clicked()
{
    if(oPedido_proveedor->next())
        llenar_campos();
    else
        TimedMessageBox * t = new TimedMessageBox(this,tr("Final de archivo alcanzado"));
}
