#include "frmpedidosproveedor.h"
#include "ui_frmpedidosproveedor.h"
#include "../Busquedas/db_consulta_view.h"
#include "../Auxiliares/frmdialogoimprimir.h"
#include "../Auxiliares/frmaddentregascuenta.h"
#include "../Auxiliares/entregascuenta.h"
#include <QPrintDialog>


FrmPedidosProveedor::FrmPedidosProveedor(QWidget *parent, bool showCerrar) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmPedidosProveedor),
    helper(this),
    prov(this),
    toolButton(tr("Pedidos \nProv."),":/Icons/PNG/pedido_pro.png",this),
    menuButton(QIcon(":/Icons/PNG/pedido_pro.png"),tr("Pedidos Prov."),this),
    shortCut(new QPushButton(QIcon(":/Icons/PNG/pedido_pro.png"),"",this))
{
    ui->setupUi(this);

    oPedido_proveedor = new PedidoProveedor(this);
    bloquearcampos(true);
    ui->lbimpreso->setVisible(false);
    ui->lblnombreProveedor->clear();
    ui->lblnumero_pedido->clear();
    shortCut->setToolTip(tr("Gestión de pedidos a proveedor"));
    shortCut->setStyleSheet("background-color: rgb(133, 170, 142)");

    //oProveedor = new Proveedor(this);
    ui->combo_pais->setModel(Configuracion_global->paises_model);
    //ui->combo_pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    helper.set_Tipo(true);
    helper.help_table(ui->Lineas);
    helper.set_tarifa(1);
    helper.setDatabase("empresa","lin_ped_pro");
    helper.setidHeader(1);
    helper.blockTable(true);

    connect(&helper,SIGNAL(lineaReady(lineaDetalle*)),this,SLOT(lineaReady(lineaDetalle*)));
    connect(&helper,SIGNAL(lineaDeleted(int)),this,SLOT(lieaDeleted(int)));


    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(ui->chklporc_rec,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));
    connect(ui->btnAnadir,SIGNAL(clicked()),this,SLOT(anadir_pedido()));
    connect(ui->btnEditar,SIGNAL(clicked()),this,SLOT(editar_pedido()));
    connect(ui->btnGuardar,SIGNAL(clicked()),this,SLOT(guardar_pedido()));
    connect(ui->btnDeshacer,SIGNAL(clicked()),this,SLOT(deshacer()));
    connect(ui->btnSiguiente,SIGNAL(clicked()),this,SLOT(siguiente()));
    connect(ui->btnAnterior,SIGNAL(clicked()),this,SLOT(anterior()));
    connect(ui->btnImprimir,SIGNAL(clicked()),this,SLOT(imprimir()));
    connect(ui->btn_borrar,SIGNAL(clicked()),this,SLOT(borrar_pedido()));

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

    ui->txtiva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtiva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtiva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtiva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));


    ui->btn_cerrar->setVisible(showCerrar);

    //--------------
    // LLenar tabla
    //--------------

    model = new QSqlQueryModel(this);
    model->setQuery("select id,pedido,fecha,recepcion,cif_nif,codigo_proveedor,proveedor from ped_pro where ejercicio = "+
                    Configuracion_global->cEjercicio+" order by pedido desc",Configuracion_global->empresaDB);
    ui->tabla->setModel(model);
    formatotabla();

    //----------------------------
    // lleno combo orden busquedas
    //----------------------------
    QStringList lista;
    lista << tr("pedido") <<tr("fecha") <<tr("recepción") <<tr("codigo_proveedor") <<tr("proveedor");
    lista << tr("cif/nif");
    ui->cboOrden->addItems(lista);

    //----------------------------
    // Combo modo
    //----------------------------
    QStringList modo;
    modo << tr("A-Z") << tr("Z-A");
    ui->cboModo->addItems(modo);


}

FrmPedidosProveedor::~FrmPedidosProveedor()
{
    delete ui;
}

void FrmPedidosProveedor::llenarProveedor(int id, bool isNew)
{
    if(isNew)
    {
        ui->btnAnadir->clicked();
        ui->tabWidget_2->setCurrentIndex(1);
        ui->btnAnadirLinea->clicked();
    }
    prov.Recuperar("Select * from proveedores where id="+QString::number(id),1);
    ui->txtcodigo_proveedor->setText(prov.codigo);
    ui->txtproveedor->setText(prov.proveedor);
    ui->txtdireccion1->setText(prov.direccion1);
    ui->txtdireccion2->setText(prov.direccion2);
    ui->txtpoblacion->setText(prov.poblacion);
    ui->txtprovincia->setText(prov.provincia);
    ui->txtcp->setText(prov.cp);
    ui->txtcif->setText(prov.cif);
    ui->combo_pais->setCurrentText(Configuracion::Devolver_pais(prov.id_pais));
    ui->lblnombreProveedor->setText(prov.proveedor);
    ui->chklporc_rec->setChecked(prov.recargo_equivalencia);
}

void FrmPedidosProveedor::lineaReady(lineaDetalle * ld)
{
    //-----------------------------------------------------
    // Insertamos línea de pedido y controlamos acumulados
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

        QSqlQuery query_lin_ped_pro(Configuracion_global->empresaDB);
        query_lin_ped_pro.prepare("INSERT INTO lin_ped_pro (id_cab,id_articulo,codigo,"
                                  "descripcion, cantidad, precio,subtotal,porc_dto,dto,porc_iva,"
                                  "iva,total,cantidad_pendiente) VALUES (:id_cab,:id_articulo,:codigo,"
                                  ":descripcion,:cantidad,:precio,:subtotal,:porc_dto,:dto,"
                                  ":porc_iva,:iva,:total,:cantidad_pendiente);");
        query_lin_ped_pro.bindValue(":id_cab", oPedido_proveedor->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":cantidad_pendiente",ld->cantidad);
        query_lin_ped_pro.bindValue(":preio",ld->precio);
        query_lin_ped_pro.bindValue(":subtotal",ld->subtotal);
        query_lin_ped_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_ped_pro.bindValue(":dto",ld->dto);
        query_lin_ped_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_ped_pro.bindValue(":iva",0); // importe iva hay que calcularlo
        query_lin_ped_pro.bindValue(":total",ld->total);
        if (!query_lin_ped_pro.exec()){
            ok_empresa = false;
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Ocurrió un error al insertar la nueva línea: %1").arg(query_lin_ped_pro.lastError().text()),
                                 tr("Aceptar"));
        }
        // ---------------------------------
        // Actualización stock y acumulados
        //----------------------------------

        queryArticulos.prepare("update articulos set cantidad_pendiente_recibir = cantidad_pendiente_recibir+:cant_recibir, "
                               " stock_real = stock_real + :cant_recibir2 where codigo=:codigo");
        queryArticulos.bindValue(":cant_recibir",ld->cantidad);
        queryArticulos.bindValue(":cant_recibir2",ld->cantidad);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec() && ok_empresa){
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        } else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
        }

        ld->idLinea = query_lin_ped_pro.lastInsertId().toInt();

    } else
    {
        // --------------------------
        // Descuento unidades pedidas
        //---------------------------
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("update articulos set cantidad_pendiente_recibir = cantidad_pendiente_recibir-:cant_recibir,"
                               "stock_real = stock_real - :cant_recibir2 where codigo=:codigo");
        queryArticulos.bindValue(":cant_recibir",ld->cantidad_old);
        queryArticulos.bindValue(":cant_recibir2",ld->cantidad_old);
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
        QSqlQuery query_lin_ped_pro(Configuracion_global->empresaDB);
        query_lin_ped_pro.prepare("UPDATE lin_ped_pro SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo=:codigo,"
                                  "descripcion =:descripcion,"
                                  "cantidad =:cantidad,"
                                  "cantidad_pendiente =:cantidad_pendiente,"
                                  "precio =:precio,"
                                  "subtotal =:subtotal,"
                                  "porc_dto =:porc_dto,"
                                  "dto =:dto,"
                                  "porc_iva =:porc_iva,"
                                  "iva =:iva,"
                                  "total =:total "
                                  "WHERE id = :id;");

        query_lin_ped_pro.bindValue(":id_cab", oPedido_proveedor->id);
        query_lin_ped_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_ped_pro.bindValue(":codigo",ld->codigo);
        query_lin_ped_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_ped_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_ped_pro.bindValue(":cantidad_pendiente",ld->cantidad);
        query_lin_ped_pro.bindValue(":precio",ld->precio);
        query_lin_ped_pro.bindValue(":subtotal",ld->subtotal);
        query_lin_ped_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_ped_pro.bindValue(":dto",ld->dto);
        query_lin_ped_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_ped_pro.bindValue(":iva",0); // importe iva hay que calcularlo
        query_lin_ped_pro.bindValue(":total",ld->total);
        query_lin_ped_pro.bindValue(":id",ld->idLinea);

        if (!query_lin_ped_pro.exec()) {
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Ocurrió un error al guardar la línea: %1").arg(query_lin_ped_pro.lastError().text()),
                                 tr("Aceptar"));
            ok_empresa = false;
        }
        // ---------------------------------
        // Actualización stock y acumulados
        //----------------------------------
        queryArticulos.prepare("update articulos set cantidad_pendiente_recibir = cantidad_pendiente_recibir + :cant_recibir, "
                               "stock_real = stock_real + :cant_recibir2 where codigo=:codigo");
        queryArticulos.bindValue(":cant_recibir",ld->cantidad);
        queryArticulos.bindValue(":cant_recibir2",ld->cantidad);
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

void FrmPedidosProveedor::lineaDeleted(lineaDetalle * ld)
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
        // --------------------------
        // Descuento unidades pedidas
        //---------------------------
        QSqlQuery queryArticulos(Configuracion_global->groupDB);
        queryArticulos.prepare("update articulos set cantidad_pendiente_recibir = cantidad_pendiente_recibir-:cant_recibir,"
                               "stock_real = stock_real + :cant_recibir2 where codigo=:codigo");
        queryArticulos.bindValue(":cant_recibir",ld->cantidad_old);
        queryArticulos.bindValue(":cant_recibir2",ld->cantidad_old);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if (!queryArticulos.exec())
        {
            ok_Maya = false;
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Se produjo un error al actualizar stock : %1").arg(queryArticulos.lastError().text()));

        }
        //----------------------
        // Borrar línea pedido
        //----------------------
        QSqlQuery querylin_ped_pro(Configuracion_global->empresaDB);
        querylin_ped_pro.prepare("delete from lin_ped_pro where id =:id");
        querylin_ped_pro.bindValue(":id",id);
        if(querylin_ped_pro.exec() && ok_Maya)
        {
            Configuracion_global->empresaDB.commit();
            Configuracion_global->groupDB.commit();
        } else
        {
            Configuracion_global->empresaDB.rollback();
            Configuracion_global->groupDB.rollback();
        }
    }
    //qDebug() << "borra" << id;
    delete ld;
}

void FrmPedidosProveedor::totalChanged(double base, double dto, double subtotal, double iva, double re, double total, QString moneda)
{
    total += iva;
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)+moneda));
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',Configuracion_global->decimales)+moneda));
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales)+moneda));
    ui->txtiva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)+moneda));
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)+moneda));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)+moneda));
    ui->lbl_total->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)+moneda));

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)+moneda));
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)+moneda));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)+moneda));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)+moneda));
    this->moneda = moneda;
}

void FrmPedidosProveedor::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPedidosProveedor::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPedidosProveedor::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda( QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}

void FrmPedidosProveedor::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',Configuracion_global->decimales)));
}


void FrmPedidosProveedor::bloquearcampos(bool estado)
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
    ui->btn_borrar->setEnabled(estado);
    ui->btnDeshacer->setEnabled(!estado);
    ui->btnEditar->setEnabled(estado);
    ui->btnGuardar->setEnabled(!estado);
    ui->btnSiguiente->setEnabled(estado);
    ui->btnAnadirLinea->setEnabled(!estado);

    ui->btn_borrarLinea->setEnabled(!estado);
    ui->botBuscarCliente->setEnabled(!estado);
    helper.blockTable(estado);
    // activo controles que deben estar activos.
    ui->cboOrden->setEnabled(estado);
    ui->cboModo->setEnabled(estado);
    ui->txtBuscar->setReadOnly(!estado);


}

void FrmPedidosProveedor::buscar_proveeedor()
{
//    FrmBuscarProveedor buscar(this);
//    if(buscar.exec() == QDialog::Accepted)
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
        oPedido_proveedor->id_proveedor = id_proveedor;
        llenarProveedor(id_proveedor);
    }
}

void FrmPedidosProveedor::anadir_pedido()
{
    int id = oPedido_proveedor->nuevo_pedido_proveedor();
    clear();
    oPedido_proveedor->recuperar(id);
    this->id = id;
    emit block();
    llenar_campos();
    bloquearcampos(false);
    }

void FrmPedidosProveedor::guardar_pedido()
{
    guardar_campos_en_objeto();
    oPedido_proveedor->id =this->id;
    oPedido_proveedor->guardar();

    int regs = ui->Lineas->rowCount();
   // helper.saveTable(oPedido_proveedor->id,"empresa","lin_ped_pro");
    oPedido_proveedor->recuperar(oPedido_proveedor->id);
    llenar_campos();
    bloquearcampos(true);
    emit unblock();
}

void FrmPedidosProveedor::editar_pedido()
{
    bloquearcampos(false);
    emit block();
    ui->txtcodigo_proveedor->setFocus();
}
void FrmPedidosProveedor::deshacer()
{
    llenar_campos();
    bloquearcampos(true);
    emit unblock();
}

void FrmPedidosProveedor::siguiente()
{
    oPedido_proveedor->recuperar("select * from ped_pro where pedido > "+
                                 QString::number(oPedido_proveedor->pedido)+ " order by pedido limit 0,1",1);
    llenar_campos();
}

void FrmPedidosProveedor::anterior()
{
    oPedido_proveedor->recuperar("select * from ped_pro where pedido < "
                                 +QString::number(oPedido_proveedor->pedido)+ " order by pedido desc  limit 0,1 ",2);
    llenar_campos();
}

void FrmPedidosProveedor::imprimir()
{
    oPedido_proveedor->imprimir(oPedido_proveedor->id);
}

void FrmPedidosProveedor::borrar_pedido()
{
    if(QMessageBox::question(this,tr("Gestión de Pedidos a proveedores"),
                             tr("¿Desea eliminar este pedido?\nEsta opción no se podrá deshacer"),
                             tr("Cancelar"),tr("Aceptar")) == QMessageBox::Accepted)
    {
        bool exito = oPedido_proveedor->borrar(oPedido_proveedor->id);
        if (exito == true)
            clear();


    }
}

void FrmPedidosProveedor::llenar_campos()
{
    this->id =oPedido_proveedor->id;
  //  helper.set_id_cabecera(oPedido_proveedor->id);
    ui->txtpedido->setText(QString::number(oPedido_proveedor->pedido));
  //  ui->lblSerie->setText(QString::number(oPedido_proveedor->ejercicio));
    ui->lblnumero_pedido->setText(QString::number(oPedido_proveedor->pedido));
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
    ui->txtcif->setText(oPedido_proveedor->cif_nif);
    ui->txtbase->setText(QString::number(oPedido_proveedor->base_total));
    ui->txtsubtotal->setText(QString::number(oPedido_proveedor->subtotal));
    ui->txtimporte_descuento->setText(QString::number(oPedido_proveedor->dto));
    ui->txtiva->setText(QString::number(oPedido_proveedor->iva));
    ui->txtporc_rec1->setText(QString::number(oPedido_proveedor->rec_total));
    ui->chklporc_rec->setChecked(oPedido_proveedor->recargo_equivalencia);
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
    QString filter = QString("id_cab = '%1'").arg(oPedido_proveedor->id);
    helper.porc_iva1 = ui->txtporc_iva1->text().toDouble();
    helper.porc_iva2 = ui->txtporc_iva2->text().toDouble();
    helper.porc_iva3 = ui->txtporc_iva3->text().toDouble();
    helper.porc_iva4 = ui->txtporc_iva4->text().toDouble();
    helper.fillTable("empresa","lin_ped_pro",filter);
    helper.resizeTable();
    cargar_tabla_entregas();

}

void FrmPedidosProveedor::guardar_campos_en_objeto()
{
    oPedido_proveedor->id = this->id;
    oPedido_proveedor->pedido = ui->lblnumero_pedido->text().toInt();
   // oPedido_proveedor->ejercicio = ui->lblSerie->text().toInt();
    oPedido_proveedor->proveedor = ui->txtproveedor->text();
    oPedido_proveedor->fecha = ui->txtfecha->date();
    oPedido_proveedor->recepcion =ui->txtfechaRecepcion->date();
    oPedido_proveedor->codigo_proveedor = ui->txtcodigo_proveedor->text();
    oPedido_proveedor->direccion1 = ui->txtdireccion1->text();
    oPedido_proveedor->direccion2 = ui->txtdireccion2->text();
    oPedido_proveedor->cp = ui->txtcp->text();
    oPedido_proveedor->poblacion = ui->txtpoblacion->text();
    oPedido_proveedor->provincia = ui->txtprovincia->text();
    oPedido_proveedor->cif_nif = ui->txtcif->text();
    oPedido_proveedor->base_total = ui->txtbase_total_2->text().toDouble();
    oPedido_proveedor->subtotal = ui->txtsubtotal->text().toDouble();
    oPedido_proveedor->dto = ui->txtimporte_descuento->text().toDouble();
    oPedido_proveedor->iva = ui->txtiva->text().toDouble();
    oPedido_proveedor->rec_total= ui->txttotal_recargo->text().toDouble();
    oPedido_proveedor->total = ui->txttotal->text().toDouble();
    oPedido_proveedor->enviado = ui->chkenviado->isChecked();
    oPedido_proveedor->recibido = ui->chkRecibido->isChecked();
    oPedido_proveedor->recibido_completo = ui->chkcompleto->isChecked();
    oPedido_proveedor->recargo_equivalencia = ui->chklporc_rec->isChecked();
    oPedido_proveedor->comentario = ui->txtcomentario->toPlainText();
    oPedido_proveedor->fecha_entrega = ui->txtfechaRecepcion->date();
    oPedido_proveedor->direccion_entrega1 = ui->txtdireccion_entrega1->text();
    oPedido_proveedor->direccion_entrega2 = ui->txtdireccion_entrega1_2->text();
    oPedido_proveedor->cp_entrega = ui->txtcp_entrega->text();
    oPedido_proveedor->poblacion_entrega = ui->txtpoblacion_entrega->text();
    oPedido_proveedor->provincia_entrega = ui->txtprovincia_entrega->text();
    oPedido_proveedor->horario_activo = ui->txtHorarioEntrega->text();
    oPedido_proveedor->base1 = ui->txtbase1->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->base2 = ui->txtbase2->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->base3 = ui->txtbase3->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->base4 = ui->txtbase4->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->iva1 = ui->txtiva1->text().toInt();
    oPedido_proveedor->iva2 = ui->txtiva2->text().toInt();
    oPedido_proveedor->iva3 = ui->txtiva3->text().toInt();
    oPedido_proveedor->iva4 = ui->txtiva4->text().toInt();
    oPedido_proveedor->iva1 = ui->txtiva1->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->iva2 = ui->txtiva2->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->iva3 = ui->txtiva3->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->iva4 = ui->txtiva4->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->porc_rec1 = ui->txtporc_rec1->text().toInt();
    oPedido_proveedor->porc_rec2 = ui->txtporc_rec2->text().toInt();
    oPedido_proveedor->porc_rec3 = ui->txtporc_rec3->text().toInt();
    oPedido_proveedor->porc_rec4 = ui->txtporc_rec4->text().toInt();
    oPedido_proveedor->rec1 = ui->txtporc_rec1->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->rec2 = ui->txtporc_rec2->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->rec3 = ui->txtporc_rec3->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->rec4 = ui->txtporc_rec4->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->total1 = ui->txttotal1->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->total2 = ui->txttotal2->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->total3 = ui->txttotal3->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
    oPedido_proveedor->total4 = ui->txttotal4->text().replace(".","").replace(",",".").replace(moneda,"").toDouble();
}



void FrmPedidosProveedor::clear()
{
    ui->txtpedido->clear();
   // ui->lblSerie->clear();
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
  //  ui->lblSerie->clear();
    ui->lblnumero_pedido->clear();
}

void FrmPedidosProveedor::resizeTable(int x)
{
    Q_UNUSED(x);
    helper.resizeTable();
}

void FrmPedidosProveedor::on_btnSiguiente_clicked()
{
    if(!pedido.next())
        TimedMessageBox * t = new TimedMessageBox(this,tr("Final de archivo alcanzado"));
}

void FrmPedidosProveedor::on_btnAnterior_clicked()
{
    if(!pedido.prev())
        TimedMessageBox * t = new TimedMessageBox(this,tr("Principio de archivo alcanzado"));
}

void FrmPedidosProveedor::on_btnAnadirEntregas_clicked()
{
    frmAddEntregasCuenta frmEntregas(this);
    if(frmEntregas.exec() == QDialog::Accepted){
        EntregasCuenta oEntrega(this);
        if(!oEntrega.Anadir(2,oPedido_proveedor->id_proveedor,frmEntregas.importe,frmEntregas.fecha,frmEntregas.concepto))
            QMessageBox::warning(this,tr("Gestión de pedidos a proveedor"),
                                 tr("Falló el insertar una nueva entrega a cuenta"),
                                 tr("Aceptar"));
        else
        {
            cargar_tabla_entregas();

        }

    }

}

void FrmPedidosProveedor::cargar_tabla_entregas()
{
    QSqlQueryModel *modelEntregas = new  QSqlQueryModel(this);
    modelEntregas->setQuery("select * from proveedor_a_cuenta where id_proveedor = "+
                            QString::number(oPedido_proveedor->id_proveedor)+" and disponible >0",Configuracion_global->groupDB);
    ui->tabla_entregas->setModel(modelEntregas);
}


void FrmPedidosProveedor::on_btnImprimir_clicked()
{
    //TODO imprimir
}


void FrmPedidosProveedor::on_txtBuscar_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    filter_table();
}

void FrmPedidosProveedor::formatotabla()
{
    QStringList columnas;
    QVariantList sizes;
    columnas <<"id" << tr("Pedido") << tr("Fecha") <<tr("F.Recepción") <<tr("Cif/Nif") <<tr("Código") <<tr("Proveedor");
    sizes <<0 << 120 << 100 <<100 << 120 <<120 <<300;
    for(int cnt =0;cnt<columnas.length();cnt++)
    {
        model->setHeaderData(cnt,Qt::Horizontal,columnas.at(cnt));
        ui->tabla->setColumnWidth(cnt,sizes.at(cnt).toInt());

    }
    ui->tabla->setColumnHidden(0,true);
}

void FrmPedidosProveedor::filter_table()
{
    QHash <QString,QString> h;
    h[tr("pedido")] ="pedido";
    h[tr("fecha")] = "fecha";
    h[tr("recepción")] = "recepcion";
    h[tr("codigo_proveedor")] ="codigo_proveedor";
    h[tr("proveedor")] = "proveedor";

    QString orden = h.value(ui->cboOrden->currentText());
    QString arg1 = ui->txtBuscar->text();
    QString modo;
    if(ui->cboModo->currentText()==tr("A-Z"))
        modo = "";
    else
        modo = "DESC";

    model->setQuery("select id,pedido,fecha,recepcion,cif_nif,codigo_proveedor,proveedor from ped_pro where ejercicio = "+
                    Configuracion_global->cEjercicio+" and "+orden+ " like '%"+arg1+ "%' order by "+orden +" "+modo,
                    Configuracion_global->empresaDB);
}


void FrmPedidosProveedor::on_tabla_doubleClicked(const QModelIndex &index)
{
    QSqlQueryModel* mymodel = qobject_cast<QSqlQueryModel*>(ui->tabla->model());
    int id = Configuracion_global->devolver_id_tabla(mymodel,index);
    oPedido_proveedor->recuperar("select * from ped_pro where id =" +QString::number(id));
    llenar_campos();
    ui->stackedWidget->setCurrentIndex(0);

}

void FrmPedidosProveedor::on_tabla_clicked(const QModelIndex &index)
{
    QSqlQueryModel *model = qobject_cast<QSqlQueryModel*>(ui->tabla->model());
    int id = Configuracion_global->devolver_id_tabla(model,index);
    oPedido_proveedor->recuperar("select * from ped_pro where id =" +QString::number(id));
    llenar_campos();

}

void FrmPedidosProveedor::on_btnBuscar_clicked()
{
    ui->txtBuscar->clear();
    ui->stackedWidget->setCurrentIndex(1);
    ui->txtBuscar->setFocus();
}

void FrmPedidosProveedor::on_cboModo_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    filter_table();
}

void FrmPedidosProveedor::on_cboOrden_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    filter_table();
}

void FrmPedidosProveedor::on_btnLimpiar_clicked()
{
    ui->cboModo->setCurrentIndex(0);
    ui->cboOrden->setCurrentIndex(0);
    ui->txtBuscar->clear();
    filter_table();
}
