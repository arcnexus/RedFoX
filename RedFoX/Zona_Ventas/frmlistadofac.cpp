#include "frmlistadofac.h"
#include "ui_frmlistadofac.h"

#include "Auxiliares/Globlal_Include.h"
#include "Auxiliares/dlgsetupmail.h"

frmListadoFac::frmListadoFac(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmListadoFac)
{
    ui->setupUi(this);
    connect(ui->btnSalir,SIGNAL(clicked()),this,SLOT(close()));
}

frmListadoFac::~frmListadoFac()
{
    delete ui;
}

void frmListadoFac::on_rdb1Cliente_toggled(bool checked)
{
    ui->txt1Cliente->setEnabled(checked);
}

void frmListadoFac::on_rdbRGClientes_toggled(bool checked)
{
    ui->lblCliIni->setEnabled(checked);
    ui->lblCliFin->setEnabled(checked);
    ui->txtClienteIni->setEnabled(checked);
    ui->txtCFin->setEnabled(checked);
}

void frmListadoFac::on_rdbRGFechas_toggled(bool checked)
{
    ui->lblFechaDesde->setEnabled(checked);
    ui->lblFechaHasta->setEnabled(checked);
    ui->dateDesde->setEnabled(checked);
    ui->deteHasta->setEnabled(checked);
}

void frmListadoFac::on_rdbRGImportes_toggled(bool checked)
{
    ui->lblImporteDesde->setEnabled(checked);
    ui->lblImporteHasta->setEnabled(checked);
    ui->spinDesde->setEnabled(checked);
    ui->spinHasta->setEnabled(checked);
}

void frmListadoFac::on_btnPrew_clicked()
{
    QMap <QString,QString> parametros_sql;
    QString report = "lista_factura_"+QString::number(1);//TODO idioma documento
    parametros_sql["Empresa.cab_fac"] = getCaPreSql();
    parametros_sql["General.clientes"] = getClientesSql();
    parametros_sql["General.empresas"] = QString("id = %1").arg(Configuracion_global->idEmpresa);
    Configuracion::ImprimirPreview(report,parametros_sql,getParametros());
}

void frmListadoFac::on_btnPrint_clicked()
{
    QMap <QString,QString> parametros_sql;
    QString report = "lista_factura_"+QString::number(1);//TODO idioma documento
    parametros_sql["Empresa.cab_fac"] = getCaPreSql();
    parametros_sql["General.clientes"] = getClientesSql();
    parametros_sql["General.empresas"] = QString("id = %1").arg(Configuracion_global->idEmpresa);
    Configuracion::ImprimirDirecto(report,parametros_sql,getParametros());
}

void frmListadoFac::on_btnPdf_clicked()
{
    QMap <QString,QString> parametros_sql;
    QString report = "lista_factura_"+QString::number(1);//TODO idioma documento
    parametros_sql["Empresa.cab_fac"] = getCaPreSql();
    parametros_sql["General.clientes"] = getClientesSql();
    parametros_sql["General.empresas"] = QString("id = %1").arg(Configuracion_global->idEmpresa);
    Configuracion::ImprimirPDF(report,parametros_sql,getParametros());
}

void frmListadoFac::on_btnMail_clicked()
{
    DlgSetUpMail d(this);
    if(d.exec() == QDialog::Accepted)
    {
        QMap <QString,QString> parametros_sql;
        QString report = "lista_factura_"+QString::number(1);//TODO idioma documento

        QString pdfname = QString("ListadoAlbaranes");
        QString asunto = d.ui->txtAsunto->text();
        QString texto = d.ui->txtTexto->toPlainText();
        QString nombre = d.ui->txtNombre->text();
        QString mail = d.ui->txtMail->text();

        parametros_sql["Empresa.cab_fac"] = getCaPreSql();
        parametros_sql["General.clientes"] = getClientesSql();
        parametros_sql["General.empresas"] = QString("id = %1").arg(Configuracion_global->idEmpresa);
        Configuracion::EviarMail(report,parametros_sql,getParametros(),pdfname,mail,nombre,asunto,texto);
    }
}
QMap<QString, QString> frmListadoFac::getParametros()
{
    QMap <QString,QString> ret;

    double desde = ui->spinDesde->value();
    double hasta = ui->spinHasta->value();
    ret["@today"] = tr("Facturas a fecha %1").arg(QDate::currentDate().toString("yyyy-MM-dd"));
    if(ui->rdbRGImportes->isChecked())
        ret["@importes"] = tr("Con importe desde %1 hasta %2").arg(QString::number(desde,'f',2)).arg(QString::number(hasta,'f',2));
    if(ui->rdbRGFechas->isChecked())
        ret["@fechas"] = tr("Emitidas desde %1 hasta %2").arg(ui->dateDesde->date().toString("yyyy-MM-dd")).arg(ui->deteHasta->date().toString("yyyy-MM-dd"));
    if(ui->rdb1Cliente->isChecked())
        ret["@clientes"] = tr("Del cliente %1").arg(ui->txt1Cliente->text());
    else if(ui->rdbRGClientes->isChecked())
        ret["@clientes"] = tr("Del cliente %1 al %2").arg(ui->txtClienteIni->text()).arg(ui->txtCFin->text());
    return ret;
}
QString frmListadoFac::getClientesSql()
{
    QString ret = "codigo_cliente >0 ";
    if(ui->rdb1Cliente->isChecked())
        ret = QString("codigo_cliente = %1 ").arg(ui->txt1Cliente->text());
    else if(ui->rdbRGClientes->isChecked())
        ret = QString("codigo_cliente >= %1 and codigo_cliente <= %2 ").arg(ui->txtClienteIni->text()).arg(ui->txtCFin->text());

    ret.append("order by nombre_fiscal");
    return ret;
}
QString frmListadoFac::getCaPreSql()
{
    QString ret = "";
    if(ui->rdbRGFechas->isChecked())
        ret.append(QString(" fecha >= '%1' and fecha <= '%2' ").arg(ui->dateDesde->date().toString("yyyy-MM-dd")).arg(ui->deteHasta->date().toString("yyyy-MM-dd")));

    if(ui->rdbRGImportes->isChecked())
    {
        if(!ret.isEmpty())
            ret.append("and");
        ret.append(QString(" total >= %1 and total <= %2 ").arg(ui->spinDesde->value()).arg(ui->spinHasta->value()));
    }

    return ret;
}

