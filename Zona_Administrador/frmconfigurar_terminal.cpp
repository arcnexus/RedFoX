#include "frmconfigurar_terminal.h"
#include "ui_frmconfigurar_terminal.h"

frmConfigurar_terminal::frmConfigurar_terminal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmConfigurar_terminal)
{
    ui->setupUi(this);
    QSqlQueryModel *series = new QSqlQueryModel(this);
    series->setQuery("select serie from series",Configuracion_global->empresaDB);
    ui->cboserie->setModel(series);
    QSqlQueryModel *users = new QSqlQueryModel(this);
    users->setQuery("select nombre from usuarios",Configuracion_global->groupDB);
    ui->cboUsuarioActivo->setModel(users);
    QSqlQueryModel *cajas = new QSqlQueryModel(this);
    cajas->setQuery("select desc_caja from cajas",Configuracion_global->empresaDB);
    ui->cbocaja->setModel(cajas);

}

frmConfigurar_terminal::~frmConfigurar_terminal()
{
    delete ui;
}

void frmConfigurar_terminal::on_btnAceptar_clicked()
{
    QSettings settings(qApp->applicationDirPath()+"/MayaConfig.ini", QSettings::IniFormat);
    QString error;
    settings.setValue("cUsuarioActivo",ui->cboUsuarioActivo->currentText());
    settings.setValue("contrasenaactiva",Configuracion_global->Crypt(ui->txtContrasena->text().trimmed()));
    settings.setValue("importe_apertura",ui->txtImporte_abertura->text());
    int id_caja = SqlCalls::SelectOneField("cajas","id",QString("desc_caja = %1").arg(ui->cbocaja->currentText()),
                                           Configuracion_global->empresaDB,error).toInt();
    settings.setValue("cajaactiva",id_caja);
    accept();


}

void frmConfigurar_terminal::on_txtImporte_abertura_editingFinished()
{
    ui->txtImporte_abertura->blockSignals(true);
    ui->txtImporte_abertura->setText(Configuracion_global->toFormatoMoneda(ui->txtImporte_abertura->text()));
    ui->txtImporte_abertura->blockSignals(false);
}
