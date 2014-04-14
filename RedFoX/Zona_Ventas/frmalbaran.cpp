#include "frmalbaran.h"
#include "ui_frmalbaran.h"


#include "../Busquedas/db_consulta_view.h"
#include "Auxiliares/datedelegate.h"
#include "Auxiliares/monetarydelegate.h"
#include "Auxiliares/monetarydelegate_totals.h"
#include "Auxiliares/numericdelegate.h"
#include "../Zona_Ventas/factura.h"
#include "../vencimientos.h"
//
#include "../Almacen/articulo.h"
#include<math.h>
#include "Auxiliares/frmeditline.h"

#include "frmlistadoalbaran.h"

FrmAlbaran::FrmAlbaran(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmAlbaran),
    menuButton(QIcon(":/Icons/PNG/albaran.png"),tr("Albaranes"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/albaran.png"),"",this))
{    
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de albaranes a clientes"));
    __init = false;
    this->installEventFilter(this);
}

FrmAlbaran::~FrmAlbaran()
{
    delete ui;
}
void FrmAlbaran::init_querys()
{
    if(!__init)
        return;
    modelLineas->setQuery("select id,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                          "from lin_alb where id = 0;",Configuracion_global->empresaDB);
    m->setQuery(QString("select id,serie,albaran,fecha,cif,total_albaran,cliente from cab_alb where ejercicio =%1 order by albaran desc").arg(
                    Configuracion_global->cEjercicio),Configuracion_global->empresaDB);
    series->setQuery("select serie from series", Configuracion_global->empresaDB);
    formato_tabla();
    ui->tabWidget_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(1);
}

void FrmAlbaran::init()
{
    if(__init)
        return;
    ui->setupUi(this);

    oAlbaran = new Albaran(this);
    oCliente2 = new Cliente(this);

    setUpBusqueda();

    ui->table2->setItemDelegateForColumn(3,new DateDelegate(this));
    ui->table2->setItemDelegateForColumn(5,new MonetaryDelegate(this));

    // Pongo valores por defecto
    ui->lbfacturado->setVisible(false);
    ui->lbimpreso->setVisible(false);

    ui->comboPais->setModel(Configuracion_global->paises_model);
    ui->cboPais_entrega->setModel(Configuracion_global->paises_model);

    ui->comboPais->setModelColumn(1);
    ui->cboPais_entrega->setModelColumn(1);
    ui->cbo_forma_pago->setModel(Configuracion_global->formapago_model);
    ui->cbo_forma_pago->setModelColumn(2);

    ui->cboAgente->setModel(Configuracion_global->agentes_model);
    ui->cboAgente      ->setModelColumn(2);

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));

    // Modelo y formato tabla lineas
    modelLineas = new QSqlQueryModel(this);
    m = new QSqlQueryModel(this);

    ui->Lineas->setModel(modelLineas);
    ui->table2->setModel(m);

    cboseries->setCurrentIndex(0);

    ui->Lineas->setItemDelegateForColumn(3,new NumericDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(4,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(5,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(6,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(7,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(8,new MonetaryDelegate(this,true));
    ui->Lineas->setItemDelegateForColumn(9,new MonetaryDelegate_totals(this,true));

    ui->btn_borrar->setEnabled(false);
    ui->btnEditar->setEnabled(false);
    ui->btnFacturar->setEnabled(false);
    ui->btnSiguiente->setEnabled(false);
    ui->btnAnterior->setEnabled(false);
    ui->btnImprimir->setEnabled(false);
    ui->btnBuscar->setEnabled(true);
    oAlbaran->id = -1;
    ui->btnSiguiente->setEnabled(true);

    formato_tabla();

    ui->stackedWidget->setCurrentIndex(1);

    ui->cboporc_iva_gasto1->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto2->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto3->setModel(Configuracion_global->iva_model);

    ui->cboporc_iva_gasto1->setModelColumn(4);
    ui->cboporc_iva_gasto2->setModelColumn(4);
    ui->cboporc_iva_gasto3->setModelColumn(4);

    ui->cboDivisa->setModel(Configuracion_global->divisas_model);
    ui->cboDivisa->setModelColumn(1);    

    QList<QWidget*> l = this->findChildren<QWidget*>();
    QList<QWidget*> ::Iterator it;

    for( it = l.begin() ;it!= l.end();++it )
        (*it)->installEventFilter(this);

    BloquearCampos(true);
    __init = true;
}

void FrmAlbaran::LLenarCampos()
{
    if(!__init)
        return;
    ui->txtTelefonoCli->setText(oAlbaran->telefono);
    ui->txtFaxCli->setText(oAlbaran->fax);
    ui->txtMovilCli->setText(oAlbaran->movil);
    ui->txtEmailCli->setText(oAlbaran->email);

    ui->cboSerie->setCurrentIndex(ui->cboSerie->findText(oAlbaran->serie));
    ui->lblCliente->setText(oAlbaran->cliente);
    ui->lblAlbaran->setText(oAlbaran->albaran);
    ui->txtcodigo_cliente->setText(oAlbaran->codigo_cliente);
    ui->txtalbaran->setText(oAlbaran->albaran);
    ui->txtfecha->setDate(oAlbaran->fecha);
    ui->txtfecha_factura->setDate(oAlbaran->fecha_factura);
    ui->txtcliente->setText(oAlbaran->cliente);
    ui->txtdireccion1->setText(oAlbaran->direccion1);
    ui->txtdireccion2->setText(oAlbaran->direccion2);
    ui->txtcp->setText(oAlbaran->cp);
    ui->txtpoblacion->setText(oAlbaran->poblacion);
    ui->txtprovincia->setText(oAlbaran->provincia);

    for(auto i= 0; i< Configuracion_global->paises_model->rowCount();++i)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oAlbaran->id_pais)
        {
            ui->comboPais->setCurrentIndex(i);
            break;
        }
    }

    ui->txtDireccion1_entrega->setText(oAlbaran->direccion1_entrega);
    ui->txtDireccion2_entrega->setText(oAlbaran->direccion2_entrega);
    ui->txtCp_entrega->setText(oAlbaran->cp_entrega);
    ui->txtPoblacion_entrega->setText(oAlbaran->poblacion_entrega);
    ui->txtProvincia_entrega->setText(oAlbaran->provincia_entrega);

    for(auto i= 0; i< Configuracion_global->paises_model->rowCount();++i)
    {
        if(Configuracion_global->paises_model->record(i).value("id").toInt() == oAlbaran->id_pais_entrega)
        {
            ui->cboPais_entrega->setCurrentIndex(i);
            break;
        }
    }
    ui->txtemail_alternativa->setText(oAlbaran->email_entrega);
    ui->txtcomentarios_alternativa->setPlainText(oAlbaran->comentarios_entrega);


    ui->txtcif->setText(oAlbaran->cif);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda( QString::number(oAlbaran->subtotal,'f',Configuracion_global->decimales)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->dto,'f',Configuracion_global->decimales)));
    ui->txtbase_total->setText(Configuracion_global->toFormatoMoneda(QString::number( oAlbaran->base_total,'f',Configuracion_global->decimales)));
    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number( oAlbaran->base_total,'f',Configuracion_global->decimales)));

    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva_total,'f',Configuracion_global->decimales)));
    ui->txttotal_total->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total_albaran,'f',Configuracion_global->decimales)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->total_albaran,'f',Configuracion_global->decimales)));
    ui->txtrec_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec_total,'f',Configuracion_global->decimales)));

    ui->lbimpreso->setVisible(oAlbaran->impreso);

    ui->lbfacturado->setVisible(oAlbaran->facturado);

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
    ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->rec_total,'f',Configuracion_global->decimales)));
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbaran->iva_total,'f',Configuracion_global->decimales)));
    ui->txtpedido_cliente->setText(oAlbaran->pedido_cliente);
    ui->txtcNumFra->setText(oAlbaran->factura);
    ui->txtGastoDist1->setText(oAlbaran->desc_gasto1);
    ui->txtGastoDist2->setText(oAlbaran->desc_gasto2);
    ui->txtGastoDist3->setText(oAlbaran->desc_gasto3);
    ui->SpinGastoDist1->setValue(oAlbaran->imp_gasto1);
    ui->SpinGastoDist2->setValue(oAlbaran->imp_gasto2);
    ui->SpinGastoDist3->setValue(oAlbaran->imp_gasto3);
    ui->cboporc_iva_gasto1->setCurrentIndex(ui->cboporc_iva_gasto1->findText(QString::number(oAlbaran->porc_iva_gasto1)));
    ui->cboporc_iva_gasto2->setCurrentIndex(ui->cboporc_iva_gasto2->findText(QString::number(oAlbaran->porc_iva_gasto2)));
    ui->cboporc_iva_gasto3->setCurrentIndex(ui->cboporc_iva_gasto3->findText(QString::number(oAlbaran->porc_iva_gasto3)));
    ui->spiniva_gasto1->setValue(oAlbaran->iva_gasto1);
    ui->spiniva_gasto2->setValue(oAlbaran->iva_gasto2);
    ui->spiniva_gasto3->setValue(oAlbaran->iva_gasto3);


    ui->chkrecargo_equivalencia->setChecked(oAlbaran->recargo_equivalencia);


    ui->txtentregado_a_cuenta->setText(QString::number(oAlbaran->entregado_a_cuenta));
    ui->spinporc_Dto->setValue(oAlbaran->porc_dto);
    ui->spinPorc_dto_pp->setValue(oAlbaran->porc_dto_pp);

    oCliente2->Recuperar("Select * from clientes where id ="+QString::number(oAlbaran->id_cliente),false);

    for(auto i=0;i< Configuracion_global->formapago_model->rowCount();++i)
    {
        if(Configuracion_global->formapago_model->record(i).value("id").toInt() == oCliente2->id_forma_pago)
        {
            ui->cbo_forma_pago->setCurrentIndex(i);
            break;
        }
    }
    //---------------------------
    // DIVISA
    //---------------------------
    for(auto i = 0;i<Configuracion_global->divisas_model->rowCount();++i)
    {
        if(Configuracion_global->divisas_model->record(i).value("id").toInt() == oAlbaran->id_divisa)
        {
            ui->cboDivisa->setCurrentIndex(i);
            break;
        }
    }
    //----------------------------
    // Barra Busquedas
    //----------------------------


    for (auto a=0; a< Configuracion_global->agentes_model->rowCount();a++)
    {
        if(oAlbaran->id_agente == Configuracion_global->agentes_model->record(a).value("id").toInt())
        {
            ui->cboAgente->setCurrentIndex(a);
            break;
        }
    }
    ui->txtimporte_irpf->setText(QString::number(oAlbaran->irpf,'f',2));
    ui->spinporc_irpf->setValue(oAlbaran->porc_irpf);


    ui->lblPedido->setVisible(!(ui->txtpedido_cliente->text().isEmpty()||ui->txtpedido_cliente->text() == "0"));
    ui->txtpedido_cliente->setVisible(!(ui->txtpedido_cliente->text().isEmpty()||ui->txtpedido_cliente->text() == "0"));

    ui->lblFac->setVisible(!(ui->txtcNumFra->text().isEmpty()||ui->txtcNumFra->text()== "0"));
    ui->txtcNumFra->setVisible(!(ui->txtcNumFra->text().isEmpty()||ui->txtcNumFra->text()== "0"));
    ui->lblFechFac->setVisible(!(ui->txtcNumFra->text().isEmpty()||ui->txtcNumFra->text()== "0"));
    ui->txtfecha_factura->setVisible(!(ui->txtcNumFra->text().isEmpty()||ui->txtcNumFra->text()== "0"));
}
void FrmAlbaran::LLenarCamposCliente()
{
    if(!__init)
        return;
    //oCliente2->Recuperar("Select * from clientes where id ="+QString::number(oAlbaran->id_cliente));
    ui->lblCliente->setText(oCliente2->nombre_fiscal);
    ui->txtcodigo_cliente->setText(oCliente2->codigo_cliente);
    ui->txtcliente->setText(oCliente2->nombre_fiscal);

    // dirección
    ui->txtdireccion1->setText(oCliente2->direccion1);
    ui->txtdireccion2->setText(oCliente2->direccion2);
    ui->txtcp->setText(oCliente2->cp);
    ui->txtpoblacion->setText(oCliente2->poblacion);
    ui->txtprovincia->setText(oCliente2->provincia);
    for(auto i = 0; i< Configuracion_global->paises_model->rowCount();++i)
    {
        if(oCliente2->id_pais == Configuracion_global->paises_model->record(i).value("id").toInt())
        {
            ui->comboPais->setCurrentIndex(i);
            break;
        }
    }
    // Forma de Pago
    for(int i =0;i<Configuracion_global->formapago_model->rowCount();i++)
    {
        if(Configuracion_global->formapago_model->record(i).value("id").toInt() == oCliente2->id_forma_pago)
        {
            int ind_fp = ui->cbo_forma_pago->findText(Configuracion_global->formapago_model->record(i).value(
                                                        "forma_pago").toString());
            ui->cbo_forma_pago->setCurrentIndex(ind_fp);
            break;
        }
        else
        {
            ui->cbo_forma_pago->setCurrentIndex(-1);
        }

    }

    // Recargo de equivalencia
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
    QString error;
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

        for(auto a= 0; a< Configuracion_global->paises_model->rowCount();++a)
        {
            if(Configuracion_global->paises_model->record(a).value("id").toInt() == i.value().value("pais").toInt())
            {
                ui->cboPais_entrega->setCurrentIndex(a);
                break;
            }
        }
        ui->txtemail_alternativa->setText(i.value().value("email").toString());
        ui->txtcomentarios_alternativa->setPlainText(i.value().value("comentarios").toString());
    }

    // Datos fiscales
    ui->txtcif->setText(oCliente2->cif_nif);    
    ui->chkrecargo_equivalencia->setChecked(oCliente2->lIRPF);
    oAlbaran->recargo_equivalencia = (oCliente2->lIRPF);

    oAlbaran->id_cliente = oCliente2->id;

    for(int i =0;i<Configuracion_global->divisas_model->rowCount();i++)
    {
        if(Configuracion_global->divisas_model->record(i).value("id").toInt() == oCliente2->id_divisa)
        {
            int ind_divisa = ui->cboDivisa->findText(Configuracion_global->divisas_model->record(i).value("moneda").toString());
            ui->cboDivisa->setCurrentIndex(ind_divisa);
            break;
        }
        else
        {
            ui->cboDivisa->setCurrentIndex(0);
        }
    }

    for (auto a=0; a< Configuracion_global->agentes_model->rowCount();a++)
    {
        if(oCliente2->id_agente == Configuracion_global->agentes_model->record(a).value("id").toInt())
        {
            ui->cboAgente->setCurrentIndex(a);
            break;
        }
    }
}

void FrmAlbaran::VaciarCampos() {
    ui->lblCliente->setText("");
    ui->lblAlbaran->setText("");
    QDate fecha;
    ui->txtcodigo_cliente->setText("");
    ui->txtalbaran->setText("");
    ui->cboDivisa->setCurrentIndex(-1);
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
    ui->txtbase_total->setText("0,00");
    ui->txtiva_total->setText("0,00");
    ui->txttotal_total->setText("0,00");
    ui->lbimpreso->setVisible(false);
    ui->lbfacturado->setVisible(false);

    ui->txtcomentario->setText("");
    ui->txtbase1->setText(0);
    ui->txtbase2->setText(0);
    ui->txtbase3->setText(0);
    ui->txtbase4->setText(0);
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
    ui->txtrec->setText(0);
    ui->txtentregado_a_cuenta->setText("0,00");
    ui->txtpedido_cliente->setText("");
    ui->txttotal_iva_2->setText("0,00");
    ui->txtbase_total_2->setText("0,00");
    ui->txtrec_2->setText("0,00");
    ui->txttotal_2->setText("0,00");
    ui->txtsubtotal->setText("0,00");
    ui->cboSerie->setCurrentIndex(0);
    serie = ui->cboSerie->currentText();
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
    ui->btnImprimir->setEnabled(true);

    m_busqueda->block(!state);
}

void FrmAlbaran::LLenarAlbaran()
{
    oAlbaran->id_agente = Configuracion_global->agentes_model->record(ui->cboAgente->currentIndex()).value("id").toInt();
    oAlbaran->serie = ui->cboSerie->currentText();
    oAlbaran->codigo_cliente= (ui->txtcodigo_cliente->text());
    oAlbaran->albaran= ui->txtalbaran->text();
    oAlbaran->id_divisa = Configuracion_global->divisas_model->record(ui->cboDivisa->currentIndex()).value("id").toInt();
    oAlbaran->fecha= (ui->txtfecha->date());
    oAlbaran->fecha_factura= (ui->txtfecha_factura->date());
    oAlbaran->cliente= (ui->txtcliente->text());
    oAlbaran->direccion1= (ui->txtdireccion1->text());
    oAlbaran->direccion2= (ui->txtdireccion2->text());
    oAlbaran->cp= (ui->txtcp->text());
    oAlbaran->poblacion= (ui->txtpoblacion->text());
    oAlbaran->provincia= (ui->txtprovincia->text());
    oAlbaran->pais= (ui->comboPais->currentText());

    for(int i =0;i<Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("pais").toString() == ui->comboPais->currentText())
        {
            oAlbaran->id_pais = Configuracion_global->paises_model->record(i).value("id").toInt();
            break;
        }
    }
    oAlbaran->direccion1_entrega= (ui->txtDireccion1_entrega->text());
    oAlbaran->direccion2_entrega= (ui->txtDireccion2_entrega->text());
    oAlbaran->cp_entrega = (ui->txtCp_entrega->text());
    oAlbaran->poblacion_entrega= (ui->txtPoblacion_entrega->text());
    oAlbaran->provincia_entrega= (ui->txtProvincia_entrega->text());

    for(int i =0;i<Configuracion_global->paises_model->rowCount();i++)
    {
        if(Configuracion_global->paises_model->record(i).value("pais").toString() == ui->cboPais_entrega->currentText())
        {
            oAlbaran->id_pais_entrega = Configuracion_global->paises_model->record(i).value("id").toInt();
            break;
        }
    }
    oAlbaran->email_entrega = ui->txtemail_alternativa->text();
    oAlbaran->comentarios_entrega = ui->txtcomentarios_alternativa->toPlainText();
    oAlbaran->cif= (ui->txtcif->text());
    oAlbaran->subtotal= (Configuracion_global->MonedatoDouble(ui->txtsubtotal->text()));
    oAlbaran->dto= (Configuracion_global->MonedatoDouble(ui->txtimporte_descuento->text()));
    oAlbaran->base_total= (Configuracion_global->MonedatoDouble(ui->txtbase_total->text()));
    oAlbaran->iva_total= (Configuracion_global->MonedatoDouble(ui->txtiva_total->text()));
    oAlbaran->total_albaran= (Configuracion_global->MonedatoDouble(ui->txttotal_total->text()));
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
    oAlbaran->rec_total= (Configuracion_global->MonedatoDouble(ui->txtrec->text()));
    oAlbaran->dto = Configuracion_global->MonedatoDouble(ui->txtimporte_descuento->text());

    oAlbaran->recargo_equivalencia = ui->chkrecargo_equivalencia->isChecked();
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
    oAlbaran->id_forma_pago = Configuracion_global->formapago_model->record(ui->cbo_forma_pago->currentIndex()).value("id").toInt();
    oAlbaran->telefono= ui->txtTelefonoCli->text();
    oAlbaran->fax = ui->txtFaxCli->text();
    oAlbaran->movil = ui->txtMovilCli->text();
    oAlbaran->email = ui->txtEmailCli->text();

    oAlbaran->irpf = ui->txtimporte_irpf->text().toDouble();
    oAlbaran->porc_irpf =ui->spinporc_irpf->value();
}

void FrmAlbaran::on_btnSiguiente_clicked()
{
    if(oAlbaran->RecuperarAlbaran("Select * from cab_alb where albaran >'"+oAlbaran->albaran+"' order by albaran  limit 1 "))
    {
        LLenarCampos();
        ui->btn_borrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
        ui->btnFacturar->setEnabled(true);
        ui->btnAnterior->setEnabled(true);
        ui->btnImprimir->setEnabled(true);
        ui->btnBuscar->setEnabled(true);
    }
    else
    {
        TimedMessageBox::Box(this,tr("Se ha llegado al final del archivo.\n"
                                                          "No hay mas albaranes disponibles"));
        VaciarCampos();
        ui->btn_borrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btnFacturar->setEnabled(false);
        ui->btnSiguiente->setEnabled(false);
        oAlbaran->id++;
    }
    refrescar_modelo();
}

void FrmAlbaran::on_btnAnterior_clicked()
{
    if(oAlbaran->RecuperarAlbaran("Select * from cab_alb where albaran <'"+oAlbaran->albaran+"' order by albaran desc limit 1 "))
    {
        LLenarCampos();
        ui->btn_borrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
        ui->btnFacturar->setEnabled(true);
        ui->btnSiguiente->setEnabled(true);
    }
    else
    {
        TimedMessageBox::Box(this,tr("Se ha llegado al final del archivo.\n"
                                                          "No hay mas presupuestos disponibles"));
        VaciarCampos();
        ui->btn_borrar->setEnabled(false);
        ui->btnEditar->setEnabled(false);
        ui->btnFacturar->setEnabled(false);
        ui->btnAnterior->setEnabled(false);
        oAlbaran->id = -1;
    }
    refrescar_modelo();
}

void FrmAlbaran::on_btnAnadir_clicked()
{
    Configuracion_global->empresaDB.transaction();
    Configuracion_global->groupDB.transaction();
    if(Configuracion_global->contabilidad)
        Configuracion_global->contaDB.transaction();

    in_edit = false;
    VaciarCampos();    
    LLenarAlbaran();
    int id =oAlbaran->AnadirAlbaran(serie);
    oAlbaran->RecuperarAlbaran(QString("select * from cab_alb where id = %1").arg(id));
    refrescar_modelo();
    ui->stackedWidget->setCurrentIndex(0);
    ocultarBusqueda();
    LLenarCampos();
    ui->txtalbaran->setText(oAlbaran->albaran);
    BloquearCampos(false);
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
    if(ui->stackedWidget->currentIndex()==1)
    {
        if(!ui->table2->currentIndex().isValid())
            return;
        int id = m->record(ui->table2->currentIndex().row()).value("id").toInt();
        oAlbaran->RecuperarAlbaran(QString("Select * from cab_alb where id=%1 order by albaran  limit 1").arg(id));
    }
    in_edit = true;
    if (oAlbaran->editable == true)
    {
        Configuracion_global->empresaDB.transaction();
        Configuracion_global->groupDB.transaction();
        if(Configuracion_global->contabilidad)
            Configuracion_global->contaDB.transaction();
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
    LLenarAlbaran();
    bool succes = oAlbaran->GuardarAlbaran(oAlbaran->id);
    if(succes)
    {
        LLenarCampos();
        succes  = Configuracion_global->empresaDB.commit();
        succes &= Configuracion_global->groupDB.commit();
        if(Configuracion_global->contabilidad)
            succes &=Configuracion_global->contaDB.commit();
    }
    if(succes)
    {
        oAlbaran->RecuperarAlbaran(QString("select * from cab_alb where id = %1").arg(oAlbaran->id));
        LLenarCampos();
        TimedMessageBox::Box(this,tr("Albarán guardado con éxito"));
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
    int id = oAlbaran->id;
    if(ui->stackedWidget->currentIndex()==1)
    {
        if(!ui->table2->currentIndex().isValid())
            return;
        id = m->record(ui->table2->currentIndex().row()).value("id").toInt();
    }
    if (QMessageBox::question(this,tr("Borrar"),
                              tr("Esta acción no se puede deshacer.\n¿Desea continuar?"),
                              tr("Cancelar"),
                              tr("&Continuar"))== QMessageBox::Accepted)
    {
        bool succes = true;        
        Configuracion_global->empresaDB.transaction();
        QSqlQuery q(Configuracion_global->empresaDB);

        if (QMessageBox::question(this,tr("Borrar"),
                                  tr("¿Desea devolver el stock al almacén?"),
                                  tr("No"),
                                  tr("&Sí"))== QMessageBox::Accepted)
        {
            QString error;
            QMap<int, QSqlRecord> lines = SqlCalls::SelectRecord("lin_alb",QString("id_cab = %1").arg(id),Configuracion_global->empresaDB,error);
            QMapIterator<int, QSqlRecord> _it(lines);
            while(_it.hasNext() && succes)
            {
                _it.next();
                QSqlRecord _it_r = _it.value();
                succes &= Articulo::acum_devolucion_cli(_it_r.value("id_articulo").toInt(),_it_r.value("cantidad").toFloat(),
                                                       _it_r.value("total").toDouble(),QDate::currentDate());
            }
        }
        q.prepare("DELETE FROM lin_alb WHERE id_cab = "+QString::number(id));
        succes &= q.exec();


        q.prepare("DELETE FROM cab_alb WHERE id = "+QString::number(id));
        succes &= q.exec();

        if(succes)
        {
            succes  = Configuracion_global->empresaDB.commit();
            succes &= Configuracion_global->groupDB.commit();
            if(Configuracion_global->contabilidad)
                succes &=Configuracion_global->contaDB.commit();
        }

        if(succes)
        {
            TimedMessageBox::Box(this,tr("Albarán borrado con exito"));
            VaciarCampos();
            oAlbaran->id = -1;
            on_btnSiguiente_clicked();
        }
        else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
            if(Configuracion_global->contabilidad)
                Configuracion_global->contaDB.rollback();
            TimedMessageBox::Box(this,tr("Error al borrar albarán"),TimedMessageBox::Critical);            
        }
    }
}


void FrmAlbaran::on_btndeshacer_clicked()
{
    Configuracion_global->empresaDB.rollback();
    Configuracion_global->groupDB.rollback();
    if(Configuracion_global->contabilidad)
        Configuracion_global->contaDB.rollback();
    LLenarCampos();
    BloquearCampos(true);
    emit unblock();
}

void FrmAlbaran::on_btnImprimir_clicked()
{
    if(ui->stackedWidget->currentIndex()==1)
    {
        if(!ui->table2->currentIndex().isValid())
            return;
        int id = m->record(ui->table2->currentIndex().row()).value("id").toInt();
        oAlbaran->RecuperarAlbaran(QString("Select * from cab_alb where id=%1 order by albaran  limit 1").arg(id));
    }
    FrmDialogoImprimir dlg_print(this);
    dlg_print.set_email(oCliente2->email);
    dlg_print.set_preview(false);
    if(dlg_print.exec() == dlg_print.Accepted)
    {


      //  ui->lblFacturaImpresa->setVisible(true);
        int valor = dlg_print.get_option();
        QMap <QString,QString> parametros_sql;
        parametros_sql["General.empresas"] = QString("id = %1").arg(Configuracion_global->idEmpresa);
        parametros_sql["Empresa.cab_alb"] = QString("id = %1").arg(oAlbaran->id);
        parametros_sql["Empresa.lin_alb"] = QString("id_cab = %1").arg(oAlbaran->id);
        QString report = "albaran_"+QString::number(oCliente2->ididioma);

        QString pdfname = QString("Albaran-%1-%2").arg(oAlbaran->serie).arg(oAlbaran->albaran);
        QString asunto = tr("Envio de albarán %1").arg(oAlbaran->albaran);
        QString texto = tr("Estimado/a %1:\n"
                         "Le adjunto el albarán nº %2/%3.\n\n"
                         "Un saludo.").arg(oCliente2->nombre_fiscal).arg(oAlbaran->serie).arg(oAlbaran->albaran);
        QMap <QString,QString> parametros;
        //TODO parametros
        switch (valor) {
        case 1: // Impresora
            Configuracion::ImprimirDirecto(report,parametros_sql,parametros);
            break;
        case 2: // email
            Configuracion::EviarMail(report,parametros_sql,parametros,pdfname,dlg_print.get_email(),oCliente2->nombre_fiscal,asunto,texto);
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
    ui->table2->setColumnHidden(0,true);
    QStringList header2;
    QVariantList sizes2;
    header2 << tr("id") << tr("Código") << tr("Descripción") << tr("cantidad") << tr("pvp") << tr("pvp recom.") << tr("Subtotal");
    header2 << tr("porc_dto") << tr("porc_iva") << tr("Total");
    sizes2 << 0 << 100 << 300 << 100 << 100 <<100 << 100 <<100 << 100 <<110;
    for(int i = 0; i <header2.size();i++)
    {
        ui->Lineas->setColumnWidth(i,sizes2.at(i).toInt());
        modelLineas->setHeaderData(i,Qt::Horizontal,header2.at(i));
    }
}

void FrmAlbaran::filter_table(QString texto, QString orden, QString modo)
{
    if(!__init)
        return;
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
            "%' and serie = '"+this->serie+"'  and ejercicio ="+Configuracion_global->cEjercicio+ " order by "+order +" "+modo;
    m->setQuery(cSQL,Configuracion_global->empresaDB);
    //ui->table2->selectRow(0);
}

void FrmAlbaran::setUpBusqueda()
{
    m_busqueda = new BarraBusqueda(this);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_Hover);

    cboseries = new QComboBox(this);
    series = new QSqlQueryModel(this);
    ui->cboSerie->setModel(series);
    cboseries->setModel(series);
    cboseries->setCurrentIndex(0);

    QStringList orden;
    orden <<tr("Albarán")<<tr("Fecha")<<tr("Cif")<<tr("Cliente");
    m_busqueda->setOrderCombo(orden);

    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    m_busqueda->setModeCombo(modo);

    connect(m_busqueda,SIGNAL(showMe()),this,SLOT(mostrarBusqueda()));
    connect(m_busqueda,SIGNAL(hideMe()),this,SLOT(ocultarBusqueda()));
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

    QPushButton* borrar = new QPushButton(QIcon(":/Icons/PNG/borrar.png"),tr("Borrar Albarán"),this);
    connect(borrar,SIGNAL(clicked()),this,SLOT(on_btn_borrar_clicked()));
    m_busqueda->addWidget(borrar);
    this->orden = "Albarán";

    if(userLevelInModule() == MayaModule::Administrador)
    {
        QPushButton* Forzar_edicion = new QPushButton(QIcon(":/Icons/PNG/abrecaja.png"),tr("Forzar edición"),this);
        connect(Forzar_edicion,SIGNAL(clicked()),this,SLOT(on_btnForzar_edicion_clicked()));
        m_busqueda->addWidget(Forzar_edicion);
    }

    m_busqueda->addSpacer();
    QPushButton* list = new QPushButton(QIcon(":/Icons/PNG/reports.png"),tr("Listados"),this);
    connect(list,SIGNAL(clicked()),this,SLOT(listados()));
    m_busqueda->addWidget(list);

    connect(m_busqueda,SIGNAL(key_Down_Pressed()),ui->table2,SLOT(setFocus()));   
}

void FrmAlbaran::on_cboseries_currentIndexChanged(const QString &arg1)
{
    this->serie = arg1;
    filter_table(this->texto,this->orden,this->modo);
}

void FrmAlbaran::refrescar_modelo()
{
    if(!__init)
        return;
    modelLineas->setQuery(QString("select id,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                              "from lin_alb where id_cab = %1;").arg(oAlbaran->id),Configuracion_global->empresaDB);
    calcular_albaran();
}

void FrmAlbaran::calcular_albaran()
{
    if(!__init)
        return;
    double subtotal,dto,dtopp,base,iva,re,total;
    double base1,iva1,re1,total1;
    double base2,iva2,re2,total2;
    double base3,iva3,re3,total3;
    double base4,iva4,re4,total4;
    base1= 0; base2 = 0; base3 =0;base4 = 0;
    iva1=0; iva2 =0; iva3=0; iva4 = 0;
    re1=0;re2=0;re3=0;re4 =0;
    total1 =0; total2 =0;total3=0; total4=0;
    subtotal =0; dto=0; dtopp =0; base =0; iva =0; re =0; total =0;

    double _dtoPP = 1-((ui->spinPorc_dto_pp->value()+ui->spinporc_Dto->value())/100);

    QMap <int,QSqlRecord> l;
    QString error;
    l = SqlCalls::SelectRecord("lin_alb",QString("id_cab=%1").arg(oAlbaran->id),Configuracion_global->empresaDB,error);
    QMapIterator <int,QSqlRecord> li(l);
    while(li.hasNext())
    {
        li.next();
        subtotal += li.value().value("subtotal").toDouble();
        // base1
        if(li.value().value("porc_iva").toFloat() == ui->txtporc_iva1->text().toFloat())
            base1 += li.value().value("total").toDouble()* _dtoPP;

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
    if(ui->chkrecargo_equivalencia->isChecked())
    {
        re1 = base1 * (ui->txtporc_rec1->text().toFloat()/100);
        re2 = base2 * (ui->txtporc_rec2->text().toFloat()/100);
        re3 = base3 * (ui->txtporc_rec3->text().toFloat()/100);
        re4 = base4 * (ui->txtporc_rec4->text().toFloat()/100);
    }

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

    if(ui->cboporc_iva_gasto1->currentText().toFloat()  == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat()  == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat()  == ui->txtporc_iva4->text().toFloat())
        base4 += ui->SpinGastoDist3->value();
    iva4 = base4 * (ui->txtporc_iva4->text().toFloat()/100);    

    // TOTALES GENERALES
    dtopp = subtotal *(ui->spinPorc_dto_pp->value()/100.0);
    double _dto = subtotal *(ui->spinporc_Dto->value()/100.0);
    base = base1 + base2+base3+base4;
    iva = iva1 + iva2 +iva3+iva4;
    re  = re1 +re2 + re3 + re4;
    double irpf = base *(ui->spinporc_irpf->value()/100.0);

    total = (base - irpf) + iva +re;
    total1 = base1 +iva1 +re1;
    total2 = base2 +iva2 +re2;
    total3 = base3 +iva3 +re3;
    total4 = base4 +iva4 +re4;

    ui->txtimporte_irpf->setText     (Configuracion_global->toFormatoMoneda(QString::number(irpf ,'f',2)));
    ui->txtDtoPP->setText            (Configuracion_global->toFormatoMoneda(QString::number(dtopp,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(_dto ,'f',Configuracion_global->decimales_campos_totales)));

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



    this->moneda = moneda;
    ui->txtbase_total->setText (Configuracion_global->toFormatoMoneda(QString::number(base    ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText   (Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva_total->setText  (Configuracion_global->toFormatoMoneda(QString::number(iva     ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec->setText        (Configuracion_global->toFormatoMoneda(QString::number(re      ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_total->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec->setText        (Configuracion_global->toFormatoMoneda(QString::number(re      ,'f',Configuracion_global->decimales_campos_totales)));


    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base           ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda (QString::number(iva            ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtrec_2->setText      (Configuracion_global->toFormatoMoneda (QString::number(re             ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));

}

void FrmAlbaran::calcular_iva_gastos()
{
    ui->spiniva_gasto1->setValue((ui->SpinGastoDist1->value()*(ui->cboporc_iva_gasto1->currentText().toDouble()/100)));
    ui->spiniva_gasto2->setValue((ui->SpinGastoDist2->value()*(ui->cboporc_iva_gasto2->currentText().toDouble()/100)));
    ui->spiniva_gasto3->setValue((ui->SpinGastoDist3->value()*(ui->cboporc_iva_gasto3->currentText().toDouble()/100)));
}

bool FrmAlbaran::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Show && obj == this)
    {
        init_querys();
    }
    else if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(obj == ui->Lineas){
            if (keyEvent->key() == Qt::Key_Plus)
            {
                on_btnAnadirLinea_clicked();
            }
            if(keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
                on_Lineas_doubleClicked(ui->Lineas->currentIndex());

        }
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
        if(keyEvent->key() == Qt::Key_Return)
            if(ui->stackedWidget->currentIndex() == 1)
                on_table2_doubleClicked(ui->table2->currentIndex());
        return false;
    }
    else if(event->type() == QEvent::Resize && obj == this)
    {
        if(!__init)
            init();
        _resizeBarraBusqueda(m_busqueda);
    }
    return MayaModule::eventFilter(obj,event);
}

void FrmAlbaran::on_table2_doubleClicked(const QModelIndex &index)
{
    int id = ui->table2->model()->data(ui->table2->model()->index(index.row(),0),Qt::EditRole).toInt();
    if(oAlbaran->RecuperarAlbaran("select * from cab_alb where id ="+QString::number(id)))
    {
        LLenarCampos();
        ui->stackedWidget->setCurrentIndex(0);
        ui->btnEditar->setEnabled(true);
        ocultarBusqueda();
        refrescar_modelo();
        ui->btn_borrar->setEnabled(true);
        ui->btnEditar->setEnabled(true);
        ui->btnFacturar->setEnabled(true);
        ui->btnSiguiente->setEnabled(true);
        ui->btnAnterior->setEnabled(true);
    }
}

void FrmAlbaran::on_btnBuscar_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    mostrarBusqueda();
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
    if(ui->txtcNumFra->text().isEmpty()||ui->txtcNumFra->text()== "0")
    {
        if(QMessageBox::question(this,tr("Albaranes a clientes"),tr("¿Desea realmente facturar este albarán?"),
                                 tr("No"),tr("Sí"))==QMessageBox::Accepted)
        {
            Configuracion_global->empresaDB.transaction();
            Configuracion_global->groupDB.transaction();
            if(Configuracion_global->contabilidad)
                Configuracion_global->contaDB.transaction();

            bool transaccion = true;

            Factura oFactura(this);

            // LLENAR CABECERA
            QHash <QString, QVariant > cab_fac;

            cab_fac["codigo_cliente"] = oAlbaran->codigo_cliente;
            cab_fac["serie"] = oAlbaran->serie;
            cab_fac["factura"] = oFactura.NuevoNumeroFactura(serie);
            cab_fac["fecha"] = QDate::currentDate();
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
            cab_fac["poblacion_entrega"] = oAlbaran->poblacion_entrega;
            cab_fac["provincia_entrega"] = oAlbaran->provincia_entrega;
            cab_fac["cp_entrega"] = oAlbaran->cp_entrega;
            cab_fac["id_pais_entrega"] = oAlbaran->id_pais_entrega;
            cab_fac["email_entrega"] = oAlbaran->email_entrega;
            cab_fac["comentarios_entrega"] = oAlbaran->comentarios_entrega;
            cab_fac["cif"] = oAlbaran->cif;
            cab_fac["telefono"] = oAlbaran->telefono;
            cab_fac["fax"] = oAlbaran->fax;
            cab_fac["movil"] = oAlbaran->movil;
            cab_fac["email"] = oAlbaran->email;
            cab_fac["recargo_equivalencia"] = oAlbaran->recargo_equivalencia;
            cab_fac["subtotal"] = oAlbaran->subtotal;
            cab_fac["porc_dto"] = oAlbaran->porc_dto;
            cab_fac["porc_dto_pp"] = oAlbaran->porc_dto_pp;
            cab_fac["dto"] = oAlbaran->dto;
            cab_fac["dto_pp"] = oAlbaran->dto_pp;
            cab_fac["base"] = oAlbaran->base_total;
            cab_fac["porc_irpf"] = oAlbaran->porc_irpf;
            cab_fac["irpf"] = oAlbaran->irpf;
            cab_fac["iva"] = oAlbaran->iva_total;
            cab_fac["total"] = oAlbaran->total_albaran;
            cab_fac["impreso"]= oAlbaran->impreso;
            cab_fac["cobrado"] = false;
            cab_fac["contabilizado"] = false;
            cab_fac["id_forma_pago"] = oAlbaran->id_forma_pago;
            //cab_fac["forma_pago"] = oAlbaran->;
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
            cab_fac["importe_pendiente"] = oAlbaran->total_albaran;
            cab_fac["codigo_entidad"] = oCliente2->entidad_bancaria;
            cab_fac["oficina_entidad"] = oCliente2->oficina_bancaria;
            cab_fac["dc_cuenta"] = oCliente2->dc;
            cab_fac["cuenta_corriente"] = oCliente2->cuenta_corriente;
            cab_fac["desc_gasto1"] = oAlbaran->desc_gasto1;
            cab_fac["desc_gasto2"] = oAlbaran->desc_gasto2;
            cab_fac["desc_gasto3"] = oAlbaran->desc_gasto3;
            cab_fac["imp_gasto1"] = oAlbaran->imp_gasto1;
            cab_fac["imp_gasto2"] = oAlbaran->imp_gasto2;
            cab_fac["imp_gasto3"] = oAlbaran->imp_gasto3;
            cab_fac["porc_iva_gasto1"] = oAlbaran->porc_iva_gasto1;
            cab_fac["porc_iva_gasto2"] = oAlbaran->porc_iva_gasto2;
            cab_fac["porc_iva_gasto3"] = oAlbaran->porc_iva_gasto3;
            cab_fac["iva_gasto1"] = oAlbaran->iva_gasto1;
            cab_fac["iva_gasto2"] = oAlbaran->iva_gasto2;
            cab_fac["iva_gasto3"] = oAlbaran->iva_gasto3;
            cab_fac["id_transportista"] = oAlbaran->id_transportista;
            cab_fac["ejercicio"] = Configuracion_global->cEjercicio.toInt();
            cab_fac["editable"] = true;
            cab_fac["pendiente"] = oAlbaran->total_albaran;
            cab_fac["id_divisa"] = oAlbaran->id_divisa;
            cab_fac["id_agente"] = oAlbaran->id_agente;
            QString error;

            int new_id = SqlCalls::SqlInsert(cab_fac,"cab_fac",Configuracion_global->empresaDB,error);

            if(new_id > -1)
            {
                // CARGAR FACTURA
                oFactura.RecuperarFactura(new_id);

                //  INSERTAR LÍNEAS
                QSqlQuery lineas_pre(Configuracion_global->empresaDB);

                if(lineas_pre.exec("Select * from lin_ped where id_cab ="+QString::number(oAlbaran->id)))
                {
                    QHash <QString, QVariant> h_lineas_fac;
                    while (lineas_pre.next())
                    {

                        h_lineas_fac["id_cab"] = new_id;
                        h_lineas_fac["id_articulo"] = lineas_pre.record().value("id_articulo").toInt();
                        h_lineas_fac["codigo"] = lineas_pre.record().value("codigo").toString();
                        h_lineas_fac["cantidad"] =  lineas_pre.record().value("cantidad").toInt();
                        h_lineas_fac["descripcion"] = lineas_pre.record().value("descripcion").toString();
                        h_lineas_fac["precio"] = lineas_pre.record().value("precio").toDouble();
                        h_lineas_fac["subtotal"] = lineas_pre.record().value("subtotal").toDouble();
                        h_lineas_fac["porc_dto"] = lineas_pre.record().value("porc_dto").toDouble();
                        h_lineas_fac["dto"] = lineas_pre.record().value("dto").toDouble();
                        h_lineas_fac["porc_iva"] = lineas_pre.record().value("porc_iva").toDouble();
                        h_lineas_fac["total"] = lineas_pre.record().value("total").toDouble();

                        int new_id = SqlCalls::SqlInsert(h_lineas_fac,"lin_fac",Configuracion_global->empresaDB,error);

                        if(new_id == -1)
                        {
                            QMessageBox::warning(this,tr("Pedidos cliente"),
                                                 tr("Ocurrió un error al crear las líneas de factura:\n%1").arg(error));

                            transaccion = false;
                        }
                    }
                }

                if(transaccion)
                {
                    // Insertamos datos factura en pedido
                    oAlbaran->factura = QString("%1/%2").arg(oFactura.serie).arg(oFactura.factura);
                    oAlbaran->editable = false;
                    oAlbaran->fecha_factura = QDate::currentDate();
                    oAlbaran->GuardarAlbaran(oAlbaran->id);
                    ui->txtcNumFra->setText(oAlbaran->factura);

                    vencimientos vto(this);

                    // Asiento contable
                    bool creado = true;
                    if(Configuracion_global->contabilidad)
                    {

                        oCliente2->Recuperar("select * from clientes where id = "+QString::number(oFactura.id_cliente));
                        if(oFactura.apunte == 0)
                            creado = oFactura.Apunte();
                        else
                            creado = oFactura.EditApunte(oFactura.apunte);
                    }

                    // Vencimiento
                    if(creado && vto.calcular_vencimiento(oFactura.fecha,oFactura.id_forma_pago,oFactura.id_cliente,0,oFactura.id,(oFactura.serie+"/"+oFactura.factura),1,
                                             true,oFactura.total))
                    {
                        QString texto;
                        texto = tr("Se ha creado una nueva factura.\ncon el número ")+ cab_fac.value("factura").toString()+
                                tr("\n y de importe: ")+QString::number(cab_fac.value("total").toDouble(),'f',Configuracion_global->decimales);
                        TimedMessageBox::Box(this,texto);

                        bool commited = Configuracion_global->empresaDB.commit();
                        commited &= Configuracion_global->groupDB.commit();
                        if(Configuracion_global->contabilidad)
                            commited &= Configuracion_global->contaDB.commit();
                        oAlbaran->RecuperarAlbaran("select * from cab_alb where id ="+QString::number(oAlbaran->id));
                        LLenarCampos();
                    }
                    else
                    {
                        Configuracion_global->empresaDB.rollback();
                        Configuracion_global->groupDB.rollback();
                        if(Configuracion_global->contabilidad)
                            Configuracion_global->contaDB.rollback();
                        QMessageBox::warning(this,tr("Traspaso a factura"),tr("No se pudo crear la factura "),tr("Aceptar"));
                    }

                }
                else
                {
                    Configuracion_global->empresaDB.rollback();
                    Configuracion_global->groupDB.rollback();
                    if(Configuracion_global->contabilidad)
                        Configuracion_global->contaDB.rollback();
                    QMessageBox::warning(this,tr("Traspaso a factura"),tr("No se pudo crear la factura "),tr("Aceptar"));
                }
            }
            else
            {
                Configuracion_global->empresaDB.rollback();
                Configuracion_global->groupDB.rollback();
                if(Configuracion_global->contabilidad)
                    Configuracion_global->contaDB.rollback();
                QMessageBox::warning(this,tr("Pedidos"),
                                     tr("No se ha podido crear la factura"),tr("Aceptar"));
            }
        }
    }
    else
    {
        TimedMessageBox::Box(this,tr("Este pedido ya ha sido facturado: %1").arg(oAlbaran->factura));
    }
}

void FrmAlbaran::mostrarBusqueda()
{
    ui->stackedWidget->setCurrentIndex(1);
    _showBarraBusqueda(m_busqueda);
    m_busqueda->doFocustoText();
}

void FrmAlbaran::ocultarBusqueda()
{
    _hideBarraBusqueda(m_busqueda);
}

void FrmAlbaran::on_btnAnadirLinea_clicked()
{
    if(ui->btnGuardar->isEnabled())
    {
            frmEditLine frmeditar(this);
            frmeditar.init();
            connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(refrescar_modelo()));

            frmeditar.setUse_re(ui->chkrecargo_equivalencia->isChecked());

            frmeditar.set_acumula(true);
            frmeditar.set_linea(0,"lin_alb");
            frmeditar.set_tabla("lin_alb");
            frmeditar.set_id_cliente(oCliente2->id);
            frmeditar.set_id_tarifa(oCliente2->idTarifa);
            frmeditar.set_id_cab(oAlbaran->id);
            frmeditar.set_venta(true);
            if(!frmeditar.exec() == QDialog::Accepted)
                modelLineas->setQuery(QString("select id,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                                      "from lin_alb where id_cab = %1;").arg(oAlbaran->id),Configuracion_global->empresaDB);
                calcular_albaran();

    } else{
        QMessageBox::warning(this,tr("Gestión de albaranes"),tr("Debe editar el albarán para añadir líneas"),
                             tr("Aceptar"));
    }

}

void FrmAlbaran::on_Lineas_doubleClicked(const QModelIndex &index)
{
    if(ui->btnGuardar->isEnabled())
    {
        int id_lin = ui->Lineas->model()->data(index.model()->index(index.row(),0)).toInt();
        if(id_lin >0)
        {
            frmEditLine frmeditar(this);
            frmeditar.init();
            connect(&frmeditar,SIGNAL(refrescar_lineas()),this,SLOT(refrescar_modelo()));
            frmeditar.setUse_re(ui->chkrecargo_equivalencia->isChecked());
            frmeditar.set_acumula(true);
            frmeditar.set_id_cliente(oCliente2->id);
            frmeditar.set_id_tarifa(oCliente2->idTarifa);
            frmeditar.set_id_cab(oAlbaran->id);
            frmeditar.set_venta(true);
            frmeditar.set_linea(id_lin,"lin_alb");
            frmeditar.set_tabla("lin_alb");
            frmeditar.set_editando();
            frmeditar.exec();

            //refrescar_modelo();
            calcular_albaran();
            ui->Lineas->setFocus();

        }
    }else
    {
        QMessageBox::warning(this, tr("Gestión de albaranes"),tr("Debe editar la albarán para poder modificar las líneas"),
                             tr("Aceptar"));

    }

}

void FrmAlbaran::on_btn_borrarLinea_clicked()
{
    if(QMessageBox::question(this,tr("Lineas de albarán"), tr("¿Borrar la linea?"),
                             tr("No"),tr("Borrar")) == QMessageBox::Accepted)
    {
        QModelIndex index = ui->Lineas->currentIndex();
        int id_lin = ui->Lineas->model()->index(index.row(),0).data().toInt();
        //TODO .
        oAlbaran->borrar_linea(id_lin);
        modelLineas->setQuery(QString("select id,codigo,descripcion,cantidad,precio,precio_recom,subtotal,porc_dto,porc_iva,total "
                                      "from lin_alb where id_cab = %1;").arg(oAlbaran->id),Configuracion_global->empresaDB);
        calcular_albaran();
        ui->Lineas->setFocus();

    }
}

void FrmAlbaran::on_SpinGastoDist1_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    calcular_iva_gastos();
    calcular_albaran();
}

void FrmAlbaran::on_SpinGastoDist2_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    calcular_iva_gastos();
    calcular_albaran();
}

void FrmAlbaran::on_SpinGastoDist3_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    calcular_iva_gastos();
    calcular_albaran();
}

void FrmAlbaran::on_cboporc_iva_gasto1_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    calcular_iva_gastos();
    calcular_albaran();
}

void FrmAlbaran::on_cboporc_iva_gasto2_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    calcular_iva_gastos();
    calcular_albaran();
}

void FrmAlbaran::on_cboporc_iva_gasto3_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    calcular_iva_gastos();
    calcular_albaran();
}

void FrmAlbaran::on_spinporc_Dto_valueChanged(double /*arg1*/)
{
    calcular_albaran();
}

void FrmAlbaran::on_spinPorc_dto_pp_valueChanged(double /*arg1*/)
{
    calcular_albaran();
}

void FrmAlbaran::on_spinporc_irpf_valueChanged(double /*arg1*/)
{
    calcular_albaran();
}

void FrmAlbaran::on_chkrecargo_equivalencia_toggled(bool checked)
{
    if (checked) {
        ui->chkrecargo_equivalencia->setChecked(true);
        oAlbaran->recargo_equivalencia = 1;
        ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
        ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
        ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
        ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));
        oAlbaran->porc_rec1 = ui->txtporc_rec1->text().toFloat();
        oAlbaran->porc_rec2 = ui->txtporc_rec2->text().toFloat();
        oAlbaran->porc_rec3 = ui->txtporc_rec3->text().toFloat();
        oAlbaran->porc_rec4 = ui->txtporc_rec4->text().toFloat();
    } else {
        ui->chkrecargo_equivalencia->setChecked(false);
        oAlbaran->recargo_equivalencia = 0;
        ui->txtporc_rec1->setText("0.00");
        ui->txtporc_rec2->setText("0.00");
        ui->txtporc_rec3->setText("0.00");
        ui->txtporc_rec4->setText("0.00");
        oAlbaran->porc_rec1 = 0;
        oAlbaran->porc_rec2 = 0;
        oAlbaran->porc_rec3 = 0;
        oAlbaran->porc_rec4 = 0;
    }
    QString error;
    for(auto i = 0; i<modelLineas->rowCount();++i)
    {
        QSqlRecord r = modelLineas->record(i);
        QHash<QString,QVariant> lin;
        double iva_art = r.value("porc_iva").toDouble();
        double porc_re_art;

        if(iva_art == oAlbaran->porc_iva1)
            porc_re_art = oAlbaran->porc_rec1;
        if(iva_art == oAlbaran->porc_iva2)
            porc_re_art = oAlbaran->porc_rec2;
        if(iva_art == oAlbaran->porc_iva3)
            porc_re_art = oAlbaran->porc_rec3;
        if(iva_art == oAlbaran->porc_iva4)
            porc_re_art = oAlbaran->porc_rec4;

        lin["porc_rec"] = porc_re_art;
        lin["rec"] = r.value("total").toDouble() * (porc_re_art/100.0);

        if(!SqlCalls::SqlUpdate(lin,"lin_alb",Configuracion_global->empresaDB,QString("id=%1").arg(r.value("id").toInt()),error))
        {
            QMessageBox::critical(this,tr("Error al actualizar lineas"),error);
            break;
        }
    }
    calcular_albaran();
}

void FrmAlbaran::listados()
{
    frmListadoAlbaran d(this);
    d.exec();
}
