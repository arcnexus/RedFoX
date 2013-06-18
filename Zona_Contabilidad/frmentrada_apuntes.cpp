#include "frmentrada_apuntes.h"
#include "ui_frmentrada_apuntes.h"

FrmEntrada_apuntes::FrmEntrada_apuntes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmEntrada_apuntes)
{
    ui->setupUi(this);
    oApunte = new apuntes(this);
}

FrmEntrada_apuntes::~FrmEntrada_apuntes()
{
    delete ui;
}

void FrmEntrada_apuntes::on_btnnuevo_asiento_clicked()
{
    controles(true);
    int num_apunte = oApunte->nuevo_numero_apunte();
    ui->txtnum_asiento->setText(QString::number(num_apunte));
    ui->txtfecha_asiento->setDate(QDate::currentDate());
    ui->txtfecha_asiento->setFocus();
}

void FrmEntrada_apuntes::controles(bool stat)
{
    ui->txtcuenta_debe->setEnabled(stat);
    ui->txtcuenta_haber->setEnabled(stat);
    ui->txtdesc_cuenta_debe->setEnabled(stat);
    ui->txtdesc_cuenta_haber->setEnabled(stat);
    ui->txtdocumento_debe->setEnabled(stat);
    ui->txtdoc_haber->setEnabled(stat);
    ui->txtfecha_asiento->setEnabled(stat);
    ui->txtimporte_debe->setEnabled(stat);
    ui->txtimporte_haber->setEnabled(stat);
    ui->cbo_moneda->setEnabled(stat);
    ui->btnbuscar_cuenta_debe->setEnabled(stat);
    ui->btnbuscar_cuenta_haber->setEnabled(stat);
    ui->btncerrar_asiento->setEnabled(stat);
    ui->btnnuevo_asiento->setEnabled(!stat);
}


void FrmEntrada_apuntes::on_btncerrar_asiento_clicked()
{
    controles(false);
}

void FrmEntrada_apuntes::on_btnanadir_linea_clicked()
{
    bool ok = oApunte->nuevalinea();
}
