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
    settings.setValue("cUsuarioActivo",ui->cboUsuarioActivo->currentText());
    settings.setValue("contrasenaactiva",Configuracion_global->Crypt(ui->txtContrasena->text().trimmed()));
    settings.setValue("cajaactiva",ui->cbocaja->currentText());
    accept();


}
