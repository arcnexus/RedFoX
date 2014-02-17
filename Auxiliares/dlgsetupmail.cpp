#include "dlgsetupmail.h"


DlgSetUpMail::DlgSetUpMail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSetUpMail)
{
    ui->setupUi(this);
    connect(ui->btnAcept,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->btnCancel,SIGNAL(clicked()),this,SLOT(reject()));
}

DlgSetUpMail::~DlgSetUpMail()
{
    delete ui;
}

void DlgSetUpMail::on_btnCli_clicked()
{
    //TODO BUSCAR CLIENTE
}

void DlgSetUpMail::on_btnDist_clicked()
{
    //TODO BUSCAR DISTRIBUIDOR
}
