#include "frmbuscarcliente.h"
#include "ui_frmbuscarcliente.h"

#include "../Zona_Pacientes/cliente.h"

FrmBuscarCliente::FrmBuscarCliente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmBuscarCliente)
{
    ui->setupUi(this);
   // this::connect(this,SIGNAL(enviar_id_cliente(int)),frmClientes,SLOT(set_id_Cliente(int)));

    ModelClientes = 0;
}

FrmBuscarCliente::~FrmBuscarCliente()
{
    delete ui;
}

void FrmBuscarCliente::on_pushButton_clicked()
{
    // 1º - Preparamos la Select y la asociamos a un QSqlQueryModel
    QString  cSQL;
    cSQL ="Select id,cNombreFiscal,cCifNif,cPoblacion from clientes where cNombrefiscal like '%"+ui->txtBuscar->text().trimmed()+"%'";

    //NOTE - asignale parent a los pointer y se borraran solos al borrar el parent
    ModelClientes = new QSqlQueryModel(this);

    ModelClientes->setQuery(cSQL,QSqlDatabase::database("terra"));
    ui->Grid->setModel(ModelClientes);

    // 2º - Creamos Objeto de la clase Cabecera
    Cabecera = new QHeaderView(Qt::Horizontal,this);
    // 3º - La vinculamos a QTableView
    ui->Grid->setHorizontalHeader(Cabecera);
   // Cabecera->setResizeMode(1,QHeaderView::Fixed);
    Cabecera->resizeSection(1,300);
   // Cabecera->setResizeMode(2,QHeaderView::Fixed);
    Cabecera->resizeSection(2,100);
   // Cabecera->setResizeMode(3,QHeaderView::Fixed);
    Cabecera->resizeSection(3,200);
    Cabecera->setVisible(true);
    // 4º - Defino cabecera Modelo
    ModelClientes->setHeaderData(1,Qt::Horizontal,tr("Nombre del cliente"));
    ModelClientes->setHeaderData(2,Qt::Horizontal,tr("CIF/NIF/NIE"));
    ModelClientes->setHeaderData(3,Qt::Horizontal,tr("Población"));
    // 5º - Parametrizo valores QTableView
    ui->Grid->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->Grid->setAlternatingRowColors(true);
    ui->Grid->setFocus();
    //emit enviar_id_cliente( ui->txtBuscar->text().toInt());
}

int FrmBuscarCliente::DevolverID()
{
    if(ModelClientes)
    {
        QModelIndex celda=ui->Grid->currentIndex();
        QModelIndex index=ModelClientes->index(celda.row(),0);     ///< '0' es la posicion del registro que nos interesa

        QVariant pKey=ModelClientes->data(index,Qt::EditRole);
        return pKey.toInt();
    }
    return -1;
}
