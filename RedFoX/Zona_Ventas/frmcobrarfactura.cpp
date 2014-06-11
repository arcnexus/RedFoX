#include "frmcobrarfactura.h"
#include "ui_frmcobrarfactura.h"

#include "../Auxiliares/Globlal_Include.h"
#include "frmgestioncobros2.h"

#include "../Auxiliares/datedelegate.h"
#include "../Auxiliares/monetarydelegate_totals.h"

FrmCobrarFactura::FrmCobrarFactura(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmCobrarFactura)
{
    ui->setupUi(this);

    vencimientos = new QSqlQueryModel(this);
    ui->tabla_vencimientos->setModel(vencimientos);

    ui->tabla_vencimientos->setItemDelegateForColumn(1,new DateDelegate(this));
    ui->tabla_vencimientos->setItemDelegateForColumn(2,new DateDelegate(this));
    ui->tabla_vencimientos->setItemDelegateForColumn(3, new MonetaryDelegate_totals(this,true));
    ui->tabla_vencimientos->setItemDelegateForColumn(4, new MonetaryDelegate_totals(this,true));
    ui->tabla_vencimientos->setItemDelegateForColumn(5, new MonetaryDelegate_totals(this,true));
}

FrmCobrarFactura::~FrmCobrarFactura()
{
    delete ui;
}

void FrmCobrarFactura::setFactura(int id)
{
    _idFactura = id;
    vencimientos->setQuery(QString("select id,fecha,vencimiento,importe,pagado,pendiente_cobro from clientes_deuda where id_factura = %1 and id_empresa = %2;")
                           .arg(id).arg(Configuracion_global->idEmpresa),Configuracion_global->groupDB);
    QStringList headers;
    QVariantList sizes;
    headers  <<"id"  <<tr("Fecha") <<tr("Vencimiento") <<tr("Importe") << tr("Pagado") <<tr("Pendiente");
    sizes    << 0    << 100             << 150         << 150          << 150           << 150;

    for(int x=1;x<sizes.size();x++)
    {
         ui->tabla_vencimientos->setColumnWidth(x,sizes.at(x).toInt());
         vencimientos->setHeaderData(x,Qt::Horizontal,headers.at(x));
    }
    ui->tabla_vencimientos->setColumnHidden(0,true);
    ui->btnCobrar->setEnabled(false);
}

void FrmCobrarFactura::on_btnCobrar_clicked()
{
    QModelIndex index = ui->tabla_vencimientos->currentIndex();
    if(!index.isValid())
        return;

    int id= vencimientos->record(index.row()).value("id").toInt();
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

    setFactura(_idFactura);
}

void FrmCobrarFactura::on_tabla_vencimientos_clicked(const QModelIndex &index)
{
    ui->btnCobrar->setEnabled(vencimientos->record(index.row()).value("pendiente_cobro").toDouble() != 0);
}
