#include "dlgpedidoalbfact.h"
#include "ui_dlgpedidoalbfact.h"

DlgPedidoAlbFact::DlgPedidoAlbFact(QString label, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPedidoAlbFact)
{
    ui->setupUi(this);
    ui->label->setText(label);
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

DlgPedidoAlbFact::~DlgPedidoAlbFact()
{
    delete ui;
}

void DlgPedidoAlbFact::on_btnAccept_clicked()
{

}
