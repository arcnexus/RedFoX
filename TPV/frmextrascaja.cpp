#include "frmextrascaja.h"
#include "ui_frmextrascaja.h"

frmExtrasCaja::frmExtrasCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmExtrasCaja)
{
    ui->setupUi(this);
    ui->cboUsuario->setModel(Configuracion_global->usuarios_model);
}

frmExtrasCaja::~frmExtrasCaja()
{
    delete ui;
}

void frmExtrasCaja::on_btnAceptar_clicked()
{
    QHash <QString, QVariant> h;
    QString error;
    h["concepto"] = ui->txtExtras->toPlainText();
    int id_usuario = SqlCalls::SelectOneField("mayaglobal`.`usuarios","id",QString("nombre = '%1'").arg(ui->cboUsuario->currentText()),
                                              Configuracion_global->groupDB,error).toInt();
    h["id_usuario"] = id_usuario;
    h["importe"] = Configuracion_global->MonedatoDouble(ui->txtImporte->text());
    h["fecha"]= QDate::currentDate().toString("yyyyMMdd");
    h["hora"] = QTime::currentTime().toString("hh:mm");
    h["entrada"]  = 2;
    int new_id = SqlCalls::SqlInsert(h,"e_s_caja",Configuracion_global->empresaDB,error);
    if(new_id <0)
        QMessageBox::warning(this,tr("Gestión de tickets"),tr("se produjo un error al insertar extras: %1").arg(error));
    else
        TimedMessageBox *t = new TimedMessageBox(this,tr("Proceso registrado"));
    accept();
}

void frmExtrasCaja::on_txtImporte_editingFinished()
{
    ui->txtImporte->blockSignals(true);
    ui->txtImporte->setText(Configuracion_global->toFormatoMoneda(ui->txtImporte->text()));
    ui->txtImporte->blockSignals(false);
}
