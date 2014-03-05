#include "frmretirardecaja.h"
#include "ui_frmretirardecaja.h"

FrmRetirardeCaja::FrmRetirardeCaja(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmRetirardeCaja)
{
    ui->setupUi(this);
    ui->cbousuario->setModel(Configuracion_global->usuarios_model);
}

FrmRetirardeCaja::~FrmRetirardeCaja()
{
    delete ui;
}

void FrmRetirardeCaja::on_txtImporte_editingFinished()
{
    ui->txtImporte->blockSignals(true);
    ui->txtImporte->setText(Configuracion_global->toFormatoMoneda(ui->txtImporte->text()));
    ui->txtImporte->blockSignals(false);
}

void FrmRetirardeCaja::on_btnAceptar_clicked()
{
    QHash <QString, QVariant> h;
    QString error;
    h["concepto"] = ui->txtMotivo->toPlainText();
    //TODO revisar este id
    int id_usuario = SqlCalls::SelectOneField("redfoxglobal`.`usuarios","id",QString("nombre = '%1'").arg(ui->cbousuario->currentText()),
                                              Configuracion_global->globalDB,error).toInt();
    h["id_usuario"] = id_usuario;
    h["importe"] = Configuracion_global->MonedatoDouble(ui->txtImporte->text());
    h["fecha"]= QDate::currentDate().toString("yyyyMMdd");
    h["hora"] = QTime::currentTime().toString("hh:mm");
    h["entrada"]  = 0;
    int new_id = SqlCalls::SqlInsert(h,"e_s_caja",Configuracion_global->empresaDB,error);
    if(new_id <0)
        QMessageBox::warning(this,tr("Gestión de tickets"),tr("se produjo un error al insertar: %1").arg(error));
    else
        TimedMessageBox *t = new TimedMessageBox(this,tr("Ya puede retirar el dinero,\n  Proceso registrado"));
    accept();


}
