#include "frmtpv.h"
#include "ui_frmtpv.h"

FrmTPV::FrmTPV(QWidget *parent) :
    MayaModule(module_zone(),module_name(),parent),
    ui(new Ui::FrmTPV),
    toolButton(tr("TPV"),":/Icons/PNG/tpv.png",this),
    menuButton(QIcon(":/Icons/PNG/tpv.png"),tr("TPV"),this),
    push(new QPushButton(QIcon(":/Icons/PNG/tpv.png"),"",this))
{
    ui->setupUi(this);
    push->setStyleSheet("background-color: rgb(133, 170, 142)");
    push->setToolTip(tr("Gestión de Caja"));
}

FrmTPV::~FrmTPV()
{
    delete ui;
}

void FrmTPV::on_btnClientes_clicked()
{
    if(ui->btnClientes->text() == tr("Mostrar Cliente"))
    {
        ui->frameticket->setMinimumHeight(ui->frameticket->height()-190);
        QPropertyAnimation *animation = new QPropertyAnimation(ui->frameclientes, "size",this);
        animation->setDuration(3000);

        animation->setStartValue(QSize(ui->frameclientes->width(), 0));
        animation->setEndValue(QSize(ui->frameclientes->width(),190));


        animation->setEasingCurve(QEasingCurve::OutBounce);
        connect(animation, SIGNAL(finished()),this,SLOT(tamano_grande()));
        animation->start();



        ui->btnClientes->setText(tr("Ocultar Cliente"));
    } else
    {
        ui->btnClientes->setText(tr("Mostrar Cliente"));
        ui->frameclientes->setMinimumHeight(0);
        ui->frameticket->setMaximumHeight(99999);

        QPropertyAnimation *animation = new QPropertyAnimation(ui->frameclientes, "size",this);
        animation->setDuration(3000);
        animation->setStartValue(QSize(ui->frameclientes->width(),190));
        animation->setEndValue(QSize(ui->frameclientes->width(), 0));
        animation->setEasingCurve(QEasingCurve::OutBounce);
        connect(animation, SIGNAL(finished()),this,SLOT(tamano_pequeno()));
        animation->start();

    }
}

void FrmTPV::tamano_grande()
{
    ui->frameticket->setMaximumHeight(ui->frameticket->height()+190);
}

void FrmTPV::tamano_pequeno()
{
    ui->frameticket->setMaximumHeight(ui->frameticket->height()-190);
}
