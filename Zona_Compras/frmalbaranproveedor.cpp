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
    helper(this),
    prov(this),
    toolButton(tr("Albaranes \nProv."),":/Icons/PNG/albaran_pro.png",this),
    menuButton(QIcon(":/Icons/PNG/albaran_pro.png"),tr("Albaranes Prov."),this),
    push(new QPushButton(QIcon(":/Icons/PNG/albaran_pro.png"),"",this))
{
    ui->setupUi(this);
    oAlbPro = new AlbaranProveedor(this);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de albaranes de proveedor"));
    ui->cbo_pais->setModel(Configuracion_global->paises_model);
    ui->cbo_pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    helper.set_Tipo(true);
    helper.help_table(ui->Lineas);
    helper.set_tarifa(1);
    helper.setDatabase("empresa","lin_alb_pro");
    helper.setidHeader(1);
    helper.blockTable(true);


    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(ui->chklporc_rec,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));


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

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
    ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
    ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
    ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));

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

    ui->lbfacturado->setVisible(false);
    ui->lblNumFactura->setVisible(false);
    ui->txtfecha_factura->setVisible(false);
    ui->txtcNumFra->setVisible(false);
}

FrmAlbaranProveedor::~FrmAlbaranProveedor()
{
    delete ui;
}

void FrmAlbaranProveedor::llenarProveedor(int id)
{
    prov.clear();

    prov.Recuperar("Select * from proveedores where id="+QString::number(id),0);
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
    ui->txtentregado_a_cuenta->setText(Configuracion_global->toFormatoMoneda(QString::number(prov.entregado_a_cuenta,'f',2)));
    ui->chklporc_rec->setChecked(prov.recargo_equivalencia);
    ui->lblproveedor->setText(prov.proveedor);
    oAlbPro->id_proveedor = prov.id;
}

void FrmAlbaranProveedor::llenar_tabla_entregas()
{
    QSqlQueryModel *modelEntregas = new QSqlQueryModel(this);
    modelEntregas->setQuery("select id, fecha_entrega, concepto, importe from proveedor_a_cuenta where id_proveedor = "+
                            QString::number(prov.id),QSqlDatabase::database("Maya"));
    ui->tabla_entregas->setModel(modelEntregas);
    ui->tabla_entregas->setColumnHidden(0,true);
    ui->tabla_entregas->setColumnWidth(1,120);
    ui->tabla_entregas->setItemDelegateForColumn(1,new DateDelegate );
    ui->tabla_entregas->setColumnWidth(2,400);
    ui->tabla_entregas->setColumnWidth(3,120);
    ui->tabla_entregas->setItemDelegateForColumn(3, new MonetaryDelegate);
    QStringList cabecera;
    cabecera << tr("id") << tr("Fecha entrega") << tr("Concepto") << tr("Importe");
    for(int pos = 0; pos < cabecera.size();pos++)
    {
        modelEntregas->setHeaderData(pos,Qt::Horizontal,cabecera.at(pos));
    }
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
    helper.resizeTable();
}


void FrmAlbaranProveedor::totalChanged(double base, double dto, double subtotal, double iva, double re, double total, QString moneda)
{
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto,'f',2))+moneda);
    ui->txtsubtotal->setText(Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',2))+moneda);
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    ui->txttotal_recargo->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);
    ui->lbl_total->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);

    ui->txtbase_total->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2))+moneda);
    ui->txttotal_iva->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2))+moneda);
    ui->txtimporte_rec_total->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2))+moneda);
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2))+moneda);

    this->moneda = moneda;
}

void FrmAlbaranProveedor::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec1->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmAlbaranProveedor::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmAlbaranProveedor::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec3->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmAlbaranProveedor::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',2)));
    ui->txtporc_rec4->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(total,'f',2)));
}

void FrmAlbaranProveedor::on_btnSiguiente_clicked()
{
    oAlbPro->Recuperar("Select * from alb_pro where id >"+QString::number(oAlbPro->id)+" limit 0,1",1);
    llenar_campos();
    ui->btnAnterior->setEnabled(true);
    ui->btnBorrar->setEnabled(true);
    ui->btnImprimir->setEnabled(true);
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
    llenarProveedor(oAlbPro->id_proveedor);
    ui->txttotal1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->total1,'f',2)));
    ui->txttotal2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->total2,'f',2)));
    ui->txttotal3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->total3,'f',2)));
    ui->txttotal4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->total4,'f',2)));
    ui->txtporc_iva1->setText(QString::number(oAlbPro->porc_iva1));
    ui->txtporc_iva2->setText(QString::number(oAlbPro->porc_iva2));
    ui->txtporc_iva3->setText(QString::number(oAlbPro->porc_iva3));
    ui->txtporc_iva4->setText(QString::number(oAlbPro->porc_iva4));
    ui->txtiva1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->iva1,'f',2)));
    ui->txtiva2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->iva2,'f',2)));
    ui->txtiva3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->iva3,'f',2)));
    ui->txtiva4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->iva4,'f',2)));
    ui->txtporc_rec1->setText(QString::number(oAlbPro->porc_rec1));
    ui->txtporc_rec2->setText(QString::number(oAlbPro->porc_rec2));
    ui->txtporc_rec3->setText(QString::number(oAlbPro->porc_rec3));
    ui->txtporc_rec4->setText(QString::number(oAlbPro->porc_rec4));
    ui->txtrec1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->rec1,'f',2)));
    ui->txtrec2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->rec2,'f',2)));
    ui->txtrec3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->rec3,'f',2)));
    ui->txtrec4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->rec4,'f',2)));


    ui->txtbase1->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->base1,'f',2)));
    ui->txtbase2->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->base2,'f',2)));
    ui->txtbase3->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->base3,'f',2)));
    ui->txtbase4->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->base4,'f',2)));
    ui->txtcNumFra->setText(oAlbPro->factura);
    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->base_total,'f',2)));
    ui->txtiva_total->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->iva_total,'f',2)));
    ui->txtimporte_rec_total->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->importe_rec_total,'f',2)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number(oAlbPro->total,'f',2)));
    ui->txtcomentario->setText(oAlbPro->comentario);
    ui->txtpedido_cliente->setText(QString::number(oAlbPro->pedido));
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
        queryArticulos.prepare("select id from articulos where codigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery query_lin_alb_pro(QSqlDatabase::database("empresa"));
        query_lin_alb_pro.prepare("INSERT INTO lin_alb_pro (id_cab,id_articulo,codigo_articulo_proveedor,"
                                  "descripcion, cantidad, precio,subtotal,porc_dto,dto,porc_iva,"
                                  "iva,total) VALUES (:id_cab,:id_articulo,:codigo_articulo_proveedor,"
                                  ":descripcion,:cantidad,:precio,:subtotal,:porc_dto,:dto,"
                                  ":porc_iva,:iva,:total);");
        query_lin_alb_pro.bindValue(":id_cab", oAlbPro->id);
        query_lin_alb_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_alb_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_alb_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_alb_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_alb_pro.bindValue(":precio",ld->precio);
        query_lin_alb_pro.bindValue(":subtotal",ld->subtotal);
        query_lin_alb_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_alb_pro.bindValue(":dto",ld->dto);
        query_lin_alb_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_alb_pro.bindValue(":iva",(ld->subtotal * ld->iva_perc)/100);
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

        queryArticulos.prepare("update articulos set cantidad_pendiente_recibir = cantidad_pendiente_recibir+:cant_recibir, "
                               " stock_real = stock_real + :cant_recibir2 where codigo=:codigo");
        queryArticulos.bindValue(":cant_recibir",ld->cantidad);
        queryArticulos.bindValue(":cant_recibir2",ld->cantidad);
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
                               "stock_real = stock_real - :cant_recibir where codigo=:codigo");
        queryArticulos.bindValue(":cant_recibir",ld->cantidad_old);
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
        QSqlQuery query_lin_alb_pro(QSqlDatabase::database("empresa"));
        query_lin_alb_pro.prepare("UPDATE lin_alb_pro SET "
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

        query_lin_alb_pro.bindValue(":id_cab", oAlbPro->id);
        query_lin_alb_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_alb_pro.bindValue(":codigo",ld->codigo);
        query_lin_alb_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_alb_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_alb_pro.bindValue(":precio",ld->precio);
        query_lin_alb_pro.bindValue(":subtotal",ld->subtotal);
        query_lin_alb_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_alb_pro.bindValue(":dto",ld->dto);
        query_lin_alb_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_alb_pro.bindValue(":iva",(ld->subtotal * ld->iva_perc)/100);
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
                               "stock_real = stock_real + :cant_recibir where codigo=:codigo");
        queryArticulos.bindValue(":cant_recibir",ld->cantidad);
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
    ui->txtcodigo_proveedor->setFocus();
}

void FrmAlbaranProveedor::on_btnGuardar_clicked()
{
    if(!ui->txtalbaran->text().isEmpty())
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
        ui->txtalbaran->setFocus();
    }
}

void FrmAlbaranProveedor::guardar_campos_en_objeto()
{
    oAlbPro->albaran = ui->txtalbaran->text();
    oAlbPro->fecha = ui->txtfecha->date();
    oAlbPro->proveedor = ui->txtproveedor->text();
    oAlbPro->cif_proveedor = ui->txtcif->text();
    oAlbPro->total1 = ui->txttotal1->text().replace(",",".").toDouble();
    oAlbPro->total2 = ui->txttotal2->text().replace(",",".").toDouble();
    oAlbPro->total3 = ui->txttotal3->text().replace(",",".").toDouble();
    oAlbPro->total4 = ui->txttotal4->text().replace(",",".").toDouble();
    oAlbPro->porc_iva1 = ui->txtporc_iva1->text().toFloat();
    oAlbPro->porc_iva2 = ui->txtporc_iva2->text().toFloat();
    oAlbPro->porc_iva3 = ui->txtporc_iva3->text().toFloat();
    oAlbPro->porc_iva4 = ui->txtporc_iva4->text().toFloat();
    oAlbPro->iva1 = ui->txtiva1->text().replace(",",".").toDouble();
    oAlbPro->iva2 = ui->txtiva2->text().replace(",",".").toDouble();
    oAlbPro->iva3 = ui->txtiva3->text().replace(",",".").toDouble();
    oAlbPro->iva4 = ui->txtiva4->text().replace(",",".").toDouble();
    oAlbPro->porc_rec1 = ui->txtporc_rec1->text().toFloat();
    oAlbPro->porc_rec1 = ui->txtporc_rec1->text().toFloat();
    oAlbPro->porc_rec1 = ui->txtporc_rec1->text().toFloat();
    oAlbPro->porc_rec1 = ui->txtporc_rec1->text().toFloat();
    oAlbPro->rec1 = ui->txtrec1->text().replace(",",".").toDouble();
    oAlbPro->rec2 = ui->txtrec2->text().replace(",",".").toDouble();
    oAlbPro->rec3 = ui->txtrec3->text().replace(",",".").toDouble();
    oAlbPro->rec4 = ui->txtrec4->text().replace(",",".").toDouble();
    oAlbPro->base1 = ui->txtbase1->text().replace(",",".").toDouble();
    oAlbPro->base2 = ui->txtbase2->text().replace(",",".").toDouble();
    oAlbPro->base3 = ui->txtbase3->text().replace(",",".").toDouble();
    oAlbPro->base4 = ui->txtbase4->text().replace(",",".").toDouble();
    oAlbPro->factura = ui->txtcNumFra->text().replace(",",".").toDouble();
    oAlbPro->base_total = ui->txtbase->text().replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->iva_total = ui->txtiva_total->text().replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->importe_rec_total = ui->txtimporte_rec_total->text().replace(moneda,"").replace(",",".").toDouble();
    oAlbPro->total = ui->txttotal->text().replace(",",".").replace(moneda,"").toDouble();
    oAlbPro->comentario = ui->txtcomentario->toPlainText();
}



void FrmAlbaranProveedor::on_btnBuscar_clicked()
{
    db_consulta_view busca_albaran;
    busca_albaran.set_db("empresa");
    QStringList cCampo,cCabecera;
    QVariantList vTamanos;
    cCampo << "albaran" << "proveedor";
    cCabecera <<"Albarán" << "Fecha" << "Proveedor" << "Total";
    vTamanos << 0 << 120 << 130 << 250 <<120;
    busca_albaran.set_SQL("Select id, albaran,fecha, proveedor,total from alb_pro");
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
    helper.resizeTable();
}

void FrmAlbaranProveedor::on_botBuscarCliente_clicked()
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

void FrmAlbaranProveedor::on_btnAnadir_clicked()
{
    int id = oAlbPro->anadir();
    oAlbPro->Recuperar(id);
    this->id = id;
    emit block();
    llenar_campos();
    bloquearcampos(false);
    ui->txtcodigo_proveedor->setFocus();
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

void FrmAlbaranProveedor::on_tabWidget_2_currentChanged(int)
{
    helper.resizeTable();
}
