#include "editeventform.h"
#include "ui_editeventform.h"
#include "../Busquedas/frmbuscarcliente.h"
#include "./Zona_Pacientes/cliente.h"
EditEventForm::EditEventForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditEventForm)
{
    ui->setupUi(this);
    this->setWindowTitle("Editar evento");

    ui->combo_cliente->setModel(Configuracion_global->client_model);
    ui->combo_cliente->setModelColumn(Configuracion_global->client_model->fieldIndex("cNombreFiscal"));
    if(!medic)
    {
        ui->especialidad_group->hide();
        ui->reunion_group->setTitle("Reunion con:");
    }
    else
    {
        ui->departamento_group->hide();
        ui->reunion_group->setTitle("Cita con:");
    }
}

EditEventForm::~EditEventForm()
{
    delete ui;
}

void EditEventForm::setButtonColor()
{
    int r = event_color.red();
    int g = event_color.green();
    int b = event_color.blue();
    QString style = QString("background-color: rgb(%1,%2,%3)").arg(r).arg(g).arg(b);
    ui->btn_getColor->setStyleSheet(style);
}

void EditEventForm::on_btn_getColor_clicked()
{
    QColorDialog d(this);
    if(d.exec() == QDialog::Accepted)
    {
        event_color = d.currentColor();
        setButtonColor();
    }
}

void EditEventForm::on_btn_cancelar_clicked()
{
    EditEventForm::done(QDialog::Rejected);
}

void EditEventForm::on_btn_guardar_clicked()
{
    asunto = ui->txt_asunto->toPlainText();
    asunto.replace("\n","<br>");
    isCita = ui->reunion_group->isChecked();
    QSqlRelationalTableModel* client_model = (QSqlRelationalTableModel*)ui->combo_cliente->model();
    id_cliente = client_model->record(ui->combo_cliente->currentIndex()).value("Id").toInt();
    id_spec;
    id_depart;
    EditEventForm::done(QDialog::Accepted);
}

void EditEventForm::on_pushButton_clicked()
{
    FrmBuscarCliente BuscarClientes(this);
    if(BuscarClientes.exec() == QDialog::Accepted)
    {
        int nId = BuscarClientes.DevolverID();
        QString cId = QString::number(nId);

        Cliente oCliente;
        oCliente.Recuperar("Select * from clientes where id ="+cId+" order by id limit 1 ");
        QString name = oCliente.cNombreFiscal;
        int i = ui->combo_cliente->findText(name);
        ui->combo_cliente->setCurrentIndex(i);
    }
}
