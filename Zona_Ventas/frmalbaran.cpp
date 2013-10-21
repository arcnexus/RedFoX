#include "frmalbaran.h"
#include "ui_frmalbaran.h"
#include "albaran.h"
#include "../Zona_Pacientes/cliente.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Auxiliares/datedelegate.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Zona_Ventas/factura.h"
#include "../vencimientos.h"
//
#include "../Almacen/articulo.h"
#include<math.h>

Albaran *oAlbaran = new Albaran();
Cliente *oCliente2 = new Cliente();

FrmAlbaran::FrmAlbaran(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmAlbaran),
    helper(this),
    menuButton(QIcon(":/Icons/PNG/albaran.png"),tr("Albaranes"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/albaran.png"),"",this))
{
    ui->setupUi(this);

    // Pongo valores por defecto
    ui->lbfacturado->setVisible(false);
    ui->lbimpreso->setVisible(false);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de albaranes a clientes"));

    ui->comboPais->setModel(Configuracion_global->paises_model);
  //  ui->comboPais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));
    ui->cboPais_entrega->setModel(Configuracion_global->paises_model);
    //ui->cboPais_entrega->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    QSqlQueryModel *formas_pago = new QSqlQueryModel(this);
    formas_pago->setQuery("select forma_pago from formpago",Configuracion_global->groupDB);
    ui->cbo_forma_pago->setModel(formas_pago);


    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));
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

    connect(&helper,SIGNAL(lineaReady(lineaDetalle*)),this,SLOT(lineaReady(lineaDetalle*)));

    connect(&helper,SIGNAL(lineaDeleted(lineaDetalle*)),this,SLOT(lineaDeleted(lineaDetalle*)));

    connect(ui->chklporc_rec,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

    VaciarCampos();
    ui->btn_borrar->setEnabled(false);
    ui->btnEditar->setEnabled(false);
    ui->btnFacturar->setEnabled(false);
    ui->btnSiguiente->setEnabled(false);
    ui->btnAnterior->setEnabled(false);
    ui->btnImprimir->setEnabled(false);
    ui->btnBuscar->setEnabled(true);
    oAlbaran->id = -1;
    ui->btnSiguiente->setEnabled(true);

    //--------------
    // llenar tabla
    //--------------
    m = new QSqlQueryModel(this);
    m->setQuery("select id,serie,albaran,fecha,cif,total_albaran,cliente from cab_alb order by albaran desc",Configuracion_global->empresaDB);
   // ui->tabla->setModel(m);
    ui->table2->setModel(m);
    formato_tabla();
    connect(ui->table2->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(on_table2_row_moved(QModelIndex,QModelIndex)));


    //--------------------
    // llenar cboBusqueda
    //--------------------

    ui->radBusqueda->setChecked(true);
    ui->stackedWidget->setCurrentIndex(1);
    //---------------------
    // Llenar combos ivas
    //---------------------
    QSqlQueryModel *iva = new QSqlQueryModel(this);
    iva->setQuery("select iva from tiposiva order by iva desc",Configuracion_global->groupDB);
    ui->cboporc_iva_gasto1->setModel(iva);
    ui->cboporc_iva_gasto2->setModel(iva);
    ui->cboporc_iva_gasto3->setModel(iva);

    //-------------
    // Series
    //-------------
    cboseries = new QComboBox(this);
    series = new QSqlQueryModel(this);
    series->setQuery("select serie from series", Configuracion_global->empresaDB);
    ui->cboSerie->setModel(series);
    cboseries->setModel(series);



    ui->frameRad->setVisible(false);

    //-----------------
    // EVENTS
    //-----------------
    this->installEventFilter(this);
//    ui->txtcodigo_cliente->installEventFilter(this);
//    ui->txtcp->installEventFilter(this);
//    ui->txtCp_entrega->installEventFilter(this);
//    ui->table2->installEventFilter(this);

    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;

    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);


    setUpBusqueda();
    BloquearCampos(true);
    ui->table2->selectRow(0);
}

FrmAlbaran::~FrmAlbaran()
{
    delete ui;
}





void FrmAlbaran::LLenarCampos() {
    int index = ui->cboSerie->findText(oAlbaran->serie);
    ui->cboSerie->setCurrentIndex(index);
    ui->lblCliente->setText(oAlbaran->cliente);
    ui->lblAlbaran->setText(QString::number(oAlbaran->albaran));
    int lEstado;
    ui->txtcodigo_cliente->setText(oAlbaran->codigo_cliente);
    ui->txtalbaran->setText(QString::number(oAlbaran->albaran));
    ui->txtfecha->setDate(oAlbaran->fecha);
    ui->txtfecha_factura->setDate(oAlbaran->fecha_factura);
    ui->txtcliente->setText(oAlbaran->cliente);
    ui->txtdireccion1->setText(oAlbaran->direccion1);
    ui->txtdireccion2->setText(oAlbaran->direccion2);
    ui->txtcp->setText(oAlbaran->cp);
    ui->txtpoblacion->setText(oAlbaran->poblacion);
    ui->txtprovincia->setText(oAlbaran->provincia);
    index = ui->comboPais->findText(Configuracion_global->Devolver_pais(oAlbaran->id_pais));
    ui->comboPais->setCurrentIndex(index);
    ui->txtDireccion1_entrega->setText(oAlbaran->direccion1_entrega);
    ui->txtDireccion2_entrega->setText(oAlbaran->direccion2_entrega);
    ui->txtCp_entrega->setText(oAlbaran->cp_entrega);
    ui->txtPoblacion_entrega->setText(oAlbaran->poblacion_entrega);
    ui->txtProvincia_entrega->setText(oAlbaran->provincia_entrega);

    index = ui->cboPais_entrega->findText(Configuracion_global->Devolver_pais(oAlbaran->id_pais_entrega));
    ui->cboPais_entrega->setCurrentIndex(index);
    ui->txtemail_alternativa->setText(oAlbaran->email_entrega);
    ui->txtcomentarios_alternativa->setPlainText(oAlbaran->comentarios_entrega);


    ui->txtcif->setText(oAlbaran->cif);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda( QString::number(oAlbaran->subtotal,'f',Configuracion_global->decimales)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->dto,'f',Configuracion_global->decimales)));
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number( oAlbaran->base_total,'f',Configuracion_global->decimales)));
    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number( oAlbaran->base_total,'f',Configuracion_global->decimales)));

    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva_total,'f',Configuracion_global->decimales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total_albaran,'f',Configuracion_global->decimales)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total_albaran,'f',Configuracion_global->decimales)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec_total,'f',Configuracion_global->decimales)));
    lEstado = oAlbaran->impreso;
    if((lEstado == true)) {
        ui->lbimpreso->setVisible(true);
    } else {
        ui->lbimpreso->setVisible(false);
    }
    lblimpreso->setVisible(lEstado);
    lEstado = oAlbaran->facturado;
    if ((lEstado == true)) {
        ui->lbfacturado->setVisible(true);
    } else {
        ui->lbfacturado->setVisible(false);

    }
    //ui->lblfacturado->setVisible(lEstado);
    ui->txtcomentario->setText(oAlbaran->comentario);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->base1,'f',Configuracion_global->decimales)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->base2,'f',Configuracion_global->decimales)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->base3,'f',Configuracion_global->decimales)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->base4,'f',Configuracion_global->decimales)));
    ui->txtporc_iva1->setText(QString::number(oAlbaran->porc_iva1));
    ui->txtporc_iva2->setText(QString::number(oAlbaran->porc_iva2));
    ui->txtporc_iva3->setText(QString::number(oAlbaran->porc_iva3));
    ui->txtporc_iva4->setText(QString::number(oAlbaran->porc_iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva1,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva2,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva3,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva4,'f',Configuracion_global->decimales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total1,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total2,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total3,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total4,'f',Configuracion_global->decimales)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->porc_rec1,'f',Configuracion_global->decimales)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->porc_rec2,'f',Configuracion_global->decimales)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->porc_rec3,'f',Configuracion_global->decimales)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->porc_rec4,'f',Configuracion_global->decimales)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec1,'f',Configuracion_global->decimales)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec2,'f',Configuracion_global->decimales)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec3,'f',Configuracion_global->decimales)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec4,'f',Configuracion_global->decimales)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec_total,'f',Configuracion_global->decimales)));
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva_total,'f',Configuracion_global->decimales)));
    ui->txtpedido_cliente->setText(oAlbaran->pedido_cliente);
    ui->txtcNumFra->setText(oAlbaran->factura);
    ui->txtGastoDist1->setText(oAlbaran->desc_gasto1);
    ui->txtGastoDist2->setText(oAlbaran->desc_gasto2);
    ui->txtGastoDist3->setText(oAlbaran->desc_gasto3);
    ui->SpinGastoDist1->setValue(oAlbaran->imp_gasto1);
    ui->SpinGastoDist2->setValue(oAlbaran->imp_gasto2);
    ui->SpinGastoDist3->setValue(oAlbaran->imp_gasto3);
    index = ui->cboporc_iva_gasto1->findText(QString::number(oAlbaran->porc_iva_gasto1));
    ui->cboporc_iva_gasto1->setCurrentIndex(index);
    index = ui->cboporc_iva_gasto2->findText(QString::number(oAlbaran->porc_iva_gasto2));
        ui->cboporc_iva_gasto2->setCurrentIndex(index);
    index = ui->cboporc_iva_gasto3->findText(QString::number(oAlbaran->porc_iva_gasto3));
        ui->cboporc_iva_gasto3->setCurrentIndex(index);
    ui->spiniva_gasto1->setValue(oAlbaran->iva_gasto1);
    ui->spiniva_gasto2->setValue(oAlbaran->iva_gasto2);
    ui->spiniva_gasto3->setValue(oAlbaran->iva_gasto3);

        if(oAlbaran->recargo_equivalencia==true)
        ui->chklporc_rec->setChecked(true);
    else
        ui->chklporc_rec->setChecked(false);

    ui->txtentregado_a_cuenta->setText(QString::number(oAlbaran->entregado_a_cuenta));
    ui->spinporc_Dto->setValue(oAlbaran->porc_dto);
    ui->spinPorc_dto_pp->setValue(oAlbaran->porc_dto_pp);

    oCliente2->Recuperar("Select * from clientes where id ="+QString::number(oAlbaran->id_cliente));
    helper.set_tarifa(oCliente2->tarifa_cliente);
    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();

    QString filter = QString("id_Cab = '%1'").arg(oAlbaran->id);
    helper.fillTable("empresa","lin_alb",filter);
    helper.setId_cliente(oCliente2->id);
    helper.set_tipo_dto_tarifa(oCliente2->tipo_dto_tarifa);
    //-------------------
    // combo forma pago
    //-------------------
    QString error;
    QString fp = SqlCalls::SelectOneField("formpago","forma_pago",QString("id=%1").arg(oCliente2->id_forma_pago),
                                          Configuracion_global->groupDB,error).toString();
    if(error.isEmpty())
    {
        int index = ui->cbo_forma_pago->findText(fp);
        ui->cbo_forma_pago->setCurrentIndex(index);
    }
    //----------------------------
    // Barra Busquedas
    //----------------------------
    lblfacturado->setVisible(oAlbaran->facturado);
    lblimpreso->setVisible(oAlbaran->impreso);

}
void FrmAlbaran::LLenarCamposCliente()
{
    ui->lblCliente->setText(oCliente2->nombre_fiscal);
    ui->txtcodigo_cliente->setText(oCliente2->codigo_cliente);
    ui->txtcliente->setText(oCliente2->nombre_fiscal);
    //---------------------------------
    // dirección
    //---------------------------------
    ui->txtdireccion1->setText(oCliente2->direccion1);
    ui->txtdireccion2->setText(oCliente2->direccion2);
    ui->txtcp->setText(oCliente2->cp);
    ui->txtpoblacion->setText(oCliente2->poblacion);
    ui->txtprovincia->setText(oCliente2->provincia);
    int index = ui->comboPais->findText(oAlbaran->pais);
    ui->comboPais->setCurrentIndex(index);
    //---------------------------------
    // Forma de Pago
    //---------------------------------
    QString error;
    QString clausulas = QString("id =%1").arg(oCliente2->id_forma_pago);
    QString fp = SqlCalls::SelectOneField("formpago","forma_pago",clausulas,
                                          Configuracion_global->groupDB,error).toString();
    if(error.isEmpty())
    {
        int index = ui->cbo_forma_pago->findText(fp);
        ui->cbo_forma_pago->setCurrentIndex(index);
    }
    //---------------------------------
    // Recargo de equivalencia
    //---------------------------------
    if(oCliente2->recargo_equivalencia == true)
    {
        ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
        ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
        ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
        ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));

    } else
    {
        ui->txtporc_rec1->setText("0,00");
        ui->txtporc_rec2->setText("0,00");
        ui->txtporc_rec3->setText("0,00");
        ui->txtporc_rec4->setText("0,00");
    }


    //---------------------------------
    // Comprobar direccion alternativa
    //---------------------------------
    QMap <int,QSqlRecord> rec;
    QStringList condiciones;
    condiciones << QString("id_cliente = %1").arg(oCliente2->id) << "direccion_envio = 1";
    rec = SqlCalls::SelectRecord("cliente_direcciones",condiciones,Configuracion_global->groupDB,error);

    QMapIterator <int,QSqlRecord> i(rec);
    while(i.hasNext())
    {
        i.next();
        ui->txtDireccion1_entrega->setText(i.value().value("direccion1").toString());
        ui->txtDireccion2_entrega->setText(i.value().value("direccion2").toString());
        ui->txtCp_entrega->setText(i.value().value("cp").toString());
        ui->txtPoblacion_entrega->setText(i.value().value("poblacion").toString());
        ui->txtProvincia_entrega->setText(i.value().value("provincia").toString());
        int index = ui->cboPais_entrega->findText(Configuracion_global->Devolver_pais(i.value().value("pais").toInt()));
        ui->cboPais_entrega->setCurrentIndex(index);
        ui->txtemail_alternativa->setText(i.value().value("email").toString());
        ui->txtcomentarios_alternativa->setPlainText(i.value().value("comentarios").toString());

    }
    //----------------------
    // Datos fiscales
    //----------------------
    ui->txtcif->setText(oCliente2->cif_nif);
    helper.set_tarifa(oCliente2->tarifa_cliente);
    if (oCliente2->lIRPF==1) {
        ui->chklporc_rec->setChecked(true);
        oAlbaran->recargo_equivalencia = (1);
    } else {
        ui->chklporc_rec->setChecked(false);
        oAlbaran->recargo_equivalencia = (0);
    }

    oCliente2->Recuperar("Select * from clientes where id ="+QString::number(oAlbaran->id_cliente));
    helper.set_tarifa(oCliente2->tarifa_cliente);

    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();
    oAlbaran->id_cliente = oCliente2->id;
    helper.setId_cliente(oCliente2->id);
    helper.set_tipo_dto_tarifa(oCliente2->tipo_dto_tarifa);

}

void FrmAlbaran::VaciarCampos() {
    ui->lblCliente->setText("");
    ui->lblAlbaran->setText("");
    QDate fecha;
    ui->txtcodigo_cliente->setText("");
    ui->txtalbaran->setText("");
    ui->txtfecha->setDate(fecha.currentDate());
    ui->txtfecha_factura->setDate(fecha.currentDate());
    ui->txtcliente->setText("");
    ui->txtdireccion1->setText("");
    ui->txtdireccion2->setText("");
    ui->txtcp->setText("");
    ui->txtpoblacion->setText("");
    ui->txtprovincia->setText("");
    ui->comboPais->setCurrentIndex(0);
    ui->txtcif->setText("");
    ui->txtsubtotal->setText(0);
    ui->txtimporte_descuento->setText("0,00");
    ui->txtbase->setText("0,00");
    ui->txtiva->setText("0,00");
    ui->txttotal->setText("0,00");
    ui->lbimpreso->setVisible(false);
    ui->lbfacturado->setVisible(false);

    ui->txtcomentario->setText("");
    ui->txtbase1->setText(0);
    ui->txtbase2->setText(0);
    ui->txtbase3->setText(0);
    ui->txtbase4->setText(0);
    //Configuracion_global->CargarDatos();

    //QList<QString> keys = Configuracion_global->ivas.uniqueKeys();
    //ui->txtporc_iva1->setText(QString::number(Configuracion_global->ivas[keys.at(0)].value("iva").toDouble()));
    //ui->txtporc_iva2->setText(QString::number(Configuracion_global->ivas[keys.at(1)].value("iva").toDouble()));
    //ui->txtporc_iva3->setText(QString::number(Configuracion_global->ivas[keys.at(2)].value("iva").toDouble()));
    //ui->txtporc_iva4->setText(QString::number(Configuracion_global->ivas[keys.at(3)].value("iva").toDouble()));
    //ui->txtporc_rec1->clear();
    //ui->txtporc_rec2->clear();
    //ui->txtporc_rec3->clear();
    //ui->txtporc_rec4->clear();
    ui->txtiva1->setText(0);
    ui->txtiva2->setText(0);
    ui->txtiva3->setText(0);
    ui->txtiva4->setText(0);
    ui->txttotal1->setText(0);
    ui->txttotal2->setText(0);
    ui->txttotal3->setText(0);
    ui->txttotal4->setText(0);
    ui->txtporc_rec1->setText(0);
    ui->txtporc_rec2->setText(0);
    ui->txtporc_rec3->setText(0);
    ui->txtporc_rec4->setText(0);
    ui->txttotal_recargo->setText(0);
    ui->txtentregado_a_cuenta->setText("0,00");
    ui->txtpedido_cliente->setText("");
    ui->txttotal_iva_2->setText("0,00");
    ui->txtbase_total_2->setText("0,00");
    ui->txttotal_recargo_2->setText("0,00");
    ui->txttotal_2->setText("0,00");
    ui->txtsubtotal->setText("0,00");

    helper.fillTable("empresa","lin_alb","id_Cab = -1");
}

void FrmAlbaran::BloquearCampos(bool state)
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

    // Qdoblespinbox
    QList<QDoubleSpinBox *> SpinEditList = this->findChildren<QDoubleSpinBox *>();
    QDoubleSpinBox *SpinEdit;
    foreach (SpinEdit, SpinEditList) {
        SpinEdit->setEnabled(!state);
    }

    ui->btnAnadir->setEnabled(state);
    ui->btnAnterior->setEnabled(state);
    ui->btnBuscar->setEnabled(state);
    ui->btnEditar->setEnabled(state);
    ui->btnGuardar->setEnabled(!state);
    ui->btndeshacer->setEnabled(!state);
    ui->btnSiguiente->setEnabled(state);
    ui->botBuscarCliente->setEnabled(!state);
    ui->btnFacturar->setEnabled(!state);
    ui->txtalbaran->setReadOnly(true);
    ui->btnAnadirLinea->setEnabled(!state);
    ui->btn_borrarLinea->setEnabled(!state);
    ui->btn_borrar->setEnabled(state);    
    ui->btnFacturar->setEnabled(state);

    helper.blockTable(state);
    m_busqueda->block(!state);
}

void FrmAlbaran::LLenarAlbaran()
{
    oAlbaran->codigo_cliente= (ui->txtcodigo_cliente->text());
    oAlbaran->serie = (ui->cboSerie->currentText());
    oAlbaran->albaran= (ui->txtalbaran->text().toInt());
    oAlbaran->fecha= (ui->txtfecha->date());
    oAlbaran->fecha_factura= (ui->txtfecha_factura->date());
    oAlbaran->cliente= (ui->txtcliente->text());
    oAlbaran->direccion1= (ui->txtdireccion1->text());
    oAlbaran->direccion2= (ui->txtdireccion2->text());
    oAlbaran->cp= (ui->txtcp->text());
    oAlbaran->poblacion= (ui->txtpoblacion->text());
    oAlbaran->provincia= (ui->txtprovincia->text());
    oAlbaran->pais= (ui->comboPais->currentText());
    oAlbaran->id_pais = Configuracion_global->paises[ui->comboPais->currentText()].value("id").toInt();
    oAlbaran->direccion1_entrega= (ui->txtDireccion1_entrega->text());
    oAlbaran->direccion2_entrega= (ui->txtDireccion2_entrega->text());
    oAlbaran->cp_entrega = (ui->txtCp_entrega->text());
    oAlbaran->poblacion_entrega= (ui->txtPoblacion_entrega->text());
    oAlbaran->provincia_entrega= (ui->txtProvincia_entrega->text());
    oAlbaran->id_pais_entrega = Configuracion_global->paises[ui->cboPais_entrega->currentText()].value("id").toInt();
    oAlbaran->email_entrega = ui->txtemail_alternativa->text();
    oAlbaran->comentarios_entrega = ui->txtcomentarios_alternativa->toPlainText();
    oAlbaran->cif= (ui->txtcif->text());
    oAlbaran->subtotal= (Configuracion_global->MonedatoDouble(ui->txtsubtotal->text()));
    oAlbaran->dto= (Configuracion_global->MonedatoDouble(ui->txtimporte_descuento->text()));
    oAlbaran->base_total= (Configuracion_global->MonedatoDouble(ui->txtbase->text()));
    oAlbaran->iva_total= (Configuracion_global->MonedatoDouble(ui->txtiva->text()));
    oAlbaran->total_albaran= (Configuracion_global->MonedatoDouble(ui->txttotal->text()));
    oAlbaran->comentario= (ui->txtcomentario->toPlainText());
    oAlbaran->base1= Configuracion_global->MonedatoDouble(ui->txtbase1->text());
    oAlbaran->base2= (Configuracion_global->MonedatoDouble(ui->txtbase2->text()));
    oAlbaran->base3= (Configuracion_global->MonedatoDouble(ui->txtbase3->text()));
    oAlbaran->base4= (Configuracion_global->MonedatoDouble(ui->txtbase4->text()));
    oAlbaran->porc_iva1= (Configuracion_global->MonedatoDouble(ui->txtporc_iva1->text()));
    oAlbaran->porc_iva2= (Configuracion_global->MonedatoDouble(ui->txtporc_iva2->text()));
    oAlbaran->porc_iva3= (Configuracion_global->MonedatoDouble(ui->txtporc_iva3->text()));
    oAlbaran->porc_iva4= (Configuracion_global->MonedatoDouble(ui->txtporc_iva4->text()));
    oAlbaran->iva1= (Configuracion_global->MonedatoDouble(ui->txtiva1->text()));
    oAlbaran->iva2= (Configuracion_global->MonedatoDouble(ui->txtiva2->text()));
    oAlbaran->iva3= (Configuracion_global->MonedatoDouble(ui->txtiva3->text()));
    oAlbaran->iva4= (Configuracion_global->MonedatoDouble(ui->txtiva4->text()));
    oAlbaran->total1= (Configuracion_global->MonedatoDouble(ui->txttotal1->text()));
    oAlbaran->total2= (Configuracion_global->MonedatoDouble(ui->txttotal2->text()));
    oAlbaran->total3= (Configuracion_global->MonedatoDouble(ui->txttotal3->text()));
    oAlbaran->total4= (Configuracion_global->MonedatoDouble(ui->txttotal4->text()));
    oAlbaran->porc_rec1= (Configuracion_global->MonedatoDouble(ui->txtporc_rec1->text()));
    oAlbaran->porc_rec2= (Configuracion_global->MonedatoDouble(ui->txtporc_rec2->text()));
    oAlbaran->porc_rec3= (Configuracion_global->MonedatoDouble(ui->txtporc_rec3->text()));
    oAlbaran->porc_rec4= (Configuracion_global->MonedatoDouble(ui->txtporc_rec4->text()));
    oAlbaran->rec1= (Configuracion_global->MonedatoDouble(ui->txtporc_rec1->text()));
    oAlbaran->rec2= (Configuracion_global->MonedatoDouble(ui->txtporc_rec2->text()));
    oAlbaran->rec3= (Configuracion_global->MonedatoDouble(ui->txtporc_rec3->text()));
    oAlbaran->rec4= (Configuracion_global->MonedatoDouble(ui->txtporc_rec4->text()));
    oAlbaran->rec_total= (Configuracion_global->MonedatoDouble(ui->txttotal_recargo->text()));
    oAlbaran->dto = Configuracion_global->MonedatoDouble(ui->txtimporte_descuento->text());
    if (ui->chklporc_rec->isChecked())
        oAlbaran->recargo_equivalencia = (1);
    else
        oAlbaran->recargo_equivalencia = (0);

    oAlbaran->pedido_cliente= (ui->txtpedido_cliente->text());
    oAlbaran->entregado_a_cuenta = Configuracion_global->MonedatoDouble(ui->txtentregado_a_cuenta->text());
    oAlbaran->desc_gasto1 = ui->txtGastoDist1->text();
    oAlbaran->desc_gasto2 = ui->txtGastoDist2->text();
    oAlbaran->desc_gasto3 = ui->txtGastoDist3->text();
    oAlbaran->imp_gasto1 = ui->SpinGastoDist1->value();
    oAlbaran->imp_gasto2 = ui->SpinGastoDist2->value();
    oAlbaran->imp_gasto3 = ui->SpinGastoDist3->value();
    oAlbaran->porc_iva_gasto1 = ui->cboporc_iva_gasto1->currentText().toDouble();
    oAlbaran->porc_iva_gasto1 = ui->cboporc_iva_gasto1->currentText().toDouble();
    oAlbaran->porc_iva_gasto1 = ui->cboporc_iva_gasto1->currentText().toDouble();
    oAlbaran->iva_gasto1 = ui->spiniva_gasto1->value();
    oAlbaran->iva_gasto2 = ui->spiniva_gasto2->value();
    oAlbaran->iva_gasto3 = ui->spiniva_gasto3->value();
    //---------------
    // forma pago
    //---------------
    QString error;
    oAlbaran->id_forma_pago = SqlCalls::SelectOneField("formpago","id",
                                                       QString("forma_pago ='%1'").arg(ui->cbo_forma_pago->currentText()),
                                                       Configuracion_global->groupDB,error).toInt();

}

void FrmAlbaran::on_btnSiguiente_clicked()
{
    int albaran = oAlbaran->albaran;
    if(oAlbaran->RecuperarAlbaran("Select * from cab_alb where albaran >'"+QString::number(albaran)+"' order by albaran  limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oAlbaran->id);
        helper.fillTable("empresa","lin_alb",filter);
        ui->btn_borrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
        ui->btnFacturar->setEnabled(true);
        ui->btnAnterior->setEnabled(true);
        ui->btnImprimir->setEnabled(true);
        ui->btnBuscar->setEnabled(true);
    }
    else
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Se ha llegado al final del archivo.\n"
                                                          "No hay mas albaranes disponibles"));
        VaciarCampos();
        ui->btn_borrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btnFacturar->setEnabled(false);
        ui->btnSiguiente->setEnabled(false);
        oAlbaran->id++;
    }
}

void FrmAlbaran::on_btnAnterior_clicked()
{
    int albaran = oAlbaran->albaran;
    if(oAlbaran->RecuperarAlbaran("Select * from cab_alb where albaran <'"+QString::number(albaran)+"' order by albaran desc limit 1 "))
    {
        LLenarCampos();
        QString filter = QString("id_Cab = '%1'").arg(oAlbaran->id);
        helper.fillTable("empresa","lin_alb",filter);
        ui->btn_borrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
        ui->btnFacturar->setEnabled(true);
        ui->btnSiguiente->setEnabled(true);
    }
    else
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Se ha llegado al final del archivo.\n"
                                                          "No hay mas presupuestos disponibles"));
        VaciarCampos();
        ui->btn_borrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btnFacturar->setEnabled(false);
        ui->btnAnterior->setEnabled(false);
        oAlbaran->id = -1;
    }
}

void FrmAlbaran::on_btnAnadir_clicked()
{
    in_edit = false;
    ui->radEdicion->setChecked(true);

    VaciarCampos();    
    LLenarAlbaran();
    //-----------------------
    // Elección de serie
    //-----------------------
    QDialog* dlg = new QDialog(this);
    dlg->setWindowTitle(tr("Seleccione serie"));
    dlg->resize(170,150);
    QComboBox* box = new QComboBox(dlg);
    QPushButton*  btn = new QPushButton("Aceptar",dlg);
    QVBoxLayout lay(dlg);

    lay.addWidget(box);
    lay.addWidget(btn);

    dlg->setLayout(&lay);

    QSqlQueryModel *l = new QSqlQueryModel(this);
    l->setQuery("select serie from series",Configuracion_global->empresaDB);
    box->setModel(l);

    connect(btn,SIGNAL(clicked()),dlg,SLOT(accept()));
    dlg->exec();//aki se podria poner otro boton y cancelar todo?

    QString serie = box->currentText();
    dlg->deleteLater();



    int id =oAlbaran->AnadirAlbaran(serie);
    oAlbaran->RecuperarAlbaran(QString("select * from cab_alb where id = %1").arg(id));
    LLenarCampos();
    ui->txtalbaran->setText(QString::number(oAlbaran->albaran));
    BloquearCampos(false);
   // ui->txtcodigo_cliente->setFocus();
    emit block();
}

void FrmAlbaran::on_botBuscarCliente_clicked()
{
    db_consulta_view consulta;
    QStringList campos;
    campos  <<"nombre_fiscal" <<"codigo_cliente" << "cif_nif"<< "poblacion" << "telefono1";
    consulta.set_campoBusqueda(campos);
    consulta.set_texto_tabla("clientes");
    consulta.set_db("Maya");
    consulta.set_SQL("select id,codigo_cliente,nombre_fiscal,cif_nif,poblacion,telefono1 from clientes");
    QStringList cabecera;
    QVariantList tamanos;
    cabecera  << tr("Código") << tr("Nombre") << tr("CIF/NIF") << tr("Población") << tr("Teléfono");
    tamanos <<"0" << "100" << "300" << "100" << "180" <<"130";
    consulta.set_headers(cabecera);
    consulta.set_tamano_columnas(tamanos);
    consulta.set_titulo("Busqueda de Clientes");
    if(consulta.exec())
    {
        int id = consulta.get_id();
        oCliente2->Recuperar("select * from clientes where id="+QString::number(id));
        LLenarCamposCliente();
    }

}



void FrmAlbaran::on_btnEditar_clicked()
{
    in_edit = true;
    ui->radEdicion->setChecked(true);

    if (oAlbaran->editable == true)
    {
            BloquearCampos(false);
            emit block();
    }
    else
    {
        QMessageBox::warning(qApp->activeWindow(),tr("Editar Albarán"),tr("No se puede editar un albarán que ha sido facturado, solo los NO facturados se pueden editar")+
                             tr("<p><b> Si necesita modificar algo genere una factura nueva y realice el abono correspondiente</b>")+
                                tr("y luego si es preciso realice un nuevo albarán y facture de nuevo "),tr("OK"));
    }

}

void FrmAlbaran::on_btnGuardar_clicked()
{        
    Configuracion_global->empresaDB.transaction();
    LLenarAlbaran();
    bool succes = oAlbaran->GuardarAlbaran(oAlbaran->id);
    if(succes)
    {
        LLenarCampos();
        succes = Configuracion_global->empresaDB.commit();
    }
    if(succes)
    {
        TimedMessageBox * t = new TimedMessageBox(this,tr("Albarán guardado con éxito"));
        BloquearCampos(true);
        emit unblock();
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),
                              tr("Error al guardar el albaran.\n")+Configuracion_global->empresaDB.lastError().text(),
                              tr("&Aceptar"));
        Configuracion_global->empresaDB.rollback();
    }
    ui->btnFacturar->setEnabled(true);
}

void FrmAlbaran::on_btn_borrar_clicked()
{
    if (QMessageBox::question(this,tr("Borrar"),
                              tr("Esta acción no se puede deshacer.\n¿Desea continuar?"),
                              tr("Cancelar"),
                              tr("&Continuar"))== QMessageBox::Accepted)
    {
        bool succes = true;        
        Configuracion_global->empresaDB.transaction();
        QSqlQuery q(Configuracion_global->empresaDB);

        //TODO control de stock
        q.prepare("DELETE FROM lin_alb WHERE id_Cab = "+QString::number(oAlbaran->id));
        succes &= q.exec();

        q.prepare("DELETE FROM cab_alb WHERE id = "+QString::number(oAlbaran->id));
        succes &= q.exec();

        if(succes)
            succes = Configuracion_global->empresaDB.commit();

        if(succes)
        {
            TimedMessageBox * t = new TimedMessageBox(this,tr("Albarán borrado con exito"));
            VaciarCampos();
            oAlbaran->id = -1;
            on_btnSiguiente_clicked();
        }
        else
        {
            TimedMessageBox * t = new TimedMessageBox(this,tr("Error al borrar albarán"),TimedMessageBox::Critical);
            Configuracion_global->empresaDB.rollback();
        }
    }
}

void FrmAlbaran::totalChanged(double base, double dto, double subtotal, double iva, double re, double total, QString moneda)
{

    subtotal +=(ui->SpinGastoDist1->value() + ui->SpinGastoDist2->value() + ui->SpinGastoDist3->value());
    if(dto == 0)
        dto = subtotal *(ui->spinporc_Dto->value()/100.0);
    double dtopp = subtotal *(ui->spinPorc_dto_pp->value()/100.0);
    ui->txtDtoPP->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(dtopp,Configuracion_global->decimales_campos_totales)));
    base = subtotal -(dto+dtopp);

    if(oCliente2->grupo_iva == 1)
        calcular_iva_gastos();
    iva += (ui->spiniva_gasto1->value() + ui->spiniva_gasto2->value() + ui->spiniva_gasto3->value());
    total = base + iva + re;
    if(oCliente2->grupo_iva == 2 || oCliente2->grupo_iva == 3 || oCliente2->grupo_iva == 4)
    {
        iva = 0;
        total = base;
    }
    QString sbase;
    sbase = Configuracion_global->toRound(base,Configuracion_global->decimales_campos_totales);
    //qDebug()  <<"BASE: " << sbase;
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(base,Configuracion_global->decimales_campos_totales)));
    base = Configuracion_global->MonedatoDouble(ui->txtbase->text());
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(dto,Configuracion_global->decimales_campos_totales)));
    dto = Configuracion_global->MonedatoDouble(ui->txtimporte_descuento->text());
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(subtotal,Configuracion_global->decimales_campos_totales)));
    subtotal = Configuracion_global->MonedatoDouble(ui->txtsubtotal->text());
    if(oCliente2->grupo_iva == 1)
    {
        ui->txtiva->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(iva,Configuracion_global->decimales_campos_totales)));
        ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(re,Configuracion_global->decimales_campos_totales)));
    }
    else
    {
        ui->txtiva->setText("0,00");
        ui->txttotal_recargo->setText("0,00");
    }
    iva = Configuracion_global->MonedatoDouble(ui->txtiva->text());
    re = Configuracion_global->MonedatoDouble(ui->txttotal_recargo->text());
    total = base + iva + re;
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(total,Configuracion_global->decimales_campos_totales)));

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(base,Configuracion_global->decimales_campos_totales)));
    if(oCliente2->grupo_iva ==1)
    {
        ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(iva,Configuracion_global->decimales_campos_totales)));
        ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(re,Configuracion_global->decimales_campos_totales)));
    } else
    {
        ui->txttotal_iva_2->setText("0,00");
        ui->txttotal_recargo_2->setText("0,00");
    }


        ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(total,Configuracion_global->decimales_campos_totales)));
}

void FrmAlbaran::desglose1Changed(double base, double iva, double re, double total)
{
    if(oCliente2->grupo_iva == 2 || oCliente2->grupo_iva == 3 || oCliente2->grupo_iva == 4)
    {
        iva = 0;
        total = base;
    }
//    if(ui->cboporc_iva_gasto1->currentText() == QString::number(oAlbaran->porc_iva1))
//        base +=ui->SpinGastoDist1->value();
//    if(ui->cboporc_iva_gasto2->currentText() == QString::number(oAlbaran->porc_iva1))
//        base +=ui->SpinGastoDist2->value();
//    if(ui->cboporc_iva_gasto3->currentText() == QString::number(oAlbaran->porc_iva1))
//        base +=ui->SpinGastoDist3->value();

    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(base,Configuracion_global->decimales_campos_totales)));
    base = Configuracion_global->MonedatoDouble(ui->txtbase1->text());
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(iva,Configuracion_global->decimales_campos_totales)));
    iva = Configuracion_global->MonedatoDouble(ui->txtiva1->text());
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(re,Configuracion_global->decimales_campos_totales)));
    re = Configuracion_global->MonedatoDouble(ui->txtrec1->text());
    total = base + iva + re;
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(total,Configuracion_global->decimales_campos_totales)));
}

void FrmAlbaran::desglose2Changed(double base, double iva, double re, double total)
{
    if(oCliente2->grupo_iva == 2 || oCliente2->grupo_iva == 3 || oCliente2->grupo_iva == 4)
    {
        iva = 0;
        total = base;
    }
//    if(ui->cboporc_iva_gasto1->currentText() == QString::number(oAlbaran->porc_iva2))
//        base +=ui->SpinGastoDist1->value();
//    if(ui->cboporc_iva_gasto2->currentText() == QString::number(oAlbaran->porc_iva2))
//        base +=ui->SpinGastoDist2->value();
//    if(ui->cboporc_iva_gasto3->currentText() == QString::number(oAlbaran->porc_iva2))
//        base +=ui->SpinGastoDist3->value();
    total = base + iva + re;
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    base = Configuracion_global->MonedatoDouble(ui->txtbase2->text());
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    iva = Configuracion_global->MonedatoDouble(ui->txtiva1->text());
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    re = Configuracion_global->MonedatoDouble(ui->txtrec1->text());
    total = base + iva + re;
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales_campos_totales)));
}

void FrmAlbaran::desglose3Changed(double base, double iva, double re, double total)
{
    if(oCliente2->grupo_iva == 2 || oCliente2->grupo_iva == 3 || oCliente2->grupo_iva == 4)
    {
        iva = 0;
        total = base;
    }
//    if(ui->cboporc_iva_gasto1->currentText() == QString::number(oAlbaran->porc_iva3))
//        base +=ui->SpinGastoDist1->value();
//    if(ui->cboporc_iva_gasto2->currentText() == QString::number(oAlbaran->porc_iva3))
//        base +=ui->SpinGastoDist2->value();
//    if(ui->cboporc_iva_gasto3->currentText() == QString::number(oAlbaran->porc_iva3))
//        base +=ui->SpinGastoDist3->value();
    total = base + iva + re;
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmAlbaran::desglose4Changed(double base, double iva, double re, double total)
{
    if(oCliente2->grupo_iva == 2 || oCliente2->grupo_iva == 3 || oCliente2->grupo_iva == 4)
    {
        iva = 0;
        total = base;
    }
//    if(ui->cboporc_iva_gasto1->currentText() == QString::number(oAlbaran->porc_iva4))
//        base +=ui->SpinGastoDist1->value();
//    if(ui->cboporc_iva_gasto2->currentText() == QString::number(oAlbaran->porc_iva4))
//        base +=ui->SpinGastoDist2->value();
//    if(ui->cboporc_iva_gasto3->currentText() == QString::number(oAlbaran->porc_iva4))
//        base +=ui->SpinGastoDist3->value();
    total = base + iva + re;
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmAlbaran::lineaReady(lineaDetalle * ld)
{
    //-----------------------------------------------------
    // Insertamos línea de pedido y controlamos acumulados
    //-----------------------------------------------------

    bool ok_empresa,ok_Maya;
    ok_empresa = true;
    ok_Maya = true;
    Configuracion_global->empresaDB.transaction();
    Configuracion_global->groupDB.transaction();

    //Nueva linea
    if (ld->idLinea == -1)
    {
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;
        QSqlQuery query_lin_alb(Configuracion_global->empresaDB);
        query_lin_alb.prepare("INSERT INTO lin_alb (id_Cab, id_articulo, codigo, cantidad,"
                                  "descripcion, precio, subtotal, porc_dto,"
                                  " dto, porc_iva, total)"
                                  "VALUES (:id_cab,:id_articulo,:codigo,:cantidad,"
                                  ":descripcion,:precio,:subtotal,:porc_dto,"
                                  ":dto,:porc_iva,:total);");
        query_lin_alb.bindValue(":id_cab", oAlbaran->id);
        query_lin_alb.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_alb.bindValue(":codigo",ld->codigo);
        query_lin_alb.bindValue(":descripcion",ld->descripcion);
        query_lin_alb.bindValue(":cantidad",ld->cantidad);
        query_lin_alb.bindValue(":precio",ld->precio);
        query_lin_alb.bindValue(":subtotal",ld->subtotal);
        query_lin_alb.bindValue(":porc_dto",ld->dto_perc);
        query_lin_alb.bindValue(":dto",ld->dto);
        query_lin_alb.bindValue(":iva",(ld->subtotal *ld->iva_perc)/100); // importe iva hay que calcularlo
        query_lin_alb.bindValue(":total",ld->total);
        if (!query_lin_alb.exec()){
            ok_empresa = false;
            QMessageBox::warning(this,tr("Gestión de albaranes"),
                                 tr("Ocurrió un error al insertar la nueva línea: %1").arg(query_lin_alb.lastError().text()),
                                 tr("Aceptar"));
        }
        //TODO control de stock fact clientes

        if(queryArticulos.exec() && ok_empresa){
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        } else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
        }

        ld->idLinea = query_lin_alb.lastInsertId().toInt();

    } else // Editando linea
    {
        //TODO control de stock editando en fact clientes
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery query_lin_alb(Configuracion_global->empresaDB);
        query_lin_alb.prepare("UPDATE lin_alb SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo =:codigo,"
                                  "descripcion =:descripcion,"
                                  "cantidad =:cantidad,"
                                  "precio =:precio,"
                                  "subtotal =:subtotal,"
                                  "porc_dto =:porc_dto,"
                                  "dto =:dto,"
                                  "iva =:iva,"
                                  "porc_iva =:porc_iva,"
                                  "total =:total "
                                  "WHERE id = :id;");

        query_lin_alb.bindValue(":id_cab", oAlbaran->id);
        query_lin_alb.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_alb.bindValue(":codigo",ld->codigo);
        query_lin_alb.bindValue(":descripcion",ld->descripcion);
        query_lin_alb.bindValue(":cantidad",ld->cantidad);
        query_lin_alb.bindValue(":precio",ld->precio);
        query_lin_alb.bindValue(":subtotal",ld->subtotal);
        query_lin_alb.bindValue(":porc_dto",ld->dto_perc);
        query_lin_alb.bindValue(":dto",ld->dto);
        query_lin_alb.bindValue(":porc_iva",ld->iva_perc);
        query_lin_alb.bindValue(":iva",(ld->subtotal * ld->iva_perc)/100);
        query_lin_alb.bindValue(":total",ld->total);
        query_lin_alb.bindValue(":id",ld->idLinea);

        if (!query_lin_alb.exec()) {
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Ocurrió un error al guardar la línea: %1").arg(query_lin_alb.lastError().text()),
                                 tr("Aceptar"));
            ok_empresa = false;
        }
        //TODO control stock editando linea alb
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

void FrmAlbaran::lineaDeleted(lineaDetalle * ld)
{
    //todo borrar de la bd y stock y demas
    //if id = -1 pasando olimpicamente
    bool ok_empresa,ok_Maya;
    ok_empresa = true;
    ok_Maya = true;
    Configuracion_global->empresaDB.transaction();
    Configuracion_global->groupDB.transaction();
    if(ld->idLinea >-1)
    {
        //TODO control de stock
        QSqlQuery q(Configuracion_global->empresaDB);
        q.prepare("delete from lin_alb where id =:id");
        q.bindValue(":id",ld->idLinea);
        if(q.exec() && ok_Maya)
        {
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        } else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
        }
    }
    delete ld;
}




void FrmAlbaran::on_btndeshacer_clicked()
{
    Configuracion_global->groupDB.rollback();
    Configuracion_global->empresaDB.rollback();
    LLenarCampos();
    BloquearCampos(true);
}

void FrmAlbaran::on_tabWidget_2_currentChanged(int index)
{
    Q_UNUSED(index);
    helper.resizeTable();
}

void FrmAlbaran::on_btnImprimir_clicked()
{
    FrmDialogoImprimir dlg_print(this);
    dlg_print.set_email(oCliente2->email);
    dlg_print.set_preview(false);
    if(dlg_print.exec() == dlg_print.Accepted)
    {
        bool ok  /* =oFactura->set_impresa(true)*/;
        if(ok)
        {
            ui->lbimpreso->setVisible(true);
            int valor = dlg_print.get_option();
             QMap <QString,QVariant> parametros;

            switch (valor) {
            case 1: // Impresora
                //Configuracion_global->imprimir("factura_"+QString::number(oCliente2->ididioma),false,false,parametros,this);
                break;
            case 2: // email
                // TODO - enviar pdf por mail
                break;
            case 3: // PDF
                //Configuracion_global->imprimir("factura_"+QString::number(oCliente2->ididioma),true,false,parametros,this);
                break;
            case 4: //preview
                //Configuracion_global->imprimir("factura_"+QString::number(oCliente2->ididioma),false,true,parametros,this);
                break;
            default:
                break;
            }

        }
    }
}

void FrmAlbaran::on_radBusqueda_toggled(bool checked)
{
    if(checked)
        ui->stackedWidget->setCurrentIndex(1);
    else
        ui->stackedWidget->setCurrentIndex(0);
}

void FrmAlbaran::formato_tabla()
{
    // id,albaran,fecha,cif,cliente
    QStringList headers;
    QVariantList sizes;
    headers << "id" <<tr("Ser.") << tr("Albarán") <<tr("fecha") <<tr("cif") <<tr("Total") <<tr("cliente");
    sizes << 0 <<40 << 120 << 120 <<120 <<120 <<300;
    for(int i = 0; i <headers.length();i++)
    {
        ui->table2->setColumnWidth(i,sizes.at(i).toInt());
        m->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
    ui->table2->setItemDelegateForColumn(3,new DateDelegate);
    ui->table2->setItemDelegateForColumn(5,new MonetaryDelegate);
}

void FrmAlbaran::filter_table(QString texto, QString orden, QString modo)
{
    this->texto = texto;
    this->orden = orden;
    this->modo = modo;
    this->serie = cboseries->currentText();
    ui->stackedWidget->setCurrentIndex(1);
    QHash <QString,QString> h;
    h[tr("Albarán")] = "albaran";
    h[tr("Fecha")] = "fecha";
    h[tr("Cif")] = "cif";
    h[tr("Cliente")] = "cliente";
    QString order = h.value(this->orden);
    QString cSQL;

    if (modo == tr("A-Z"))
        modo = "";
    else
        modo = "DESC";
    cSQL = "select id, serie, albaran,fecha,cif,total_albaran,cliente from cab_alb where "+order+" like '%"+texto.trimmed()+
            "%' and serie = '"+this->serie+"' order by "+order +" "+modo;
    m->setQuery(cSQL,Configuracion_global->empresaDB);
}

void FrmAlbaran::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);

    QStringList orden;
    orden <<tr("Albarán")<<tr("Fecha")<<tr("Cif")<<tr("Cliente");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    //connect(this,&MayaModule::hideBusqueda,this,&FrmAlbaran::ocultarBusqueda);
    connect(m_busqueda,SIGNAL(doSearch(QString,QString,QString)),this,SLOT(filter_table(QString,QString,QString)));

    QHBoxLayout *layoutZ1 = new QHBoxLayout(this);
    QLabel* lblseries = new QLabel(this);
    lblseries->setText(tr("Series:"));

    layoutZ1->addWidget(lblseries);
    layoutZ1->addWidget(cboseries);
    m_busqueda->addLayoutZ1(layoutZ1);
    connect(cboseries,SIGNAL(currentIndexChanged(QString)),this,SLOT(on_cboseries_currentIndexChanged(QString)));

    QPushButton* add2 = new QPushButton(QIcon(":/Icons/PNG/add.png"),tr("Añadir albarán"),this);
    connect(add2,SIGNAL(clicked()),this,SLOT(on_btnAnadir_clicked()));
    m_busqueda->addWidget(add2);

    QPushButton* edit = new QPushButton(QIcon(":/Icons/PNG/edit.png"),tr("Editar albarán"),this);
    connect(edit,SIGNAL(clicked()),this,SLOT(on_btnEditar_clicked()));
    m_busqueda->addWidget(edit);

    QPushButton* print = new QPushButton(QIcon(":/Icons/PNG/impresora2.png"),tr("Imprimir albarán"),this);
    connect(print,SIGNAL(clicked()),this,SLOT(on_btnImprimir_clicked()));
    m_busqueda->addWidget(print);

    QHBoxLayout* lay = new QHBoxLayout();
    lblimpreso = new QLabel(tr("Impreso"),this);
    lblfacturado = new QLabel(tr("FActurado"),this);
    QString style = "background-color: rgb(255, 0, 0);color: rgb(255, 255, 255);";
    lblimpreso->setStyleSheet(style);
    lblfacturado->setStyleSheet(style);
    lblimpreso->setAlignment(Qt::AlignHCenter);
    lblfacturado->setAlignment(Qt::AlignHCenter);

    lay->addWidget(lblimpreso);
    lay->addWidget(lblfacturado);
    m_busqueda->addLayout(lay);

//    lblfacturado->setVisible(false);
//    lblimpreso->setVisible(false);

    QPushButton* borrar = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar Albarán"),this);
    connect(borrar,SIGNAL(clicked()),this,SLOT(on_btn_borrar_clicked()));
    m_busqueda->addWidget(borrar);
    this->orden = "Albarán";

    if(Configuracion_global->nivel == 7)
    {
        QPushButton* Forzar_edicion = new QPushButton(QIcon(":/Icons/PNG/abrecaja.png"),tr("Forzar edición"),this);
        connect(Forzar_edicion,SIGNAL(clicked()),this,SLOT(on_btnForzar_edicion_clicked()));
        m_busqueda->addWidget(Forzar_edicion);
    }
    connect(m_busqueda,SIGNAL(key_Down_Pressed()),ui->table2,SLOT(setFocus()));
}

void FrmAlbaran::on_table2_row_moved(QModelIndex actual, QModelIndex previous)
{
    Q_UNUSED(previous);
    on_table2_clicked(actual);
}

void FrmAlbaran::on_cboseries_currentIndexChanged(const QString &arg1)
{
    this->serie = arg1;
    filter_table(this->texto,this->orden,this->modo);
}

void FrmAlbaran::calcular_iva_gastos()
{
    ui->spiniva_gasto1->setValue((ui->SpinGastoDist1->value()*(ui->cboporc_iva_gasto1->currentText().toDouble()/100)));
    ui->spiniva_gasto2->setValue((ui->SpinGastoDist2->value()*(ui->cboporc_iva_gasto2->currentText().toDouble()/100)));
    ui->spiniva_gasto3->setValue((ui->SpinGastoDist3->value()*(ui->cboporc_iva_gasto3->currentText().toDouble()/100)));
}

bool FrmAlbaran::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//        if(obj == ui->txtcodigo_transportista)
//        {
//            if(keyEvent->key() == Qt::Key_F1)
//                buscar_transportista();
//        }
        if(keyEvent->key() ==Qt::Key_Escape)
            return true;
        if(obj == ui->txtcodigo_cliente)
        {
            if(keyEvent->key() == Qt::Key_F1)
                on_botBuscarCliente_clicked();
        }
        if(keyEvent->key() == Qt::Key_F1)
            if(ui->btnEditar->isEnabled())
                mostrarBusqueda();
        if(keyEvent->key() == Qt::Key_Return)
            if(ui->stackedWidget->currentIndex() == 1)
                on_table2_doubleClicked(ui->table2->currentIndex());
        return false;
    }
    else if(event->type() == QEvent::Resize)
        _resizeBarraBusqueda(m_busqueda);
    return MayaModule::eventFilter(obj,event);
}

void FrmAlbaran::on_cboOrden_currentIndexChanged(const QString &arg1)
{
   // Q_UNUSED(arg1);
   // filter_table();
}

void FrmAlbaran::on_table2_clicked(const QModelIndex &index)
{
    int id = ui->table2->model()->data(ui->table2->model()->index(index.row(),0),Qt::EditRole).toInt();
    oAlbaran->RecuperarAlbaran("select * from cab_alb where id ="+QString::number(id));
    LLenarCampos();
    BloquearCampos(true);
    ui->table2->setFocus();
}

void FrmAlbaran::on_table2_doubleClicked(const QModelIndex &index)
{
    int id = ui->table2->model()->data(ui->table2->model()->index(index.row(),0),Qt::EditRole).toInt();
    oAlbaran->RecuperarAlbaran("select * from cab_alb where id ="+QString::number(id));
    LLenarCampos();
    ui->stackedWidget->setCurrentIndex(0);
    ui->radEdicion->setChecked(true);
    ui->btnEditar->setEnabled(true);
    ocultarBusqueda();
}

void FrmAlbaran::on_btnBuscar_clicked()
{
    ui->radBusqueda->setChecked(true);
}

void FrmAlbaran::on_btnForzar_edicion_clicked()
{
    if(QMessageBox::question(this,tr("Edición de albaranes"),tr("¿Está seguro/a de quitar la marca de bloqueo?"),
                             tr("No"),tr("Desbloquear"))==QMessageBox::Accepted)
    {
        QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->table2->model());
        QModelIndex index = ui->table2->currentIndex();
        int id = Configuracion_global->devolver_id_tabla(model,index);
        QHash <QString, QVariant> h;
        QString error;
        h["editable"] = 1;

        bool updated = SqlCalls::SqlUpdate(h,"cab_alb",Configuracion_global->empresaDB,QString("id= %1").arg(id),error);
        if(updated)
            QMessageBox::information(this,tr("Editar albarán"),tr("Ahora puede editar el albarán"),tr("OK"));
        else
            QMessageBox::warning(this,tr("Editar albarán"),tr("Ocurrió un error : ")+error,tr("OK"));
    }
}

void FrmAlbaran::on_btnFacturar_clicked()
{
    if(QMessageBox::question(this,tr("Gestión de Albaranes"),
                             tr("¿Desea facturar este albarán?"),tr("No"),tr("Sí"))==QMessageBox::Accepted)
    {
        Configuracion_global->empresaDB.transaction();

        if(QString(oAlbaran->factura).isEmpty()|| oAlbaran->factura == "0")
        {
            QHash <QString,QVariant> cab_fac;
            QString error;
            //---------------------------------
            // Pasamos valores reales al QHash
            //---------------------------------
            cab_fac["codigo_cliente"] = oAlbaran->codigo_cliente;
            cab_fac["factura"] = oAlbaran->factura;
            cab_fac["ejercicio"] = oAlbaran->ejercicio;
            cab_fac["serie"] = oAlbaran->serie;
            cab_fac["fecha"] = oAlbaran->fecha;
            //TODO - vencimientos;
            // cab_fac["fecha_cobro"] = oAlbaran->fecha_cobro;
            cab_fac["id_cliente"] = oAlbaran->id_cliente;
            cab_fac["cliente"] = oAlbaran->cliente;
            cab_fac["direccion1"] = oAlbaran->direccion1;
            cab_fac["direccion2"] = oAlbaran->direccion2;
            cab_fac["cp"] = oAlbaran->cp;
            cab_fac["poblacion"] = oAlbaran->poblacion;
            cab_fac["provincia"] = oAlbaran->provincia;
            cab_fac["id_pais"] = oAlbaran->id_pais;
            cab_fac["direccion1_entrega"] = oAlbaran->direccion1_entrega;
            cab_fac["direccion2_entrega"] = oAlbaran->direccion2_entrega;
            cab_fac["cp_entrega"] = oAlbaran->cp_entrega;
            cab_fac["poblacion_entrega"] = oAlbaran->poblacion_entrega;
            cab_fac["provincia_entrega"] = oAlbaran->provincia_entrega;
            cab_fac["id_pais_entrega"] = oAlbaran->id_pais_entrega;
            cab_fac["cif"] = oAlbaran->cif;
            cab_fac["recargo_equivalencia"] = oAlbaran->recargo_equivalencia;
            cab_fac["subtotal"] = oAlbaran->subtotal;
            cab_fac["porc_dto"] = oAlbaran->porc_dto;
            cab_fac["porc_dto_pp"] = oAlbaran->porc_dto_pp;
            cab_fac["dto"] = oAlbaran->dto;
            cab_fac["dto_pp"] = oAlbaran->dto_pp;
            cab_fac["base"] = oAlbaran->base_total;
            cab_fac["iva"] = oAlbaran->iva_total;
            cab_fac["total"] = oAlbaran->total_albaran;
            cab_fac["impreso"] = oAlbaran->impreso;
            //TODO - formapago
    //        cab_fac["id_forma_pago"] = oAlbaran->id_forma_pago;
    //        cab_fac["forma_pago"] = oAlbaran->forma_pago;
            cab_fac["comentario"] = oAlbaran->comentario;
            cab_fac["base1"] = oAlbaran->base1;
            cab_fac["base2"] = oAlbaran->base2;
            cab_fac["base3"] = oAlbaran->base3;
            cab_fac["base4"] = oAlbaran->base4;
            cab_fac["porc_iva1"] = oAlbaran->porc_iva1;
            cab_fac["porc_iva2"] = oAlbaran->porc_iva2;
            cab_fac["porc_iva3"] = oAlbaran->porc_iva3;
            cab_fac["porc_iva4"] = oAlbaran->porc_iva4;
            cab_fac["iva1"] = oAlbaran->iva1;
            cab_fac["iva2"] = oAlbaran->iva2;
            cab_fac["iva3"] = oAlbaran->iva3;
            cab_fac["iva4"] = oAlbaran->iva4;
            cab_fac["total1"] = oAlbaran->total1;
            cab_fac["total2"] = oAlbaran->total2;
            cab_fac["total3"] = oAlbaran->total3;
            cab_fac["total4"] = oAlbaran->total4;
            cab_fac["porc_rec1"] = oAlbaran->porc_rec1;
            cab_fac["porc_rec2"] = oAlbaran->porc_rec2;
            cab_fac["porc_rec3"] = oAlbaran->porc_rec3;
            cab_fac["porc_rec4"] = oAlbaran->porc_rec4;
            cab_fac["rec1"] = oAlbaran->rec1;
            cab_fac["rec2"] = oAlbaran->rec2;
            cab_fac["rec3"] = oAlbaran->rec3;
            cab_fac["rec4"] = oAlbaran->rec4;
            cab_fac["total_recargo"] = oAlbaran->rec_total;
            cab_fac["entregado_a_cuenta"] = oAlbaran->entregado_a_cuenta;
            cab_fac["importe_pendiente"] = oAlbaran->total_albaran;
            cab_fac["codigo_entidad"] = oCliente2->entidad_bancaria;
            cab_fac["oficina_entidad"] = oCliente2->oficina_bancaria;
            cab_fac["dc_cuenta"] =oCliente2->dc;
            cab_fac["cuenta_corriente"] = oCliente2->cuenta_corriente;
            cab_fac["pedido_cliente"] = oAlbaran->pedido_cliente;
            // TODO IRPF
            //cab_fac["irpf"] = oAlbaran->;
            //cab_fac["irpf"] = oAlbaran->irpf;
            cab_fac["id_transportista"] = oAlbaran->id_transportista;
            cab_fac["desc_gasto1"] = oAlbaran->desc_gasto1;
            cab_fac["desc_gasto2"] = oAlbaran->desc_gasto2;
            cab_fac["desc_gasto3"] = oAlbaran->desc_gasto3;
            cab_fac["imp_gasto1"] = oAlbaran->imp_gasto1;
            cab_fac["imp_gasto2"] = oAlbaran->imp_gasto2;
            cab_fac["imp_gasto3"] = oAlbaran->imp_gasto3;
            // h["total"] == ui->txttotal->text().replace(".","").replace(",",".").toDouble();
           Factura oFactura;
    //        QDialog* dlg = new QDialog(this);
    //        dlg->setWindowTitle(tr("Seleccione serie factura"));
    //        dlg->resize(270,150);
    //        QComboBox* box = new QComboBox(dlg);
    //        QPushButton*  btn = new QPushButton("Aceptar",dlg);
    //        QVBoxLayout lay(dlg);

    //        lay.addWidget(box);
    //        lay.addWidget(btn);

    //        dlg->setLayout(&lay);

    //        QSqlQueryModel *l = new QSqlQueryModel(this);
    //        l->setQuery("select serie from series",Configuracion_global->empresaDB);
    //        box->setModel(l);

    //        connect(btn,SIGNAL(clicked()),dlg,SLOT(accept()));
    //        dlg->exec();//aki se podria poner otro boton y cancelar todo?

    //        QString serie = box->currentText();
    //        dlg->deleteLater();
            QString fra =  oFactura.NuevoNumeroFactura(oAlbaran->serie);
            QString num = oAlbaran->serie+"/"+ fra;

            cab_fac["factura"] = fra;
            cab_fac["fecha"] = QDate::currentDate();
            cab_fac["serie"] = oAlbaran->serie;

            int added = SqlCalls::SqlInsert(cab_fac,"cab_fac",Configuracion_global->empresaDB,error);
            if(added>-1)
            {
                // -----------------
                // Lineas de factura
                // -----------------
                int added_l;
                QHash <QString, QVariant> h_l;
                QString d = QString("id_cab = %1").arg(oAlbaran->id);
                QMap<int, QSqlRecord> map = SqlCalls::SelectRecord("lin_alb", d,Configuracion_global->empresaDB, error);
                  QMapIterator<int, QSqlRecord> i_l(map);
                  while (i_l.hasNext())
                  {
                      i_l.next();
                      QSqlRecord r_l = i_l.value();
                      for(int i= 0; i< r_l.count();i++)
                          h_l.insert(r_l.fieldName(i),r_l.value(i));
                      h_l.remove("id");
                      h_l["id_cab"] = added;
                     added_l = SqlCalls::SqlInsert(h_l,"lin_fac",Configuracion_global->empresaDB,error);
                     if(added_l == -1)
                         break;
                  }
                if(added_l == -1)
                {
                    Configuracion_global->empresaDB.rollback();
                    qDebug() <<added;
                    qDebug() <<error;
                } else
                {
                    QHash <QString, QVariant> p;
                    QString c;

                    p["factura"] = num;
                    p["facturado"] = 1;
                    p["fecha_factura"] = QDate::currentDate();
                    c = "id="+QString::number(oAlbaran->id);
                    bool updated = SqlCalls::SqlUpdate(p,"cab_alb",Configuracion_global->empresaDB,c,error);

                    if(updated)
                    {
                        Configuracion_global->empresaDB.commit();
                        oFactura.RecuperarFactura(added);
                        t = new TimedMessageBox(qApp->activeWindow(),
                                                                  QObject::tr("Se ha creado la factura num:")+
                                                Configuracion_global->serie+"/"+num);
                        vencimientos vto(this);
                        //-----------------
                        // Asiento contable
                        //-----------------
                        if(Configuracion_global->contabilidad)
                        {

                            bool creado;
                            oCliente2->Recuperar("select * from clientes where id = "+QString::number(oFactura.id_cliente));
                            if(oFactura.apunte == 0)
                                creado = oFactura.Apunte();
                            else
                                creado = oFactura.EditApunte(oFactura.apunte);
                            if(!creado)
                                Configuracion_global->empresaDB.rollback();
                        }
                        //------------------
                        // Vencimiento
                        //------------------
                        vto.calcular_vencimiento(oFactura.fecha,oFactura.id_cliente,0,oFactura.id,(oFactura.serie+"/"+oFactura.factura),1,
                                                 "V",oFactura.total);
                        oAlbaran->RecuperarAlbaran("select * from cab_alb where id ="+QString::number(oAlbaran->id));
                        LLenarCampos();
                    } else
                    {
                        QMessageBox::warning(this,tr("Traspaso a factura"),QString(tr("No se pudo crear la factura error: ")+"%1").arg(error),tr("Aceptar"));

                        Configuracion_global->empresaDB.rollback();
                    }
                }
            } else
            {
                QMessageBox::warning(this,tr("Traspaso a factura"),
                                   QString(tr("No se pudo crear la factura: Error BD: %1").arg(error)),tr("Aceptar"));
                Configuracion_global->empresaDB.rollback();
            }

        } else
            QMessageBox::information(this,tr("Traspasar albarán"),tr("Este albarán ya ha sido traspasado\nNo puede traspasar dos veces el mismo albarán"),
                                         tr("Aceptar"));
    }
}

void FrmAlbaran::on_spinporc_Dto_editingFinished()
{
    //--------------------------------------------
    // Cambio dto en líneas
    //--------------------------------------------
    QMap <int,QSqlRecord> rec;
    QString error;
    QStringList clauses;
    Articulo oArt(this);
    int id_art,id_lin;
    float porc_dto_esp, porc_dto_lin;
    double subtotal;

    clauses << QString("id_cab = %1").arg(oAlbaran->id);

    rec = SqlCalls::SelectRecord("lin_alb",clauses,Configuracion_global->empresaDB,error);
    QMapIterator <int, QSqlRecord> i(rec);
    while (i.hasNext())
    {
        i.next();
        id_lin = i.value().value("id").toInt();
        id_art = i.value().value("id_articulo").toInt();
        subtotal = i.value().value("subtotal").toDouble();
        porc_dto_lin = i.value().value("porc_dto").toFloat();

        porc_dto_esp = oArt.asigna_dto_linea(id_art,oCliente2->id,ui->spinporc_Dto->value(),porc_dto_lin);
        QHash <QString,QVariant> f;
        clauses.clear();
        clauses << QString("id = %1").arg(id_lin);
        f["porc_dto"] = porc_dto_esp;
        f["dto"] = subtotal *(porc_dto_esp/100.0);

        bool upd = SqlCalls::SqlUpdate(f,"lin_alb",Configuracion_global->empresaDB,clauses,error);
        if(!upd)
            QMessageBox::warning(this,tr("Cambio DTO"),tr("No se pudo realizar el cambio en las líneas, error:%1").arg(error),
                                 tr("Aceptar"));
        else
            helper.calculatotal();
    }
    //--------------------------
    // Calculo dto total
    //--------------------------
    double dto = (Configuracion_global->MonedatoDouble(ui->txtsubtotal->text())*(ui->spinporc_Dto->value()/100.0));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales_campos_totales)));
    oAlbaran->dto = dto;
    QString filter = QString("id_cab = '%1'").arg(oAlbaran->id);
    helper.fillTable("empresa","lin_alb",filter);


}

void FrmAlbaran::on_spinPorc_dto_pp_editingFinished()
{
    float dto_pp;
    dto_pp = (Configuracion_global->MonedatoDouble(ui->txtsubtotal->text())*(ui->spinPorc_dto_pp->value()/100.0));
    ui->txtDtoPP->setText(Configuracion_global->toFormatoMoneda(QString::number(dto_pp,'f',Configuracion_global->decimales_campos_totales)));
    oAlbaran->dto_pp = dto_pp;
    QString filter = QString("id_cab = '%1'").arg(oAlbaran->id);
    helper.fillTable("empresa","lin_alb",filter);
}

void FrmAlbaran::mostrarBusqueda()
{
    _showBarraBusqueda(m_busqueda);
    m_busqueda->doFocustoText();
}

void FrmAlbaran::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}


