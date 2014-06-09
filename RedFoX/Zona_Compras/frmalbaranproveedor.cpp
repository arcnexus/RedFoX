#include "frmalbaranproveedor.h"
#include "ui_frmalbaranproveedor.h"
#include "../Busquedas/db_consulta_view.h"
#include "Auxiliares/datedelegate.h"
#include "Auxiliares/monetarydelegate.h"
#include "Auxiliares/frmaddentregascuenta.h"
#include "Auxiliares/entregascuenta.h"
#include "Auxiliares/frmeditline.h"

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
    connect(btnPrint,SIGNAL(clicked()),this,SLOT(on_btnImprimir_clicked()));
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
    ui->stackedWidget->setCurrentIndex(1);    
    ui->tabWidget_2->setCurrentIndex(0);
}

void FrmAlbaranProveedor::init()
{
    if(__init)
        return;

    ui->setupUi(this);
    oAlbPro = new AlbaranProveedor(this);

    modelLineas = new QSqlQueryModel(this);
    lineas_anterior = new QSqlQueryModel(this);


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
    ui->chklporc_rec->setChecked(prov.recargo_equivalencia);
    ui->lblproveedor->setText(prov.proveedor);
    oAlbPro->id_proveedor = prov.id;
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

    oAlbPro->dto
    oAlbPro->telefono
    oAlbPro->fax
    oAlbPro->movil

    oAlbPro->id_forma_pago
    oAlbPro->impreso

    oAlbPro->gasto_to_coste
    oAlbPro->editable
    ///*/

    ui->txtGastoDist1->setText(oAlbPro->desc_gasto1);
    ui->txtGastoDist2->setText(oAlbPro->desc_gasto2);
    ui->txtGastoDist3->setText(oAlbPro->desc_gasto3);
    ui->SpinGastoDist1->setValue(oAlbPro->imp_gasto1);
    ui->SpinGastoDist2->setValue(oAlbPro->imp_gasto2);
    ui->SpinGastoDist3->setValue(oAlbPro->imp_gasto3);

    ui->cboporc_iva_gasto1->setCurrentText(QString::number(oAlbPro->porc_iva_gasto1));
    ui->cboporc_iva_gasto2->setCurrentText(QString::number(oAlbPro->porc_iva_gasto2));
    ui->cboporc_iva_gasto3->setCurrentText(QString::number(oAlbPro->porc_iva_gasto3));

    ui->spiniva_gasto1->setValue(oAlbPro->iva_gasto1);
    ui->spiniva_gasto2->setValue(oAlbPro->iva_gasto2);
    ui->spiniva_gasto3->setValue(oAlbPro->iva_gasto3);
    ui->spinPorc_dto_pp->setValue(oAlbPro->porc_dto);

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
        ui->txtcNumFra->setVisible(false);
    } else
    {
        ui->btnFacturar->setEnabled(false);
        ui->lbfacturado->setVisible(true);
        ui->lblNumFactura->setVisible(true);
        ui->txtcNumFra->setVisible(true);
    }
    llenarLineas();
    calcular_albaran();
}

void FrmAlbaranProveedor::on_btnEditar_clicked()
{
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if(!ui->tabla->currentIndex().isValid())
            return;
        int id = model_busqueda->record(ui->tabla->currentIndex().row()).value("id").toInt();
        oAlbPro->Recuperar(id);
    }
    if(!oAlbPro->factura.isEmpty())
    {
        QMessageBox::warning(this,tr("No puede editar este albarán"),tr("Este albarán ha sido facturado.\nNo se puede editar"));
        return;
    }

    total_anterior = oAlbPro->base_total;
    fecha_anterior = oAlbPro->fecha;
    lineas_anterior->setQuery(QString("select id,id_articulo,cantidad,total from lin_alb_pro where id_cab = %1;").arg(oAlbPro->id),Configuracion_global->empresaDB);
    editando = true;
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
            double acumular = oAlbPro->base_total;
            if(editando)
            {
                //BORRAR LOS ACUMULADOS ANTERIORES COMPLETAMENTE
                if(!Proveedor::acumular(oAlbPro->id_proveedor,fecha_anterior.month(),-1.0 * total_anterior))
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
            }

            if(Proveedor::acumular(oAlbPro->id_proveedor,oAlbPro->fecha.month(),acumular))
            {
                for(auto i=0; i<modelLineas->rowCount();++i)
                {
                    QSqlRecord r = modelLineas->record(i);
                    if(!Articulo::acumulado_compras(r.value("id_articulo").toInt(),r.value("cantidad").toDouble(),r.value("total").toDouble(),oAlbPro->fecha))
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
    oAlbPro->codigo_proveedor = ui->txtcodigo_proveedor->text();
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
    oAlbPro->factura = ui->txtcNumFra->text();
    oAlbPro->base_total = ui->txtbase->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->iva_total = ui->txtiva_total->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->total_recargo = ui->txtimporte_rec_total->text().replace(".","").replace(moneda,"").replace(".","").replace(",",".").toDouble();
    oAlbPro->total = ui->txttotal->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->comentario = ui->txtcomentario->toPlainText();


    //oAlbPro->id_proveedor =
    //oAlbPro->pedido =
    oAlbPro->recargo_equivalencia = ui->chklporc_rec->isChecked();
    oAlbPro->ejercicio = ui->txtfecha->date().year();
    oAlbPro->subtotal = Configuracion_global->MonedatoDouble(ui->txtsubtotal->text());
    oAlbPro->direccion1 = ui->txtdireccion1->text();
    oAlbPro->direccion2 = ui->txtdireccion2->text();
    oAlbPro->cp = ui->txtcp->text();
    oAlbPro->poblacion = ui->txtpoblacion->text();
    oAlbPro->provincia = ui->txtprovincia->text();
    oAlbPro->id_pais = Configuracion_global->paises_model->record(ui->cbo_pais->currentIndex()).value("id").toInt();
    oAlbPro->porc_dto = ui->spinPorc_dto_pp->value();
    oAlbPro->dto = Configuracion_global->MonedatoDouble(ui->txtimporte_descuento->text());
    //oAlbPro->telefono
    //oAlbPro->fax =
    //oAlbPro->movil =
    oAlbPro->codigo_proveedor = ui->txtcodigo_proveedor->text();
    //oAlbPro->id_forma_pago =
    //oAlbPro->impreso =
    oAlbPro->desc_gasto1 = ui->txtGastoDist1->text();
    oAlbPro->desc_gasto2 = ui->txtGastoDist2->text();
    oAlbPro->desc_gasto3 = ui->txtGastoDist3->text();
    oAlbPro->imp_gasto1 = ui->SpinGastoDist1->value();
    oAlbPro->imp_gasto2 = ui->SpinGastoDist2->value();
    oAlbPro->imp_gasto3 = ui->SpinGastoDist3->value();
    oAlbPro->porc_iva_gasto1 = ui->cboporc_iva_gasto1->currentText().toDouble();
    oAlbPro->porc_iva_gasto2 = ui->cboporc_iva_gasto3->currentText().toDouble();
    oAlbPro->porc_iva_gasto3 = ui->cboporc_iva_gasto2->currentText().toDouble();
    oAlbPro->iva_gasto1 = ui->spiniva_gasto1->value();
    oAlbPro->iva_gasto2 = ui->spiniva_gasto2->value();
    oAlbPro->iva_gasto3 = ui->spiniva_gasto3->value();
    //oAlbPro->gasto_to_coste =
    //oAlbPro->editable =
}

void FrmAlbaranProveedor::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    mostrarBusqueda();
}

void FrmAlbaranProveedor::on_btnAnadir_clicked()
{
    editando = false;
    Configuracion_global->transaction();
    if(oAlbPro->anadir())
    {
        oAlbPro->Recuperar(oAlbPro->id);
        emit block();
        llenar_campos();
        bloquearcampos(false);
        ui->stackedWidget->setCurrentIndex(0);
        ui->tabWidget_2->setCurrentIndex(0);
        ocultarBusqueda();
        ui->txtcodigo_proveedor->setFocus();
        ui->txtfecha->setDate(QDate::currentDate());
    }
    else
    {
        Configuracion_global->rollback();
    }
}

void FrmAlbaranProveedor::on_btnBorrar_clicked()
{
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if(!ui->tabla->currentIndex().isValid())
            return;
        int _id = model_busqueda->record(ui->tabla->currentIndex().row()).value("id").toInt();
        oAlbPro->Recuperar(_id);
    }
    if(!oAlbPro->factura.isEmpty())
    {
        QMessageBox::warning(this,tr("No puede borrar este albarán"),tr("Este albarán ha sido facturado.\nNo se puede borrar"));
        return;
    }

    if(QMessageBox::warning(this,tr("Gestión de albaranes de proveedor"),
                            tr("¿Seguro que desea borrar ese albarán?"),tr("NO"),tr("SÍ")) == QMessageBox::Accepted)
    {
        oAlbPro->borrar();
        mostrarBusqueda();
    }
}

//void FrmAlbaranProveedor::on_cboOrdenar_por_currentIndexChanged(const QString &arg1)
//{
//    QHash <QString,QString> h;
//    h[tr("albarán")] = "albaran";
//    h[tr("fecha")] = "fecha";
//    h[tr("cif/nif")] = "cif_proveedor";
//    h[tr("Proveedor")] = "proveedor";
//    QString order = h.value(arg1);
//    if (order == "albaran" || order =="fecha")
//        model_busqueda->setQuery("select id,albaran,fecha,cif_proveedor,proveedor from alb_pro order by "+order+" desc",Configuracion_global->empresaDB);
//    else
//        model_busqueda->setQuery("select id,albaran,fecha,cif_proveedor,proveedor from alb_pro order by "+order,Configuracion_global->empresaDB);
//}

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

    if(event->type() == QEvent::Resize && obj == this)
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
    if(ui->stackedWidget->currentIndex() == 1)
    {
        if(!ui->tabla->currentIndex().isValid())
            return;
        int id = model_busqueda->record(ui->tabla->currentIndex().row()).value("id").toInt();
        oAlbPro->Recuperar(id);
    }
    FrmDialogoImprimir dlg_print(this);
    dlg_print.set_email(prov.email);
    dlg_print.set_preview(false);
    if(dlg_print.exec() == dlg_print.Accepted)
    {
        bool ok  = true/* = oFactura->set_impresa(true)*/;
        if(ok)
        {
            //ui->lbimpreso->setVisible(true);
            int valor = dlg_print.get_option();
            QMap <QString,QString> parametros_sql;
            parametros_sql["General.empresas"] = QString("id = %1").arg(Configuracion_global->idEmpresa);
            parametros_sql["Empresa.alb_pro"] = QString("id = %1").arg(oAlbPro->id);
            parametros_sql["Empresa.lin_alb_pro"] = QString("id_cab = %1").arg(oAlbPro->id);
            QString report = "alb_pro_"+QString::number(1);//TODO idioma en proveedores??

            QString pdfname = QString("Pedido-%1").arg(oAlbPro->pedido);
            QString asunto = tr("Envio de albarán %1").arg(oAlbPro->pedido);
            QString texto = tr("Estimado/a %1:\n"
                             "Le adjunto el albarán nº %2.\n\n"
                             "Un saludo.").arg(oAlbPro->proveedor).arg(oAlbPro->pedido);
            QMap <QString,QString> parametros;
            //TODO add parametros
            switch (valor) {
            case 1: // Impresora
                Configuracion::ImprimirDirecto(report,parametros_sql,parametros);
                break;
            case 2: // email
                Configuracion::EviarMail(report,parametros_sql,parametros,pdfname,dlg_print.get_email(),oAlbPro->proveedor,asunto,texto);
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
    QString error;
    for(auto i = 0; i<modelLineas->rowCount();++i)
    {
        QSqlRecord r = modelLineas->record(i);
        QHash<QString,QVariant> lin;
        double iva_art = r.value("porc_iva").toDouble();
        double porc_re_art;

        if(iva_art == oAlbPro->porc_iva1)
            porc_re_art = oAlbPro->porc_rec1;
        if(iva_art == oAlbPro->porc_iva2)
            porc_re_art = oAlbPro->porc_rec2;
        if(iva_art == oAlbPro->porc_iva3)
            porc_re_art = oAlbPro->porc_rec3;
        if(iva_art == oAlbPro->porc_iva4)
            porc_re_art = oAlbPro->porc_rec4;

        lin["porc_rec"] = porc_re_art;
        lin["rec"] = r.value("total").toDouble() * (porc_re_art/100.0);

        if(!SqlCalls::SqlUpdate(lin,"lin_alb_pro",Configuracion_global->empresaDB,QString("id=%1").arg(r.value("id").toInt()),error))
        {
            QMessageBox::critical(this,tr("Error al actualizar lineas"),error);
            break;
        }
    }
    calcular_albaran();
}

void FrmAlbaranProveedor::on_btnAnadirLinea_clicked()
{
    frmEditLine frmeditar(this);
    frmeditar.init();
    connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(llenarLineas()));

    frmeditar.set_venta(false);
    frmeditar.setTipoDoc(frmEditLine::Albaran);
    frmeditar.setAdd_pendientes(false);

    frmeditar.setUse_re(ui->chklporc_rec->isChecked());

    frmeditar.set_linea(0,"lin_alb_pro");
    frmeditar.set_tabla("lin_alb_pro");
    frmeditar.set_id_cab(oAlbPro->id);

    frmeditar.exec();
    llenarLineas();
}

void FrmAlbaranProveedor::on_btn_borrarLinea_clicked()
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
            if(ok && SqlCalls::SqlDelete("lin_alb_pro",Configuracion_global->empresaDB,QString("id = %1").arg(id_lin),error))
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
            if(ok && SqlCalls::SqlDelete("lin_alb_pro",Configuracion_global->empresaDB,QString("id = %1").arg(id_lin),error))
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

void FrmAlbaranProveedor::on_Lineas_doubleClicked(const QModelIndex &index)
{
    int id_lin = ui->Lineas->model()->data(index.model()->index(index.row(),0)).toInt();
    if(id_lin >0)
    {
        frmEditLine frmeditar(this);
        frmeditar.init();
        connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(llenarLineas()));

        frmeditar.set_venta(false);
        frmeditar.setTipoDoc(frmEditLine::Albaran);
        frmeditar.setAdd_pendientes(false);

        frmeditar.setUse_re(ui->chklporc_rec->isChecked());

        frmeditar.set_id_cab(oAlbPro->id);
        frmeditar.set_linea(id_lin,"lin_alb_pro");
        frmeditar.set_tabla("lin_alb_pro");
        frmeditar.set_editando();
        frmeditar.exec();

        calcular_albaran();
        ui->Lineas->setFocus();
    }
}

void FrmAlbaranProveedor::on_btnFacturar_clicked()
{
    if(!oAlbPro->factura.isEmpty())
        return;

    if(QMessageBox::question(this,tr("¿Facturar albarán?"),tr("¿Desea facturar este albarán?"),
            tr("No"),tr("Facturar")) == QMessageBox::Accepted)
    {
        QDialog d(this);
        d.setWindowTitle(tr("Idique fecha y nº de documento"));

        QLabel l_fecha(tr("Fecha:"),&d);
        QLabel l_numero(tr("Nº de documento"),&d);

        QDateEdit dateedit(QDate::currentDate(),&d);
        QLineEdit lineedit(&d);

        QPushButton btn_accept(tr("Aceptar"),&d);
        QPushButton btn_cancel(tr("Cancelar"),&d);


        QGridLayout layout;

        layout.addWidget(&l_fecha,0,0,1,1);
        layout.addWidget(&dateedit,0,1,1,1);

        layout.addWidget(&l_numero,1,0,1,1);
        layout.addWidget(&lineedit,1,1,1,1);

        layout.addWidget(&btn_cancel,2,0,1,1);
        layout.addWidget(&btn_accept,2,1,1,1);

        d.setLayout(&layout);

        connect(&btn_accept,SIGNAL(clicked()),&d,SLOT(accept()));
        connect(&btn_cancel,SIGNAL(clicked()),&d,SLOT(reject()));

        QString doc = "";
        do
        {
            if(d.exec() == QDialog::Rejected)
            {
                return;
            }
            doc = lineedit.text();
        }
        while(doc.isEmpty());

        QHash<QString,QVariant> _data;
        _data["factura"] = doc;
        _data["fecha"] = dateedit.date();
        _data["id_proveedor"] = oAlbPro->id_proveedor;
        _data["proveedor"] = oAlbPro->proveedor;
        _data["cif_proveedor"] = oAlbPro->cif_proveedor;
        _data["total1"] = Configuracion_global->MonedatoDouble(ui->txttotal1->text());
        _data["total2"] = Configuracion_global->MonedatoDouble(ui->txttotal2->text());
        _data["total3"] = Configuracion_global->MonedatoDouble(ui->txttotal3->text());
        _data["total4"] = Configuracion_global->MonedatoDouble(ui->txttotal4->text());
        _data["porc_iva1"] = ui->txtporc_iva1->text().toDouble();
        _data["porc_iva2"] = ui->txtporc_iva2->text().toDouble();
        _data["porc_iva3"] = ui->txtporc_iva3->text().toDouble();
        _data["porc_iva4"] = ui->txtporc_iva4->text().toDouble();
        _data["iva1"] = Configuracion_global->MonedatoDouble(ui->txtiva1->text());
        _data["iva2"] = Configuracion_global->MonedatoDouble(ui->txtiva2->text());
        _data["iva3"] = Configuracion_global->MonedatoDouble(ui->txtiva3->text());
        _data["iva4"] = Configuracion_global->MonedatoDouble(ui->txtiva4->text());
        _data["base1"] = Configuracion_global->MonedatoDouble(ui->txtbase1->text());
        _data["base2"] = Configuracion_global->MonedatoDouble(ui->txtbase2->text());
        _data["base3"] = Configuracion_global->MonedatoDouble(ui->txtbase3->text());
        _data["base4"] = Configuracion_global->MonedatoDouble(ui->txtbase4->text());
        _data["base_total"] = Configuracion_global->MonedatoDouble(ui->txtbase_total->text());
        _data["iva_total"] = Configuracion_global->MonedatoDouble(ui->txttotal_iva->text());
        _data["total"] = Configuracion_global->MonedatoDouble(ui->txttotal->text());
        _data["comentario"] = ui->txtcomentario->toPlainText();
        _data["recargo_equivalencia"] = ui->chklporc_rec->isChecked();
        _data["porc_rec1"] = ui->txtporc_rec1->text().toDouble();
        _data["porc_rec2"] = ui->txtporc_rec2->text().toDouble();
        _data["porc_rec3"] = ui->txtporc_rec3->text().toDouble();
        _data["porc_rec4"] = ui->txtporc_rec4->text().toDouble();
        _data["rec1"] = Configuracion_global->MonedatoDouble(ui->txtrec1->text());
        _data["rec2"] = Configuracion_global->MonedatoDouble(ui->txtrec2->text());
        _data["rec3"] = Configuracion_global->MonedatoDouble(ui->txtrec3->text());
        _data["rec4"] = Configuracion_global->MonedatoDouble(ui->txtrec4->text());
        _data["total_recargo"] = Configuracion_global->MonedatoDouble(ui->txttotal_recargo->text());
        _data["ejercicio"] = dateedit.date().year();
        _data["subtotal"] = Configuracion_global->MonedatoDouble(ui->txtsubtotal->text());
        _data["direccion1"] = oAlbPro->direccion1;
        _data["direccion2"] = oAlbPro->direccion2;
        _data["cp"] = oAlbPro->cp;
        _data["poblacion"] = oAlbPro->poblacion;
        _data["provincia"] = oAlbPro->provincia;
        _data["id_pais"] = oAlbPro->id_pais;
        _data["porc_dto"] = ui->spinPorc_dto_pp->value();
        _data["dto"] = Configuracion_global->MonedatoDouble(ui->txtimporte_descuento->text());
        _data["telefono"] = oAlbPro->telefono;
        _data["fax"] = oAlbPro->fax;
        _data["movil"] = oAlbPro->movil;
        _data["codigo_proveedor"] = oAlbPro->codigo_proveedor;
        _data["id_forma_pago"] = oAlbPro->id_forma_pago;
        _data["impreso"] = false;
        _data["desc_gasto1"] = ui->txtGastoDist1->text();
        _data["desc_gasto2"] = ui->txtGastoDist2->text();
        _data["desc_gasto3"] = ui->txtGastoDist3->text();
        _data["imp_gasto1"] = ui->SpinGastoDist1->value();
        _data["imp_gasto2"] = ui->SpinGastoDist2->value();
        _data["imp_gasto3"] = ui->SpinGastoDist3->value();
        _data["porc_iva_gasto1"] = ui->cboporc_iva_gasto1->currentText().toDouble();
        _data["porc_iva_gasto2"] = ui->cboporc_iva_gasto2->currentText().toDouble();
        _data["porc_iva_gasto3"] = ui->cboporc_iva_gasto3->currentText().toDouble();
        _data["iva_gasto1"] = ui->spiniva_gasto1->value();
        _data["iva_gasto2"] = ui->spiniva_gasto2->value();
        _data["iva_gasto3"] = ui->spiniva_gasto3->value();
        _data["editable"] = false;
        _data["albaran"] = oAlbPro->albaran;

        Configuracion_global->transaction();
        QString error;
        int id = SqlCalls::SqlInsert(_data,"fac_pro",Configuracion_global->empresaDB,error);
        bool success = id != -1;
        if(!success)
        {
            QMessageBox::critical(this,tr("Error al insertar Factura"),error);
            Configuracion_global->rollback();
            return;
        }

        QString tabla = "lin_fac_pro";
        for(auto i = 0; i<modelLineas->rowCount(); ++i)
        {
            QSqlRecord r = modelLineas->record(i);
            QHash<QString,QVariant> _data;

            _data["id_cab"] = id;
            _data["id_articulo"] = r.value("id_articulo");
            _data["codigo"] = r.value("codigo");
            _data["descripcion"] = r.value("descripcion");
            _data["precio"] = r.value("precio");
            double porc_iva = r.value("porc_iva").toDouble();
            _data["porc_iva"] = porc_iva;

            _data["cantidad"] = r.value("cantidad");
            _data["subtotal"] = r.value("subtotal");

            _data["total"] = r.value("total");

            _data["porc_dto"] = r.value("porc_dto");
            _data["dto"] = _data.value("subtotal").toDouble() * (_data.value("porc_dto").toDouble() / 100);

            _data["iva"] = _data.value("total").toDouble() * (_data.value("porc_iva").toDouble() / 100);

            if(ui->chklporc_rec->isChecked())
            {
                double porc_re = 0;
                for(auto a = 0; a< Configuracion_global->iva_model->rowCount(); ++a)
                {
                    if(Configuracion_global->iva_model->record(a).value("iva").toDouble() == porc_iva)
                    {
                        porc_re = Configuracion_global->iva_model->record(a).value("recargo_equivalencia").toDouble();
                        break;
                    }
                }
                _data["porc_rec"] = porc_re;
                _data["rec"] = _data.value("total").toDouble() * (_data.value("porc_rec").toDouble() / 100);
            }

            //  _data["coste_real_unidad"] =

            _data["albaran"] = oAlbPro->albaran;

            if(SqlCalls::SqlInsert(_data,tabla,Configuracion_global->empresaDB,error) == -1)
            {
                QMessageBox::critical(this,tr("Error al insertar lineas de Factura"),error);
                success = false;
                break;
            }
        }

        QDialog d2(this);

        QLabel label(tr("Fecha de vencimiento"),&d2);
        QDateEdit edit(dateedit.date(),&d2);
        QPushButton aceptar(tr("Aceptar"),&d2);
        QPushButton cancelar(tr("Cancelar"),&d2);
        QGridLayout lay(&d2);

        lay.addWidget(&label,0,0,1,1);
        lay.addWidget(&edit,0,1,1,1);
        lay.addWidget(&aceptar,1,0,1,1);
        lay.addWidget(&cancelar,1,1,1,1);

        connect(&aceptar,SIGNAL(clicked()),&d2,SLOT(accept()));
        connect(&cancelar,SIGNAL(clicked()),&d2,SLOT(reject()));

        double total_factura = oAlbPro->total;
        double acum = total_factura;
        do
        {
            if(d2.exec() == QDialog::Accepted)
            {
                bool ok;
                double cantidad = QInputDialog::getDouble(this, tr("Valor del vencimiento"),
                                                          tr("Cantidad:"), 0, 0, acum, 2, &ok);

                cantidad = Configuracion_global->MonedatoDouble(Configuracion_global->toRound(cantidad,2));
                if (ok)
                {
                    acum = acum - cantidad;
                    SqlData data;
                    data["id_empresa"] = Configuracion_global->idEmpresa;
                    data["id_documento"] = id;
                    data["documento"] = doc;
                    data["fecha_deuda"] = dateedit.date();
                    data["vencimiento"] = edit.date();
                    data["importe_deuda"] = cantidad;
                    data["pendiente"] = cantidad;
                    data["pagado"] = 0.0;
                    QString error;
                    if(!SqlCalls::SqlInsert(data,"deudas_proveedores",Configuracion_global->groupDB,error))
                    {
                        QMessageBox::critical(this,tr("Error al insertar vencimiento"),error);
                        Configuracion_global->rollback();
                        return;
                    }
                }
                else
                {
                    Configuracion_global->rollback();
                    return;
                }
            }
            else
            {
                Configuracion_global->rollback();
                return;
            }
        }while((int)acum > 0);


        oAlbPro->factura = doc;
        success &= oAlbPro->guardar();

        if(success)
        {
            Configuracion_global->commit();
            llenar_campos();
            TimedMessageBox::Box(parentWidget(),tr("Factura creada con éxito"));
        }
        else
        {
            Configuracion_global->rollback();
        }
    }
}
