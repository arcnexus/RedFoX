#include "frmgestioncobros.h"
#include "ui_frmgestioncobros.h"
#include "../Zona_Ventas/frmgestioncobros2.h"
#include "Auxiliares/monetarydelegate_totals.h"
#include "Auxiliares/datedelegate.h"

void frmGestionCobros::format_tables()
{
    QStringList headers;
    QVariantList sizes;
    headers  << "id" << tr("Fecha") << tr("Vencimiento") << tr("Documento")
             << tr("Importe") << tr("Pagado") << tr("Pendiente de cobro");
    sizes <<0  << 100 << 100 << 120 << 120 << 120 << 120;

    for(int x=1; x<sizes.size(); ++x)
    {
         ui->tabla_deuda->setColumnWidth(x,sizes.at(x).toInt());
         deudas->setHeaderData(x,Qt::Horizontal,headers.at(x));
    }

    ui->tabla_deuda->setColumnHidden(0,true);

    ui->tabla_clientes->setColumnHidden(0,true);

    ui->tabla_clientes->setColumnWidth(2,70);
    ui->tabla_clientes->setColumnWidth(2,135);

    modelo_clientes->setHeaderData(1,Qt::Horizontal,tr("Código"));
    modelo_clientes->setHeaderData(2,Qt::Horizontal,tr("Denominación"));
    modelo_clientes->setHeaderData(3,Qt::Horizontal,tr("Cif/Nif"));
}

frmGestionCobros::frmGestionCobros(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::frmGestionCobros),
    menuButton(QIcon(":/Icons/PNG/Cobros.png"),tr("Gest. Cobros"),this)
{
    __init = false;
    this->installEventFilter(this);
}

frmGestionCobros::~frmGestionCobros()
{
    delete ui;
}

void frmGestionCobros::on_radPendientes_toggled(bool /*checked*/)
{
    init_querys();
}

void frmGestionCobros::on_radPagados_toggled(bool /*checked*/)
{
    init_querys();
}

void frmGestionCobros::on_radTodos_toggled(bool /*checked*/)
{
    init_querys();
}

bool frmGestionCobros::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::Show && obj == this)
    {
        init_querys();
    }
    else if(event->type() == QEvent::Resize && obj == this)
    {
        if(!__init)
            init();
    }
    return MayaModule::eventFilter(obj,event);
}

void frmGestionCobros::init_querys()
{
    if(!__init)
        return;

    QString sql = QString("select id,fecha,vencimiento,documento,importe,pagado,pendiente_cobro from clientes_deuda where id_empresa = %1 ").arg(Configuracion_global->idEmpresa);
    sql.append(QString("and vencimiento >= %1 and vencimiento <= %2 ").arg(ui->txtfecha_ini->date().toString("yyyyMMdd"))
                                                                     .arg(ui->txtfecha_fin->date().toString("yyyyMMdd")));
    if(ui->radPendientes->isChecked())
        sql.append("and pendiente_cobro>0 ");
    else if(ui->radPagados->isChecked())
        sql.append("and pendiente_cobro=0 ");

    if(ui->chkCliente->isChecked())
    {
        if(ui->tabla_clientes->currentIndex().isValid())
        {
            int id_cli = modelo_clientes->record(ui->tabla_clientes->currentIndex().row()).value("id").toInt();
            sql.append(QString("and id_cliente = %1").arg(id_cli));
        }
    }
    deudas->setQuery(sql,Configuracion_global->groupDB);
    format_tables();
}

void frmGestionCobros::on_txtbuscar_cliente_editingFinished()
{
    QString sql = QString("select id,codigo_cliente, nombre_fiscal,"
            "cif_nif from clientes where ");

    if(ui->radNombre->isChecked())
        sql.append("nombre_fiscal ");
    else if(ui->radCodigo->isChecked())
        sql.append("codigo_cliente ");
    else
        sql.append("cif_nif ");

    if(ui->radEmpieza->isChecked())
        sql.append(QString("like '%1%'").arg(ui->txtbuscar_cliente->text()));
    else
        sql.append(QString("like '%%1%'").arg(ui->txtbuscar_cliente->text()));

    modelo_clientes->setQuery(sql,Configuracion_global->groupDB);
    format_tables();
}

void frmGestionCobros::on_txtfecha_ini_dateChanged(const QDate &/*date*/)
{
    init_querys();
}

void frmGestionCobros::on_txtfecha_fin_dateChanged(const QDate &/*date*/)
{
    init_querys();
}

void frmGestionCobros::init()
{
    if(__init)
        return;
    ui->setupUi(this);

    deudas = new QSqlQueryModel(this);
    modelo_clientes = new QSqlQueryModel(this);

    ui->tabla_deuda->setModel(deudas);
    ui->tabla_clientes->setModel(modelo_clientes);

    ui->tabla_deuda->setItemDelegateForColumn(1,new DateDelegate(this));
    ui->tabla_deuda->setItemDelegateForColumn(2,new DateDelegate(this));
    ui->tabla_deuda->setItemDelegateForColumn(4, new MonetaryDelegate_totals(this,true));
    ui->tabla_deuda->setItemDelegateForColumn(5, new MonetaryDelegate_totals(this,true));
    ui->tabla_deuda->setItemDelegateForColumn(6, new MonetaryDelegate_totals(this,true));

    ui->txtfecha_ini->setDate(QDate::currentDate());
    ui->txtfecha_fin->setDate(QDate::currentDate().addDays(7));
    ui->btnCerrar->setVisible(false);

    __init = true;
}

void frmGestionCobros::on_tabla_clientes_clicked(const QModelIndex &/*index*/)
{
    init_querys();
}

void frmGestionCobros::on_btncobro_total_clicked()
{
    QModelIndex index = ui->tabla_deuda->currentIndex();
    if(!index.isValid())
        return;

    int id= deudas->record(index.row()).value("id").toInt();
    int id_factura,id_ticket;
    double pendiente_cobro;

    QMap <int,QSqlRecord> d;
    QString error;
    d = SqlCalls::SelectRecord("clientes_deuda",QString("id= %1 and id_empresa=%2").arg(id).arg(Configuracion_global->idEmpresa),Configuracion_global->groupDB,error);
    if(d.count()>0)
    {
        id_ticket = d.value(id).value("id_ticket").toInt();
        id_factura = d.value(id).value("id_factura").toInt();
        pendiente_cobro = d.value(id).value("pendiente_cobro").toDouble();

    }
    FrmGestionCobros2 gc2(this);
    gc2.setId_factura(id_factura);
    gc2.setId_ticket(id_ticket);
    gc2.setImporte(pendiente_cobro,id);
    gc2.exec();
    init_querys();
}
