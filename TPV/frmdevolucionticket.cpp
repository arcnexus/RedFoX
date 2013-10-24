#include "frmdevolucionticket.h"
#include "ui_frmdevolucionticket.h"
#include "../Auxiliares/monetarydelegate.h"
#include "../Auxiliares/datedelegate.h"

FrmDevolucionTicket::FrmDevolucionTicket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmDevolucionTicket)
{
    ui->setupUi(this);
    //-------------------
    // Tabla tickets
    //-------------------
    tickets = new QSqlQueryModel(this);
    tickets->setQuery("select id,caja,fecha,hora,serie,ticket,importe from cab_tpv",Configuracion_global->empresaDB);
    ui->tabla_cabeceras->setModel(tickets);

    QStringList headers;
    QVariantList sizes;
    headers << "id" <<tr("Caja") <<tr("Fecha") << tr("Hora") <<tr("Serie") <<tr("Ticket") <<tr("Total");
    sizes << 0 << 60 << 90  <<60 << 80 << 90 << 90;
    for(int i = 0; i<sizes.size();i++)
    {
        ui->tabla_cabeceras->setColumnWidth(i,sizes.at(i).toInt());
        tickets->setHeaderData(i,Qt::Horizontal,headers.at(i));
    }
    ui->tablalineas->setItemDelegateForColumn(2,new DateDelegate);
    ui->tablalineas->setItemDelegateForColumn(6,new MonetaryDelegate);

    //-----------------
    // Conexiones
    //-----------------
    connect(ui->tabla_cabeceras->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(refrescar_lineas(QModelIndex,QModelIndex)));
 }

FrmDevolucionTicket::~FrmDevolucionTicket()
{
    delete ui;
}

void FrmDevolucionTicket::refrescar_lineas(QModelIndex current, QModelIndex previous)
{
    Q_UNUSED (previous);

}
