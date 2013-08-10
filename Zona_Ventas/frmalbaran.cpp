#include "frmalbaran.h"
#include "ui_frmalbaran.h"
#include "albaran.h"
#include "../Zona_Pacientes/cliente.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Auxiliares/datedelegate.h"
//
#include "../Almacen/articulo.h"

Albaran *oAlbaran = new Albaran();
Cliente *oCliente2 = new Cliente();

FrmAlbaran::FrmAlbaran(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmAlbaran),
    helper(this),
    toolButton(tr("Albaranes"),":/Icons/PNG/albaran.png",this),
    menuButton(QIcon(":/Icons/PNG/albaran.png"),tr("Albaranes"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/albaran.png"),"",this))
{
    ui->setupUi(this);
    // Pongo valores por defecto
    ui->lbfacturado->setVisible(false);
    ui->lbimpreso->setVisible(false);
    ui->lblNumFactura->setVisible(false);
    ui->txtcNumFra->setVisible(false);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de albaranes a clientes"));

    ui->comboPais->setModel(Configuracion_global->paises_model);
    ui->comboPais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));

    BloquearCampos(true);

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
    m->setQuery("select id,albaran,fecha,cif,cliente from cab_alb order by albaran desc",Configuracion_global->empresaDB);
    ui->tabla->setModel(m);
    formato_tabla();

    //--------------------
    // llenar cboBusqueda
    //--------------------
    QStringList dbIndex;
    dbIndex << tr("Albarán") <<tr("Fecha") <<tr("cif") <<tr("cliente");
    ui->cboOrden->addItems(dbIndex);
    ui->radBusqueda->setChecked(true);
    ui->stackedWidget->setCurrentIndex(1);

    ui->txtBuscar->setFocus();

}

FrmAlbaran::~FrmAlbaran()
{
    delete ui;
}


void FrmAlbaran::LLenarCampos() {
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

    QList<QString> keys = Configuracion_global->paises.uniqueKeys();
    for (int i = 0;i<keys.size();i++)
    {
        if(Configuracion_global->paises[keys.at(i)].value("id").toInt() == oAlbaran->id_pais)
        {
            int index = ui->comboPais->findText(keys.at(i));
            ui->comboPais->setCurrentIndex(index);
        }
    }

    ui->txtcif->setText(oAlbaran->cif);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda( QString::number(oAlbaran->subtotal,'f',2)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->dto,'f',2)));
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number( oAlbaran->base_total,'f',2)));
    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number( oAlbaran->base_total,'f',2)));

    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva_total,'f',2)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total_albaran,'f',2)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total_albaran,'f',2)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec_total,'f',2)));
    lEstado = oAlbaran->impreso;
    if((lEstado == true)) {
        ui->lbimpreso->setVisible(true);
    } else {
        ui->lbimpreso->setVisible(false);
    }
    lEstado = oAlbaran->facturado;
    if ((lEstado == true)) {
        ui->lbfacturado->setVisible(true);
        ui->txtfecha_factura->setVisible(true);
        ui->txtcNumFra->setVisible(true);
    } else {
        ui->lbfacturado->setVisible(false);
        ui->txtfecha_factura->setVisible(false);
        ui->txtcNumFra->setVisible(false);

    }
    ui->txtcomentario->setText(oAlbaran->comentario);
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->base1,'f',2)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->base2,'f',2)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->base3,'f',2)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->base4,'f',2)));
    ui->txtporc_iva1->setText(QString::number(oAlbaran->porc_iva1));
    ui->txtporc_iva2->setText(QString::number(oAlbaran->porc_iva2));
    ui->txtporc_iva3->setText(QString::number(oAlbaran->porc_iva3));
    ui->txtporc_iva4->setText(QString::number(oAlbaran->porc_iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva1,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva2,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva3,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva4,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total1,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total2,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total3,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total4,'f',2)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->porc_rec1,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->porc_rec2,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->porc_rec3,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->porc_rec4,'f',2)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec1,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec2,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec3,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec4,'f',2)));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec_total,'f',2)));
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva_total,'f',2)));
    ui->txtpedido_cliente->setText(oAlbaran->pedido_cliente);
    if(oAlbaran->recargo_equivalencia==true)
        ui->chklporc_rec->setChecked(true);
    else
        ui->chklporc_rec->setChecked(false);

    ui->txtentregado_a_cuenta->setText(QString::number(oAlbaran->entregado_a_cuenta));
    oCliente2->Recuperar("Select * from clientes where id ="+QString::number(oAlbaran->id_cliente));
    helper.set_tarifa(oCliente2->tarifa_cliente);
    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();

    QString filter = QString("id_Cab = '%1'").arg(oAlbaran->id);
    helper.fillTable("empresa","lin_alb",filter);
    helper.set_tipo_dto_tarifa(oCliente2->tipo_dto_tarifa);
}
void FrmAlbaran::LLenarCamposCliente()
{
    ui->lblCliente->setText(oCliente2->nombre_fiscal);
    ui->txtcodigo_cliente->setText(oCliente2->codigo_cliente);
    ui->txtcliente->setText(oCliente2->nombre_fiscal);
    ui->txtdireccion1->setText(oCliente2->direccion1);
    ui->txtdireccion2->setText(oCliente2->direccion2);
    ui->txtcp->setText(oCliente2->cp);
    ui->txtpoblacion->setText(oCliente2->poblacion);
    ui->txtprovincia->setText(oCliente2->provincia);
    int index = ui->comboPais->findText(oAlbaran->pais);
    ui->comboPais->setCurrentIndex(index);
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
    ui->lblNumFactura->setVisible(false);
    ui->txtcNumFra->setVisible(false);
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
    ui->cboOrden->setEnabled(state);
    ui->txtBuscar->setReadOnly(!state);

    helper.blockTable(state);
}

void FrmAlbaran::LLenarAlbaran()
{
    oAlbaran->codigo_cliente= (ui->txtcodigo_cliente->text());
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
    oAlbaran->cif= (ui->txtcif->text());
    oAlbaran->subtotal= (ui->txtsubtotal->text().replace(".","").toDouble());
    oAlbaran->dto= (ui->txtimporte_descuento->text().replace(".","").toDouble());
    oAlbaran->base_total= (ui->txtbase->text().replace(".","").toDouble());
    oAlbaran->iva_total= (ui->txtiva->text().replace(".","").toDouble());
    oAlbaran->total_albaran= (ui->txttotal->text().replace(".","").toDouble());
    oAlbaran->comentario= (ui->txtcomentario->toPlainText());
    oAlbaran->base1= (ui->txtbase1->text().replace(".","").toDouble());
    oAlbaran->base2= (ui->txtbase2->text().replace(".","").toDouble());
    oAlbaran->base3= (ui->txtbase3->text().replace(".","").toDouble());
    oAlbaran->base4= (ui->txtbase4->text().replace(".","").toDouble());
    oAlbaran->porc_iva1= (ui->txtporc_iva1->text().replace(".","").toDouble());
    oAlbaran->porc_iva2= (ui->txtporc_iva2->text().replace(".","").toDouble());
    oAlbaran->porc_iva3= (ui->txtporc_iva3->text().replace(".","").toDouble());
    oAlbaran->porc_iva4= (ui->txtporc_iva4->text().replace(".","").toDouble());
    oAlbaran->iva1= (ui->txtiva1->text().replace(".","").toDouble());
    oAlbaran->iva2= (ui->txtiva2->text().replace(".","").toDouble());
    oAlbaran->iva3= (ui->txtiva3->text().replace(".","").toDouble());
    oAlbaran->iva4= (ui->txtiva4->text().replace(".","").toDouble());
    oAlbaran->total1= (ui->txttotal1->text().replace(".","").toDouble());
    oAlbaran->total2= (ui->txttotal2->text().replace(".","").toDouble());
    oAlbaran->total3= (ui->txttotal3->text().replace(".","").toDouble());
    oAlbaran->total4= (ui->txttotal4->text().replace(".","").toDouble());
    oAlbaran->porc_rec1= (ui->txtporc_rec1->text().replace(".","").toDouble());
    oAlbaran->porc_rec2= (ui->txtporc_rec2->text().replace(".","").toDouble());
    oAlbaran->porc_rec3= (ui->txtporc_rec3->text().replace(".","").toDouble());
    oAlbaran->porc_rec4= (ui->txtporc_rec4->text().replace(".","").toDouble());
    oAlbaran->rec1= (ui->txtporc_rec1->text().replace(".","").toDouble());
    oAlbaran->rec2= (ui->txtporc_rec2->text().replace(".","").toDouble());
    oAlbaran->rec3= (ui->txtporc_rec3->text().replace(".","").toDouble());
    oAlbaran->rec4= (ui->txtporc_rec4->text().replace(".","").toDouble());
    oAlbaran->rec_total= (ui->txttotal_recargo->text().replace(".","").toDouble());
    oAlbaran->dto = ui->txtimporte_descuento->text().replace(".","").toDouble();
    if (ui->chklporc_rec->isChecked())
        oAlbaran->recargo_equivalencia = (1);
    else
        oAlbaran->recargo_equivalencia = (0);

    oAlbaran->pedido_cliente= (ui->txtpedido_cliente->text());
    oAlbaran->entregado_a_cuenta = ui->txtentregado_a_cuenta->text().replace(".","").toDouble();
}

void FrmAlbaran::on_btnSiguiente_clicked()
{
    int albaran = oAlbaran->id;
    if(oAlbaran->RecuperarAlbaran("Select * from cab_alb where id >'"+QString::number(albaran)+"' order by albaran  limit 1 "))
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
    int albaran = oAlbaran->id;
    if(oAlbaran->RecuperarAlbaran("Select * from cab_alb where id <'"+QString::number(albaran)+"' order by albaran desc limit 1 "))
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
    VaciarCampos();    
    LLenarAlbaran();
    oAlbaran->AnadirAlbaran();
    ui->txtalbaran->setText(QString::number(oAlbaran->albaran));
    BloquearCampos(false);
    ui->txtcodigo_cliente->setFocus();    
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
    if (!oAlbaran->facturado == true)
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
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',2))+moneda);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',2))+moneda);
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);
}

void FrmAlbaran::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmAlbaran::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmAlbaran::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmAlbaran::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
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
                                  "dto =:porc_dto,"
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
    headers << "id" << tr("Albarán") <<tr("fecha") <<tr("cif") <<tr("cliente");
    sizes << 0 << 120 << 120 <<120 <<300;
    for(int i = 0; i <headers.length();i++)
    {
        ui->tabla->setColumnWidth(i,sizes.at(i).toInt());
        m->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
    ui->tabla->setItemDelegateForColumn(2,new DateDelegate);
}

void FrmAlbaran::on_cboOrden_currentIndexChanged(const QString &arg1)
{
    //<< tr("Albarán") <<tr("Fecha") <<tr("cif") <<tr("cliente");
    QHash <QString,QString> h;
    h[tr("Albarán")] = "albaran";
    h[tr("Fecha")] = "fecha";
    h[tr("cif")] = "cif";
    h[tr("cliente")] = "cliente";
    QString order = h.value(arg1);
    if(order =="cliente" || order =="cif")
        m->setQuery("select id,albaran,fecha,cif,cliente from cab_alb order by "+order,Configuracion_global->empresaDB);
    else
        m->setQuery("select id,albaran,fecha,cif,cliente from cab_alb order by "+order +" desc",Configuracion_global->empresaDB);

}

void FrmAlbaran::on_tabla_clicked(const QModelIndex &index)
{
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tabla->model());
    int id = Configuracion_global->devolver_id_tabla(model,index);
    oAlbaran->RecuperarAlbaran("select * from cab_alb where id ="+QString::number(id));
    LLenarCampos();
    BloquearCampos(true);
}

void FrmAlbaran::on_tabla_doubleClicked(const QModelIndex &index)
{
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tabla->model());
    int id = Configuracion_global->devolver_id_tabla(model,index);
    oAlbaran->RecuperarAlbaran("select * from cab_alb where id ="+QString::number(id));
    LLenarCampos();
    BloquearCampos(true);
    ui->radEdicion->setChecked(true);
}

void FrmAlbaran::on_btnBuscar_clicked()
{
    ui->radBusqueda->setChecked(true);
}

void FrmAlbaran::on_txtBuscar_textEdited(const QString &arg1)
{
    //<< tr("Albarán") <<tr("Fecha") <<tr("cif") <<tr("cliente");
    QHash <QString,QString> h;
    h[tr("Albarán")] = "albaran";
    h[tr("Fecha")] = "fecha";
    h[tr("cif")] = "cif";
    h[tr("cliente")] = "cliente";
    QString order = h.value(ui->cboOrden->currentText());


    if(order =="cliente" || order =="cif")
        m->setQuery("select id,albaran,fecha,cif,cliente from cab_alb where "+order+" like '%"+arg1.trimmed()+
                    "%' order by "+order,Configuracion_global->empresaDB);
    else
        m->setQuery("select id,albaran,fecha,cif,cliente from cab_alb where "+order+"like '%"+arg1.trimmed()+
                    "%' order by "+order +" desc",Configuracion_global->empresaDB);

}
