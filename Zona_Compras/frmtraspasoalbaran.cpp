#include "frmtraspasoalbaran.h"
#include "ui_frmtraspasoalbaran.h"

frmTraspasoAlbaran::frmTraspasoAlbaran(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmTraspasoAlbaran)
{
    ui->setupUi(this);
    oPedidoProveedor = new PedidoProveedor(this);
    connect(ui->btnTraspasar,SIGNAL(clicked()),this,SLOT(traspasar()));
}

frmTraspasoAlbaran::~frmTraspasoAlbaran()
{
    delete ui;
}

void frmTraspasoAlbaran::traspasar()
{
    oPedidoProveedor->convertir_en_albaran();
}
