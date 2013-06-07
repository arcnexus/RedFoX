#include "frmfacturasproveedor.h"
#include "ui_frmfacturasproveedor.h"

FrmFacturasProveedor::FrmFacturasProveedor(QWidget *parent, bool showCerrar) :
    QDialog(parent),
    ui(new Ui::FrmFacturasProveedor),
    helper(this),
    prov(this)
{
    ui->setupUi(this);

    ui->combo_pais->setModel(Configuracion_global->paises_model);
    ui->combo_pais->setModelColumn(Configuracion_global->paises_model->fieldIndex("pais"));

    helper.set_Tipo(true);
    helper.help_table(ui->Lineas);

    connect(ui->btnAnadirLinea,SIGNAL(clicked()),&helper,SLOT(addRow()));
    connect(ui->btn_borrarLinea,SIGNAL(clicked()),&helper,SLOT(removeRow()));
    connect(ui->chklRecargoEq,SIGNAL(toggled(bool)),&helper,SLOT(set_UsarRE(bool)));

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

    ui->txtnPorcentajeIva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtnPorcentajeIva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtnPorcentajeIva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtnPorcentajeIva4->setText(Configuracion_global->ivaList.at(3));

    ui->txtnRec1->setText(Configuracion_global->reList.at(0));
    ui->txtnRec2->setText(Configuracion_global->reList.at(1));
    ui->txtnRec3->setText(Configuracion_global->reList.at(2));
    ui->txtnRec4->setText(Configuracion_global->reList.at(3));

    ui->btn_cerrar->setVisible(showCerrar);
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
    QSqlDatabase::database("empresa").transaction();
    QSqlDatabase::database("Maya").transaction();
    if (ld->idLinea == -1)
    {
        //qDebug()<< ld->idLinea;
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("select id from articulos where cCodigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;

        QSqlQuery query_lin_fac_pro(QSqlDatabase::database("empresa"));
        query_lin_fac_pro.prepare("INSERT INTO lin_fac_pro (id_cab,id_articulo,codigo_articulo_proveedor,"
                                  "cDescripcion, nCantidad, rCoste,rSubTotal,nDto,rDto,nIva,"
                                  "rIva,rTotal) VALUES (:id_cab,:id_articulo,:codigo_articulo_proveedor,"
                                  ":descripcion,:cantidad,:coste_bruto,:subtotal_coste,:porc_dto,:dto,"
                                  ":porc_iva,:iva,:total);");
        query_lin_fac_pro.bindValue(":id_cab", oFacPro->id);
        query_lin_fac_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_fac_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_fac_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_fac_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_fac_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_fac_pro.bindValue(":subtotal_coste",ld->subTotal);
        query_lin_fac_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_fac_pro.bindValue(":dto",ld->dto);
        query_lin_fac_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_fac_pro.bindValue(":iva",(ld->subTotal * ld->iva_perc)/100);
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

        queryArticulos.prepare("update articulos set nCantidadPendienteRecibir = nCantidadPendienteRecibir+:nRecibir, "
                               " nStockReal = nStockReal + :nRecibir2 where cCodigo=:codigo");
        queryArticulos.bindValue(":nRecibir",ld->cantidad);
        queryArticulos.bindValue(":nRecibir2",ld->cantidad);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec() && ok_empresa){
            QSqlDatabase::database("empresa").commit();
            QSqlDatabase::database("Maya").commit();
        } else
        {
            QSqlDatabase::database("empresa").rollback();
            QSqlDatabase::database("Maya").rollback();
        }

        ld->idLinea = query_lin_fac_pro.lastInsertId().toInt();

    } else
    {
        // --------------------------
        // Descuento unidades pedidas
        //---------------------------
        QSqlQuery queryArticulos(QSqlDatabase::database("Maya"));
        queryArticulos.prepare("update articulos set "
                               "nStockReal = nStockReal - :nRecibir where cCodigo=:codigo");
        queryArticulos.bindValue(":nRecibir",ld->cantidad_old);
        queryArticulos.bindValue(":codigo",ld->codigo);
        if (!queryArticulos.exec())
        {
            ok_Maya = false;
            QMessageBox::warning(this,tr("Gestión de pedidos"),
                                 tr("Se produjo un error al actualizar stock : %1").arg(queryArticulos.lastError().text()));

        }

        queryArticulos.prepare("select id from articulos where cCodigo =:codigo");
        queryArticulos.bindValue(":codigo",ld->codigo);
        if(queryArticulos.exec())
            queryArticulos.next();
        else
            ok_Maya = false;
        QSqlQuery query_lin_fac_pro(QSqlDatabase::database("empresa"));
        query_lin_fac_pro.prepare("UPDATE lin_fac_pro SET "
                                  "id_articulo =:id_articulo,"
                                  "codigo_articulo_proveedor =:codigo_articulo_proveedor,"
                                  "cDescripcion =:descripcion,"
                                  "nCantidad =:cantidad,"
                                  "rCoste =:coste_bruto,"
                                  "rSubtotal =:subtotal_coste,"
                                  "nDto =:porc_dto,"
                                  "rDto =:dto,"
                                  "nIva =:porc_iva,"
                                  "rIva =:iva,"
                                  "rTotal =:total "
                                  "WHERE id = :id;");

        query_lin_fac_pro.bindValue(":id_cab", oFacPro->id);
        query_lin_fac_pro.bindValue(":id_articulo", queryArticulos.record().value("id").toInt());
        query_lin_fac_pro.bindValue(":codigo_articulo_proveedor",ld->codigo);
        query_lin_fac_pro.bindValue(":descripcion",ld->descripcion);
        query_lin_fac_pro.bindValue(":cantidad",ld->cantidad);
        query_lin_fac_pro.bindValue(":coste_bruto",ld->importe);
        query_lin_fac_pro.bindValue(":subtotal_coste",ld->subTotal);
        query_lin_fac_pro.bindValue(":porc_dto",ld->dto_perc);
        query_lin_fac_pro.bindValue(":dto",ld->dto);
        query_lin_fac_pro.bindValue(":porc_iva",ld->iva_perc);
        query_lin_fac_pro.bindValue(":iva",0); // importe iva hay que calcularlo
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
                               "nStockReal = nStockReal + :nRecibir where cCodigo=:codigo");
        queryArticulos.bindValue(":nRecibir",ld->cantidad);
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



void FrmFacturasProveedor::llenarProveedor(int id)
{
    prov.Recuperar("Select * from proveedores where id="+QString::number(id),1);
    ui->txtcCodigoCliente->setText(prov.cCodigo);
    ui->txtcCliente->setText(prov.cProveedor);
    ui->txtcDireccion->setText(prov.cDireccion1);
    ui->txtcDireccion2->setText(prov.cDireccion2);
    ui->txtcPoblacion->setText(prov.cPoblacion);
    ui->txtcProvincia->setText(prov.cProvincia);
    ui->txtcCp->setText(prov.cCP);
    ui->txtcCif->setText(prov.cCif);
    ui->combo_pais->setCurrentText(Configuracion::Devolver_pais(prov.idpais));
}

void FrmFacturasProveedor::totalChanged(double base, double dto, double subTotal, double iva, double re, double total, QString moneda)
{
    ui->txtrBase->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2))+moneda);
    ui->txtrImporteDescuento->setText(Configuracion_global->FormatoNumerico(QString::number(dto,'f',2))+moneda);
    ui->txtrSubtotal->setText(Configuracion_global->FormatoNumerico(QString::number(subTotal,'f',2))+moneda);
    ui->txtrImporteIva->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2))+moneda);
    //ui->txtrTotalRecargoEq->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2))+moneda);
    ui->txtrTotal->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2))+moneda);
    ui->lbl_total->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2))+moneda);

    ui->txtrBaseTotal_2->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2))+moneda);
    ui->txtrTotalIVA_2->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2))+moneda);
    ui->txtrTotalRecargoEq_2->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2))+moneda);
    ui->txtrTotal_2->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2))+moneda);
}

void FrmFacturasProveedor::desglose1Changed(double base, double iva, double re, double total)
{
    ui->txtrBase1->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2)));
    ui->txtrIVA1->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2)));
    ui->txtrRecargoEq1->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2)));
    ui->txtrTotal1->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2)));
}

void FrmFacturasProveedor::desglose2Changed(double base, double iva, double re, double total)
{
    ui->txtrBase2->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2)));
    ui->txtrIVA2->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2)));
    ui->txtrRecargoEq2->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2)));
    ui->txtrTotal2->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2)));
}

void FrmFacturasProveedor::desglose3Changed(double base, double iva, double re, double total)
{
    ui->txtrBase3->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2)));
    ui->txtrIVA3->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2)));
    ui->txtrRecargoEq3->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2)));
    ui->txtrTotal3->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2)));
}

void FrmFacturasProveedor::desglose4Changed(double base, double iva, double re, double total)
{
    ui->txtrBase4->setText(Configuracion_global->FormatoNumerico(QString::number(base,'f',2)));
    ui->txtrIVA4->setText(Configuracion_global->FormatoNumerico(QString::number(iva,'f',2)));
    ui->txtrRecargoEq4->setText(Configuracion_global->FormatoNumerico(QString::number(re,'f',2)));
    ui->txtrTotal4->setText(Configuracion_global->FormatoNumerico(QString::number(total,'f',2)));
}
