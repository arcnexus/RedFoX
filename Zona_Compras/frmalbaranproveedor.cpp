#include "frmalbaranproveedor.h"
#include "ui_frmalbaranproveedor.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Auxiliares/datedelegate.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/frmaddentregascuenta.h"
#include "../Auxiliares/entregascuenta.h"

FrmAlbaranProveedor::FrmAlbaranProveedor(QWidget *parent, bool showCerrar) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmAlbaranProveedor),
    prov(this),
    menuButton(QIcon(":/Icons/PNG/albaran_pro.png"),tr("Albaranes Prov."),this),
    push(new QPushButton(QIcon(":/Icons/PNG/albaran_pro.png"),"",this))
{
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de albaranes de proveedor"));
    _showCerrar = showCerrar;
    __init = false;
    this->installEventFilter(this);
}
FrmAlbaranProveedor::~FrmAlbaranProveedor()
{
    delete ui;
}

void FrmAlbaranProveedor::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);

    QStringList orden;
    orden << tr("Albarán") << tr("Fecha") <<tr("Cif/Nif") <<tr("Proveedor");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(m_busqueda,SIGNAL(hideMe()),this,SLOT(ocultarBusqueda()));
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));

    QPushButton *btnAdd = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir"),this);
    connect(btnAdd,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(btnAdd);

    QPushButton *btnEdit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar"),this);
    connect(btnEdit,SIGNAL(clicked()),this,SLOT(on_btnEditar_clicked()));
    m_busqueda->addWidget(btnEdit);

    QPushButton *btnDelete = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar"),this);
    connect(btnDelete,SIGNAL(clicked()),this,SLOT(on_btnBorrar_clicked()));
    m_busqueda->addWidget(btnDelete);

    QPushButton *btnPrint = new QPushButton(QIcon(":/Icons/PNG/print2.png"),tr("Imprimir"),this);
    connect(btnDelete,SIGNAL(clicked()),this,SLOT(on_btnImprimir_clicked()));
    m_busqueda->addWidget(btnPrint);
    m_busqueda->addSpacer();

    connect(m_busqueda,SIGNAL(key_Down_Pressed()),ui->tabla,SLOT(setFocus()));
}

void FrmAlbaranProveedor::init_querys()
{
    if(!__init)
        return;
    model_busqueda->setQuery("select id,albaran,fecha,cif_proveedor,proveedor from alb_pro order by albaran desc",Configuracion_global->empresaDB);
    formato_tabla();
}

void FrmAlbaranProveedor::init()
{
    if(__init)
        return;

    ui->setupUi(this);
    oAlbPro = new AlbaranProveedor(this);
    modelEntregas = new QSqlQueryModel(this);
    modelLineas = new QSqlQueryModel(this);
    ui->tabla_entregas->setModel(modelEntregas);
    ui->Lineas->setModel(modelLineas);

    ui->cbo_pais->setModel(Configuracion_global->paises_model);
    ui->cbo_pais->setModelColumn(1);

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));

    ui->btn_cerrar->setVisible(_showCerrar);

    ui->cboporc_iva_gasto1->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto1->setModelColumn(4);
    ui->cboporc_iva_gasto1->setCurrentIndex(ui->cboporc_iva_gasto1->findText(Configuracion_global->ivaList.at(0)));

    ui->cboporc_iva_gasto2->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto2->setModelColumn(4);
    ui->cboporc_iva_gasto2->setCurrentIndex(ui->cboporc_iva_gasto2->findText(Configuracion_global->ivaList.at(0)));

    ui->cboporc_iva_gasto3->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto3->setModelColumn(4);
    ui->cboporc_iva_gasto3->setCurrentIndex(ui->cboporc_iva_gasto3->findText(Configuracion_global->ivaList.at(0)));

    // control estado widgets
    ui->btnAnterior->setEnabled(false);
    ui->btnBorrar->setEnabled(false);
    ui->btnImprimir->setEnabled(false);
    ui->btnFacturar->setEnabled(false);
    oAlbPro->id = 0;
    ui->lblAlbaran->setText("");
    ui->lblproveedor->setText("");

    ui->lbfacturado->setVisible(false);
    ui->lblNumFactura->setVisible(false);
    ui->txtfecha_factura->setVisible(false);
    ui->txtcNumFra->setVisible(false);

    // Modelo Tabla
    model_busqueda = new QSqlQueryModel(this);
    ui->tabla->setModel(model_busqueda);
    ui->tabla->selectRow(0);

    ui->stackedWidget->setCurrentIndex(1);

    setUpBusqueda();

    bloquearcampos(true);

    // CONTROL DE EVENTOS
    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;

    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);

    ui->tabla->setItemDelegateForColumn(2,new DateDelegate(this));
    ui->tabla_entregas->setItemDelegateForColumn(1,new DateDelegate );
    ui->tabla_entregas->setItemDelegateForColumn(3, new MonetaryDelegate);
    __init = true;
}

void FrmAlbaranProveedor::llenarProveedor(int id)
{
    prov.clear();

    prov.Recuperar(id);
    ui->txtcodigo_proveedor->setText(prov.codigo);
    ui->txtproveedor->setText(prov.proveedor);
    ui->txtdireccion1->setText(prov.direccion1);
    ui->txtdireccion2->setText(prov.direccion2);
    ui->txtpoblacion->setText(prov.poblacion);
    ui->txtprovincia->setText(prov.provincia);
    ui->txtcp->setText(prov.cp);
    ui->txtcif->setText(prov.cif);
    int ind = ui->cbo_pais->findText(Configuracion_global->Devolver_pais(prov.id_pais));
    ui->cbo_pais->setCurrentIndex(ind);
    ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(prov.entregado_a_cuenta,'f',Configuracion_global->decimales)));
    ui->chklporc_rec->setChecked(prov.recargo_equivalencia);
    ui->lblproveedor->setText(prov.proveedor);
    oAlbPro->id_proveedor = prov.id;
}

void FrmAlbaranProveedor::llenar_tabla_entregas()
{
    modelEntregas->setQuery("select id, fecha_entrega, concepto, importe from proveedor_a_cuenta where id_proveedor = "+
                            QString::number(prov.id),Configuracion_global->groupDB);   
    ui->tabla_entregas->setColumnHidden(0,true);
    ui->tabla_entregas->setColumnWidth(1,120);
    ui->tabla_entregas->setColumnWidth(2,400);
    ui->tabla_entregas->setColumnWidth(3,120);

    QStringList cabecera;
    cabecera << tr("id") << tr("Fecha entrega") << tr("Concepto") << tr("Importe");
    for(int pos = 0; pos < cabecera.size();pos++)
    {
        modelEntregas->setHeaderData(pos,Qt::Horizontal,cabecera.at(pos));
    }
}

void FrmAlbaranProveedor::bloquearcampos(bool state)
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
    // SpinBox

    QList<QDoubleSpinBox *> DSpinBoxList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *DSpinBox;
    foreach (DSpinBox, DSpinBoxList) {
        DSpinBox->setReadOnly(state);
    }
    // CheckBox
    QList<QCheckBox *> CheckBoxList = this->findChildren<QCheckBox *>();
    QCheckBox *CheckBox;
    foreach (CheckBox, CheckBoxList) {
        CheckBox->setEnabled(!state);
    }
    // QTextEdit
    QList<QTextEdit *> TextEditList = this->findChildren<QTextEdit *>();
    QTextEdit *TextEdit;
    foreach (TextEdit,TextEditList) {
        TextEdit->setReadOnly(state);
    }
    // QDateEdit
    QList<QDateEdit *> DateEditList = this->findChildren<QDateEdit *>();
    QDateEdit *DateEdit;
    foreach (DateEdit, DateEditList) {
        DateEdit->setEnabled(!state);
    }

    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    ui->btnBorrar->setEnabled(state);
    ui->btnDeshacer->setEnabled(!state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->btnAnadirLinea->setEnabled(!state);

    ui->btn_borrarLinea->setEnabled(!state);
    ui->botBuscarProv->setEnabled(!state);
    ui->btnFacturar->setEnabled(!state);
    m_busqueda->block(!state);
}

void FrmAlbaranProveedor::formato_tabla()
{
    QStringList header;
    header <<"id" <<tr("Albarán") <<tr("fecha") <<tr("CIF/NIF") <<tr("Proveedor");
    QVariantList sizes;
    sizes << 0 << 120 << 120 << 120 <<300;
    for(int i = 0; i<header.length();i++)
    {
        ui->tabla->setColumnWidth(i,sizes.at(i).toInt());
        model_busqueda->setHeaderData(i,Qt::Horizontal,header.at(i));
    }

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

void FrmAlbaranProveedor::llenarLineas()
{
    if(!__init)
        return;
    modelLineas->setQuery(QString("select id,id_articulo,codigo,descripcion,cantidad,precio,subtotal, porc_dto,total,porc_iva,coste_real_unidad  "
                              "from lin_alb_pro where id_cab = %1;").arg(oAlbPro->id),Configuracion_global->empresaDB);
    formato_tabla();
    calcular_albaran();
}

void FrmAlbaranProveedor::calcular_albaran()
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

    //TODO revisar comentarios esa funcion
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
        re1 = base1 * (ui->txtporc_rec1->text().toFloat()/100);
    if(ui->chklporc_rec->isChecked())
        re2 = base2 * (ui->txtporc_rec2->text().toFloat()/100);
    if(ui->chklporc_rec->isChecked())
        re3 = base3 * (ui->txtporc_rec3->text().toFloat()/100);
    if(ui->chklporc_rec->isChecked())
        re4 = base4 * (ui->txtporc_rec4->text().toFloat()/100);

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

    this->moneda = moneda;
    ui->txtbase->setText             (Configuracion_global->toFormatoMoneda(QString::number(base    ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto     ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText         (Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva_total->setText              (Configuracion_global->toFormatoMoneda(QString::number(iva     ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText    (Configuracion_global->toFormatoMoneda(QString::number(re      ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText            (Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));

    ui->txtbase_total->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_iva->setText (Configuracion_global->toFormatoMoneda(QString::number(iva ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtimporte_rec_total->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));

    //ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    //ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    //ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));

}

void FrmAlbaranProveedor::filter_table(QString texto, QString orden, QString modo)
{
    ui->stackedWidget->setCurrentIndex(1);

    QHash <QString,QString> h;
    h[tr("Albarán")] = "albaran";
    h[tr("Fecha")] = "fecha";
    h[tr("Cif/Nif")] = "cif_proveedor";
    h[tr("Proveedor")] = "proveedor";
    QString order = h.value(orden);

    if (modo == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    model_busqueda->setQuery("select id,albaran,fecha,cif_proveedor,proveedor from alb_pro "
                    "where "+order+" like '%"+texto.trimmed()+"%' order by "+order+" "+modo,Configuracion_global->empresaDB);
    ui->tabla->selectRow(0);
}


void FrmAlbaranProveedor::on_btnSiguiente_clicked()
{
    oAlbPro->Recuperar("Select * from alb_pro where id >"+QString::number(oAlbPro->id)+" limit 0,1",1);
    llenar_campos();
}

void FrmAlbaranProveedor::on_btnAnterior_clicked()
{
    oAlbPro->Recuperar("Select * from alb_pro where id <"+QString::number(oAlbPro->id)+" order by id desc limit 0,1",2);
    llenar_campos();
}

void FrmAlbaranProveedor::llenar_campos()
{

    ui->txtalbaran->setText(oAlbPro->albaran);
    ui->lblAlbaran->setText(oAlbPro->albaran);
    ui->txtfecha->setDate(oAlbPro->fecha);

    ui->txtproveedor->setText(oAlbPro->proveedor);
    ui->txtcif->setText(oAlbPro->cif_proveedor);
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->total1,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->total2,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->total3,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->total4,'f',Configuracion_global->decimales)));

    ui->txtporc_iva1->setText(QString::number(oAlbPro->porc_iva1));
    ui->txtporc_iva2->setText(QString::number(oAlbPro->porc_iva2));
    ui->txtporc_iva3->setText(QString::number(oAlbPro->porc_iva3));
    ui->txtporc_iva4->setText(QString::number(oAlbPro->porc_iva4));

    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->iva1,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->iva2,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->iva3,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->iva4,'f',Configuracion_global->decimales)));

    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->base1,'f',Configuracion_global->decimales)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->base2,'f',Configuracion_global->decimales)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->base3,'f',Configuracion_global->decimales)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->base4,'f',Configuracion_global->decimales)));

    ui->txtcNumFra->setText(oAlbPro->factura);
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->base_total,'f',Configuracion_global->decimales)));
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->iva_total,'f',Configuracion_global->decimales)));

    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->total,'f',Configuracion_global->decimales)));
    ui->txtcomentario->setText(oAlbPro->comentario);

    ui->txtpedido_cliente->setText(QString::number(oAlbPro->pedido));
    ui->chklporc_rec->setChecked(oAlbPro->recargo_equivalencia);
    ui->txtporc_rec1->setText(QString::number(oAlbPro->porc_rec1));
    ui->txtporc_rec2->setText(QString::number(oAlbPro->porc_rec2));
    ui->txtporc_rec3->setText(QString::number(oAlbPro->porc_rec3));
    ui->txtporc_rec4->setText(QString::number(oAlbPro->porc_rec4));

    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->rec1,'f',Configuracion_global->decimales)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->rec2,'f',Configuracion_global->decimales)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->rec3,'f',Configuracion_global->decimales)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->rec4,'f',Configuracion_global->decimales)));

    ui->txtimporte_rec_total->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->total_recargo,'f',Configuracion_global->decimales)));

    /*
    oAlbPro->ejercicio
    oAlbPro->subtotal

    oAlbPro->porc_dto
    oAlbPro->dto
    oAlbPro->telefono
    oAlbPro->fax
    oAlbPro->movil

    oAlbPro->id_forma_pago
    oAlbPro->impreso
    oAlbPro->desc_gasto1
    oAlbPro->desc_gasto2
    oAlbPro->desc_gasto3
    oAlbPro->imp_gasto1
    oAlbPro->imp_gasto2
    oAlbPro->imp_gasto3
    oAlbPro->porc_iva_gasto1
    oAlbPro->porc_iva_gasto2
    oAlbPro->porc_iva_gasto3
    oAlbPro->iva_gasto1
    oAlbPro->iva_gasto2
    oAlbPro->iva_gasto3
    oAlbPro->gasto_to_coste
    oAlbPro->editable
    ///*/

    ui->txtcodigo_proveedor->setText(oAlbPro->codigo_proveedor);
    ui->txtdireccion1->setText(oAlbPro->direccion1);
    ui->txtdireccion2->setText(oAlbPro->direccion2);
    ui->txtpoblacion->setText(oAlbPro->poblacion);
    ui->txtprovincia->setText(oAlbPro->provincia);
    ui->txtcp->setText(oAlbPro->cp);
    ui->cbo_pais->setCurrentIndex(ui->cbo_pais->findText(Configuracion_global->Devolver_pais(oAlbPro->id_pais)));

    //ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(prov.entregado_a_cuenta,'f',Configuracion_global->decimales)));

    if(ui->txtcNumFra->text().isEmpty())
    {
        ui->btnFacturar->setEnabled(true);
        ui->lbfacturado->setVisible(false);
        ui->lblNumFactura->setVisible(false);
        ui->txtfecha_factura->setVisible(false);
        ui->txtcNumFra->setVisible(false);
    } else
    {
        ui->btnFacturar->setEnabled(false);
        ui->lbfacturado->setVisible(true);
        ui->lblNumFactura->setVisible(true);
        ui->txtfecha_factura->setVisible(true);
        ui->txtcNumFra->setVisible(true);
    }

    llenar_tabla_entregas();
    llenarLineas();
    calcular_albaran();
}

void FrmAlbaranProveedor::on_btnEditar_clicked()
{
    Configuracion_global->transaction();
    bloquearcampos(false);
    emit block();
    ui->stackedWidget->setCurrentIndex(0);
    ui->txtcodigo_proveedor->setFocus();
}

void FrmAlbaranProveedor::on_btnGuardar_clicked()
{
    if(!ui->txtalbaran->text().isEmpty())
    {
        guardar_campos_en_objeto();
        if(oAlbPro->guardar())
        {
            llenar_campos();
            bloquearcampos(true);
            Configuracion_global->commit();
            emit unblock();
        }
    }
    else
    {
        QMessageBox::information(this,tr("Gestión de Albaranes de proveedor"),
                                 tr("Debe especificar el numero de albarán antes de guardar"),tr("Aceptar"));
        ui->txtalbaran->setFocus();
    }
}

void FrmAlbaranProveedor::guardar_campos_en_objeto()
{
    oAlbPro->albaran = ui->txtalbaran->text();
    oAlbPro->fecha = ui->txtfecha->date();
    oAlbPro->proveedor = ui->txtproveedor->text();
    oAlbPro->cif_proveedor = ui->txtcif->text();
    oAlbPro->total1 = ui->txttotal1->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->total2 = ui->txttotal2->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->total3 = ui->txttotal3->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->total4 = ui->txttotal4->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->porc_iva1 = ui->txtporc_iva1->text().toFloat();
    oAlbPro->porc_iva2 = ui->txtporc_iva2->text().toFloat();
    oAlbPro->porc_iva3 = ui->txtporc_iva3->text().toFloat();
    oAlbPro->porc_iva4 = ui->txtporc_iva4->text().toFloat();
    oAlbPro->iva1 = ui->txtiva1->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->iva2 = ui->txtiva2->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->iva3 = ui->txtiva3->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->iva4 = ui->txtiva4->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->porc_rec1 = ui->txtporc_rec1->text().toFloat();
    oAlbPro->porc_rec1 = ui->txtporc_rec1->text().toFloat();
    oAlbPro->porc_rec1 = ui->txtporc_rec1->text().toFloat();
    oAlbPro->porc_rec1 = ui->txtporc_rec1->text().toFloat();
    oAlbPro->rec1 = ui->txtrec1->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->rec2 = ui->txtrec2->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->rec3 = ui->txtrec3->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->rec4 = ui->txtrec4->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->base1 = ui->txtbase1->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->base2 = ui->txtbase2->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->base3 = ui->txtbase3->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->base4 = ui->txtbase4->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->factura = ui->txtcNumFra->text().replace(".","").replace(",",".").toDouble();
    oAlbPro->base_total = ui->txtbase->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->iva_total = ui->txtiva_total->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->total_recargo = ui->txtimporte_rec_total->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble();
    oAlbPro->total = ui->txttotal->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->comentario = ui->txtcomentario->toPlainText();
}

void FrmAlbaranProveedor::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    mostrarBusqueda();
}

void FrmAlbaranProveedor::on_btnAnadir_clicked()
{
    Configuracion_global->transaction();
    if(oAlbPro->anadir())
    {
        oAlbPro->Recuperar(oAlbPro->id);
        emit block();
        llenar_campos();
        bloquearcampos(false);
        ui->stackedWidget->setCurrentIndex(0);
        ui->txtcodigo_proveedor->setFocus();
    }
    else
    {
        Configuracion_global->rollback();
    }
}

void FrmAlbaranProveedor::on_btnBorrar_clicked()
{
    if(QMessageBox::warning(this,tr("Gestión de albaranes de proveedor"),
                            tr("¿Seguro que desea borrar ese albarán?"),tr("NO"),tr("SÍ")) == QMessageBox::Accepted)
    {
        oAlbPro->borrar(oAlbPro->id);
        on_btnAnterior_clicked();
    }
}

void FrmAlbaranProveedor::on_btnAnadirEntrega_clicked()
{
    frmAddEntregasCuenta frmEntregas(this);
    if(frmEntregas.exec() == QDialog::Accepted){
        EntregasCuenta oEntrega(this);
        if(!oEntrega.Anadir(2,prov.id,frmEntregas.importe,frmEntregas.fecha,frmEntregas.concepto))
            QMessageBox::warning(this,tr("Gestión de proveedores"),
                                 tr("Falló el insertar una nueva entrega a cuenta"),
                                 tr("Aceptar"));
        else
            llenar_tabla_entregas();
    }

}

void FrmAlbaranProveedor::on_radBusqueda_toggled(bool checked)
{
    if(checked)
        ui->stackedWidget->setCurrentIndex(1);
    else
        ui->stackedWidget->setCurrentIndex(0);
}

void FrmAlbaranProveedor::on_cboOrdenar_por_currentIndexChanged(const QString &arg1)
{
    QHash <QString,QString> h;
    h[tr("albarán")] = "albaran";
    h[tr("fecha")] = "fecha";
    h[tr("cif/nif")] = "cif_proveedor";
    h[tr("Proveedor")] = "proveedor";
    QString order = h.value(arg1);
    if (order == "albaran" || order =="fecha")
        model_busqueda->setQuery("select id,albaran,fecha,cif_proveedor,proveedor from alb_pro order by "+order+" desc",Configuracion_global->empresaDB);
    else
        model_busqueda->setQuery("select id,albaran,fecha,cif_proveedor,proveedor from alb_pro order by "+order,Configuracion_global->empresaDB);
}

void FrmAlbaranProveedor::on_tabla_doubleClicked(const QModelIndex &index)
{
    int id = Configuracion_global->devolver_id_tabla(model_busqueda,index);
    oAlbPro->Recuperar(id);
    llenar_campos();
    bloquearcampos(true);
    ui->stackedWidget->setCurrentIndex(0);
    ocultarBusqueda();
}

void FrmAlbaranProveedor::mostrarBusqueda()
{
    ui->stackedWidget->setCurrentIndex(1);
    _showBarraBusqueda(m_busqueda);
    m_busqueda->doFocustoText();
}

void FrmAlbaranProveedor::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}
bool FrmAlbaranProveedor::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Show && obj == this)
    {
        if(!__init)
            init();
        init_querys();
    }

    if(event->type() == QEvent::Resize)
    {
        if(!__init)
            init();
        _resizeBarraBusqueda(m_busqueda);
    }


    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Return)
        {
            on_tabla_doubleClicked(ui->tabla->currentIndex());
            return true;
        }

        if(keyEvent->key() == Qt::Key_Escape)
            return true;

        if(keyEvent->key() == Qt::Key_F1)
        {
            if(ui->btnEditar->isEnabled())
            {
                if(m_busqueda->isShow())
                    ocultarBusqueda();
                else
                    mostrarBusqueda();
                return true;
            }
        }        
    }
    return MayaModule::eventFilter(obj,event);
}

void FrmAlbaranProveedor::on_btnImprimir_clicked()
{
    //TODO - IMPRIMIR
}

void FrmAlbaranProveedor::on_btnDeshacer_clicked()
{
    Configuracion_global->rollback();
    llenar_campos();
    bloquearcampos(true);
    emit unblock();
}

void FrmAlbaranProveedor::on_botBuscarProv_clicked()
{
    db_consulta_view consulta;
    QStringList campos;
    campos << "proveedor";
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
        llenarProveedor(id_proveedor);
        oAlbPro->id_proveedor = id_proveedor;
    }
}

void FrmAlbaranProveedor::on_chklporc_rec_toggled(bool checked)
{
    if (checked) {
        oAlbPro->recargo_equivalencia = 1;
        ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
        ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
        ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
        ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));
        oAlbPro->porc_rec1 = ui->txtporc_rec1->text().toFloat();
        oAlbPro->porc_rec2 = ui->txtporc_rec2->text().toFloat();
        oAlbPro->porc_rec3 = ui->txtporc_rec3->text().toFloat();
        oAlbPro->porc_rec4 = ui->txtporc_rec4->text().toFloat();
    } else {
        oAlbPro->recargo_equivalencia = 0;
        ui->txtporc_rec1->setText("0.00");
        ui->txtporc_rec2->setText("0.00");
        ui->txtporc_rec3->setText("0.00");
        ui->txtporc_rec4->setText("0.00");
        oAlbPro->porc_rec1 = 0;
        oAlbPro->porc_rec2 = 0;
        oAlbPro->porc_rec3 = 0;
        oAlbPro->porc_rec4 = 0;
    }
    calcular_albaran();
}
