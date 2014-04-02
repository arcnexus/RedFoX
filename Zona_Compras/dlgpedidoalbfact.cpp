#include "dlgpedidoalbfact.h"
#include "ui_dlgpedidoalbfact.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Almacen/articulo.h"
#include "proveedor.h"
#include <QTimer>
DlgPedidoAlbFact::DlgPedidoAlbFact(QString label,QString prov, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPedidoAlbFact)
{
    _tipo = Albaran;
    ui->setupUi(this);
    ui->label->setText(label);
    this->setWindowTitle(label);
    ui->lbl_prov->setText(prov);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));

    ui->cboporc_iva_gasto1->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto1->setModelColumn(4);
    ui->cboporc_iva_gasto1->setCurrentIndex(ui->cboporc_iva_gasto1->findText(Configuracion_global->ivaList.at(0)));

    ui->cboporc_iva_gasto2->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto2->setModelColumn(4);
    ui->cboporc_iva_gasto2->setCurrentIndex(ui->cboporc_iva_gasto2->findText(Configuracion_global->ivaList.at(0)));

    ui->cboporc_iva_gasto3->setModel(Configuracion_global->iva_model);
    ui->cboporc_iva_gasto3->setModelColumn(4);
    ui->cboporc_iva_gasto3->setCurrentIndex(ui->cboporc_iva_gasto3->findText(Configuracion_global->ivaList.at(0)));

    connect(ui->SpinGastoDist1,SIGNAL(valueChanged(double)),this,SLOT(calcular(double)));
    connect(ui->SpinGastoDist3,SIGNAL(valueChanged(double)),this,SLOT(calcular(double)));
    connect(ui->SpinGastoDist2,SIGNAL(valueChanged(double)),this,SLOT(calcular(double)));
    connect(ui->spinPorc_dto_pp,SIGNAL(valueChanged(double)),this,SLOT(calcular(double)));

    connect(ui->cboporc_iva_gasto1,SIGNAL(currentIndexChanged(int)),this,SLOT(cbo_IndexChanged(int)));
    connect(ui->cboporc_iva_gasto2,SIGNAL(currentIndexChanged(int)),this,SLOT(cbo_IndexChanged(int)));
    connect(ui->cboporc_iva_gasto3,SIGNAL(currentIndexChanged(int)),this,SLOT(cbo_IndexChanged(int)));

    ui->txtporc_iva1->setText(Configuracion_global->ivaList.at(0));
    ui->txtporc_iva2->setText(Configuracion_global->ivaList.at(1));
    ui->txtporc_iva3->setText(Configuracion_global->ivaList.at(2));
    ui->txtporc_iva4->setText(Configuracion_global->ivaList.at(3));

    ui->fechaDoc->setDate(QDate::currentDate());
}

DlgPedidoAlbFact::~DlgPedidoAlbFact()
{
    delete ui;
}

void DlgPedidoAlbFact::on_btnAccept_clicked()
{
    if(ui->txtDoc->text().isEmpty())
    {
        QMessageBox::warning(this,tr("Nº de documento"),tr("especifique el nº de documento antes de proceder"));
        return;
    }
    Configuracion_global->transaction();
    if(crear_documento())
    {
        Configuracion_global->commit();
        TimedMessageBox * t = new TimedMessageBox(parentWidget(),tr("%1 creado con éxito").arg(_tipo == Albaran ? tr("Albarán") : tr("Factura")));
        this->accept();
    }
    else
    {
        Configuracion_global->rollback();       
    }
}

void DlgPedidoAlbFact::calcular(double)
{
    double base  = 0.0, iva  = 0.0, re  = 0.0, total  = 0.0, subtotal= 0.0;
    double base1 = 0.0, iva1 = 0.0, re1 = 0.0, total1 = 0.0;
    double base2 = 0.0, iva2 = 0.0, re2 = 0.0, total2 = 0.0;
    double base3 = 0.0, iva3 = 0.0, re3 = 0.0, total3 = 0.0;
    double base4 = 0.0, iva4 = 0.0, re4 = 0.0, total4 = 0.0;

    double _dtoPP = 1-(ui->spinPorc_dto_pp->value()/100);

    for(auto i=0 ; i< ui->tablaLineas->rowCount(); ++i)
    {
        int id = ui->tablaLineas->item(i,0)->data(Qt::DisplayRole).toInt();
        double cantidad = qobject_cast<QDoubleSpinBox*>(ui->tablaLineas->cellWidget(i,5))->value();
        double art_base = cantidad * ui->tablaLineas->item(i,6)->data(Qt::DisplayRole).toDouble();

        double porc_iva = lineas.value(id).value("porc_iva").toDouble();
        double dto_linea = 1 -( qobject_cast<QDoubleSpinBox*>(ui->tablaLineas->cellWidget(i,7))->value()/100);

        art_base *= dto_linea;

        ui->tablaLineas->item(i,8)->setText(Configuracion_global->toFormatoMoneda(QString::number(art_base,'f',Configuracion_global->decimales_campos_totales)));
        if(porc_iva > 20.0)
            base1 += art_base * _dtoPP;
        else if (porc_iva > 8.0)
            base2 += art_base * _dtoPP;
        else if (porc_iva > 0)
            base3 += art_base * _dtoPP;
        else
            base4 += art_base * _dtoPP;

        subtotal += art_base;
    }

    //AÑADO GASTOS
    double _iva_base1 = Configuracion_global->ivaList.at(0).toDouble();
    double _iva_base2 = Configuracion_global->ivaList.at(1).toDouble();
    double _iva_base3 = Configuracion_global->ivaList.at(2).toDouble();
    double _iva_base4 = Configuracion_global->ivaList.at(3).toDouble();

    ui->spiniva_gasto1->setValue(ui->cboporc_iva_gasto1->currentText().toFloat() * ui->SpinGastoDist1->value() / 100.00);
    ui->spiniva_gasto2->setValue(ui->cboporc_iva_gasto2->currentText().toFloat() * ui->SpinGastoDist2->value() / 100.00);
    ui->spiniva_gasto3->setValue(ui->cboporc_iva_gasto3->currentText().toFloat() * ui->SpinGastoDist3->value() / 100.00);

    if(ui->chkrecargo_equivalencia->isChecked())
        re1 = base1 * (Configuracion_global->reList.at(0).toDouble()/100);
    if(ui->chkrecargo_equivalencia->isChecked())
        re2 = base2 * (Configuracion_global->reList.at(1).toDouble()/100);
    if(ui->chkrecargo_equivalencia->isChecked())
        re3 = base3 * (Configuracion_global->reList.at(2).toDouble()/100);
    if(ui->chkrecargo_equivalencia->isChecked())
        re4 = base4 * (Configuracion_global->reList.at(3).toDouble()/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == _iva_base1)
        base1 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == _iva_base1)
        base1 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == _iva_base1)
        base1 += ui->SpinGastoDist3->value();
    iva1 = base1 * (_iva_base1/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == _iva_base2)
        base2 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == _iva_base2)
        base2 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == _iva_base2)
        base2 += ui->SpinGastoDist3->value();
    iva2 = base2 * (_iva_base2/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == _iva_base3)
        base3 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == _iva_base3)
        base3 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == _iva_base3)
        base3 += ui->SpinGastoDist3->value();
    iva3 = base3 * (_iva_base3/100);

    if(ui->cboporc_iva_gasto1->currentText().toFloat() == _iva_base4)
        base4 += ui->SpinGastoDist1->value();
    if(ui->cboporc_iva_gasto2->currentText().toFloat() == _iva_base4)
        base4 += ui->SpinGastoDist2->value();
    if(ui->cboporc_iva_gasto3->currentText().toFloat() == _iva_base4)
        base4 += ui->SpinGastoDist3->value();
    iva4 = base4 * (_iva_base4/100);

    double dtopp = subtotal *(ui->spinPorc_dto_pp->value()/100.0);

    base = base1 + base2+base3+base4;
    iva = iva1 + iva2 +iva3+iva4;
    re  = re1 +re2 + re3 + re4;

    total = (base /*- irpf*/) + iva +re;   //TODO ¿IRPF?
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

    //ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(Configuracion_global->toRound(dtopp,Configuracion_global->decimales_campos_totales)));

    //this->moneda = moneda;
    ui->txtbase->setText             (Configuracion_global->toFormatoMoneda(QString::number(base    ,'f',Configuracion_global->decimales_campos_totales)));
    //ui->txtimporte_descuento->setText(Configuracion_global->toFormatoMoneda(QString::number(dto     ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtsubtotal->setText         (Configuracion_global->toFormatoMoneda(QString::number(subtotal,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText              (Configuracion_global->toFormatoMoneda(QString::number(iva     ,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo->setText    (Configuracion_global->toFormatoMoneda(QString::number(re      ,'f',Configuracion_global->decimales_campos_totales)));
    //ui->txttotal->setText            (Configuracion_global->toFormatoMoneda(QString::number((base-irpf)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));
    //ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));

    ui->txtbase->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txtiva->setText (Configuracion_global->toFormatoMoneda(QString::number(iva ,'f',Configuracion_global->decimales_campos_totales)));
   // ui->txtrec->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal->setText(Configuracion_global->toFormatoMoneda(QString::number((base/*-irpf*/)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));

    ui->txtbase_total_2->setText(Configuracion_global->toFormatoMoneda(QString::number(base,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_iva_2->setText(Configuracion_global->toFormatoMoneda(QString::number(iva,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_recargo_2->setText(Configuracion_global->toFormatoMoneda(QString::number(re,'f',Configuracion_global->decimales_campos_totales)));
    ui->txttotal_2->setText(Configuracion_global->toFormatoMoneda(QString::number((base/*-irpf*/)+(iva+re),'f',Configuracion_global->decimales_campos_totales)));
}

void DlgPedidoAlbFact::validarcantidad(int row, int column)
{
    if(column != 8)
    {
        calcular(0.0);
        return;
    }

    int id_art = ui->tablaLineas->item(row,9)->data(Qt::DisplayRole).toInt();
    if(QMessageBox::question(this,tr("Actualizar coste"),tr("¿Desea actualizar el coste del artículo?"),
                          tr("No"),tr("Si")) == QMessageBox::Accepted)
    {
        Configuracion_global->transaction();
        double new_coste = Configuracion_global->MonedatoDouble(ui->tablaLineas->item(row,column)->text());
        QHash<QString,QVariant> _data;
        _data["coste"] = new_coste;
        QString error;
        if(SqlCalls::SqlUpdate(_data,"articulos",Configuracion_global->groupDB,QString("id=%1").arg(id_art),error))
        {
            if(Articulo::update_coste_kits(id_art,new_coste))
                Configuracion_global->commit();
            else
                Configuracion_global->rollback();
        }
        else
        {
            QMessageBox::critical(qApp->activeWindow(),tr("Error guardando árticulo"),error);
            Configuracion_global->rollback();
        }
    }
    calcular(0.0);
}

QMap<int, QSqlRecord> DlgPedidoAlbFact::getLineas() const
{
    return lineas;
}

void DlgPedidoAlbFact::setLineas(const QMap<int, QSqlRecord> &value)
{
    lineas = value;
    ui->tablaLineas->clear();
    ui->tablaLineas->setRowCount(0);
    ui->tablaLineas->setColumnCount(10);

    QStringList cabecera;
    cabecera <<tr("id") << tr("C.prov") << tr("Descripción")
            <<tr("Recibidos") <<tr("En Albarán anterior")
            << (_tipo ? tr("Este Albarán") : tr("Esta factura"))
            <<tr("Precio Ud.") <<tr("% DTO") <<tr("Total");
    ui->tablaLineas->setHorizontalHeaderLabels(cabecera);

    int pos = 0;
    for(auto l = lineas.cbegin() ; l != lineas.cend() ; ++l)
    {
        ui->tablaLineas->setRowCount(pos+1);
        const QSqlRecord r = l.value();
        QTableWidgetItem *item_columna0 = new QTableWidgetItem(r.value("id").toString());
        item_columna0->setFlags(item_columna0->flags() & (~Qt::ItemIsEditable));
        item_columna0->setTextColor(Qt::blue);
        ui->tablaLineas->setItem(pos,0,item_columna0);        

        QTableWidgetItem *item_columna1 = new QTableWidgetItem(r.value("codigo").toString());
        item_columna1->setFlags(item_columna1->flags() & (~Qt::ItemIsEditable));
        item_columna1->setTextColor(Qt::blue);
        ui->tablaLineas->setItem(pos,1,item_columna1);

        QTableWidgetItem *item_columna3 = new QTableWidgetItem(r.value("descripcion").toString());
        item_columna3->setFlags(item_columna3->flags() & (~Qt::ItemIsEditable));
        item_columna3->setTextColor(Qt::blue);
        ui->tablaLineas->setItem(pos,2,item_columna3);
        ui->tablaLineas->setColumnWidth(2,140);


        QTableWidgetItem *item_columna6 = new QTableWidgetItem(r.value("cantidad_recibida").toString());
        if(r.value("cantidad_recibida").toInt()>0)
            item_columna6->setBackgroundColor(Qt::yellow);
        if(r.value("cantidad_pendiente").toInt() <=0)
            item_columna6->setBackgroundColor(Qt::green);
        if(r.value("cantidad_recibida").toInt()==0 )
            item_columna6->setBackgroundColor(Qt::yellow);

        item_columna6->setTextColor(Qt::blue); // color de los items
        item_columna6->setFlags(item_columna6->flags() & (~Qt::ItemIsEditable));
        item_columna6->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tablaLineas->setItem(pos,3,item_columna6);

        QTableWidgetItem *item_columna4 = new QTableWidgetItem(r.value("en_albaran").toString());
        item_columna4->setFlags(item_columna1->flags() & (~Qt::ItemIsEditable));
        item_columna4->setTextColor(Qt::blue);
        ui->tablaLineas->setItem(pos,4,item_columna4);

        QDoubleSpinBox* sp = new QDoubleSpinBox(this);
        sp->setMaximum(r.value("cantidad_recibida").toDouble() - r.value("en_albaran").toDouble());
        sp->setMinimum(0);
        sp->setValue(sp->maximum());
        connect(sp,SIGNAL(valueChanged(double)),this,SLOT(calcular(double)));
        ui->tablaLineas->setCellWidget(pos,5,sp);

        QTableWidgetItem *item_columna8 = new QTableWidgetItem(QString::number(r.value("precio").toDouble(),'f',Configuracion_global->decimales));
        item_columna8->setTextColor(Qt::black); // color de los items
        ui->tablaLineas->setItem(pos,6,item_columna8);


        QDoubleSpinBox* sp_dto = new QDoubleSpinBox(this);
        sp_dto->setMaximum(100.0);
        sp_dto->setMinimum(0);
        sp_dto->setValue(0);
        connect(sp_dto,SIGNAL(valueChanged(double)),this,SLOT(calcular(double)));
        ui->tablaLineas->setCellWidget(pos,7,sp_dto);

        QTableWidgetItem *item_columna_total = new QTableWidgetItem("0.00");
        item_columna_total->setFlags(item_columna_total->flags() & (~Qt::ItemIsEditable));
        item_columna_total->setTextColor(Qt::blue);
        ui->tablaLineas->setItem(pos,8,item_columna_total);

        QTableWidgetItem *item_columna10 = new QTableWidgetItem(r.value("id_articulo").toString());
        item_columna10->setTextColor(Qt::black); // color de los items
        item_columna10->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tablaLineas->setItem(pos,9,item_columna10);
        ui->tablaLineas->setColumnHidden(9,true);
        pos++;
    }
    ui->tablaLineas->setColumnHidden(0,true);
    ui->tablaLineas->setItemDelegateForColumn(6,new MonetaryDelegate(this));    
    ui->tablaLineas->setColumnWidth(1,160);
    ui->tablaLineas->setColumnWidth(2,340);
    calcular(0.0);
    connect(ui->tablaLineas,SIGNAL(cellChanged(int,int)),this,SLOT(validarcantidad(int, int)));
}

void DlgPedidoAlbFact::set_proveedor(int id)
{
    QString error;
    QMap<int, QSqlRecord> records = SqlCalls::SelectRecord("proveedores",QString("id=%1").arg(id),Configuracion_global->groupDB,error);
    if(records.isEmpty())
        QMessageBox::critical(this,tr("Error al recuperar proveedor"),error);
    else
        r_prov = records.first();
}

void DlgPedidoAlbFact::set_tipo(DlgPedidoAlbFact::tipo t)
{
    _tipo = t;
}


void DlgPedidoAlbFact::on_chkrecargo_equivalencia_toggled(bool checked)
{
    if (checked)
    {
        ui->txtporc_rec1->setText(Configuracion_global->reList.at(0));
        ui->txtporc_rec2->setText(Configuracion_global->reList.at(1));
        ui->txtporc_rec3->setText(Configuracion_global->reList.at(2));
        ui->txtporc_rec4->setText(Configuracion_global->reList.at(3));
    }
    else
    {
        ui->txtporc_rec1->setText("0.00");
        ui->txtporc_rec2->setText("0.00");
        ui->txtporc_rec3->setText("0.00");
        ui->txtporc_rec4->setText("0.00");
    }
    calcular(0.0);
}

void DlgPedidoAlbFact::cbo_IndexChanged(int)
{
    calcular(0.0);
}

bool DlgPedidoAlbFact::crear_documento()
{
    QHash<QString,QVariant> _data = _datosComunes();
    QString tabla;
    if(_tipo == Albaran)
    {
        tabla = "alb_pro";
        _data["albaran"] = ui->txtDoc->text();
    }
    else
    {
        tabla = "fac_pro";
        _data["factura"] = ui->txtDoc->text();
    }

    QString error;

    int id = SqlCalls::SqlInsert(_data,tabla,Configuracion_global->empresaDB,error);
    bool success = id != -1;
    if(!success)
    {
        QMessageBox::critical(this,tr("Error al insertar %1").arg(_tipo == Albaran ? tr("Albarán") : tr("Factura")),error);
        return false;
    }
    success &= crear_lineas(id);
    success &= Proveedor::acumular(r_prov.value("id").toInt(), ui->fechaDoc->date().month() , Configuracion_global->MonedatoDouble(ui->txttotal->text()));
    return success;
}

bool DlgPedidoAlbFact::crear_lineas(int id_cab)
{
    //TODO compactar lineas con el mismo articulo
    if(id_cab < 0 )
        return false;
    bool success = true;
    QString error;
    QString tabla = _tipo == Albaran ? "lin_alb_pro" : "lin_fac_pro";
    for(auto i = 0; i<ui->tablaLineas->rowCount(); ++i)
    {
        QHash<QString,QVariant> _data;
        int id_linea_ped = ui->tablaLineas->item(i,0)->data(Qt::DisplayRole).toInt();
        _data["id_cab"] = id_cab;
        _data["id_articulo"] = lineas.value(id_linea_ped).value("id_articulo");
        _data["codigo"] = ui->tablaLineas->item(i,1)->data(Qt::DisplayRole);
        _data["descripcion"] = ui->tablaLineas->item(i,2)->data(Qt::DisplayRole);
        _data["precio"] = ui->tablaLineas->item(i,6)->data(Qt::DisplayRole);        
        double porc_iva = lineas.value(id_linea_ped).value("porc_iva").toDouble();
        _data["porc_iva"] = porc_iva;
        if(ui->chkrecargo_equivalencia->isChecked())
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
            _data["rec"] = Configuracion_global->MonedatoDouble(ui->txttotal_recargo->text());
        }

        _data["cantidad"] = qobject_cast<QDoubleSpinBox*>(ui->tablaLineas->cellWidget(i,5))->value();
        _data["subtotal"] = _data.value("cantidad").toDouble() * _data.value("precio").toDouble();

        _data["total"] = Configuracion_global->MonedatoDouble(ui->tablaLineas->item(i, 8)->text());

        _data["porc_dto"] = qobject_cast<QDoubleSpinBox*>(ui->tablaLineas->cellWidget(i,7))->value();
        _data["dto"] = _data.value("subtotal").toDouble() * (_data.value("porc_dto").toDouble() / 100);

        _data["iva"] = _data.value("total").toDouble() * (_data.value("porc_iva").toDouble() / 100);
        //  _data["coste_real_unidad"] =

        if(SqlCalls::SqlInsert(_data,tabla,Configuracion_global->empresaDB,error) == -1)
        {
            QMessageBox::critical(this,tr("Error al insertar lineas de %1").arg(_tipo == Albaran ? tr("Albarán") : tr("Factura")),error);
            success = false;
            break;
        }
        QHash<QString,QVariant> _ped_data;
        _ped_data["en_albaran"] = lineas.value(id_linea_ped).value("en_albaran").toDouble() + _data.value("cantidad").toDouble();
        if(!SqlCalls::SqlUpdate(_ped_data,"lin_ped_pro",Configuracion_global->empresaDB,QString("id = %1").arg(id_linea_ped),error))
        {
            QMessageBox::critical(this,tr("Error al actualizar lineas de Pedido"),error);
            success = false;
            break;
        }

        if(!Articulo::acumulado_compras(_data.value("id_articulo").toInt(),_data.value("cantidad").toDouble(),_data.value("total").toDouble(),ui->fechaDoc->date()))
        {
            success = false;
            break;
        }
    }
    return success;
}


QHash<QString, QVariant> DlgPedidoAlbFact::_datosComunes()
{
    QHash<QString,QVariant> _data;
    _data["fecha"] = ui->fechaDoc->date();
    _data["id_proveedor"] = r_prov.value("id");
    _data["proveedor"] = r_prov.value("proveedor");
    _data["cif_proveedor"] = r_prov.value("cif");
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
    _data["base_total"] = Configuracion_global->MonedatoDouble(ui->txtbase_total_2->text());
    _data["iva_total"] = Configuracion_global->MonedatoDouble(ui->txttotal_iva_2->text());
    _data["total"] = Configuracion_global->MonedatoDouble(ui->txttotal->text());
    _data["comentario"] = ui->txtComentario->toPlainText();
    _data["recargo_equivalencia"] = ui->chkrecargo_equivalencia->isChecked();
    _data["porc_rec1"] = ui->txtporc_rec1->text().toDouble();
    _data["porc_rec2"] = ui->txtporc_rec2->text().toDouble();
    _data["porc_rec3"] = ui->txtporc_rec3->text().toDouble();
    _data["porc_rec4"] = ui->txtporc_rec4->text().toDouble();
    _data["rec1"] = Configuracion_global->MonedatoDouble(ui->txtrec1->text());
    _data["rec2"] = Configuracion_global->MonedatoDouble(ui->txtrec2->text());
    _data["rec3"] = Configuracion_global->MonedatoDouble(ui->txtrec3->text());
    _data["rec4"] = Configuracion_global->MonedatoDouble(ui->txtrec4->text());
    _data["total_recargo"] = Configuracion_global->MonedatoDouble(ui->txttotal_recargo->text());
    _data["ejercicio"] = ui->fechaDoc->date().year();
    _data["subtotal"] = Configuracion_global->MonedatoDouble(ui->txtsubtotal->text());
    _data["direccion1"] = r_prov.value("direccion1");
    _data["direccion2"] = r_prov.value("direccion2");
    _data["cp"] = r_prov.value("cp");
    _data["poblacion"] = r_prov.value("poblacion");
    _data["provincia"] = r_prov.value("provincia");
    _data["id_pais"] = r_prov.value("id_pais");
    _data["porc_dto"] = ui->spinPorc_dto_pp->value();
    _data["dto"] = Configuracion_global->MonedatoDouble(ui->txtDto_pp->text());
    _data["telefono"] = r_prov.value("telefono1");
    _data["fax"] = r_prov.value("fax");
    _data["movil"] = r_prov.value("movil");
    _data["codigo_proveedor"] = r_prov.value("codigo");
    _data["id_forma_pago"] = r_prov.value("id_forma_pago");
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
    _data["editable"] = true;

    return _data;
}
