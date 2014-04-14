#include "frmeditaravisos.h"
#include "ui_frmeditaravisos.h"

FrmEditarAvisos::FrmEditarAvisos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmEditarAvisos)
{
    ui->setupUi(this);
}

FrmEditarAvisos::~FrmEditarAvisos()
{
    delete ui;
}

void FrmEditarAvisos::setValores(int id)
{
    this->id = id;
    QMap <int,QSqlRecord> av;
    QString error;
    av = SqlCalls::SelectRecord("avisos",QString("id = %1").arg(id),Configuracion_global->groupDB,error);
    if(av.count()>0)
    {
        ui->txtAviso->setPlainText(av.value(id).value("aviso").toString());
        ui->dtFechaHora->setDateTime(av.value(id).value("fecha_hora_aviso").toDateTime());
    }

}

void FrmEditarAvisos::on_btnAceptar_clicked()
{
    //TODO - Guardar cambios postergar
}

void FrmEditarAvisos::on_btnCerrar_clicked()
{
    QHash <QString, QVariant> h;
    QString error;
    h["completado"] =true;
    bool success = SqlCalls::SqlUpdate(h,"avisos",Configuracion_global->groupDB,QString("id=%1").arg(this->id),error);
    if(!success)
        QMessageBox::warning(this,tr("Gestión avisos"),tr("Ocurrió un error al marcar el aviso como cerrado."),tr("Aceptar"));
    else
        accept();
}
