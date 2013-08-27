#include "frmtransportistas.h"
#include "ui_frmtransportistas.h"

FrmTransportistas::FrmTransportistas(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmTransportistas),
    toolButton(tr("Transportistas"),":/Icons/PNG/transport.png",this),
    menuButton(QIcon(":/Icons/PNG/transport.png"),tr("Transportistas"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/transport.png"),"",this))


{
    ui->setupUi(this);
}

FrmTransportistas::~FrmTransportistas()
{
    delete ui;
}
