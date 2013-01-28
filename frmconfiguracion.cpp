#include "frmconfiguracion.h"
#include "ui_frmconfiguracion.h"

FrmConfiguracion::FrmConfiguracion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmConfiguracion)
{
     ui->setupUi(this);
     QSettings settings("infint", "terra");
    int nIndex = ui->txtcDriver->findText(settings.value("cDriverBDTerra").toString());
    if (nIndex !=-1)
        ui->txtcDriver->setCurrentIndex(nIndex);
    ui->txtcRutaSQLITE->setText(settings.value("cRutaDBTerra").toString());
    ui->txtcHost->setText(settings.value("cHostBDTerra").toString());
    ui->txtcUsuario->setText(settings.value("cUserBDTerra").toString());
    ui->txtcContrasena->setText(settings.value("cPasswordBDTerra").toString());
    ui->txtcPais->setText(settings.value("cPais").toString());
    ui->txtnEjercicio->setText(settings.value("cEjercicioActivo").toString());
    ui->txtnDigitosFactura->setText(QString::number(settings.value("nDigitosFactura").toInt()));
    ui->txtnIva1->setText(QString::number(settings.value("nIVA1").toDouble(),'f',0));
    ui->txtnIva2->setText(QString::number(settings.value("nIVA2").toDouble(),'f',0));
    ui->txtnIva3->setText(QString::number(settings.value("nIVA3").toDouble(),'f',0));
    ui->txtnIva4->setText(QString::number(settings.value("nIVA4").toDouble(),'f',0));
    ui->txtnRE1->setText(QString::number(settings.value("nRE1").toDouble(),'f',2));
    ui->txtnRE2->setText(QString::number(settings.value("nRE2").toDouble(),'f',2));
    ui->txtnRE3->setText(QString::number(settings.value("nRE3").toDouble(),'f',2));
    ui->txtnRE4->setText(QString::number(settings.value("nRE4").toDouble(),'f',2));
    if(settings.value("lProfesional").toInt()==1)
        ui->chkRetencionIRPF->setChecked(true);
    else
        ui->chkRetencionIRPF->setChecked(false);
    ui->txtnIRPF->setText(QString::number(settings.value("nIRPF").toInt()));

}

FrmConfiguracion::~FrmConfiguracion()
{
    delete ui;
}

void FrmConfiguracion::on_btnGuardar_clicked()
{
    QSettings settings("infint", "terra");
    settings.setValue("cDriverBDTerra",ui->txtcDriver->currentText());
    settings.setValue("cRutaDBTerra",ui->txtcRutaSQLITE->text());
    settings.setValue("cHostBDTerra",ui->txtcHost->text());
    settings.setValue("cUserBDTerra",ui->txtcUsuario->text());
    settings.setValue("cPasswordBDTerra",ui->txtcContrasena->text());
    settings.setValue("cPais",ui->txtcPais->text());
    settings.setValue("cEjercicioActivo",ui->txtnEjercicio->text());
    settings.setValue("nDigitosFactura",ui->txtnDigitosFactura->text());
    settings.setValue("nIVA1",ui->txtnIva1->text().toInt());
    settings.setValue("nIVA2",ui->txtnIva2->text().toInt());
    settings.setValue("nIVA3",ui->txtnIva3->text().toInt());
    settings.setValue("nIVA4",ui->txtnIva4->text().toInt());
    settings.setValue("nRE1",ui->txtnRE1->text().toDouble());
    settings.setValue("nRE2",ui->txtnRE2->text().toDouble());
    settings.setValue("nRE3",ui->txtnRE3->text().toDouble());
    settings.setValue("nRE4",ui->txtnRE4->text().toDouble());
    if (ui->chkRetencionIRPF->isChecked())
        settings.setValue("lProfesional",1);
    else
        settings.setValue("lProfesional",0);
    settings.setValue("nIRPF",ui->txtnIRPF->text().toDouble());
    QMessageBox::information(qApp->activeWindow(),tr("CONFIGURACIÓN TERRA"),tr("Los datos de configuración se han guardado correctamente"),tr("ok"));
    close();
}
